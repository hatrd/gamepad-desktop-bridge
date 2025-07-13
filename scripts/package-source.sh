#!/bin/bash
# Source package creation script

set -e

if [ $# -eq 0 ]; then
    echo "Usage: $0 <version>"
    echo "Example: $0 v1.0.0"
    exit 1
fi

VERSION=$1
PACKAGE_NAME="xbox-controller-api-${VERSION}-source"

echo "Creating source package for version: $VERSION"

# Create temporary directory
TEMP_DIR=$(mktemp -d)
PACKAGE_DIR="$TEMP_DIR/$PACKAGE_NAME"

# Copy source files
mkdir -p "$PACKAGE_DIR"
cp -r . "$PACKAGE_DIR/"

# Navigate to package directory
cd "$PACKAGE_DIR"

# Clean up unnecessary files
echo "Cleaning up unnecessary files..."
rm -rf .git/
rm -rf build*/
rm -rf out/
rm -rf bin/
rm -rf lib/
rm -rf vcpkg_installed/
rm -rf .vs/
rm -rf .vscode/
rm -f *.user
rm -f *.tmp
rm -f *.log
rm -f *.tar.gz

# Create version file
echo "$VERSION" > VERSION

# Navigate back to temp directory
cd "$TEMP_DIR"

# Create archive
echo "Creating archive..."
tar -czf "${PACKAGE_NAME}.tar.gz" "$PACKAGE_NAME/"

# Move to current working directory (not ORIGINAL_DIR)
mv "${PACKAGE_NAME}.tar.gz" "${OLDPWD}/"

# Cleanup
rm -rf "$TEMP_DIR"

echo "Source package created: ${PACKAGE_NAME}.tar.gz"
echo "Size: $(du -h ${OLDPWD}/${PACKAGE_NAME}.tar.gz | cut -f1)"