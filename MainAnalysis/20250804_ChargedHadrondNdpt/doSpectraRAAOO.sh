#!/bin/bash

LUMIAAtoPP=85743.91 ## OO lumi in mb-1, Jordan JSON per 1 PD

echo "Running spectra macro..."
g++ -o Spectra Spectra.cpp $(root-config --cflags --glibs) -lASImage

# Run the spectra macro for NeNe
# Inputs:
# 1. Collision system: e.g. "NeNe"
# 2. Input file for AA spectra
# 3. Histogram name for AA spectra
# 4. Input file for AA systematics
# 5. Output histogram name for AA systematics
# 6. Lumi scaling for NeNe
# 7. Output directory for results

./Spectra "OO" "output_OOReferenceCentralValue/MergedOutput.root" "hTrkPt" "systematic_variations/oosystematics.root" "hSystematic_total" $LUMIAAtoPP "output_OOReferenceCentralValue"

root -l RAA_OO.C
