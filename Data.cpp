/**********************************************************************
	> File Name: Data.cpp
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Mon 08 Jul 2013 09:14:40 AM CST
 *********************************************************************/

//#define _DATA_UTEST_


#include "Data.h"

Data::Data()
{
	this->m = 0;
	this->n = 0;
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
	this->L.resize(0);
	this->A.resize(0);

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

void Data::disp() const
{
	for ( int i = 0; i < this->m; i++ )
	{
		cout << this->L[i] << " ";
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


#ifdef _DATA_UTEST_

void test1()
{
	Data D;
	D.csvread( "test/case1.csv" );
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
	
	assert( isSame(D.getL(),L) );
	assert( isSame(D.getA(),A) );
}

void test2()
{
	Data D;
	D.csvread( "test/case2.csv" );
	vector<double> L = D.getL();
	vector< vector<double> > A = D.getA();
	
	assert( D.getM()==15&&D.getN()==4 );
	assert( D.getM()==L.size() );
	
	double ll[15] = {0,0,1,1,0,0,0,1,1,1,1,1,1,1,0};
	vector<double> l = vv1( ll, 15 );
	assert( isSame( L, l ) );
	double aa[15] = {0,0,0,1,0,0,0,1,1,1,1,1,0,0,0};
	vector<double> a = vv1( aa, 15 );
	assert( isSame(D.getFeatures(2),a) );
}

void liveTest()
{
	Data D;
	D.csvread( "dataset/pro1.csv" );
	vector<double> L = D.getL();
	vector< vector<double> > A = D.getA();
	
	// Test: Constructor
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

	// Test: getFeatures( int )
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
}


int main()
{
	test1();	// done
	test2();	// done
	
	liveTest();

	cout << "All Test Cases Passed." << endl;
	return 0;
}

#endif

