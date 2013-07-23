/*************************************************************************
	> File Name: BaggingData.h
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Tue 23 Jul 2013 11:45:36 AM CST
 ************************************************************************/

#pragma once

#include "CsvData.h"
using namespace std;

#define FILENAME "dataset/pro1.csv"

class BaggingData: public CsvData
{
public:
	BaggingData( string filename=FILENAME );
	void reshuffling();

	vector<int> cs;
	vector<int> fs;
	int bm;
	int bn;

protected:
	void get80Labels();
	void get80Features();
	
	vector<int> L_idx;
	vector<int> A_idx;
};

