#include <stdio.h>
#include <mpi.h>
 
int main(int argc, char **argv) {
    int rank, iterations, threshold, block_size, config[3];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        config[0] = 1000;
        config[1] = 50;
        config[2] = 256;
    }
    MPI_Bcast(config, 3, MPI_INT, 0, MPI_COMM_WORLD);
    iterations = config[0]; threshold = config[1]; block_size = config[2];
    printf("P%d received: Iterations=%d, Threshold=%d, Block size=%d\\n",
           rank, iterations, threshold, block_size);
    MPI_Finalize();
    return 0;
}
