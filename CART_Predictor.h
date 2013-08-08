/**********************************************************************
	> File Name: CART_Predictor.h
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Tue 23 Jul 2013 08:17:25 PM CST
 *********************************************************************/

#pragma once

#include <queue>
#include "Predictor.h"
#include "VAR_Measurer.h"
#include "ST_Model.h"
using namespace std;


struct Node			
{
	int fIdx;
	double obValue;
	Node *left;
	Node *right;
	int high;
	vector<int> cs;		// which cases(rows) will be considered.
	int cnum;			// the num of cases
	int single;			// mark is the single child node (just for debug)
	int lIdx;			// the leaf idx of the tree

	Node( const vector<int> &cs, int cnum, int high, int fIdx=-1, double obValue=-1, Node *l=NULL, Node *r=NULL ) 
	{
		this->fIdx = fIdx;
		this->obValue = obValue;
		this->left = l;
		this->right = r;
		this->high = high;
		this->cs = cs;
		this->cnum = cnum;
		this->single = 0;
		this->lIdx = -1;
	}
};


class CART_Predictor: public Predictor
{

public:
	// virtual method	
	void train( const TR_Data& D, const vector<int>& cs, const vector<int>& fs );
	vector<double> predict( const Model& model, const TR_Data& T );
	void saveModel( const string& fNM ) const;

	// own method
	CART_Predictor( int maxH=10 );
	~CART_Predictor();
	void train( const TR_Data& D );

	vector<double> predict( const Data& T ) const;
	double predict( const vector<double>& a );
	void saveTrees( ofstream &fobj ) const;
	
	// disp
	void dispTree() const;
	void dispLeaves() const;

	// getter
	const Node* getRoot() const;
	int getMAX_HIGH() const;
	int getHigh() const;
	// big data getter
	const vector<double>& getLabels() const;
	const vector<Node*>& getLeaf() const;
	const vector<Node*>& getInNode() const;
	const ST_Model& getModel() const;

private:
	void foundALeaf( Node *node, const vector<double> &L );

	Node *root;
	int MAX_HIGH;
	int high;
	vector<double> labels;
	vector<Node*> leaf;
	vector<Node*> inNode;	// for test

	ST_Model model;

	VAR_Measurer c_msr;		// default use accurater
	//GINI d_msr;
	int sinNodeNum;			// number of single node
};


