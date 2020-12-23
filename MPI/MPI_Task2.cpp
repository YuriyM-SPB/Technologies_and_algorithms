#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <iostream>
#define MAX 1000000

using namespace std;

int main(int* argc, char** argv)
{
	int ProcNum, ProcRank, product, result;
	int* vector1;
	int* vector2;
	double t1, t2;
	int max_array = MAX;

	vector1 = new int[max_array];
	vector2 = new int[max_array];

	MPI_Init(argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

	if (ProcRank == 0) {

		//Filling the vectors
		srand(time(NULL));
		for (int i = 0; i < max_array; i++) {
			vector1[i] = rand() % 100;
			vector2[i] = rand() % 50;
		}

		//Using regular sum
		int sum = 0;
		t1 = MPI_Wtime();
		for (int i = 0; i < max_array; i++) {
			sum += vector1[i] * vector2[i];
		}
		t2 = MPI_Wtime();
		printf("Scalar product is %d\n", sum);
		printf("Regular time is %f\n", (t2 - t1));
		t1 = MPI_Wtime();
	}

	//Broadcasting the vectors and starting the timer
	MPI_Bcast(vector1, max_array, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(vector2, max_array, MPI_INT, 0, MPI_COMM_WORLD);

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
	int proc_sum = 0;
	for (int i = start; i < finish; i++) {
		proc_sum += vector1[i] * vector2[i];
	}

	//getting the result and stopping the timer
	MPI_Reduce(&proc_sum, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (ProcRank == 0) {
		t2 = MPI_Wtime();
		printf("Scalar product is %d\n", result);
		printf("Parallel time is %f\n", (t2 - t1));
	}


	MPI_Finalize();
}
