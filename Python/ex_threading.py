#!/usr/bin/env python
"""
Use multi-threads to do the same job on different dataset
"""
import os
import multiprocessing as mp
import threading
import numpy as np
import time

def do_something(array, opt):
    res = None
    if opt == 1: ## add everything
       res = sum([x+y for x,y in array])
    elif opt == 2:
       res = sum([x*y for x,y in array])
    elif opt == 3:
        res = sum([x**2 + y**2 for x,y in array])
    else:
        res = 0
    return res

import numpy as np

def increase_by_one(array):
    array += 1


if __name__ == "__main__":

    np.random.seed(23)
    n_evts = 10
    n_vars = 2
    min_val = 1
    max_val = 100
    A = np.random.random_integers(min_val, max_val, (n_evts, n_vars))
    print(A.shape)

    data = np.ones((4,1))
    #increase_by_one(data)
    t = threading.Thread(target=increase_by_one, args=(data,))
    t.start()

    t2 = threading.Thread(target=increase_by_one, args=(data,))
    t2.start()

    t.join()
    t2.join()
    print(data)

    exit(0)

    options = [1, 2, 3, 4]
    t_start = time.time()
    ## for it sequentially
    results = [do_something(A, opt) for opt in options]
    t_seq = time.time()
    print("{:.0f} seconds for sequential")

    time.sleep(1)

    max_threads = 4
    n_threads = 0
    threads = []
