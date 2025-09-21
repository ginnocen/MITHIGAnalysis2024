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

./Execute --Input "$FILEPATH" \
   --Output ${OUTPUT}/output_${COUNTER}.root \
   --IsData $ISDATA \
   --CollisionSystem OO \
   --Fraction 1.0 \
   --ApplyTriggerRejection $APPLYTRIGGERREJECTION \
   --ApplyEventRejection $APPLYEVENTREJECTION \
   --ApplyTrackRejection $APPLYTRACKREJECTION \
   --rejectTracksBelowPt $REJECTTRACKSBELOWPT \
   --PFTree particleFlowAnalyser/pftree \
   --sampleType $SAMPLETYPE \
   --DebugMode false \
   --includeL1EMU false \
   --CorrectionPath ${ProjectBase}/CommonCode/root/ \
   --HideProgressBar true
wait

sleep 0.2
wait
