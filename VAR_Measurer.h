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
	vector< vector<double> > measure( const Data &D, const vector<int> &cs, const vector<int> &fs );
	double estimateLabel( const vector<double> &L, const vector<int> &cs );
	bool endCondition( const vector<double> &L, vector<int> cs, int num );

	// own methods
	VAR_Measurer();					// constructor for accurater
	VAR_Measurer( int saNum );		// constructor for averager
	double getSpByValueIdx( int idx1, int idx2 );

	// getter
	int getSPLIT_AREA_NUM() const;
	vector< vector<double> > getSp() const;

private:
	vector<double> getSplitPoints( const vector<double> &f, vector<int> cs=vector<int>() );
	vector<double> getSplitPoints( const list< pair<int,double> >& f, vector<int> cs );

	int SPLIT_AREA_NUM;			// also a tag for averager
	vector< vector<double> > sp;// will update every loop
};
