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
    file_name = '/Volumes/Lion/Documents/Data/ForTest/ggf.root'
    nentries = 10
    print("Total Entries: {}".format(nentries))

    try:
        n_workers = int(os.getenv('SLURM_CPUS_PER_TASK'))
    except  (ValueError, TypeError):
        n_workers = 2

    print("# workers:", n_workers)

    from functools import partial
    import multiprocessing as mp
    import numpy as np

    evt_ids = np.linspace(0, nentries-1, nentries, dtype=long)

    evt_ids = [x.tolist() for x in np.array_split(evt_ids, n_workers)]
    print(len(evt_ids))
    pool = mp.Pool(n_workers)
    pp_func=partial(process_event, file_name=file_name)
    pool.map(pp_func, evt_ids)
