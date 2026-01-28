#!/usr/bin/env bash
set -e

# Compile loop.C into an executable called 'loop'
g++ -O2 -std=c++17 loop.C $(root-config --cflags --libs) -o loop

# Run it (forward any args)
./loop "$@"
