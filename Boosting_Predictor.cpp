/**********************************************************************
	> File Name: Boosting_Predictor.cpp
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Wed 21 Aug 2013 07:41:02 PM CST
 *********************************************************************/

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
