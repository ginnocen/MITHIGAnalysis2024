#!/bin/bash

EXECUTABLE=${1}
FILEPATH=${2}
COUNTER=${3}
OUTPUT=${4}
YEAR=${5}
ISDATA=${6}
TRIG_REJECT=${7:-2}
EVENT_REJECT=${8:-true}
ZDCGAP_REJECT=${9:-true}
D_REJECT=${10:-or}
ZDCM_THRESH=${11:-1000}
ZDCP_THRESH=${12:-1100}

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

sleep 0.1
wait
