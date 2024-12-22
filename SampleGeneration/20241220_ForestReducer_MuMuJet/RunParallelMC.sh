#!/bin/bash
MAXCORES=120
SAMPLEID=0

echo "Running on sample ID: $SAMPLEID"

if [ "$SAMPLEID" -eq 0 ]; then
    NAMEMC="SkimMC2018PbPb_Version20241220_InputForest_20241217_DiJet_pThat-15_TuneCP5_HydjetDrumMB_5p02TeV_Pythia8"
    FOLDER="/data00/g2ccbar/mc2018/forest_12172024"
fi
echo "Running on sample: $NAMEMC"
echo "Running on folder: $FOLDER"

OUTPUTMC="outputMC"
counter=0
filelistMC="filelist.txt"
MERGEDOUTPUTMC="$NAMEMC"
MERGEDOUTPUTMCFILE="$NAMEMC/mergedfile.root"

rm -rf $MERGEDOUTPUTMC
mkdir $MERGEDOUTPUTMC
cp ./RunParallelMC.sh $MERGEDOUTPUTMC/.

# Function to monitor active processes
wait_for_slot() {
    while (( $(jobs -r | wc -l) >= MAXCORES )); do
        # Wait a bit before checking again
        sleep 1
    done
}

ls $FOLDER/HiForestMiniAOD_*.root > $filelistMC
echo "File list created successfully: $filelistMC"

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
            --IsData false \
            --IsPP false \
            --Output "$OUTPUTMC/output_$counter.root" \
            --MinJetPT 80 --Fraction 1.0 & 
    ((counter++))
    wait_for_slot
done < "$filelistMC"
wait 

hadd $MERGEDOUTPUTMCFILE $OUTPUTMC/output_*.root
echo "All done MC!"
echo "Merged output file: $MERGEDOUTPUTMCFILE"
