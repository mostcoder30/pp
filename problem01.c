#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int rank, size;
    int all[12] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120};
    int local[3], left = -1;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4)
    {
        if (rank == 0)
            printf("Run with exactly 4 processes.\\n");
        MPI_Finalize();
        return 0;
    }

    for (int i = 0; i < 3; i++)
        local[i] = all[rank * 3 + i];

    /*Receive from the left , then send to the right*/

    if (rank > 0)
        MPI_Recv(&left, 1, MPI_INT, rank - 1, 10,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    if (rank < size - 1)
        MPI_Send(&local[2], 1, MPI_INT, rank + 1, 10, MPI_COMM_WORLD);

    if (rank > 0)
        printf("p%d: Left boundary received = %d\\n", rank, left);
    if (rank < size - 1)
        printf("p%d: Right boundary sent = %d\\n", rank, local[2]);

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0)
        printf("Boundary exchange completed successfully . \\n");
    MPI_Finalize();
    return 0;
}
