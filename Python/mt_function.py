#!/usr/bin/env python
"""
Use multi-threads to do the same job on different dataset
"""
import os
import multiprocessing as mp
import threading
import numpy as np

def add_abc(a, b, c):
    return a+b+c

if __name__ == "__main__":

    A = [1, 2, 3, 4]  # 10
    B = [2, 2, 2, 2]  # 8
    C = [1, 1, 1, 1]  # 4

    ### it does not work..
    # np.random.seed(23)
    # A = np.random.random_integers(1, 100, 1000000)
    # B = np.random.random_integers(1, 100, 1000000)
    # C = np.random.random_integers(1, 100, 1000000)

    max_threads = 4
    n_threads = 0
    threads = []
    total_sum = 0  # 10+8+4=22

    mp.set_start_method('spawn')
    que = mp.Queue()

    pool = mp.Pool(max_threads)
    total_sum = 0
    for n_threads in range(len(A)):
        res = pool.apply_async(add_abc, args=(A[n_threads], B[n_threads], C[n_threads]) )
        total_sum += res.get()

    print("total sum is: {}".format(total_sum))
