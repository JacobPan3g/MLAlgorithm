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
}

void MT_Model::save( const string& fNM ) const
{
	for ( int i = 0; i < this->mdlPtrV.size(); i++ ) {
		
	}
}

// Own Method
MT_Model::MT_Model()
{
	this->mdlPtrV.clear();
}

MT_Model::~MT_Model()
{
	// cause double free
	//for ( int i = 0; i < this->mdlPtrV.size(); i++ ) {
	//	delete this->mdlPtrV[i];
	//}
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
{}

void MT_Model::addModel( const ST_Model &mdl )
{
	this->mdlPtrV.push_back( &mdl );
}

// getter
const vector<const ST_Model*> MT_Model::getMdlPtrV() const
{
	return this->mdlPtrV;
}
