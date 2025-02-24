#!/bin/bash

# Check if exactly 2 arguments are passed
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 'line_to_find' 'replacement_line'"
    exit 1
fi

# Read arguments
line_to_find="$1"
replacement_line="$2"

# Loop through all files in the current directory
for file in *; do
    # Skip directories
    if [ -f "$file" ]; then
        # Replace the line in the file
        sed -i "s|$line_to_find|$replacement_line|g" "$file"
        echo "Processed: $file"
    fi
done

echo "Replacement complete."

