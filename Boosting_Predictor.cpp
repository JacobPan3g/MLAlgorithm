/**********************************************************************
	> File Name: Boosting_Predictor.cpp
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Wed 21 Aug 2013 07:41:02 PM CST
 *********************************************************************/

#define _BOOSTING_PREDICTOR_UTEST_

#include "CART_Predictor.cpp"
#include "MT_Model.cpp"

#include "vector_x.cpp"

#include "Boosting_Predictor.h"

// Static Method
vector<double> Boosting_Predictor::predict( const MT_Model& mdl, const Data& T )
{
	const vector<const ST_Model*>& mdlPtrV = mdl.getMdlPtrV();
	int bagNum = mdlPtrV.size();
	vector<double> res( T.getL().size(), 0 );
	for ( int i = 0; i < bagNum; i++ ) {
		res += CART_Predictor::predict( *(mdlPtrV[i]), T );
	}
	return res;
}

Boosting_Predictor::Boosting_Predictor( int bagNum, int maxH )
{
	this->bagNum = bagNum;
	this->maxH = maxH;
	this->baserPtrV.clear();
}

Boosting_Predictor::~Boosting_Predictor()
{
	for ( int i = 0; i < this->baserPtrV.size(); i++ ) {
		delete this->baserPtrV[i];
	}
}

void Boosting_Predictor::saveModel( const string& fNM ) const
{
	this->model.save( fNM );	
}

void Boosting_Predictor::dispModel() const
{
	for ( int i = 0; i < this->bagNum; i++ ) {
		cout << "--Boosting-- " << i << " -----------------" << endl;
		this->baserPtrV[i]->dispModel();
	}
	cout << endl;
}

void Boosting_Predictor::train( TR_Data& D )
{
	assert( this->baserPtrV.size()==0 );

	this->baserPtrV.resize( this->bagNum );

	vector<double> L = D.getL();
	vector<double> p;
	for ( int i = 0; i < this->bagNum; i++ ) {
		this->baserPtrV[i] = new CART_Predictor( this->maxH );
		this->baserPtrV[i]->train( D );

		//this->baserPtrV[i]->dispModel();
		this->model.addModel( this->baserPtrV[i]->getModel() );
		if ( i != this->bagNum - 1 ) {
			p = this->baserPtrV[i]->predict( D );
			L = L - p;
			D.setL( L );
			
			//cout << this->baserPtrV[i]->getRoot()->fIdx << endl;
			//cout << D.getL() << endl;
		}
	}
}

/*********************************************************************
 * Unit Test
 * by Jacob Pan
 *********************************************************************/

#ifdef _BOOSTING_PREDICTOR_UTEST_

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
	
	D.fmtread( "test/case1.fmt" );
	T.csvread( "test/case1.csv" );
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
	bt.saveModel( "model/case1.bmmdl" );
	MT_Model m13( "model/case1.bmmdl" );
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
	MT_Model m15( "model/case1.bmmdl" );

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
	Boosting_Predictor bt( 3, 1 );
	vector<double> p;
	
	D.fmtread( "test/case2.fmt" );
	T.csvread( "test/case2.csv" );
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
	//m12.show();
#endif

#ifdef _TEST_2_3_
/* Test 2.3
 * Goal: 1. test saveModel(): with( save() and load at MT_Model )
 */
	bt.saveModel( "model/case2.bmmdl" );
	MT_Model m13( "model/case2.bmmdl" );
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
	MT_Model m15( "model/case2.bmmdl" );

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
	D.fmtread( "dataset/pro1.fmt" );
#endif

#ifdef _PRO_1_TRAIN_
	Boosting_Predictor bt( bagNum, maxH );
	bt.train( D );
	bt.dispModel();
	bt.saveModel( "model/pro1.bmmdl" );
#endif

#ifdef _PRO_1_PREDICT_
	vector<double> p;
	MT_Model mdl( "model/pro1.bmmdl" );
	p = Boosting_Predictor::predict( mdl, D );
	double RMSE = rmse( p, D.getL() );
	//disp( p );
	cout << RMSE << endl;
	//assert( RMSE < 0.5 );
#endif

	time_t toc = clock();
	cout << "Time: " << (double)(toc-tic)/CLOCKS_PER_SEC << "s"<< endl;
}

int main()
{
	test1();	//done
	test2();	//done

	pro1Test( 4 );

	cout << "All Unit Cases Passed." << endl;
	return 0;
}

#endif
