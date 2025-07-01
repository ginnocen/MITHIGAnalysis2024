#!/bin/bash
DATE=$(date +%Y%m%d)

source clean.sh

MAXCORES=20  # too many parallel cores can cause event loss, increase with caution!
NFILES=-1 # number of files to cap the processing at, if -1 processess all files
DOGENLEVEL=0
ISDATA=0
SAMPLETYPE=0 # 0 for HIJING 00, 1 for Starlight SD, 2 for Starlight DD, 4 for HIJING alpha-O, -1 for data
DEBUGMODE=1
INCLUDEPPSANDFSC=1

NAME="${DATE}_Skim_OOMCforJing"
PATHSAMPLE="/eos/cms/store/group/phys_heavyions/wangj/Forest2025/MinBias_Pythia_Angantyr_OO_5362GeV/crab_HiForest_250520_Pythia_Angantyr_OO_OO_5362GeV_250626/250629_005206/0000"

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

    echo ./ProcessSingleFile-OO.sh "$FILEPATH" $COUNTER $OUTPUT $DOGENLEVEL $ISDATA $SAMPLETYPE $DEBUGMODE $INCLUDEPPSANDFSC &
    ./ProcessSingleFile-OO.sh "$FILEPATH" $COUNTER $OUTPUT $DOGENLEVEL $ISDATA $SAMPLETYPE $DEBUGMODE $INCLUDEPPSANDFSC &

    wait_for_slot
    ((COUNTER++))
done
wait

hadd $MERGEDOUTPUT $OUTPUT/output_*.root
echo "All done!"
echo "Merged output file: $MERGEDOUTPUT"
