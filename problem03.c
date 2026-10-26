#include<stdio.h>
#include<mpi.h>

int main(int argc, char **argv){
    int rank , size, send_value, recv_value;
    double start , elapsed;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    send_value = 100;

    int next = (rank + 1)% size;
    int prev = (rank - 1 + size) % size;

    MPI_Barrier(MPI_COMM_WORLD);

    start = MPI_Wtime();

    MPI_Sendrecv(&send_value, 1, MPI_INT, next, 30, 
        &recv_value, 1, MPI_INT, prev, 30,MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    elapsed = MPI_Wtime() - start ;

    printf("p%d sends %d to p%d \np%d receives %d from p%d\n",
            rank, send_value,  next, rank, recv_value, prev);

    double max_elapsed;
    MPI_Reduce(&elapsed, &max_elapsed, 1, 
        MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if( rank == 0){
        printf("Ring exchange completed . Maximum elapsed time = %f s\n",max_elapsed);
    }
    MPI_Finalize();
    return 0;


}