#!/bin/bash

# TODO: Check for missing system dependencies


SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")
PROCS=$(nproc)

cd $SCRIPTPATH/..
printf "### Pulling dependent submodules\n"
git submodule update --remote --recursive

printf "\n### Generating build files\n"
set -e
cmake -B build .
cd build
printf "\n### Building R-Type\n"
make -j $PROCS
printf "\n### Build done\n"