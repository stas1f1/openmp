#include <iostream>
#include "mpi.h"

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	//matrix initialization by each process
	int n = 500;
	int maxv = 1000;

	int** A = new int* [n];
	int** B = new int* [n];
	int** C = new int* [n];

	for (int i = 0; i < n; i++)
	{
		A[i] = new int[n];
		B[i] = new int[n];
		C[i] = new int[n];

		for (int j = 0; j < n; j++)
		{
			A[i][j] = rand() % maxv;
			B[i][j] = rand() % maxv;
			C[i][j] = 0;
		}
	}

	//barrier process synchronization

	MPI_Barrier(MPI_COMM_WORLD); 

	// start timing for each process

	double start = MPI_Wtime();

	// matrix multiplication

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			for (int k = 0; k < n; k++)
				C[i][j] += A[i][k] * B[k][j];

	// end of timing

	double finish = MPI_Wtime();
	double total_time = finish - start;

	// output the execution time of matrix multiplication at each process
	std::cout << "# of process: : " << rank 
		<< ", total time: " << total_time
		<< std::endl;

	MPI_Finalize();
}