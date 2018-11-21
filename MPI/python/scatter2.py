#!/usr/bin/env python
"""
a simple scatter
"""

def main():
    from mpi4py import MPI

    comm = MPI.COMM_WORLD
    size = comm.Get_size()
    rank = comm.Get_rank()

    if rank == 0:
        data = [(x+1)**x for x in range(size*2)]
        print 'we will be scattering:',data
    else:
        data = None

    data = comm.scatter(data, root=0)
    print 'rank',rank,'has data:',data


if __name__ == "__main__":
    main()
