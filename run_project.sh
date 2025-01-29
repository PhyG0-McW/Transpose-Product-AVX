#!/bin/bash
set -e  # Exit on error

echo "Initializing and updating submodules..."
git submodule update --init --recursive

echo "Navigating to googletest directory..."
cd googletest

echo "Creating build directory..."
mkdir -p build
cd build

echo "Running CMake..."
cmake ..

cd ..
cd ..

echo "Building the project..."
make

echo "Running the project..."
make run

echo "Running tests..."
make test
