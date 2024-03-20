#!/usr/bin/python3

# The CI pipeline does currently not use this scripts, but a public github action for clang tidy. This script is kept for local development.

# This script analyzes the code with clang-tidy according to the rules in the .clang-tidy file. An optional parameter can be passed to try to fix errors in place.
# Usage: ./scripts/run-clang-tidy.py [fix]

from os import path, cpu_count
from glob import glob
from subprocess import run, PIPE
from sys import argv
from distutils.spawn import find_executable
from concurrent.futures import ThreadPoolExecutor

# Check if clang-tidy is installed
if not find_executable("clang-tidy"):
    print("Error: clang-tidy is not installed.")
    exit(1)

# Check if .clang-tidy file exists
if not path.isfile(".clang-tidy"):
    print("Error: .clang-tidy file not found.")
    exit(1)

# Check if an optional parameter is passed to try to fix erros in place
fix_inplace = ""
build_folder = "build"
for arg in argv[1:]:
    if arg == "--fix":
        fix_inplace = "-fix-errors"
        continue
    elif arg.startswith("--build="):
        build_folder = arg.split("=")[1]
        continue
    else:
        print("Error: Invalid argument. Please pass '--fix' to try to fix errors in place or '--build=<build_folder>' to specify the build folder.")
        exit(1)

# First, get all of the source files and headers to be analyzed.
files = glob('**/*.cpp', recursive=True)
files += glob('**/*.h', recursive=True)
files += glob('**/*.hpp', recursive=True)

files = list(filter(lambda file: not file.startswith("build/") and not file.startswith("CMake"), files))

print(files)

# Define output files
TMP_FILES_LIST_FILE=".tmp-clang-tidy-files-list"
TMP_REPORT_FILE=".tmp-clang-tidy-report"

# Ensure that output files are empty, so without previous log data
# echo -n "" >${TMP_REPORT_FILE}
# echo -n "" >${TMP_FILES_LIST_FILE}

def run_clang_tidy(file):
  print("Starting clang-tidy for file: " + file)
  clang_tidy_result = run(['clang-tidy', '-p', build_folder, fix_inplace, '-extra-arg=-std=c++17', '--config-file=.clang-tidy', file], stdout=PIPE)
  # TODO store result in file
  print("Finished clang-tidy for file: " + file)
  print(clang_tidy_result.stdout.decode('utf-8'))


with ThreadPoolExecutor(max_workers=cpu_count()) as executor:
    for file in files:
       future = executor.submit(run_clang_tidy, file)


# if [ ! -s "$TMP_REPORT_FILE" ]; then
#   # report file is empty
#   rm $TMP_REPORT_FILE
#   exit 0
# fi

# echo "clang-tidy check failed, see report in $TMP_REPORT_FILE. The list of files that were analyzed is in $TMP_FILES_LIST_FILE"
# exit 1
