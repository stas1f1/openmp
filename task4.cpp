#include <iostream>
#include "mpi.h"
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);
	int rank, n, i, message;
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD, &n);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	std::srand(rank * std::time(nullptr));
	int v = rand() % n;
	int * send = &v;
	int * recv = (int*)malloc(n * sizeof(int));

	MPI_Allgather(send, 1, MPI_INT, recv, 1, MPI_INT, MPI_COMM_WORLD);

	string output = "Gathered at rank " + std::to_string(rank) + ": ";

	for (int i = 0; i < n; i++)
	{
		if (recv[i] > rank)
			output += std::to_string(recv[i]) + " ";
 	}

	cout << output << endl;

	MPI_Finalize();
	return 0;
}