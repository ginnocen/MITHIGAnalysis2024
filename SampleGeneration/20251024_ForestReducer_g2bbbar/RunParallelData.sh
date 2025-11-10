#!/bin/bash
MAXCORES=40
SAMPLEID=0
source clean.sh

echo "Running on sample ID: $SAMPLEID"

if [ "$SAMPLEID" -eq 0 ]; then
    NAMEData="/data00/g2ccbar/data2018/skim_110525_0"
    FOLDER="/eos/cms/store/group/phys_heavyions/aholterm/g2qqbar/HighEGJet/crab_btagged_and_svtagged_jets_DATA_test/251022_062442/0009" 
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
            --IsPP true \
            --svtx true \
            --Output "$OUTPUTData/output_$counter.root" \
            --PFJetCollection ak3PFJetAnalyzer/t \
            --MinJetPT 0 --Fraction 1.0 & 
    ((counter++))
    wait_for_slot
done < "$filelistData"
wait 

hadd $MERGEDOUTPUTDataFILE $OUTPUTData/output_*.root
echo "All done Data!"
echo "Merged output file: $MERGEDOUTPUTMCFILE"
