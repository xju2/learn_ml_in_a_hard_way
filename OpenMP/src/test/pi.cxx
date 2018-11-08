#include <stdio.h>
#include "omp.h"


int main() {
	static long num_steps = 100000;
	double step;
	double pi, sum = 0.0;

	const int NUM_THRREADS = 8;

	step = 1./(double)num_steps;
	double sum_partial[NUM_THRREADS];

	int njobs = num_steps/NUM_THRREADS;


double start_time = omp_get_wtime();
omp_set_num_threads(NUM_THRREADS);
#pragma omp parallel
{
	int ID = omp_get_thread_num();
	int start = ID*njobs;
	int end = start + njobs;
	sum_partial[ID] = 0;
	for (int i=start; i < end; i++) {
		double x = (i+0.5)*step;
		sum_partial[ID] += 4./(1 + x*x);
	}
}
	for(int i = 0; i < NUM_THRREADS; i++){
		sum += sum_partial[i];
	}
	pi = step * sum;

	printf("PI: %.10f finished in %fs\n", 
			pi, omp_get_wtime()-start_time);
	return 0;
}
