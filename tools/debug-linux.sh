#!/bin/bash

SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")
PROCS=$(nproc)

cd $SCRIPTPATH/..
printf "### Pulling dependent submodules\n"
git submodule update --init --recursive

printf "\n### Generating build files\n"
set -e
cmake -DCMAKE_BUILD_TYPE=Debug -B build .
cd build
printf "\n### Building R-Type\n"
make -j $PROCS
printf "\n### Build done\n"

RELEASE_DIR=$SCRIPTPATH/../release/

printf "\n### Copying shared libraries (.so files)\n"
find . -type f \( -name "*.so" -o -name "*.so.*" \) -exec cp -P {} $RELEASE_DIR \;
find . -type f \( -name "*.dylib" -o -name "*.dylib" \) -exec cp -P {} $RELEASE_DIR \;

find . -type l \( -name "*.so" -o -name "*.so.*" \) -exec cp -P {} $RELEASE_DIR \;
find . -type l \( -name "*.dylib" -o -name "*.dylib" \) -exec cp -P {} $RELEASE_DIR \;

printf "\n### Shared libraries copied to release folder\n"