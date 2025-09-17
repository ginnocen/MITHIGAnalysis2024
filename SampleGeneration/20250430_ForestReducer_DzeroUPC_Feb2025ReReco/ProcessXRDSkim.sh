#!/bin/bash

SERVER=${1}
FILEPATH=${2}
COUNTER=${3}
OUTPUT=${4}
MAXCORES=${5}

mkdir -p "${OUTPUT}/temp_inputs/"
FILE="${OUTPUT}/temp_inputs/job_${COUNTER}.root"
rm $FILE &> /dev/null
xrdcp -N --parallel $MAXCORES -t 2 $SERVER$FILEPATH $FILE
wait

echo "Processing $FILE"
./Execute --Input "$FILE" \
    --Output "${OUTPUT}/output_${COUNTER}.root" \
    --Year 2023 \
    --ApplyTriggerRejection 2 \
    --ApplyEventRejection true \
    --ApplyZDCGapRejection true \
    --ApplyDRejection or \
    --ZDCMinus1nThreshold 1000 \
    --ZDCPlus1nThreshold 1100 \
    --IsData true \
    --HideProgressBar true
wait

sleep 0.2
rm $FILE
wait
