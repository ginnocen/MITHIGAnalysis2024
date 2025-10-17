#!/bin/bash
DATE=$(date +%Y%m%d)

MAXCORES=40
INPUT_ON_XRD=0
XRDSERV="root://xrootd.cmsaf.mit.edu/"

TAG="Skim_2023Data_Feb2025ReReco_HIForward11_HFFix"
NAME="${DATE}_${TAG}"
FILELIST="filelist_2023Data_Feb2025ReReco_HIForward11_HFFix.txt"
OUTPUT="/data00/jdlang/UPCD0LowPtAnalysis/SkimsData/output_$NAME"
MERGEDOUTPUT="/data00/jdlang/UPCD0LowPtAnalysis/SkimsData/$NAME.root"
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
        ./ProcessXRDSkim.sh $XRDSERV $FILEPATH $COUNTER $OUTPUT $MAXCORES true &
    else
        ./ProcessLocalSkim.sh $FILEPATH $COUNTER $OUTPUT true &
    fi
    ((COUNTER++))
    wait_for_slot
done < "$FILELIST"
wait 

hadd $MERGEDOUTPUT $OUTPUT/output_*.root
echo "All done!"
echo "Merged output file: $MERGEDOUTPUT"
