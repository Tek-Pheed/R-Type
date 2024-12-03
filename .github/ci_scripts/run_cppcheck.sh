#!/bin/sh

cppcheck --enable=all --quiet --suppress=missingIncludeSystem --suppress=missingInclude \
--suppress=unusedFunction --suppress=preprocessorErrorDirective --suppress=unmatchedSuppression --project=build/compile_commands.json \
--error-exitcode=0 > check.txt 2>&1
