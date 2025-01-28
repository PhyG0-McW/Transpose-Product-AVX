#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdint.h>

#define MAX_TEST_CASES 10
#define MAX_NAME_LENGTH 50

// Helper functions
void printMatrix(int n, int32_t matrix[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%4d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void fillMatrix(int n, int32_t matrix[n][n]) {
    int value = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = value++;
        }
    }
}

double get_time_diff(struct timeval start, struct timeval end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
}

void transpose_original(int n, int32_t input[n][n], int32_t output[n][n]) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            output[i][j] = input[j][i];
        }
    }
}


typedef struct {
    double time_taken;
    void* output;
} TestResult;

typedef void (*TestFunction)(int n, int32_t input[][n], int32_t output[][n]);

TestResult run_single_test(TestFunction func, int n, int32_t input[][n], int iterations) {
    struct timeval start, end;
    int32_t (*output)[n] = malloc(n * n * sizeof(int32_t));
    
    gettimeofday(&start, NULL);
    for (int i = 0; i < iterations; i++) {
        memset(output, 0, n * n * sizeof(int32_t));
        func(n, input, output);
    }
    gettimeofday(&end, NULL);
    
    TestResult result = {
        .time_taken = get_time_diff(start, end),
        .output = output
    };
    return result;
}

int validate_func(int n, int32_t output1[][n], int32_t output2[][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (output1[i][j] != output2[i][j]) {
                return 0;
            }
        }
    }
    return 1;
}

typedef struct {
    int size;
    int32_t (*input)[];
    char description[100];
} TestCase;

typedef struct {
    char func_name[MAX_NAME_LENGTH];
    double times[MAX_TEST_CASES];
    double avg_time;
    double min_time;
    double max_time;
} FunctionStats;

void print_separator(int width) {
    for (int i = 0; i < width; i++) printf("-");
    printf("\n" );
}

void print_centered(const char* text, int width) {
    int padding = (width - strlen(text)) / 2;
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s\n", text);
}

void compare_performance(
    const char* func1_name,
    const char* func2_name,
    TestFunction func1,
    TestFunction func2,
    TestCase test_cases[],
    int num_test_cases,
    int iterations,
    int (*validate_func)(int n, int32_t output1[][n], int32_t output2[][n])
) {
    const int TABLE_WIDTH = 100;
    FunctionStats stats1 = {0}, stats2 = {0};
    strncpy(stats1.func_name, func1_name, MAX_NAME_LENGTH-1);
    strncpy(stats2.func_name, func2_name, MAX_NAME_LENGTH-1);
    
    print_separator(TABLE_WIDTH);
    print_centered("PERFORMANCE COMPARISON", TABLE_WIDTH);
    print_separator(TABLE_WIDTH);
    
    printf( "Total test cases: %d\n", num_test_cases);
    printf("Iterations per test: %d\n" , iterations);
    print_separator(TABLE_WIDTH);
    
    printf("%-20s | %-15s | %-15s | %-15s | %-15s\n", 
           "Test Case", "Size", func1_name, func2_name, "Speedup");
    print_separator(TABLE_WIDTH);
    
    for (int i = 0; i < num_test_cases; i++) {
        TestCase* tc = &test_cases[i];
        
        TestResult result1 = run_single_test(func1, tc->size, tc->input, iterations);
        TestResult result2 = run_single_test(func2, tc->size, tc->input, iterations);
        
        stats1.times[i] = result1.time_taken;
        stats2.times[i] = result2.time_taken;
        
        double speedup = result2.time_taken / result1.time_taken;
        
        printf("%-20s | %4d x %-8d | %13.6f | %13.6f | %13.2fx\n",
               tc->description, tc->size, tc->size,
               result1.time_taken, result2.time_taken, speedup);
        
        if (validate_func != NULL) {
            if (!validate_func(tc->size, result1.output, result2.output)) {
                printf( "WARNING: Output validation failed for test case %d!\n" , i + 1);
            }
        }
        
        free(result1.output);
        free(result2.output);
    }
    print_separator(TABLE_WIDTH);
    
    // Calculate statistics
    for (int i = 0; i < num_test_cases; i++) {
        // Function 1 stats
        stats1.avg_time += stats1.times[i];
        stats1.min_time = (i == 0) ? stats1.times[i] : 
                         (stats1.times[i] < stats1.min_time ? stats1.times[i] : stats1.min_time);
        stats1.max_time = (i == 0) ? stats1.times[i] : 
                         (stats1.times[i] > stats1.max_time ? stats1.times[i] : stats1.max_time);
        
        // Function 2 stats
        stats2.avg_time += stats2.times[i];
        stats2.min_time = (i == 0) ? stats2.times[i] : 
                         (stats2.times[i] < stats2.min_time ? stats2.times[i] : stats2.min_time);
        stats2.max_time = (i == 0) ? stats2.times[i] : 
                         (stats2.times[i] > stats2.max_time ? stats2.times[i] : stats2.max_time);
    }
    
    stats1.avg_time /= num_test_cases;
    stats2.avg_time /= num_test_cases;
    
    printf("\nSUMMARY STATISTICS:\n");
    printf("%-20s | %-15s | %-15s | %-15s\n", "Function", "Min Time", "Avg Time", "Max Time");
    print_separator(TABLE_WIDTH);
    printf("%-20s | %13.6f | %13.6f | %13.6f\n",
           stats1.func_name, stats1.min_time, stats1.avg_time, stats1.max_time);
    printf("%-20s | %13.6f | %13.6f | %13.6f\n",
           stats2.func_name, stats2.min_time, stats2.avg_time, stats2.max_time);
    print_separator(TABLE_WIDTH);
    
    double overall_speedup = stats2.avg_time / stats1.avg_time;
    if (overall_speedup > 1.0) {
        printf(  "\nOverall: %s is %.2fx faster than %s\n" ,
               func1_name, overall_speedup, func2_name);
    } else {
        printf(  "\nOverall: %s is %.2fx slower than %s\n" ,
               func1_name, 1.0 / overall_speedup, func2_name);
    }
}

#endif