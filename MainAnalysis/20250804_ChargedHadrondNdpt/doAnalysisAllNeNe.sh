#!/bin/bash


MODE="output_NeNeReferenceCentralValue"
./RunParallelReadParam.sh \
    --Output ${MODE} \
    --Input NeNe_InputFileList.txt \
    --CollisionSystem true \
    --ApplyEventSelection true \
    --UseEventWeight true \
    --UseSpeciesWeight true \
    --UseTrackWeight true \
    --TrackWeightSelection 2 \
    --TriggerChoice 1 \
    --EventSelectionOption 2 \
    --SpeciesCorrectionOption 2 \
    --ScaleFactor 1

echo "Running PlotTrkPtVariantsWithRatioToUnweighted_Save..."
root -l -b -q 'PlotTrkPtVariantsWithRatioToUnweighted_Save.C("'"${MODE}/MergedOutput.root"'", "'"${MODE}/trkPtVariants_ratioToRaw.png"'", "'"${MODE}/trkPtVariants_ratioToRaw.root"'")'
