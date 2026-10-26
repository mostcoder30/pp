#include <stdio.h>
#include <mpi.h>
 
int main(int argc, char **argv) {
    int rank, size, send_value, recv_value;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size != 2) {
        if (rank == 0) printf("Run with exactly 2 processes.\\n");
        MPI_Finalize(); return 0;
    }
    send_value = (rank == 0) ? 100 : 200;
 
    /* Safe exchange: send and receive are paired. */
    MPI_Sendrecv(&send_value, 1, MPI_INT, 1-rank, 80,
                 &recv_value, 1, MPI_INT, 1-rank, 80,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("P%d sent %d and received %d\\n", rank, send_value, recv_value);
    if (rank == 0) printf("Communication completed without deadlock.\\n");
    MPI_Finalize();
    return 0;
}
 
/*
Unsafe pattern (do not run as the main program):
  MPI_Send(..., 1-rank, ...);
  MPI_Recv(..., 1-rank, ...);
If both sends wait for receives, the program may deadlock.
*/
