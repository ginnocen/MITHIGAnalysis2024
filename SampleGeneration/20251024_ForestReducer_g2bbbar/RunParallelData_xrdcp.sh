#!/bin/bash
source clean.sh
SCRIPTDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Get parameters
PATHSAMPLE="$1"
OUTPUT="$2"

if [ -z "$PATHSAMPLE" ] || [ -z "$OUTPUT" ]; then
    echo "Usage: $0 <input_path> <output_path>"
    echo "Example: $0 /store/group/... /data00/output/skim"
    exit 1
fi

### SKIMMER PARAMETERS ###
ISDATA=true
ISPP=true
DOSVTX=false
PFJETS=ak3PFJetAnalyzer/t
MINJETPT=0
FRACTION=1.0

### OTHER PARAMETERS ###
MAXCORES=50  
NFILES=-1
XRDSERV="root://eoscms.cern.ch/" # eos xrootd server, path should start /store/group...

wait_for_slot() {
    while (( $(jobs -r | wc -l) >= MAXCORES )); do
        # Wait a bit before checking again
        sleep 1
    done
}

rm -rf $OUTPUT &> /dev/null
mkdir -p $OUTPUT
mkdir -p "${OUTPUT}/temp_inputs/"


# Loop through each file in the file list (recursively search all subdirectories)
COUNTER=0
for FILEPATH in $(xrdfs $XRDSERV ls -R $PATHSAMPLE | grep 'HiForest'); do

    if [ $NFILES -gt 0 ] && [ $COUNTER -ge $NFILES ]; then
        break
    fi

    LOCALFILE="${OUTPUT}/temp_inputs/job_${COUNTER}.root"
    rm $LOCALFILE &> /dev/null

    echo "Starting job $COUNTER ($(jobs -r | wc -l) jobs currently running)"
    
    (
        xrdcp -N ${XRDSERV}${FILEPATH} $LOCALFILE
        ${SCRIPTDIR}/Execute --Input "$LOCALFILE" \
        --IsData $ISDATA \
        --IsPP $ISPP \
        --svtx $DOSVTX \
        --Output "$OUTPUT/output_$COUNTER.root" \
        --PFJetCollection $PFJETS \
        --MinJetPT $MINJETPT \
        --Fraction $FRACTION
        rm $LOCALFILE
        echo "FINISHED job $COUNTER: $FILEPATH"
    ) &

    wait_for_slot
    ((COUNTER++))
done
wait

hadd -f $OUTPUT/mergedfile.root $OUTPUT/output_*.root
rm -f $OUTPUT/output_*.root
rm -rf "${OUTPUT}/temp_inputs/"
echo "Processing COMPLETE"