#!/bin/sh
#
# This is cealnup script for cleaning make, tmp and other build files.
#
# Plz. Use it before commit project to git

echo "Run: make clean";
make clean;

names="CMakeCache.txt config.h cmake_install.cmake Makefile install_manifest.txt CMakeLists.txt.user *.cbp *~ *.ccx ui_*.h *.so *.a *.gz *.bz2 *.qm"

for name in $names
do
    echo "Removing files: ${name}"
    find ./ -name ${name} -type f -delete;
done

tmp_dirs="CMakeFiles build qtcreator-build"

for tmp_dir in $tmp_dirs
do
    echo "Removing tmp dirs: ${tmp_dir}"
    find ./ -name ${tmp_dir} -type d -exec rm -rf '{}' \;
done

echo "Done";
