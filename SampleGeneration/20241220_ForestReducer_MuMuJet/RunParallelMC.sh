#!/bin/bash
MAXCORES=120

NAME="SkimMC2018PbPb_Version20241201_InputForest_20241201_DiJet_pThat-15_TuneCP5_HydjetDrumMB_5p02TeV_Pythia8_PARTIAL"
OUTPUT="output"
counter=0
filelist="/data/NewSkims_gtoccbar/InputList/20241201_DiJet_pThat-15_TuneCP5_HydjetDrumMB_5p02TeV_Pythia8_PARTIAL.txt"
MERGEDOUTPUT="/data/NewSkims_gtoccbar/$NAME.root"
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
            --IsData false \
            --IsPP false \
            --Output "$OUTPUT/output_$counter.root" \
            --MinJetPT 40 --Fraction 1.0 & 
    ((counter++))
    wait_for_slot
done < "$filelist"
wait 

hadd $MERGEDOUTPUT $OUTPUT/output_*.root
echo "All done!"
echo "Merged output file: $MERGEDOUTPUT"
