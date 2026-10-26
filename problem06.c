#include<stdio.h>
#include<mpi.h>

int main(int argc , char **argv){
    int rank, size;
    int global[8] = {10, 20, 30, 40, 50, 60, 70, 80};
    int local[2], left_halo, right_halo, out[2];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(size != 4){
        if(rank == 0){
            printf("Run with exactly 4 processes.\n");
        }
        MPI_Finalize();
        return 0;
    }

    local[0] = global[rank * 2];
    local[1] = global[rank * 2 + 1];

    left_halo = local[0];
    right_halo = local[1];

    if(rank > 0){
        MPI_Sendrecv(&local[0], 1, MPI_INT, rank - 1, 40,
             &left_halo, 1, MPI_INT, rank -1 , 41,
              MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    if(rank < size - 1){
        MPI_Sendrecv(&local[1], 1, MPI_INT, rank + 1, 41, 
            &right_halo, 1, MPI_INT, rank + 1, 40,
        MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    out[0] = left_halo + local[0] + local[1];
    out[1] = right_halo + local[0] + local[1];

    if(rank == 0){ 
        out[0] = local[0] + local[1];
        printf("p%d : [%d %d] + right_halo %d \n", rank, local[0], local[1],global[rank *2 + 2]);
    }
    else if(rank < size - 1 ) {
        out[1] = local[0] + local[1] * 2;
        printf("p%d : left_halo %d + [%d %d] + right_halo %d \n", rank, global[rank * 2 - 1],local[0], local[1],global[rank * 2 + 2]);
    }else{
        printf("p%d : left_halo %d + [%d %d] \n", rank, global[rank * 2 - 1],local[0], local[1]);
    }
    
    int all_out[8];
    MPI_Gather(out, 2, MPI_INT, all_out, 2, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank == 0){
        printf("stencil result :");
        for(int i = 0 ; i < 8; i++){
            printf("%d ", all_out[i]);
        }
        printf("\n");
    }
    MPI_Finalize();
    return 0;
}