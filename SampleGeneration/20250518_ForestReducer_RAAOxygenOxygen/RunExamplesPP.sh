#!/bin/bash
source clean.sh

rm -rf Output/
mkdir -p Output/
./Execute --Input ${PATHSAMPLE}/crab_Forest_20250523_ppref2024output_PPRefZeroBiasPlusForward0.root \
   --Output ${PATHSKIMSAMPLE}/crab_Forest_20250523_ppref2024output_PPRefZeroBiasPlusForward0.root \
   --DoGenLevel false \
   --Year 2017 \ # we are using for the moment 2017 pp track correction, even if the sample is 2024
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
