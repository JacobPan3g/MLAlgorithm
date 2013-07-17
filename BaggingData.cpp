/*************************************************************************
	> File Name: baggingTree.cpp
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: Tue 16 Jul 2013 07:30:40 PM CST
 ************************************************************************/

#include "CsvData.cpp"
using namespace std;

#define FILENAME "dataset/pro1.csv"

class BaggingData: public CsvData
{
public:
	BaggingData( string filename=FILENAME );
	

protected:
	vector<double> get80Labels();
	vector< vector<double> > get80Features();
	
	vector<int> L_idx;
	vector<int> A_idx;
};

BaggingData::BaggingData( string filename ): 
	CsvData(filename)
{
	// this time the value of m is the super value: CsvData::m
	//cout << this->m << endl;
	this->L_idx.resize( CsvData::m );
	this->A_idx.resize( CsvData::n );
	this->m = CsvData::m * 0.8;
	this->n = CsvData::n * 0.8;
	// this time the value of m is 0.8 times of before
	//cout << this->m << endl;
	for ( int i = 0; i < this->L_idx.size(); i++ )
		this->L_idx[i] = i;
	for ( int i = 0; i < this->A_idx.size(); i++ )
		this->A_idx[i] = i;

	this->L = this->get80Labels();	// this func have to call first.(change this->L_idx )
	this->A = this->get80Features();
}

vector<double> BaggingData::get80Labels()
{
	vector<double> res(this->m);
	shuffling( this->L_idx );
	for ( int i = 0; i < this->m; i++ )
		res[i] = this->L[this->L_idx[i]];
	return res;
}

vector< vector<double> > BaggingData::get80Features()
{
	vector< vector<double> > res(this->m);
	shuffling( this->A_idx );
	for ( int i = 0; i < this->m; i++ )
	{	
		vector<double> tmp( this->n );
		for ( int j = 0; j < this->n; j++ )
		{
			tmp[j] = A[this->L_idx[i]][this->A_idx[j]];
		}
		res[i] = tmp;
	}
	return res;
}


/*
int main()
{
	BaggingData data( "dataset/pro1.csv" );
	cout << data.m << " " << data.n << endl;
	cout << data.CsvData::m << endl;
	disp( data.L_idx );
	disp( data.D_idx );
	disp( data.get80Labels() );
	cout << 
	return 0;
}
*/
