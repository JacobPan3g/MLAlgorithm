/**********************************************************************
	> File Name: Measurer.h
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Tue 23 Jul 2013 12:04:39 PM CST
 *********************************************************************/

#pragma once


#include "TR_Data.h"
using namespace std;

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
	virtual pair<int,double> measure( const TR_Data &D, const vector<int> &cs, const vector<int> &fs ) = 0;
	virtual double estimateLabel( const vector<double> &L, const vector<int> &cs ) = 0;
	virtual bool endCondition( const vector<double> &L, vector<int> cs, int num ) = 0;
};

