/**********************************************************************
	> File Name: Bagging_Predictor.cpp
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Thu 08 Aug 2013 08:01:34 PM CST
 *********************************************************************/

#define _BAGGING_PREDICTOR_UTEST_


#include "CART_Predictor.cpp"
#include "BG_Data.cpp"
#include "MT_Model.cpp"

#include "Bagging_Predictor.h"

Bagging_Predictor::Bagging_Predictor( int bagNum, int maxH )
{
	this->bagNum = bagNum;
	this->maxH = maxH;
	this->baserPtrV.clear();
}

Bagging_Predictor::~Bagging_Predictor()
{
	for ( int i = 0; i < this->baserPtrV.size(); i++ ) {
		delete this->baserPtrV[i];
	}
}

void Bagging_Predictor::saveModel( const string& fNM ) const
{
	this->model.save( fNM );	
}

void Bagging_Predictor::dispModel() const
{
	for ( int i = 0; i < this->bagNum; i++ ) {
		cout << "--Bagging-- " << i << " -----------------" << endl;
		this->baserPtrV[i]->dispModel();
	}
	cout << endl;
}

void Bagging_Predictor::train( BG_Data& D )
{
	assert( this->baserPtrV.size()==0 );
	assert( countTag(D.getCs())==D.getM() );
	assert( countTag(D.getFs())==D.getN() );

	//cout << D.getCs().size() << endl;
	//disp( D.getCs() );

	this->baserPtrV.resize( this->bagNum );
	for ( int i = 0; i < this->bagNum; i++ ) {
		D.reshuffling();
		this->baserPtrV[i] = new CART_Predictor( this->maxH );
		this->baserPtrV[i]->train( D, D.getCs(), D.getFs() );
		
		//this->baserPtrV[i]->dispModel();
		this->model.addModel( this->baserPtrV[i]->getModel() );
	}
}

//void Bagging_Predictor::train( TR_Data& D, const vector<int>& cs, const vector<int>& fs );

vector<double> Bagging_Predictor::predict( const Data& T ) const
{
	assert( this->bagNum!=0 );
	vector< vector<double> > res( this->bagNum );
	for ( int i = 0; i < this->bagNum; i++ ) {
		res[i] = this->baserPtrV[i]->predict( T );
	}
	return meanR( res );
}

//double Bagging_Predictor::predict( const vector<double>& a ) const;

// use in predict part
//vector<double> Bagging_Predictor::predict( const ST_Model& mdl, const Data& T ) const;
//double Bagging_Predictor::predict( const ST_Model& mdl, const vector<double>& a ) const;

// getter
int Bagging_Predictor::getBagNum() const
{
	return this->bagNum;
}

int Bagging_Predictor::getMaxH() const
{
	return this->maxH;
}

const vector<CART_Predictor*> Bagging_Predictor::getBaserPtrV() const
{
	return this->baserPtrV;
}

const MT_Model& Bagging_Predictor::getModel() const
{
	return this->model;
}


/*********************************************************************
 * Unit Test
 * by Jacob Pan
 *********************************************************************/

#ifdef _BAGGING_PREDICTOR_UTEST_

void test1()
{

#define _TEST_1_1_
#define _TEST_1_2_
#define _TEST_1_3_
#define _TEST_1_4_
#define _TEST_1_5_
	
	BG_Data D;
	Data T;
	Bagging_Predictor bg( 3 );
	vector<double> p;
	
	D.fmtread( "test/case1.fmt" );
	T.csvread( "test/case1.csv" );
	bg.train( D );

#ifdef _TEST_1_1_
/* Test 1.1
 * Goal: 1. train
 */
	//bg.dispModel();
#endif

#ifdef _TEST_1_2_
/* Test 1.2
 * Goal: 1. test model
 */
	MT_Model m12 = bg.getModel();
	m12.show();
#endif

#ifdef _TEST_1_3_
/* Test 1.3
 * Goal: 1. test saveModel()
 */
	bg.saveModel( "model/case1.mmdl" );
	MT_Model m13( "model/case1.mmdl" );
	m13.show();
/*
	int m13f[] = { 0, -1, -1 };
	int m13l[] = { 1, -1, -1 };
	int m13r[] = { 2, -1, -1 };
	double m13o[] = { 0.6, 1, 2 };
	
	assert( isSame(m13.getFIdxV(),m13f,sizeof(m13f)/sizeof(int)) );
	assert( isSame(m13.getLeftV(),m13l,sizeof(m13l)/sizeof(int)) );
	assert( isSame(m13.getRightV(),m13r,sizeof(m13r)/sizeof(int)) );
	assert( isSame(m13.getObValV(),m13o,sizeof(m13o)/sizeof(double)) );
*/
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
}


void test2()
{

#define _TEST_2_1_
#define _TEST_2_2_
#define _TEST_2_3_
#define _TEST_2_4_
#define _TEST_2_5_
	
	BG_Data D;
	Data T;
	Bagging_Predictor bg( 3 );
	vector<double> p;
	
	D.fmtread( "test/case2.fmt" );
	T.csvread( "test/case2.csv" );
	bg.train( D );

#ifdef _TEST_2_1_
/* Test 2.1
 * Goal: 1. train
 */
	//bg.dispModel();
#endif

	//p = bg.predict( T );
	//disp( p );
}

int main()
{
	test1();
	test2();

	cout << "All Unit Cases Passed." << endl;
	return 0;
}

#endif
