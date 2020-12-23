#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <iostream>

int main()
{
    int counter, number_of_iterations, chunkSize;

    double t1, t2;
    number_of_iterations = 10;
    counter = 0;
    chunkSize = 10000;

    t1 = omp_get_wtime();
#pragma omp parallel for shared(counter) schedule(dynamic, chunkSize)
    for (int i = 0; i < number_of_iterations; i++)
    {
#pragma omp atomic
        counter += 1;
    }
    t2 = omp_get_wtime();

    printf("Atomic time for %d iterations is %f\n", number_of_iterations, (t2 - t1));
    printf("Total sum is %d\n", counter);
    counter = 0;

    t1 = omp_get_wtime();
#pragma omp parallel for shared(counter) schedule(dynamic, chunkSize)
    for (int i = 0; i < number_of_iterations; i++)
    {
#pragma omp critical
        counter += 1;
    }
    t2 = omp_get_wtime();

    printf("Critical time for %d iterations is %f\n", number_of_iterations, (t2 - t1));
    printf("Total sum is %d\n", counter);
    counter = 0;

    omp_lock_t my_lock;
    omp_init_lock(&my_lock);
    t1 = omp_get_wtime();
#pragma omp parallel for shared(counter) schedule(dynamic, chunkSize)
    for (int i = 0; i < number_of_iterations; i++)
    {
        omp_set_lock(&my_lock);
        counter += 1;
        omp_unset_lock(&my_lock);
    }
    t2 = omp_get_wtime();

    printf("Lock time for %d iterations is %f\n", number_of_iterations, (t2 - t1));
    printf("Total sum is %d\n", counter);
    counter = 0;

    t1 = omp_get_wtime();

#pragma omp parallel for schedule(dynamic, chunkSize) reduction(+ \
                                                                : counter)
    for (int i = 0; i < number_of_iterations; i++)
    {
        counter += 1;
    }
    t2 = omp_get_wtime();

    printf("Reduction time for %d iterations is %f\n", number_of_iterations, (t2 - t1));
    printf("Total sum is %d\n", counter);
    counter = 0;
}
