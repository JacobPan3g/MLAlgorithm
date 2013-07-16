/*************************************************************************
	> File Name: bagging.cpp
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: Tue 16 Jul 2013 09:15:14 PM CST
 ************************************************************************/

#include "BaggingData.cpp"
#include "tree.cpp"
using namespace std;

void bagging()
{
	BaggingData D( "dataset/pro1.csv" );
	BinaryTree tree( D );
	//cout << D.bagNum << endl;
}

int main()
{
	bagging();

	return 0;
}
