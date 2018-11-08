#include <stdio.h>
#include "omp.h"

/**
 * Reduction 
 * */

int main() {
	static long num_steps = 100000;
	double step;
	double pi, sum = 0.0;

	const int NUM_THRREADS = 8;

	step = 1./(double)num_steps;

int i;
double start_time = omp_get_wtime();
omp_set_num_threads(NUM_THRREADS);
#pragma omp parallel for reduction (+:sum)
{
	for (i=0; i < num_steps; i++) {
		double x = (i+0.5)*step;
		sum += 4./(1 + x*x);
	}
}
	pi = step * sum;

	printf("PI: %.10f finished in %fs\n", 
			pi, omp_get_wtime()-start_time);
	return 0;
}
