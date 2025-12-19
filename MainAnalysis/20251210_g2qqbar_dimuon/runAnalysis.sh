#!/bin/bash

source clean.sh

INPUT="/data00/g2ccbar/mc2018/skim_121925_soft_0/mergedfile.root"
#INPUT="/data00/g2ccbar/mc2018/skim_120925_1/mergedfile.root"

./ExecuteEfficiency \
    --Input $INPUT \
    --Output "testefficiencies_soft2.root" \
    --IsData false \
    --ptBins 60,80,100,120,160,200,250,300 \
    --muPt 3.5 \

echo "DONE WITH EFFICIENCIES"

./MakeDistros \
    --Input $INPUT \
    --Input_Efficiency "testefficiencies_soft2.root" \
    --Output "testdistros_soft2.root" \
    --IsData false \
    --chargeSelection 1 \
    --ptBins 60,80,100,120,160,200,250,300 \
    --muPt 3.5 \

echo "DONE WITH DISTRIBUTIONS"
