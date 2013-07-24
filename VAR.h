/*************************************************************************
	> File Name: VAR.h
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Tue 23 Jul 2013 12:29:18 PM CST
 ************************************************************************/

#pragma once

#include "Measurer.h"
using namespace std;

class VAR: public Measurer
{	
public:
	// implement virtual methods
	vector< vector<double> > measure( const CsvData &D, const vector<int> &r, const vector<int> &c );
	double estimateLabel( const vector<double> &L, const vector<int> &tag );
	bool endCondition( const vector<double> v, vector<int> tag, int num );

	// own methods
	double getSpByValueIdx( int x );

private:
	vector<double> getSplitPoints( const vector<double> &v );

};
