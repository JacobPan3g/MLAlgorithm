/*************************************************************************
	> File Name: tree.cpp
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: Sun 07 Jul 2013 10:57:34 AM CST
 ************************************************************************/

#pragma once

#include <queue>
#include <cmath>
#include "SS.cpp"
//#include "GINI.cpp"
using namespace std;
#define MAX_HIGH 10

class Node			// be a class cause it have to free some spaces
{
public:
	int fIdx;
	double obValue;
	Node *left;
	Node *right;
	int high;
	vector<int> cases;	// which cases(rows) will be considered.
	int cnum;			// the num of cases
	int single;			// mark is the single child node (just for debug)
	Node( const vector<int> &cases, int cnum, int high, int fIdx=-1, double obValue=-1, Node *l=NULL, Node *r=NULL ) 
	{
		this->fIdx = fIdx;
		this->obValue = obValue;
		this->left = l;
		this->right = r;
		this->high = high;
		this->cases = cases;
		this->cnum = cnum;
		this->single = 0;
	}
};


class BinaryTree
{
public:
	BinaryTree( const CsvData &D );
	void disp();
	Node *root;
	vector<int> features;	// tag which feature can be considered
	vector<Node*> leaf;
	vector<double> labels;
private:
	void bulidTree( const CsvData &D );	
};

BinaryTree::BinaryTree( const CsvData &D )
{
	this->features = vector<int>(D.n, 1);
	this->bulidTree( D );
}

void BinaryTree::bulidTree( const CsvData &D )
{
	queue<Node*> q;
	vector<int> rows( D.m, 1 );
	this->root = new Node( rows, D.m, 0 );
	q.push( this->root );
	while ( !q.empty() )
	{
		Node *node = q.front();
		q.pop();
		// end 1
		if ( node->high >= MAX_HIGH )
		{
			this->leaf.push_back( node );	// a leaf needn't find minF
			this->labels.push_back( estimateLabel( D.L, node->cases ) );
			continue;
		}
		// end 2
		/// is all features used
		if ( isAll( this->features, 0 ) )
		{
			this->leaf.push_back( node );
			this->labels.push_back( estimateLabel( D.L, node->cases ) );
			continue;
		}
		// end 3
		/// the measure's end condition
		if ( endCondition( D.L, node->cases, node->cnum ) )
		{
			node->fIdx = -1;
			this->leaf.push_back( node );
			this->labels.push_back( estimateLabel( D.L, node->cases ) );
			continue;
		}

		// get the optimal feature
		vector< vector<double> > ms = measure( D, node->cases, node->cnum, this->features );
		vector<double> tmp = min( ms, this->features );
		double minMS = tmp[0];
		int minF = (int)tmp[1];		// ms start at first feature
		double obValue = getSpByValueIdx( (int)tmp[2] );
		/// tag and asign
		this->features[minF] = 0;
		node->fIdx = minF;
		node->obValue = obValue;
		// separate the data by minF( need to know the minF )
		vector<int> rows1( D.m, 0 );
		vector<int> rows2( D.m, 0 );
		int tmpN1 = 0;
		int tmpN2 = 0;
		for ( int i = 0; i < D.m; i++ )
		{
			if ( !node->cases[i] )
				continue;
			if ( D.A[i][node->fIdx] <= obValue )
			{
				tmpN1++;
				rows1[i] = 1;
			}
			else
			{
				tmpN2++;
				rows2[i] = 1;
			}
		}
		
		/*
		// it's not good for debugging
		if ( tmpN1 == 0 || tmpN2 == 0 )
		{
			// retry. **dead-loop	i.e. ignore one node
			//this->features[minF] = 1;
			node->fIdx = -1;
			q.push( node );
			continue;
		}*/
		
		int high = node->high + 1;
		if ( tmpN1 != 0 )
		{
			node->left = new Node( rows1, tmpN1, high );
			q.push(node->left);
		}
		if ( tmpN2 != 0 )
		{
			node->right = new Node( rows2, tmpN2, high );
			q.push(node->right);
		}

		if ( tmpN1 == 0 || tmpN2 == 0 )
			node->single = 1;
	}
}

void BinaryTree::disp()
{
	queue<Node*> q;
	q.push(this->root);
	int high = 0;
	while( !q.empty() )
	{
		Node *node = q.front();
		q.pop();
		
		if ( node->high > high )
		{
			cout << endl;
			high++;
		}
		cout << node->fIdx;
		if ( node->single == 1 )
			cout << "*";
		cout << " ";

		if ( node->left )
			q.push( node->left );
		if ( node->right )
			q.push( node->right );
	}
	cout << endl;
}

/*
int main()
{
	time_t tic, toc;
	tic = clock();
	CsvData D = CsvData("dataset/pro1.csv");

	BinaryTree tree(D);
	//cout << tree.leaf.size() << endl;
	tree.disp();
	cout << "cnum\thigh\tlabel\t" << endl;
	for ( int i = 0; i < tree.leaf.size(); i++ )
		cout << tree.leaf[i]->cnum << "\t" 
			 << tree.leaf[i]->high << "\t" 
			 << tree.labels[i] << endl;
	cout << tree.leaf.size() << endl;
	
	toc = clock();
	double tol = (double)(toc-tic)/CLOCKS_PER_SEC;
	cout << "Time: " << tol << " s" << endl;
	return 0;
}*/
