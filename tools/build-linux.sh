#!/bin/bash

# TODO: Check for missing system dependencies


SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

cd $SCRIPTPATH/..
printf "### Pulling dependent submodules\n"
git pull --recurse-submodules

printf "\n### Generating build files\n"
set -e
cmake -B build .
cd build
printf "\n### Building R-Type\n"
make
printf "\n### Build done\n"