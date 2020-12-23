#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 300000000


int max_finder(int* a) {
	int max_value = 0;
#pragma omp parallel for 
	for (int i = 0; i < MAX; i++) {
		if (a[i] > max_value) {
#pragma omp critical
			{
				if (a[i] > max_value) max_value = a[i];
			}
		}
	}
	return max_value;
}



int main(int argc, char* argv[]) {

	int* our_vector;
	double t1, t2;

	int max_value = 0;

	our_vector = new int[MAX];

	srand(time(NULL));
	for (int i = 0; i < MAX; i++) {
		our_vector[i] = rand() % 100;
	}

	t1 = omp_get_wtime();

	for (int i = 0; i < MAX; i++) {
		int current_value = our_vector[i];
		if (current_value > max_value)
			max_value = current_value;
	}
	t2 = omp_get_wtime();

	printf("The largest number is: %d\n", max_value);
	printf("Regular time is %f\n", (t2 - t1));

	t1 = omp_get_wtime();
	int para_result = max_finder(our_vector);
	t2 = omp_get_wtime();

	printf("The largest number is: %d\n", para_result);
	printf("Parallel time is %f\n", (t2 - t1));

	delete[]our_vector;
}
