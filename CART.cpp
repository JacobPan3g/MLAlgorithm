/*************************************************************************
	> File Name: tree.cpp
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: Sun 07 Jul 2013 10:57:34 AM CST
 ************************************************************************/

#define _CART_UTEST_


#include "CART.h"
#include "VAR.cpp"

CART::CART( const CsvData &D, int spNum, int maxH )
{
	vector<int> cs(D.m, 1);
	vector<int> fs(D.n, 1);
	this->init( D, cs, fs, spNum, maxH );
}

CART::CART( const CsvData &D, const vector<int> &cs, const vector<int> &fs, int spNum, int maxH )
{
	this->init( D, cs, fs, spNum, maxH );
}

CART::~CART()
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

void CART::init( const CsvData &D, const vector<int> &cs, const vector<int> &fs, int spNum, int maxH )
{
	this->c_msr = VAR( spNum );
	this->MAX_HIGH = maxH;
	this->high = 0;
	this->features = fs;
	this->bulidTree( D, cs );
}

void CART::bulidTree( const CsvData &D, const vector<int> &rows )
{
	queue<Node*> q;
	this->sinNodeNum = 0;
	
	this->root = new Node( rows, D.m, 0 );
	q.push( this->root );
	while ( !q.empty() )
	{
		Node *node = q.front();
		q.pop();
		// end 1
		if ( node->high >= MAX_HIGH )
		{
			this->foundALeaf( node, D.L ); 	// a leaf needn't find minF
			continue;
		}
		// end 2
		/// is all features used
		if ( isAll( this->features, 0 ) )
		{
			this->foundALeaf( node, D.L ); 	// a leaf needn't find minF
			continue;
		}
		// end 3
		// is all same label
		if ( isAllSame( D.L, node->cases ) )
		{
			this->foundALeaf( node, D.L );
			continue;
		}
		// end 4
		/// the measure's end condition
		if ( c_msr.endCondition( D.L, node->cases, node->cnum ) )
		{
			this->foundALeaf( node, D.L ); 	// a leaf needn't find minF
			continue;
		}
		
		// handle the inner node
		inNode.push_back( node );

		// get the optimal feature
		vector< vector<double> > ms = c_msr.measure( D, node->cases, this->features );
		vector<double> tmp = min( ms, this->features );
		double minMS = tmp[0];
		int minF = (int)tmp[1];		// ms start at first feature
		double obValue = c_msr.getSpByValueIdx( minF, (int)tmp[2] );
		/// tag and asign
		this->features[minF] = 0;
		node->fIdx = minF;
		node->obValue = obValue;
		// separate the data by minF( need to know the minF )
		vector<int> part1( D.m, 0 );
		vector<int> part2( D.m, 0 );
		int num1 = 0;
		int num2 = 0;
		for ( int i = 0; i < D.m; i++ )
		{
			if ( !node->cases[i] )
				continue;
			if ( D.A[i][node->fIdx] <= obValue )
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
		assert( num1+num2==countTag(node->cases) );
		
		// handle the single node
		if ( num1 == 0 || num2 == 0 )
		{
			node->single = 1;
			this->sinNodeNum++;

			/*node->fIdx = -1;
			this->features[minF] = 1;
			q.push( node );*/
		}
		//else{

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
		//}

	}

	assert( this->high <= MAX_HIGH );
	assert( root!=NULL );
	assert( leaf.size()!=0 );
	assert( leaf.size()==labels.size() );
	assert( countTag(this->features)+inNode.size()==D.n );
	assert( high <= (int)log2(D.n)+1 );
	assert( inNode.size()-sinNodeNum+1==leaf.size() );	//(m-1)i+1=t
}

void CART::foundALeaf( Node *node, const vector<double> &L )
{
	node->lIdx = this->leaf.size();
	this->high = node->high > this->high? node->high : this->high;
	this->leaf.push_back( node );
	this->labels.push_back( c_msr.estimateLabel( L, node->cases ) );
}

void CART::dispTree()
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


void CART::dispLeaves()
{
	cout << "lIdx\tcnum\thigh\tlabel\t" << endl;
	for ( int i = 0; i < this->leaf.size(); i++ )
		cout << i << "\t"
			 << this->leaf[i]->cnum << "\t" 
			 << this->leaf[i]->high << "\t" 
			 << this->labels[i] << endl;
	cout << "Total: " << this->leaf.size() << endl;

}

double CART::predict( const vector<double> &a )
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

vector<double> CART::predict( const CsvData &test )
{
	vector<double> res( test.m );
	for ( int i = 0; i < test.m; i++ )
		res[i] = this->predict( test.A[i] );
	return res;
}

void CART::saveTree( string filename )
{
	queue<Node*> q;
	vector<int> fIdxV;
	vector<int> leftV;
	vector<int> rightV;
	vector<double> oValV;

	fIdxV.push_back( this->root->fIdx );
	q.push( this->root );
	
	while ( !q.empty() )
	{
		Node *node = q.front();
		q.pop();

		if ( node->fIdx != -1 )
		{
			oValV.push_back( node->obValue );

			if ( node->left != NULL )
			{
				q.push( node->left );
				leftV.push_back( fIdxV.size() );
				fIdxV.push_back( node->left->fIdx );
			}
			else
				leftV.push_back( -1 );

			if ( node->right != NULL )
			{
				q.push( node->right );
				rightV.push_back( fIdxV.size() );
				fIdxV.push_back( node->right->fIdx );
			}
			else
				rightV.push_back( -1 );
		}
		else
		{
			oValV.push_back( this->labels[node->lIdx] );	// save the labels
			leftV.push_back( -1 );
			rightV.push_back( -1 );
		}
	}
/*	disp( fIdxV );
	disp( oValV );
	disp( leftV );
	disp( rightV );
	cout << fIdxV.size() << endl << oValV.size() << endl << leftV.size() << endl << rightV.size() << endl;
*/
	// save
	ofstream obj( filename.c_str(), ofstream::app );
	save( obj, fIdxV, "," );
	save( obj, leftV, "," );
	save( obj, rightV, ",");
	save( obj, oValV, "," );
	obj.close();
}


#ifdef _CART_UTEST_

void test1()
{

#define _TEST_1_1_
#define _TEST_1_2_
	
	CsvData D;
	D.csvread( "test/case1.csv" );
	vector<int> tag(D.n, 1);	// just for first assert in each test
	tag[0] = 0;					// mean just choose F0

#ifdef _TEST_1_1_
/* Test 1.1 
 * Type: averager
 * Goal: 1. calculation
 *		 2. tree-build
 */
	CART c1T( D, 2 );
	c1T.dispTree();

	assert( isAll(c1T.features, 1, tag) );
	assert( c1T.inNode.size()==1 );
	assert( c1T.inNode[0]->fIdx==0 );
	assert( c1T.inNode[0]->obValue==0.5 );
	assert( c1T.high==1 );
	assert( c1T.leaf.size()==2 );
	assert( c1T.labels[0]==1&&c1T.labels[1]==2);
#endif

#ifdef _TEST_1_2_
/* Test 1.2
 * Type: accurater
 * Goal: 1. calculation
 *		 2. tree-build
 */
	CART c2T( D, -1 );
	c2T.dispTree();
	
	assert( isAll(c2T.features, 1, tag) );
	assert( c2T.inNode.size()==1 );
	assert( c2T.inNode[0]->fIdx==0 );
	assert( c2T.inNode[0]->obValue==0.5 );
	assert( c2T.high==1 );
	assert( c2T.leaf.size()==2 );
	assert( c2T.labels[0]==1&&c2T.labels[1]==2);
#endif
}

void test2()
{

#define _TEST_2_1_

	CsvData D;
	D.csvread( "test/case2.csv" );

#ifdef _TEST_2_1_
/* Test 2.1
 * Type: accurater
 * Goal: 1. calculation
 *		 2. Tree-bulid
 */
	CART t1( D, -1 );
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
void liveTest( int spNum, int maxH )
{
	time_t tic = clock();

	CsvData D;
	D.csvread("dataset/pro1.csv");
	CART tree(D, spNum, maxH);

	tree.dispTree();
	tree.dispLeaves();
	//cout << tree.predict( D.A[18] ) << endl;
	
	tree.saveTree( "trees/UT_all" );
	//cout << tree.inNode[17]->obValue << endl;
	
	// all use
	//assert( isAll(tree.features,0) );

	time_t toc = clock();
	cout << "Time: " << (double)(toc-tic)/CLOCKS_PER_SEC << "s"<< endl;
}

int main()
{
//	test1();	// done
//	test2();	// done
	
	liveTest( -1, 10 );

	cout << "All Unit Cases Passed." << endl;
	return 0;
}

#endif
