#!/usr/bin/env python
"""
Use multi-threads to do the same job on different dataset
"""
import os
import numpy as np

def add_abc(a, b, c):
    return a+b+c

if __name__ == "__main__":

    #A = [1, 2, 3, 4]  # 10
    #B = [2, 2, 2, 2]  # 8
    #C = [1, 1, 1, 1]  # 4
    np.random.seed(23)
    A = np.random.random_integers(1, 100, 1000000)
    B = np.random.random_integers(1, 100, 1000000)
    C = np.random.random_integers(1, 100, 1000000)
    total_sum = 0  # 10+8+4=22

    for n_threads in range(len(A)):
        res = A[n_threads] + B[n_threads] + C[n_threads]
        total_sum += res

    print("total sum is: {}".format(total_sum))
