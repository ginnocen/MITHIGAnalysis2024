#!/bin/bash

# Usage: ./run_filter.sh <input_file.root> <output_file.root>

# Check for arguments
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <input_file.root> <output_file.root>"
    exit 1
fi

# Variables
INPUT_FILE=$1
OUTPUT_FILE=$2

# Run the compiled program
rm filterEvents
g++ -o filterEvents filterEvents.cpp `root-config --cflags --libs`
./filterEvents "$INPUT_FILE" "$OUTPUT_FILE"
