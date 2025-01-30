#include <gtest/gtest.h>
#include <immintrin.h>
#define N 10

extern "C" {
    #include "../Transpose/main_avx256.h"
    #include "../Transpose/main_avx128.h"
    #include "../Product/main.h"
    #include "../common/common.c"
}

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
    memset(output, 0, N * N * sizeof(int32_t));
    memset(expected, 0, N * N * sizeof(int32_t));

    multiplyOriginal(N, input, expected);

    multiplyOriginal(N, input, output);

    ASSERT_EQ(validate_func(N, output, expected), 1);

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
