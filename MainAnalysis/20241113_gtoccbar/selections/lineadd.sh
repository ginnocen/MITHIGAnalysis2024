#!/bin/bash

# Check if the user supplied a line to add
if [ $# -lt 1 ]; then
    echo "Usage: $0 'line to add'"
    exit 1
fi

# The line to add
line_to_add="$1"

# Iterate over all files in the current directory
for file in *.csv; do
    if [ -f "$file" ]; then
        # Append the line to the bottom of the file
        echo "$line_to_add" >> "$file"
        echo "Added line to $file"
    fi
done

echo "Line added to all files in the current directory."

