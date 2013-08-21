/**********************************************************************
	> File Name: Boosting_Predictor.h
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Wed 21 Aug 2013 07:31:40 PM CST
 *********************************************************************/

#include <iostream>
using namespace std;

class Boosting_Predictor
{
public:
	// Static Method
	static vector<double> predict( const MT_Model& mdl, const Data& T );
	
	Boosting_Predictor( int bagNum, int maxH=10 );
	~Boosting_Predictor();

	void saveModel( const string& fNM ) const;
	void dispModel() const;
	
	void train( TR_Data& D );
	
	// getter
	int getBagNum() const { return this->bagNum; }
	int getMaxH() const { return this->maxH; }
	const MT_Model &getModel() const { return this->model; }
	const vector<CART_Predictor*> &getBaserPtrV() const { return this->baserPtrV; }

private:
	int bagNum;
	int maxH;
	vector<CART_Predictor*> baserPtrV;

	MT_Model model;

};
