/*************************************************************************
	> File Name: tree.cpp
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: Sun 07 Jul 2013 10:57:34 AM CST
 ************************************************************************/

#define _CART_PREDICTOR_UTEST_


#include <queue>

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

	this->model = ST_Model( this->root, this->labels );

	assert( this->high <= MAX_HIGH );
	assert( root!=NULL );
	assert( leaf.size()!=0 );
	assert( leaf.size()==labels.size() );
	assert( countTag(myFs)+inNode.size()==countTag(fs) );
	assert( high <= (int)log2(n)+1 );
	assert( inNode.size()-sinNodeNum+1==leaf.size() );	//(m-1)i+1=t
}

vector<double> CART_Predictor::predict( const ST_Model& mdl, const Data& T ) const
{
	int m = T.getM();
	vector<double> res( m );
	for ( int i = 0; i < m; i++ ) {
		res[i] = this->predict( mdl, T.getA()[i] );
	}
	return res;
}


/* Function: saveModel()
 *		-- train for a model from data
 *	Call: foundALeaf(), 
 *	Update:	c_msr, root, fs, high, labels, leaf, inNode, sinNodeNum
 *	Return: void 
 */
void CART_Predictor::saveModel( const string& fNM ) const
{
	this->model.save( fNM );
}

// Own Method
CART_Predictor::CART_Predictor( int maxH )
{
	this->root = NULL;
	this->c_msr = VAR_Measurer();
	this->MAX_HIGH = maxH;
	this->high = 0;
}

CART_Predictor::~CART_Predictor()
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

void CART_Predictor::train( const TR_Data& D )
{
	vector<int> cs( D.getM(), 1 );
	vector<int> fs( D.getN(), 1 );
	this->train( D, cs, fs );
}

vector<double> CART_Predictor::predict( const Data& T ) const
{
	return this->predict( this->model, T );
}

double CART_Predictor::predict( const vector<double> &a ) const
{
	return this->predict( this->model, a );
}

double CART_Predictor::predict( const ST_Model& mdl, const vector<double> &a ) const
{
	vector<int> fIdxV = mdl.getFIdxV();
	vector<int> leftV = mdl.getLeftV();
	vector<int> rightV = mdl.getRightV();
	vector<double> obValV = mdl.getObValV();
	
	int i = 0;
	while ( fIdxV[i] != -1 ) {
		if ( a[fIdxV[i]] <= obValV[i] ) {
			i = leftV[i];
		}
		else {
			i = rightV[i];
		}
	}
	return obValV[i];	
}

void CART_Predictor::saveTrees( ofstream &fobj ) const
{
	
}

void CART_Predictor::dispModel() const
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

const ST_Model& CART_Predictor::getModel() const
{
	return this->model;
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

#define _TEST_1_1_
#define _TEST_1_2_
#define _TEST_1_3_
#define _TEST_1_4_
#define _TEST_1_5_
	
	TR_Data D;
	D.fmtread( "test/case1.fmt" );
	Data T;
	T.csvread( "test/case1.csv" );
	int m = D.getM();
	int n = D.getN();
	vector<double> p;

	CART_Predictor c1T;
	c1T.train( D );

#ifdef _TEST_1_1_
/* Test 1.1
 * Type: accurater
 * Goal: 1. tree-build
 */
	//c1T.dispModel();

	vector<Node*> inNode = c1T.getInNode();
	assert( inNode.size()==1 );
	assert( inNode[0]->fIdx==0 );
	assert( inNode[0]->obValue==0.6 );
	assert( c1T.getHigh()==1 );
	assert( c1T.getLeaf().size()==2 );
	assert( c1T.getLabels()[0]==1&&c1T.getLabels()[1]==2);
#endif

#ifdef _TEST_1_2_
/* Test 1.2
 * Type: accurater
 * Goal: 1. test model
 */
	ST_Model model = c1T.getModel();
	//disp( model.getFIdxV() );
	//disp( model.getLeftV() );
	//disp( model.getRightV() );
	//disp( model.getObValV() );
	
	int m12f[] = { 0, -1, -1 };
	int m12l[] = { 1, -1, -1 };
	int m12r[] = { 2, -1, -1 };
	double m12o[] = { 0.6, 1, 2 };
	
	assert( isSame(model.getFIdxV(),m12f,sizeof(m12f)/sizeof(int)) );
	assert( isSame(model.getLeftV(),m12l,sizeof(m12l)/sizeof(int)) );
	assert( isSame(model.getRightV(),m12r,sizeof(m12r)/sizeof(int)) );
	assert( isSame(model.getObValV(),m12o,sizeof(m12o)/sizeof(double)) );
#endif

#ifdef _TEST_1_3_
/* Test 1.3
 * Type: accurater
 * Goal: 1. test saveModel()
 */
	c1T.saveModel( "model/case1.mdl" );
	ST_Model m13( "model/case1.mdl" );
	//disp( m13.getFIdxV() );
	//disp( m13.getLeftV() );
	//disp( m13.getRightV() );
	//disp( m13.getObValV() );
	
	int m13f[] = { 0, -1, -1 };
	int m13l[] = { 1, -1, -1 };
	int m13r[] = { 2, -1, -1 };
	double m13o[] = { 0.6, 1, 2 };
	
	assert( isSame(m13.getFIdxV(),m13f,sizeof(m13f)/sizeof(int)) );
	assert( isSame(m13.getLeftV(),m13l,sizeof(m13l)/sizeof(int)) );
	assert( isSame(m13.getRightV(),m13r,sizeof(m13r)/sizeof(int)) );
	assert( isSame(m13.getObValV(),m13o,sizeof(m13o)/sizeof(double)) );
#endif

#ifdef _TEST_1_4_
/* Test 1.4
 * Type: accurater
 * Goal: 1. predict after train
 */
	p = c1T.predict( T );
	//disp( p );
	//cout << rmse( p, T.getL() ) << endl;

	double p4[] = {1,1,1,2,2};
	assert( isSame(p,p4,sizeof(p4)/sizeof(double)) );
	assert( rmse(p,T.getL())==0 );
#endif

#ifdef _TEST_1_5_
/* Test 1.5
 * Type: accurater
 * Goal: 1. predict away train
 */
	CART_Predictor c5T;
	ST_Model m15( "model/case1.mdl" );

	p = c5T.predict( m15, T );
	//disp( p );
	//cout << rmse( p, T.getL() ) << endl;

	double p5[] = {1,1,1,2,2};
	assert( isSame(p,p5,sizeof(p5)/sizeof(double)) );
	assert( rmse(p,T.getL())==0 );
#endif
}

void test2()
{

#define _TEST_2_1_
#define _TEST_2_2_
#define _TEST_2_3_
#define _TEST_2_4_
#define _TEST_2_5_

	TR_Data D;
	//D.csvread( "test/case2.csv" );
	D.fmtread( "test/case2.fmt" );
	Data T;
	T.csvread( "test/case2.csv" );
	int m = D.getM();
	int n = D.getN();
	vector<double> p;

#ifdef _TEST_2_1_
/* Test 2.1
 * Type: accurater
 * Goal: 1. calculation
 *		 2. Tree-bulid
 */
	CART_Predictor c1T;
	c1T.train( D );
	//c1T.dispModel();

	vector<Node*> inNode = c1T.getInNode();
	vector<double> labels = c1T.getLabels();
	assert( inNode.size()==2 );
	assert( inNode[0]->fIdx==2&&inNode[1]->fIdx==1 );
	assert( inNode[0]->obValue==0.5&&inNode[1]->obValue==0.5 );
	assert( c1T.getHigh()==2 );
	assert( c1T.getLeaf().size()==3 );
	assert( labels[0]==1&&labels[1]==0&&labels[2]==1 );
#endif

#ifdef _TEST_2_2_
/* Test 2.2
 * Type: accurater
 * Goal: 1. test model
 */
	ST_Model model = c1T.getModel();
	//disp( model.getFIdxV() );
	//disp( model.getLeftV() );
	//disp( model.getRightV() );
	//disp( model.getObValV() );
	
	int m12f[] = { 2, 1, -1, -1, -1 };
	int m12l[] = { 1, 3, -1, -1, -1 };
	int m12r[] = { 2, 4, -1, -1, -1 };
	double m12o[] = { 0.5, 0.5, 1, 0, 1 };
	
	assert( isSame(model.getFIdxV(),m12f,sizeof(m12f)/sizeof(int)) );
	assert( isSame(model.getLeftV(),m12l,sizeof(m12l)/sizeof(int)) );
	assert( isSame(model.getRightV(),m12r,sizeof(m12r)/sizeof(int)) );
	assert( isSame(model.getObValV(),m12o,sizeof(m12o)/sizeof(double)) );
#endif

#ifdef _TEST_2_3_
/* Test 2.3
 * Type: accurater
 * Goal: 1. test saveModel()
 */
	c1T.saveModel( "model/case2.mdl" );
	ST_Model m13( "model/case2.mdl" );
	//disp( m13.getFIdxV() );
	//disp( m13.getLeftV() );
	//disp( m13.getRightV() );
	//disp( m13.getObValV() );
	
	int m13f[] = { 2, 1, -1, -1, -1 };
	int m13l[] = { 1, 3, -1, -1, -1 };
	int m13r[] = { 2, 4, -1, -1, -1 };
	double m13o[] = { 0.5, 0.5, 1, 0, 1 };
	
	assert( isSame(m13.getFIdxV(),m13f,sizeof(m13f)/sizeof(int)) );
	assert( isSame(m13.getLeftV(),m13l,sizeof(m13l)/sizeof(int)) );
	assert( isSame(m13.getRightV(),m13r,sizeof(m13r)/sizeof(int)) );
	assert( isSame(m13.getObValV(),m13o,sizeof(m13o)/sizeof(double)) );
#endif

#ifdef _TEST_2_4_
/* Test 2.4
 * Type: accurater
 * Goal: 1. predict after train
 */
	p = c1T.predict( T );
	//disp( p );
	//cout << rmse( p, T.getL() ) << endl;

	double p4[] = {0,0,1,1,0,0,0,1,1,1,1,1,1,1,0};
	assert( isSame(p,p4,sizeof(p4)/sizeof(double)) );
	assert( rmse(p,T.getL())==0 );
#endif

#ifdef _TEST_2_5_
/* Test 2.5
 * Type: accurater
 * Goal: 1. predict away train
 */
	CART_Predictor c5T;
	ST_Model m15( "model/case2.mdl" );

	p = c5T.predict( m15, T );
	//disp( p );
	//cout << rmse( p, T.getL() ) << endl;

	double p5[] = {0,0,1,1,0,0,0,1,1,1,1,1,1,1,0};
	assert( isSame(p,p5,sizeof(p5)/sizeof(double)) );
	assert( rmse(p,T.getL())==0 );
#endif
}

void pro1Test( int maxH )
{
	time_t tic = clock();

	TR_Data D;
	D.fmtread("dataset/pro1.fmt");
	CART_Predictor tree( maxH );
	tree.train( D );

	tree.dispModel();
	//tree.dispLeaves();
	
	//cout << tree.predict( D.A[18] ) << endl;
	
	tree.saveModel( "model/pro1_all_sr" );
	//cout << tree.inNode[17]->obValue << endl;

	assert( tree.getRoot()->fIdx==38 );

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
