/**********************************************************************
	> File Name: SingleTree.h
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Thu 01 Aug 2013 04:01:28 PM CST
 *********************************************************************/

#pragma once


#include "Model.h"
#include "Node.h"

class ST_Model: public Model
{
public:
	// Virtual Method
	void load( const string& fNM );
	void save( const string& fNM ) const;
	
	// Own Method
	ST_Model();
	//ST_Model( const CART_Predictor* pdPtr );
	ST_Model( const Node *root, const vector<double> labels );
	ST_Model( const string& fNM );
	ST_Model( ifstream &fin );
	ST_Model( const ST_Model &mdl );
	void load( ifstream &fin );
	void save( ofstream &fout ) const;
	void show() const;

	// getter
	const vector<int>& getFIdxV() const;
	const vector<int>& getLeftV() const;
	const vector<int>& getRightV() const;
	const vector<double>& getObValV() const;

	bool operator==( const ST_Model& mdl );
private:
	vector<int> fIdxV;
	vector<int> leftV;
	vector<int> rightV;
	vector<double> obValV;
};
