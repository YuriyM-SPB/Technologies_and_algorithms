#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <iostream>
#define MAX 100000000

using namespace std;

int max_finder(int *array, int array_size) {

	int max_value = 0;

	for (int i = 0; i < array_size; i++) {
		if (array[i] > max_value)
			max_value = array[i];
	}

	return max_value;
}


int main(int* argc, char** argv)
{
    int ProcNum, ProcRank, max_value, result;
	int max_array = MAX;
	int* our_vector;
	double t1, t2;

	//int max_value = 0;

	our_vector = new int[max_array];

    MPI_Init(argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

	if (ProcRank == 0) {

		//Filling the array
		srand(time(NULL));
		for (int i = 0; i < max_array; i++) {
			our_vector[i] = rand() % 100;
		}

		//Using regular sorting
		t1 = MPI_Wtime();
		int max_value = max_finder(our_vector, max_array);
		t2 = MPI_Wtime();
		printf("The maximum value of the array is %d\n", max_value);
		printf("Regular time is %f\n", (t2 - t1));
		t1 = MPI_Wtime();
	}

	//Broadcasting the array and starting the timer
	MPI_Bcast(our_vector, max_array, MPI_INT, 0, MPI_COMM_WORLD);

	//if (ProcRank == 0)
	//	t1 = MPI_Wtime();

	//dividing into chunks
	int chunk_size = max_array / ProcNum;
	int start = chunk_size * ProcRank;
	int finish = chunk_size * (ProcRank + 1);
	if (ProcRank == ProcNum - 1) {
		finish = max_array;
	}

	//finding max value for each chunk
	int proc_max_value = 0;
	for (int i = start; i < finish; i++) {
		if (our_vector[i] > proc_max_value)
			proc_max_value = our_vector[i];
	}

	//getting the result and stopping the timer
	MPI_Reduce(&proc_max_value, &result, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

	if (ProcRank == 0) {
		t2 = MPI_Wtime();
		printf("The maximum value of the array is %d\n", result);
		printf("Parallel time is %f\n", (t2 - t1));
	}
		

    MPI_Finalize();
}


