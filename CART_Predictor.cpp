/*************************************************************************
	> File Name: tree.cpp
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: Sun 07 Jul 2013 10:57:34 AM CST
 ************************************************************************/

#define _CART_PREDICOTR_UTEST_


#include "CART_Predictor.h"
#include "VAR_Measurer.cpp"
#include "ST_Model.cpp"


void CART_Predictor::train( const TR_Data &D, const vector<int>& cs, const vector<int>& fs )
{
	vector<double> L = D.getL();
	int m = D.getM();
	int n = D.getN();

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
		if ( isAll( this->fs, 0 ) )
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
		MS ms = c_msr.measure( D, node->cs, this->fs );
		/// tag and asign
		this->features[ms.fIdx] = 0;
		node->fIdx = ms.fIdx;
		node->obValue = ms.obVal;

		// separate the data by minF( need to know the minF )
		/**/ // can optimize here
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
		
		// handle the single node
		if ( num1 == 0 || num2 == 0 )
		{
			node->single = 1;
			this->sinNodeNum++;
		}

		int h = node->high + 1;
		if ( num1 != 0 )
		{
			node->left = new Node( part1, num1, h );
			q.push(node->left);
		}
		if ( num2 != 0 )
		{
			node->right = new Node( part2, num2, h );
			q.push(node->right);
		}
	}

	assert( this->high <= MAX_HIGH );
	assert( root!=NULL );
	assert( leaf.size()!=0 );
	assert( leaf.size()==labels.size() );
	assert( countTag(this->fs)+inNode.size()==D.n );
	assert( high <= (int)log2(D.n)+1 );
	assert( inNode.size()-sinNodeNum+1==leaf.size() );	//(m-1)i+1=t
}

vector<double> CART_Predictor::predict( const Model& model, const TR_Data &T )
{
	vector<double> res( test.m );
	for ( int i = 0; i < test.m; i++ )
		res[i] = this->predict( test.A[i] );
	return res;
}

void CART_Predictor::saveModel( string fNM ) const
{
/*	queue<Node*> q;
	vector<int> fIdxV;
	vector<int> leftV;
	vector<int> rightV;
	vector<double> oValV;
*/
	ST_Model m( this );	

/*	disp( fIdxV );
	disp( oValV );
	disp( leftV );
	disp( rightV );
	cout << fIdxV.size() << endl << oValV.size() << endl << leftV.size() << endl << rightV.size() << endl;
*/
	// save
	ofstream obj( filename.c_str()/*, ofstream::app*/ );
	save( obj, m.fIdxV, "," );
	save( obj, m.leftV, "," );
	save( obj, m.rightV, ",");
	save( obj, m.obValV, "," );
	obj.close();
}

// Own Method
CART_Predictor::CART_Predictor( int maxH )
{
	this->c_msr = VAR();
	this->MAX_HIGH = maxH;
	this->high = 0;
}

CART_Predictor::~CART()
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

vector<int> CART_Predictor::getFs() const
{
	return this->fs;
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
	return this->getLeaf;
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
	this->labels.push_back( c_msr.estimateLabel( L, node->cases ) );
}




/*********************************************************************
 * Unit Test
 * by Jacob Pan
 *********************************************************************/

#ifdef _CART_Predictor_UTEST_

// just a tree build
void test1()
{

//#define _TEST_1_1_
#define _TEST_1_2_
	
	TR_Data D;
	D.fmtread( "test/case1.fmt" );
	vector<int> tag(D.n, 1);	// just for first assert in each test
	tag[0] = 0;					// mean just choose F0

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
	CART_Predictor c2T( D );
	c2T.dispTree();

	vector<Node*> inNode = c2T.getInNode();
	assert( isAll(c2T.getFs(), 1, tag) );
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

//#define _TEST_2_1_

	TR_Data D;
	D.csvread( "test/case2.csv" );

#ifdef _TEST_2_1_
/* Test 2.1
 * Type: accurater
 * Goal: 1. calculation
 *		 2. Tree-bulid
 */
	CART_Predictor t1();
	t1.dispTree();

	vector<int> tag( D.n, 1 ); tag[1]=0; tag[2]=0;
	assert( isAll(t1.features,1,tag) );
	assert( t1.inNode.size()==2 );
	assert( t1.inNode[0]->fIdx==2&&t1.inNode[1]->fIdx==1 );
	assert( t1.inNode[0]->obValue==0&&t1.inNode[1]->obValue==0 );
	assert( t1.high==2 );
	assert( t1.leaf.size()==3 );
	assert( t1.labels[0]==1&&t1.labels[1]==0&&t1.labels[2]==1 );
#endif
}

/*
 * @param1: spNum	the num of sp | -1 for accurater
 * @param2:	maxH	the max high of tree
 */
/*void liveTest( int spNum, int maxH )
{
	time_t tic = clock();

	TR_Data D;
	D.csvread("dataset/pro1.csv");
	CART_Predictor tree(D, spNum, maxH);

	tree.dispTree();
	tree.dispLeaves();
	//cout << tree.predict( D.A[18] ) << endl;
	
	tree.saveTree( "trees/UT_test" );
	//cout << tree.inNode[17]->obValue << endl;
	
	// all use
	//assert( isAll(tree.features,0) );

	time_t toc = clock();
	cout << "Time: " << (double)(toc-tic)/CLOCKS_PER_SEC << "s"<< endl;
}*/

int main()
{
	test1();	// done
//	test2();	// done
	
//	liveTest( -1, 1 );

	cout << "All Unit Cases Passed." << endl;
	return 0;
}

#endif
