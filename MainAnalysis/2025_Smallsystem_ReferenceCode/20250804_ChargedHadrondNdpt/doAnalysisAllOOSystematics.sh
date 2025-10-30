#!/bin/bash

DOCENTRALVALUE=true    
DOSYSTEMATIC_TRACK=true
DOSYSTEMATIC_EVTSEL=true
DOSYSTEMATIC_SPECIES=true

INFILE="OO_InputFileList.txt"

if [ "$DOCENTRALVALUE" = true ]; then
    echo "Running central value..."

    #default
    ./RunParallelReadParam.sh --Output systematic_variations/OOCentralValue --Input $INFILE --CollisionSystem true --ApplyEventSelection true --UseEventWeight true --UseSpeciesWeight true --UseTrackWeight true --TrackWeightSelection 2 --TriggerChoice 1 --EventSelectionOption 2 --SpeciesCorrectionOption 2 --ScaleFactor 1
    ./RunParallelReadParam.sh --Output systematic_variations/OOUncorrected --Input $INFILE --CollisionSystem true --ApplyEventSelection true --UseEventWeight false --UseSpeciesWeight false --UseTrackWeight false --TrackWeightSelection 2 --TriggerChoice 1 --EventSelectionOption 2 --SpeciesCorrectionOption 2 --ScaleFactor 1
fi

if [ "$DOSYSTEMATIC_TRACK" = true ]; then
    echo "Running track weight systematics..."

    #track weight variations
    ./RunParallelReadParam.sh --Output systematic_variations/OOSystLooseTrack   --Input $INFILE --CollisionSystem true --ApplyEventSelection true --UseEventWeight true --UseSpeciesWeight true --UseTrackWeight true --TrackWeightSelection 1 --TriggerChoice 1 --EventSelectionOption 2 --SpeciesCorrectionOption 2 --ScaleFactor 1
    ./RunParallelReadParam.sh --Output systematic_variations/OOSystTightTrack   --Input $INFILE --CollisionSystem true --ApplyEventSelection true --UseEventWeight true --UseSpeciesWeight true --UseTrackWeight true --TrackWeightSelection 3 --TriggerChoice 1 --EventSelectionOption 2 --SpeciesCorrectionOption 2 --ScaleFactor 1
    ./RunParallelReadParam.sh --Output systematic_variations/OOSansTrackWeight  --Input $INFILE --CollisionSystem true --ApplyEventSelection true --UseEventWeight true --UseSpeciesWeight true --UseTrackWeight false --TrackWeightSelection 2 --TriggerChoice 1 --EventSelectionOption 2 --SpeciesCorrectionOption 2 --ScaleFactor 1
fi

if [ "$DOSYSTEMATIC_EVTSEL" = true ]; then
    echo "Running event selection systematics..."

    #eventselection variations
    ./RunParallelReadParam.sh --Output systematic_variations/OOSystLooseEsel   --Input $INFILE --CollisionSystem true --ApplyEventSelection true --UseEventWeight true --UseSpeciesWeight true --UseTrackWeight true --TrackWeightSelection 2 --TriggerChoice 1 --EventSelectionOption 1 --SpeciesCorrectionOption 2 --ScaleFactor 1
    ./RunParallelReadParam.sh --Output systematic_variations/OOSystTightEsel   --Input $INFILE --CollisionSystem true --ApplyEventSelection true --UseEventWeight true --UseSpeciesWeight true --UseTrackWeight true --TrackWeightSelection 2 --TriggerChoice 1 --EventSelectionOption 3 --SpeciesCorrectionOption 2 --ScaleFactor 1
    ./RunParallelReadParam.sh --Output systematic_variations/OOSansEselWeight  --Input $INFILE --CollisionSystem true --ApplyEventSelection true --UseEventWeight false --UseSpeciesWeight true --UseTrackWeight true --TrackWeightSelection 2 --TriggerChoice 1 --EventSelectionOption 2 --SpeciesCorrectionOption 2 --ScaleFactor 1
fi

if [ "$DOSYSTEMATIC_SPECIES" = true ]; then
    echo "Running species correction systematics..."

    #species correction variations
    ./RunParallelReadParam.sh --Output systematic_variations/OOSystLooseSpecies   --Input $INFILE --CollisionSystem true --ApplyEventSelection true --UseEventWeight true --UseSpeciesWeight true --UseTrackWeight true --TrackWeightSelection 2 --TriggerChoice 1 --EventSelectionOption 2 --SpeciesCorrectionOption 1 --ScaleFactor 1
    ./RunParallelReadParam.sh --Output systematic_variations/OOSystTightSpecies   --Input $INFILE --CollisionSystem true --ApplyEventSelection true --UseEventWeight true --UseSpeciesWeight true --UseTrackWeight true --TrackWeightSelection 2 --TriggerChoice 1 --EventSelectionOption 2 --SpeciesCorrectionOption 3 --ScaleFactor 1
    ./RunParallelReadParam.sh --Output systematic_variations/OOSansSpeciesWeight  --Input $INFILE --CollisionSystem true --ApplyEventSelection true --UseEventWeight true --UseSpeciesWeight false --UseTrackWeight true --TrackWeightSelection 2 --TriggerChoice 1 --EventSelectionOption 2 --SpeciesCorrectionOption 2 --ScaleFactor 1
fi

cd systematic_variations
root -l -b -q "CompareSystematics.C(\"OO\", $DOSYSTEMATIC_TRACK, $DOSYSTEMATIC_EVTSEL, $DOSYSTEMATIC_SPECIES, \"oosystematics.root\", \"adaptive\")"
cd ..

echo "Systematic variations done! :)" 