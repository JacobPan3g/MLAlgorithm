/*************************************************************************
	> File Name: bagging.cpp
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: Tue 16 Jul 2013 09:15:14 PM CST
 ************************************************************************/

#include "BaggingData.cpp"
#include "tree.cpp"
using namespace std;

double bagging( int bagNum, const vector<double> &a )
{
	vector<double> res( bagNum );
	
	for ( int i = 0; i < bagNum; i++ )
	{
		BaggingData D;
		BinaryTree tree( D );
		tree.dispTree();
		//tree.dispLeaves();
		res[i] = tree.predict( a );
	}
	return mean( res );
}

int main()
{
	CsvData test("dataset/pro1.csv");
	cout << bagging( 2, test.A[18] ) << endl;

	return 0;
}
