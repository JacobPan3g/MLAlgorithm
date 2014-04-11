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
	int *mapIdxs = new int[m];
	for ( int i=0,j=0; i < m; i++ ) {
		if ( !cs[i] ) continue;
		mL[j] = L[i];
		mapIdxs[i] = j;
		j++;
	}
	double mSums[2];
	mSums[0] = sqSums; mSums[1] = sums;

	//MPI Bcast the m, n, L, sqSums, sums
	MPI_Bcast( mn, 2, MPI_INT, 0, MPI_COMM_WORLD );
	MPI_Bcast( mL, nums, MPI_DOUBLE, 0, MPI_COMM_WORLD );
	MPI_Bcast( mSums, 2, MPI_DOUBLE, 0, MPI_COMM_WORLD );

	// each thread
	int numprocs;
	MPI_Comm_size( MPI_COMM_WORLD, &numprocs );
	// stop the more thread
	if ( numprocs > nums+1 )
		for ( int i=nums+1; i < numprocs; i++ )
			MPI_Send( 0, 0, MPI_INT, i, 0, MPI_COMM_WORLD );
	
	int fIdx = 0;
	int *mI = new int[nums];
	double *mF = new double[nums];
	//cout << "numprocs: " << numprocs << endl;
	for ( int i = 0; i < (numprocs-1<nums?numprocs-1:nums); i++ )	
	{
		// to correct the fIdx
		if ( fIdx >= n ) break;
		while ( !fs[fIdx] ) {
			fIdx++;
			if ( fIdx >= n ) break;
		}
		if ( fIdx >= n ) break;
		
		//MPI Send the cow of A
		list< pair<int,double> >::const_iterator it = fmtV[fIdx].begin();
		for ( int j=0; it != fmtV[fIdx].end(); it++ ) {
			if ( !cs[it->first] ) continue;
			mI[j] = mapIdxs[it->first];
			mF[j] = it->second;
			j++;
		}
		//MPI_TAG == 0 for stop signal
		//MPI_TAG = fIdx + 1
		MPI_Send( mI, nums, MPI_INT, i+1, fIdx+1, MPI_COMM_WORLD );
		MPI_Send( mF, nums, MPI_DOUBLE, i+1, fIdx+1, MPI_COMM_WORLD );
		fIdx++;
	}
		
	//MPI Receive the res
		// 0 for spVal
		// 1 for VAR
		// 2 for 1
	double res[3];
	MPI_Status sta;
	int sender;
	int recvIdx;
	for ( int i = 0; i < n; i++ ) {
		if ( !fs[i] ) continue;
		
		//cout << "waiting recv" << endl;
		MPI_Recv( &res, 3, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &sta );
		double VAR = res[1];
		sender = sta.MPI_SOURCE;
		recvIdx = sta.MPI_TAG;
		//cout << "FIdx,spVal,VAR: " << recvIdx<<","<<res[0]<<"," << VAR << endl;

		if ( VAR < this->minVAR || (VAR == this->minVAR && recvIdx < this->minFIdx) ) { 
			this->minFIdx = recvIdx;
			this->minSpVal = res[0];
			this->minVAR = VAR;
			this->m1 = (int)res[2];
			this->m2 = nums - m1;
		}

		// if not finish send
		if ( fIdx < n ) {
			// to correct the fIdx
			//cout << "not finish" << endl;
			while ( !fs[fIdx] ) {
				fIdx++;
				if ( fIdx >= n ) break;
			}
			if ( fIdx >= n ) break;
			
			//MPI Send the cow of A
			list< pair<int,double> >::const_iterator it = fmtV[fIdx].begin();
			for ( int j=0; it != fmtV[fIdx].end(); it++ ) {
				if ( !cs[it->first] ) continue;
				mI[j] = mapIdxs[it->first];
				mF[j] = it->second;
				j++;
			}
			MPI_Send( mI, nums, MPI_INT, sender, fIdx+1, MPI_COMM_WORLD );
			MPI_Send( mF, nums, MPI_DOUBLE, sender, fIdx+1, MPI_COMM_WORLD );
			fIdx++;
		}
		else {	// stop the thread
			//cout << "fIdx: " << fIdx << endl;
			MPI_Send( 0, 0, MPI_INT, sender, 0, MPI_COMM_WORLD );
		}
	}
	delete[] mL;
	delete[] mapIdxs;
	delete[] mI;
	delete[] mF;

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

	
	cout << "minFIdx: " << this->minFIdx << endl;
	//cout << "minVAR: " << this->minVAR << endl;
	//cout << "minSpVal: " << this->minSpVal << endl;
	//cout << "m1, m2: " << this->m1 << ", " << this->m2 << endl;
	

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

	//cout << "sp,var: ";

	int numSp = 0;	// num of sp
	for ( int i = 0; i < m; i++ ) {
		if ( isFirst ) {
			isFirst = false;
			beforeItem = F[i];
		}
		else {
			if ( beforeItem != F[i] ) {
				double sp = (F[i]+beforeItem)/2;
				//cout << "sum1,num1: " << sum1 << "," << num1 << " ";
				double var = computeVAR( sqSums, sums, m, sum1, num1 );
				
				//cout << sp << "," << var << "; ";

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

	//cout << endl;
	return MS( -1, minSp, minVAR, m1, m-m1 );
}

void VAR_Measurer::MPI_slaveThread()
{
	int rank;
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	//cout << "Thread " << rank << " Start." << endl;
	
	while( 1 ) {
		int mn[2];
		//cout << "Thread " << rank << " wait mn." << endl;
		MPI_Bcast( mn, 2, MPI_INT, MPI_master, MPI_COMM_WORLD );
		//cout << "Thread " << rank << " recv mn." << " | ";
		int m = mn[0], n = mn[1];
		//cout << m << " " << n << " ";
		if ( m==-1&&n==-1 ) break;

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
		while (1) {
			//cout << "Thread " << rank << " wait I." << endl;
			MPI_Recv( I, m, MPI_INT, MPI_master, MPI_ANY_TAG, MPI_COMM_WORLD, &sta );
			//cout << "Thread " << rank << " recv I." << " | ";
			//cout << "tag: " << sta.MPI_TAG << " ";
			if ( sta.MPI_TAG == 0 ) break;
			
			int fIdx = sta.MPI_TAG-1;
			MPI_Recv( F, m, MPI_DOUBLE, MPI_master, MPI_ANY_TAG, MPI_COMM_WORLD, &sta );
			assert( fIdx==sta.MPI_TAG-1 );
			//cout << " |";
			//for ( int i = 0; i < m; i++ ) cout << L[i] << " ";
			//cout << "| ";
			//cout << fIdx << " ";
			//for ( int i = 0; i < m; i++ ) cout << I[i] << " ";
			//cout << " I ";
			//for ( int i = 0; i < m; i++ ) cout << F[i] << " ";
			//cout << endl;

			MS ms = VAR_Measurer::MPI_minVar( L, I, F, sqSum, sum, m );
			assert( ms.fIdx == -1 );

			//cout << "res: " << ms.VAR << endl;

			//Send the res to master
				// 0 for spVal
				// 1 for VAR
				// 2 for 1
			double res[3];
			res[0] = ms.spVal; res[1] = ms.VAR; res[2] = ms.m1;
			MPI_Send( res, 3, MPI_DOUBLE, MPI_master, fIdx, MPI_COMM_WORLD );
			//cout << "send slave: " << MPI_master  << endl;
		}
		delete[] L;
		delete[] I;
		delete[] F;
	}
	cout << "finish slave:" << rank  << endl;
}

void VAR_Measurer::MPI_stopSlaveThread()
{
/*	int numprocs;
	MPI_Comm_size( MPI_COMM_WORLD, &numprocs );

	for ( int i=1; i < numprocs; i++ ) {
		MPI_Send( 0, 0, MPI_INT, i, 0, MPI_COMM_WORLD );	
	}
*/
	int mn[2] = {-1, -1};
	MPI_Bcast( mn, 2, MPI_INT, 0, MPI_COMM_WORLD );
}
