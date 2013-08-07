/**********************************************************************
	> File Name: SingleTree.h
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Thu 01 Aug 2013 04:01:28 PM CST
 *********************************************************************/

#pragma once

#include "Model.h"
using namespace std;

class ST_Model: public Model
{
public:
	void load( const CART_Predictor* pdPtr );
	void save( string fNM );
	
	SingleTree( const CART_Predictor* pdPtr );	

//private:
	vector<int> fIdxV;
	vector<int> leftV;
	vector<int> rightV;
	vector<double> obValV;
};
