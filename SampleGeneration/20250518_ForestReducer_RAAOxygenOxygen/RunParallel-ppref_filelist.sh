#!/bin/bash
DATE=$(date +%Y%m%d)

source clean.sh

MAXCORES=30  # too many parallel cores can cause event loss, increase with caution!
NFILES=-1 # number of files to cap the processing at, if -1 processess all files
ISDATA=1
APPLYTRIGGERREJECTION=1 #  trigger = 0 for no rejection, 1 for ZeroBias, 2 for MinBias
APPLYEVENTREJECTION=1
APPLYTRACKREJECTION=1
SAMPLETYPE=-1 # 0 for HIJING 00, 1 for Starlight SD, 2 for Starlight DD, 4 for HIJING alpha-O, -1 for data
XRDSERV="root://eoscms.cern.ch/" # eos xrootd server, path should start /store/group...


# ============================================================
# pp data, low pT PD, Vipul's filelist for crosscheck
# ============================================================
NAME="${DATE}_Skim_ppref2024_data_CROSSCHECK_1to9087"
FILELIST="PPRefZeroBiasPlusForward4.txt"

# set your output directory here
OUTPUT="/data00/$USER/OOsamples/Skims/output_$NAME"



# Function to monitor active processes
wait_for_slot() {
    while (( $(jobs -r | wc -l) >= MAXCORES )); do
        # Wait a bit before checking again
        sleep 1
    done
}

rm -rf $OUTPUT &> /dev/null
mkdir -p $OUTPUT

# Loop through each file in the file list
COUNTER=0
while IFS= read -r FILEPATH; do

    if [ $NFILES -gt 0 ] && [ $COUNTER -ge $NFILES ]; then
        break
    fi

    echo ./ProcessSingleFile-ppref-xrd.sh $FILEPATH $COUNTER $OUTPUT $ISDATA $APPLYTRIGGERREJECTION $APPLYEVENTREJECTION $APPLYTRACKREJECTION $SAMPLETYPE $XRDSERV $MAXCORES &
    ./ProcessSingleFile-ppref-xrd.sh $FILEPATH $COUNTER $OUTPUT $ISDATA $APPLYTRIGGERREJECTION $APPLYEVENTREJECTION $APPLYTRACKREJECTION $SAMPLETYPE $XRDSERV $MAXCORES &

    wait_for_slot
    ((COUNTER++))
done < $FILELIST
wait

echo "Processing COMPLETE"
