/*************************************************************************
	> File Name: VAR.cpp
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Mon 08 Jul 2013 01:36:58 PM CST
 ************************************************************************/

#define _VAR_UTEST_


#include "VAR.h"
#include "CsvData.cpp"

#define LEAF_MAX_NUM 5

VAR::VAR()
{
	this->SPLIT_AREA_NUM = -1;
}

VAR::VAR( int saNum )
{
	this->SPLIT_AREA_NUM = saNum;
}

vector<double> VAR::getSplitPoints( const vector<double> &v )
{
	// Average Spilt the Area [0,1]
	if ( this->SPLIT_AREA_NUM != -1 )
	{
		vector<double> sp(SPLIT_AREA_NUM+1);
		double step = 1.0/SPLIT_AREA_NUM;
		double tmp = 0;
		for ( int i = 0; i < sp.size(); i++ )
			sp[i] = i * step;
		return sp;
	}
	// Accurater
	else
		return vv1( v );
}

double VAR::getSpByValueIdx( int x )
{
	return 0 + x * 1.0/SPLIT_AREA_NUM;
}

// some col tagged 0 will sort as a zero-size vector
vector< vector<double> > VAR::measure( const CsvData &D, const vector<int> &r, const vector<int> &c )
{
	vector< vector<double> > vars( D.n );
	for ( int i = 0; i < D.n; i++ )	
	{
		if ( !c[i] )
			continue;
			
		vector<double> tmp;
		vector<double> f = D.getFeatures(i, r);	// have deal with tag
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

			disp( D.L, part1 );
			disp( D.L, part2 );

			double var1 = variance( D.L, part1 );
			double var2 = variance( D.L, part2 );
			double num = num1 + num2;
			double var = num1/num * var1 + num2/num * var2;
			tmp.push_back( var );
		}
		vars[i] = tmp;
	}

	return vars;
}


double VAR::estimateLabel( const vector<double> &L, const vector<int> &tag )
{
	return mean(L, tag);
}

bool VAR::endCondition( vector<double> v, vector<int> tag, int num )
{
	return num < LEAF_MAX_NUM;
}


#ifdef _VAR_UTEST_

void test2()
{
	CsvData D;
	VAR ms;
	vector< vector<double> > res;
	vector<int> r, c;	
}

int main()
{
	CsvData D;
	VAR ms;
	vector< vector<double> > res;
	vector<int> r, c;	
/*
	// Test Case 1.1
	D.csvread( "test/case1.csv" );
	r.resize( D.m, 1 );
	c.resize( D.n, 1 );
	res = ms.measure( D, r, c );
	//disp(res);
	vector< vector<double> > var = v2( 4, 3, 0.24 );
	var[0][0] = 0.13333; var[0][1] = 0;
	var[1][0] = 0;
	var[3][0] = 0.2; var[3][1] = 0;
	//disp(var);

	assert( isSame( res, var ) );

	// Test Case 1.2
	r[4]=0;
	c[1]=0;
	res=ms.measure( D, r, c );
	disp( res );
	var = v2( 4, 3, 0.1875 );
	var[0][0] = 0.125; var[0][1] = 0;
	var[1].resize(0);
	var[3][0] = 0.166667; var[3][1] = 0;
	disp( var );

	assert( isSame( res, var ) );
	

	// Live Test
	D.csvread("dataset/pro1.csv");
	r.resize( D.m, 1 );
	c.resize( D.n, 1 );
	ms = VAR();
	res = ms.measure( D, r, c );

	assert( res.size()==46 );
	
	//disp(res);

//	D.csvread( "dataset/pro1.csv" );
//	r.resize( D.m, 1 );
//	c.resize( D.n, 1 ); c[38]=0;c[33]=0;c[17]=1;c[21]=0;
//	ms = VAR();
*/

	// Test Case 2.1
	D.csvread( "test/case2.csv" );
	r.resize( D.m, 1 );
	c.resize( D.n, 1 );
	res = ms.measure( D, r, c );
	disp(res);

	vector< vector<int> >  
	


	cout << "All Unit Cases Passed." << endl;
	return 0;
}

#endif
