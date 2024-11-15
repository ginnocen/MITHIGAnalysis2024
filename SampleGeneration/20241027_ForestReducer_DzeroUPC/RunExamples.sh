#!/bin/bash

rm -rf Output/
mkdir -p Output/

#################
### Data PbPb ###
#################

./Execute --Input /data/NewSkims23_24/20241102_ForestOldReco23sample_Dataexample/HiForestMiniAOD_UPCPbPb23_HiVertex_279.root \
   --Output Output/SkimmedHiForestMiniAOD_UPCPbPb23_HiVertex_279.root \
   --Year 2023 \
   --IsData true \
   --ApplyDPreselection false \
   --PFTree particleFlowAnalyser/pftree \

###################
### MC Gen PbPb ###
###################

./Execute --Input /home/data/public/hannah/mc_productions/OfficialMC_pTHat2/UnmergedForests/ForcedD0Decay100M_BeamA/HiForestMiniAOD_44.root \
   --Output Output/output_44.root \
   --Year 2023 \
   --IsData false \
   --ApplyDPreselection false \
   --PFTree particleFlowAnalyser/pftree \
   --DGenTree Dfinder/ntGen
