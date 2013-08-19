/**********************************************************************
	> File Name: Node.h
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Mon 19 Aug 2013 01:51:28 PM CST
 *********************************************************************/

#pragma once


struct Node			
{
	int fIdx;
	double obValue;
	Node *left;
	Node *right;
	int high;
	vector<int> cs;		// which cases(rows) will be considered.
	int cnum;			// the num of cases
	int single;			// mark is the single child node (just for debug)
	int lIdx;			// the leaf idx of the tree

	Node( const vector<int> &cs, int cnum, int high, int fIdx=-1, double obValue=-1, Node *l=NULL, Node *r=NULL ) 
	{
		this->fIdx = fIdx;
		this->obValue = obValue;
		this->left = l;
		this->right = r;
		this->high = high;
		this->cs = cs;
		this->cnum = cnum;
		this->single = 0;
		this->lIdx = -1;
	}
};
