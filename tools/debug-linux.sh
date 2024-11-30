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

#-DCMAKE_C_FLAGS="-fsanitize=thread"