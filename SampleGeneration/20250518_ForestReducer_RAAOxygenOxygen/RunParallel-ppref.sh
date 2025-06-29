#!/bin/bash
DATE=$(date +%Y%m%d)

source clean.sh

MAXCORES=40
NFILES=1 # number of files to cap the processing at, if -1 processess all files
# set =1 for agreed upon debug file for Vipul comparison (we just used his first file in the directory HiForestMiniAOD_1.root), set =999 for full production

NAME="${DATE}_Skim_ppref2024"
PATHSAMPLE="/eos/cms/store/group/phys_heavyions/vpant/ppref2024output/PPRefZeroBiasPlusForward4/crab_ppref2024/250324_080237/0000"
# set your output directory here
OUTPUT="/data00/kdeverea/OOsamples/Skims/output_$NAME"
MERGEDOUTPUT="/data00/kdeverea/OOsamples/Skims/$NAME.root"
rm $MERGEDOUTPUT &> /dev/null

# Function to monitor active processes
wait_for_slot() {
    while (( $(jobs -r | wc -l) >= MAXCORES )); do
        # Wait a bit before checking again
        sleep 1
    done
}

echo "Forest sample path: $PATHSAMPLE"
rm -rf $OUTPUT &> /dev/null
mkdir -p $OUTPUT

# Loop through each file in the file list
COUNTER=0
for FILEPATH in "$PATHSAMPLE"/HiForestMiniAOD*; do

    if [ $NFILES -gt 0 ] && [ $COUNTER -ge $NFILES ]; then
        break
    fi

    echo ./ProcessSinglepprefFile.sh $FILEPATH $COUNTER $OUTPUT &
    ./ProcessSinglepprefFile.sh $FILEPATH $COUNTER $OUTPUT &

    wait_for_slot
    ((COUNTER++))
done
wait

hadd $MERGEDOUTPUT $OUTPUT/output_*.root
echo "All done!"
echo "Merged output file: $MERGEDOUTPUT"
