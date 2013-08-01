/**********************************************************************
	> File Name: SingleTree.h
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Thu 01 Aug 2013 04:01:28 PM CST
 *********************************************************************/

#pragma once

#include "Model.h"
using namespace std;

class SingleTree: Model
{
public:
	void load( const CART* pdPtr );
	void save( string fNM );
	
	SingleTree( const CART* pdPtr );	

//private:
	vector<int> fIdxV;
	vector<int> leftV;
	vector<int> rightV;
	vector<double> obValV;
};
