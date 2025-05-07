#!/bin/bash
DATE=$(date +%Y%m%d)

MAXCORES=40

NAME="${DATE}_ForestDfinderData23LowPtSkim_NEWSkimConfig"
OUTPUT="/data00/jdlang/UPCD0LowPtAnalysis/SkimsData/output_NEW"
counter=0
filelist="filelist_2023Data_Jan2024ReReco_HIForward0.txt"
MERGEDOUTPUT="/data00/jdlang/UPCD0LowPtAnalysis/SkimsData/$NAME.root"
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
            --PFTree particleFlowAnalyser/pftree \
            --HideProgressBar true &
 #           --DGenTree Dfinder/ntGen &
    ((counter++))
    wait_for_slot
done < "$filelist"
wait 

hadd $MERGEDOUTPUT $OUTPUT/output_*.root
echo "All done!"
echo "Merged output file: $MERGEDOUTPUT"
