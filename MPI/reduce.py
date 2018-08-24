"""
MPI reduction
"""
from __future__ import print_function

from mpi4py import MPI
import numpy as np

def multiplication(arr):
    res = 1
    for x in arr:
        res = res*x
    return res


def main():
    comm = MPI.COMM_WORLD
    size = comm.Get_size()
    rank = comm.Get_rank()

    value = np.array(rank,'d')

    print(' Rank: ',rank, ' value = ', value)

    # initialize the np arrays that will store the results:
    value_sum      = np.array(0.0,'d')
    value_max      = np.array(0.0,'d')
    value_mul      = np.array(0.0,'d')

    # perform the reductions:
    comm.Reduce(value, value_sum, op=MPI.SUM, root=0)
    comm.Reduce(value, value_max, op=MPI.MAX, root=0)
    #comm.Reduce(value, value_mul, op=multiplication, root=0)

    if rank == 0:
        print(' Rank 0: value_sum =    ',value_sum)
        print(' Rank 0: value_max =    ',value_max)
        print(' Rank 0: value_mul =    ',value_mul)



if __name__ == "__main__":
    main()
