/**********************************************************************
	> File Name: MT_Model.cpp
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Mon 19 Aug 2013 07:29:56 PM CST
 *********************************************************************/

#pragma once

#include "ST_Model.cpp"

#include "MT_Model.h"

// Virtual Method
void MT_Model::load( const string& fNM )
{
	assert( this->mdlPtrV.size()==0 );
	ifstream fin( fNM.c_str() );
	string tmpS;
	int sz;
	fin >> tmpS >> sz;
	this->mdlPtrV.resize( sz );
	for ( int i = 0; i < sz; i++ ) {
		this->mdlPtrV[i] = new ST_Model( fin );
		//this->mdlPtrV[i]->load( fin );
	}
	fin.close();
}

void MT_Model::save( const string& fNM ) const
{
	ofstream fout( fNM.c_str() );
	fout << "num: " << this->mdlPtrV.size() << endl;
	for ( int i = 0; i < this->mdlPtrV.size(); i++ ) {
		fout << endl;
		mdlPtrV[i]->save( fout );
	}
	fout.close();
}

// Own Method
MT_Model::MT_Model()
{
	this->mdlPtrV.clear();
	this->selfNew = false;
}

MT_Model::~MT_Model()
{
	// cause double free
	if ( selfNew )
		for ( int i = 0; i < this->mdlPtrV.size(); i++ )
			delete this->mdlPtrV[i];
}

/*
MT_Model::MT_Model( const vector<CART_Predictor*> &pdPtrV )
{
	int sz = pdPtrV.size();
	this->mdlPtrV.resize( sz );
	for ( int i = 0; i < sz; i++ ) {
		this->mdlPtrV[i] = new ST_Model( pdPtrV[i]->getRoot(), pdPtrV[i]->getLabels() );
	}
}*/

MT_Model::MT_Model( const string& fNM )
{
	this->selfNew = true;
	this->load( fNM );
}

void MT_Model::addModel( const ST_Model &mdl )
{
	this->mdlPtrV.push_back( &mdl );
}

void MT_Model::show() const
{
	for ( int i = 0; i < this->mdlPtrV.size(); i++ ) {
		this->mdlPtrV[i]->show();
	}
}

// getter
const vector<const ST_Model*> MT_Model::getMdlPtrV() const
{
	return this->mdlPtrV;
}
