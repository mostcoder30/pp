#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int rank, size, row[4], sum = 0, total = 0;
    int matrix[4][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};

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

    if (rank == 0)
    {
        for (int j = 0; j < 4; j++)
            row[j] = matrix[0][j];

        for (int p = 1; p < size; p++)
            MPI_Send(matrix[p], 4, MPI_INT, p, 20, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Recv(row, 4, MPI_INT, 0, 20, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    for (int j = 0; j < 4; j++)
        sum += row[j];

    printf("p%d: Row = %d %d %d %d, Sum = %d\\n",
           rank, row[0], row[1], row[2], row[3], sum);

    if (rank == 0)
    {
        total = sum;
        for (int p = 1; p < size; p++)
        {
            int worker_sum;
            MPI_Recv(&worker_sum, 1, MPI_INT, p, 21,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total += worker_sum;
        }
        printf("Total matrix sum = %d\\n", total);
    }
    else
    {
        MPI_Send(&sum, 1, MPI_INT, 0, 21, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}