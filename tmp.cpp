#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include "JPTool.cpp"
using namespace std;

int main()
{
	vector<int> v;
	v.push_back( 0 );
	v.push_back( 1 );
	v.push_back( 2 );
	v.push_back( 3 );
	v.push_back( 4 );
	v.push_back( 5 );

	v[1] = 9;
	v[3] = 8;

	sort( v.begin(), v.end() );

	//for ( int i = 0; i < a.size(); i++ )
	//	cout << a[i] << " ";
	//cout << endl;
	disp( v );

	

	return 0;
}
