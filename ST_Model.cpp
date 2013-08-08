/**********************************************************************
	> File Name: ST_Model.cpp
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Thu 01 Aug 2013 04:15:49 PM CST
 *********************************************************************/

#include "ST_Model.h"


// Virtual Method
void ST_Model::load( const string& fNM )
{
	ifstream fin( fNM.c_str() );
	string tmpS;
	int size;

	fin >> tmpS >> size;
	this->fIdxV = vector<int>( size );
	this->leftV = vector<int>( size );
	this->rightV = vector<int>( size );
	this->obValV = vector<double>( size );

	fin >> tmpS;
	for ( int i = 0; i < size; i++ ) {
		fin >> this->fIdxV[i];
	}	
	fin >> tmpS;
	for ( int i = 0; i < size; i++ ) {
		fin >> this->leftV[i];
	}
	fin >> tmpS;
	for ( int i = 0; i < size; i++ ) {
		fin >> this->rightV[i];
	}
	fin >> tmpS;
	for ( int i = 0; i < size; i++ ) {
		fin >> this->obValV[i];
	}
	fin.close();
}

void ST_Model::save( const string& fNM ) const
{
	assert( this->fIdxV.size()==this->leftV.size() );
	assert( this->rightV.size()==this->obValV.size() );
	assert( this->leftV.size()==this->rightV.size() );

	ofstream fout( fNM.c_str() );
	fout << "Size: " <<  this->fIdxV.size() << endl;
	fout << "fIdxV" << endl;
	txtwrite( fout, this->fIdxV );
	fout << "leftV" << endl;
	txtwrite( fout, this->leftV );
	fout << "rightV" << endl;
	txtwrite( fout, this->rightV );
	fout << "obValV" << endl;
	txtwrite( fout, this->obValV );
	fout.close();
}

// Own Method
ST_Model::ST_Model()
{
}

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
 
	assert( this->fIdxV.size()==this->leftV.size() );
	assert( this->rightV.size()==this->obValV.size() );
	assert( this->leftV.size()==this->rightV.size() );
}

ST_Model::ST_Model( const string& fNM )
{
	this->load( fNM );
}

// getter
const vector<int>& ST_Model::getFIdxV()
{
	return this->fIdxV;
}

const vector<int>& ST_Model::getLeftV()
{
	return this->leftV;
}

const vector<int>& ST_Model::getRightV()
{
	return this->rightV;
}

const vector<double>& ST_Model::getObValV()
{
	return this->obValV;
}
