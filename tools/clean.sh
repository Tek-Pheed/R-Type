#!/bin/bash

SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

cd $SCRIPTPATH/..

set -e
rm -r build/*
rm -r release/*
printf "Cleaned build directory\n"
