#!/bin/bash

rm -rf Output/
mkdir -p Output/

./Execute --Input /data00/OOsamples/crab_HiForest_250513_Hijing_ppreco_1500pre2_250507.root \
   --Output skim_HiForest_250513_Hijing_ppreco_1500pre2_250507.root \
   --Year 2024 \
   --IsData false \
   --ZDCTree zdcanalyzer/zdcrechit \
   --ApplyTriggerRejection 0 \
   --ApplyEventRejection true \
   --PFTree particleFlowAnalyser/pftree \

