#!/bin/bash
MAXCORES=40

NAME="20250117_ForestDfinderData23Skim_v3"
OUTPUT="output"
counter=0
filelist="/data00/UPCD0LowPtAnalysis_2023ZDCORData_2023reco/InputListForests/20241106_filelist_SkimOldReco23sample_DataAll.txt"
MERGEDOUTPUT="/data00/UPCD0LowPtAnalysis_2023ZDCORData_2023reco/SkimsData/$NAME.root"
#MERGEDOUTPUT="$NAME.root"
rm $MERGEDOUTPUT

# Function to monitor active processes
wait_for_slot() {
    while (( $(jobs -r | wc -l) >= MAXCORES )); do
        # Wait a bit before checking again
        sleep 1
    done
}


# Check if the filelist is empty
if [[ ! -s "$filelist" ]]; then
    echo "No matching files found in Samples directory."
    exit 1
fi

echo "File list created successfully: $filelist"
rm -rf $OUTPUT
mkdir $OUTPUT
# Loop through each file in the file list
while IFS= read -r file; do
            echo "Processing $file"
            ./Execute --Input "$file" \
            --Output "$OUTPUT/output_$counter.root" \
            --Year 2023 \
            --ApplyTriggerRejection 2 \
            --ApplyEventRejection true \
            --ApplyZDCGapRejection true \
            --ApplyDRejection or \
            --ZDCMinus1nThreshold 1000 \
            --ZDCPlus1nThreshold 1100 \
            --IsData true \
            --PFTree particleFlowAnalyser/pftree &
 #           --DGenTree Dfinder/ntGen &
    ((counter++))
    wait_for_slot
done < "$filelist"
wait 

hadd $MERGEDOUTPUT $OUTPUT/output_*.root
echo "All done!"
echo "Merged output file: $MERGEDOUTPUT"
