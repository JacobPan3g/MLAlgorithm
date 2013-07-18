/*************************************************************************
	> File Name: bagging.cpp
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Tue 16 Jul 2013 09:15:14 PM CST
 ************************************************************************/

#include "BaggingData.cpp"
#include "tree.cpp"
using namespace std;

double bagging( int maxH, int bagNum, const vector<double> &a )
{
	vector<double> res( bagNum );
	
	for ( int i = 0; i < bagNum; i++ )
	{
		BaggingData D;
		BinaryTree tree( D, D.cs, D.fs, maxH );
		//tree.dispTree();
		//tree.dispLeaves();
		res[i] = tree.predict( a );
	}
	return mean( res );
}

double bagging( int maxH, int bagNum, const CsvData &test )
{
	vector< vector<double> > tmp( bagNum ); //bagNum * test.m
	for ( int i = 0; i < bagNum; i++ )
	{
		BaggingData D;
		BinaryTree tree( D, D.cs, D.fs, maxH );
		tmp[i] = tree.predict( test );
	}
	vector<double> p = meanR( tmp );
	return rmse( p, test.L );
}

int main()
{
	time_t tic = clock();

	CsvData test("dataset/pro1.csv");
	cout << bagging( 2, 10, test ) << endl;

	time_t toc = clock();
	double tt = (double)(toc-tic)/CLOCKS_PER_SEC;
	cout << "Time: " << tt << "s" << endl;

	return 0;
}
