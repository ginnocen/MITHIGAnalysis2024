#!/bin/bash
MAXCORES=120
SAMPLEID=4

echo "Running on sample ID: $SAMPLEID"

if [ "$SAMPLEID" -eq 0 ]; then
    NAMEMC="20241216_v1_filelist20241216_Pthat5_100Million_Inclusive_BeamAv1"
    FOLDER="/data/UPCD0analysis_2023data_HIN24003/ForestsMC/OfficialMC_pTHat5/UnmergedForests/100Million_Inclusive_v1"
elif [ "$SAMPLEID" -eq 1 ]; then
    NAMEMC="20241216_v1_filelist20241216_Pthat0_ForceD0Decay100M_BeamA_v1"
    FOLDER="/data/UPCD0analysis_2023data_HIN24003/ForestsMC/OfficialMC_pTHat0/UnmergedForests/ForcedD0Decay100M_BeamA"
elif [ "$SAMPLEID" -eq 2 ]; then
    NAMEMC="20241216_v1_filelist20241216_Pthat0_ForceD0Decay100M_BeamB_v1"
    FOLDER="/data/UPCD0analysis_2023data_HIN24003/ForestsMC/OfficialMC_pTHat0/UnmergedForests/ForcedD0Decay100M_BeamB"
elif [ "$SAMPLEID" -eq 3 ]; then
    NAMEMC="20241216_v1_filelist20241216_Pthat2_ForceD0Decay100M_BeamA_v1"
    FOLDER="/data/UPCD0analysis_2023data_HIN24003/ForestsMC/OfficialMC_pTHat2/UnmergedForests/ForcedD0Decay100M_BeamA"
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

ls $FOLDER/HiForestMiniAOD_*2*.root > $filelistMC
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
            --ApplyTriggerRejection 0 \
            --ApplyEventRejection false \
            --ApplyZDCGapRejection false \
            --ApplyDRejection 2 \
            --PFTree particleFlowAnalyser/pftree \
            --DGenTree Dfinder/ntGen &
    ((counterMC++))
    wait_for_slot
done < "$filelistMC"
wait 

hadd $MERGEDOUTPUTMCFILE $OUTPUTMC/output_*.root
echo "All done MC!"
echo "Merged output file: $MERGEDOUTPUTMCFILE"
