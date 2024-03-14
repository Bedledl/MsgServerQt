#!/bin/bash

# The CI pipeline does currently not use this scripts, but a public github action for clang format. This script is kept for local development.

# This script checks if the code is formatted according to the .clang-format file. An optional parameter can be passed to fix the format in place.
# Usage: ./scripts/run-clang-format.sh [fix]

# Check if clang-format is installed
if ! [ -x "$(command -v clang-format)" ]; then
  echo 'Error: clang-format is not installed.' >&2
  exit 1
fi

# Check if .clang-format file exists
if [ ! -f ".clang-format" ]; then
  echo 'Error: .clang-format file not found.' >&2
  exit 1
fi

# Check if an optional parameter is passed to fix the format in place
if [ "$1" == "fix" ]; then
  echo "Fixing the format in place"
  RUN_MODE="-i"
else
  RUN_MODE="--dry-run"
fi

# First, get all of the source files and headers to be analyzed.
SOURCES=$(find -name "*.cpp" -not -path "./build/*")
HEADERS_C_STYLE=$(find -name "*.h" -not -path "./build/*")
HEADERS_CPP_STYLE=$(find -name "*.hpp" -not -path "./build/*")
PROTO_FILES=$(find -name "*.proto" -not -path "./build/*")

# Define output files
TMP_FILES_LIST_FILE=".tmp-clang-format-files-list"
TMP_REPORT_FILE=".tmp-clang-format-report"

# Ensure that output files are empty, so without previous log data
echo -n "" >${TMP_REPORT_FILE}
echo -n "" >${TMP_FILES_LIST_FILE}

for FILES in ${SOURCES} ${HEADERS_C_STYLE} ${HEADERS_CPP_STYLE} ${PROTO_FILES}; do
  if [ -n "$FILES" ]; then
    clang-format ${RUN_MODE} -style=file $FILES >>$TMP_REPORT_FILE
    echo $FILES >>$TMP_FILES_LIST_FILE
  fi
done

if [ ! -s "$TMP_REPORT_FILE" ]; then
  # report file is empty
  rm $TMP_REPORT_FILE
  exit 0
fi

echo "clang-format check failed, see report in $TMP_REPORT_FILE. The list of files that were analyzed is in $TMP_FILES_LIST_FILE"
exit 1
