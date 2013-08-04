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
 *		c. format( VAR_Measurer )
 *	2. Write to
 *		a. csv
 *		b. format( VAR_Measurer )
 */
class Data
{
public:
	Data();		// do nothing

	// read
	void dirread( const vector<double>& L, const vector< vector<double> >& A );
	void csvread( const string& fNM );
	void fmtread( const string& fNM );

	// write
	void csvwrite( const string& fNM );
	void fmtwrite( const string& fNM ); 

	vector<double> getFeatures( int fIdx, vector<int> fs=vector<int>() ) const;
	void dispData() const;
	
	// getter
	int getM() const;
	int getN() const;
	// big data getter
	const vector<double>& getL() const;
	const vector< vector<double> >& getA() const;
	const vector< list< pair<int,double> > > getFmtV() const;

protected:
	void init();
	void toFmt();

	int m;
	int n;
	vector<double> L;
	vector< vector<double> > A;
	// Just for Tree algorthm
	vector< list< pair<int,double> > > fmtV;
};

