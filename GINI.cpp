/*************************************************************************
	> File Name: GINI.cpp
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Mon 08 Jul 2013 01:36:58 PM CST
 ************************************************************************/

#include "CsvData.cpp"
using namespace std;

vector<double> measure( const CsvData &D, const vector<int> &r, int m, const vector<int> &c )
{
	vector<double> ginis(D.n);
	for ( int i = 0; i < D.n; i++ )
	{
		if ( !c[i] )
			continue;

		int dNum = 0;
		int cNum1 = 0;
		int cNum2 = 0;
		for ( int j = 0; j < D.m; j++ )
		{
			if ( !r[j] )
				continue;

			if ( D.A[j][i] == 1 )
			{
				dNum++;
				if ( D.L[j] == 1 )
					cNum1++;
			}
			else
			{
				if ( D.L[j] == 1 )
					cNum2++;
			}
		}
		double P = dNum/(m+1e-8);
		double p1 = cNum1/(dNum+1e-8);
		double p2 = cNum2/(m-dNum+1e-8);
		ginis[i] = P*(2*p1*(1-p1)) + (1-P)*(2*p2*(1-p2));
	}
	return ginis;
}

double estimateLabel( const vector<double> &L, vector<int> tag )
{
	// get the mean
	int sum = 0;
	int num = 0;
	for ( int i = 0; i < L.size(); i++ )
	{
		if ( tag[i] )
		{
			sum += L[i];
			num++;
		}
	}

	double mean = (double)sum/num;
	// estimate the label
	if ( mean >= 0 )
		return 1;
	else
		return -1;
}

bool endCondition( vector<double> v, vector<int> tag, int num )
{
	return isAllSame( v, tag );
}

/*
int main()
{
	CsvData D = CsvData("a1a.csv");
	
	vector<int> r(D.m, 1), c(D.n, 1);
	vector<double> res = GINI( D, r, D.m, c );
	cout << res.size() << endl;

	for ( int i = 0; i < res.size(); i++ )
		cout << res[i] << endl;
	
	return 0;
}*/
