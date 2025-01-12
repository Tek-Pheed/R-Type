#!/bin/bash

SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")
PROCS=$(nproc)

cd $SCRIPTPATH/..
printf "### Pulling dependent submodules...\n"
git submodule update --init --recursive

printf "\n### Generating build files...\n"
set -e
cmake -B build . -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O2"
cd build
printf "\n### Building R-Type...\n"
make -j $PROCS
printf "\n### Build done\n"

RELEASE_DIR=$SCRIPTPATH/../release/

printf "\n### Copying produced binaries...\n"
find . -type f \( -name "*.so" -o -name "*.so.*" \) -exec cp -P {} $RELEASE_DIR \;
find . -type f \( -name "*.dylib" -o -name "*.dylib" \) -exec cp -P {} $RELEASE_DIR \;

find . -type l \( -name "*.so" -o -name "*.so.*" \) -exec cp -P {} $RELEASE_DIR \;
find . -type l \( -name "*.dylib" -o -name "*.dylib" \) -exec cp -P {} $RELEASE_DIR \;

if [ ! -d "$SCRIPTPATH/../release/assets" ]; then
    printf "\n### Copying assets...\n"
    cp -fR $SCRIPTPATH/../assets/ $SCRIPTPATH/../release/assets
fi

printf "\n### Done\n"
