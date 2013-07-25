/*************************************************************************
	> File Name: tree.cpp
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: Sun 07 Jul 2013 10:57:34 AM CST
 ************************************************************************/

#define _CART_UTEST_


#include "CART.h"
#include "VAR.cpp"

CART::CART( const CsvData &D, int maxH )
{
	vector<int> cs(D.m, 1);
	vector<int> fs(D.n, 1);
	this->init( D, cs, fs, maxH );
}

CART::CART( const CsvData &D, const vector<int> &cs, const vector<int> &fs, int maxH )
{
	this->init( D, cs, fs, maxH );
}

CART::~CART()
{
	queue<Node*> q;
	q.push( this->root );
	while( !q.empty() )
	{
		Node *node = q.front();
		q.pop();
		if ( node->left != NULL )
			q.push( node->left );
		if ( node->right != NULL )
			q.push( node->right );

		delete node;
	}
}

void CART::init( const CsvData &D, const vector<int> &cs, const vector<int> &fs, int maxH )
{
	this->MAX_HIGH = maxH;
	this->high = 0;
	this->features = fs;
	this->bulidTree( D, cs );
}

void CART::bulidTree( const CsvData &D, const vector<int> &rows )
{
	queue<Node*> q;
	this->sinNodeNum = 0;
	
	this->root = new Node( rows, D.m, 0 );
	q.push( this->root );
	while ( !q.empty() )
	{
		Node *node = q.front();
		q.pop();
		// end 1
		if ( node->high >= MAX_HIGH )
		{
			this->foundALeaf( node, D.L ); 	// a leaf needn't find minF
			continue;
		}
		// end 2
		/// is all features used
		if ( isAll( this->features, 0 ) )
		{
			this->foundALeaf( node, D.L ); 	// a leaf needn't find minF
			continue;
		}
		// end 3
		/// the measure's end condition
		if ( c_msr.endCondition( D.L, node->cases, node->cnum ) )
		{
			this->foundALeaf( node, D.L ); 	// a leaf needn't find minF
			continue;
		}
		
		// handle the inner node
		inNode.push_back( node );

		// get the optimal feature
		vector< vector<double> > ms = c_msr.measure( D, node->cases, this->features );
		vector<double> tmp = min( ms, this->features );
		double minMS = tmp[0];
		int minF = (int)tmp[1];		// ms start at first feature
		double obValue = c_msr.getSpByValueIdx( (int)tmp[2] );
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
		
		// handle the single node
		if ( tmpN1 == 0 || tmpN2 == 0 )
		{
			node->single = 1;
			this->sinNodeNum++;

			/*node->fIdx = -1;
			this->features[minF] = 1;
			q.push( node );*/
		}
		//else{

		int h = node->high + 1;
		if ( tmpN1 != 0 )
		{
			node->left = new Node( rows1, tmpN1, h );
			q.push(node->left);
		}
		if ( tmpN2 != 0 )
		{
			node->right = new Node( rows2, tmpN2, h );
			q.push(node->right);
		}
		//}

	}

	assert( this->high <= MAX_HIGH );
	assert( root!=NULL );
	assert( leaf.size()!=0 );
	assert( leaf.size()==labels.size() );
	assert( count(this->features)+inNode.size()==D.n );
	assert( high <= (int)log2(D.n)+1 );
	assert( inNode.size()-sinNodeNum+1==leaf.size() );	//(m-1)i+1=t
}

void CART::foundALeaf( Node *node, const vector<double> &L )
{
	node->lIdx = this->leaf.size();
	this->high = node->high > this->high? node->high : this->high;
	this->leaf.push_back( node );
	this->labels.push_back( c_msr.estimateLabel( L, node->cases ) );
}

void CART::dispTree()
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


void CART::dispLeaves()
{
	cout << "lIdx\tcnum\thigh\tlabel\t" << endl;
	for ( int i = 0; i < this->leaf.size(); i++ )
		cout << i << "\t"
			 << this->leaf[i]->cnum << "\t" 
			 << this->leaf[i]->high << "\t" 
			 << this->labels[i] << endl;
	cout << "Total: " << this->leaf.size() << endl;

}

double CART::predict( const vector<double> &a )
{

	Node *node = this->root;
	while( node->fIdx != -1 )
	{

		if ( a[node->fIdx] <= node->obValue )
			node = node->left;
		else
			node = node->right;
	}
	return this->labels[node->lIdx];
}

vector<double> CART::predict( const CsvData &test )
{
	vector<double> res( test.m );
	for ( int i = 0; i < test.m; i++ )
		res[i] = this->predict( test.A[i] );
	return res;
}

void CART::saveTree( string filename )
{
	queue<Node*> q;
	vector<int> fIdxV;
	vector<int> leftV;
	vector<int> rightV;
	vector<double> oValV;

	fIdxV.push_back( this->root->fIdx );
	q.push( this->root );
	
	while ( !q.empty() )
	{
		Node *node = q.front();
		q.pop();

		if ( node->fIdx != -1 )
		{
			oValV.push_back( node->obValue );

			if ( node->left != NULL )
			{
				q.push( node->left );
				leftV.push_back( fIdxV.size() );
				fIdxV.push_back( node->left->fIdx );
			}
			else
				leftV.push_back( -1 );

			if ( node->right != NULL )
			{
				q.push( node->right );
				rightV.push_back( fIdxV.size() );
				fIdxV.push_back( node->right->fIdx );
			}
			else
				rightV.push_back( -1 );
		}
		else
		{
			oValV.push_back( this->labels[node->lIdx] );	// save the labels
			leftV.push_back( -1 );
			rightV.push_back( -1 );
		}
	}
/*	disp( fIdxV );
	disp( oValV );
	disp( leftV );
	disp( rightV );
	cout << fIdxV.size() << endl << oValV.size() << endl << leftV.size() << endl << rightV.size() << endl;
*/
	// save
	ofstream obj( filename.c_str(), ofstream::app );
	save( obj, fIdxV, "," );
	save( obj, leftV, "," );
	save( obj, rightV, ",");
	save( obj, oValV, "," );
	obj.close();
}


#ifdef _CART_UTEST_

int main()
{
	time_t tic, toc;
	tic = clock();

	CsvData D;

	// Unit Test 1
	D.csvread( "test/case1.csv" );
	CART c1T( D );
	c1T.dispTree();

	vector<int> tag(D.n, 1);
	tag[0] = 0;

	assert( isAll(c1T.features, 1, tag) );
	assert( c1T.inNode.size()==1 );
	assert( c1T.inNode[0]->fIdx==0 );
	assert( c1T.inNode[0]->obValue==0.5 );
	assert( c1T.high==1 );
	assert( c1T.leaf.size()==2 );
	assert( c1T.labels[0]==1&&c1T.labels[1]==2);

	// Live Test
	D.csvread("dataset/pro1.csv");
	CART tree(D, 10);

	tree.dispTree();
	//tree.dispLeaves();
	//cout << tree.predict( D.A[18] ) << endl;
	
	tree.saveTree( "trees/UT_20" );
	//cout << tree.inNode[17]->obValue << endl;
	
	assert( isAll(tree.features,0) );
	//assert(  )

	toc = clock();
	double tol = (double)(toc-tic)/CLOCKS_PER_SEC;
	cout << "Time: " << tol << " s" << endl;
	return 0;
}

#endif
