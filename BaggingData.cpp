/*************************************************************************
	> File Name: baggingTree.cpp
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: Tue 16 Jul 2013 07:30:40 PM CST
 ************************************************************************/

#define _BAGGINGDATA_UTEST_


#include "BaggingData.h"
#include "CsvData.cpp"

BaggingData::BaggingData( string filename ): 
	CsvData(filename)
{
	this->L_idx.resize( CsvData::m );
	this->A_idx.resize( CsvData::n );
	this->bm = CsvData::m * 0.8;
	this->bn = CsvData::n * 0.8;
	for ( int i = 0; i < this->L_idx.size(); i++ )
		this->L_idx[i] = i;
	for ( int i = 0; i < this->A_idx.size(); i++ )
		this->A_idx[i] = i;

	this->reshuffling();
}

void BaggingData::reshuffling()
{
	this->get80Labels();
	this->get80Features();
}

void BaggingData::get80Labels()
{
	this->cs.resize( this->m, 0 );
	shuffling( this->L_idx );				// shuffling
	for ( int i = 0; i < this->bm; i++ )	// choose 80%
		this->cs[this->L_idx[i]] = 1;

	assert( cs[L_idx[0]]==1 );
	assert( cs[L_idx[bm-1]]==1 );
	assert( cs[L_idx[rand()%bm]]==1 );
}

void BaggingData::get80Features()
{
	this->fs.resize( this->n, 0 );
	shuffling( this->A_idx );
	for ( int i = 0; i < this->bn; i++ )
		this->fs[this->A_idx[i]] = 1;

	assert( fs[A_idx[0]]==1 );
	assert( fs[A_idx[bn-1]]==1 );
	assert( fs[A_idx[rand()%bn]]==1 );
}


#ifdef _BAGGINGDATA_UTEST_

int main()
{
	BaggingData D( "dataset/pro1.csv" );
	
	// Test Size
	assert( D.m==9126&&D.n==46 );
	assert( D.bm==7300&&D.bn==36 );

	// Test reshuffling()
	vector<int> old_cs = D.cs;
	vector<int> old_fs = D.fs;
	D.reshuffling();
	assert( !isSame( old_cs, D.cs ) );
	assert( !isSame( old_fs, D.fs ) );

	cout << "All Test Cases Passed." << endl;
	return 0;
}

#endif
