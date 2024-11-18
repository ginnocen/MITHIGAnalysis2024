#!/bin/bash

rm -rf Output/
mkdir -p Output/

#################
### Data PbPb ###
#################

./Execute --Input /data/gtoccbar/20241023_DataPbPb2018gtoccbar_v2/241024_002902/0000/HiForestMiniAOD_479.root --IsData true --IsPP false --Output SkimReco.root --MinJetPT 80 --Fraction 1.00
