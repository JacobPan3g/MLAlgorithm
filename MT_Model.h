/**********************************************************************
	> File Name: MT_Model.h
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Mon 19 Aug 2013 07:16:42 PM CST
 *********************************************************************/

#pragma once


class MT_Model
{
public:
	// Virtual Method
	void load( const string& fNM );
	void save( const string& fNM ) const;
	
	// Own Method
	MT_Model();
	~MT_Model();
	//MT_Model( const vector<CART_Predictor*> &pdPtrV );
	MT_Model( const string& fNM );
	void addModel( const ST_Model &mdl );
	void show() const;

	// getter
	const vector<const ST_Model*> getMdlPtrV() const;

private:
	vector<const ST_Model*> mdlPtrV;
	bool selfNew;
};

