#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#define MAX 5000

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

int minmax_para(int **a) {
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
            if (j >= i) {
                int value = rand() % 100;
                our_matrix[i][j] = value;
            }
            else
                our_matrix[i][j] = 0;
            //std::cout << our_matrix[i][j] << " \n"[j == MAX - 1];
        }
    }

    return our_matrix;
}

int** create_band_matrix() {
    int** our_matrix = new int* [MAX];
    for (int a = 0; a < MAX; a++) {
        our_matrix[a] = new int[MAX];
    }

    srand(time(NULL));
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            if (i%2 == 0) {
                int value = rand() % 100;
                our_matrix[i][j] = value;
            }
            else
                our_matrix[i][j] = 0;
            //std::cout << our_matrix[i][j] << " \n"[j == MAX - 1];
        }
    }

    return our_matrix;
}

int main(int argc, char* argv[])
{
    
    double t1, t2;

    int** band_matrix = create_band_matrix();
    int** triangle_matrix = create_matrix();


    t1 = omp_get_wtime();

    int max_regular_1 = minmax_regular(band_matrix);

    t2 = omp_get_wtime();

    printf("Maximum value found: %d\n", max_regular_1);
    printf("Regular time for band matrix is %f\n", (t2 - t1));

    t1 = omp_get_wtime();

    int max_regular_2 = minmax_regular(triangle_matrix);

    t2 = omp_get_wtime();

    printf("Maximum value found: %d\n", max_regular_2);
    printf("Regular time for triangle matrix is %f\n", (t2 - t1));

    t1 = omp_get_wtime();

    int max_para_1 = minmax_para(band_matrix);

    t2 = omp_get_wtime();

    printf("Maximum value found: %d\n", max_para_1);
    printf("Parallel time for band matrix is %f\n", (t2 - t1));


    t1 = omp_get_wtime();
    int max_para_2 = minmax_para(triangle_matrix);
    t2 = omp_get_wtime();
    printf("Maximum value found: %d\n", max_para_2);
    printf("Parallel time for triangle matrix is %f\n", (t2 - t1));

    for (int i = 0; i < MAX; ++i) {
        delete[] band_matrix[i];
    }
    delete[] band_matrix;

    for (int i = 0; i < MAX; ++i) {
        delete[] triangle_matrix[i];
    }
    delete[] triangle_matrix;
}
