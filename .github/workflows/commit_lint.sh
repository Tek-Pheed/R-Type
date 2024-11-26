#! /bin/bash

set -e

STANDARD="(ADD|FIX|RM|DOCS|REFACT|TEST|MERGE)"

LAST_COMMITS=$(git log --format="%s" -n 10)

FAILED=false

while IFS= read -r COMMIT; do
  if [[ ! $COMMIT =~ $STANDARD ]]; then
    echo "Error : $COMMIT don't respect the standard"
    FAILED=true
  fi
done <<< "$LAST_COMMITS"

if [ "$FAILED" = true]; then
  exit 1
fi