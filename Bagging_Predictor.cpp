/**********************************************************************
	> File Name: Bagging_Predictor.cpp
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Thu 08 Aug 2013 08:01:34 PM CST
 *********************************************************************/

#define _BAGGING_PREDICTOR_UTEST_


#include "Bagging_Predictor.h"
#include "BG_Data.cpp"


Bagging_Predictor::Bagging_Predictor( int bagNum, int maxH )
{
	this->bagNum = bagNum;
	this->maxH = maxH;
	this->baserPtrV.clear();
}
Bagging_Predictor::~Bagging_Predictor()
{}

void Bagging_Predictor::saveModel( const string& fNM ) const
{
}

void Bagging_Predictor::dispModel() const
{
	for ( int i = 0; i < this->bagNum; i++ ) {
		this->baserPtrV[i]->dispModel();
	}
}

void Bagging_Predictor::train( BG_Data& D )
{
	assert( this->baserPtrV.size()==0 );

	this->baserPtrV.resize( this->bagNum );
	for ( int i = 0; i < this->bagNum; i++ ) {
		D.reshuffling();
		this->baserPtrV[i] = new CART_Predictor( this->maxH );
		this->baserPtrV[i]->train( D, D.getCs(), D.getFs() );
	}
}

//void Bagging_Predictor::train( TR_Data& D, const vector<int>& cs, const vector<int>& fs );

//vector<double> Bagging_Predictor::predict( const Data& T ) const;
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



#ifdef _BAGGING_PREDICTOR_UTEST_

void test1()
{

#define _TEST_1_1_
#define _TEST_1_2_
#define _TEST_1_3_
#define _TEST_1_4_
#define _TEST_1_5_
	
	BG_Data D;
	D.fmtread( "test/case1.fmt" );
	Data T;
	T.csvread( "test/case1.csv" );

	Bagging_Predictor bg( 3 );
	bg.train( D );

	vector<double> p;
	
	bg.dispModel();

	//p = bg.predict( T );
	//disp( p );
}


int main()
{
	return 0;
}

#endif
