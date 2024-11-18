#!/bin/bash
MAXCORES=120

NAME="20241113_DataPbPb2018gtoccbar_v2_TestV0"
OUTPUT="output"
counter=0
filelist="/data/NewSkims_gtoccbar/InputList/20241023_DataPbPb2018gtoccbar_v2.txt"
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
            --IsData true \
            --IsPP false \
            --Output "$OUTPUT/output_$counter.root" \
            --MinJetPT 60 --Fraction 1.0 & 
    ((counter++))
    wait_for_slot
done < "$filelist"
wait 

hadd $MERGEDOUTPUT $OUTPUT/output_*.root
echo "All done!"
echo "Merged output file: $MERGEDOUTPUT"
