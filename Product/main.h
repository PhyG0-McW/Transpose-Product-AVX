#ifndef MAIN_H
#define MAIN_H
#include "common.h"


void transpose_avx256(int n, int32_t *input, int32_t *output);

void multiplyOriginal(int n, int32_t* input, int32_t* output);


void multiplyOriginal(int n, int32_t* input, int32_t* output) {
    for(int i = 0; i < n; i++) {
        for(int k = 0; k < n; k++) {
            for(int j = 0; j < n; j++) {
                output[i * n + j] += input[i * n + k] * input[k * n + j];
            }
        }
    }
}

void multiply(int n, int32_t *input, int32_t *output) {
    int32_t *transpose = (int32_t *)malloc(n * n * sizeof(int32_t));
    
    if (!transpose) {
        return;  // Handle memory allocation failure
    }

    // First transpose the matrix for efficient memory access
    transpose_avx256(n, input, transpose);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int tempSum = 0;
            int k = 0;

            // Use AVX to process 8 elements at a time
            for (; k + 8 <= n; k += 8) {
                __m256i row = _mm256_loadu_si256((__m256i*)&input[i * n + k]);
                __m256i col = _mm256_loadu_si256((__m256i*)&transpose[j * n + k]);

                // Multiply the vectors
                __m256i product = _mm256_mullo_epi32(row, col);

                // Horizontal add for partial sums
                __m256i temp = _mm256_hadd_epi32(product, product);
                __m256i hadd = _mm256_hadd_epi32(temp, temp);

                tempSum += _mm256_extract_epi32(hadd, 0);
                tempSum += _mm256_extract_epi32(hadd, 4);
            }

            // Handle the remaining elements using scalar multiplication
            for (; k < n; k++) {
                tempSum += input[i * n + k] * transpose[j * n + k];
            }

            output[i * n + j] = tempSum;
        }
    }

    free(transpose);  // Free dynamically allocated memory
}

#endif