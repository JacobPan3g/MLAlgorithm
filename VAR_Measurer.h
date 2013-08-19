/**********************************************************************
	> File Name: VAR_Measurer.h
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Tue 23 Jul 2013 12:29:18 PM CST
 *********************************************************************/

#pragma once


#include "Measurer.h"
using namespace std;

class VAR_Measurer: public Measurer
{	
public:
	// implement virtual methods
	MS measure( const TR_Data &D, const vector<int> &cs, const vector<int> &fs );
	double estimateLabel( const vector<double> &L, const vector<int> &cs );
	bool endCondition( const vector<double> &L, vector<int> cs, int num );

	// own methods
	VAR_Measurer();					// constructor for accurater

	// getter
	vector< vector<double> > getSp() const;
	vector< vector<int> > getIdxs() const;
	vector< vector<double> > getVars() const;
	// single getter
	const vector<int>& getPart1( int i=-1, int k=-1 ) const;
	const vector<int>& getPart2( int i=-1, int k=-1 ) const;
	int getNum1( int i=-1, int k=-1 ) const;
	int getNum2( int i=-1, int k=-1 ) const;

private:
	void getSplitPoints( const vector< list< pair<int,double> > >& fmtV, const vector<double>& L, int m, int n, vector<int> cs );
	double computeVAR( int i, int k, const list< pair<int,double> >& fmt, const vector<double>& L, int m, int n, vector<int> cs );

	// will update every loop
	vector< vector<double> > sp;	// just use to test
	vector< vector<int> > idxs;		// record num for each sp
	vector< vector<double> > vars;	// for variance calculate
	
	vector< vector< vector<int> > > part1s;
	vector< vector< vector<int> > > part2s;
	vector< vector<int> > num1s;
	vector<int> nums;

	vector< vector<double> > sums1;
	vector< vector<double> > sqSums1;
	vector<double> sums;
	vector<double> sqSums;

	// res idx
	int fIdx;
	int spIdx;
};
