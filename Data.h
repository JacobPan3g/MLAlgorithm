/**********************************************************************
	> File Name: Data.h
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Tue 23 Jul 2013 11:40:32 AM CST
 *********************************************************************/

#pragma once


#include "JPTool.cpp"

/*	Class Data
 *		-- focus on Data read/write( or format)
 *	
 *	Type:	Tool Class
 *	Inside:	sort L and A
 *	
 *	Capability:
 *	1. Read from
 *		a. directly
 *		b. csv
 *	2. Write to
 *		a. csv
 */
class Data
{
public:
	Data();		// do nothing

	// read
	void dirread( const vector<double>& L, const vector< vector<double> >& A );
	void csvread( const string& fNM );

	// write
	void csvwrite( const string& fNM );

	vector<double> getFeatures( int fIdx, vector<int> fs=vector<int>() ) const;
	void dispData() const;
	
	// getter
	int getM() const;
	int getN() const;
	// big data getter
	const vector<double>& getL() const;
	const vector< vector<double> >& getA() const;

protected:
	void init();

	int m;
	int n;
	vector<double> L;
	vector< vector<double> > A;
};

