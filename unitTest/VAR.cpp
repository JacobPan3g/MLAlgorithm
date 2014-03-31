#include "../VAR_Measurer.cpp"
using namespace std;

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
	
	D.fmtread( "../test/case1.fmt" );
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
	
	D.fmtread( "../test/case2.fmt" );
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

	D.fmtread("../dataset/pro1.fmt");
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
