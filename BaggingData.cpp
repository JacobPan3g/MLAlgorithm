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
	void reshuffling();

	vector<int> cs;
	vector<int> fs;
	int bm;
	int bn;

protected:
	void get80Labels();
	void get80Features();
	
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
	this->bm = CsvData::m * 0.8;
	this->bn = CsvData::n * 0.8;
	// this time the value of m is 0.8 times of before
	//cout << this->m << endl;
	for ( int i = 0; i < this->L_idx.size(); i++ )
		this->L_idx[i] = i;
	for ( int i = 0; i < this->A_idx.size(); i++ )
		this->A_idx[i] = i;

	this->reshuffling();
}

void BaggingData::reshuffling()
{
	this->get80Labels();
	this->get80Features();
}

void BaggingData::get80Labels()
{
	this->cs.resize( this->m, 0 );
	shuffling( this->L_idx );				// shuffling
	for ( int i = 0; i < this->bm; i++ )	// choose 80%
		this->cs[this->L_idx[i]] = 1;
}

void BaggingData::get80Features()
{
	this->fs.resize( this->n, 0 );
	shuffling( this->A_idx );
	for ( int i = 0; i < this->bn; i++ )
		this->fs[this->A_idx[i]] = 1;
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
