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
   --Output ${OUTPUT}/output_${COUNTER}.root \
   --DoGenLevel false \
   --Year 2024 \
   --IsData true \
   --IsPP true \
   --Fraction 1.0 \
   --ApplyTriggerRejection true \
   --ApplyEventRejection false \
   --ApplyTrackRejection true \
   --PFTree particleFlowAnalyser/pftree \
   --sampleType -1 \
   --DebugMode true \
   --TrackEfficiencyPath ${ProjectBase}/CommonCode/root/ \
   --MakeEventWeight false \
   --EvtSelCorrectionPath ${ProjectBase}/CommonCode/root/EventSelEffFile-ppref.root \
   --HideProgressBar false
wait

sleep 0.2
rm $FILE
wait
