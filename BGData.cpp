/**********************************************************************
	> File Name: BGData.cpp
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Tue 16 Jul 2013 07:30:40 PM CST
 *********************************************************************/

#define _BGDATA_UTEST_


#include "BGData.h"
#include "Data.cpp"

BGData::BGData()
{
	this->bm = 0;
	this->bn = 0;
}

void BGData::csvread( const string& fNM )
{
	Data::csvread( fNM );
	this->L_idx.resize( Data::m );
	this->A_idx.resize( Data::n );
	this->bm = Data::m * 0.8;
	this->bn = Data::n * 0.8;
	for ( int i = 0; i < this->L_idx.size(); i++ )
		this->L_idx[i] = i;
	for ( int i = 0; i < this->A_idx.size(); i++ )
		this->A_idx[i] = i;

	this->reshuffling();
}

void BGData::reshuffling()
{
	this->get80Labels();
	this->get80Features();
}

// getter
vector<int> BGData::getCs() const
{
	return this->cs;
}

vector<int> BGData::getFs() const
{
	return this->fs;
}

vector<int> BGData::getL_idx() const
{
	return this->L_idx;
}

vector<int> BGData::getA_idx() const
{
	return this->A_idx;
}

int BGData::getM() const
{
	return this->bm;
}

int BGData::getN() const
{
	return this->bn;
}

// Private
void BGData::get80Labels()
{
	this->cs = vector<int>( this->m, 0 );
	shuffling( this->L_idx );				// shuffling
	for ( int i = 0; i < this->bm; i++ )	// choose 80%
		this->cs[this->L_idx[i]] = 1;

	assert( cs[L_idx[0]]==1 );
	assert( cs[L_idx[bm-1]]==1 );
	assert( cs[L_idx[rand()%bm]]==1 );
}

void BGData::get80Features()
{
	this->fs = vector<int>( this->n, 0 );
	shuffling( this->A_idx );
	for ( int i = 0; i < this->bn; i++ )
		this->fs[this->A_idx[i]] = 1;

	assert( fs[A_idx[0]]==1 );
	assert( fs[A_idx[bn-1]]==1 );
	assert( fs[A_idx[rand()%bn]]==1 );
}


#ifdef _BGDATA_UTEST_

void test1()
{
	BGData D;
	D.csvread( "test/case1.csv" );
	
	vector<int> old_cs = D.getCs();
	vector<int> old_fs = D.getFs();
	D.reshuffling();
	assert( !isSame(old_cs,D.getCs()) );
	assert( !isSame(old_fs,D.getFs()) );
}

void liveTest()
{
	BGData D;
	D.csvread( "dataset/pro1.csv" );
	
	// Test Size
	assert( D.Data::getM()==9126&&D.Data::getN()==46 );
	assert( D.getM()==7300&&D.getN()==36 );

	// Test reshuffling()
	vector<int> old_cs = D.getCs();
	vector<int> old_fs = D.getFs();
	D.reshuffling();
	assert( !isSame(old_cs,D.getCs()) );
	assert( !isSame(old_fs,D.getFs()) );
}

int main()
{
	test1();	// done

	liveTest();
	
	cout << "All Test Cases Passed." << endl;
	return 0;
}

#endif
