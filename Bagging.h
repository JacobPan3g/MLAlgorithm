/**********************************************************************
	> File Name: Bagging.h
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Thu 01 Aug 2013 12:57:57 PM CST
 *********************************************************************/

#pragma once


#include "BaggingData.h"
using namespace std;

class Bagging
{
public:
	Bagging( int maxH, int bagNum );
	~Bagging();
	void train( const BaggingData& D );
	vector<double> predict( const Model& model, const BaggingData& T );
	void saveModel( string filename ) const;

	int getMaxH() const;
	int getBagNum() const;
	vector<CART*> getBaseV() const;

private:
	int maxH;
	int bagNum;
	vector<CART*> baserV; 
};
