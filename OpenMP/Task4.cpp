#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#define MAX 10000

int minmax_regular(int** a) {
    int i = 0;
    int j = 0;
    int max_value = 0;
    int min_element = a[0][0];
    for (i = 0; i < MAX; i++) {
        for (j = 0; j < MAX; j++) {
            int current_value = a[i][j];
            if (current_value < min_element) {
                min_element = current_value;
            }

        }
        if (min_element > max_value)
            max_value = min_element;
    }
    return max_value;
}

int minmax_para(int** a) {
    int i = 0;
    int j = 0;
    int max_value = 0;
    int threads_number = omp_get_max_threads();
    //int chunkSize = MAX / threads_number;

    int min_element = a[0][0];

#pragma omp parallel shared(a, max_value) private(i,j) 
    {
#pragma omp for schedule(dynamic)
        for (i = 0; i < MAX; i++) {
            for (j = 0; j < MAX; j++) {
                int current_value = a[i][j];
                if (current_value < min_element) {
                    min_element = current_value;
                }

            }
            if (min_element > max_value) {
#pragma omp critical
                if (min_element > max_value) {
                    max_value = min_element;
                }
            }

        }
    }
    return max_value;
}

int** create_matrix() {
    int** our_matrix = new int* [MAX];
    for (int a = 0; a < MAX; a++) {
        our_matrix[a] = new int[MAX];
    }

    srand(time(NULL));
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
                int value = rand() % 100;
                our_matrix[i][j] = value;
        }
    }

    return our_matrix;
}

int main(int argc, char* argv[])
{

    double t1, t2;
    int** our_matrix = create_matrix();

    t1 = omp_get_wtime();
    int max_regular = minmax_regular(our_matrix);
    t2 = omp_get_wtime();
    printf("Maximum value found: %d\n", max_regular);
    printf("Regular time is %f\n", (t2 - t1));

    t1 = omp_get_wtime();
    int max_para = minmax_para(our_matrix);
    t2 = omp_get_wtime();
    printf("Maximum value found: %d\n", max_para);
    printf("Parallel time is %f\n", (t2 - t1));

    for (int i = 0; i < MAX; ++i) {
        delete[] our_matrix[i];
    }

    delete[] our_matrix;
}


