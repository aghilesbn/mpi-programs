#include <stdio.h>
#include <mpi.h>

/*
 * ring_nonblocking.c
 * Demonstrates non-blocking point-to-point communication in a ring topology.
 * This version avoids deadlock by using MPI_Isend and MPI_Irecv.
 */

int main(int argc, char **argv) {
    int rank, size;
    int sendbuf, recvbuf;
    int succ, pred;
    MPI_Request reqs[2];
    MPI_Status stats[2];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    succ = (rank + 1) % size;
    pred = (rank - 1 + size) % size;

    sendbuf = rank;

    MPI_Irecv(&recvbuf, 1, MPI_INT, pred, 0, MPI_COMM_WORLD, &reqs[0]);
    MPI_Isend(&sendbuf, 1, MPI_INT, succ, 0, MPI_COMM_WORLD, &reqs[1]);

    MPI_Waitall(2, reqs, stats);

    printf("Process %d sent %d to %d and received %d from %d\n",
           rank, sendbuf, succ, recvbuf, pred);

    MPI_Finalize();
    return 0;
}
