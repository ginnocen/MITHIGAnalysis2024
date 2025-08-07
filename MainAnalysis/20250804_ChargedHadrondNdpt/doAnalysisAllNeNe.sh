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


#Systematic variation

DOCENTRALVALUE=true    
DOSYSTEMATIC_TRACK=true
DOSYSTEMATIC_EVTSEL=true
DOSYSTEMATIC_SPECIES=true

if [ "$DOCENTRALVALUE" = true ]; then
    echo "Running central value..."

    #default
    ./RunParallelReadParam.sh --Output systematic_variations/NeNeCentralValue --Input NeNe_InputFileList.txt --CollisionSystem true --ApplyEventSelection true --UseEventWeight true --UseSpeciesWeight true --UseTrackWeight true --TrackWeightSelection 2 --TriggerChoice 1 --EventSelectionOption 2 --SpeciesCorrectionOption 2 --ScaleFactor 1
fi

if [ "$DOSYSTEMATIC_TRACK" = true ]; then
    echo "Running track weight systematics..."

    #track weight variations
    ./RunParallelReadParam.sh --Output systematic_variations/NeNeSystLooseTrack   --Input NeNe_InputFileList.txt --CollisionSystem true --ApplyEventSelection true --UseEventWeight true --UseSpeciesWeight true --UseTrackWeight true --TrackWeightSelection 1 --TriggerChoice 1 --EventSelectionOption 2 --SpeciesCorrectionOption 2 --ScaleFactor 1
    ./RunParallelReadParam.sh --Output systematic_variations/NeNeSystTightTrack   --Input NeNe_InputFileList.txt --CollisionSystem true --ApplyEventSelection true --UseEventWeight true --UseSpeciesWeight true --UseTrackWeight true --TrackWeightSelection 3 --TriggerChoice 1 --EventSelectionOption 2 --SpeciesCorrectionOption 2 --ScaleFactor 1
fi

if [ "$DOSYSTEMATIC_EVTSEL" = true ]; then
    echo "Running event selection systematics..."

    #eventselection variations
    ./RunParallelReadParam.sh --Output systematic_variations/NeNeSystLooseEsel   --Input NeNe_InputFileList.txt --CollisionSystem true --ApplyEventSelection true --UseEventWeight true --UseSpeciesWeight true --UseTrackWeight true --TrackWeightSelection 2 --TriggerChoice 1 --EventSelectionOption 1 --SpeciesCorrectionOption 2 --ScaleFactor 1
    ./RunParallelReadParam.sh --Output systematic_variations/NeNeSystTightEsel   --Input NeNe_InputFileList.txt --CollisionSystem true --ApplyEventSelection true --UseEventWeight true --UseSpeciesWeight true --UseTrackWeight true --TrackWeightSelection 2 --TriggerChoice 1 --EventSelectionOption 3 --SpeciesCorrectionOption 2 --ScaleFactor 1
fi

if [ "$DOSYSTEMATIC_SPECIES" = true ]; then
    echo "Running species correction systematics..."

    #species correction variations
    ./RunParallelReadParam.sh --Output systematic_variations/NeNeSystLooseSpecies   --Input NeNe_InputFileList.txt --CollisionSystem true --ApplyEventSelection true --UseEventWeight true --UseSpeciesWeight true --UseTrackWeight true --TrackWeightSelection 2 --TriggerChoice 1 --EventSelectionOption 2 --SpeciesCorrectionOption 1 --ScaleFactor 1
    ./RunParallelReadParam.sh --Output systematic_variations/NeNeSystTightSpecies   --Input NeNe_InputFileList.txt --CollisionSystem true --ApplyEventSelection true --UseEventWeight true --UseSpeciesWeight true --UseTrackWeight true --TrackWeightSelection 2 --TriggerChoice 1 --EventSelectionOption 2 --SpeciesCorrectionOption 3 --ScaleFactor 1
fi

cd systematic_variations
root -l -b -q "CompareSystematics.C(\"NeNe\", $DOSYSTEMATIC_TRACK, $DOSYSTEMATIC_EVTSEL, $DOSYSTEMATIC_SPECIES, \"nenesystematics.root\")"
cd ..

echo "Systematic variations done! :)"


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

root -l RAA_NeNe.C #final result
