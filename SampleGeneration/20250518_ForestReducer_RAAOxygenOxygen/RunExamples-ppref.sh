#!/bin/bash
source clean.sh

INPUT=/afs/cern.ch/work/g/ginnocen/public/OOsamples/Forests
OUTPUT=Output

./Execute --Input $INPUT/HiForestMiniAOD_ppchargedhadron2024_debugfile.root \
   --Output $OUTPUT/Skim_HiForestMiniAOD_ppchargedhadron2024_debugfile.root \
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
