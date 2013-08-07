/**********************************************************************
	> File Name: ST_Model.cpp
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Thu 01 Aug 2013 04:15:49 PM CST
 *********************************************************************/

#include "ST_Model.h"

ST_Model::ST_Model( const CART_Predictor *pdPtr )
{
	queue<const Node*> q;

	const Node* root = pdPtr->getRoot();
	vector<double> labels = pdPtr->getLabels();

	this->fIdxV.push_back( root->fIdx );
	q.push( root );
	
	while ( !q.empty() )
	{
		const Node *node = q.front();
		q.pop();

		if ( node->fIdx != -1 )
		{
			this->obValV.push_back( node->obValue );

			if ( node->left != NULL )
			{
				q.push( node->left );
				this->leftV.push_back( this->fIdxV.size() );
				this->fIdxV.push_back( node->left->fIdx );
			}
			else
				this->leftV.push_back( -1 );

			if ( node->right != NULL )
			{
				q.push( node->right );
				this->rightV.push_back( this->fIdxV.size() );
				this->fIdxV.push_back( node->right->fIdx );
			}
			else
				this->rightV.push_back( -1 );
		}
		else
		{
			this->obValV.push_back( labels[node->lIdx] );	// save the labels
			this->leftV.push_back( -1 );
			this->rightV.push_back( -1 );
		}
	}
	
}

void ST_Model::load( const CART_Predictor* pdPtr )
{

}

void ST_Model::save( const string& fNM )
{

}
