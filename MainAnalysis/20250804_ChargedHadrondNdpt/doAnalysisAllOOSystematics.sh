#!/bin/bash

DOSYSTEMATICSTRACK=true

if [ "$DOSYSTEMATICSTRACK" = true ]; then
    echo "Running PlotSystematics..."
./RunParallelReadParam.sh --Output OOSystNominalTrack --Input OO_InputFileList_Reference.txt --CollisionSystem true --ApplyEventSelection true --UseEventWeight false --UseSpeciesWeight false --UseTrackWeight true --TrackWeightSelection 1 --TriggerChoice 1 --EventSelectionOption 1 --SpeciesCorrectionOption 0 --ScaleFactor 1
./RunParallelReadParam.sh --Output OOSystLooseTrack   --Input OO_InputFileList_Reference.txt --CollisionSystem true --ApplyEventSelection true --UseEventWeight false --UseSpeciesWeight false --UseTrackWeight true --TrackWeightSelection 2 --TriggerChoice 1 --EventSelectionOption 1 --SpeciesCorrectionOption 0 --ScaleFactor 1
./RunParallelReadParam.sh --Output OOSystTightTrack   --Input OO_InputFileList_Reference.txt --CollisionSystem true --ApplyEventSelection true --UseEventWeight false --UseSpeciesWeight false --UseTrackWeight true --TrackWeightSelection 3 --TriggerChoice 1 --EventSelectionOption 1 --SpeciesCorrectionOption 0 --ScaleFactor 1

## HERE WE NEED TO ADD THE MACROS FOR COMPARING CROSS_SECTIONS and extract systematics
fi
