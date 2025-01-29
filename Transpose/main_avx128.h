
#ifndef AVX_128
#define AVX_128

#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include <immintrin.h>
#include <sys/time.h>
#include <string.h>
#include "common.h"

void transpose_avx128(int n, int32_t *input, int32_t *output) {
    // First, handle all complete 4x4 blocks using SIMD
    int block_limit_i = (n / 4) * 4;  
    int block_limit_j = (n / 4) * 4;

    for (int i = 0; i < block_limit_i; i += 4) {
        for (int j = 0; j < block_limit_j; j += 4) {
            // Load four rows into SIMD registers
            __m128i row0 = _mm_loadu_si128((__m128i*)&input[i * n + j + 0]);
            __m128i row1 = _mm_loadu_si128((__m128i*)&input[(i + 1) * n + j]);
            __m128i row2 = _mm_loadu_si128((__m128i*)&input[(i + 2) * n + j]);
            __m128i row3 = _mm_loadu_si128((__m128i*)&input[(i + 3) * n + j]);

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
            _mm_storeu_si128((__m128i*)&output[(j + 0) * n + i], result0);
            _mm_storeu_si128((__m128i*)&output[(j + 1) * n + i], result1);
            _mm_storeu_si128((__m128i*)&output[(j + 2) * n + i], result2);
            _mm_storeu_si128((__m128i*)&output[(j + 3) * n + i], result3);
        }
    }

    // Handle remaining elements using scalar operations
    for (int i = 0; i < block_limit_i; i++) {
        for (int j = block_limit_j; j < n; j++) {
            output[j * n + i] = input[i * n + j];
        }
    }

    for (int i = block_limit_i; i < n; i++) {
        for (int j = 0; j < n; j++) {
            output[j * n + i] = input[i * n + j];
        }
    }
}
#endif