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
	const vector<int>& getPart1() const { return this->part1; }
	const vector<int>& getPart2() const { return this->part2; }

	static MS minVar( double L[], int I[], double F[], double sqSumes, double sums, int m );

private:
	MS minVarAndSp( const vector<double> L, const list< pair<int,double> > F, double sqSums, double sums, int nums, const vector<int> cs );
	static double computeVAR( double sqSums, double sums, int nums, double sum1, int num1);

	// res 
	int minFIdx;
	double minSpVal;
	double minVAR;
	int m1;
	int m2;
	vector<int> part1;
	vector<int> part2;
};
