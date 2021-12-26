#include <iostream>
#include "mpi.h"

#define BUFSIZE 100

using namespace std;

void file_create(const char* filename, const char* data, int len) {
	MPI_File fh; 
	MPI_Status status; 
	MPI_File_open(MPI_COMM_WORLD, filename, 
		MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh); 
	MPI_File_write(fh, data, len, MPI_CHAR, &status);
	MPI_File_close(&fh); 
	return;
}


int main(int argc, char** argv)
{
	int bufsize, num, sum;
	MPI_Init(&argc, &argv);
	MPI_Status status;
	MPI_File fh;
	char buf[BUFSIZE];

	int del_after;

	file_create("file.txt", "some \nword", 10);

	MPI_File_open(MPI_COMM_WORLD, "file.txt", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
	MPI_File_set_view(fh, 0, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);
	sum = 0;
	do {
		MPI_File_read(fh, buf, BUFSIZE, MPI_CHAR, &status);
		MPI_Get_count(&status, MPI_CHAR, &num);
		printf("buf=%s\n", buf);
		sum += num;
	} while (num >= BUFSIZE);
	MPI_File_close(&fh);
	//print the number of read symbols sum from the file

	printf("The number of read symbols sum from the file %d\n", sum);
	printf("Write 1 to delete file, 0 to keep it: ");
	MPI_Barrier(MPI_COMM_WORLD);
	cin >> del_after;

	if (del_after)
	{
		MPI_File_delete("file.txt", MPI_INFO_NULL);
		printf("file.txt deleted successfully\n");
	}

	MPI_Finalize();
}