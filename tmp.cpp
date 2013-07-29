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
	int *a;
	a = { 1, 3, 5 };

	disp( a, 3 );
	return 0;
}
