#!/bin/bash
MAXCORES=40
SAMPLEID=0
source clean.sh

echo "Running on sample ID: $SAMPLEID"

if [ "$SAMPLEID" -eq 0 ]; then
    NAMEData="/data00/g2ccbar/data2018/skim_09232025_DiJet_pThat-15_TuneCP5_HydjetDrumMB_5p02TeV_Pythia8_0/"
    FOLDER="/data00/g2ccbar/data2018/minBiasForest_02022025/mb0" 
fi
echo "Running on sample: $NAMEData"
echo "Running on folder: $FOLDER"

OUTPUTData="outputData"
counter=0
filelistData="filelist.txt"
MERGEDOUTPUTData="$NAMEData"
MERGEDOUTPUTDataFILE="$NAMEData/mergedfile.root"

rm -rf $MERGEDOUTPUTData
mkdir $MERGEDOUTPUTData
cp ./RunParallelData.sh $MERGEDOUTPUTData/.

# Function to monitor active processes
wait_for_slot() {
    while (( $(jobs -r | wc -l) >= MAXCORES )); do
        # Wait a bit before checking again
        sleep 1
    done
}

ls $FOLDER/HiForestMiniAOD_*.root > $filelistData
echo "File list created successfully: $filelistData"

# Check if the filelist is empty
if [[ ! -s "$filelistData" ]]; then
    echo "No matching files found in Samples directory."
    exit 1
fi

echo "File list created successfully: $filelistData"
rm -rf $OUTPUTData
mkdir $OUTPUTData
# Loop through each file in the file list
while IFS= read -r file; do
            echo "Processing $file"
            ./Execute --Input "$file" \
            --IsData true \
            --IsPP false \
            --svtx false \
            --Output "$OUTPUTData/output_$counter.root" \
            --MinJetPT 0 --Fraction 1.0 & 
    ((counter++))
    wait_for_slot
done < "$filelistData"
wait 

hadd $MERGEDOUTPUTDataFILE $OUTPUTData/output_*.root
echo "All done Data!"
echo "Merged output file: $MERGEDOUTPUTMCFILE"
