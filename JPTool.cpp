/*************************************************************************
	> File Name: JPTool.cpp
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Tue 09 Jul 2013 04:31:42 PM CST
 ************************************************************************/

//#define _JPTOOL_UTEST_


#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>		// rand()
#include <cmath>
#include <ctime>		// time()
#include <fstream>
#include <cassert>
using namespace std;

#define EPS 1e-5


/*
 * statistic
 */
int count( const vector<int> &tag )
{
	int res = 0;
	for ( int i = 0; i < tag.size(); i++ )
		if ( tag[i] == 1 )
			res++;
	return res;
}

template <class T>
vector<T> min( const vector<T> &ginis, vector<int> tag=vector<int>() )
{	
	if ( tag.size() == 0 )
		tag = vector<int>( ginis.size(), 1 );

	vector<T> res(2, 0);	// idx 1 for min, idx 2 for idx
	res[0] = 1e8;

	for ( int i = 0; i < ginis.size(); i++ )
		if ( tag[i] == 1 && ginis[i] < res[0] )
		{
			res[0] = ginis[i];
			res[1] = i;
		}
	return res;
}

template <class T>
vector<T> min( const vector< vector<T> > &v2, vector<int> tag=vector<int>() )
{	
	if ( tag.size() == 0 )
		tag = vector<int>( v2.size(), 1 );

	vector<T> res(3, 0);	// idx 0 for min, idx 1 for idx, idx 2 for value idx
	res[0] = 1e8;

	for ( int i = 0; i < v2.size(); i++ )
	{
		for ( int j = 0; j < v2[i].size(); j++ )
		{
			if ( tag[i] == 1 && v2[i][j] < res[0] )
			{
				res[0] = v2[i][j];
				res[1] = i;
				res[2] = j;
			}
		}
	}
	return res;
}

template <class T>
bool isAll( const vector<T> &v, T value, vector<int> tag=vector<int>() )
{
	if ( tag.size() == 0 )
		tag = vector<int>( v.size(), 1 );
	
	bool all = true;
	for ( int i = 0; i < v.size(); i++ )
	{
		if ( tag[i] && v[i] != value )
			all = false;
	}
	return all;
}

template <class T>
bool isAllSame( const vector<T> &v, vector<int> tag=vector<int>() )
{
	if ( tag.size() == 0 )
		tag = vector<int>( v.size(), 1 );
	
	bool allSame = true;
	bool isEmpty = true;	// if empty, function return true
	T tmp;
	for ( int i = 0; i < v.size(); i++ )
	{
		if ( tag[i] )
		{
			tmp = v[i];
			isEmpty = false;
			break;
		}
	}

	if ( !isEmpty )
		for ( int i = 0; i < v.size(); i++ )
			if ( tag[i] && v[i] != tmp )
				allSame = false;

	return allSame;
}

template <class T>
bool isSame( const vector<T> &v, const vector<T> &w )
{
	bool res = true;
	if ( v.size() != w.size() )
		res = false;
	else
		for( int i = 0; i < v.size(); i++ )
			if ( v[i] - w[i] >= EPS )
			{
				res = false;
				break;
			}
	return res;
}

template <class T>
bool isSame( const vector< vector<T> > &v2, const vector< vector<T> > &w2 )
{
	bool res = true;
	// one v is NULL
	if ( v2.size() == 0 || w2.size() == 0 )
		if ( v2.size() != 0 || w2.size() != 0 )
			res = false;
		else
			res = true;
	else if ( v2.size() != w2.size() || v2[0].size() != w2[0].size() )
		res = false;
	else
		for ( int i = 0; i < v2.size(); i++ )
			for ( int j = 0; j < v2[0].size(); j++ )
				if ( v2[i][j] - w2[i][j] >= EPS )
				{	
					res = false;
					break;
				}
	return res;
}

template <class T>
T sum( const vector<T> &v, vector<int> tag=vector<int>() )
{
	if ( tag.size() == 0 )
		tag = vector<int>( v.size(), 1 );

	T res = 0;
	for ( int i = 0; i < v.size(); i++ )
		res += v[i] * tag[i];
	return res;
}

template <class T>
double mean( const vector<T> &v, vector<int> tag=vector<int>() )
{
	if ( tag.size() == 0 )
		tag = vector<int>( v.size(), 1 );

	T res = 0;
	int num = 0;
	for ( int i = 0; i < v.size(); i++ )
	{
		if ( tag[i] == 1 )
		{
			res += v[i];
			num++;
		}
	}
	return res/(num+1e-8);
}

template <class T>
vector<double> meanR( const vector< vector<T> > &v2 )
{
	int n = v2[0].size();
	int m = v2.size();
	vector<double> res( n, 0 );
	for ( int j = 0; j < n; j++ )
	{
		for ( int i = 0; i < m; i++ )
			res[j] += v2[i][j];
		res[j] /= m;
	}
	return res;
}

template <class T>
double variance( const vector<T> &v, vector<int> tag=vector<int>() )
{
	if ( tag.size() == 0 )
		tag = vector<int>( v.size(), 1 );
	
	int num = 0;
	double res = 0;
	double aver = mean( v, tag );
	for ( int i = 0; i < v.size(); i++ )
	{
		if ( tag[i] == 1 )
		{
			num++;
			res	+= pow(v[i]-aver, 2 );	
		}
	}
	return res/(num+1e-8);
}

/*
 * Random
 */
// generate random number in [0,x]
int random( int x )
{
	srand( (int)time(0) );
	return rand()%(x+1);
}

template <class T>
void shuffling( vector<T> &v )
{
	int n = v.size();
	for ( int i = n-1; i >= 0; i-- )
	{
		// shuffling for n times
		int j =	random( i );
		T tmp = v[i];
		v[i] = v[j];
		v[j] = tmp;
	}
	//return v;
}

/*
 * disp
 */
template <class T>
void disp( const vector<T> &v )
{
	for ( int i = 0; i < v.size(); i++ )
		cout << v[i] << " ";
	cout << endl;
}

template <class T>
void disp( vector< vector<T> > v2 )
{
	for ( int i = 0; i < v2.size(); i++ )
	{
		for ( int j = 0; j < v2[i].size(); j++ )
			cout << v2[i][j] << " ";
		cout << endl;
	}
	cout << endl;
}


/*
 * string
 */
template <class T>
vector<T> split( string str, string pattern )
{
	vector<T> res;
	str += pattern;
	int i = 0;
	int pos = str.find( pattern, i );
	
	while( pos != -1 )
	{
		string s = str.substr( i, pos-i );
		T d = atof( s.c_str() );
		res.push_back( d );
		i = pos + pattern.size();
		pos = str.find( pattern, i );
	}
	return res;
}

/*
 * error rate in ML
 */
template <class T>
double rmse( const vector<T> &p, const vector<T> &r )
{
	double res = 0;
	for ( int i = 0; i < p.size(); i++ )
		res += pow( p[i] - r[i], 2 );
	return sqrt( res/p.size() );
}

/*
 * Persistence
 */
template <class T>
void save( string filename, const vector<T> &v ) 
{
	ofstream fobj( filename.c_str() );
	for ( int i = 0; i < v.size(); i++ )
		fobj << v[i] << endl;
	fobj.close();
}

template <class T>
void save( ofstream &fobj, const vector<T> &v, string d=" " )
{
	for ( int i = 0; i < v.size(); i++ )
		fobj << v[i] << d;
	fobj << endl;
}

template <class T>
void csvread( string filename, vector< vector<T> > &v2 )
{
	string line;
	ifstream fobj( filename.c_str() );
	while ( getline( fobj, line ) )
	{
		vector<T> tmp = split<T>( line, "," );
		v2.push_back( tmp );
	}
	fobj.close();
}

/*
 * init
 */
template <class T>
vector< vector<T> > v2( int r, int c, T value )
{
	vector< vector<T> > res( r );
	for ( int i = 0; i < res.size(); i++ )
	{
		res[i].resize( c, value );
	}
	return res;
}

#ifdef _JPTOOL_UTEST_

int main()
{
	vector< vector<double> > a = v2(2,3,2.0);
	vector< vector<double> > b = v2<double>(2,3,1);
	
	/*for ( int i = 0; i < a.size(); i++ )
	{
		a[i].resize( 3, 1 );
		b[i].resize( 3, 1 );
	}*/

	
	disp( a );
	cout << endl;
	disp( b );
	if ( isSame( a, b) )
		cout << "Same" << endl;
	
	return 0;
}

#endif
