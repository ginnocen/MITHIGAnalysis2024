#!/bin/bash

source clean.sh

INPUT="/data00/g2ccbar/mc2018/skim_010526_soft_0/mergedfile.root"
#INPUT="/data00/g2ccbar/mc2018/skim_120925_1/mergedfile.root"

./ExecuteEfficiency \
    --Input $INPUT \
    --Output "testefficiencies_soft.root" \
    --IsData false \
    --ptBins 60,80,100,120,160,200,250,300 \
    --muPt 3.5 \

echo "DONE WITH EFFICIENCIES"

./MakeDistros \
    --Input $INPUT \
    --Input_Efficiency "testefficiencies_soft.root" \
    --Output "testdistros_soft.root" \
    --IsData false \
    --chargeSelection 0 \
    --ptBins 60,80,100,120,160,200,250,300 \
    --muPt 3.5 \
    --makeplots true \

echo "DONE WITH DISTRIBUTIONS"

./ExecuteYield \
    --Input "testdistros_soft.root" \
    --Templates "testdistros_soft.root" \
    --Output "testyields_soft.root" \
    --ptBins 60,80,100,120,160,200,250,300 \
    --doJpsi false \
    --doLF true \
    --makeplots true \

echo "DONE WITH YIELD FITTING"
