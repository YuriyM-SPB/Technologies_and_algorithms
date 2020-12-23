#include <iostream>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

double function(double x) {
	return sqrt(x);
}

double Riemann(double a, double b, int interval_number) {
	double width = (b - a) / interval_number;
	double result = 0.0;

	for (int i = 0; i < interval_number; i++) {
		double x = a + width * i;
		result += function(x) * width;
	}

	return result;
}


double Riemann_parallel(double a, double b, int interval_number) {
	double width = (b - a) / interval_number;
	double result = 0.0;

#pragma omp parallel shared(width) reduction (+: result) num_threads(16)
	{
# pragma omp for
		for (int i = 0; i < interval_number; i++) {
			double x = a + width * i;
			result += function(x) * width;
		}
	}
	return result;
}


int main() {
	double rman, rman_para;
	double a = 5.0;
	double b = 100.0;
	int interval = 100000000.0;
	double t1, t2;

	t1 = omp_get_wtime();
	rman = Riemann(a, b, interval);
	t2 = omp_get_wtime();
	printf("Regular time is %f\n", (t2 - t1));
	printf("%f\n", rman);

	t1 = omp_get_wtime();
	rman_para = Riemann_parallel(a, b, interval);
	t2 = omp_get_wtime();
	printf("Parallel time is %f\n", (t2 - t1));
	printf("%f\n", rman_para);

}
