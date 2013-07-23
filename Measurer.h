/*************************************************************************
	> File Name: Measurer.h
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Tue 23 Jul 2013 12:04:39 PM CST
 ************************************************************************/

#pragma once

#include "CsvData.h"
using namespace std;

class Measurer
{
public:
	virtual vector< vector<double> > measure( const CsvData &D, const vector<int> &r, const vector<int> &c ) = 0;
	virtual double estimateLabel( const vector<double> &L, const vector<int> &tag ) = 0;
	virtual bool endCondition( const vector<double> v, vector<int> tag, int num ) = 0;

};

