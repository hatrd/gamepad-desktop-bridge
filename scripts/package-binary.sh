#!/bin/bash
# Binary package creation script

set -e

VERSION=${1:-"dev"}
PLATFORM=$(uname -s | tr '[:upper:]' '[:lower:]')
ARCH=$(uname -m)

if [ "$ARCH" = "x86_64" ]; then
    ARCH="x64"
elif [ "$ARCH" = "aarch64" ]; then
    ARCH="arm64"
fi

PACKAGE_NAME="xbox-controller-api-${VERSION}-${PLATFORM}-${ARCH}"
BUILD_DIR="build"

echo "Creating binary package: $PACKAGE_NAME"

if [ ! -d "$BUILD_DIR" ]; then
    echo "Error: Build directory not found. Please build the project first."
    exit 1
fi

# Create package directory
PACKAGE_DIR="$PACKAGE_NAME"
rm -rf "$PACKAGE_DIR"
mkdir -p "$PACKAGE_DIR"

# Copy binary
if [ -f "$BUILD_DIR/xbox_controller_api" ]; then
    cp "$BUILD_DIR/xbox_controller_api" "$PACKAGE_DIR/"
elif [ -f "$BUILD_DIR/Release/xbox_controller_api.exe" ]; then
    cp "$BUILD_DIR/Release/xbox_controller_api.exe" "$PACKAGE_DIR/"
elif [ -f "$BUILD_DIR/xbox_controller_api.exe" ]; then
    cp "$BUILD_DIR/xbox_controller_api.exe" "$PACKAGE_DIR/"
else
    echo "Error: Executable not found in build directory"
    exit 1
fi

# Copy documentation
cp README.md "$PACKAGE_DIR/"
cp BUILD.md "$PACKAGE_DIR/"
[ -f LICENSE ] && cp LICENSE "$PACKAGE_DIR/"
[ -f CHANGELOG.md ] && cp CHANGELOG.md "$PACKAGE_DIR/"

# Copy dependencies (platform specific)
if [ "$PLATFORM" = "windows" ]; then
    # Windows DLLs
    find "$BUILD_DIR" -name "*.dll" -exec cp {} "$PACKAGE_DIR/" \;
elif [ "$PLATFORM" = "linux" ]; then
    # Linux shared libraries
    mkdir -p "$PACKAGE_DIR/lib"
    # Copy SDL3 and other shared libs if needed
    ldd "$BUILD_DIR/xbox_controller_api" | grep "=> /" | awk '{print $3}' | grep -E "(sdl|SDL)" | xargs -I {} cp {} "$PACKAGE_DIR/lib/" 2>/dev/null || true
elif [ "$PLATFORM" = "darwin" ]; then
    # macOS frameworks and dylibs
    mkdir -p "$PACKAGE_DIR/lib"
    otool -L "$BUILD_DIR/xbox_controller_api" | grep -E "(sdl|SDL)" | awk '{print $1}' | xargs -I {} cp {} "$PACKAGE_DIR/lib/" 2>/dev/null || true
fi

# Create archive
if [ "$PLATFORM" = "windows" ]; then
    zip -r "${PACKAGE_NAME}.zip" "$PACKAGE_DIR"
    echo "Package created: ${PACKAGE_NAME}.zip"
else
    tar -czf "${PACKAGE_NAME}.tar.gz" "$PACKAGE_DIR"
    echo "Package created: ${PACKAGE_NAME}.tar.gz"
fi

# Cleanup
rm -rf "$PACKAGE_DIR"

echo "Package size: $(du -h ${PACKAGE_NAME}.* | cut -f1)"