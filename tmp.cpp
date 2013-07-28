#include <iostream>
#include <vector>
#include <cmath>
#include "JPTool.cpp"
using namespace std;


int main()
{
	int a[] = { 1, 3, 5 };
	vector<int> tmp( 5, 0 );
	setTags( tmp, a, 3 );

	disp( tmp );
	return 0;
}
