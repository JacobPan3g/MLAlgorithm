/*************************************************************************
	> File Name: SQ.cpp
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Mon 08 Jul 2013 01:36:58 PM CST
 ************************************************************************/

#include "CsvData.cpp"
using namespace std;

#define _TEST_

#define LEAF_MAX_NUM 5
#define SPLIT_AREA_NUM 10

vector<double> getSplitPoints( const vector<double> &v )
{
	vector<double> sp(SPLIT_AREA_NUM+1);
	double step = 1.0/SPLIT_AREA_NUM;
	double tmp = 0;
	for ( int i = 0; i < sp.size(); i++ )
		sp[i] = i * step;
	return sp;
}

double getSpByValueIdx( int x )
{
	return 0 + x * 1.0/SPLIT_AREA_NUM;
}

vector< vector<double> > measure( const CsvData &D, const vector<int> &r, int m, const vector<int> &c )
{
	vector< vector<double> > ss( D.n );
	for ( int i = 0; i < D.n; i++ )	
	{
		if ( !c[i] )
			continue;
			
		vector<double> tmp;
		vector<double> f = D.getFeatures(i);
		vector<double> sp = getSplitPoints( f );
		for ( int k = 0; k < sp.size(); k++ )
		{
			// desperate into two parts
			vector<int> part1(D.m, 0), part2(D.m, 0);
			int num1 = 0;
			int num2 = 0;
			for ( int j = 0; j < D.m; j++ )
			{
				if ( !r[j] )
					continue;

				if ( f[j] <= sp[k] )
				{
					part1[j] = 1;
					num1++;
				}
				else
				{
					part2[j] = 1;
					num2++;
				}
			}
	
			double var1 = variance( D.L, part1 );
			double var2 = variance( D.L, part2 );
			double num = num1 + num2;
			double ss = num1/num * var1 + num2/num * var2;
			tmp.push_back( ss );
		}
		ss[i] = tmp;
	}

	return ss;
}


double estimateLabel( const vector<double> &L, vector<int> tag )
{
	return mean(L, tag);
}

bool endCondition( vector<double> v, vector<int> tag, int num )
{
	return num < LEAF_MAX_NUM;
}

/*
#include <cstdio>
int main()
{
	CsvData D = CsvData("dataset/pro1.csv");
	
	vector<int> r(D.m, 1), c(D.n, 1);
	vector< vector<double> > res = measure( D, r, D.m, c );
	cout << res.size() << endl;

	disp(res);

	return 0;
}*/
