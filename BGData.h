/**********************************************************************
	> File Name: BGData.h
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Tue 23 Jul 2013 11:45:36 AM CST
 *********************************************************************/

#pragma once


#include "Data.h"

class BGData: public Data
{
public:
	BGData();
	void csvread( const string& fNM );
	void reshuffling();

	// getter
	vector<int> getCs() const;
	vector<int> getFs() const;
	vector<int> getL_idx() const;
	vector<int> getA_idx() const;
	int getM() const;
	int getN() const;

protected:
	void get80Labels();
	void get80Features();
	
	vector<int> cs;
	vector<int> fs;
	int bm;
	int bn;
	vector<int> L_idx;
	vector<int> A_idx;
};

