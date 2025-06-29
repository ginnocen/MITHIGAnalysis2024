#!/bin/bash

FILEPATH=${1}
COUNTER=${2}
OUTPUT=${3}

file="$FILEPATH"

./Execute --Input "$file" \
   --Output ${OUTPUT}/output_${COUNTER}.root \
   --DoGenLevel false \
   --Year 2024 \
   --IsData true \
   --IsPP true \
   --Fraction 1.0 \
   --ApplyTriggerRejection true \
   --ApplyEventRejection true \
   --ApplyTrackRejection true \
   --PFTree particleFlowAnalyser/pftree \
   --sampleType -1 \
   --DebugMode true \
   --TrackEfficiencyPath ${ProjectBase}/CommonCode/root/
wait

sleep 0.1
wait
