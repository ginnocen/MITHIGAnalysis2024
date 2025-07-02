#!/bin/bash
DATE=$(date +%Y%m%d)

source clean.sh

MAXCORES=20  # too many parallel cores can cause event loss, increase with caution!
NFILES=1 # number of files to cap the processing at, if -1 processess all files
DOGENLEVEL=0
ISDATA=1
SAMPLETYPE=-1 # 0 for HIJING 00, 1 for Starlight SD, 2 for Starlight DD, 4 for HIJING alpha-O, -1 for data
SAVETRIGGERBITS=2 # 0 for not HLT saved, 1 for HLT OO, 2 for HLT pO
DEBUGMODE=1
INCLUDEPPSANDFSC=1

NAME="${DATE}_pO_PhysicsIonPhysics0_393952_test1"
PATHSAMPLE="/eos/cms/store/group/phys_heavyions/jdlang/Run3_OO_2025Data_QuickForest/pO_PhysicsIonPhysics0_393952/crab_pO_PhysicsIonPhysics0_393952/250701_063441/0001"

# set your output directory here
OUTPUT="/data00/bakovacs/OOsamples/Skims/output_$NAME"
MERGEDOUTPUT="/data00/bakovacs/OOsamples/Skims/$NAME.root"
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

    echo ./ProcessSingleFile-OO.sh "$FILEPATH" $COUNTER $OUTPUT $DOGENLEVEL $ISDATA $SAMPLETYPE $SAVETRIGGERBITS $DEBUGMODE $INCLUDEPPSANDFSC  &
    ./ProcessSingleFile-OO.sh "$FILEPATH" $COUNTER $OUTPUT $DOGENLEVEL $ISDATA $SAMPLETYPE $SAVETRIGGERBITS $DEBUGMODE $INCLUDEPPSANDFSC &

    wait_for_slot
    ((COUNTER++))
done
wait

hadd $MERGEDOUTPUT $OUTPUT/output_*.root
echo "All done!"
echo "Merged output file: $MERGEDOUTPUT"
