#ifndef AVX_256
#define AVX_256 

#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include <immintrin.h>
#include <string.h>
#include <sys/time.h>
#include "common.h"

void transpose_avx256(int n, int32_t *input, int32_t *output) {
    int block_limit_i = (n / 8) * 8;
    int block_limit_j = (n / 8) * 8;

    // Transpose 8x8 matrices using AVX instructions
    for (int i = 0; i < block_limit_i; i += 8) {
        for (int j = 0; j < block_limit_j; j += 8) {
            // Load 8 rows of 8 elements each
            __m256i row0 = _mm256_loadu_si256((__m256i*)&input[(i + 0) * n + j]);
            __m256i row1 = _mm256_loadu_si256((__m256i*)&input[(i + 1) * n + j]);
            __m256i row2 = _mm256_loadu_si256((__m256i*)&input[(i + 2) * n + j]);
            __m256i row3 = _mm256_loadu_si256((__m256i*)&input[(i + 3) * n + j]);
            __m256i row4 = _mm256_loadu_si256((__m256i*)&input[(i + 4) * n + j]);
            __m256i row5 = _mm256_loadu_si256((__m256i*)&input[(i + 5) * n + j]);
            __m256i row6 = _mm256_loadu_si256((__m256i*)&input[(i + 6) * n + j]);
            __m256i row7 = _mm256_loadu_si256((__m256i*)&input[(i + 7) * n + j]);

            // Interleave two rows (1-1) elements
            __m256i temp0 = _mm256_unpacklo_epi32(row0, row1);
            __m256i temp1 = _mm256_unpackhi_epi32(row0, row1);
            __m256i temp2 = _mm256_unpacklo_epi32(row2, row3);
            __m256i temp3 = _mm256_unpackhi_epi32(row2, row3);
            __m256i temp4 = _mm256_unpacklo_epi32(row4, row5);
            __m256i temp5 = _mm256_unpackhi_epi32(row4, row5);
            __m256i temp6 = _mm256_unpacklo_epi32(row6, row7);
            __m256i temp7 = _mm256_unpackhi_epi32(row6, row7);

            // Interleave two rows (2-2) elements
            __m256i q0 = _mm256_unpacklo_epi64(temp0, temp2);
            __m256i q1 = _mm256_unpackhi_epi64(temp0, temp2);
            __m256i q2 = _mm256_unpacklo_epi64(temp1, temp3);
            __m256i q3 = _mm256_unpackhi_epi64(temp1, temp3);
            __m256i q4 = _mm256_unpacklo_epi64(temp4, temp6);
            __m256i q5 = _mm256_unpackhi_epi64(temp4, temp6);
            __m256i q6 = _mm256_unpacklo_epi64(temp5, temp7);
            __m256i q7 = _mm256_unpackhi_epi64(temp5, temp7);

            // Permutate two 256-bit lanes with controller
            __m256i result0 = _mm256_permute2x128_si256(q0, q4, 0x20);
            __m256i result1 = _mm256_permute2x128_si256(q1, q5, 0x20);
            __m256i result2 = _mm256_permute2x128_si256(q2, q6, 0x20);
            __m256i result3 = _mm256_permute2x128_si256(q3, q7, 0x20);
            __m256i result4 = _mm256_permute2x128_si256(q0, q4, 0x31);
            __m256i result5 = _mm256_permute2x128_si256(q1, q5, 0x31);
            __m256i result6 = _mm256_permute2x128_si256(q2, q6, 0x31);
            __m256i result7 = _mm256_permute2x128_si256(q3, q7, 0x31);

            // Store the final result in the transposed position
            _mm256_storeu_si256((__m256i*)&output[(j + 0) * n + i], result0);
            _mm256_storeu_si256((__m256i*)&output[(j + 1) * n + i], result1);
            _mm256_storeu_si256((__m256i*)&output[(j + 2) * n + i], result2);
            _mm256_storeu_si256((__m256i*)&output[(j + 3) * n + i], result3);
            _mm256_storeu_si256((__m256i*)&output[(j + 4) * n + i], result4);
            _mm256_storeu_si256((__m256i*)&output[(j + 5) * n + i], result5);
            _mm256_storeu_si256((__m256i*)&output[(j + 6) * n + i], result6);
            _mm256_storeu_si256((__m256i*)&output[(j + 7) * n + i], result7);
        }
    }

    // Handle the remaining elements
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