/*************************************************************************
	> File Name: bagging.cpp
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Tue 16 Jul 2013 09:15:14 PM CST
 ************************************************************************/

#include "BaggingData.cpp"
#include "tree.cpp"
using namespace std;


class Bagging
{
public:
	Bagging( int maxH, int bagNum );
	void train( string tName );
	vector<double> predict( string pName, string treesFile );

private:
	int maxH;
	int bagNum;
};

/*
double bagging( int maxH, int bagNum, const vector<double> &a )
{
	vector<double> res( bagNum );
	
	for ( int i = 0; i < bagNum; i++ )
	{
		BaggingData D;
		BinaryTree tree( D, D.cs, D.fs, maxH );
		//tree.saveTree( "trees/bag.tree" );
		//tree.dispTree();
		//tree.dispLeaves();
		res[i] = tree.predict( a );
	}
	return mean( res );
}

double bagging( int maxH, int bagNum, const CsvData &test )
{
	vector< vector<double> > tmp( bagNum ); //bagNum * test.m
	for ( int i = 0; i < bagNum; i++ )
	{
		BaggingData D;
		BinaryTree tree( D, D.cs, D.fs, maxH );
		tree.saveTree( "trees/bag.tree" );
		tmp[i] = tree.predict( test );
	}
	// predict
	vector<double> p = meanR( tmp );
	save( "p/h-2_bn-10_10s.p", p );
	return rmse( p, test.L );
}*/

Bagging::Bagging( int maxH, int bagNum )
{
	this->maxH = maxH;
	this->bagNum = bagNum;
}

void Bagging::train( string tName )
{
	BaggingData tSet( tName );
	for ( int i = 0; i < this->bagNum; i++ )
	{
		tSet.reshuffling();
		BinaryTree tree( tSet, tSet.cs, tSet.fs, this->maxH );
		tree.saveTree( "trees/bagging.tree" );
	}
}

vector<double> Bagging::predict( string pName, string treesFile )
{
	vector< vector<double> > trees;
	csvread( treesFile, trees );
	
	CsvData pSet( pName );

	vector<double> p( pSet.m );
	for ( int i = 0; i < pSet.m; i++ )
	{
		vector<double> tmp( this->bagNum );
		int tmpIdx = 0;
		for ( int j = 0; j < bagNum; j++ )
		{
			int k = j*4; 
			int cur = 0;
			while ( trees[k][cur] != -1 )
			{
				if ( pSet.A[i][ (int)trees[k][cur] ] <= trees[k+3][cur] )	// f <= obVal
					cur = trees[k+1][cur];		// leftward
				else
					cur = trees[k+2][cur];		// rightward
			}
			tmp[tmpIdx] = trees[k+3][cur];		// get the label
		}
		p[i] = mean( tmp );
	}
	return p;
}

int main()
{
	time_t tic = clock();

	//CsvData test("dataset/pro1.csv");
	//cout << bagging( 2, 10, test ) << endl;

	Bagging bag( 2, 10 );
	bag.train( "dataset/pro1.csv" );
	vector<double> p =  bag.predict( "dataset/pro1_t.csv", "trees/bagging.tree" );
	save( "res", p );
	cout << p.size() << endl;

	time_t toc = clock();
	double tt = (double)(toc-tic)/CLOCKS_PER_SEC;
	cout << "Time: " << tt << "s" << endl;

	return 0;
}
