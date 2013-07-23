/*************************************************************************
	> File Name: Tree.cpp
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Mon 22 Jul 2013 02:43:17 PM CST
 ************************************************************************/

#include "JPTool"

class Trees
{
public:
	Trees( string filename );
	
	vector< vector<int> > fIdxV2;
	vector< vector<int> > leftV2;
	vector< vector<int> > rightV2;
	vector< vector<double> > oValV2;
};

Trees::Trees( string filename )
{
	vector< vector<double> > v2;
	csvread( filename, v2 );

	vector<double> p;
	for (  )
}




