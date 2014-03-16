/**********************************************************************
	> File Name: RGF_Predictor.h
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Thu 22 Aug 2013 01:14:45 PM CST
 *********************************************************************/

#pragma once

class RGF_Predictor
{
public:
	// Static Method
	static vector<double> predict( const MT_Model& mdl, const Data& T );

	RFG_Predictor( int maxH=10 );
	
	void train( TR_Data& D );

private:
	int maxH;

	TR_Data D;
	CARTx_Predictor* pdPtr;
};

