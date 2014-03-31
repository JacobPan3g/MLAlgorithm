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
 *	Call:	getSplitPoints(), computeVAR()
 *	Update:	sp, idxs, part1, part2, num1, num2
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
	this->sums = 0;
	this->sqSums = 0;
	this->nums = 0;
	for ( int i = 0; i < L.size(); i++ ) {
		if ( cs[i] ) {
			this->sums += L[i];
			this->sqSums += pow( L[i], 2 );
			this->nums++;
		}
	}

	this->getSplitPoints( fmtV, L, m, n, cs );

	// calculate the variance
	this->vars.resize( n );
	int minFIdx = -1;
	int minSpIdx = -1;
	double minVar = 1e8;

	// each feature
	for ( int i = 0; i < n; i++ )	
	{
		if ( !fs[i] )
			continue;
	
		// each split point
		int sz = this->sp[i].size();
		this->vars[i] = vector<double>( sz );
		for ( int k = 0; k < sz; k++ ) {
			// for calculation
/*			double N1 = this->num1s[i][k] + 1e-8;
			double N2 = this->nums - this->num1s[i][k] + 1e-8;
			double sqSum1 = this->sqSums1[i][k];
			double sqSum2 = this->sqSums - this->sqSums1[i][k];
			double sum1 = this->sums1[i][k];
			double sum2 = this->sums - this->sums1[i][k];
			double aver1 = sum1/N1;
			double aver2 = sum2/N2;
			double sqAver1 = pow( aver1, 2 );
			double sqAver2 = pow( aver2, 2 );

			double var1 = sqSum1/N1 - 2*aver1*sum1/N1 + sqAver1;
			double var2 = sqSum2/N2 - 2*aver2*sum2/N2 + sqAver2;
			
			double N = N1 + N2;
			assert( N != 0 );
			double var = N1/N*var1 + N2/N*var2;
			this->vars[i][k] = var;
*/

			double m1 = this->num1s[i][k] + 1e-8;
			double m2 = this->nums - this->num1s[i][k] + 1e-8;
			double sum1 = this->sums1[i][k];
			double sum2 = this->sums - this->sums1[i][k];

			double M = m1 + m2;
			assert( M != 0 );
			double var = (this->sqSums - pow(sum1,2)/m1 - pow(sum2,2)/m2) / M;
			this->vars[i][k] = var;

			if ( var < minVar ) {
				minFIdx = i;
				minSpIdx = k;
				minVar = var;
			}
		}
	}

	// set the result
	this->fIdx = minFIdx;
	this->spIdx = minSpIdx;
	this->obVal = this->sp[minFIdx][minSpIdx];
	this->part1 = vector<int>( m, 0 );
	this->part2 = cs;
	
	list< pair<int,double> >::const_iterator it = fmtV[minFIdx].begin();
	for ( ; it != fmtV[this->fIdx].end(); it++ ) {
		if ( cs[it->first] ) {
			if ( it->second > obVal )
				break;
			this->part1[it->first] = 1;
			this->part2[it->first] = 0;
		}
	}

	// Assert
	assert( countTag(cs)==this->nums );
	assert( countTag(this->part1)==this->getNum1() );
	assert( countTag(this->part2)==this->getNum2() );
	assert( this->fIdx < n );
	assert( this->spIdx < this->sp[this->fIdx].size() );

	return MS(minFIdx,this->obVal,minVar);

//	return MS( 0, 0, 0 );
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

// getter
vector< vector<int> > VAR_Measurer::getIdxs() const
{
	return this->idxs;
}

vector< vector<double> > VAR_Measurer::getSp() const
{
	return this->sp;
}

vector< vector<double> > VAR_Measurer::getVars() const
{
	return this->vars;
}


const vector<int>& VAR_Measurer::getPart1() const
{
	return this->part1;
}

const vector<int>& VAR_Measurer::getPart2() const
{
	return this->part2;
}

int VAR_Measurer::getNum1( int i, int k ) const
{
	if ( i==-1&&k==-1 ) {
		i = this->fIdx;
		k = this->spIdx;
	}

	return this->num1s[i][k];
}

int VAR_Measurer::getNum2( int i, int k ) const
{
	if ( i==-1&&k==-1 ) {
		i = this->fIdx;
		k = this->spIdx;
	}

	return this->nums - this->num1s[i][k];
}


// Private Method

/* Function: getSplitPoints()
 *		-- use fmtV to find all sp for all F
 *						and sum，sqSum of part 1
 *	Update:	sp, idxs
 *	Return: void 
 */
void VAR_Measurer::getSplitPoints( const vector< list< pair<int,double> > >& fmtV, const vector<double>& L, int m, int n, vector<int> cs )
{
	assert( fmtV.size()==n );
	assert( fmtV[0].size()==m );
	assert( cs.size()==m );

	this->sp.resize( n );
	this->idxs.resize( n );
	this->sums1.resize( n );
	///this->sqSums1.resize( n );


	this->num1s.resize( n );
	//this->part1s.resize( n );
	//this->part2s.resize( n );
	
	for ( int i = 0; i < n; i++ ) {
		this->sp[i] = vector<double>( m );
		this->idxs[i] = vector<int>( m );
		this->sums1[i] = vector<double>( m, 0 );
		///this->sqSums1[i] = vector<double>( m, 0 );

		this->num1s[i] = vector<int>( m, 0 );
		//this->part1s[i].resize( m );
		//this->part2s[i].resize( m );

		bool isFirst = true;
		double beforeItem;		// the front diff Item

		double sum1 = 0;
		///double sqSum1 = 0;
		int num1 = 0;	// num of 1st part
		//vector<int> p1( m, 0 );
		//vector<int> p2( cs );

		list< pair<int,double> >::const_iterator it = fmtV[i].begin();
		int num = 0;	// num of sp
		int idx = 0;	// idx at all cases
		for ( ; it != fmtV[i].end(); it++, idx++ ) {
			if ( cs[it->first] ) {
				//num1++;
				if ( isFirst ) {
					isFirst = false;
					beforeItem = it->second;
				}
				else {
					if ( beforeItem != it->second ) {
						
						this->idxs[i][num] = idx;
						this->sp[i][num] = (it->second+beforeItem)/2;
						this->sums1[i][num] = sum1;
						///this->sqSums1[i][num] = sqSum1;
						
						this->num1s[i][num] = num1;
						//this->part1s[i][num] = p1;
						//this->part2s[i][num] = p2;

						beforeItem = it->second;
						num++;
					}
				}
				// set the part
				//p1[it->first] = 1;
				//p2[it->first] = 0;
				
				// calculate the sum
				sum1 += L[it->first];
				///sqSum1 += pow( L[it->first], 2 );
				num1++;
			}
		}

		if ( num == 0 ) {
			this->idxs[i][num] = idx;
			this->sp[i][num] = fmtV[i].begin()->second;
			this->sums1[i][num] = sum1;
			///this->sqSums1[i][num] = sqSum1;
			
			this->num1s[i][num] = num1;
			//this->part1s[i][num] = p1;
			//this->part2s[i][num] = p2;
			
			num = 1;
		}

		// correct the size
		int sz = num;
		this->sp[i].resize( sz );
		this->idxs[i].resize( sz );
		this->sums1[i].resize( sz );
		///this->sqSums1[i].resize( sz );
		this->num1s[i].resize( sz );
		//this->part1s[i].resize( sz );
		//this->part2s[i].resize( sz );
	
		// Assert
		int ass_sp_num = this->sp[i].size();
		assert( ass_sp_num <= m );
		assert( ass_sp_num==this->idxs[i].size() );
		assert( ass_sp_num==this->sums1[i].size() );
		///assert( ass_sp_num==this->sqSums1[i].size() );
		assert( ass_sp_num==this->num1s[i].size() );
		//assert( ass_sp_num==this->part1s[i].size() );
		//assert( ass_sp_num==this->part2s[i].size() );
		
		srand( (int)time(0) );
		if ( ass_sp_num != 0 ) {
			int ass_idx = rand() % ass_sp_num;
			//assert( this->sums1[i][ass_idx] <= this->sums );
			///assert( this->sqSums1[i][ass_idx] <= this->sqSums+1e-5 );
			assert( this->num1s[i][ass_idx] <= this->nums );
			assert( this->num1s[i][ass_idx] <= this->idxs[i][ass_idx] );
			//assert( countTag(this->part1s[i][ass_idx])+countTag(this->part2s[i][ass_idx])==this->nums[i] );	
		}
	}
}
