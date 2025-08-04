#!/bin/bash


LUMIAAtoPP=1.0


MODE="output_OOReferenceCentralValue"
./RunParallelReadParam.sh --Output ${MODE} --Input OO_InputFileList_Reference.txt --CollisionSystem true --ApplyEventSelection true --UseEventWeight false --UseSpeciesWeight false --UseTrackWeight true --TrackWeightSelection 2 --MinTrackPt 400 --MinLeadingTrackPt -1 --TriggerChoice 1 --EventSelectionOption 1 --SpeciesCorrectionOption 0 --ScaleFactor 1

echo "Running PlotTrkPtVariantsWithRatioToUnweighted_Save..."
root -l -b -q 'PlotTrkPtVariantsWithRatioToUnweighted_Save.C("'"${MODE}/MergedOutput.root"'", "'"${MODE}/trkPtVariants_ratioToRaw.png"'", "'"${MODE}/trkPtVariants_ratioToRaw.root"'")'

echo "Running spectra macro..."
g++ -o Spectra Spectra.cpp $(root-config --cflags --glibs) -lASImage
./Spectra


