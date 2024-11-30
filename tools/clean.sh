#!/bin/bash

SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

cd $SCRIPTPATH/..

set -e
cd build
rm -rf .
printf "Cleaned build directory"
