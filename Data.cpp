/**********************************************************************
	> File Name: Data.cpp
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Mon 08 Jul 2013 09:14:40 AM CST
 *********************************************************************/

//#define _DATA_UTEST_
#define _DATA_IN_CHECK_

#ifndef _DATA_UTEST_
#pragma once
#endif


#include "Data.h"

Data::Data()
{
	this->init();
}

void Data::dirread( const vector<double>& L, const vector< vector<double> >& A )
{
	this->init();
	this->L = L;
	this->A = A;

	this->m = A.size();
	this->n = A[0].size();
}

/*
 * csvread()
 * mainly in 3 parts
 * 1. read by lind
 * 2. split by ','
 * 3. transform char[] to num
 */
void Data::csvread( const string& fNM )
{
	this->init();

	string line;
	ifstream fobj( fNM.c_str() );

	while ( getline( fobj, line ) )		//* the main cmd to read line
	{
		vector<double> tmp = split<double>( line, "," );
		this->L.push_back( tmp[0] );
		tmp.erase( tmp.begin() );
		this->A.push_back( tmp );
	}
	fobj.close();	
	
	this->m = A.size();
	this->n = A[0].size();
}

void Data::csvwrite( const string& fNM )
{}

vector<double> Data::getFeatures( int fIdx, vector<int> fs ) const
{
	if ( fs.size() == 0 )
		fs = vector<int>( this->m, 1 );

	vector<double> res;
	for ( int i = 0; i < this->m; i++ )
		if ( fs[i] == 1 )
			res.push_back( this->A[i][fIdx] );
	return res;
}

void Data::dispData() const
{
	for ( int i = 0; i < this->m; i++ )
	{
		cout << this->L[i] << "\t";
		for ( int j = 0; j < this->n; j++ )
			cout << this->A[i][j] << " ";
		cout << endl;
	}
}

// getter
int Data::getM() const
{
	return this->m;
}

int Data::getN() const
{
	return this->n;
}

const vector<double>& Data::getL() const
{
	return this->L;
}

const vector< vector<double> >& Data::getA() const
{
	return this->A;
}

// Private Methods
void Data::init()
{
	this->m = 0;
	this->n = 0;
	this->L.clear();
	this->A.clear();
}


/*********************************************************************
 * Unit Test for SubClass
 * by Jacob Pan
 *********************************************************************/

#ifdef _DATA_IN_CHECK_

void Data_case1_check( const Data& D )
{

#define _DATA_IN_CASE_1_1_
#define _DATA_IN_CASE_1_2_

#ifdef _DATA_IN_CASE_1_1_
/* Case 1.1
 * Goal: 1. test read data from csv
 */
	double l[5] = { 1, 1, 1, 2, 2 };
	double a[5][4] = {	0,   0,   0.5, 0.4,
						0,   0,   0.5, 0.5,
						0.5, 0,   0.5, 0,
						0.7, 0.5, 0.5, 0.8,
						0.9, 0.5, 0.5, 0.9	};
	vector<double> L(5);
	vector< vector<double> > A(5);
	for ( int i = 0; i < 5; i++ )
	{
		L[i] = l[i];
		A[i].resize( 4 );
	}
	for( int i = 0; i < 5; i++ )
		for ( int j = 0; j < 4; j++ )
			A[i][j] = a[i][j];

	assert( D.getM()==5&&D.getN()==4 );
	assert( isSame(D.getL(),L) );
	assert( isSame(D.getA(),A) );
#endif

#ifdef _DATA_IN_CASE_1_2_
/* Case 1.2
 * Goal: 1. test getFeatures( int )
 */
	double f0[] = {0,   0,   0.5, 0.7, 0.9};
	double f1[] = {0,   0,   0,   0.5, 0.5};
	double f2[] = {0.5, 0.5, 0.5, 0.5, 0.5};
	double f3[] = {0.4, 0.5, 0,   0.8, 0.9};
	
	assert( isSame(D.getFeatures(0),f0,5) );
	assert( isSame(D.getFeatures(1),f1,5) );
	assert( isSame(D.getFeatures(2),f2,5) );
	assert( isSame(D.getFeatures(3),f3,5) );
#endif
}

void Data_case2_check( const Data& D )
{

#define _DATA_IN_CASE_2_1_
#define _DATA_IN_CASE_2_2_

#ifdef _DATA_IN_CASE_2_1_
/* Case 2.1
 * Goal: 1. test read data from csv
 */
	vector<double> L = D.getL();
	vector< vector<double> > A = D.getA();
	
	assert( D.getM()==15&&D.getN()==4 );
	assert( D.getM()==L.size() );
	
	double ll[15] = {0,0,1,1,0,0,0,1,1,1,1,1,1,1,0};
	vector<double> l = vv1( ll, 15 );
	assert( isSame( L, l ) );
	double a0[] = {0,0,0,0};
	double a8[] = {1,0,1,2};
	assert( isSame(A[0],a0,4) );
	assert( isSame(A[8],a8,4) );
#endif

#ifdef _DATA_IN_CASE_2_2_
/* Case 2.1
 * Goal: 1. test getFeatures( int )
 */
	double aa[15] = {0,0,0,1,0,0,0,1,1,1,1,1,0,0,0};
	vector<double> a = vv1( aa, 15 );
	assert( isSame(D.getFeatures(2),a) );	
#endif
}

void Data_pro1_check( const Data& D )
{

#define _DATA_IN_PRO_1_1_
#define _DATA_IN_PRO_1_2_

	vector<double> L = D.getL();
	vector< vector<double> > A = D.getA();
	
#ifdef _DATA_IN_PRO_1_1_
/* Pro1 Test 1
 * Goal: 1. test read data from csv
 */
	assert( D.getM()==9126&&D.getN()==46 );
	assert( L.size()==9126&&A.size()==9126&&A[0].size()==46 );

	assert( L[0]==0 );
	assert( L[9125]==1 );
	assert( L[1]==0&&L[3]==0 );
	assert( L[217]==1 );
	assert( L[8]==0&&L[18]==2 );
	assert( L[925]==1 );

	assert( abs(A[0][0]-0.016838)<1e-5 );
	assert( A[9125][0]==1 );
	assert( abs(A[0][45]-0.16667)<1e-5 );
	assert( A[9125][45]==0 );
	assert( A[1][3]==0 );
	assert( A[2][17]==0 );
	assert( abs(A[8][18]-0.54546)<1e-5 );
	assert( A[9][25]==0);
#endif

#ifdef _DATA_IN_PRO_1_2_
/* Pro1 Test 2
 * Goal: 1. test getFeatures( int )
 */
	int _test1[] = {0, 46, 1, 3, 8, 18 };
	for ( int i = 0; i < 6; i++ )
	{
		vector<double> f = D.getFeatures( _test1[i] );
		assert( f.size()==9126 );
	}

	vector<double> f = D.getFeatures( 13 );
	assert( f[0]==0 );
	assert( f[9125]==0 );
	assert( f[13]==0 );
	assert( f[818]==0 );
	assert( f[217]==0 );
	assert( f[9025]==0 );	
#endif
}

#endif


/*********************************************************************
 * Unit Test
 * by Jacob Pan
 *********************************************************************/

#ifdef _DATA_UTEST_

void test1()
{
	Data D;
	D.csvread( "test/case1.csv" );
	Data_case1_check( D );	
}

void test2()
{
	Data D;
	D.csvread( "test/case2.csv" );
	Data_case2_check( D );
}

void pro1Test()
{
	Data D;
	D.csvread( "dataset/pro1.csv" );
	Data_pro1_check( D );
}

int main()
{
	test1();	// done
	test2();	// done

	pro1Test();	// done

	cout << "All Test Cases Passed." << endl;
	return 0;
}

#endif

