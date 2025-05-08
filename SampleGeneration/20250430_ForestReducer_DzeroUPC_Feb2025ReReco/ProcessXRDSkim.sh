#!/bin/bash

SERVER=${1}
FILEPATH=${2}
COUNTER=${3}
OUTPUT=${4}
MAXCORES=${5}

(( $MAXCORES > 30 )) && MAXCORES=30

mkdir -p "${OUTPUT}/temp_inputs/"
file="${OUTPUT}/temp_inputs/job_${COUNTER}.root"
rm $file &> /dev/null
xrdcp -N --parallel $MAXCORES -t 2 $SERVER$FILEPATH $file
wait

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

sleep 0.2
rm $file
wait
