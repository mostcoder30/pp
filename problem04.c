#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

int main(int argc, char **argv){
    int rank,size , bytes = 1024, iterations = 5;
    char *buffer;
    double start, total;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(size != 2){
        if(rank == 0){
            printf("Run with exactly 2 processes.\n ");
        }
        MPI_Finalize();
        return 0;
    }

    if(argc > 1) bytes = atoi(argv[1]);
    if(bytes < 1) bytes = 1;

    buffer = (char *)malloc((size_t)bytes);

    if(!buffer) {MPI_Abort(MPI_COMM_WORLD, 1);}

    for(int i = 0; i < iterations ; i++){
        buffer[i] = (char)(i % 127);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    start = MPI_Wtime();
    for(int i = 0; i < iterations; i++){
        if(rank == 0){
            MPI_Send(buffer, bytes, MPI_CHAR, 1, 40, MPI_COMM_WORLD);
            MPI_Recv(buffer, bytes, MPI_CHAR, 1, 41, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }else{
            MPI_Recv(buffer, bytes, MPI_CHAR, 0, 40, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            MPI_Send(buffer, bytes, MPI_CHAR, 0, 41, MPI_COMM_WORLD);
        }
    }
    total = MPI_Wtime() - start;

    if(rank == 0){
        double avg_rt = total / iterations ;
        printf("Message size : %d bytes \nIterations : %d \ntotal round trips : %d\n ",
            bytes, iterations, iterations);
        printf("Average round_trip : %f seconds \n",avg_rt);
        printf("Average one_way : %f seconds \n", avg_rt / 2.0);
        printf("Approx. bandwidth: %f MB/s \n",
            (bytes / (avg_rt / 2.0)) / 1.0e6);
    }
    free(buffer);
    MPI_Finalize();
    return 0;
}