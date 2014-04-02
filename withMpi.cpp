#include <mpi.h>
#include "Boosting_Predictor.cpp"

int main( int argc, char* argv[] )
{
	const int master = 0;

	int rank, numprocs;

	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size( MPI_COMM_WORLD, &numprocs );

	if ( master == rank ) {		
		TR_Data D;
		D.fmtread( "../dataset/pro1.fmt" );
		
		Boosting_Predictor bt( bagNum, maxH );
		bt.train( D );
	}
	
	MPI_Finalize();
	return 0;
}
