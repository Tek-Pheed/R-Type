#!/bin/sh

mkdir -p __cppcheckWorkaroundInclude
find . -type f \( -iname \*.h -o -iname \*.inl -o -iname \*.hpp \) -exec cp {} __cppcheckWorkaroundInclude \;

cppcheck --enable=all --quiet  --suppress=missingInclude \
  --suppress=unusedFunction --suppress=unmatchedSuppression:{} \
--error-exitcode=1 -I__cppcheckWorkaroundInclude . > check.txt 2>&1
