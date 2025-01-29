# Compiler and flags
CC := gcc
CXX := g++  # Use g++ for GTest linking
CFLAGS := -Wall -Werror
GTEST_LIBS := -lgtest -lgtest_main -lpthread

# Directories
BUILD_DIR := build
COMMON_DIR := common
TRANSPOSE_DIR := Transpose
PRODUCT_DIR := Product
TEST_DIR := tests

# Include directory for headers
INCLUDES := -I$(COMMON_DIR) -I$(TRANSPOSE_DIR) -I$(PRODUCT_DIR)

# Output directory
TRANSPOSE_OUT := $(BUILD_DIR)/$(TRANSPOSE_DIR)
PRODUCT_OUT := ${BUILD_DIR}/${PRODUCT_DIR}
TEST_OUT := $(BUILD_DIR)/tests

# Source and header files
COMMON_SRC := $(COMMON_DIR)/common.c
TRANSPOSE_AVX128_SRC := $(TRANSPOSE_DIR)/main_avx128.c
TRANSPOSE_AVX256_SRC := $(TRANSPOSE_DIR)/main_avx256.c
PRODUCT_SRC := ${PRODUCT_DIR}/main.c
TEST_SRC := $(TEST_DIR)/test_main.cpp

# Header files
TRANSPOSE_AVX128_HDR := $(TRANSPOSE_DIR)/main_avx128.h
TRANSPOSE_AVX256_HDR := $(TRANSPOSE_DIR)/main_avx256.h
PRODUCT_HDR := ${PRODUCT_DIR}/main.h
COMMON_HDR := $(COMMON_DIR)/common.h

# Target executables
TRANSPOSE_AVX128_TARGET := $(TRANSPOSE_OUT)/avx128
TRANSPOSE_AVX256_TARGET := $(TRANSPOSE_OUT)/avx256
PRODUCT_TARGET := ${PRODUCT_OUT}/product
TEST_TARGET := $(TEST_OUT)/run_tests

# Default target - excludes tests
.PHONY: all
all: $(TRANSPOSE_AVX128_TARGET) $(TRANSPOSE_AVX256_TARGET) $(PRODUCT_TARGET)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(TRANSPOSE_OUT)
	mkdir -p ${PRODUCT_OUT}
	mkdir -p $(TEST_OUT)

# Transpose AVX-128 target
$(TRANSPOSE_AVX128_TARGET): $(TRANSPOSE_AVX128_SRC) $(COMMON_SRC) $(TRANSPOSE_AVX128_HDR) $(COMMON_HDR) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -mavx $(INCLUDES) $(TRANSPOSE_AVX128_SRC) $(COMMON_SRC) -o $@ -lm

# Transpose AVX-256 target
$(TRANSPOSE_AVX256_TARGET): $(TRANSPOSE_AVX256_SRC) $(COMMON_SRC) $(TRANSPOSE_AVX256_HDR) $(COMMON_HDR) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -mavx2 -mavx $(INCLUDES) $(TRANSPOSE_AVX256_SRC) $(COMMON_SRC) -o $@ -lm

# Product target
${PRODUCT_TARGET}: ${PRODUCT_SRC} $(COMMON_SRC) ${PRODUCT_HDR} $(COMMON_HDR) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -mavx2 -mavx $(INCLUDES) ${PRODUCT_SRC} $(COMMON_SRC) -o $@ -lm

# Test target - separate
$(TEST_TARGET): $(TEST_SRC) $(COMMON_HDR) | $(BUILD_DIR)
	$(CXX) $(CFLAGS) -mavx2 -mavx -I$(COMMON_DIR) $< $(GTEST_LIBS) -o $@

# Clean target
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

# Run main executables
.PHONY: run
run: all
	@echo "Running Transpose AVX-128 vs Scalar..."
	@$(TRANSPOSE_AVX128_TARGET)
	@echo "Running Transpose AVX-256 vs Scalar..."
	@$(TRANSPOSE_AVX256_TARGET)
	@echo "Running Multiplication AVX-256 vs Scalar..."
	@$(PRODUCT_TARGET)

# Separate target for running tests
.PHONY: test
test: $(TEST_TARGET)
	@echo "Running Unit Tests..."
	@$(TEST_TARGET)