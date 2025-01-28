#include <stdio.h>
#include <stdint.h>
#include <immintrin.h>
#include <sys/time.h>
#include <string.h>
#include "common.h"

void transpose_avx128(int n, int32_t input[n][n], int32_t output[n][n]) {
    // First, handle all complete 4x4 blocks using SIMD
    int block_limit_i = (n / 4) * 4;  
    int block_limit_j = (n / 4) * 4;

    for (int i = 0; i < block_limit_i; i += 4) {
        for (int j = 0; j < block_limit_j; j += 4) {
            // Load four rows into SIMD registers
            __m128i row0 = _mm_loadu_si128((__m128i*)&input[i + 0][j]);
            __m128i row1 = _mm_loadu_si128((__m128i*)&input[i + 1][j]);
            __m128i row2 = _mm_loadu_si128((__m128i*)&input[i + 2][j]);
            __m128i row3 = _mm_loadu_si128((__m128i*)&input[i + 3][j]);

            // Transpose 4x4 block using SIMD operations
            __m128i temp0 = _mm_unpacklo_epi32(row0, row1);
            __m128i temp1 = _mm_unpacklo_epi32(row2, row3);
            __m128i temp2 = _mm_unpackhi_epi32(row0, row1);
            __m128i temp3 = _mm_unpackhi_epi32(row2, row3);

            __m128i result0 = _mm_unpacklo_epi64(temp0, temp1);
            __m128i result1 = _mm_unpackhi_epi64(temp0, temp1);
            __m128i result2 = _mm_unpacklo_epi64(temp2, temp3);
            __m128i result3 = _mm_unpackhi_epi64(temp2, temp3);

            // Store transposed results
            _mm_storeu_si128((__m128i*)&output[j + 0][i], result0);
            _mm_storeu_si128((__m128i*)&output[j + 1][i], result1);
            _mm_storeu_si128((__m128i*)&output[j + 2][i], result2);
            _mm_storeu_si128((__m128i*)&output[j + 3][i], result3);
        }
    }

    // Handle remaining elements using scalar operations
    for (int i = 0; i < block_limit_i; i++) {
        for (int j = block_limit_j; j < n; j++) {
            output[j][i] = input[i][j];
        }
    }

    for (int i = block_limit_i; i < n; i++) {
        for (int j = 0; j < n; j++) {
            output[j][i] = input[i][j];
        }
    }
}



int main() {

    const int smallSize = 100;
    const int mediumSize = 500;
    const int largeSize = 1000;

    const int iterations = 50;

     int32_t (*input)[smallSize] = malloc(smallSize * smallSize * sizeof(int32_t));
     int32_t (*medium)[mediumSize] = malloc(mediumSize * mediumSize * sizeof(int32_t));
     int32_t (*large)[largeSize] = malloc(largeSize * largeSize * sizeof(int32_t));

     TestCase test_cases[3] = {
        {
            .size = smallSize,
            .input = (int32_t (*)[])input,
            .description = "Small Matrix"
        },
        {
            .size = mediumSize,
            .input = (int32_t (*)[])medium,
            .description = "Medium Matrix"
        },
        {
            .size = largeSize,
            .input = (int32_t (*)[])large,
            .description = "Large Matrix"
        },
    };

    fillMatrix(smallSize, input);
    fillMatrix(mediumSize, medium);
    fillMatrix(largeSize, large);

    compare_performance(
            "AVX-128 Transpose", "Scalar Transpose",
            transpose_avx128, transpose_original,
            test_cases, 3, iterations,
            validate_func
    );


    return 0;
}