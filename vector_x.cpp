/**********************************************************************
	> File Name: vectorOpReride.cpp
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Wed 21 Aug 2013 08:04:37 PM CST
 *********************************************************************/

#include <iostream>
#include <vector>
#include <cassert>
#include <fstream>
using namespace std;

//#include "JPTool.cpp"

template <class T>
ostream& operator<<( ostream& os, const vector<T>& v1 )
{
	int i;
	for ( i = 0; i < v1.size()-1; i++ )
		os << v1[i] << "\t";
	os << v1[i];
	return os;
}
/*
template <class T>
istream& operator>>( istream& is, vector<T>& v1 )
{
	return is;
}*/


template <class T>
vector<T> operator+( const vector<T>& v1, const vector<T>& w1 )
{
	int sz = v1.size();
	assert( sz == w1.size() );
	vector<T> res( sz );
	for ( int i = 0; i < sz; i++ ) {
		res[i] = v1[i] + w1[i];
	}
	return res;
}

template <class T>
vector<T>& operator+=( vector<T>& v1, const vector<T>& w1 )
{
	int sz = v1.size();
	assert( sz == w1.size() );
	for ( int i = 0; i < sz; i++ ) {
		v1[i] += w1[i];
	}
	return v1;
}

template <class T>
vector<T> operator-( const vector<T>& v1, const vector<T>& w1 )
{
	int sz = v1.size();
	assert( sz == w1.size() );
	vector<T> res( sz );
	for ( int i = 0; i < sz; i++ ) {
		res[i] = v1[i] - w1[i];
	}
	return res;
}

template <class T>
vector<T>& operator-=( vector<T>& v1, const vector<T>& w1 )
{
	int sz = v1.size();
	assert( sz == w1.size() );
	for ( int i = 0; i < sz; i++ ) {
		v1[i] -= w1[i];
	}
	return v1;
}

/*
int main()
{
	vector<int> a( 5, 5 ), b( 5, 3 );
	ofstream fout( "output.txt" );
	cout << a - b << endl;
	fout << a + b << endl;
	fout << a - b << endl;
	fout.close();

	a += b;
	cout << a << endl;
	a -= b;
	cout << a << endl;

	return 0;
}*/
