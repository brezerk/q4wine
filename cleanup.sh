#!/bin/bash

echo ' * Following script will remove all files not needed for release process. Hit C^ to abort or any key to continue.'
read

rm -rf .git build config.h
find -name CMakeLists.txt.user -delete
find -name CMakeCache.txt -delete
find -name cmake_install.cmake -delete
find -name '*.swo' -delete
find -name '*.swp' -delete
find doc -mindepth 1 -delete


echo 'Done. Happy releasing :P'
