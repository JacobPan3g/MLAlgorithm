/**********************************************************************
	> File Name: RGF_Predictor.cpp
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Thu 22 Aug 2013 01:20:09 PM CST
 *********************************************************************/

#ifdef _RGF_PREDICTOR_UTEST_


#include "CARTx_Predictor.cpp"

#include "RGF_Predictor.h"

// Static Method
//vector<double> RGF_Predictor::predict( const MT_Model& mdl, const Data& T );

RGF_Predictor::RFG_Predictor( const TR_Data& D, int maxH=10 )
	: D(D), pdPtr(NULL), maxH(maxH) 
{
	this->pdPtr = new CARTx_Predictor( D, maxH );
}

RGF_Predictor::~RFG_Predictor()
{
	delete this->pdPtr;
}

void RGF_Predictor::train( TR_Data& D )
{
	while ( fs,  )

	double rmse1 = this->pdPtr->try_train_step();
	
	double rmse2 = 0;
	vector<double> L = this->D.getL();
	vector<double> p = this->pdPtr->predict( D );
	this->D.setL( L-p );
	this->try_pdPtr->new CARTx_Predictor( D, this->maxH );
	rmse2 = try_predict();

	if ( rmse1 <= rmse2 ) {
		this->pdPtr->do_train_step();
	}
	else {
		model << this->pdPtr;
		this->pdPtr = this->try_pdPtr;
		this->try_pdPtr = NULL;
	}
}
