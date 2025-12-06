#!/bin/bash

# Make TGraphs!
root -l -b -q MakeGraphs_RebinRAA.cpp

# Plot Everything!
root -l -b -q MakePlots_RebinRAAVsPt.cpp
root -l -b -q MakePlots_RebinRAAVsA.cpp
root -l -b -q MakePlots_RebinRAAVsN.cpp
root -l -b -q MakePlots_RebinRAAVsA_DataVsTheory.cpp
