#!/bin/bash
#source clean.sh
SCRIPTDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

NAME="skim_120925_0"
PATHSAMPLE="/store/group/phys_heavyions/aholterm/g2qqbar/HighEGJet/crab_btagged_and_svtagged_jets_DATA_HFfindersA/251202_223354/0000"
OUTPUT="/data00/g2ccbar/data2018/$NAME"

### SKIMMER PARAMETERS ###
ISDATA=true
ISPP=true
DOSVTX=false
PFJETS=ak3PFJetAnalyzer/t
MINJETPT=0
FRACTION=1.0

### OTHER PARAMETERS ###
MAXCORES=20  
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


# Loop through each file in the file list
COUNTER=0
for FILEPATH in $(xrdfs $XRDSERV ls $PATHSAMPLE | grep 'HiForest'); do

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
    ) &

    wait_for_slot
    ((COUNTER++))
done
wait

hadd -f $OUTPUT/mergedfile.root $OUTPUT/output_*.root
rm -rf "${OUTPUT}/temp_inputs/"
echo "Processing COMPLETE"