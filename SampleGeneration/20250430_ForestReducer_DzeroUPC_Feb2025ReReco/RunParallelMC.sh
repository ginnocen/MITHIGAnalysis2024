#!/bin/bash
DATE=$(date +%Y%m%d)

MAXCORES=40
INPUT_ON_XRD=0
XRDSERV="root://xrootd.cmsaf.mit.edu/"

TAG="Skim_2023MC_ForceD0Decay_Pthat0_BeamA"
NAME="${DATE}_${TAG}"
FILELIST="filelist_2023MC_ForceD0Decay_Pthat0_BeamA.txt"
OUTPUT="/data00/jdlang/UPCD0LowPtAnalysis/SkimsMC/output_$NAME"
MERGEDOUTPUT="/data00/jdlang/UPCD0LowPtAnalysis/SkimsMC/$NAME.root"
rm $MERGEDOUTPUT &> /dev/null

# Function to monitor active processes
wait_for_slot() {
    while (( $(jobs -r | wc -l) >= MAXCORES )); do
        # Wait a bit before checking again
        sleep 1
    done
}

# Check if the filelist is empty
if [[ ! -s "$FILELIST" ]]; then
    echo "No matching files found in Samples directory."
    exit 1
fi

echo "File list: $FILELIST"
rm -rf $OUTPUT &> /dev/null
mkdir -p $OUTPUT
COUNTER=1
while IFS= read -r FILEPATH; do
    if (( $INPUT_ON_XRD == 1 )); then
        ./ProcessXRDSkim.sh $XRDSERV $FILEPATH $COUNTER $OUTPUT $MAXCORES false &
    else
        ./ProcessLocalSkim.sh $FILEPATH $COUNTER $OUTPUT false &
    fi
    ((COUNTER++))
    wait_for_slot
done < "$FILELIST"
wait 

hadd $MERGEDOUTPUT $OUTPUT/output_*.root
echo "All done MC!"
echo "Merged output file: $MERGEDOUTPUT"
