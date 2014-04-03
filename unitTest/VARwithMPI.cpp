#include <mpi.h>
#include "../VAR_Measurer.cpp"

void test1()
{
	TR_Data D;	
	VAR_Measurer ms;
	MS res;
	vector<int> r, c;	
	
	D.fmtread( "../test/case1.fmt" );
	r = vector<int>( D.getM(), 1 );
	c = vector<int>( D.getN(), 1 );
	res = ms.measure( D, r, c );

	assert( res.fIdx==0 );
	assert( isEqual(res.spVal, 0.6) );
	assert( isEqual(res.VAR, 0) );
	assert( res.m1==3 );
	assert( res.m2==2 );

	vector<int> part1 = ms.getPart1();
	vector<int> part2 = ms.getPart2();
	//disp( part1 );
	//disp( part2 );
	int p1[] = {1,1,1,0,0};
	int p2[] = {0,0,0,1,1};
	assert( isSame(part1,p1,sizeof(p1)/sizeof(int)) );
	assert( isSame(part2,p2,sizeof(p2)/sizeof(int)) );
}

void test2()
{
	TR_Data D;	
	VAR_Measurer ms;
	MS res;
	vector<int> r, c;	
	
	D.fmtread( "../test/case2.fmt" );
	r = vector<int>( D.getM(), 1 );
	c = vector<int>( D.getN(), 1 );
	res = ms.measure( D, r, c );

	//cout << res.fIdx << " " << res.spVal << " " << res.VAR << endl;
	//cout << res.m1 << " " << res.m2 << endl;
	
	assert( res.fIdx==2 );
	assert( isEqual(res.spVal, 0.5) );
	assert( isEqual(res.VAR, 0.13333) );
	assert( res.m1==9 );
	assert( res.m2==6 );

	vector<int> part1 = ms.getPart1();
	vector<int> part2 = ms.getPart2();
	//disp( part1 );
	//disp( part2 );
	int p1[] = {1,1,1,0,1,1,1,0,0,0,0,0,1,1,1};
	int p2[] = {0,0,0,1,0,0,0,1,1,1,1,1,0,0,0,};
	assert( isSame(part1,p1,sizeof(p1)/sizeof(int)) );
	assert( isSame(part2,p2,sizeof(p2)/sizeof(int)) );
}

int main( int argc, char* argv[] )
{
	const int master = 0;

	int rank, numprocs;

	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size( MPI_COMM_WORLD, &numprocs );

	if ( master == rank ) {		
		test1();
		//test2();	
		cout << "All Unit Cases Passed." << endl;
	}
	else {
		int mn[2];
		MPI_Bcast( mn, 2, MPI_INT, master, MPI_COMM_WORLD );
		int m = mn[0], n = mn[1];
		//cout << m << " " << n << " ";
	
		double *L = new double[m];
		MPI_Bcast( L, m, MPI_DOUBLE, master, MPI_COMM_WORLD );
		//for ( int i = 0; i < m; i++ ) cout << L[i] << " ";
		//cout << endl;

		double sums[2];
		MPI_Bcast( sums, 2, MPI_DOUBLE, master, MPI_COMM_WORLD );
		double sqSum = sums[0], sum = sums[1];
		//cout << sqSum << " " << sum << endl;


		int *I = new int[m];
		double *F = new double[m];
		MPI_Status sta;
		MPI_Recv( I, m, MPI_INT, master, 0, MPI_COMM_WORLD, &sta );
		MPI_Recv( F, m, MPI_DOUBLE, master, 1, MPI_COMM_WORLD, &sta );
		//for ( int i = 0; i < m; i++ ) cout << I[i] << " ";
		//cout << " ";
		//for ( int i = 0; i < m; i++ ) cout << F[i] << " ";
		//cout << endl;

		MS ms = VAR_Measurer::minVar( L, I, F, sqSum, sum, m );
		assert( ms.fIdx == -1 );

		//Send the res to master
			// 0 for spVal
			// 1 for VAR
			// 2 for 1
		double res[3];
		res[0] = ms.spVal; res[1] = ms.VAR; res[2] = ms.m1;
		MPI_Send( res, 3, MPI_DOUBLE, master, 99, MPI_COMM_WORLD );
	}


	MPI_Finalize();
	return 0;
}
