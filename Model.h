/**********************************************************************
	> File Name: Model.h
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Thu 01 Aug 2013 03:59:20 PM CST
 *********************************************************************/

#pragma once


class Model
{
public:
	virtual void load( const string& fNM ) = 0;
	virtual void save( const string& fNM ) const = 0;
};

