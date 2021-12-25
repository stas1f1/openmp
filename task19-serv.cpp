#include <iostream>
#include <string>
#include "mpi.h"

using namespace std;

int main(int argc, char** argv)
{
	int recv_message;
	int message = 25;
	MPI_Init(&argc, &argv);
	char port_name[MPI_MAX_PORT_NAME];
	MPI_Status status;
	MPI_Comm intercomm;

	MPI_Open_port(MPI_INFO_NULL, port_name);
	cout << "Portname: " << port_name << endl;

	MPI_Comm_accept(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm);
	cout << "Server connection successful" << endl;

	MPI_Send(&message, 1, MPI_INT, 0, 0, intercomm);
	cout << "Server sends: " << message << endl;

	MPI_Recv(&recv_message, 1, MPI_INT, 0, 0, intercomm, &status);
	cout << "Server receives: " << recv_message << endl;

	MPI_Comm_free(&intercomm);
	MPI_Close_port(port_name);

	MPI_Finalize();
	return 0;
}