/**********************************************************************
	> File Name: TR_Data.cpp
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Tue 06 Aug 2013 11:55:36 AM CST
 *********************************************************************/

#define _TR_DATA_UTEST_
#define _TR_DATA_IN_CHECK_


#include "TR_Data.h"
#include "Data.cpp"

TR_Data::TR_Data() : Data()
{
	this->init();
}

void TR_Data::dispFmtV( int f, int r ) const
{
	if ( r == -1 ) {
		r = this->fmtV.size()-1;
	}

	for ( int i = f; i <= r; i++ ) {
		list< pair<int,double> >::const_iterator it = this->fmtV[i].begin();
		while ( it != this->fmtV[i].end() ) {
			cout << it->first << "|" << it->second << " ";
			it++;
		}
		cout << endl;
	}
}

void TR_Data::dirread( const vector<double>& L, const vector< vector<double> >& A )
{
	Data::dirread( L, A );
	this->toFmt();
}

void TR_Data::csvread( const string& fNM )
{
	Data::csvread( fNM );
	this->toFmt();
}

void TR_Data::fmtread( const string& fNM )
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

void TR_Data::fmtwrite( const string& fNM )
{
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

//getter
const vector< list< pair<int,double> > > TR_Data::getFmtV() const
{
	return this->fmtV;
}

// cmp method
bool cmp( const pair<int, double>& a, const pair<int, double>& b )
{
	return a.second < b.second;
}

// Private Methods
void TR_Data::init()
{
	Data::init();
	this->fmtV.clear();
}

void TR_Data::toFmt()
{
	this->fmtV = vector< list <pair<int,double> > >( this->n );
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
 * Unit Test for SubClass
 * by Jacob Pan
 *********************************************************************/

#ifdef _TR_DATA_IN_CHECK_

void TR_Data_case1_check( const TR_Data& D )
{

#define _TR_DATA_CASE_1_1_
#define _TR_DATA_CASE_1_2_

#ifdef _TR_DATA_CASE_1_1_
/* Case 1.1
 * Goal: 1. regression tree for super test
 */
	Data_case1_check( D );
#endif

#ifdef _TR_DATA_CASE_1_2_
/* Case 1.2
 * Goal: 1. test fmtV
 */
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
#endif
}

void TR_Data_case2_check( const TR_Data& D )
{
#define _TR_DATA_CASE_2_1_
#define _TR_DATA_CASE_2_2_

#ifdef _TR_DATA_CASE_2_1_
/* Case 2.1
 * Goal: 1. regression tree for super test
 */
	Data_case2_check( D );
#endif

#ifdef _TR_DATA_CASE_2_2_
/* Case 2.2
 * Goal: 1. test fmtV
 */
	vector< list< pair<int,double> > > fmtV = D.getFmtV();
	assert( fmtV.size()==D.getN() );

	//D.dispFmtV();
	
	// just check one line
	list< pair<int,double> >::iterator it = fmtV[3].begin();
	assert( it->first==0&&it->second==0 );
	assert( (++it)->first==3&&it->second==0 );
	assert( (++it)->first==4&&it->second==0 );
	assert( (++it)->first==5&&it->second==0 );
	assert( (++it)->first==14&&it->second==0 );
	assert( (++it)->first==1&&it->second==1 );
	assert( (++it)->first==2&&it->second==1 );
	assert( (++it)->first==6&&it->second==1 );
	assert( (++it)->first==7&&it->second==1 );
	assert( (++it)->first==11&&it->second==1 );
	assert( (++it)->first==12&&it->second==1 );
	assert( (++it)->first==8&&it->second==2 );
	assert( (++it)->first==9&&it->second==2 );
	assert( (++it)->first==10&&it->second==2 );
	assert( (++it)->first==13&&it->second==2 );
#endif
}

void TR_Data_pro1_check( const TR_Data& D )
{
#define _TR_DATA_PRO_1_1_
#define _TR_DATA_PRO_1_2_

#ifdef _TR_DATA_PRO_1_1_
/* pro1 test 1
 * Goal: 1. regression tree for super test
 */
	Data_pro1_check( D );
#endif
	
#ifdef _TR_DATA_PRO_1_2_
/* pro1 test 2
 * Goal: 1. fmtV
 */
	vector< list< pair<int,double> > > fmtV = D.getFmtV();
	assert( fmtV.size()==D.getN() );

	//D.dispFmtV();
	
	// just check some value
	list< pair<int,double> >::reverse_iterator it = fmtV[3].rbegin();
	assert( it->first==9110&&it->second==1 );
	assert( (++it)->first==9108&&it->second==1 );
	assert( (++it)->first==9103&&it->second==1 );
	assert( (++it)->first==9100&&it->second==1 );
#endif
}


#endif


/*********************************************************************
 * Unit Test
 * by Jacob Pan
 *********************************************************************/

#ifdef _TR_DATA_UTEST_

void test1()
{
	TR_Data D4csv;
	D4csv.csvread( "test/case1.csv" );
	D4csv.fmtwrite( "tmp/tCase1.fmt" );
	
	TR_Data D;
	D.fmtread( "tmp/tCase1.fmt" );
	TR_Data_case1_check( D );
}

void test2()
{
	TR_Data D4csv;
	D4csv.csvread( "test/case2.csv" );
	D4csv.fmtwrite( "tmp/tCase2.fmt" );
	
	TR_Data D;
	D.fmtread( "tmp/tCase2.fmt" );
	TR_Data_case2_check( D );
}

void pro1Test()
{
	TR_Data D;
	D.fmtread( "dataset/pro1.fmt" );
	TR_Data_pro1_check( D );

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
