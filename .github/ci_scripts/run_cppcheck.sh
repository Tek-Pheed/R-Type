#!/bin/sh

cppcheck --enable=all --quiet --suppress=missingIncludeSystem --suppress=missingInclude \
--suppress=unusedFunction --suppress=preprocessorErrorDirective --suppress=unmatchedSuppression --project=build/compile_commands.json \
--suppress=useStlAlgorithm --suppress=checkersReport --error-exitcode=1 > check.txt 2>&1
