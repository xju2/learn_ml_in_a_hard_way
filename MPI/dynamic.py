#!/usr/bin/env python
"""
The *Mater* has n nodes (specified in mpirun -n),
each node spawns 5 processes that calculates the PI.
Yes, each node does exactly the same thing in this simple
example. So you will see PI is printed n times
"""
from mpi4py import MPI
import numpy
import sys


comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

comm = MPI.COMM_SELF.Spawn(sys.executable,
                           args=['cpi.py'],
                           maxprocs=5)

N = numpy.array(200, 'i')

# send N to each process
comm.Bcast([N, MPI.INT], root=MPI.ROOT)
PI = numpy.array(0.0, 'd')
comm.Reduce(None, [PI, MPI.DOUBLE],
            op=MPI.SUM, root=MPI.ROOT)

print(PI)
comm.Disconnect()
