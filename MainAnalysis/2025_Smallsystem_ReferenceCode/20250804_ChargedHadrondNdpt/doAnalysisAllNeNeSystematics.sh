#!/bin/bash

DOCENTRALVALUE=true    
DOSYSTEMATIC_TRACK=true
DOSYSTEMATIC_EVTSEL=true
DOSYSTEMATIC_SPECIES=true

INFILE="NeNe_InputFileList.txt"



if [ "$DOCENTRALVALUE" = true ]; then
    echo "Running central value..."

    #default
    ./RunParallelReadParam.sh --Output systematic_variations/NeNeCentralValue --Input $INFILE --CollisionSystem true --ApplyEventSelection true --UseEventWeight true --UseSpeciesWeight true --UseTrackWeight true --TrackWeightSelection 2 --TriggerChoice 1 --EventSelectionOption 2 --SpeciesCorrectionOption 2 --ScaleFactor 1
    ./RunParallelReadParam.sh --Output systematic_variations/NeNeUncorrected --Input $INFILE --CollisionSystem true --ApplyEventSelection true --UseEventWeight false --UseSpeciesWeight false --UseTrackWeight false --TrackWeightSelection 2 --TriggerChoice 1 --EventSelectionOption 2 --SpeciesCorrectionOption 2 --ScaleFactor 1
fi

if [ "$DOSYSTEMATIC_TRACK" = true ]; then
    echo "Running track weight systematics..."

    #track weight variations
    ./RunParallelReadParam.sh --Output systematic_variations/NeNeSystLooseTrack   --Input $INFILE --CollisionSystem true --ApplyEventSelection true --UseEventWeight true --UseSpeciesWeight true --UseTrackWeight true --TrackWeightSelection 1 --TriggerChoice 1 --EventSelectionOption 2 --SpeciesCorrectionOption 2 --ScaleFactor 1
    ./RunParallelReadParam.sh --Output systematic_variations/NeNeSystTightTrack   --Input $INFILE --CollisionSystem true --ApplyEventSelection true --UseEventWeight true --UseSpeciesWeight true --UseTrackWeight true --TrackWeightSelection 3 --TriggerChoice 1 --EventSelectionOption 2 --SpeciesCorrectionOption 2 --ScaleFactor 1
    ./RunParallelReadParam.sh --Output systematic_variations/NeNeSansTrackWeight  --Input $INFILE --CollisionSystem true --ApplyEventSelection true --UseEventWeight true --UseSpeciesWeight true --UseTrackWeight false --TrackWeightSelection 2 --TriggerChoice 1 --EventSelectionOption 2 --SpeciesCorrectionOption 2 --ScaleFactor 1
fi

if [ "$DOSYSTEMATIC_EVTSEL" = true ]; then
    echo "Running event selection systematics..."

    #eventselection variations
    ./RunParallelReadParam.sh --Output systematic_variations/NeNeSystLooseEsel   --Input $INFILE --CollisionSystem true --ApplyEventSelection true --UseEventWeight true --UseSpeciesWeight true --UseTrackWeight true --TrackWeightSelection 2 --TriggerChoice 1 --EventSelectionOption 1 --SpeciesCorrectionOption 2 --ScaleFactor 1
    ./RunParallelReadParam.sh --Output systematic_variations/NeNeSystTightEsel   --Input $INFILE --CollisionSystem true --ApplyEventSelection true --UseEventWeight true --UseSpeciesWeight true --UseTrackWeight true --TrackWeightSelection 2 --TriggerChoice 1 --EventSelectionOption 3 --SpeciesCorrectionOption 2 --ScaleFactor 1
    ./RunParallelReadParam.sh --Output systematic_variations/NeNeSansEselWeight  --Input $INFILE --CollisionSystem true --ApplyEventSelection true --UseEventWeight false --UseSpeciesWeight true --UseTrackWeight true --TrackWeightSelection 2 --TriggerChoice 1 --EventSelectionOption 2 --SpeciesCorrectionOption 2 --ScaleFactor 1
fi

if [ "$DOSYSTEMATIC_SPECIES" = true ]; then
    echo "Running species correction systematics..."

    #species correction variations
    ./RunParallelReadParam.sh --Output systematic_variations/NeNeSystLooseSpecies   --Input $INFILE --CollisionSystem true --ApplyEventSelection true --UseEventWeight true --UseSpeciesWeight true --UseTrackWeight true --TrackWeightSelection 2 --TriggerChoice 1 --EventSelectionOption 2 --SpeciesCorrectionOption 1 --ScaleFactor 1
    ./RunParallelReadParam.sh --Output systematic_variations/NeNeSystTightSpecies   --Input $INFILE --CollisionSystem true --ApplyEventSelection true --UseEventWeight true --UseSpeciesWeight true --UseTrackWeight true --TrackWeightSelection 2 --TriggerChoice 1 --EventSelectionOption 2 --SpeciesCorrectionOption 3 --ScaleFactor 1
    ./RunParallelReadParam.sh --Output systematic_variations/NeNeSansSpeciesWeight  --Input $INFILE --CollisionSystem true --ApplyEventSelection true --UseEventWeight true --UseSpeciesWeight false --UseTrackWeight true --TrackWeightSelection 2 --TriggerChoice 1 --EventSelectionOption 2 --SpeciesCorrectionOption 2 --ScaleFactor 1
fi

cd systematic_variations
root -l -b -q "CompareSystematics.C(\"NeNe\", $DOSYSTEMATIC_TRACK, $DOSYSTEMATIC_EVTSEL, $DOSYSTEMATIC_SPECIES, \"nenesystematics.root\", \"adaptive\")"
cd ..


echo "Systematic variations done! :)" 
