#include<stdio.h>
#include<mpi.h>

int main(int argc , char **argv){
    int rank , size ;
    int a[2][2] = {{1, 2},{3, 4}};
    int b[2][2] = {{5, 6},{7, 8}};

    int rowa[2], c[2][2], rowc[2];
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(size != 2){
        if(rank == 0){
            printf("Run with exactly 2 processes. \n");
        }
        MPI_Finalize();
        return 0;
    }
    
    MPI_Bcast(b, 4, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(a, 2, MPI_INT, rowa, 2, MPI_INT, 0, MPI_COMM_WORLD);

    for(int j = 0; j < 2; j++){
        rowc[j] = rowa[0] * b[0][j] + rowa[1] * b[1][j]; // b - column 00,10/ 01,11
        
    }

    MPI_Gather(rowc, 2, MPI_INT, c, 2, MPI_INT, 0, MPI_COMM_WORLD);
    if(rank == 0){
        printf("Matrix c = A * B \n");
        for(int i = 0; i < 2; i++){
            printf("%d %d\n",c[i][0],c[i][1]);
        }
    }
    MPI_Finalize();
    return 0;
}