#!/usr/bin/env python
"""
Use multi-threads to do the same job on different dataset
"""

def square(a):
    return a**2

if __name__ == "__main__":
    import multiprocessing as mp

    n_workers = 4
    aa = [0, 1, 2, 3, 4, 5, 6 ,7, 8, 9, 10, 11]
    with mp.Pool(processes=n_workers) as pool:
        bb = pool.map(square, aa)

        print(bb)
