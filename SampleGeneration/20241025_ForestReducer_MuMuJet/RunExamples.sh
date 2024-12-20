#!/bin/bash

rm -rf Output/
mkdir -p Output/

#################
### Data PbPb ###
#################

./Execute --Input /data00/g2ccbar/mc2018/forest_12172024/HiForestMiniAOD_1-236.root --IsData true --IsPP false --Output SkimReco.root --MinJetPT 80 --Fraction 1.00
