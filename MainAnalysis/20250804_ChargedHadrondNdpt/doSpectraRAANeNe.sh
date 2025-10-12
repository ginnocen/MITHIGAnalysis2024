#!/bin/bash

LUMIAAtoPP=10793.05184615 # lumi in mb-1, matches the golden JSON file overlap with Lynn's JSON, divided by 60 (per PD lumi).
#85743.91 OO lumi, Jordan JSON

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

./Spectra "NeNe" "output_NeNeReferenceCentralValue/MergedOutput.root" "hTrkPt" "systematic_variations/nenesystematics.root" "hSystematic_total" $LUMIAAtoPP "output_NeNeReferenceCentralValue"

root -l RAA_NeNe.C
