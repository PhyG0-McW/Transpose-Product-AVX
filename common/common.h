#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <immintrin.h>

#define MAX_TEST_CASES 10
#define MAX_NAME_LENGTH 50

typedef struct {
    double time_taken;
    int32_t* output;
} TestResult;

typedef struct {
    int size;
    int32_t *input;
    char description[100];
} TestCase;

typedef struct {
    char func_name[MAX_NAME_LENGTH];
    double times[MAX_TEST_CASES];
    double avg_time;
    double min_time;
    double max_time;
} FunctionStats;

typedef void (*TestFunction)(int n, int32_t* input, int32_t* output);


void printMatrix(int n, int32_t* matrix);

void fillMatrix(int n, int32_t* matrix);

void transpose_original(int n, int32_t* input, int32_t* output);

int validate_func(int n, int32_t* output1, int32_t* output2);

void compare_performance(
    const char* func1_name,
    const char* func2_name,
    TestFunction func1,
    TestFunction func2,
    TestCase test_cases[],
    int num_test_cases,
    int iterations,
    int (*validate_func)(int n, int32_t* output1, int32_t* output2)
);

#endif