/**********************************************************************
	> File Name: BG_Data.cpp
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Tue 16 Jul 2013 07:30:40 PM CST
 *********************************************************************/

#define _BG_DATA_UTEST_
#define _BG_DATA_IN_IN_CHECK_


#include "BG_Data.h"
#include "TR_Data.cpp"

BG_Data::BG_Data()
{
	this->bm = 0;
	this->bn = 0;
}

void BG_Data::csvread( const string& fNM )
{
	TR_Data::csvread( fNM );
	this->prepare4Shuffling();
}

void BG_Data::fmtread( const string& fNM )
{
	TR_Data::fmtread( fNM );
	this->prepare4Shuffling();
}

void BG_Data::reshuffling()
{
	this->get80Labels();
	this->get80Features();
}

// getter
vector<int> BG_Data::getCs() const
{
	return this->cs;
}

vector<int> BG_Data::getFs() const
{
	return this->fs;
}

vector<int> BG_Data::getL_idx() const
{
	return this->L_idx;
}

vector<int> BG_Data::getA_idx() const
{
	return this->A_idx;
}

int BG_Data::getM() const
{
	return this->bm;
}

int BG_Data::getN() const
{
	return this->bn;
}

// Private
void BG_Data::prepare4Shuffling()
{
	this->L_idx.resize( TR_Data::m );
	this->A_idx.resize( TR_Data::n );
	this->bm = TR_Data::m * 0.8;
	this->bn = TR_Data::n * 0.8;
	for ( int i = 0; i < this->L_idx.size(); i++ )
		this->L_idx[i] = i;
	for ( int i = 0; i < this->A_idx.size(); i++ )
		this->A_idx[i] = i;

	this->cs = vector<int>( this->bm, 1 );
	this->fs = vector<int>( this->bn, 1 );
	this->cs.resize( this->m, 0 );
	this->fs.resize( this->n, 0 );

	assert( countTag(cs)==this->bm );
	assert( countTag(fs)==this->bn );
}

void BG_Data::get80Labels()
{
	this->cs = vector<int>( this->m, 0 );
	shuffling( this->L_idx );				// shuffling
	for ( int i = 0; i < this->bm; i++ )	// choose 80%
		this->cs[this->L_idx[i]] = 1;

	assert( cs[L_idx[0]]==1 );
	assert( cs[L_idx[bm-1]]==1 );
	assert( cs[L_idx[rand()%bm]]==1 );
}

void BG_Data::get80Features()
{
	this->fs = vector<int>( this->n, 0 );
	shuffling( this->A_idx );
	for ( int i = 0; i < this->bn; i++ )
		this->fs[this->A_idx[i]] = 1;

	assert( fs[A_idx[0]]==1 );
	assert( fs[A_idx[bn-1]]==1 );
	assert( fs[A_idx[rand()%bn]]==1 );
}


/*********************************************************************
 * Unit Test for SubClass
 * by Jacob Pan
 *********************************************************************/

#ifdef _BG_DATA_IN_IN_CHECK_

void BG_Data_case1_check( BG_Data& D )	// shuffling() need non-const
{

#define _DATA_IN_CASE_1_1_
#define _DATA_IN_CASE_1_2_

#ifdef _DATA_IN_CASE_1_1_
/* Case 1.1
 * Goal: 1. regression tree for super test
 */
	TR_Data_case1_check( D );
#endif

#ifdef _DATA_IN_CASE_1_2_
/* Case 1.2
 * Coal: 1. test shuffling 
 */
	vector<int> old_cs = D.getCs();
	vector<int> old_fs = D.getFs();
	D.reshuffling();
	assert( !isSame(old_cs,D.getCs()) );
	assert( !isSame(old_fs,D.getFs()) );
#endif
}

void BG_Data_case2_check( BG_Data &D )
{
	
#define _DATA_IN_CASE_2_1_
#define _DATA_IN_CASE_2_2_

#ifdef _DATA_IN_CASE_2_1_
/* Case 2.1
 * Goal: 1. regression tree for super test
 */
	TR_Data_case2_check( D );
#endif

#ifdef _DATA_IN_CASE_2_2_
/* Case 2.2
 * Coal: 1. test shuffling 
 */
	vector<int> old_cs = D.getCs();
	vector<int> old_fs = D.getFs();
	D.reshuffling();
	assert( !isSame(old_cs,D.getCs()) );
	assert( !isSame(old_fs,D.getFs()) );
#endif
}

void BG_Data_pro1_check( BG_Data &D )
{
	
#define _DATA_IN_PRO_1_1_
#define _DATA_IN_PRO_1_2_

#ifdef _DATA_IN_PRO_1_1_
/* pro1 test 1
 * Goal: 1. regression tree for super test
 */
	TR_Data_pro1_check( D );
#endif

#ifdef _DATA_IN_PRO_1_2_
/* pro1 test 2
 * Coal: 1. test shuffling 
 */
	// Test Size
	assert( D.Data::getM()==9126&&D.Data::getN()==46 );
	assert( D.getM()==7300&&D.getN()==36 );

	// Test reshuffling()
	vector<int> old_cs = D.getCs();
	vector<int> old_fs = D.getFs();
	D.reshuffling();
	assert( !isSame(old_cs,D.getCs()) );
	assert( !isSame(old_fs,D.getFs()) );
#endif
}


#endif

/*********************************************************************
 * Unit Test
 * by Jacob Pan
 *********************************************************************/

#ifdef _BG_DATA_UTEST_

void test1()
{
	BG_Data D4csv;
	D4csv.csvread( "test/case1.csv" );
	BG_Data_case1_check( D4csv );	

	BG_Data D;
	D.fmtread( "test/case1.fmt" );
	BG_Data_case1_check( D );	
}

void test2()
{
	BG_Data D4csv;
	D4csv.csvread( "test/case2.csv" );
	BG_Data_case2_check( D4csv );	

	BG_Data D;
	D.fmtread( "test/case2.fmt" );
	BG_Data_case2_check( D );	
}

void pro1Test()
{
	BG_Data D4csv;
	D4csv.csvread( "dataset/pro1.csv" );
	BG_Data_pro1_check( D4csv );	

	BG_Data D;
	D.fmtread( "dataset/pro1.fmt" );
	BG_Data_pro1_check( D );	
}

int main()
{
	test1();	// done
	test2();	// done

	pro1Test();
	
	cout << "All Test Cases Passed." << endl;
	return 0;
}

#endif
