#include <iostream> 
#include <stdlib.h> 
#include "mpi.h" 

using namespace std;

int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	const int arr_sz = 1000000;
	const int size_ = 4;
	const int chunk_sz = 250000;

	int init[arr_sz];
	int init_[arr_sz];
	double start, end;
	int sub_sums[size_];

	if (rank == 0)
		for (int i = 0; i < arr_sz; i++)
			init[i] = 1;

	start = MPI_Wtime();
	MPI_Scatter(init, chunk_sz, MPI_INT, &init_, arr_sz / size, MPI_INT, 0, MPI_COMM_WORLD);

	long int sub_sum = 0;
	for (int i = 0; i < chunk_sz; i++)
		sub_sum = sub_sum + init_[i];

	MPI_Gather(&sub_sum, 1, MPI_INT, &sub_sums, 1, MPI_INT, 0, MPI_COMM_WORLD);
	

	long int sum = 0;
	if (rank == 0) {
		sum = 0;
		for (int i = 0; i < size; i++)
			sum += sub_sums[i];

		std::cout << "Cascade scheme total sum: " << sum << std::endl;
		double total_time = MPI_Wtime() - start;
		std::cout << "Execution time for cascade is: " << total_time << std::endl;
	}

	//MPI_reduce variant

	start = MPI_Wtime();
	MPI_Scatter(init, chunk_sz, MPI_INT, &init_, chunk_sz, MPI_INT, 0, MPI_COMM_WORLD);

	sub_sum = 0;
	for (int i = 0; i < chunk_sz; i++) 
		sub_sum += init_[i];

	sum = 0;
	MPI_Reduce(&sub_sum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		std::cout << "MPI_Reduce total sum: " << sum << std::endl;
		double total_time = MPI_Wtime() - start;
		std::cout << "Execution time for MPI_Reduce is: " << total_time << std::endl;
	}

	MPI_Finalize();
	return 0;
}