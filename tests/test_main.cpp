#include <gtest/gtest.h>
#include <immintrin.h>
#define N 100

extern "C" {
    #include "../Transpose/main_avx256.h"
    #include "../Transpose/main_avx128.h"
    #include "../Product/main.h"
    #include "../common/common.c"
}


/*************  ✨ Codeium Command ⭐  *************/
/// \brief Tests the AVX128 implementation of matrix transpose.
///
/// This test initializes an input matrix with values, transposes it using both
/// the original scalar implementation and the AVX128 SIMD implementation, and
/// verifies that the output from the AVX128 version matches the expected results
/// produced by the original method. It asserts that the validation function returns 1,
/// indicating that the output matrix is correctly transposed.

/******  2600c61e-0c0a-4ed4-82fb-aa54bcaabb0a  *******/
TEST(Transpose, AVX128) {

    int32_t* input = (int32_t*)malloc(N * N * sizeof(int32_t));
    int32_t* output = (int32_t*)malloc(N * N * sizeof(int32_t));
    int32_t* expected = (int32_t*)malloc(N * N * sizeof(int32_t));

    fillMatrix(N, input);

    transpose_original(N, input, expected);

    transpose_avx128(N, input, output);

    ASSERT_EQ(validate_func(N, output, expected), 1);

}


TEST(Transpose, AVX256) {

    int32_t* input = (int32_t*)malloc(N * N * sizeof(int32_t));
    int32_t* output = (int32_t*)malloc(N * N * sizeof(int32_t));
    int32_t* expected = (int32_t*)malloc(N * N * sizeof(int32_t));

    fillMatrix(N, input);

    transpose_original(N, input, expected);

    transpose_avx256(N, input, output);

    ASSERT_EQ(validate_func(N, output, expected), 1);

}


TEST(Transpose, MATRIX_AVX256) {

    int32_t* input = (int32_t*)malloc(N * N * sizeof(int32_t));
    int32_t* output = (int32_t*)malloc(N * N * sizeof(int32_t));
    int32_t* expected = (int32_t*)malloc(N * N * sizeof(int32_t));

    fillMatrix(N, input);

    multiplyOriginal(N, input, expected);

    multiply(N, input, output);

    ASSERT_EQ(validate_func(N, output, expected), 1);

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
