/**********************************************************************
	> File Name: Model.h
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Thu 01 Aug 2013 03:59:20 PM CST
 *********************************************************************/

#pragma once


#include "CART.h"

class Model
{
public:
	virtual void load( const CART* pdPtr ) = 0;
	virtual void save( string fNM ) = 0;
};

