/**********************************************************************
	> File Name: VAR_Measurer.cpp
	> Author: Jacob Pan
	> Mail: zhenjian3g@gmail.com 
	> Created Time: Mon 08 Jul 2013 01:36:58 PM CST
 *********************************************************************/

#include "VAR_Measurer.h"
#include "TR_Data.cpp"

#define LEAF_MAX_NUM 1


/* Function: measure()
 *		-- get the min VAR value for all f
 *	Call:	minVarAndSp()
 *	Update:	minFIdx, minSpVal, minVAR, m1, m2
 *	Return: MS 
 */
MS VAR_Measurer::measure( const TR_Data &D, const vector<int> &cs, const vector<int> &fs )
{
	int m = D.getM();
	int n = D.getN();
	vector<double> L = D.getL();
	vector< list< pair<int,double> > > fmtV = D.getFmtV();
	
	assert( cs.size()==m );
	assert( fs.size()==n );

	// get the total value
	double sqSums = 0;
	double sums = 0;
	int nums = 0;
	for ( int i = 0; i < L.size(); i++ ) {
		if ( cs[i] ) {
			sums += L[i];
			sqSums += pow( L[i], 2 );
			nums++;
		}
	}

	// calculate the variance
	this->minFIdx = -1;
	this->minVAR = 1e8;

	// each feature
	for ( int i = 0; i < n; i++ )	
	{
		if ( !fs[i] )
			continue;
	
		MS ms = this->minVarAndSp( L, fmtV[i], sqSums, sums, nums, cs );
		assert(ms.fIdx==-1);

		if ( ms.VAR < minVAR ) { 
			this->minFIdx = i;
			this->minSpVal = ms.spVal;
			this->minVAR = ms.VAR;
			this->m1 = ms.m1;
			this->m2 = ms.m2;
		}
	}

	// set the parts
	this->part1 = vector<int>( m, 0 );
	this->part2 = cs;

	list< pair<int,double> >::const_iterator it = fmtV[this->minFIdx].begin();
	for ( ; it != fmtV[this->minFIdx].end(); it++ ) {
		if ( !cs[it->first] ) continue;
		if ( it->second > this->minSpVal ) break;
		this->part1[it->first] = 1;
		this->part2[it->first] = 0;
	}

	// Assert
	assert( countTag(cs)==nums );
	assert( this->minFIdx < n );
	assert( this->minFIdx > -1 );
	assert( this->minVAR < 1e8 );
	assert( this->m1 + this->m2 == nums );
	assert( countTag(part1)+countTag(part2)==nums );

	return MS(this->minFIdx,this->minSpVal,this->minVAR,this->m1,this->m2);
}

MS VAR_Measurer::MPI_measure( const TR_Data &D, const vector<int> &cs, const vector<int> &fs )
{
	int m = D.getM();
	int n = D.getN();
	vector<double> L = D.getL();
	vector< list< pair<int,double> > > fmtV = D.getFmtV();
	
	assert( cs.size()==m );
	assert( fs.size()==n );

	// get the total value
	double sqSums = 0;
	double sums = 0;
	int nums = 0;
	for ( int i = 0; i < L.size(); i++ ) {
		if ( cs[i] ) {
			sums += L[i];
			sqSums += pow( L[i], 2 );
			nums++;
		}
	}

	// calculate the variance
	this->minFIdx = -1;
	this->minVAR = 1e8;

	//MPI msg data
	int mn[2];
	mn[0]=nums; mn[1]=n;
	double *mL = new double[nums];
	for ( int i=0,j=0; i < m; i++ ) {
		if ( !cs[i] ) continue;
		mL[j++] = L[i];
	}
	double mSums[2];
	mSums[0] = sqSums; mSums[1] = sums;

	//MPI Bcast the m, n, L, sqSums, sums
	MPI_Bcast( mn, 2, MPI_INT, 0, MPI_COMM_WORLD );
	MPI_Bcast( mL, nums, MPI_DOUBLE, 0, MPI_COMM_WORLD );
	MPI_Bcast( mSums, 2, MPI_DOUBLE, 0, MPI_COMM_WORLD );
	delete[] mL;

	// each feature
	for ( int i = 0; i < n; i++ )	
	{
		if ( !fs[i] )
			continue;
		
		//MPI Send the cow of A
		int *mI = new int[nums];
		double *mF = new double[nums];
		list< pair<int,double> >::const_iterator it = fmtV[i].begin();
		for ( int j=0; it != fmtV[i].end(); it++ ) {
			if ( !cs[it->first] ) continue;
			mI[j] = it->first;
			mF[j] = it->second;
			j++;
		}
		MPI_Send( mI, nums, MPI_INT, i+1, 0, MPI_COMM_WORLD );
		MPI_Send( mF, nums, MPI_DOUBLE, i+1, 1, MPI_COMM_WORLD );
		delete[] mI;
		delete[] mF;

		//MS ms = this->minVarAndSp( L, fmtV[i], sqSums, sums, nums, cs );
		//assert(ms.fIdx==-1);

		//MPI Receive the res
			// 0 for spVal
			// 1 for VAR
			// 2 for 1
		double res[3];
		MPI_Status sta;
		MPI_Recv( &res, 3, MPI_DOUBLE, MPI_ANY_SOURCE, 99, MPI_COMM_WORLD, &sta );
		double VAR = res[1];

		if ( VAR < minVAR ) { 
			this->minFIdx = i;
			this->minSpVal = res[0];
			this->minVAR = VAR;
			this->m1 = (int)res[2];
			this->m2 = nums - m1;
		}
	}

	// set the parts
	this->part1 = vector<int>( m, 0 );
	this->part2 = cs;

	list< pair<int,double> >::const_iterator it = fmtV[this->minFIdx].begin();
	for ( ; it != fmtV[this->minFIdx].end(); it++ ) {
		if ( !cs[it->first] ) continue;
		if ( it->second > this->minSpVal ) break;
		this->part1[it->first] = 1;
		this->part2[it->first] = 0;
	}

	// Assert
	assert( countTag(cs)==nums );
	assert( this->minFIdx < n );
	assert( this->minFIdx > -1 );
	assert( this->minVAR < 1e8 );
	assert( this->m1 + this->m2 == nums );
	assert( countTag(part1)+countTag(part2)==nums );

	return MS(this->minFIdx,this->minSpVal,this->minVAR,this->m1,this->m2);
}

double VAR_Measurer::estimateLabel( const vector<double> &L, const vector<int> &cs )
{
	return mean(L, cs);
}

bool VAR_Measurer::endCondition( const vector<double> &L, vector<int> cs, int num )
{
	return num <= LEAF_MAX_NUM;
}

// own methods
VAR_Measurer::VAR_Measurer()
{
}

// Private Method

/* Function: minVarAndSp()
 *		-- find the optimial <sp, var> for every f
 *	Update:	none 
 *	Return: MS 
 */
MS VAR_Measurer::minVarAndSp( const vector<double> L, const list< pair<int,double> > F, double sqSums, double sums, int nums, const vector<int> cs )
{
	assert( F.size()==cs.size() );
	assert( cs.size() >= nums );
	
	bool isFirst = true;
	double beforeItem;		// the front diff Item

	double sum1 = 0;
	int num1 = 0;		// num of 1st part

	double minVAR = 1e8;
	double minSp = 1e8;
	int m1;
	int m2;

	int i = 0;
	list< pair<int,double> >::const_iterator it = F.begin();
	for ( ; it != F.end(); it++ ) {
		if ( cs[it->first] ) {
			//num1++;
			if ( isFirst ) {
				isFirst = false;
				beforeItem = it->second;
			}
			else {
				if ( beforeItem != it->second ) {
					double sp = (it->second+beforeItem)/2;
					// for calculation in sp
					double var = computeVAR( sqSums, sums, nums, sum1, num1 );
					
					if ( var < minVAR ) {
						minSp = sp;
						minVAR = var;
						m1 = num1;
						m2 = nums - num1;
					}

					beforeItem = it->second;
					i++;
				}
			}
			// calculate the sum
			sum1 += L[it->first];
			num1++;
		}
	}

	if ( i == 0 ) {
		minSp = F.begin()->second;
		minVAR = computeVAR( sqSums, sums, nums, sum1, num1 );
		m1 = num1;
		m2 = nums - num1;
		assert( m2==0 );
	}
	
	return MS( -1, minSp, minVAR, m1, m2 );
}

double VAR_Measurer::computeVAR( double sqSums, double sums, int nums, double sum1, int num1)
{
	double m1 = num1 + 1e-8;
	double m2 = nums - num1 + 1e-8;
	double sum2 = sums - sum1;
	double M = m1 + m2;
	assert( M != 0 );
	return (sqSums - pow(sum1,2)/m1 - pow(sum2,2)/m2) / M;
}


MS VAR_Measurer::MPI_minVar( double L[], int I[], double F[], double sqSums, double sums, int m )
{
/*
	cout << " "; for( int i=0; i < m; i++ ) cout << L[i] << " ";
	cout << " "; for( int i=0; i < m; i++ ) cout << I[i] << " ";
	cout << " "; for( int i=0; i < m; i++ ) cout << F[i] << " ";
	cout << endl;
*/
	bool isFirst = true;
	double beforeItem;		// the front diff Item

	double sum1 = 0;
	int num1 = 0;		// num of 1st part

	double minVAR = 1e8;
	double minSp = 1e8;
	int m1;

	int numSp = 0;	// num of sp
	for ( int i = 0; i < m; i++ ) {
		if ( isFirst ) {
			isFirst = false;
			beforeItem = F[i];
		}
		else {
			if ( beforeItem != F[i] ) {
				double sp = (F[i]+beforeItem)/2;
				double var = computeVAR( sqSums, sums, m, sum1, num1 );
				
				if ( var < minVAR ) {
					minSp = sp;
					minVAR = var;
					m1 = num1;
				}
				beforeItem = F[i];
				numSp++;
			}
		}
		// calculate the sum
		sum1 += L[I[i]];
		num1++;
	}

	if ( numSp == 0 ) {
		minSp = F[0];
		minVAR = computeVAR( sqSums, sums, m, sum1, num1 );
		m1 = num1;
		assert( m1==m );
	}
	
	return MS( -1, minSp, minVAR, m1, m-m1 );
}

void VAR_Measurer::MPI_slaveThread()
{
	int mn[2];
	MPI_Bcast( mn, 2, MPI_INT, MPI_master, MPI_COMM_WORLD );
	int m = mn[0], n = mn[1];
	//cout << m << " " << n << " ";

	double *L = new double[m];
	MPI_Bcast( L, m, MPI_DOUBLE, MPI_master, MPI_COMM_WORLD );
	//for ( int i = 0; i < m; i++ ) cout << L[i] << " ";
	//cout << endl;

	double sums[2];
	MPI_Bcast( sums, 2, MPI_DOUBLE, MPI_master, MPI_COMM_WORLD );
	double sqSum = sums[0], sum = sums[1];
	//cout << sqSum << " " << sum << endl;


	int *I = new int[m];
	double *F = new double[m];
	MPI_Status sta;
	MPI_Recv( I, m, MPI_INT, MPI_master, 0, MPI_COMM_WORLD, &sta );
	MPI_Recv( F, m, MPI_DOUBLE, MPI_master, 1, MPI_COMM_WORLD, &sta );
	//for ( int i = 0; i < m; i++ ) cout << I[i] << " ";
	//cout << " ";
	//for ( int i = 0; i < m; i++ ) cout << F[i] << " ";
	//cout << endl;

	MS ms = VAR_Measurer::MPI_minVar( L, I, F, sqSum, sum, m );
	assert( ms.fIdx == -1 );

	//Send the res to master
		// 0 for spVal
		// 1 for VAR
		// 2 for 1
	double res[3];
	res[0] = ms.spVal; res[1] = ms.VAR; res[2] = ms.m1;
	MPI_Send( res, 3, MPI_DOUBLE, MPI_master, 99, MPI_COMM_WORLD );
}
