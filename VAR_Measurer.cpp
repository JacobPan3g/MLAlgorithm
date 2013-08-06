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


MS VAR_Measurer::measure( const TR_Data &D, const vector<int> &cs, const vector<int> &fs )
{
	int m = D.getM();
	int n = D.getN();
	vector<double> L = D.getL();
	vector< list< pair<int,double> > > fmtV = D.getFmtV();
	assert( cs.size()==m );
	assert( fs.size()==n );

	this->getSplitPoints( fmtV, m, n, cs );

	int minIdx = -1;
	double minObVal = 0;
	double minVar = 1e8;

	for ( int i = 0; i < n; i++ )	
	{
		if ( !fs[i] )
			continue;
		
		// Binary Search
		int l_idx = 0;
		int r_idx = this->sp[i].size() - 1;
		double VARl, VARr;
		while ( r_idx - l_idx > 1 ) {

			VARl = this->computeVAR( i, l_idx, fmtV[i], L, m, n, cs );
			VARr = this->computeVAR( i, r_idx, fmtV[i], L, m, n, cs );

			if ( VARl < VARr ) {
				r_idx = (l_idx + r_idx) / 2;
			}
			else {
				l_idx = (l_idx + r_idx) / 2;
			}
		}
		double var;
		if ( l_idx != r_idx ) {
			VARl = this->computeVAR( i, l_idx, fmtV[i], L, m, n, cs );
			VARr = this->computeVAR( i, r_idx, fmtV[i], L, m, n, cs );
			var = VARl < VARr? VARl : VARr;
			r_idx = VARl < VARr? l_idx : r_idx;
		}
		else {
			var = this->computeVAR( i, r_idx, fmtV[i], L, m, n, cs );
		}

		if ( var == 0 ) {
			return MS(i, this->sp[i][r_idx], var);	
		}

		if ( var < minVar ) {
			minIdx = i;
			minObVal = this->sp[i][r_idx];	
			minVar = var;
		}
	}
	return MS(minIdx,minObVal,minVar);
}

double VAR_Measurer::computeVAR( int i, int k, const list< pair<int,double> >& fmt, const vector<double>& L, int m, int n, vector<int> cs )
{	
	// desperate into two parts
	vector<int> part1;
	vector<int> part2;
	int num1 = 0;
	int num2 = 0;
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

// getter
vector< vector<int> > VAR_Measurer::getIdxs() const
{
	return this->idxs;
}

vector< vector<double> > VAR_Measurer::getSp() const
{
	return this->sp;
}

// Private Method
void VAR_Measurer::getSplitPoints( const vector< list< pair<int,double> > >& fmtV, int m, int n, vector<int> cs )
{
	this->sp.resize( n );
	this->idxs.resize( n );
	for ( int i = 0; i < n; i++ ) {
		this->sp[i] = vector<double>( m );
		this->idxs[i] = vector<int>( m );
		bool isFirst = true;
		double beforeItem;

		list< pair<int,double> >::const_iterator it = fmtV[i].begin();
		int num = 0;
		int idx = 0;
		for ( ; it != fmtV[i].end(); it++, idx++ ) {
			if ( cs[it->first] ) {
				if ( isFirst ) {
					isFirst = false;
					beforeItem = it->second;
					this->sp[i][num] = it->second;
					num++;
				}
				else {
					if ( beforeItem != it->second ) {
						this->idxs[i][num-1] = idx;
						beforeItem = it->second;
						this->sp[i][num] = it->second;
						num++;
					}
				}
			}
		}
		int sz = num==1? num : num-1;
		this->sp[i].resize( sz );
		this->idxs[i].resize( sz );
	}
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
 	assert( res.fIdx==0&&res.obVal==0.5&&res.msVal==0 );
 	
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
}

void test2()
{
#define _TEST_2_1_
#define _TEST_2_2_

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
 	assert( res.fIdx==2&&res.obVal==0&&isEqual(res.msVal,0.13333) );
 	
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
	test1();	//done
	test2();	//done
	
	pro1Test();	//done

	cout << "All Unit Cases Passed." << endl;
	return 0;
}

#endif
