#!/bin/bash
DATE=$(date +%Y%m%d)

source clean.sh

MAXCORES=40
NFILES=50 # files for ppref go 1-999
# set =1 for agreed upon debug file for Vipul comparison, set =999 for full production

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
for COUNTER in $(seq 1 $NFILES); do
    FILEPATH="${PATHSAMPLE}/HiForestMiniAOD_${COUNTER}.root"

    echo ./ProcessLocalSkim-ppref.sh $FILEPATH $COUNTER $OUTPUT &
    ./ProcessLocalSkim-ppref.sh $FILEPATH $COUNTER $OUTPUT &

    wait_for_slot
done
wait

hadd $MERGEDOUTPUT $OUTPUT/output_*.root
echo "All done!"
echo "Merged output file: $MERGEDOUTPUT"
