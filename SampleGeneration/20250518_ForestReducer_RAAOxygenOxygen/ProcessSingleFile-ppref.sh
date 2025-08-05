#!/bin/bash

FILEPATH=${1}
COUNTER=${2}
OUTPUT=${3}

file="$FILEPATH"

./Execute --Input "$file" \
   --Output ${OUTPUT}/output_${COUNTER}.root \
   --Year 2024 \
   --CollisionSystem pp \
   --IsData true \
   --Fraction 1.0 \
   --ApplyTriggerRejection true \
   --ApplyEventRejection true \
   --ApplyTrackRejection true \
   --PFTree particleFlowAnalyser/pftree \
   --sampleType -1 \
   --DebugMode true \
   --TrackEfficiencyPath ${ProjectBase}/CommonCode/root/ \
   --EvtSelCorrectionFiles "${ProjectBase}/CommonCode/root/OORAA_MULT_EFFICIENCY_HIJING_HF13AND.root" \
   --Species_ReweightFile "${ProjectBase}/CommonCode/root/ParticleSpeciesCorrectionFactorsOO.root" \
   --HideProgressBar true
wait

sleep 0.1
wait
