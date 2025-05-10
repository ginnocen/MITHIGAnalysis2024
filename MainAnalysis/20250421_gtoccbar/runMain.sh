#!/bin/bash
set -e  # Exit on error

echo "=== Running muon corrections ==="
cd acceptanceEfficiency || exit 1
root -l -b -q muonCorrections.C
cp muonCorrections.root ../readData

echo "=== Running data reader ==="
cd ../readData || exit 1
root -l -b -q dataReader.C
cp readData.root ../unfold

echo "=== Running unfolding steps ==="
cd ../unfold || exit 1

echo "--- Cleaning and compiling RooUnfold ---"
make clean
make

echo "--- Filling response ---"
root -l -q RunReadExample.C
cp response.root .

echo "--- Unfolding and computing ratios ---"
root -l -b -q RunReadSystUncJESdecomp.C

echo "=== All steps completed successfully ==="

