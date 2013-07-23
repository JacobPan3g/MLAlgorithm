/*************************************************************************
	> File Name: SQ.cpp
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Mon 08 Jul 2013 01:36:58 PM CST
 ************************************************************************/

#define _SS_UTEST_


#include "SS.h"
#include "CsvData.cpp"

#define LEAF_MAX_NUM 5
#define SPLIT_AREA_NUM 10

// Average Spilt the Area [0,1]
vector<double> SS::getSplitPoints( const vector<double> &v )
{
	vector<double> sp(SPLIT_AREA_NUM+1);
	double step = 1.0/SPLIT_AREA_NUM;
	double tmp = 0;
	for ( int i = 0; i < sp.size(); i++ )
		sp[i] = i * step;
	return sp;
}

double SS::getSpByValueIdx( int x )
{
	return 0 + x * 1.0/SPLIT_AREA_NUM;
}

vector< vector<double> > SS::measure( const CsvData &D, const vector<int> &r, const vector<int> &c )
{
	vector< vector<double> > ss( D.n );
	for ( int i = 0; i < D.n; i++ )	
	{
		if ( !c[i] )
			continue;
			
		vector<double> tmp;
		vector<double> f = D.getFeatures(i, r);
		vector<double> sp = getSplitPoints( f );

		assert( f.size()==count(r) );

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

			assert( count(r)==num1+num2 );
			assert( count(r)==count(part1)+count(part2) );
	
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


double SS::estimateLabel( const vector<double> &L, const vector<int> &tag )
{
	return mean(L, tag);
}

bool SS::endCondition( vector<double> v, vector<int> tag, int num )
{
	return num < LEAF_MAX_NUM;
}


#ifdef _SS_UTEST_

int main()
{
	CsvData D = CsvData("dataset/pro1.csv");
	SS ms;
	vector<int> r(D.m, 1), c(D.n, 1);
	vector< vector<double> > res = ms.measure( D, r, c );

	assert( res.size()==46 );
	assert( res[0].size()==SPLIT_AREA_NUM+1 );	// just for aver_split
	
	//disp(res);

	cout << "All Unit Cases Passed." << endl;
	return 0;
}

#endif
