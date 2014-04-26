#include <mpi.h>
#include "../Boosting_Predictor.cpp"

void test1()
{

#define _TEST_1_0_
#define _TEST_1_1_
#define _TEST_1_2_
#define _TEST_1_3_
//#define _TEST_1_4_	// not
#define _TEST_1_5_
	
#ifdef _TEST_1_0_
	TR_Data D;
	Data T;
	Boosting_Predictor bt( 3 );
	vector<double> p;
	
	D.fmtread( "../test/case1.fmt" );
	T.csvread( "../test/case1.csv" );
	bt.train( D );
#endif

#ifdef _TEST_1_1_
/* Test 1.1
 * Goal: 1. train
 */
	//bt.dispModel();
#endif

#ifdef _TEST_1_2_
/* Test 1.2
 * Goal: 1. test model
 */
	MT_Model m12 = bt.getModel();
	//m12.show();
#endif

#ifdef _TEST_1_3_
/* Test 1.3
 * Goal: 1. test saveModel(): with( save() and load at MT_Model )
 */
	bt.saveModel( "../model/case1.bmmdl" );
	MT_Model m13( "../model/case1.bmmdl" );
	//m13.show();

	assert( bt.getModel()==m13 );
#endif

#ifdef _TEST_1_4_
/* Test 1.4
 * Goal: 1. predict after train
 */
	p = bg.predict( T );
	//disp( p );
	//cout << rmse(p,T.getL()) << endl;
	assert( rmse(p,T.getL())<0.3 );
#endif

#ifdef _TEST_1_5_
/* Test 1.5
 * Goal: 1. predict away train
 */
	MT_Model m15( "../model/case1.bmmdl" );

	p = Boosting_Predictor::predict( m15, T );
	//disp( p );
	//cout << rmse( p, T.getL() ) << endl;

	assert( rmse(p,T.getL())==0 );
#endif
}

void test2()
{

#define _TEST_2_0_
#define _TEST_2_1_
#define _TEST_2_2_
#define _TEST_2_3_
//#define _TEST_2_4_	// not
#define _TEST_2_5_
	
#ifdef _TEST_2_0_
	TR_Data D;
	Data T;
	Boosting_Predictor bt( 1 );
	vector<double> p;
	
	D.fmtread( "../test/case2.fmt" );
	T.csvread( "../test/case2.csv" );
	bt.train( D );
#endif

#ifdef _TEST_2_1_
/* Test 2.1
 * Goal: 1. train
 */
	//bt.dispModel();
#endif

#ifdef _TEST_2_2_
/* Test 2.2
 * Goal: 1. test model
 */
	MT_Model m12 = bt.getModel();
	bt.dispModel();
	//m12.show();
#endif

#ifdef _TEST_2_3_
/* Test 2.3
 * Goal: 1. test saveModel(): with( save() and load at MT_Model )
 */
	bt.saveModel( "../model/case2.bmmdl" );
	MT_Model m13( "../model/case2.bmmdl" );
	//m13.show();

	assert( bt.getModel()==m13 );
#endif

#ifdef _TEST_2_4_
/* Test 2.4
 * Goal: 1. predict after train
 */
	p = bt.predict( T );
	//disp( p );
	//cout << rmse(p,T.getL()) << endl;
	assert( rmse(p,T.getL())<0.3 );
#endif

#ifdef _TEST_2_5_
/* Test 2.5
 * Goal: 1. predict away train
 */
	MT_Model m15( "../model/case2.bmmdl" );

	p = Boosting_Predictor::predict( m15, T );
	//disp( p );
	//cout << rmse( p, T.getL() ) << endl;

	assert( rmse(p,T.getL())<0.19 );
#endif
}

void pro1Test( int bagNum, int maxH=10 )
{
#define _PRO_1_DATAREAD_
#define _PRO_1_TRAIN_
#define _PRO_1_PREDICT_

	time_t tic = clock();

#ifdef _PRO_1_DATAREAD_
	TR_Data D;
	D.fmtread( "../dataset/pro1.fmt" );
#endif

#ifdef _PRO_1_TRAIN_
	Boosting_Predictor bt( bagNum, maxH );
	bt.train( D );
	//bt.dispModel();
	bt.saveModel( "../model/pro1.bmmdl" );
#endif

#ifdef _PRO_1_PREDICT_
	vector<double> p;
	MT_Model mdl( "../model/pro1.bmmdl" );
	p = Boosting_Predictor::predict( mdl, D );
	double RMSE = rmse( p, D.getL() );
	//disp( p );
	cout << RMSE << endl;
	//assert( RMSE < 0.5 );
#endif

	time_t toc = clock();
	cout << "Time: " << (double)(toc-tic)/CLOCKS_PER_SEC << "s"<< endl;
}

int main( int argc, char* argv[] )
{
	const int master = 0;

	int rank, numprocs;

	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size( MPI_COMM_WORLD, &numprocs );

	if ( master == rank ) {		
		//test1();
		//test2();
		pro1Test( 1 );
		cout << "All Unit Cases Passed." << endl;

		VAR_Measurer::MPI_stopSlaveThread();
	}
	else {
		VAR_Measurer::MPI_slaveThread();
	}


	MPI_Finalize();
	return 0;
}
