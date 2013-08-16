/**********************************************************************
	> File Name: VAR_Measurer.cpp
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Mon 08 Jul 2013 01:36:58 PM CST
 *********************************************************************/

#define _VAR_MEASURER_UTEST_


#include "VAR_Measurer.h"
#include "TR_Data.cpp"

#define LEAF_MAX_NUM 5


/* Function: measure()
 *		-- get the min VAR value for all f
 *	Call:	getSplitPoints(), computeVAR()
 *	Update:	sp, idxs, part1, part2, num1, num2
 *	Return: MS 
 */
MS VAR_Measurer::measure( const TR_Data &D, const vector<int> &cs, const vector<int> &fs )
{
	int m = D.getM();
	int n = D.getN();
	vector<double> L = D.getL();
	vector< list< pair<int,double> > > fmtV = D.getFmtV();
	
	assert( cs.size()==m );
	assert( fs.size()==n );

	this->getSplitPoints( fmtV, L, m, n, cs );
/*
	this->vars.resize( n );
	int minFIdx = -1;
	int minSpIdx = -1;
	double minVar = 1e8;

	for ( int i = 0; i < n; i++ )	
	{
		if ( !fs[i] )
			continue;
	
		int sz = this->sp[i].size();
		this->vars[i] = vector<double>( sz );
		double sum = this->sums[i];
		double aver = this->avers[i];
		for ( int k = 0; k < sz; k++ ) {
			// for calculation
			int N1 = this->num1s[i][k];
			int N2 = this->num2s[i][k];
			double sqSum1 = this->sqSums1[i][k];
			double sqSum2 = this->sqSums[i] - this->sqSums1[i][k];
			double sum1 = this->sums1[i][k];
			double sum2 = this->sums[i] - this->sums1[i][k];
			double aver1 = sum1/N1;
			double aver2 = sum2/N2;
			double sqAver1 = pow( aver1, 2 );
			double sqAver2 = pow( aver2, 2 );

			double var1 = sqSum1/N1 + 2*aver1*sum1/N1 + sqAver1;
			double var2 = sqSum2/N2 + 2*aver2*sum1/N1 + sqAver2;
			
			int N = N1 + N2;
			double var = N1/N*var1 + N2/N*var2;
			var[i][k] = var;

			if ( var < minVar ) {
				minFIdx = i;
				minSpIdx = k;
				minVar = var;
			}
		}
	}

	//cout << countTag(cs) << " " << this->num1 << " " << this->num2 << endl; 
	assert( countTag(cs)==this->num1+this->num2 );
	assert( countTag(this->part1)==this->num1 );
	assert( countTag(this->part2)==this->num2 );
	assert( minFIdx < n );
	assert( minSpIdx < this->sp[minFidx].size() );

	return MS(minFIdx,this->sp[minSpIdx],minVar);
*/
	return MS( 0, 0, 0 );
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
}
/*
// getter
vector< vector<int> > VAR_Measurer::getIdxs() const
{
	return this->idxs;
}

vector< vector<double> > VAR_Measurer::getSp() const
{
	return this->sp;
}

const vector<int>& VAR_Measurer::getPart1() const
{
	return this->part1;
}

const vector<int>& VAR_Measurer::getPart2() const
{
	return this->part2;
}

int VAR_Measurer::getNum1() const
{
	return this->num1;
}

int VAR_Measurer::getNum2() const
{
	return this->num2;
}
*/

// Private Method

/* Function: getSplitPoints()
 *		-- use fmtV to find all sp for all F
 *	Update:	sp, idxs
 *	Return: void 
 */
void VAR_Measurer::getSplitPoints( const vector< list< pair<int,double> > >& fmtV, const vector<double>& L, int m, int n, vector<int> cs )
{
	assert( fmtV.size()==n );
	assert( fmtV[0].size()==m );
	assert( cs.size()==m );

	this->sp.resize( n );
	this->idxs.resize( n );
	this->sums1.resize( n );
	this->sqSums1.resize( n );

	this->sums = vector<double>( n, 0 );
	this->sqSums = vector<double>( n, 0 );

	//this->num1s.resize( n );
	//this->part1s.resize( n );
	//this->part2s.resize( n );
	
	for ( int i = 0; i < n; i++ ) {
		this->sp[i] = vector<double>( m );
		this->idxs[i] = vector<int>( m );
		this->sums1[i] = vector<double>( m, 0 );
		this->sqSums1[i] = vector<double>( m, 0 );
		bool isFirst = true;
		double beforeItem;		// the front diff Item

		double sum = 0;
		double sqSum = 0;

		list< pair<int,double> >::const_iterator it = fmtV[i].begin();
		int num = 0;	// num of sp
		int num1 = 0;	// num of 1st part
		int idx = 0;	// idx at all cases
		for ( ; it != fmtV[i].end(); it++, idx++ ) {
			if ( cs[it->first] ) {
				num1++;
				if ( isFirst ) {
					isFirst = false;
					beforeItem = it->second;
				}
				else {
					if ( beforeItem != it->second ) {
						
						this->idxs[i][num] = idx;
						this->sp[i][num] = (it->second+beforeItem)/2;
						this->sums1[i][num] = sum;
						this->sqSums1[i][num] = sqSum;
						
						//this->num1s[i][num] = num1;


						beforeItem = it->second;
						num++;
					}
				}
				// calculate the sum
				sum += L[it->first];
				sqSum += pow( L[it->first], 2 );
			}
		}
		this->sums[i] = sum;
		this->sqSums[i] = sqSum;

		int sz = num==1? num : num-1;
		this->sp[i].resize( sz );
		this->idxs[i].resize( sz );
	
		assert( this->sp[i].size() <= m );
		assert( this->sp[i].size()==this->idxs[i].size() );
	}
}

/* Function: computeVAR()
 *		-- use a sp to compute the VAR value
 *	Update:	part1, part2, num1, num2
 *	Return: the VAR value 
 */
/*double VAR_Measurer::computeVAR( int i, int k, const list< pair<int,double> >& fmt, const vector<double>& L, int m, int n, vector<int> cs )
{	
	assert( fmt.size()==m );
	assert( cs.size()==m );
	assert( i < n );
	assert( k < this->sp[i].size() );
	assert( this->sp[i].size()==this->idxs[i].size() );
	assert( L.size()==m );

	// desperate into two parts
	this->part1.clear();
	this->part2.clear();
	this->num1 = 0;
	this->num2 = 0;
	int half = this->idxs[i].size() / 2;
	
	if ( k < half )
	{
		part1 = vector<int>( m, 0 );
		part2 = cs;
		int idx = 0;
		list< pair<int,double> >::const_iterator it = fmt.begin();
		for ( ; idx < this->idxs[i][k]; it++, idx++ ) {
			if ( !cs[ it->first ] )
				continue;
			part1[ it->first ] = 1;
			num1++;
			part2[ it->first ] = 0;
		}
		num2 = countTag( part2 );
	}
	else
	{
		part2 = vector<int>( m, 0 );
		part1 = cs;
		int idx = m - this->idxs[i][k];
		list< pair<int,double> >::const_reverse_iterator it = fmt.rbegin();
		for ( ; idx > 0; it++, idx-- ) {
			if ( !cs[ it->first ] )
				continue;
			part2[ it->first ] = 1;
			num2++;
			part1[ it->first ] = 0;
		}
		num1 = countTag( part1 );
	}

	assert( countTag(cs)==num1+num2 );
	assert( countTag(cs)==countTag(part1)+countTag(part2) );
	
	// compute
	double var1 = variance( L, part1 );
	double var2 = variance( L, part2 );
	double num = num1 + num2;
	double var = num1/num * var1 + num2/num * var2;
	return var;
}
*/

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

	TR_Data D;	
	VAR_Measurer ms;
	MS res;
	vector< vector<int> > idxs;
	vector< vector<double> > sp;
	vector<int> r, c;	
	
	D.fmtread( "test/case1.fmt" );
	r = vector<int>( D.getM(), 1 );
	c = vector<int>( D.getN(), 1 );
	res = ms.measure( D, r, c );

#ifdef _TEST_1_1_
/* Test 1.1
 * Type: accurater
 * Data: all in case1
 * Goal: 1. test the idxs
 */
 	
	idxs.resize( D.getN() );
	int c11v0[] = { 2, 3, 4 };
	int c11v1[] = { 3 };
	int c11v2[] = { 0 };
	int c11v3[] = { 1, 2, 3, 4 };
	idxs[0] = vv1( c11v0, sizeof(c11v0)/sizeof(int) );
	idxs[1] = vv1( c11v1, sizeof(c11v1)/sizeof(int) );
	idxs[2] = vv1( c11v2, sizeof(c11v2)/sizeof(int) );
	idxs[3] = vv1( c11v3, sizeof(c11v3)/sizeof(int) );
	//disp( ms.getIdxs() );
	assert( isSame(ms.getIdxs(),idxs) );
#endif

#ifdef _TEST_1_2_
/* Test 1.2
 * Type: accurater
 * Data: all in case1
 * Goal: 1. test the sp
 */	 
	sp.resize( D.getN() );
	double c12sp0[] = { 0, 0.5, 0.7 };
	double c12sp1[] = { 0 };
	double c12sp2[] = { 0.5 };
	double c12sp3[] = { 0, 0.4, 0.5, 0.8 };
	sp[0] = vv1( c12sp0, sizeof(c12sp0)/sizeof(double) ); 
	sp[1] = vv1( c12sp1, sizeof(c12sp1)/sizeof(double) ); 
	sp[2] = vv1( c12sp2, sizeof(c12sp2)/sizeof(double) ); 
	sp[3] = vv1( c12sp3, sizeof(c12sp3)/sizeof(double) );
	//disp( ms.getSp() );
	assert( isSame(ms.getSp(),sp) );
#endif

#ifdef _TEST_1_3_
/* Test 1.3
 * Type: accurater
 * Data: all in case1
 * Goal: 1. test the calculation
 */
	// res
	cout << res.fIdx << " " << res.obVal << " " << res.msVal << endl;
 	assert( res.fIdx==0&&res.obVal==0.5&&res.msVal==0 );
	// part & num
	int num1 = ms.getNum1();
	int num2 = ms.getNum2();
	vector<int> part1 = ms.getPart1();
	vector<int> part2 = ms.getPart2();
	assert( num1==3&num2==2 );
	int p1[] = {1,1,1,0,0};
	int p2[] = {0,0,0,1,1};
	assert( isSame(part1,p1,sizeof(p1)/sizeof(int)) );
	assert( isSame(part2,p2,sizeof(p2)/sizeof(int)) );

#endif
}

void test2()
{
#define _TEST_2_1_
#define _TEST_2_2_
#define _TEST_2_3_

	TR_Data D;	
	VAR_Measurer ms;
	MS res;
	vector< vector<int> > idxs;
	vector< vector<double> > sp;
	vector<int> r, c;	
	
	D.fmtread( "test/case2.fmt" );
	ms = VAR_Measurer();
	r = vector<int>( D.getM(), 1 );
	c = vector<int>( D.getN(), 1 );
	res = ms.measure( D, r, c );

#ifdef _TEST_2_1_
/* Test 1.1
 * Type: accurater
 * Data: all in case2
 * Goal: 1. test the idxs
 */
	idxs.resize( D.getN() );
	int c11v0[] = { 5, 10 };
	int c11v1[] = { 10 };
	int c11v2[] = { 9 };
	int c11v3[] = { 5, 11 };
	idxs[0] = vv1( c11v0, sizeof(c11v0)/sizeof(int) );
	idxs[1] = vv1( c11v1, sizeof(c11v1)/sizeof(int) );
	idxs[2] = vv1( c11v2, sizeof(c11v2)/sizeof(int) );
	idxs[3] = vv1( c11v3, sizeof(c11v3)/sizeof(int) );
	//disp( ms.getIdxs() );
	assert( isSame(ms.getIdxs(),idxs) );
#endif

#ifdef _TEST_2_2_
/* Test 1.2
 * Type: accurater
 * Data: all in case2
 * Goal: 1. test the sp
 */	 
	sp.resize( D.getN() );
	double c12sp0[] = { 0, 1 };
	double c12sp1[] = { 0 };
	double c12sp2[] = { 0 };
	double c12sp3[] = { 0, 1 };
	sp[0] = vv1( c12sp0, sizeof(c12sp0)/sizeof(double) ); 
	sp[1] = vv1( c12sp1, sizeof(c12sp1)/sizeof(double) ); 
	sp[2] = vv1( c12sp2, sizeof(c12sp2)/sizeof(double) ); 
	sp[3] = vv1( c12sp3, sizeof(c12sp3)/sizeof(double) );
	//disp( ms.getSp() );
	assert( isSame(ms.getSp(),sp) );
#endif

#ifdef _TEST_2_3_
/* Test 2.3
 * Type: accurater
 * Data: all in case2
 * Goal: 1. test the calculation
 */
	// res
	cout << res.fIdx << " " << res.obVal << " " << res.msVal << endl;
 	assert( res.fIdx==2&&res.obVal==0&&isEqual(res.msVal,0.13333) );
 	// num & part
	int num1 = ms.getNum1();
	int num2 = ms.getNum2();
	vector<int> part1 = ms.getPart1();
	vector<int> part2 = ms.getPart2();
	assert( num1==9&num2==6 );
	int p1[] = {1,1,1,0,1,1,1,0,0,0,0,0,1,1,1};
	int p2[] = {0,0,0,1,0,0,0,1,1,1,1,1,0,0,0,};
	assert( isSame(part1,p1,sizeof(p1)/sizeof(int)) );
	assert( isSame(part2,p2,sizeof(p2)/sizeof(int)) );

#endif
}

void pro1Test()
{
	time_t tic = clock();

	TR_Data D;
	VAR_Measurer ms;
	MS res;
	vector<int> r, c;	

	D.fmtread("dataset/pro1.fmt");
	r = vector<int>( D.getM(), 1 );
	c =vector<int>( D.getN(), 1 );
	ms = VAR_Measurer();
	res = ms.measure( D, r, c );
	//cout << res.obVal << endl;
	assert( res.fIdx==38&&isEqual(res.obVal,0.604861)&&isEqual(res.msVal,0.280569) );

	time_t toc = clock();
	cout << "Time: " << (double)(toc-tic)/CLOCKS_PER_SEC << "s"<< endl;
}

int main()
{
//	test1();	//done
//	test2();	//done
	
	pro1Test();	//done

	cout << "All Unit Cases Passed." << endl;
	return 0;
}

#endif
