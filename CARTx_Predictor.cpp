/**********************************************************************
	> File Name: CARTx_Predictor.cpp
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Thu 22 Aug 2013 02:36:11 PM CST
 *********************************************************************/

#define _CARTX_PREDICTOR_UTEST_


#include "Node.h"
#include <queue>

#include "CARTx_Predictor.h"

// Friends Method
//ostream& operator<<( ostream& os, const CARTx_Predictor& pd );

// Static Method
//vector<double> CARTx_Predictor::predict( const ST_Model& mdl, const Data& T );

// Own Method
CARTx_Predictor::CARTx_Predictor( int maxH=10 )
	: root(NULL), maxH(maxH), high(0)
{
}

CARTx_Predictor::~CARTx_Predictor()
{
	if ( this->root == NULL )
		return;

	queue<Node*> q;
	q.push( this->root );
	while( !q.empty() )
	{
		Node *node = q.front();
		q.pop();
		if ( node->left != NULL )
			q.push( node->left );
		if ( node->right != NULL )
			q.push( node->right );

		delete node;
	}
}

void CARTx_Predictor::train( const TR_Data& D )
{}

void CARTx_Predictor::train_forRoot()
{
	VAR_Measurer var;
	MS ms = var.measurer( D, cs, fs );
	
}

void CARTx_Predictor::train_forNext()
{}

