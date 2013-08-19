/**********************************************************************
	> File Name: VAR_Measurer.cpp
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Mon 08 Jul 2013 01:36:58 PM CST
 *********************************************************************/

//#define _VAR_MEASURER_UTEST_


#include "VAR_Measurer.h"
#include "TR_Data.cpp"

#define LEAF_MAX_NUM 1


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

	// get the total value
	this->sums = 0;
	this->sqSums = 0;
	this->nums = 0;
	for ( int i = 0; i < L.size(); i++ ) {
		if ( cs[i] ) {
			this->sums += L[i];
			this->sqSums += pow( L[i], 2 );
			this->nums++;
		}
	}

	this->getSplitPoints( fmtV, L, m, n, cs );

	// calculate the variance
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
		for ( int k = 0; k < sz; k++ ) {
			// for calculation
			double N1 = this->num1s[i][k] + 1e-8;
			double N2 = this->nums - this->num1s[i][k] + 1e-8;
			double sqSum1 = this->sqSums1[i][k];
			double sqSum2 = this->sqSums - this->sqSums1[i][k];
			double sum1 = this->sums1[i][k];
			double sum2 = this->sums - this->sums1[i][k];
			double aver1 = sum1/N1;
			double aver2 = sum2/N2;
			double sqAver1 = pow( aver1, 2 );
			double sqAver2 = pow( aver2, 2 );

			double var1 = sqSum1/N1 - 2*aver1*sum1/N1 + sqAver1;
			double var2 = sqSum2/N2 - 2*aver2*sum2/N2 + sqAver2;
			
			double N = N1 + N2;
			assert( N != 0 );
			double var = N1/N*var1 + N2/N*var2;
			this->vars[i][k] = var;

			if ( var < minVar ) {
				minFIdx = i;
				minSpIdx = k;
				minVar = var;
			}
		}
	}

	// set the result
	this->fIdx = minFIdx;
	this->spIdx = minSpIdx;
	this->obVal = this->sp[minFIdx][minSpIdx];
	this->part1 = vector<int>( m, 0 );
	this->part2 = cs;
	
	list< pair<int,double> >::const_iterator it = fmtV[minFIdx].begin();
	for ( ; it != fmtV[this->fIdx].end(); it++ ) {
		if ( cs[it->first] ) {
			if ( it->second > obVal )
				break;
			this->part1[it->first] = 1;
			this->part2[it->first] = 0;
		}
	}

	// Assert
	assert( countTag(cs)==this->nums );
	assert( countTag(this->part1)==this->getNum1() );
	assert( countTag(this->part2)==this->getNum2() );
	assert( this->fIdx < n );
	assert( this->spIdx < this->sp[this->fIdx].size() );

	return MS(minFIdx,this->obVal,minVar);

//	return MS( 0, 0, 0 );
}

double VAR_Measurer::estimateLabel( const vector<double> &L, const vector<int> &cs )
{
	return mean(L, cs);
}

bool VAR_Measurer::endCondition( const vector<double> &L, vector<int> cs, int num )
{
	return num <= LEAF_MAX_NUM;
}

// own methods
VAR_Measurer::VAR_Measurer()
{
}

// getter
vector< vector<int> > VAR_Measurer::getIdxs() const
{
	return this->idxs;
}

vector< vector<double> > VAR_Measurer::getSp() const
{
	return this->sp;
}

vector< vector<double> > VAR_Measurer::getVars() const
{
	return this->vars;
}


const vector<int>& VAR_Measurer::getPart1() const
{
	return this->part1;
}

const vector<int>& VAR_Measurer::getPart2() const
{
	return this->part2;
}

int VAR_Measurer::getNum1( int i, int k ) const
{
	if ( i==-1&&k==-1 ) {
		i = this->fIdx;
		k = this->spIdx;
	}

	return this->num1s[i][k];
}

int VAR_Measurer::getNum2( int i, int k ) const
{
	if ( i==-1&&k==-1 ) {
		i = this->fIdx;
		k = this->spIdx;
	}

	return this->nums - this->num1s[i][k];
}


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


	this->num1s.resize( n );
	//this->part1s.resize( n );
	//this->part2s.resize( n );
	
	for ( int i = 0; i < n; i++ ) {
		this->sp[i] = vector<double>( m );
		this->idxs[i] = vector<int>( m );
		this->sums1[i] = vector<double>( m, 0 );
		this->sqSums1[i] = vector<double>( m, 0 );

		this->num1s[i] = vector<int>( m, 0 );
		//this->part1s[i].resize( m );
		//this->part2s[i].resize( m );

		bool isFirst = true;
		double beforeItem;		// the front diff Item

		double sum1 = 0;
		double sqSum1 = 0;
		int num1 = 0;	// num of 1st part
		//vector<int> p1( m, 0 );
		//vector<int> p2( cs );

		list< pair<int,double> >::const_iterator it = fmtV[i].begin();
		int num = 0;	// num of sp
		int idx = 0;	// idx at all cases
		for ( ; it != fmtV[i].end(); it++, idx++ ) {
			if ( cs[it->first] ) {
				//num1++;
				if ( isFirst ) {
					isFirst = false;
					beforeItem = it->second;
				}
				else {
					if ( beforeItem != it->second ) {
						
						this->idxs[i][num] = idx;
						this->sp[i][num] = (it->second+beforeItem)/2;
						this->sums1[i][num] = sum1;
						this->sqSums1[i][num] = sqSum1;
						
						this->num1s[i][num] = num1;
						//this->part1s[i][num] = p1;
						//this->part2s[i][num] = p2;

						beforeItem = it->second;
						num++;
					}
				}
				// set the part
				//p1[it->first] = 1;
				//p2[it->first] = 0;
				
				// calculate the sum
				sum1 += L[it->first];
				sqSum1 += pow( L[it->first], 2 );
				num1++;
			}
		}

		if ( num == 0 ) {
			this->idxs[i][num] = idx;
			this->sp[i][num] = fmtV[i].begin()->second;
			this->sums1[i][num] = sum1;
			this->sqSums1[i][num] = sqSum1;
			
			this->num1s[i][num] = num1;
			//this->part1s[i][num] = p1;
			//this->part2s[i][num] = p2;
			
			num = 1;
		}

		// correct the size
		int sz = num;
		this->sp[i].resize( sz );
		this->idxs[i].resize( sz );
		this->sums1[i].resize( sz );
		this->sqSums1[i].resize( sz );
		this->num1s[i].resize( sz );
		//this->part1s[i].resize( sz );
		//this->part2s[i].resize( sz );
	
		// Assert
		int ass_sp_num = this->sp[i].size();
		assert( ass_sp_num <= m );
		assert( ass_sp_num==this->idxs[i].size() );
		assert( ass_sp_num==this->sums1[i].size() );
		assert( ass_sp_num==this->sqSums1[i].size() );
		assert( ass_sp_num==this->num1s[i].size() );
		//assert( ass_sp_num==this->part1s[i].size() );
		//assert( ass_sp_num==this->part2s[i].size() );
		
		srand( (int)time(0) );
		if ( ass_sp_num != 0 ) {
			int ass_idx = rand() % ass_sp_num;
			assert( this->sums1[i][ass_idx] <= this->sums );
			assert( this->sqSums1[i][ass_idx] <= this->sqSums );
			assert( this->num1s[i][ass_idx] <= this->nums );
			assert( this->num1s[i][ass_idx] <= this->idxs[i][ass_idx] );
			//assert( countTag(this->part1s[i][ass_idx])+countTag(this->part2s[i][ass_idx])==this->nums[i] );	
		}
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
#define _TEST_1_4_

	TR_Data D;	
	VAR_Measurer ms;
	MS res;
	vector< vector<int> > idxs;
	vector< vector<double> > sp;
	vector< vector<double> > vars;
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
	int c11v2[] = { 5 };
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
	double c12sp0[] = { 0.25, 0.6, 0.8 };
	double c12sp1[] = { 0.25 };
	double c12sp2[] = { 0.5 };
	double c12sp3[] = { 0.2, 0.45, 0.65, 0.85 };
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
 * Goal: 1. test the calculation of var
 */
	vars.resize( D.getN() );
	double c13vars0[] = { 0.13333, 0, 0.15 };
	double c13vars1[] = { 0 };
	double c13vars2[] = { 0.24 };
	double c13vars3[] = { 0.2, 0.13333, 0, 0.15 };
	vars[0] = vv1( c13vars0, sizeof(c13vars0)/sizeof(double) ); 
	vars[1] = vv1( c13vars1, sizeof(c13vars1)/sizeof(double) ); 
	vars[2] = vv1( c13vars2, sizeof(c13vars2)/sizeof(double) ); 
	vars[3] = vv1( c13vars3, sizeof(c13vars3)/sizeof(double) );
	//disp( ms.getVars() );
	assert( isSame(ms.getVars(),vars) );
#endif

#ifdef _TEST_1_4_
/* Test 1.4
 * Type: accurater
 * Data: all in case1
 * Goal: 1. test the res, num, and part
 */
	// res
	//cout << res.fIdx << " " << res.obVal << " " << res.msVal << endl;
 	assert( res.fIdx==0&&res.obVal==0.6&&isEqual(res.msVal,0) );
	
	// part & num
	int num1 = ms.getNum1();
	int num2 = ms.getNum2();
	vector<int> part1 = ms.getPart1();
	vector<int> part2 = ms.getPart2();
	//cout << num1 << " " << num2 << endl;
	//disp( part1 );
	//disp( part2 );
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
#define _TEST_2_4_

	TR_Data D;	
	VAR_Measurer ms;
	MS res;
	vector< vector<int> > idxs;
	vector< vector<double> > sp;
	vector< vector<double> > vars;
	vector<int> r, c;	
	
	D.fmtread( "test/case2.fmt" );
	ms = VAR_Measurer();
	r = vector<int>( D.getM(), 1 );
	c = vector<int>( D.getN(), 1 );
	res = ms.measure( D, r, c );

#ifdef _TEST_2_1_
/* Test 2.1
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
/* Test 2.2
 * Type: accurater
 * Data: all in case2
 * Goal: 1. test the sp
 */	 
	sp.resize( D.getN() );
	double c12sp0[] = { 0.5, 1.5 };
	double c12sp1[] = { 0.5 };
	double c12sp2[] = { 0.5 };
	double c12sp3[] = { 0.5, 1.5 };
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
 * Goal: 1. test the calculation of var
 */
	vars.resize( D.getN() );
	double c23vars0[] = { 0.22, 0.22 };
	double c23vars1[] = { 0.16 };
	double c23vars2[] = { 0.13333 };
	double c23vars3[] = { 0.16, 0.18182 };
	vars[0] = vv1( c23vars0, sizeof(c23vars0)/sizeof(double) ); 
	vars[1] = vv1( c23vars1, sizeof(c23vars1)/sizeof(double) ); 
	vars[2] = vv1( c23vars2, sizeof(c23vars2)/sizeof(double) ); 
	vars[3] = vv1( c23vars3, sizeof(c23vars3)/sizeof(double) );
	//disp( ms.getVars() );
	assert( isSame(ms.getVars(),vars) );
#endif

#ifdef _TEST_2_4_
/* Test 2.4
 * Type: accurater
 * Data: all in case2
 * Goal: 1. test the res, num and part
 */
	// res
	//cout << res.fIdx << " " << res.obVal << " " << res.msVal << endl;
 	assert( res.fIdx==2&&res.obVal==0.5&&isEqual(res.msVal,0.13333) );
 	// num & part
	int num1 = ms.getNum1();
	int num2 = ms.getNum2();
	vector<int> part1 = ms.getPart1();
	vector<int> part2 = ms.getPart2();
	//cout << num1 << " " << num2 << endl;
	//disp( part1 );
	//disp( part2 );
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
	//cout << res.fIdx << " " << res.obVal << " " << res.msVal << endl;
	assert( res.fIdx==38&&isEqual(res.obVal,0.748239)&&isEqual(res.msVal,0.279294) );

	time_t toc = clock();
	cout << "Time: " << (double)(toc-tic)/CLOCKS_PER_SEC << "s"<< endl;
}

int main()
{
	test1();	//done
	test2();	//done
	
	pro1Test();	//done

	cout << "All Unit Cases Passed." << endl;
	return 0;
}

#endif
