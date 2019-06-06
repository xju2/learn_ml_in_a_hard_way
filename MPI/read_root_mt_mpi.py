#!/usr/bin/env python
import os
import ROOT
tree_name = 'tree_incl_all'

def process_event(evt_id_list, file_name):
    file_ = ROOT.TFile.Open(file_name)
    tree = file_.Get(tree_name)

    print("thread({}) I am processing {} events out of {} from {}".format(
        os.getpid(), len(evt_id_list), tree.GetEntries(), file_name))

    for evt_id in evt_id_list:
        tree.GetEntry(evt_id)
        print("thread({}) --> {}".format(os.getpid(), tree.event))

    file_.Close()



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
        file_names = [os.path.join(input_dir, 'ggf.root'),
                      os.path.join(input_dir, 'vbf.root')]
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
        nentries = 8
        print("rank({}) reading {}".format(rank, file_name))
        evt_ids = np.linspace(0, nentries-1, nentries, dtype=np.long)

        evt_ids = [x.tolist() for x in np.array_split(evt_ids, n_workers)]
        pool = mp.Pool(n_workers)
        pp_func=partial(process_event, file_name=file_name)
        pool.map(pp_func, evt_ids)
