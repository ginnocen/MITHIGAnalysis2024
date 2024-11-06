#!/bin/bash

OUTPUT="output"
counter=0
MAXCORES=100
filelist="/data/NewSkims23_24/20241102_SkimOldReco23sample_DataAllPDs/filelist_short.txt"
MERGEDOUTPUT="MergedSkim_20241102_SkimOldReco23sample_DataAllPDs.root"
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

OUTPUTMC="output"
counterMC=0
MAXCORESMC=100
filelistMC="/data/NewSkims23_24/20241102_SkimOldReco23sample_MCPthat2/filelist.txt"
MERGEDOUTPUTMC="MergedSkim_20241102_SkimOldReco23sample_MCPthat2.root"
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
            ./Execute --Input "$fileMC" \
            --Output "$OUTPUTMC/output_$counterMC.root" \
            --Year 2023 \
            --IsData false \
            --ApplyDPreselection true \
            --PFTree particleFlowAnalyser/pftree &
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

