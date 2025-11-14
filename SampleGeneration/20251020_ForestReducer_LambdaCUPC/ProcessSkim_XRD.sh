#!/bin/bash

EXECUTABLE=${1}
SERVER=${2}
SERVERPATH=${3}
COUNTER=${4}
OUTPUT=${5}
YEAR=${6}
ISDATA=${7}
TRIG_REJECT=${8:-2}
EVENT_REJECT=${9:-true}
ZDCGAP_REJECT=${10:-true}
D_REJECT=${11:-or}
ZDCM_THRESH=${12:-1000}
ZDCP_THRESH=${13:-1100}
MAXCORES=${14:-20}

mkdir -p "${OUTPUT}/temp_inputs/"
FILEPATH="${OUTPUT}/temp_inputs/job_${COUNTER}.root"
rm $FILEPATH &> /dev/null
xrdcp -C auto -N --parallel $MAXCORES -t 2 $SERVER$SERVERPATH $FILEPATH
wait

echo "Processing $FILEPATH"
./$EXECUTABLE --Input "$FILEPATH" \
    --Output "${OUTPUT}/output_${COUNTER}.root" \
    --Year $YEAR \
    --IsData $ISDATA \
    --ApplyTriggerRejection $TRIG_REJECT \
    --ApplyEventRejection $EVENT_REJECT \
    --ApplyZDCGapRejection $ZDCGAP_REJECT \
    --ApplyDRejection $D_REJECT \
    --ZDCMinus1nThreshold $ZDCM_THRESH \
    --ZDCPlus1nThreshold $ZDCP_THRESH \
    --HideProgressBar true
wait

sleep 0.2
rm $FILEPATH
wait
