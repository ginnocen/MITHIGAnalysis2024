#!/bin/bash

rm -rf Output/
mkdir -p Output/

###################
### Data PbPb 23 ###
###################

./Execute --Input /data/NewSkims23_24/20241102_ForestOldReco23sample_Dataexample/HiForestMiniAOD_UPCPbPb23_HiVertex_279.root \
   --Output skim_UPCPbPb23_HiVertex_279.root \
   --Year 2023 \
   --IsData true \
   --ApplyTriggerRejection false \
   --ApplyEventRejection false \
   --ApplyZDCGapRejection false \
   --ApplyDRejection false \
   --PFTree particleFlowAnalyser/pftree

#################
### Data PbPb 24 ###
#################

./Execute --Input /data/yjlee/upcCheck/run388171_test2.root \
   --Output skim_run388171_test2.root \
   --Year 2024 \
   --IsData true \
   --ZDCTree zdcanalyzer/zdcrechit \
   --ApplyTriggerRejection false \
   --ApplyEventRejection false \
   --ApplyZDCGapRejection false \
   --ApplyDRejection false \
   --PFTree particleFlowAnalyser/pftree \

###################
### MC Gen PbPb ###
###################

./Execute --Input /home/data/public/hannah/mc_productions/OfficialMC_pTHat2/UnmergedForests/ForcedD0Decay100M_BeamA/HiForestMiniAOD_44.root \
   --Output Output/output_44.root \
   --Year 2023 \
   --IsData false \
   --ApplyTriggerRejection false \
   --ApplyEventRejection false \
   --ApplyZDCGapRejection false \
   --ApplyDRejection false \
   --PFTree particleFlowAnalyser/pftree \
   --DGenTree Dfinder/ntGen
