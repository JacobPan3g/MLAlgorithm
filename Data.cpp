/**********************************************************************
	> File Name: Data.cpp
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Mon 08 Jul 2013 09:14:40 AM CST
 *********************************************************************/

#define _DATA_UTEST_


#include "Data.h"

Data::Data()
{
	this->m = 0;
	this->n = 0;
}

void Data::dirread( const vector<double>& L, const vector< vector<double> >& A )
{
	this->init();
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

void Data::fmtread( const string& fNM )
{
	this->init();

	ifstream fobj( fNM.c_str() );
	// read the size
	int fmtV_size;	// n
	int list_size;	// m
	char tmpC;
	fobj >> tmpC >> fmtV_size >> tmpC >> list_size;
	// read the L
	fobj >> tmpC;
	this->L = vector<double>( list_size );
	this->A = vector< vector<double> >( list_size );
	for ( int i = 0; i < list_size; i++ ) {
		fobj >> this->L[i];
		this->A[i] = vector<double>( fmtV_size );
	}
	// read the fmt (A)
	fobj >> tmpC;
	this->fmtV = vector< list< pair<int,double> > >( fmtV_size );
	for ( int i = 0; i < fmtV_size; i++ ) {
		for ( int j = 0; j < list_size; j++ ) {
			pair<int,double> tmpP;
			fobj >> tmpP.first >> tmpC >> tmpP.second;
			fmtV[i].push_back( tmpP );
			this->A[tmpP.first][i] = tmpP.second;
		}
	}
	fobj.close();

	this->m = A.size();
	this->n = A[0].size();
}

void Data::csvwrite( const string& fNM )
{}

void Data::fmtwrite( const string& fNM )
{
	this->toFmt();

	ofstream fobj( fNM.c_str() );
	// save the size
	fobj << "n " << this->fmtV.size() << " m " << this->fmtV[0].size() << endl;
	// save the L
	fobj << "L" << endl;
	for ( int i = 0; i < this->L.size(); i++ ) {
		fobj << this->L[i] << " ";
	}
	fobj << endl;
	// save the fmt (A)
	fobj << "A" << endl; 
	for ( int i = 0; i < this->fmtV.size(); i++ ) {
		list< pair<int,double> >::iterator it;
		for ( it = this->fmtV[i].begin(); it != this->fmtV[i].end(); it++ ) {
			fobj << it->first << "|" << it->second << " ";
		}
		fobj << endl;
	}
	fobj.close();
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

const vector< list< pair<int,double> > > Data::getFmtV() const
{
	return this->fmtV;
}

// cmp method
bool cmp( const pair<int, double>& a, const pair<int, double>& b )
{
	return a.second < b.second;
}

// Private Methods
void Data::init()
{
	this->m = 0;
	this->n = 0;
	this->L.clear();
	this->A.clear();
	this->fmtV.clear();
}

void Data::toFmt()
{
	this->fmtV.resize( this->n );
	for ( int j = 0; j < this->n; j++ ) {
		for ( int i = 0; i < this->m; i++ ) {
			this->fmtV[j].push_back( pair<int, double>( i, this->A[i][j] ) );
		}
		assert( this->fmtV[j].size()==this->m );
		
		//disp( this->fmtV[j] );
		this->fmtV[j].sort( cmp );
		//disp( this->fmtV[j] );
	}
}


/*********************************************************************
 * Unit Test
 * by Jacob Pan
 *********************************************************************/

#ifdef _DATA_UTEST_

void test1_check( const Data &D )
{
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
}

void test1()
{

#define _TEST_1_1_
#define _TEST_1_2_

#ifdef _TEST_1_1_
/* Test 1.1
 * Goal: 1. test read data from csv
 */
	Data D2csv;
	D2csv.csvread( "test/case1.csv" );
	test1_check( D2csv );
#endif

#ifdef _TEST_1_2_
/* Test 1.2
 * Goal: 1. test read data from csv
 */
	Data D;
	D.csvread( "test/case1.csv" );
	D.fmtwrite( "test/case1.fmt" );
	Data D2fmt;
	D2fmt.fmtread( "test/case1.fmt" );
	test1_check( D2fmt );
#endif
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

// test for fmtwrite()
void test3()
{
	Data D;
	D.csvread( "test/case1.csv" );
	D.fmtwrite( "test/case1.fmt" );

	vector< list< pair<int,double> > > fmtV = D.getFmtV();
	assert( fmtV.size()==D.getN() );

	list< pair<int,double> >::iterator it = fmtV[3].begin();
	assert( it->first==2&&it->second==0 );

	//cout << (++it)->first << "|" << it->second << endl;
	/*ATTEND! There is a strange for ++it in cout << << */

	assert( (++it)->first==0&&isEqual(it->second,0.4) );
	assert( (++it)->first==1&&isEqual(it->second,0.5) );
	assert( (++it)->first==3&&isEqual(it->second,0.8) );
	assert( (++it)->first==4&&isEqual(it->second,0.9) );
}

/*
// just for output fmt
void test4()
{
	Data D;
	D.fmtread( "test/case1.fmt" );
	vector< list< pair<int,double> > > fmtV = D.getFmtV();
	for ( int i = 0; i < fmtV.size(); i++ )
		disp( fmtV[i] );
}*/

void liveTest_check( const Data& D )
{
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

void liveTest()
{

#define _LIVE_TEST_1
#define _LIVE_TEST_2_
#define _LIVE_TEST_3_


#ifdef _LIVE_TEST_1_
/* Live Test 1
 * Goal: 1. test read data from csv
 */
	Data D1csv;
	D1csv.csvread( "dataset/pro1.csv" );
	liveTest_check( D1csv );
#endif

#ifdef _LIVE_TEST_2_
/* Live Test 2
 * Goal: 1. test read data write to fmt
 */
	Data D2csv;
	D2csv.csvread( "dataset/pro1.csv" );
	D2csv.fmtwrite( "dataset/pro1.fmt" );
#endif

#ifdef _LIVE_TEST_3_
/* Live Test 3
 * Goal: 1. test read data from csv
 */
	Data D3;
	D3.csvread( "dataset/pro1.csv" );
	D3.fmtwrite( "dataset/pro1.fmt" );
	Data D3fmt;
	D3fmt.fmtread( "dataset/pro1.fmt" );
	liveTest_check( D3fmt );
#endif
}


int main()
{
//	test1();	// done
//	test2();	// done
//	test3();	// done // for fmtwrite

	liveTest();

	cout << "All Test Cases Passed." << endl;
	return 0;
}

#endif

