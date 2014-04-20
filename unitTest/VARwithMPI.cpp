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
	res = ms.MPI_measure( D, r, c );

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
	res = ms.MPI_measure( D, r, c );

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

void pro1Test()
{
	time_t tic = clock();

	TR_Data D;	
	VAR_Measurer ms;
	MS res;
	vector<int> r, c;	
	
	D.fmtread( "../dataset/pro1.fmt" );
	r = vector<int>( D.getM(), 1 );
	c = vector<int>( D.getN(), 1 );
	res = ms.MPI_measure( D, r, c );

	//cout << res.fIdx << " " << res.spVal << " " << res.VAR << endl;
	//cout << res.m1 << " " << res.m2 << endl;
	
	assert( res.fIdx==38 );
	assert( isEqual(res.spVal, 0.748239) );
	assert( isEqual(res.VAR, 0.279294) );
	assert( res.m1==6336 );
	assert( res.m2==2790 );

	time_t toc = clock();
	cout << "Time: " << (double)(toc-tic)/CLOCKS_PER_SEC << "s"<< endl;
}

int main( int argc, char* argv[] )
{
	const int master = 0;

	int rank, numprocs;

	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size( MPI_COMM_WORLD, &numprocs );

	if ( master == rank ) {	
		//cout << "numprocs: " << numprocs << endl;
		test1();
		test2();
		pro1Test();
		cout << "All Unit Cases Passed." << endl;
		VAR_Measurer::MPI_stopSlaveThread();
	}
	else {
		//if ( rank >= 4 ) cout << "66666";
		VAR_Measurer::MPI_slaveThread();
	}


	MPI_Finalize();
	return 0;
}
