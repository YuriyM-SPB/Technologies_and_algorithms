#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define MAX 150000000

int vector_result(int* a, int* b) {
	int sum = 0, i;
#pragma omp parallel shared(a, i) reduction (+: sum) 
	{
# pragma omp for
		for (int i = 0; i < MAX; i++)
			sum += a[i]*b[i];
	} 
	return sum;
}

int main(int argc, char* argv[]) {

	int* first_vector;
	int* second_vector;
	double t1, t2;

	int result = 0;

	first_vector = new int[MAX];
	second_vector = new int[MAX];

	srand(time(NULL));
	for (int i = 0; i < MAX; i++) {
		first_vector[i] = rand() % 10;
		second_vector[i] = rand() % 10;
	}
		
	t1 = omp_get_wtime();
	for (int i = 0; i < MAX; i++) {
		result += first_vector[i] * second_vector[i];
	}
	t2 = omp_get_wtime();

	printf("Regular time is %f\n", (t2 - t1));
	printf("The final product is: %d\n", result);

	t1 = omp_get_wtime();
	int para_result = vector_result(first_vector, second_vector);
	t2 = omp_get_wtime();

	printf("Parallel time is %f\n", (t2 - t1));
	printf("The final product is: %d\n", para_result);

	delete[]first_vector;
	delete[]second_vector;
}
