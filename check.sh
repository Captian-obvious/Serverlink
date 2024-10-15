#!/usr/bin/env bash
ls
# Directory to check
directory="./path_to_directory"

# File to check for
file_name="sl"

# Check if the file exists in the directory
if [[ -f "$directory/$file_name" ]]; then
  echo "File '$file_name' found in directory '$directory'."
else
  echo "File '$file_name' not found in directory '$directory'."
fi
