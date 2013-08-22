/**********************************************************************
	> File Name: RGF_Predictor.cpp
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Thu 22 Aug 2013 01:20:09 PM CST
 *********************************************************************/

#ifdef _RGF_PREDICTOR_UTEST_


#include "Boosting_Predictor.cpp"

#include "RGF_Predictor.h"

// Static Method
//vector<double> RGF_Predictor::predict( const MT_Model& mdl, const Data& T );

RGF_Predictor::RFG_Predictor( int maxH=10 )
	: Boosting_Predictor( 0, maxH )
{}

void RGF_Predictor::train( TR_Data& D )
{

}
