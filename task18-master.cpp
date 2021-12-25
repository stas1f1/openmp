#include "mpi.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char** argv)
{
	int size, rank1, rank2, total_processes;
	MPI_Status status;
	MPI_Comm intercomm;
	char slave[20] = "task18-slave";
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_spawn(slave, MPI_ARGV_NULL, 3, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm, MPI_ERRCODES_IGNORE);
	MPI_Recv(&rank1, 1, MPI_INT, 0, 0, intercomm, &status);
	MPI_Recv(&rank2, 1, MPI_INT, 1, 1, intercomm, &status);
	//Display "Slaves rank1 and rank2 are working", instead of the words rank1 and rank2 their values should be displayed.
	cout << "Slave #1 (rank " + to_string(rank1) +
		") and #2 (rank " + to_string(rank2) +
		") are working" << endl;

	MPI_Recv(&total_processes, 1, MPI_INT, 2, 2, intercomm, &status);
	cout << "Slave #3 (rank " + to_string(status.MPI_SOURCE) +
		") says that there are " + to_string(total_processes)
		+ " processes total" << endl;

	MPI_Finalize();
	return 0;
}