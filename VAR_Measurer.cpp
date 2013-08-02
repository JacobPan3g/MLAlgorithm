/**********************************************************************
	> File Name: VAR_Measurer.cpp
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Mon 08 Jul 2013 01:36:58 PM CST
 *********************************************************************/

#define _VAR_MEASURER_UTEST_


#include "VAR_Measurer.h"
#include "Data.cpp"

#define LEAF_MAX_NUM 5


// some col tagged 0 will sort as a zero-size vector
// Same as the averager and accurater
//	just differ from:	getSplitPoints()
//						getSpByValueIdx()
vector< vector<double> > VAR_Measurer::measure( const Data &D, const vector<int> &cs, const vector<int> &fs )
{
	int m = D.getM();
	int n = D.getN();
	vector<double> L = D.getL();
	assert( cs.size()==m );
	assert( fs.size()==n );

	this->sp = vector< vector<double> >( n );
	vector< vector<double> > vars( n );
	for ( int i = 0; i < n; i++ )	
	{
		if ( !fs[i] )
			continue;
			
		vector<double> tmp;
		vector<double> f = D.getFeatures(i);
		this->sp[i] = getSplitPoints( f, cs );		// update the sp

		for ( int k = 0; k < this->sp[i].size(); k++ )
		{
			// desperate into two parts
			vector<int> part1( m, 0 );
			vector<int> part2( m, 0 );
			int num1 = 0;
			int num2 = 0;
			for ( int j = 0; j < m; j++ )
			{
				if ( !cs[j] )
					continue;

				if ( f[j] <= this->sp[i][k] )
				{
					part1[j] = 1;	// reset the other side
					num1++;
				}
				else
				{
					part2[j] = 1;
					num2++;
				}
			}

			assert( countTag(cs)==num1+num2 );
			assert( countTag(cs)==countTag(part1)+countTag(part2) );

			//disp( L, part1 );
			//disp( L, part2 );

			double var1 = variance( L, part1 );
			double var2 = variance( L, part2 );
			double num = num1 + num2;
			double var = num1/num * var1 + num2/num * var2;
			tmp.push_back( var );
		}
		vars[i] = tmp;
	}

	assert( vars.size()==n );
	return vars;
}

double VAR_Measurer::estimateLabel( const vector<double> &L, const vector<int> &cs )
{
	return mean(L, cs);
}

bool VAR_Measurer::endCondition( const vector<double> &L, vector<int> cs, int num )
{
	return num < LEAF_MAX_NUM;
}

// own methods
VAR_Measurer::VAR_Measurer()
{
	this->SPLIT_AREA_NUM = -1;
}

VAR_Measurer::VAR_Measurer( int saNum )
{
	this->SPLIT_AREA_NUM = saNum;
}

vector<double> VAR_Measurer::getSplitPoints( const vector<double> &L, vector<int> cs )
{
	// Average Spilt the Area [0,1]
	if ( this->SPLIT_AREA_NUM != -1 )
	{
		vector<double> sp(SPLIT_AREA_NUM);
		double step = 1.0/SPLIT_AREA_NUM;
		for ( int i = 0; i < sp.size(); i++ ) // ignore the last one
			sp[i] = i * step;
		return sp;
	}
	// Accurater
	else
	{
		if ( cs.size() == 0 )
			cs.resize( L.size(), 1 );

		vector<double> tmp = vv1( L, cs );
		if ( tmp.size() != 1 )
			tmp.erase( tmp.end()-1 );		// erase the last one
		return tmp;
	}
}

double VAR_Measurer::getSpByValueIdx( int idx1, int idx2 )
{
	return this->sp[idx1][idx2];
}

// getter
int VAR_Measurer::getSPLIT_AREA_NUM() const
{
	return this->SPLIT_AREA_NUM;
}

vector< vector<double> > VAR_Measurer::getSp() const
{
	return this->sp;
}



/*********************************************************************
 * Unit Test
 * by Jacob Pan
 *********************************************************************/

#ifdef _VAR_MEASURER_UTEST_

void test1()
{

#define _TEST_1_1_
#define _TEST_1_2_
#define _TEST_1_3_
	
	Data D;
	VAR_Measurer ms;
	vector< vector<double> > res;
	vector< vector<double> > var;
	vector< vector<double> > sp;
	vector<int> r, c;	
	D.csvread( "test/case1.csv" );

#ifdef _TEST_1_1_
/* Test 1.1
 * Type: averager
 * Data: all in case1.csv
 * Goal: 1. test the calculation
 *		 2. test the split point
 */
	ms = VAR_Measurer(2);
	r = vector<int>( D.getM(), 1 );
	c = vector<int>( D.getN(), 1 );
	res = ms.measure( D, r, c );
	
	// test the calculation
	var = vv2( 4, 2, 0.24 );
	var[0][0] = 0.13333; var[0][1] = 0;
	var[1][0] = 0;
	var[3][0] = 0.2; var[3][1] = 0;
	assert( isSame( res, var ) );

	// test the split point
	sp = vv2( 4, 2, 0.0 );
	for ( int i = 0; i < 4; i++ )
		sp[i][1] = 0.5;
	assert( isSame( ms.getSp(), sp ) );
#endif

#ifdef _TEST_1_2_
/* Test 1.2
 * Type: averager
 * Data: part of case1.csv	| with r[4]=0, c[1]=0
 * Goal: 1. test the calculation
 *		 2. test the tag
 */
	ms = VAR_Measurer(2);
	r = vector<int>( D.getM(), 1 );
	c = vector<int>( D.getN(), 1 );
	r[4]=0;
	c[1]=0;
	res = ms.measure( D, r, c );

	var = vv2( 4, 2, 0.1875 );
	var[0][0] = 0.125; var[0][1] = 0;
	var[1].resize(0);
	var[3][0] = 0.166667; var[3][1] = 0;
	assert( isSame( res, var ) );
#endif

#ifdef _TEST_1_3_
/* Test 1.3
 * Type: accurater
 * Data: all in case1.csv
 * Goal: 1. test the calculation
 *		 2. test the sp
 */
	ms = VAR_Measurer();
	r = vector<int>( D.getM(), 1 );
	c = vector<int>( D.getN(), 1 );
	res = ms.measure( D, r, c );

	// test the calculation
	var.resize( D.getN() );
	double c13v0[] = { 0.13333, 0, 0.15 };
	double c13v1[] = { 0 };
	double c13v2[] = { 0.24 };
	double c13v3[] = { 0.2, 0.13333, 0, 0.15 };
	var[0] = vv1( c13v0, sizeof(c13v0)/sizeof(double) );
	var[1] = vv1( c13v1, sizeof(c13v1)/sizeof(double) );
	var[2] = vv1( c13v2, sizeof(c13v2)/sizeof(double) );
	var[3] = vv1( c13v3, sizeof(c13v3)/sizeof(double) );
	assert( isSame(res,var) );

	// test the sp
	sp.resize( D.getN() );
	double c13sp0[] = { 0, 0.5, 0.7 };
	double c13sp1[] = { 0 };
	double c13sp2[] = { 0.5 };
	double c13sp3[] = { 0, 0.4, 0.5, 0.8 };
	sp[0] = vv1( c13sp0, sizeof(c13sp0)/sizeof(double) ); 
	sp[1] = vv1( c13sp1, sizeof(c13sp1)/sizeof(double) ); 
	sp[2] = vv1( c13sp2, sizeof(c13sp2)/sizeof(double) ); 
	sp[3] = vv1( c13sp3, sizeof(c13sp3)/sizeof(double) );
	assert( isSame(ms.getSp(),sp) );
#endif
}

// accurater
void test2()
{

#define _TEST_2_1_
#define _TEST_2_2_

	Data D;
	VAR_Measurer ms;
	vector< vector<double> > res;
	vector< vector<double> > var;
	vector< vector<double> > sp;
	vector<int> r, c;		
	D.csvread( "test/case2.csv" );
	int m = D.getM();
	int n = D.getN();

#ifdef _TEST_2_1_
/* Test 2.1
 * Type: accurater
 * Data: all of case2.csv
 * Goal: 1. calculation
 *		 2. sp
 */
	r = vector<int>( m, 1 );
	c = vector<int>( n, 1 );
	res = ms.measure( D, r, c );

	// test the calculation
	var.resize( n );
	double tmp0[] = { 0.22, 0.22 };
	double tmp1[] = { 0.16 };
	double tmp2[] = { 0.13333 };
	double tmp3[] = { 0.16, 0.18182 };
	var[0] = vv1( tmp0, 2 );
	var[1] = vv1( tmp1, 1 );
	var[2] = vv1( tmp2, 1 );
	var[3] = vv1( tmp3, 2 );
	assert( isSame( res, var ) );

	// test the sp
	sp.resize( n );
	double c1sp0[] = { 0, 1 };
	double c1sp1[] = { 0 };
	double c1sp2[] = { 0 };
	double c1sp3[] = { 0, 1 };
	sp[0] = vv1( c1sp0, sizeof(c1sp0)/sizeof(double) );
	sp[1] = vv1( c1sp1, sizeof(c1sp1)/sizeof(double) );
	sp[2] = vv1( c1sp2, sizeof(c1sp2)/sizeof(double) );
	sp[3] = vv1( c1sp3, sizeof(c1sp3)/sizeof(double) );
	assert( isSame( ms.getSp(), sp ) );
#endif

#ifdef _TEST_2_2_
/* Test 2.2
 * Type: accurater
 * Data: part of case2.csv | with r[3,7,8,9,10,11]=0, c[2]=0
 * Goal: 1. calculation
 *		 2. sp
 */
	r = vector<int>( m, 1 );
	c = vector<int>( n, 1 );
	int tmp[] = { 3, 7, 8, 9, 10, 11 };
	resetTag( r, tmp, sizeof(tmp)/sizeof(int) );
	c[2] = 0;
	res = ms.measure( D, r, c );

	// test the calculation
	var.resize( n );
	double tmp20[] = { 0.21667, 0.16667 };
	double tmp21[] = { 0 };
	double tmp23[] = { 0.13333, 0.16667 };
	var[0] = vv1( tmp20, 2 );
	var[1] = vv1( tmp21, 1 );
	var[2].resize(0);
	var[3] = vv1( tmp23, 2 );
	assert( isSame( res, var ) );

	// test the sp
	sp.resize( n );
	double c2sp0[] = { 0, 1 };
	double c2sp1[] = { 0 };
	double c2sp3[] = { 0, 1 };
	sp[0] = vv1( c2sp0, sizeof(c2sp0)/sizeof(double) );
	sp[1] = vv1( c2sp1, sizeof(c2sp1)/sizeof(double) );
	sp[2].resize(0);
	sp[3] = vv1( c2sp3, sizeof(c2sp3)/sizeof(double) );
	assert( isSame( ms.getSp(), sp ) );
#endif
}

void liveTest()
{
	time_t tic = clock();

	Data D;
	VAR_Measurer ms;
	vector< vector<double> > res;
	vector<int> r, c;	

	D.csvread("dataset/pro1.csv");
	r = vector<int>( D.getM(), 1 );
	c =vector<int>( D.getN(), 1 );
	ms = VAR_Measurer();
	res = ms.measure( D, r, c );
//	disp( res );

	assert( res.size()==46 );

	time_t toc = clock();
	cout << "Time: " << (double)(toc-tic)/CLOCKS_PER_SEC << "s"<< endl;
}

int main()
{
	test1();	// done
	test2();	// done
	liveTest();

	cout << "All Unit Cases Passed." << endl;
	return 0;
}

#endif
