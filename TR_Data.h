/**********************************************************************
	> File Name: TR_Data.h
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Tue 06 Aug 2013 11:00:06 AM CST
 *********************************************************************/

#pragma once


#include "Data.h"

/*	Class Data
 *		-- focus on Data read/write( or format)
 *	
 *	Type:	Tool Class
 *	Inside:	sort L, A, fmtV
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
class TR_Data: public Data
{
public:
	TR_Data();		// do nothing
	void dispFmtV( int f=0, int r=-1 ) const;

	// read
	void dirread( const vector<double>& L, const vector< vector<double> >& A );
	void csvread( const string& fNM );
	void fmtread( const string& fNM );

	// write
	void fmtwrite( const string& fNM ); 

	// big data getter
	const vector< list< pair<int,double> > > getFmtV() const;
	void setL( const vector<double> &L ) { this->L = L; }

protected:
	void init();
	void toFmt();

	vector< list< pair<int,double> > > fmtV;
};

