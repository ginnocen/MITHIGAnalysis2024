#!/bin/bash

FILEPATH=${1}
COUNTER=${2}
OUTPUT=${3}
ISDATA=${4}
APPLYTRIGGERREJECTION=${5}
APPLYEVENTREJECTION=${6}
APPLYTRACKREJECTION=${7}
REJECTTRACKSBELOWPT=${8}
SAMPLETYPE=${9}
SERVER=${10}
MAXCORES=${11}

mkdir -p "${OUTPUT}/temp_inputs/"
FILE="${OUTPUT}/temp_inputs/job_${COUNTER}.root"
rm $FILE &> /dev/null
xrdcp -N --parallel $MAXCORES -t 2 $SERVER$FILEPATH $FILE
wait

./Execute --Input "$FILE" \
   --Output ${OUTPUT}/output_${COUNTER}.root \
   --IsData $ISDATA \
   --CollisionSystem OO \
   --Fraction 1.0 \
   --ApplyTriggerRejection $APPLYTRIGGERREJECTION \
   --ApplyEventRejection $APPLYEVENTREJECTION \
   --ApplyTrackRejection $APPLYTRACKREJECTION \
   --rejectTracksBelowPt $REJECTTRACKSBELOWPT \
   --sampleType $SAMPLETYPE \
   --DebugMode false \
   --includeL1EMU false \
   --CorrectionPath ${ProjectBase}/CommonCode/root/ \
   --HideProgressBar true
wait

sleep 0.2
rm $FILE
wait
