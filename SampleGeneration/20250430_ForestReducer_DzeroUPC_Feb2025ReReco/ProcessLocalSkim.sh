#!/bin/bash

FILEPATH=${1}
COUNTER=${2}
OUTPUT=${3}
ISDATA=${4}

mkdir -p "${OUTPUT}/temp_inputs/"
FILE="${OUTPUT}/temp_inputs/job_${COUNTER}.root"
rm $FILE &> /dev/null
cp $FILEPATH $FILE
wait

echo "Processing $FILE"
./Execute --Input "$FILE" \
    --Output "${OUTPUT}/output_${COUNTER}.root" \
    --Year 2023 \
    --IsData $ISDATA \
    --ApplyTriggerRejection 2 \
    --ApplyEventRejection true \
    --ApplyZDCGapRejection true \
    --ApplyDRejection no \
    --ZDCMinus1nThreshold 1000 \
    --ZDCPlus1nThreshold 1100 \
    --HideProgressBar true
wait

sleep 0.2
rm $FILE
wait
