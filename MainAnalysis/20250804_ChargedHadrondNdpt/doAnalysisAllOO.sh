#!/bin/bash

LUMIAAtoPP=150000

MODE="output_OOReferenceCentralValue"
./RunParallelReadParam.sh \
    --Output ${MODE} \
    --Input OO_InputFileList_Reference.txt \
    --CollisionSystem true \
    --ApplyEventSelection true \
    --UseEventWeight false \
    --UseSpeciesWeight false \
    --UseTrackWeight true \
    --TrackWeightSelection 2 \
    --TriggerChoice 1 \
    --EventSelectionOption 1 \
    --SpeciesCorrectionOption 0 \
    --ScaleFactor 1

echo "Running PlotTrkPtVariantsWithRatioToUnweighted_Save..."
root -l -b -q 'PlotTrkPtVariantsWithRatioToUnweighted_Save.C("'"${MODE}/MergedOutput.root"'", "'"${MODE}/trkPtVariants_ratioToRaw.png"'", "'"${MODE}/trkPtVariants_ratioToRaw.root"'")'

echo "Running spectra macro..."
g++ -o Spectra Spectra.cpp $(root-config --cflags --glibs) -lASImage

# Run the spectra macro for OO
# Inputs:
# 1. Collision system: e.g. "OO"
# 2. Input file for AA spectra
# 3. Histogram name for AA spectra
# 4. Input file for AA systematics
# 5. Output histogram name for AA systematics
# 6. Lumi scaling for OO
# 7. Output directory for results

./Spectra "OO" "output_OOReferenceCentralValue/MergedOutput.root" "hTrkPt" "ResultsUIC/pp_OO_raa_20250729_Unblinding_Final_v3.root" "OO_Total_uncertainty" $LUMIAAtoPP "output_OOReferenceCentralValue"


