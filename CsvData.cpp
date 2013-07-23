/*************************************************************************
	> File Name: csv.cpp
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Mon 08 Jul 2013 09:14:40 AM CST
 ************************************************************************/

//#define _CSVDATA_UTEST_


#include "CsvData.h"

CsvData::CsvData( string filename )
{
	this->csvread( filename );
	m = A.size();
	n = A[0].size();
}


/*
 * csvread()
 * mainly in 3 parts
 * 1. read by lind
 * 2. split by ','
 * 3. transform char[] to num
 */
void CsvData::csvread( string filename )
{
	string line;
	ifstream fobj( filename.c_str() );

	//vector< vector<double> > res;	// the blank '> >' is necessary

	while ( getline( fobj, line ) )		//* the main cmd to read line
	{
		vector<double> tmp = split<double>( line, "," );
		this->L.push_back( tmp[0] );
		tmp.erase( tmp.begin() );
		this->A.push_back( tmp );
	}
	fobj.close();	
}

vector<double> CsvData::getFeatures( int fIdx, vector<int> tag=vector<int>() ) const
{
	if ( tag.size() == 0 )
		tag = vector<int>( this->m, 1 );

	vector<double> res;
	for ( int i = 0; i < this->m; i++ )
		if ( tag[i] == 1 )
			res.push_back( this->A[i][fIdx] );
	return res;
}

void CsvData::disp()
{
	for ( int i = 0; i < this->m; i++ )
	{
		cout << this->L[i] << " ";
		for ( int j = 0; j < this->n; j++ )
			cout << this->A[i][j] << " ";
		cout << endl;
	}
}


#ifdef _CSVDATA_UTEST_

int main()
{
	// Test: Constructor
	CsvData D( "dataset/pro1.csv" );
	
	assert( D.m==9126&&D.n==46 );
	assert( D.L.size()==9126&&D.A.size()==9126&&D.A[0].size()==46 );

	assert( D.L[0]==0 );
	assert( D.L[9125]==1 );
	assert( D.L[1]==0&&D.L[3]==0 );
	assert( D.L[217]==1 );
	assert( D.L[8]==0&&D.L[18]==2 );
	assert( D.L[925]==1 );

	assert( abs(D.A[0][0]-0.016838)<1e-5 );
	assert( D.A[9125][0]==1 );
	assert( abs(D.A[0][45]-0.16667)<1e-5 );
	assert( D.A[9125][45]==0 );
	assert( D.A[1][3]==0 );
	assert( D.A[2][17]==0 );
	assert( abs(D.A[8][18]-0.54546)<1e-5 );
	assert( D.A[9][25]==0);


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

	cout << "All Test Cases Passed." << endl;
	return 0;
}

#endif

