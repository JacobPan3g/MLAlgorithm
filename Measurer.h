/**********************************************************************
	> File Name: Measurer.h
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Tue 23 Jul 2013 12:04:39 PM CST
 *********************************************************************/

#pragma once


#include "TR_Data.h"

struct MS
{
	int fIdx;
	double spVal;
	double VAR;
	int m1;
	int m2;
	MS( int fIdx=-1, double spVal=-1, double VAR=-1, int m1=-1, int m2=-1 ) {
		this->fIdx = fIdx;
		this->spVal = spVal;
		this->VAR = VAR;
		this->m1 = m1;
		this->m2 = m2;
	}
	MS& operator=( const MS& ms ) {
		this->fIdx = ms.fIdx;
		this->spVal = ms.spVal;
		this->VAR = ms.VAR;
		this->m1 = ms.m1;
		this->m2 = ms.m2;
		return *this;
	}
};

/*	Abstract Class Measurer
 *		-- focus on decide the split feature and its observe value
 *	
 *	Type:	Tool Class
 *	Inside:	none
 *	
 *	Capability:
 *	1. measure
 *	2. estimate a label
 *	3. end condition for a tree
 */
class Measurer
{
public:
	virtual MS measure( const TR_Data &D, const vector<int> &cs, const vector<int> &fs ) = 0;
	virtual double estimateLabel( const vector<double> &L, const vector<int> &cs ) = 0;
	virtual bool endCondition( const vector<double> &L, vector<int> cs, int num ) = 0;
};

