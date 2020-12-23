
#include <iostream>
#include <omp.h>
#include <stdio.h>
#define MAX 10
int main()
{
    int maxInitValue = 30;

    // Fill matrix
    int matrix[MAX][MAX];
    for (int i = 0; i < MAX; ++i)
    {
        for (int j = 0; j < MAX; ++j)
        {
            // By this way the minimum elements will be on the diagonal
            if (i == j)
            {
                matrix[i][j] = i;
                continue;
            }

            matrix[i][j] = MAX + ((i + j) % maxInitValue);
        }
    }

    // Get chunk size
    int threadCount = omp_get_max_threads();
    int chunkSize = MAX / threadCount;
    printf("Chunk size = %d\n", chunkSize);

    int maxValue = 0;

#pragma omp parallel shared(matrix, maxValue)
    {
#pragma omp for schedule(dynamic, chunkSize)
        // For each row
        for (int i = 0; i < MAX; ++i)
        {
            int minRowValue = MAX + maxInitValue;

            // For each column in current row
            for (int j = 0; j < MAX; ++j)
            {
                if (matrix[i][j] < minRowValue)
                {
                    minRowValue = matrix[i][j];
                }
            }
            printf("Min value for %d row = %d\n", i, minRowValue);

            // Check max value
            if (minRowValue > maxValue)
            {
#pragma omp critical
                if (minRowValue > maxValue)
                {
                    maxValue = minRowValue;
                }
            }
        }
    }

    printf("Maxmin value = %d\n", maxValue);
}
