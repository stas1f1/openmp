#include<iostream>
#include"mpi.h"
#define VEC_SZ 1000000


using namespace std;
int main(int argc, char** argv) {
    int v1[VEC_SZ], v2[VEC_SZ];
    int start, finish, chunk_sz, leftover_sz;
    int result = 0;
    MPI_Init(&argc, &argv);
    int rank, n, i, message;
    MPI_Status status;

    MPI_Comm_size(MPI_COMM_WORLD, &n);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    chunk_sz = VEC_SZ / n;
    leftover_sz = VEC_SZ % n;

    if (rank == 0) {
        int subproc_res;
        for (int i = 0; i < VEC_SZ; i++) {
            v1[i] = 1;
            v2[i] = 1;
        }

        start = 0;
        finish = start + chunk_sz + leftover_sz;

        for (int i = 1; i < n; i++) {
            start = finish;
            finish = min(start + chunk_sz, VEC_SZ);
            MPI_Send(&v1[start], finish - start, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&v2[start], finish - start, MPI_INT, i, 2, MPI_COMM_WORLD);
        }

        start = 0;
        finish = start + chunk_sz + leftover_sz;

        for (int i = start; i < finish; i++)
            result += v1[i] * v2[i];

        for (int i = 1; i < n; i++) {
            MPI_Recv(&subproc_res, chunk_sz, MPI_INT, MPI_ANY_SOURCE, 3, MPI_COMM_WORLD, &status);
            result += subproc_res;
        }

        cout << "Result: " << result << endl;
    }
    else {
        MPI_Recv(&v1, chunk_sz, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&v2, chunk_sz, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
        for (int i = 0; i < chunk_sz; i++)
            result += v1[i] * v2[i];
        MPI_Send(&result, 1, MPI_INT, 0, 3, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}