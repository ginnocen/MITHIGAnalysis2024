#!/bin/bash

FILEPATH=${1}
COUNTER=${2}
OUTPUT=${3}
DOGENLEVEL=${4}
ISDATA=${5}
SAMPLETYPE=${6}
DEBUGMODE=${7}


file="$FILEPATH"

./Execute --Input "$file" \
   --Output ${OUTPUT}/output_${COUNTER}.root \
   --DoGenLevel $DOGENLEVEL \
   --Year 2025 \
   --IsData $ISDATA \
   --IsPP false \
   --Fraction 1.0 \
   --ApplyTriggerRejection true \
   --ApplyEventRejection true \
   --ApplyTrackRejection true \
   --PFTree particleFlowAnalyser/pftree \
   --sampleType $SAMPLETYPE \
   --DebugMode $DEBUGMODE \
   #--TrackEfficiencyPath ${ProjectBase}/CommonCode/root/
wait

sleep 0.1
wait