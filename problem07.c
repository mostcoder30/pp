#include <stdio.h>
#include <mpi.h>
 
int main(int argc, char **argv) {
    int rank, size, sendbuf[8], recvbuf[8];
    double t0, blocking_time, nonblocking_time;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size != 2) {
        if (rank == 0) printf("Run with exactly 2 processes.\\n");
        MPI_Finalize(); return 0;
    }
    for (int i = 0; i < 8; i++) sendbuf[i] = rank * 10 + i + (rank == 0 ? 10 : -9);
    /* Gives P0: 10..17 and P1: 1..8 */
    if (rank == 0) for (int i = 0; i < 8; i++) sendbuf[i] = 10 * (i + 1);
    else for (int i = 0; i < 8; i++) sendbuf[i] = i + 1;
 
    MPI_Barrier(MPI_COMM_WORLD);
    t0 = MPI_Wtime();
    MPI_Sendrecv(sendbuf, 8, MPI_INT, 1-rank, 70,
                 recvbuf, 8, MPI_INT, 1-rank, 70,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    blocking_time = MPI_Wtime() - t0;
 
    MPI_Barrier(MPI_COMM_WORLD);
    t0 = MPI_Wtime();
    MPI_Request req[2];
    MPI_Status statuses[2];
    MPI_Irecv(recvbuf, 8, MPI_INT, 1-rank, 71, MPI_COMM_WORLD, &req[0]);
    MPI_Isend(sendbuf, 8, MPI_INT, 1-rank, 71, MPI_COMM_WORLD, &req[1]);
    MPI_Waitall(2, req, statuses);
    nonblocking_time = MPI_Wtime() - t0;
 
    printf("P%d received:", rank);
    for (int i = 0; i < 8; i++) printf(" %d", recvbuf[i]);
    printf("\\nP%d blocking time = %.9f s; nonblocking time = %.9f s\\n",
           rank, blocking_time, nonblocking_time);
    MPI_Finalize();
    return 0;
}
