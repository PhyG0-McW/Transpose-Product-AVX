# Compiler and flags
CC := gcc
CFLAGS := -Wall -Werror

# Directories
BUILD_DIR := build
COMMON_DIR := common
TRANSPOSE_DIR := Transpose
PRODUCT_DIR := Product

# Include directory for common.h
INCLUDES := -I$(COMMON_DIR)

# Output directory
TRANSPOSE_OUT := $(BUILD_DIR)/$(TRANSPOSE_DIR)
PRODUCT_OUT := ${BUILD_DIR}/${PRODUCT_DIR}

# Source files
TRANSPOSE_AVX128_SRC := $(TRANSPOSE_DIR)/main_avx128.c
TRANSPOSE_AVX256_SRC := $(TRANSPOSE_DIR)/main_avx256.c
PRODUCT_SRC := ${PRODUCT_DIR}/main.c

# Target executables
TRANSPOSE_AVX128_TARGET := $(TRANSPOSE_OUT)/avx128
TRANSPOSE_AVX256_TARGET := $(TRANSPOSE_OUT)/avx256
PRODUCT_TARGET := ${PRODUCT_OUT}/product

# Default target
.PHONY: all
all: $(TRANSPOSE_AVX128_TARGET) $(TRANSPOSE_AVX256_TARGET) $(PRODUCT_TARGET)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(TRANSPOSE_OUT)
	mkdir -p ${PRODUCT_OUT}

# Transpose AVX-128 target
$(TRANSPOSE_AVX128_TARGET): $(TRANSPOSE_AVX128_SRC) $(COMMON_DIR)/common.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -mavx $(INCLUDES) $< -o $@ -lm

# Transpose AVX-256 target
$(TRANSPOSE_AVX256_TARGET): $(TRANSPOSE_AVX256_SRC) $(COMMON_DIR)/common.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -mavx2 -mavx $(INCLUDES) $< -o $@ -lm

${PRODUCT_TARGET}: ${PRODUCT_SRC} $(COMMON_DIR)/common.h | $(BUILD_DIR) 
	$(CC) $(CFLAGS) -mavx2 -mavx $(INCLUDES) $< -o $@ -lm

# Clean target
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

# Run all executables in sequence
.PHONY: run
run: all
	@echo "Running Transpose AVX-128 vs Scalar..."
	@$(TRANSPOSE_AVX128_TARGET)
	@echo "Running Transpose AVX-256 vs Scalar..."
	@$(TRANSPOSE_AVX256_TARGET)
	@echo "Running Multiplication AVX-256 vs Scalar..."
	@$(PRODUCT_TARGET)
