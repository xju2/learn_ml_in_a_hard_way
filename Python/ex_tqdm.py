#!/usr/bin/env python
"""Example of using tqdm with concurrent.futures.ProcessPoolExecutor."""

import concurrent.futures
import math
import os
import time
from tqdm import tqdm

def dummy_function(seconds):
    """Sleep for a specified number of seconds."""
    time.sleep(seconds)
    return seconds

def main():
    """Create a dummy function to simulate a long running process."""


    # Create a list of 30 items to process
    items = [1] * 30

    # Create a ProcessPoolExecutor with 4 workers
    with concurrent.futures.ProcessPoolExecutor(max_workers=4) as executor:
        # Create a list to store the futures
        futures = []

        # Loop through each item in the list
        for item in items:
            # Submit the job to the executor
            future = executor.submit(dummy_function, item)
            # Append the future to the list
            futures.append(future)

        # Create a list to store the results
        results = []

        # Loop through each future in the list
        for future in tqdm(concurrent.futures.as_completed(futures), total=len(futures)):
            # Get the result from the future
            result = future.result()
            # Append the result to the list
            results.append(result)

    # Print the results
    print(results)

if __name__ == "__main__":
    main()
