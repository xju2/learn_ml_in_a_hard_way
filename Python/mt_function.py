#!/usr/bin/env python
"""
Use multi-threads to do the same job on different dataset
"""

import os
import threading
import time
def add_abc(a, b, c):
    res = a+b+c
    print("PID {}, thread {} get {}".format(os.getpid(), threading.current_thread(), res))
    time.sleep(3)
    print("{} done".format(threading.current_thread()))
    #return a+b+c

if __name__ == "__main__":

    A = [1, 2, 3, 4]
    B = [2, 2, 2, 2]
    C = [1, 1, 1, 1]
    max_threads = 3
    n_threads = 0
    threads = []
    while n_threads < len(A):
        if threading.active_count() < max_threads:
            x = threading.Thread(target=add_abc, args=(A[n_threads], B[n_threads], C[n_threads]))
            x.start()
            n_threads += 1
        else:
            time.sleep(3)

    print(threading.active_count())
    #for thread in threads:
    #    thread.start()

    #for thread in threads:
    #   thread.join()
