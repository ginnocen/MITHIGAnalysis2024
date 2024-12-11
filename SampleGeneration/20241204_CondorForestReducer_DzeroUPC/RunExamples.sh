#!/bin/bash

rm -rf Output/
mkdir -p Output/

###################
### Data PbPb 23 ###
###################

#./Execute --Input /data/NewSkims23_24/20241102_ForestOldReco23sample_Dataexample/HiForestMiniAOD_UPCPbPb23_HiVertex_279.root \
#   --Output skim_UPCPbPb23_HiVertex_279.root \
#   --Year 2023 \
#   --IsData true \
#   --ApplyTriggerRejection false \
#   --ApplyEventRejection false \
#   --ApplyZDCGapRejection false \
#   --ApplyDRejection false \
#   --ZDCMinus1nThreshold 1000 \
#   --ZDCPlus1nThreshold 1100 \
#   --PFTree particleFlowAnalyser/pftree

#################
### Data PbPb 24 ###
#################

#./Execute   --Input root://xrootd.cmsaf.mit.edu//store/user/jdlang/run3_2024PromptReco/Run3UPC2024_PromptReco_388000_HIForward0/HIForward0/crab_Run3UPC2024_PromptReco_388000_HIForward0/241118_225127/0000/HiForestMiniAOD_1.root \
./Execute --Input HiForestMiniAOD_1.root \
   --Output skim_run388000_test1.root \
   --Year 2024 \
   --IsData true \
   --ZDCTree zdcanalyzer/zdcrechit \
   --ApplyTriggerRejection false \
   --ApplyEventRejection true \
   --ApplyZDCGapRejection true \
   --ApplyDRejection true \
   --ZDCMinus1nThreshold 900 \
   --ZDCPlus1nThreshold 900 \
   --PFTree particleFlowAnalyser/pftree \

###################
### MC Gen PbPb ###
###################

#./Execute --Input /home/data/public/hannah/mc_productions/OfficialMC_pTHat2/UnmergedForests/ForcedD0Decay100M_BeamA/HiForestMiniAOD_44.root \
#   --Output Output/output_44.root \
#   --Year 2023 \
#   --IsData false \
#   --ApplyTriggerRejection false \
#   --ApplyEventRejection false \
#   --ApplyZDCGapRejection false \
#   --ApplyDRejection false \
#   --PFTree particleFlowAnalyser/pftree \
#   --DGenTree Dfinder/ntGen
