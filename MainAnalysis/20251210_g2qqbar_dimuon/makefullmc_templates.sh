#!/bin/bash

source clean.sh

INPUT_MC0="/data00/g2ccbar/mc2018/skim_011226_0/mergedfile.root"
INPUT_MC1="/data00/g2ccbar/mc2018/skim_011226_1/mergedfile.root"
INPUT_MC2="/data00/g2ccbar/mc2018/skim_011226_2/mergedfile.root"
INPUT_MC3="/data00/g2ccbar/mc2018/skim_011226_3/mergedfile.root"
INPUT_MC4="/data00/g2ccbar/mc2018/skim_011226_4/mergedfile.root"
INPUT_MC5="/data00/g2ccbar/mc2018/skim_011226_5/mergedfile.root"
INPUT_MC6="/data00/g2ccbar/mc2018/skim_011226_6/mergedfile.root"

INPUT_DATA="/data00/g2ccbar/data2018/supermerged.root"

# Run MakeDistros in parallel for all MC files
./MakeDistros \
    --Input $INPUT_MC0 \
    --Input_Efficiency "testefficiencies.root" \
    --Output "mcdistros_0.root" \
    --IsData false \
    --chargeSelection 0 \
    --ptBins 60,80,100,120,160,200,250,300 \
    --muPt 3.5 \
    --makeplots false &

./MakeDistros \
    --Input $INPUT_MC1 \
    --Input_Efficiency "testefficiencies.root" \
    --Output "mcdistros_1.root" \
    --IsData false \
    --chargeSelection 0 \
    --ptBins 60,80,100,120,160,200,250,300 \
    --muPt 3.5 \
    --makeplots false &

./MakeDistros \
    --Input $INPUT_MC2 \
    --Input_Efficiency "testefficiencies.root" \
    --Output "mcdistros_2.root" \
    --IsData false \
    --chargeSelection 0 \
    --ptBins 60,80,100,120,160,200,250,300 \
    --muPt 3.5 \
    --makeplots false &

./MakeDistros \
    --Input $INPUT_MC3 \
    --Input_Efficiency "testefficiencies.root" \
    --Output "mcdistros_3.root" \
    --IsData false \
    --chargeSelection 0 \
    --ptBins 60,80,100,120,160,200,250,300 \
    --muPt 3.5 \
    --makeplots false &

./MakeDistros \
    --Input $INPUT_MC4 \
    --Input_Efficiency "testefficiencies.root" \
    --Output "mcdistros_4.root" \
    --IsData false \
    --chargeSelection 0 \
    --ptBins 60,80,100,120,160,200,250,300 \
    --muPt 3.5 \
    --makeplots false &

./MakeDistros \
    --Input $INPUT_MC5 \
    --Input_Efficiency "testefficiencies.root" \
    --Output "mcdistros_5.root" \
    --IsData false \
    --chargeSelection 0 \
    --ptBins 60,80,100,120,160,200,250,300 \
    --muPt 3.5 \
    --makeplots false &

./MakeDistros \
    --Input $INPUT_MC6 \
    --Input_Efficiency "testefficiencies.root" \
    --Output "mcdistros_6.root" \
    --IsData false \
    --chargeSelection 0 \
    --ptBins 60,80,100,120,160,200,250,300 \
    --muPt 3.5 \
    --makeplots false &

# Wait for all parallel jobs to complete
wait

echo "All MC distributions created. Merging with hadd..."
hadd -f mcdistros.root mcdistros_0.root mcdistros_1.root mcdistros_2.root mcdistros_3.root mcdistros_4.root mcdistros_5.root mcdistros_6.root

echo "Creating data distributions..."
./MakeDistros \
    --Input $INPUT_DATA \
    --Input_Efficiency "testefficiencies.root" \
    --Output "datadistros.root" \
    --IsData true \
    --chargeSelection 0 \
    --ptBins 60,80,100,120,160,200,250,300 \
    --muPt 3.5 \
    --makeplots true \

echo "DONE"
