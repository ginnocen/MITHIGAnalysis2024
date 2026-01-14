#!/bin/bash

source clean.sh

INPUT="/data00/g2ccbar/mc2018/skim_011226_0/mergedfile.root"
#"/data00/g2ccbar/mc2018/skim_010526_soft_0/mergedfile.root"
#INPUT="/data00/g2ccbar/mc2018/skim_120925_1/mergedfile.root"

./ExecuteEfficiency \
    --Input $INPUT \
    --Output "testefficiencies.root" \
    --IsData false \
    --ptBins 60,80,100,120,160,200,250,300 \
    --muPt 3.5 \
    --chargeSelection 0 \
    --makeplots true \

echo "DONE WITH EFFICIENCIES"

./MakeDistros \
    --Input $INPUT \
    --Input_Efficiency "testefficiencies.root" \
    --Output "testdistros.root" \
    --IsData false \
    --chargeSelection 0 \
    --ptBins 60,80,100,120,160,200,250,300 \
    --muPt 3.5 \
    --makeplots true \

echo "DONE WITH DISTRIBUTIONS"

./ExecuteYield \
    --Input "testdistros.root" \
    --Templates "testdistros.root" \
    --Output "testyields.root" \
    --ptBins 60,80,100,120,160,200,250,300 \
    --doLF_DCA true \
    --doLF_invMass true \
    --makeplots true \

echo "DONE WITH YIELD FITTING"
