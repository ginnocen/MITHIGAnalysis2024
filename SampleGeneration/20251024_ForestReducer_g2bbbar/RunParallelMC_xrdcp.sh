#!/bin/bash
source clean.sh
SCRIPTDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Get parameters (defaults: N=0, LETTER=A)
N=${1:-0}

NAME="skim_120925_${N}"
PATHSAMPLE="/store/group/phys_heavyions/aholterm/g2qqbar/QCD_pThat-15_Dijet_TuneCP5_5p02TeV-pythia8/crab_btagged_and_svtagged_jets_MC_HFfinders/251128_191749/000${N}"
OUTPUT="/data00/g2ccbar/mc2018/$NAME"

### SKIMMER PARAMETERS ###
ISDATA=false
ISPP=true
DOSVTX=false
PFJETS=ak3PFJetAnalyzer/t
MINJETPT=0
FRACTION=1.0

### OTHER PARAMETERS ###
MAXCORES=40  
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