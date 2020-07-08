#!/usr/bin/env python
import os
def process_event(evt_id_list, tree):
    print("thread({}) I am processing {} events out of {}".format(
        os.getpid(), len(evt_id_list), tree.GetEntries()))

    for evt_id in evt_id_list:
        if tree.GetEntry(evt_id) <= 0:
            print("failed")
        print(tree.event)

if __name__ == "__main__":
    file_name = '/Volumes/Lion/Documents/Data/ForTest/ggf.root'
    import ROOT
    file_ = ROOT.TFile.Open(file_name)
    tree_name = 'tree_incl_all'
    tree = file_.Get(tree_name)
    nentries = 10
    print("Total Entries: {}".format(nentries))
    tree.GetEntry(1)
    print(tree.event)

    import os
    try:
        n_workers = int(os.getenv('SLURM_CPUS_PER_TASK'))
    except  (ValueError, TypeError):
        n_workers = 1

    print("# workers:", n_workers)

    from functools import partial
    import multiprocessing as mp
    import numpy as np

    evt_ids = np.linspace(0, nentries-1, nentries, dtype=long)

    evt_ids = [x.tolist() for x in np.array_split(evt_ids, n_workers)]
    print(len(evt_ids))
    pool = mp.Pool(n_workers)
    pp_func=partial(process_event, tree=tree)
    pool.map(pp_func, evt_ids)
