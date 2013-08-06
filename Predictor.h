/**********************************************************************
	> File Name: Predictor.h
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Tue 06 Aug 2013 07:14:04 PM CST
 *********************************************************************/

#pragma once


#include "TR_Data.h"

class Predictor
{
public:
	virtual void train( const TR_Data& D, const vector<int>& cs ) = 0;
	virtual vector<double> predict( const Model& model, const TR_Data& T ) = 0;
	virtual void saveModel( const string& fNM ) const = 0;
};
