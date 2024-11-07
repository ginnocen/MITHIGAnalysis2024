#!/bin/bash
MAXCORES=100

NAME="20241106_SkimOldReco23sample_DataAll"
OUTPUT="output"
counter=0
filelist="/data/NewSkims23_24/InputLists/20241106_filelist_SkimOldReco23sample_DataAll.txt"
MERGEDOUTPUT="/data/NewSkims23_24/$NAME.root"
rm $MERGEDOUTPUT

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
            --ApplyDPreselection true \
            --IsData true \
            --PFTree particleFlowAnalyser/pftree &
 #           --DGenTree Dfinder/ntGen &
    ((counter++))
    if (( counter % $MAXCORES == 0 )); then
        wait
    fi
done < "$filelist"
wait 

hadd $MERGEDOUTPUT $OUTPUT/output_*.root
echo "All done!"
echo "Merged output file: $MERGEDOUTPUT"
