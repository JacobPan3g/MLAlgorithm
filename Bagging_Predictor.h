/**********************************************************************
	> File Name: Bagging_Predictor.h
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Thu 08 Aug 2013 08:01:25 PM CST
 *********************************************************************/

#pragma once


//#include "BG_Data.h"
//#include "CART_Predictor.h"
//#include "MT_Model.h"

class Bagging_Predictor
{
public:
	Bagging_Predictor( int bagNum, int maxH=10 );
	Bagging_Predictor( const MT_Model& mdl );	
	~Bagging_Predictor();

	void saveModel( const string& fNM ) const;
	void dispModel() const;
	
	void train( BG_Data& D );
	void train( BG_Data& D, const vector<int>& cs, const vector<int>& fs );

	vector<double> predict( const Data& T ) const;
	double predict( const vector<double>& a ) const;
	
	// use in predict part
	vector<double> predict( const MT_Model& mdl, const Data& T ) const;
	double predict( const MT_Model& mdl, const vector<double>& a ) const;

	// getter
	int getBagNum() const;
	int getMaxH() const;
	const vector<CART_Predictor*> getBaserPtrV() const;
	const MT_Model& getModel() const;

private:
	int bagNum;
	int maxH;
	vector<CART_Predictor*> baserPtrV;

	MT_Model model;
};

