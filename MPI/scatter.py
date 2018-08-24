"""
an MPI.scatter example
"""
from __future__ import print_function

from mpi4py import MPI

def chunkIt(seq, num):
    avg = len(seq) / float(num)
    out = []
    last = 0.0

    while last < len(seq):
        out.append(seq[int(last):int(last + avg)])
        last += avg

    # Fix size, sometimes there is spillover
    # TODO: replace with while if problem persists
    if len(out)>num:
        out[-2].extend(out[-1])
        out=out[0:-1]

    if len(out)!=num:
        raise Exception("something went wrong in chunkIt, the target size differs from the actual size")

    return out

def main():
    comm = MPI.COMM_WORLD
    size = comm.Get_size()
    rank = comm.Get_rank()
    #print("Size:", size,"; Rank:", rank)

    if rank==0:
        input_ = [x for x in range(15)]
        allJobs = chunkIt(input_, size)
    else:
        allJobs = []

    # wait for all ranks
    comm.Barrier()
    rankJobs = comm.scatter(allJobs, root=0)
    print("[%i] sees %i items"%(rank, len(rankJobs)))

    # wait for all ranks
    comm.Barrier()
    res = [j**2 for j in rankJobs]


    output = comm.gather(res, root=0)
    if rank == 0:
        print("Input:", input_)
        print("Output:", [x for xx in output for x in xx])


if __name__ == "__main__":
    main()
