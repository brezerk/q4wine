#!/bin/bash
# Following script will remove all files not needed for release process.
echo 'Prep for release? (destructive)'
read
SRC_DIR="$(dirname "$(realpath -s "${0}")")"
pushd "${SRC_DIR}"
rm -rf .git build config.h doc
find . -name CMakeLists.txt.user -delete
find . -name CMakeCache.txt -delete
find . -name cmake_install.cmake -delete
find . -name '*.swo' -delete
find . -name '*.swp' -delete
find . -depth -empty -type d -delete
popd
echo 'Done. Happy releasing :P'
