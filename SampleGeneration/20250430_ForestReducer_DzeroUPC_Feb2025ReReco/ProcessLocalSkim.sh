#!/bin/bash

FILEPATH=${1}
COUNTER=${2}
OUTPUT=${3}

file="$FILEPATH"

echo "Processing $file"
./Execute --Input "$file" \
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

sleep 0.1
wait
