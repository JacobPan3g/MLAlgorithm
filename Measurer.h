/**********************************************************************
	> File Name: Measurer.h
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Tue 23 Jul 2013 12:04:39 PM CST
 *********************************************************************/

#pragma once


#include "Data.h"
using namespace std;

class Measurer
{
public:
	virtual vector< vector<double> > measure( const Data &D, const vector<int> &cs, const vector<int> &fs ) = 0;
	virtual double estimateLabel( const vector<double> &L, const vector<int> &cs ) = 0;
	virtual bool endCondition( const vector<double> &L, vector<int> cs, int num ) = 0;
};

