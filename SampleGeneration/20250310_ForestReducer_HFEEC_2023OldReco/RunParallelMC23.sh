#!/bin/bash
MAXCORES=40

NAME="20250602_ForestHFJetMC23_D0Tagged_v2_pthat0_BeamB_SmallSample"
OUTPUT="output"
counter=0
filelist="/data/HFJetUPCanalysis/InputLists/2023MCpromptReco_pthat0_BeamB.txt"
MERGEDOUTPUT="/data/HFJetUPCanalysis/MCSkims23/$NAME.root"
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
            --ApplyTriggerRejection 0 \
            --ApplyEventRejection true \
            --ApplyZDCGapRejection true \
            --ZDCMinus1nThreshold 1000 \
            --ZDCPlus1nThreshold 1100 \
            --MinJetPt 0.0 \
            --MaxJetEta 2.4 \
            --IsData false \
            --PFTree particleFlowAnalyser/pftree \
            --Fraction 0.1 &
    ((counter++))
    wait_for_slot
done < "$filelist"
wait

hadd $MERGEDOUTPUT $OUTPUT/output_*.root
echo "All done!"
echo "Merged output file: $MERGEDOUTPUT"