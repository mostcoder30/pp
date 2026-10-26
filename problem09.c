#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
 
static void sort2(int a[2]) {
    if (a[0] > a[1]) { int t = a[0]; a[0] = a[1]; a[1] = t; }
}
int main(int argc, char **argv) {
    int rank, size, data[8] = {45,12,78,3,56,23,89,10};
    int local[2], other[2], merged[4];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size != 4) {
        if (rank == 0) printf("Run with exactly 4 processes.\\n");
        MPI_Finalize(); return 0;
    }
    local[0] = data[rank*2]; local[1] = data[rank*2+1];
    sort2(local);
    printf("P%d local sorted: %d %d\\n", rank, local[0], local[1]);
 
    for (int phase = 0; phase < size; phase++) {
        int partner;
        if (phase % 2 == 0) partner = (rank % 2 == 0) ? rank+1 : rank-1;
        else partner = (rank % 2 == 0) ? rank-1 : rank+1;
        if (partner < 0 || partner >= size) continue;
 
        MPI_Sendrecv(local, 2, MPI_INT, partner, 90+phase,
                     other, 2, MPI_INT, partner, 90+phase,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        merged[0]=local[0]; merged[1]=local[1];
        merged[2]=other[0]; merged[3]=other[1];
        for (int i=0;i<4;i++)
            for (int j=i+1;j<4;j++)
                if (merged[i] > merged[j]) {
                    int t=merged[i]; merged[i]=merged[j]; merged[j]=t;
                }
        if (rank < partner) { local[0]=merged[0]; local[1]=merged[1]; }
        else { local[0]=merged[2]; local[1]=merged[3]; }
    }
 
    int sorted[8];
    MPI_Gather(local, 2, MPI_INT, sorted, 2, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        printf("Sorted array: ");
        for (int i=0;i<8;i++) printf("%d ", sorted[i]);
        printf("\\n");
    }
    MPI_Finalize();
    return 0;
}
