#!/bin/bash
source clean.sh

rm -rf Output/
mkdir -p Output/
./Execute --Input ${PATHSAMPLE}/HiForestMiniAOD_ppchargedhadron2024_debugfile.root \
   --Output ${PATHSKIMSAMPLE}/Skim_HiForestMiniAOD_ppchargedhadron2024_debugfile.root \
   --DoGenLevel false \
   --Year 2017 \
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
