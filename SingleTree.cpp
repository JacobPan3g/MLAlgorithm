/**********************************************************************
	> File Name: SingleTree.cpp
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Thu 01 Aug 2013 04:15:49 PM CST
 *********************************************************************/

#include "SingleTree.h"

SingleTree::SingleTree( const CART *pdPtr )
{
	queue<Node*> q;

	this->fIdxV.push_back( pdPtr->root->fIdx );
	q.push( pdPtr->root );
	
	while ( !q.empty() )
	{
		Node *node = q.front();
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
			this->obValV.push_back( pdPtr->labels[node->lIdx] );	// save the labels
			this->leftV.push_back( -1 );
			this->rightV.push_back( -1 );
		}
	}
	
}

void SingleTree::load( const CART* pdPtr )
{

}

void SingleTree::save( string fNM )
{

}
