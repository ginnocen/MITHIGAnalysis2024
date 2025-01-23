#!/bin/bash
MAXCORES=120
SAMPLEID=0

echo "Running on sample ID: $SAMPLEID"

if [ "$SAMPLEID" -eq 0 ]; then
    NAMEMC="20250117_v3_Pthat5_100Million_Inclusive_BeamA"
    FOLDER="/data/UPCD0analysis_2023data_HIN24003/ForestsMC/OfficialMC_pTHat5/UnmergedForests/100Million_Inclusive_v1"
    ISGAMMAN=true
elif [ "$SAMPLEID" -eq 1 ]; then
    NAMEMC="20250117_v3_Pthat0_ForceD0Decay100M_BeamA"
    FOLDER="/data/UPCD0analysis_2023data_HIN24003/ForestsMC/OfficialMC_pTHat0/UnmergedForests/ForcedD0Decay100M_BeamA"
    ISGAMMAN=true
elif [ "$SAMPLEID" -eq 2 ]; then
    NAMEMC="20250117_v3_Pthat0_ForceD0Decay100M_BeamB"
    FOLDER="/data/UPCD0analysis_2023data_HIN24003/ForestsMC/OfficialMC_pTHat0/UnmergedForests/ForcedD0Decay100M_BeamB"
    ISGAMMAN=false
elif [ "$SAMPLEID" -eq 3 ]; then
    NAMEMC="20250117_v3_Pthat2_ForceD0Decay100M_BeamA"
    FOLDER="/data/UPCD0analysis_2023data_HIN24003/ForestsMC/OfficialMC_pTHat2/UnmergedForests/ForcedD0Decay100M_BeamA"
    ISGAMMAN=true
elif [ "$SAMPLEID" -eq 4 ]; then
    NAMEMC="20250117_v3_Pthat2_ForceD0Decay100M_BeamB"
    FOLDER="/data/UPCD0analysis_2023data_HIN24003/ForestsMC/OfficialMC_pTHat2/UnmergedForests/100Million_ForcedD0Decay_BeamB_v1"
    ISGAMMAN=false
fi

echo "Running on sample: $NAMEMC"
echo "Running on folder: $FOLDER"

OUTPUTMC="outputMC"
counterMC=0
filelistMC="/data00/UPCD0LowPtAnalysis_2023ZDCORData_2023reco/SkimsMC/$NAMEMC/filelist.txt"
MERGEDOUTPUTMC="/data00/UPCD0LowPtAnalysis_2023ZDCORData_2023reco/SkimsMC/$NAMEMC"
MERGEDOUTPUTMCFILE="/data00/UPCD0LowPtAnalysis_2023ZDCORData_2023reco/SkimsMC/$NAMEMC/mergedfile.root"
rm -rf $MERGEDOUTPUTMC
mkdir $MERGEDOUTPUTMC
cp ../RunParallelMC.sh $MERGEDOUTPUTMC/.


# Function to monitor active processes
wait_for_slot() {
    while (( $(jobs -r | wc -l) >= MAXCORES )); do
        # Wait a bit before checking again
        sleep 1
    done
}

ls $FOLDER/HiForestMiniAOD_*.root > $filelistMC
echo "File list created successfully: $filelistMC"

# Check if the filelist is empty
if [[ ! -s "$filelistMC" ]]; then
    echo "No matching files found in Samples directory."
    exit 1
fi

rm -rf $OUTPUTMC
mkdir $OUTPUTMC
# Loop through each file in the file list
while IFS= read -r file; do
            echo "Processing $file"
            ./Execute --Input "$file" \
            --Output "$OUTPUTMC/output_$counterMC.root" \
            --Year 2023 \
            --IsData false \
            --IsGammaNMCtype $ISGAMMAN \
            --ApplyTriggerRejection 0 \
            --ApplyEventRejection false \
            --ApplyZDCGapRejection false \
            --ApplyDRejection no \
            --PFTree particleFlowAnalyser/pftree \
            --DGenTree Dfinder/ntGen &
    ((counterMC++))
    wait_for_slot
done < "$filelistMC"
wait 

hadd $MERGEDOUTPUTMCFILE $OUTPUTMC/output_*.root
echo "All done MC!"
echo "Merged output file: $MERGEDOUTPUTMCFILE"
