
#include "main_avx256.h"

int main() {
    const int smallSize = 100;
    const int mediumSize = 300;
    const int largeSize = 500;
    const int iterations = 1000;

     int32_t *input = (int32_t*)malloc(smallSize * smallSize * sizeof(int32_t));
     int32_t *medium = (int32_t*)malloc(mediumSize * mediumSize * sizeof(int32_t));
     int32_t *large = (int32_t*)malloc(largeSize * largeSize * sizeof(int32_t));

     TestCase test_cases[3] = {
        {
            .size = smallSize,
            .input = input,
            .description = "Small Matrix"
        },
        {
            .size = mediumSize,
            .input = medium,
            .description = "Medium Matrix"
        },
        {
            .size = largeSize,
            .input = large,
            .description = "Large Matrix"
        },
    };

    fillMatrix(smallSize, input);
    fillMatrix(mediumSize, medium);
    fillMatrix(largeSize, large);

    compare_performance(
            "AVX-256 Transpose", "Scalar Transpose",
            transpose_avx256, transpose_original,
            test_cases, 3, iterations,
            validate_func
    );

    return 0;
}
