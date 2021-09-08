#!/bin/bash

set -e

echo ' * Following script will remove all files not needed for release process.
 * Hit ^C to abort or any key to continue.'
read

SRC_DIR="$(dirname "$(realpath -s "${0}")")"

cd "${SRC_DIR}"
rm   -rf .git build config.h doc
find . -name CMakeLists.txt.user -delete
find . -name CMakeCache.txt -delete
find . -name cmake_install.cmake -delete
find . -name '*.swo' -delete
find . -name '*.swp' -delete
find . -depth -empty -type d -delete

echo 'Done. Happy releasing :P'
