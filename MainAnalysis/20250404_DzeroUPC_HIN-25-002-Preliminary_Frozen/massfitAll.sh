#!/bin/bash

source clean.sh

# These use full analysis MicroTrees:
./massfit.sh fitSettings/fullAnalysis_useGammaNForNgammaForFitFunc.json
sleep 1
./massfit.sh fitSettings/systFitSigMean_useGammaNForNgammaForFitFunc.json
sleep 1
./massfit.sh fitSettings/systFitSigAlpha_useGammaNForNgammaForFitFunc.json
sleep 1
./massfit.sh fitSettings/systFitComb_useGammaNForNgammaForFitFunc.json
sleep 1
./massfit.sh fitSettings/systFitPkBg_useGammaNForNgammaForFitFunc.json
sleep 1

# These use full their respective MicroTrees:
./massfit.sh fitSettings/systDsvpv_useGammaNForNgammaForFitFunc.json
sleep 1
./massfit.sh fitSettings/systDtrkPt_useGammaNForNgammaForFitFunc.json
sleep 1
./massfit.sh fitSettings/systRapGapLoose_useGammaNForNgammaForFitFunc.json
sleep 1
./massfit.sh fitSettings/systRapGapTight_useGammaNForNgammaForFitFunc.json
sleep 1
./massfit.sh fitSettings/systDalpha_useGammaNForNgammaForFitFunc.json
sleep 1
./massfit.sh fitSettings/systDchi2cl_useGammaNForNgammaForFitFunc.json
sleep 1

# Produce updated cross sections:
./plot.sh plotSettings/fullAnalysis.json
wait
root -x -b -q plotMassfitSignalStudy.cpp
wait
