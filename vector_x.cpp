/**********************************************************************
	> File Name: vectorOpReride.cpp
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Wed 21 Aug 2013 08:04:37 PM CST
 *********************************************************************/

#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

//#include "JPTool.cpp"

template <class T>
ostream& operator<<( ostream& out, const vector<T>& v1 )
{
	int i;
	for ( i = 0; i < v1.size()-1; i++ )
		cout << v1[i] << "\t";
	cout << v1[i];
	return out;
}

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

/*
int main()
{
	vector<int> a( 5, 5 ), b( 5, 3 );
	cout << a - b << endl;
	cout << a + b << endl;

	return 0;
}*/
