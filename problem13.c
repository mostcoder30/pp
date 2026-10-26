#include <stdio.h>
#include <mpi.h>
 
int main(int argc, char **argv) {
    int rank, size;
    int matrix[15] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    int counts[3] = {6,6,3}, displs[3] = {0,6,12};
    int local[6] = {0}, result[15] = {0};
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size != 3) {
        if (rank == 0) printf("Run with exactly 3 processes.\\n");
        MPI_Finalize(); return 0;
    }
    MPI_Scatterv(matrix, counts, displs, MPI_INT,
                 local, counts[rank], MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = 0; i < counts[rank]; i++) local[i] *= 2;
    printf("P%d processed %d rows (%d elements).\\n",
           rank, counts[rank]/3, counts[rank]);
    MPI_Gatherv(local, counts[rank], MPI_INT,
                result, counts, displs, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        printf("Final matrix:\\n");
        for (int i = 0; i < 15; i++) {
            printf("%3d", result[i]);
            if (i % 3 == 2) printf("\\n");
        }
    }
    MPI_Finalize();
    return 0;
}
