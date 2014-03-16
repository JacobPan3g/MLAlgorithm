/**********************************************************************
	> File Name: CARTx_Predictor.h
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Thu 22 Aug 2013 02:36:05 PM CST
 *********************************************************************/

#pragma once


class CARTx_Predictor//: public Predictor
{

public:
	// Friends Method
	friend ostream& operator<<( ostream& os, const CARTx_Predictor& pd );

	// Static Method
	static vector<double> predict( const ST_Model& mdl, const Data& T );
	
	// Own Method
	CARTx_Predictor( const TR_Data& D, int maxH=10 );
	~CARTx_Predictor();
	void train( const TR_Data& D );
	void train_forRoot();
	void train_forNext();

private:
	Node *root;
	int maxH;
	int high;

	TR_Data &D;
	ST_Model model;
};
