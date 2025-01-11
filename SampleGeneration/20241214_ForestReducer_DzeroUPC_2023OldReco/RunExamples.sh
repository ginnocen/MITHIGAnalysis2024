#!/bin/bash

rm -rf Output/
mkdir -p Output/
###################
### MC Gen PbPb ###
###################

./Execute --Input /afs/cern.ch/work/c/cmcginn/public/forGM/HiForestMINIAOD_pthat5_numEvent150000.root \
   --Output SkimsHiForestMINIAOD_numEvent110000.root \
   --Year 2023 \
   --IsData false \
   --ApplyTriggerRejection 0 \
   --ApplyEventRejection false \
   --ApplyZDCGapRejection false \
   --ApplyDRejection 0 \
   --PFTree particleFlowAnalyser/pftree \
   --DGenTree Dfinder/ntGen
