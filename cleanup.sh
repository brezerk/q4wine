#!/bin/sh
#
# This is cealnup script for cleaning make, tmp and other build files.
#
# Plz. Use it before commit project to git

echo "Run: make clean";
make clean;
echo "Removing cmake tmp";
rm -rdf CMakeCache.txt CMakeFiles config.h cmake_install.cmake Makefile build install_manifest.txt qtcreator-build qtc-gdbmacros CMakeLists.txt.user;
echo "Removing *~ fiels";
find ./ -name '*~' -delete;
find ./ -name '*.qm' -delete;
echo "Done";
