/*************************************************************************
	> File Name: tree.cpp
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: Sun 07 Jul 2013 10:57:34 AM CST
 ************************************************************************/

#define _CART_PREDICTOR_UTEST_

#include "CART_Predictor.h"
#include "VAR_Measurer.cpp"
#include "ST_Model.cpp"


/* Function: train()
 *		-- train for a model from data
 *	Call: foundALeaf(), 
 *	Update:	c_msr, root, high, labels, leaf, inNode, sinNodeNum
 *	Return: void 
 */
void CART_Predictor::train( const TR_Data &D, const vector<int>& cs, const vector<int>& fs )
{
	vector<double> L = D.getL();
	int m = D.getM();
	int n = D.getN();

	assert( cs.size()==m );
	assert( fs.size()==n );

	vector<int> myFs = fs;
	queue<Node*> q;
	this->sinNodeNum = 0;
	
	this->root = new Node( cs, countTag(cs), 0 );
	q.push( this->root );
	while ( !q.empty() )
	{
		Node *node = q.front();
		q.pop();
		// end 1
		if ( node->high >= this->MAX_HIGH )
		{
			this->foundALeaf( node, L ); 	// a leaf needn't find minF
			continue;
		}
		// end 2
		/// is all features used
		if ( isAll( myFs, 0 ) )
		{
			this->foundALeaf( node, L ); 	// a leaf needn't find minF
			continue;
		}
		// end 3
		// is all same label
		if ( isAllSame( L, node->cs ) )
		{
			this->foundALeaf( node, L );
			continue;
		}
		// end 4
		/// the measure's end condition
		if ( c_msr.endCondition( L, node->cs, node->cnum ) )
		{
			this->foundALeaf( node, L ); 	// a leaf needn't find minF
			continue;
		}
		
		// handle the inner node
		inNode.push_back( node );

		// get the optimal feature
		MS ms = c_msr.measure( D, node->cs, myFs );
		/// tag and asign
		myFs[ms.fIdx] = 0;
		node->fIdx = ms.fIdx;
		node->obValue = ms.obVal;
	/*
		// separate the data by minF( need to know the minF )
		vector<int> part1( m, 0 );
		vector<int> part2( m, 0 );
		int num1 = 0;
		int num2 = 0;
		for ( int i = 0; i < m; i++ )
		{
			if ( !node->cs[i] )
				continue;
			if ( D.A[i][node->fIdx] <=  )
			{
				num1++;
				part1[i] = 1;
			}
			else
			{
				num2++;
				part2[i] = 1;
			}
		}

		assert( countTag(part1)==num1 );
		assert( countTag(part2)==num2 );
		assert( num1+num2==countTag(node->cs) );
	*/
		int num1 = c_msr.getNum1();
		int num2 = c_msr.getNum2();

		// handle the single node
		if ( num1 == 0 || num2 == 0 )
		{
			node->single = 1;
			this->sinNodeNum++;
		}

		int h = node->high + 1;
		if ( num1 != 0 )
		{
			node->left = new Node( c_msr.getPart1(), num1, h );
			q.push(node->left);
		}
		if ( num2 != 0 )
		{
			node->right = new Node( c_msr.getPart2(), num2, h );
			q.push(node->right);
		}
	}

	assert( this->high <= MAX_HIGH );
	assert( root!=NULL );
	assert( leaf.size()!=0 );
	assert( leaf.size()==labels.size() );
	assert( countTag(myFs)+inNode.size()==countTag(fs) );
	assert( high <= (int)log2(n)+1 );
	assert( inNode.size()-sinNodeNum+1==leaf.size() );	//(m-1)i+1=t
}
/*
vector<double> CART_Predictor::predict( const Model& model, const TR_Data &T )
{
	vector<double> res( test.m );
	for ( int i = 0; i < test.m; i++ )
		res[i] = this->predict( test.A[i] );
	return res;
}
*/

/* Function: saveModel()
 *		-- train for a model from data
 *	Call: foundALeaf(), 
 *	Update:	c_msr, root, fs, high, labels, leaf, inNode, sinNodeNum
 *	Return: void 
 */
void CART_Predictor::saveModel( const string& fNM ) const
{
//	queue<Node*> q;
//	vector<int> fIdxV;
//	vector<int> leftV;
//	vector<int> rightV;
//	vector<double> oValV;

	ST_Model m( this );	

//	disp( fIdxV );
//	disp( oValV );
//	disp( leftV );
//	disp( rightV );
//	cout << fIdxV.size() << endl << oValV.size() << endl << leftV.size() << endl << rightV.size() << endl;

	// save
	ofstream obj( fNM.c_str() ); //, ofstream::app );
	save( obj, m.fIdxV, "," );
	save( obj, m.leftV, "," );
	save( obj, m.rightV, ",");
	save( obj, m.obValV, "," );
	obj.close();
}

// Own Method
CART_Predictor::CART_Predictor( int maxH )
{
	this->c_msr = VAR_Measurer();
	this->MAX_HIGH = maxH;
	this->high = 0;
}

CART_Predictor::~CART_Predictor()
{
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

void CART_Predictor::train( const TR_Data& D )
{
	vector<int> cs( D.getM(), 1 );
	vector<int> fs( D.getN(), 1 );
	this->train( D, cs, fs );
}

double CART_Predictor::predict( const vector<double> &a )
{

	Node *node = this->root;
	while( node->fIdx != -1 )
	{

		if ( a[node->fIdx] <= node->obValue )
			node = node->left;
		else
			node = node->right;
	}
	return this->labels[node->lIdx];
}

void CART_Predictor::saveTrees( ofstream &fobj ) const
{
	
}

void CART_Predictor::dispTree() const
{
	queue<Node*> q;
	q.push(this->root);
	int high = 0;

	while( !q.empty() )
	{
		Node *node = q.front();
		q.pop();
		
		if ( node->high > high )
		{
			cout << endl;
			high++;
		}
		cout << node->fIdx;
		if ( node->single == 1 )
			cout << "*";
		cout << " ";

		if ( node->left )
			q.push( node->left );
		if ( node->right )
			q.push( node->right );
	}
	cout << endl;
}


void CART_Predictor::dispLeaves() const
{
	cout << "lIdx\tcnum\thigh\tlabel\t" << endl;
	for ( int i = 0; i < this->leaf.size(); i++ )
		cout << i << "\t"
			 << this->leaf[i]->cnum << "\t" 
			 << this->leaf[i]->high << "\t" 
			 << this->labels[i] << endl;
	cout << "Total: " << this->leaf.size() << endl;

}

// getter
const Node* CART_Predictor::getRoot() const
{
	return this->root;
}

int CART_Predictor::getMAX_HIGH() const
{
	return this->MAX_HIGH;
}

int CART_Predictor::getHigh() const
{
	return this->high;
}

// big data getter
const vector<double>& CART_Predictor::getLabels() const
{
	return this->labels;
}

const vector<Node*>& CART_Predictor::getLeaf() const
{
	return this->leaf;
}

const vector<Node*>& CART_Predictor::getInNode() const
{
	return this->inNode;
}

// Private Method
void CART_Predictor::foundALeaf( Node *node, const vector<double> &L )
{
	node->lIdx = this->leaf.size();
	this->high = node->high > this->high? node->high : this->high;
	this->leaf.push_back( node );
	this->labels.push_back( c_msr.estimateLabel( L, node->cs ) );
}




/*********************************************************************
 * Unit Test
 * by Jacob Pan
 *********************************************************************/

#ifdef _CART_PREDICTOR_UTEST_

// just a tree build
void test1()
{

//#define _TEST_1_1_
#define _TEST_1_2_
	
	TR_Data D;
	D.fmtread( "test/case1.fmt" );
	int m = D.getM();
	int n = D.getN();

#ifdef _TEST_1_1_
/* Test 1.1 
 * Type: averager
 * Goal: 1. calculation
 *		 2. tree-build
 */
/*	CART_Predictor c1T( D, 2 );
	c1T.dispTree();

	assert( isAll(c1T.features, 1, tag) );
	assert( c1T.inNode.size()==1 );
	assert( c1T.inNode[0]->fIdx==0 );
	assert( c1T.inNode[0]->obValue==0.5 );
	assert( c1T.high==1 );
	assert( c1T.leaf.size()==2 );
	assert( c1T.labels[0]==1&&c1T.labels[1]==2);*/
#endif

#ifdef _TEST_1_2_
/* Test 1.2
 * Type: accurater
 * Goal: 1. calculation
 *		 2. tree-build
 */
	CART_Predictor c2T;
	c2T.train( D );
	c2T.dispTree();

	vector<Node*> inNode = c2T.getInNode();
	assert( inNode.size()==1 );
	assert( inNode[0]->fIdx==0 );
	assert( inNode[0]->obValue==0.5 );
	assert( c2T.getHigh()==1 );
	assert( c2T.getLeaf().size()==2 );
	assert( c2T.getLabels()[0]==1&&c2T.getLabels()[1]==2);
#endif
}

void test2()
{

#define _TEST_2_1_

	TR_Data D;
	//D.csvread( "test/case2.csv" );
	D.fmtread( "test/case2.fmt" );

#ifdef _TEST_2_1_
/* Test 2.1
 * Type: accurater
 * Goal: 1. calculation
 *		 2. Tree-bulid
 */
	CART_Predictor t1;
	t1.train( D );
	t1.dispTree();

	vector<Node*> inNode = t1.getInNode();
	vector<double> labels = t1.getLabels();
	assert( inNode.size()==2 );
	assert( inNode[0]->fIdx==2&&inNode[1]->fIdx==1 );
	assert( inNode[0]->obValue==0&&inNode[1]->obValue==0 );
	assert( t1.getHigh()==2 );
	assert( t1.getLeaf().size()==3 );
	assert( labels[0]==1&&labels[1]==0&&labels[2]==1 );
#endif
}

void pro1Test( int maxH )
{
	time_t tic = clock();

	TR_Data D;
	D.fmtread("dataset/pro1.fmt");
	CART_Predictor tree( maxH );
	tree.train( D );

	tree.dispTree();
	tree.dispLeaves();
	//cout << tree.predict( D.A[18] ) << endl;
	
	tree.saveModel( "trees/CART_Tree_B_1" );
	//cout << tree.inNode[17]->obValue << endl;
	
	// all use
	//assert( isAll(tree.features,0) );

	time_t toc = clock();
	cout << "Time: " << (double)(toc-tic)/CLOCKS_PER_SEC << "s"<< endl;
}

int main()
{
	test1();	// done
	test2();	// done
	
	pro1Test( 10 );

	cout << "All Unit Cases Passed." << endl;
	return 0;
}

#endif
