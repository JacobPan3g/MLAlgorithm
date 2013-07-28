/*************************************************************************
	> File Name: CART.h
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Tue 23 Jul 2013 08:17:25 PM CST
 ************************************************************************/

#pragma once

#include <queue>
#include "VAR.h"
using namespace std;


class Node			
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
	int lIdx;			// the leaf idx of the tree

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
		this->lIdx = -1;
	}
};


class CART
{
public:
	CART( const CsvData &D, int saNum=-1, int maxH=10 );
	CART( const CsvData &D, const vector<int> &cs, const vector<int> &fs, int saNum=-1, int maxH=10 );
	~CART();
	double predict( const vector<double> &a );
	vector<double> predict( const CsvData &test );
	void saveTree( string filename );
	// disp
	void dispTree();
	void dispLeaves();

	Node *root;
	vector<int> features;	// tag which feature can be considered
	vector<double> labels;
	vector<Node*> leaf;
	vector<Node*> inNode;	// for test
	int MAX_HIGH;
	int high;

private:
	void init( const CsvData &D, const vector<int> &cs, const vector<int> &fs, int spNum, int maxH );
	void bulidTree( const CsvData &D, const vector<int> &row );
	void foundALeaf( Node *node, const vector<double> &L );

	VAR c_msr;				// default use accurater
	//GINI d_msr;
	int sinNodeNum;			// number of single node
};


