#!/usr/bin/env python
import os
import subprocess
import threading
import time

grid_pack_file = '/global/homes/x/xju/atlas/testarea/mg5_py8_dir/submit/000000/GridPack/run_gridpack.tar.gz'
evts_per_thread = 2000

def process_event(run_name):
    #print(type(run_name))
    cmd = ['./run.sh', run_name]
    #cmd = ['echo', "running "+run_name]
    subprocess.call(cmd, shell=True)


if __name__ == "__main__":

    import numpy as np

    try:
        from mpi4py import MPI
        comm = MPI.COMM_WORLD
        size = comm.Get_size()
        rank = comm.Get_rank()
        print("World size:", size, ", rank:", rank)
        use_mpi = True
    except ImportError:
        rank = 0
        size = 1
        use_mpi = False

    input_dir = '/global/homes/x/xju/project/xju/gnn_examples/H4l_ggF_vs_VBF/input'

    if rank == 0:
        file_names = np.linspace(1, 8, 8)
        file_names = [x.tolist() for x in np.array_split(file_names, size)]
    else:
        file_names = None


    if use_mpi:
        comm.Barrier()
        file_names = comm.scatter(file_names, root=0)
    else:
        file_names = file_names[0]


    try:
        n_workers = int(os.getenv('SLURM_CPUS_PER_TASK'))
    except  (ValueError, TypeError):
        n_workers = 2

    print("rank({}), {} workers:".format(rank, n_workers))
    print("rank({}), {}".format(rank, file_names))

    from functools import partial
    import multiprocessing as mp
    import numpy as np

    for file_name in file_names:
        nentries = 3
        print("rank({}) reading {}".format(rank, file_name))
        evt_ids = np.linspace(0, nentries-1, nentries, dtype=np.long)

        ##### evt_ids = [x.tolist() for x in np.array_split(evt_ids, n_workers)]
        ijob = 0
        while ijob < nentries:
            if threading.active_count() < n_workers:
                #print(evt_ids[ijob])
                x = threading.Thread(target=process_event, args=(str(int(evt_ids[ijob])),))
                x.start()
                ijob += 1
            else:
                time.sleep(1)

        #pool = mp.Pool(n_workers)
        #pp_func=partial(process_event, file_name=file_name)
        #pool.map(pp_func, evt_ids)
