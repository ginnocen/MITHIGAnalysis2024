#!/bin/bash

mkdir -p Output/


#####################
### MC Reco Level ###
#####################

./Execute --Input Samples/DYLL_HiForestMiniAOD_86.root \
   --Output Output/DYLL_1.root \
   --Year 2018 \
   --TrackEfficiencyPath ${ProjectBase}/CommonCode/root/ \
   --DoGenLevel false \
   --IsData false \
   --IsPP false \
   --IsBackground false \
   --CheckZ true
./Execute --Input Samples/DYLL_HiForestMiniAOD_284.root \
   --Output Output/DYLL_2.root \
   --Year 2018 \
   --TrackEfficiencyPath ${ProjectBase}/CommonCode/root/ \
   --DoGenLevel false \
   --IsData false \
   --IsPP false \
   --IsBackground false \
   --CheckZ true
./Execute --Input Samples/DYLL_HiForestMiniAOD_182.root \
   --Output Output/DYLL_3.root \
   --Year 2018 \
   --TrackEfficiencyPath ${ProjectBase}/CommonCode/root/ \
   --DoGenLevel false \
   --IsData false \
   --IsPP false \
   --IsBackground false \
   --CheckZ true
hadd -f Output/DYLL.root Output/DYLL_*.root

exit

./Execute --Input Samples/Hydjet_HiForestMiniAOD_33.root \
   --Output Output/Hydjet1.root \
   --Year 2018 \
   --TrackEfficiencyPath ${ProjectBase}/CommonCode/root/ \
   --DoGenLevel false \
   --IsData false \
   --IsPP false \
   --IsBackground true \
   --CheckZ false
./Execute --Input Samples/Hydjet_HiForestMiniAOD_35.root \
   --Output Output/Hydjet2.root \
   --Year 2018 \
   --TrackEfficiencyPath ${ProjectBase}/CommonCode/root/ \
   --DoGenLevel false \
   --IsData false \
   --IsPP false \
   --IsBackground true \
   --CheckZ false
hadd -f Output/Hydjet.root Output/Hydjet[12].root

./Execute --Input Samples/ppDYLL_HiForestAOD_76.root \
   --Output Output/ppMC.root \
   --Year 2017 \
   --TrackEfficiencyPath ${ProjectBase}/CommonCode/root/ \
   --DoGenLevel false \
   --IsData false \
   --IsPP true \
   --IsBackground false \
   --CheckZ true


####################
### MC Gen Level ###
####################

./Execute --Input Samples/DYLL_HiForestMiniAOD_86.root \
   --Output Output/DYLLGen.root \
   --Year 2018 \
   --TrackEfficiencyPath ${ProjectBase}/CommonCode/root/ \
   --DoGenLevel true \
   --IsData false \
   --IsPP false \
   --IsBackground false \
   --CheckZ true

./Execute --Input Samples/Hydjet_HiForestMiniAOD_33.root \
   --Output Output/HydjetGen.root \
   --Year 2018 \
   --TrackEfficiencyPath ${ProjectBase}/CommonCode/root/ \
   --DoGenLevel true \
   --IsData false \
   --IsPP false \
   --IsBackground true \
   --CheckZ false

./Execute --Input Samples/ppDYLL_HiForestAOD_76.root \
   --Output Output/ppMCGen.root \
   --Year 2017 \
   --TrackEfficiencyPath ${ProjectBase}/CommonCode/root/ \
   --DoGenLevel true \
   --IsData false \
   --IsPP true \
   --IsBackground false \
   --CheckZ true


############
### Data ###
############

./Execute --Input Samples/SingleMuon_HiForestMiniAOD_94.root \
   --Output Output/SingleMuon.root \
   --Year 2018 \
   --TrackEfficiencyPath ${ProjectBase}/CommonCode/root/ \
   --DoGenLevel false \
   --IsData true \
   --IsPP false \
   --IsBackground false \
   --CheckZ true

./Execute --Input Samples/MB_HiForestMiniAOD_95.root \
   --Output Output/MB.root \
   --Year 2018 \
   --TrackEfficiencyPath ${ProjectBase}/CommonCode/root/ \
   --DoGenLevel false \
   --IsData true \
   --IsPP false \
   --IsBackground true \
   --CheckZ false

./Execute --Input Samples/ppSingleMuon_HiForestAOD_119.root \
   --Output Output/ppData.root \
   --Year 2017 \
   --TrackEfficiencyPath ${ProjectBase}/CommonCode/root/ \
   --DoGenLevel false \
   --IsData true \
   --IsPP true \
   --IsBackground false \
   --CheckZ true


