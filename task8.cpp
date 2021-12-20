#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "mpi.h"

int main(int argc, char* argv[]) {

    MPI_Init(&argc, &argv);
    int rank, n;
    double start, finish, total_time;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &n);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int runs = 10;

    for (int msg_sz = 1; msg_sz <= 1000000; msg_sz *= 10) {
        int* v = (int*)malloc(msg_sz * sizeof(int));

        if (rank == 0) 
            start = MPI_Wtime();


        for (int i = 1; i <= runs; i++) {

            if (rank == 0) {
                MPI_Send(v, msg_sz, MPI_INT, 1, 1, MPI_COMM_WORLD);
                MPI_Recv(v, msg_sz, MPI_INT, 1, 2, MPI_COMM_WORLD, &status);

            }
            else if (rank == 1) {
                MPI_Recv(v, msg_sz, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
                MPI_Send(v, msg_sz, MPI_INT, 0, 2, MPI_COMM_WORLD);
            }
        }

        if (rank == 0) {
            finish = MPI_Wtime();
            total_time = finish - start;
            double MB_sz = (double)(sizeof(int) * msg_sz) / (1 << 20); // in MB
            double bandwidth = MB_sz * 2.0 * (double)runs / total_time;
            std::cout << "Length of a message = " << msg_sz << ", bandwidth = " << sizeof(int) * bandwidth << "MB/s\n ";
        }
        free(v);
    }

    if (rank == 0) 
        start = MPI_Wtime();

    for (int i = 1; i <= runs; i++) {
        if (rank == 0) {
            MPI_Send(&i, 0, MPI_INT, 1, 1, MPI_COMM_WORLD);
            MPI_Recv(&i, 0, MPI_INT, 1, 2, MPI_COMM_WORLD, &status);

        }
        else if (rank == 1) {
            MPI_Recv(&i, 0, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
            MPI_Send(&i, 0, MPI_INT, 0, 2, MPI_COMM_WORLD);
        }

    }

    if (rank == 0) {
        finish = MPI_Wtime();
        total_time = finish - start;
        std::cout << "Latency = " << total_time / (2 * runs) << std::endl;
    }

    MPI_Finalize();
    return 0;
}