#!/bin/bash
MAXCORES=100

NAMEMC="20241106_filelist_SkimOldReco23sample_MCPthat2"
OUTPUTMC="outputMC"
counterMC=0
filelistMC="/data/NewSkims23_24/InputLists/20241106_filelist_SkimOldReco23sample_MCPthat2.txt"
MERGEDOUTPUTMC="/data/NewSkims23_24/$NAMEMC.root"
rm $MERGEDOUTPUTMC

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
            --ApplyDPreselection true \
            --PFTree particleFlowAnalyser/pftree \
            --DGenTree Dfinder/ntGen &
    ((counterMC++))
    if (( counterMC % $MAXCORES == 0 )); then
        wait
    fi
done < "$filelistMC"
wait 

hadd $MERGEDOUTPUTMC $OUTPUTMC/output_*.root
echo "All done MC!"
echo "Merged output file: $MERGEDOUTPUTMC"
