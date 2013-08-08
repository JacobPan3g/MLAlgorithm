/**********************************************************************
	> File Name: SingleTree.h
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Thu 01 Aug 2013 04:01:28 PM CST
 *********************************************************************/

#pragma once

#include "Model.h"
using namespace std;

class CART_Predictor;

class ST_Model: public Model
{
public:
	// Virtual Method
	void load( const string& fNM );
	void save( const string& fNM ) const;
	
	// Own Method
	ST_Model();
	ST_Model( const CART_Predictor* pdPtr );
	ST_Model( const string& fNM );

	// getter
	const vector<int>& getFIdxV();
	const vector<int>& getLeftV();
	const vector<int>& getRightV();
	const vector<double>& getObValV();

private:
	vector<int> fIdxV;
	vector<int> leftV;
	vector<int> rightV;
	vector<double> obValV;
};
