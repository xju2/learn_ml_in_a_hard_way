/* file: helloworld.c */

#include <mpi.h>
#include <stdio.h>
void sayhello(MPI_Comm comm)
{
  int size, rank;
  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);
  printf("Hello, World! "
         "I am process %d of %d.\n",
         rank, size);
}

