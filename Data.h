/**********************************************************************
	> File Name: CsvData.h
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Tue 23 Jul 2013 11:40:32 AM CST
 *********************************************************************/

#pragma once

#include "JPTool.cpp"
using namespace std;

class Data
{
public:
	Data();
	void csvread( const string& fNM );

	vector<double> getFeatures( int fIdx, vector<int> fs=vector<int>() ) const;
	void disp() const;
	
	// getter
	int getM() const;
	int getN() const;
	// big data getter
	const vector<double>& getL() const;
	const vector< vector<double> >& getA() const;

private:
	int m;
	int n;
	vector<double> L;
	vector< vector<double> > A;
};

