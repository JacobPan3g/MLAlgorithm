/*************************************************************************
	> File Name: test.cpp
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: Mon 08 Jul 2013 09:15:50 AM CST
 ************************************************************************/

#include "CsvData.cpp"
#include <ctime>
using namespace std;

int main()
{
	time_t tic, toc;
	tic = clock();

	CsvData D = CsvData("data.csv");
	cout << D.m << " " << D.n << endl;
	
	

	//vector<int> r(D.m, 1), c(D.n, 1);
	//vector<double> gini = GINI( D, r, D.m, c );

	int num = 0;
	for ( int i = 0; i < D.L.size(); i++ )
		num += D.L[i];
	cout << (double)num / D.m << endl;

	toc = clock();
	double total = (double)(toc-tic)/CLOCKS_PER_SEC;
	cout << "Time:" << total << endl;
	return 0;
}

