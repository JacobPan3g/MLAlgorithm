/**********************************************************************
	> File Name: CsvData.h
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Tue 23 Jul 2013 11:40:32 AM CST
 *********************************************************************/

#pragma once

#include "JPTool.cpp"
using namespace std;

class CsvData
{
public:
	CsvData();
	void csvread( string filename );
	vector<double> getFeatures( int fIdx, vector<int> tag=vector<int>() ) const;
	void disp() const;

	int m;
	int n;
	vector<double> L;
	vector< vector<double> > A;
};

