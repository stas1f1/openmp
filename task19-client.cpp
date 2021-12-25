#include <iostream>
#include <string>
#include "mpi.h"

using namespace std;

int main(int argc, char** argv)
{
	int rank;
	int message = 42, recv_message;
	MPI_Init(&argc, &argv);
	char port_name[MPI_MAX_PORT_NAME];
	MPI_Status status;
	MPI_Comm intercomm;
	strcpy_s(port_name, argv[1]);

	MPI_Comm_connect(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm);
	cout << "Client connection successful" << endl;

	MPI_Send(&message, 1, MPI_INT, 0, 0, intercomm);
	cout << "Client sends: " << to_string(message) << endl;

	MPI_Recv(&recv_message, 1, MPI_INT, 0, 0, intercomm, &status);
	cout << "Client recieves: " << to_string(recv_message) << endl;
	MPI_Finalize();
	return 0;
}