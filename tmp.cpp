#include <iostream>
#include <vector>
#include <cmath>
#include "JPTool.cpp"
using namespace std;

template <class T>
void disp( T arr[], int len )
{
	for ( int i = 0; i < len; i++ )
		cout << arr[i] << " ";
	cout << endl;
}


int main()
{
	vector< vector<double> > sp, ms;
	csvread( "sp.csv", sp );
	csvread( "ms.csv", ms );
	cout << sp[36].size() << " " << ms[36].size() << endl;

	return 0;
}
