#!/bin/bash

cmake . && cmake -B build -S . && make -C build > temp.txt 2>&1 || (echo "::error title=Build Checker Linux::Build Failed" && exit 1)
