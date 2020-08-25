/*
 * OpenMP implementation of Monte Carlo pi-finding algorithm
 * (based on an example in Chap. 17 of Quinn (2004))
 *
 * usage: pi <samples> <threads>
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#include <math.h>

#define M_PI 3.14159265358979323846

double percentError(double experimental, double theoretical);

int main (int argc, char *argv[])
{
	int i, count, samples, nthreads, seed;
	struct drand48_data drand_buf;
	double x, y;
	double t0, t1;
	double     elapsedTime;
	double pi;
	struct timeval     start, end;

	samples  = atoi(argv[1]);
	nthreads = atoi(argv[2]);
	omp_set_num_threads (nthreads);

	t0 = omp_get_wtime();
	count = 0;
	// start timer
	//
	gettimeofday(&start, NULL);
	#pragma omp parallel private(i, x, y, seed, drand_buf) shared(samples)
	{
	seed = 1202107158 + omp_get_thread_num() * 1999;
	srand48_r (seed, &drand_buf);
	//srand48((int)time(NULL) ^ omp_get_thread_num());

	#pragma omp for reduction(+:count)
	for (i=0; i<samples; i++) {
		drand48_r (&drand_buf, &x);
		drand48_r (&drand_buf, &y);
		//x = (double)drand48();
		//y = (double)drand48();

		if (x*x + y*y <= 1.0) count++;
	}
	}

	t1 = omp_get_wtime();
	pi = 4.0*count/samples;
	printf("Estimate of pi: %7.20f\n", pi);
	// get time
   	//
  	gettimeofday(&end, NULL);
   	printf("Elapsed Time = %f seconds\n", (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec) / 1000000);
	printf("Percent Error = %f %%\n", percentError(pi, M_PI));
}

double percentError(double experimental, double theoretical)
{
	return (double)(fabs(experimental - theoretical) / fabs(theoretical) ) * 100;
}
