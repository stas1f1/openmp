#include <iostream> 
#include <string>
#include "mpi.h"

using namespace std;

int main(int argc, char** argv)
{
	int size, rank, position, i;
	float a[10];
	char b[10], buf[100];
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	for (i = 0; i < 10; i++) {
		a[i] = rank + 1.0;
		if (rank == 0) 
			b[i] = 'a';
		else 
			b[i] = 'b';
	}
	position = 0;

	cout << "Before packing & distribution, process " << rank
		<< ": \na = [";
	for (i = 0; i < 9; i++)
		cout << a[i] << ", ";
	cout << a[9] << "] \nb = [";
	for (i = 0; i < 9; i++)
		cout << b[i] << ", ";
	cout << b[9] << "]" << endl;

	if (rank == 0) {
		MPI_Pack(a, 10, MPI_FLOAT, buf, 100, &position, MPI_COMM_WORLD);
		MPI_Pack(b, 10, MPI_CHAR, buf, 100, &position, MPI_COMM_WORLD);
		MPI_Bcast(buf, 100, MPI_PACKED, 0, MPI_COMM_WORLD);
	}
	else {
		MPI_Bcast(buf, 100, MPI_PACKED, 0, MPI_COMM_WORLD);
		position = 0;
		MPI_Unpack(buf, 100, &position, a, 10, MPI_FLOAT, MPI_COMM_WORLD);
		MPI_Unpack(buf, 100, &position, b, 10, MPI_CHAR, MPI_COMM_WORLD);
	}
	MPI_Barrier(MPI_COMM_WORLD);

	cout << "After packing & distribution, process " << rank
		<< ": \na = [";
	for (i = 0; i < 9; i++)
		cout << a[i] << ", ";
	cout << a[9] << "] \nb = [";
	for (i = 0; i < 9; i++)
		cout << b[i] << ", ";
	cout << b[9] << "]" << endl;

	MPI_Finalize();
}