#!/bin/bash

# Check if an argument is provided
if [ $# -ne 1 ]; then
    echo "Usage: $0 <string_to_remove>"
    exit 1
fi

STRING_TO_REMOVE=$1

# Loop through all files in the current directory
for file in *; do
    # Ensure it's a regular file (not a directory)
    if [ -f "$file" ]; then
        # Use sed to remove the string and delete lines containing it
        sed -i "/$STRING_TO_REMOVE/d" "$file"
        echo "Processed: $file"
    fi
done

echo "String '$STRING_TO_REMOVE' and any line containing it removed from all files in the directory."

