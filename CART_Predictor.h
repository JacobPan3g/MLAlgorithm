/**********************************************************************
	> File Name: CART_Predictor.h
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Tue 23 Jul 2013 08:17:25 PM CST
 *********************************************************************/

#pragma once

//#include "Predictor.h"
#include "VAR_Measurer.h"
#include "ST_Model.h"


class CART_Predictor//: public Predictor
{

public:
	// virtual method	

	// own method
	CART_Predictor( int maxH=10 );
	~CART_Predictor();
	void saveModel( const string& fNM ) const;
	void saveTrees( ofstream &fobj ) const;
	
	void train( const TR_Data& D );
	void train( const TR_Data& D, const vector<int>& cs, const vector<int>& fs );

	vector<double> predict( const Data& T ) const;
	double predict( const vector<double>& a ) const;
	
	// use in predict part
	vector<double> predict( const ST_Model& mdl, const Data& T ) const;
	double predict( const ST_Model& mdl, const vector<double>& a ) const;
	
	// disp
	void dispModel() const;
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


