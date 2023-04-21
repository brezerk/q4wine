#!/bin/bash

set -e

echo ' * Following script will remove all files not needed for release process.
 * Hit ^C to abort or any key to continue.'
read

SRC_DIR="$(dirname "$(realpath -s "${0}")")"

cd "${SRC_DIR}"
# rm -rf .git build config.h doc
rm -rf build
find . -name 'CMakeLists.txt.user' -delete
find . -name 'CMakeCache.txt' -delete
find . -name 'CMakeCache.txt.*' -delete
find . -name 'cmake_install.cmake' -delete
find . -name 'qtcsettings.cmake' -delete
find . -name 'Makefile' -delete
find . -name 'config.h' -delete
find . -name 'q4wine.qrc.depends' -delete
find . -name 'CMakeFiles' -type d -print0 | xargs -0 rm -rf
find . -name '.cache' -type d -print0 | xargs -0 rm -rf
find . -name '.cmake' -type d -print0 | xargs -0 rm -rf
find . -name '.qtc_clangd' -type d -print0 | xargs -0 rm -rf
find . -name '*.swo' -delete
find . -name '*.swp' -delete
find . -name '*.log' -delete
find . -name '*.cpp_parameters' -delete
find . -depth -empty -type d -delete

echo 'Done. Happy releasing :P'
