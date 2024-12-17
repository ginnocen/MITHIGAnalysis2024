#!/bin/bash
MAXCORES=100

NAMEMC="XYZ_filelist_SkimOldReco23sample_MCPthat2"
OUTPUTMC="outputMC"
counterMC=0
filelistMC="/data/NewSkims23_24/InputLists/20241106_filelist_SkimOldReco23sample_MCPthat2.txt"
MERGEDOUTPUTMC="/data/NewSkims23_24/$NAMEMC.root"
rm $MERGEDOUTPUTMC


# Function to monitor active processes
wait_for_slot() {
    while (( $(jobs -r | wc -l) >= MAXCORES )); do
        # Wait a bit before checking again
        sleep 1
    done
}


# Check if the filelist is empty
if [[ ! -s "$filelistMC" ]]; then
    echo "No matching files found in Samples directory."
    exit 1
fi

echo "File list created successfully: $filelistMC"
rm -rf $OUTPUTMC
mkdir $OUTPUTMC
# Loop through each file in the file list
while IFS= read -r file; do
            echo "Processing $file"
            ./Execute --Input "$file" \
            --Output "$OUTPUTMC/output_$counterMC.root" \
            --Year 2023 \
            --IsData false \
            --ApplyTriggerRejection 2 \
            --ApplyEventRejection false \
            --ApplyZDCGapRejection false \
            --ApplyDRejection 0 \
            --PFTree particleFlowAnalyser/pftree \
            --DGenTree Dfinder/ntGen &
    ((counter++))
    wait_for_slot
done < "$filelist"
wait 

hadd $MERGEDOUTPUTMC $OUTPUTMC/output_*.root
echo "All done MC!"
echo "Merged output file: $MERGEDOUTPUTMC"
