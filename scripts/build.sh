#!/bin/bash
# Development build script

set -e

BUILD_TYPE=${1:-Release}
PARALLEL_JOBS=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

echo "Building Xbox Controller API..."
echo "Build type: $BUILD_TYPE"
echo "Parallel jobs: $PARALLEL_JOBS"

# Create build directory
mkdir -p build
cd build

# Configure
echo "Configuring CMake..."
cmake .. \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# Build
echo "Building..."
cmake --build . --config $BUILD_TYPE --parallel $PARALLEL_JOBS

echo "Build completed successfully!"
echo "Executable location: $(pwd)/xbox_controller_api"

# Run tests if available
if command -v ctest &> /dev/null; then
    echo "Running tests..."
    ctest --build-config $BUILD_TYPE --output-on-failure
fi