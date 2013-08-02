/*************************************************************************
	> File Name: JPTool.cpp
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Tue 09 Jul 2013 04:31:42 PM CST
 ************************************************************************/

//#define _JPTOOL_UTEST_
#define _INNER_TEST_


#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>		// rand()
#include <cmath>
#include <ctime>		// time()
#include <fstream>
#include <cassert>
#include <set>
#include <iterator>		// ostream_iterator
#include <algorithm>	// copy()
#include <list>
using namespace std;

#define EPS 1e-5

#define nonzero(x) ( (double)(x==0 ? x+1e-8 : x) )
#define round(x) ((int)(x+0.5))
#define src(x) ( abs(x-round(x)) < EPS ? round(x) : x )


/*
 * statistic
 */
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
vector<T> min( const vector< vector<T> > &v2, vector<int> r=vector<int>() )
{	
	if ( r.size() == 0 )
		r = vector<int>( v2.size(), 1 );

	vector<T> res(3, 0);	// idx 0 for min, idx 1 for idx, idx 2 for value idx
	res[0] = 1e8;

	for ( int i = 0; i < v2.size(); i++ )
	{
		if ( !r[i] )
			continue;
		for ( int j = 0; j < v2[i].size(); j++ )
		{
			if ( v2[i][j] < res[0] )
			{
				res[0] = v2[i][j];
				res[1] = i;
				res[2] = j;
			}
		}
	}
	
#ifdef _INNER_TEST_
	bool isMin = true;
	bool jump = false;
	for ( int i = 0; i < v2.size(); i++ )
	{
		if ( !r[i] )
			continue;
		for ( int j = 0; j < v2[i].size(); j++ )
		{
			if ( v2[i][j] < res[0] )
			{
				isMin = false;
				jump = true;
				break;
			}
		}
		if ( jump )
			break;
	}
	assert( isMin );
#endif

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

// the v2 and w2 is a rectangle matrix
template <class T>
bool isSame( const vector< vector<T> > &v2, const vector< vector<T> > &w2 )
{
	// one v is NULL
	if ( v2.size() == 0 || w2.size() == 0 )
		if ( v2.size() != 0 || w2.size() != 0 )
			return false;
		else
			return true;
	else { 
		if ( v2.size() != w2.size() )
			return false;
		else {
			for ( int i = 0; i < v2.size(); i++ ) {
				if ( v2[i].size() != w2[i].size() ) {
					return false;
				}
			}
			// size same
			for ( int i = 0; i < v2.size(); i++ ) {
				for ( int j = 0; j < v2[i].size(); j++ ) {
					if ( v2[i][j] - w2[i][j] >= EPS ) {	
						return false;
					}
				}
			}
		}
	}
	return true;
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
	return res/nonzero(num);
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
	return res/nonzero(num);
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
//template <class T>
//void disp( const T  )

template <class T>
void disp( const vector<T> &v, vector<int> tag=vector<int>() )
{
	if ( tag.size() == 0 )
		tag.resize( v.size(), 1 );

	for ( int i = 0; i < v.size(); i++ )
		if ( tag[i] == 1 )
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

template <class T>
void disp( set<T> s )
{
	ostream_iterator<T> out_it(cout, " ");
	copy( s.begin(), s.end(), out_it ); //array use as ( a, a+l, o )
	cout << endl;
}

void disp( const string& str )
{
	cout << str << endl;
}

void disp( const list< pair<int,double> >& l )
{
	list< pair<int,double> >::const_iterator it = l.begin();
	while ( it != l.end() ) {
		cout << it->first << "|" << it->second << " ";
		it++;
	}
	cout << endl;
}

//template <class T>
//void disp( list )

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
vector<T> vv1( T arr[], int len )
{
	vector<T> res(len);
	copy( arr, arr+len, res.begin() );
	return res;
}

// transform set to vector
template <class T>
vector<T> vv1( set<T> s )
{
	vector<T> res( s.size() );
	copy( s.begin(), s.end(), res.begin() );
	return res;
}

// remove reduplication
template <class T>
vector<T> vv1( vector<T> v, vector<int> tag=vector<int>() )
{
	if ( tag.size() == 0 )
		tag.resize( v.size(), 1 );
	
	set<T> s;
	for ( int i = 0; i < v.size(); i++ )
		if ( tag[i] )
			s.insert( v[i] );
	vector<T> res( s.size() );
	copy( s.begin(), s.end(), res.begin() );
	return res;
}

template <class T>
vector< vector<T> > vv2( int r, int c, T value )
{
	vector< vector<T> > res( r );
	for ( int i = 0; i < res.size(); i++ )
	{
		res[i].resize( c, value );
	}
	return res;
}

template <class T>
set<T> ss( const vector<T> &v, vector<int> tag=vector<int>() )
{
	if ( tag.size() == 0 )
		tag.resize( v.size(), 1 );
	
	set<T> res;
	for ( int i = 0; i < v.size(); i++ )
		if ( tag[i] )
			res.insert( v[i] );
	return res;
}

/*
 * for tag
 */
int countTag( const vector<int> &tag )
{
	int res = 0;
	for ( int i = 0; i < tag.size(); i++ )
		if ( tag[i] == 1 )
			res++;
	return res;
}

void resetTag( vector<int> &tag, int idx[], int len )
{
	for ( int i = 0; i < len; i++ )
		if ( idx[i] < tag.size() )
			tag[idx[i]] = 0;
}





#ifdef _JPTOOL_UTEST_

int main()
{
	//int a[] = { 1,3,3,5,5};
	vector<int> v = vv1( { 1, 2, 3 }, 5 );
	//vector<int> tag( 5, 1 );
	//tag[0] = 0;
	//vector<int> vv = vv1( v, tag );
	//disp( vv );

	return 0;
}

#endif
