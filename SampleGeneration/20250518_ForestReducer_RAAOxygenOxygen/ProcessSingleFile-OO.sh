#!/bin/bash

FILEPATH=${1}
COUNTER=${2}
OUTPUT=${3}
DOGENLEVEL=${4}
ISDATA=${5}
SAMPLETYPE=${6}
SAVETRIGGERBITS=${7}
DEBUGMODE=${8}
INCLUDEPPSANDFSC=${9}
INCLUDEPF=${10}

file="$FILEPATH"

CORRPATH=${ProjectBase}/CommonCode/root/

./Execute --Input "$file" \
   --Output ${OUTPUT}/output_${COUNTER}.root \
   --DoGenLevel $DOGENLEVEL \
   --IsData $ISDATA \
   --Fraction 1.0 \
   --ApplyTriggerRejection true \
   --ApplyEventRejection false \
   --ApplyTrackRejection true \
   --sampleType $SAMPLETYPE \
   --DebugMode $DEBUGMODE \
   --includeFSCandPPSMode $INCLUDEPPSANDFSC \
   --saveTriggerBitsMode $SAVETRIGGERBITS \
   --TrackEfficiencyPath $CORRPATH \
   --HideProgressBar false \

wait

sleep 0.1
wait
