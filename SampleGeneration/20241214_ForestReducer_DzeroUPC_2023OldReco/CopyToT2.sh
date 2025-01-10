#!/bin/bash

ANALYSIS_DIR='MITHIGAnalysis2024'
SKIM_DIR='20241214_ForestReducer_DzeroUPC_2023OldReco'

# Remove existing files on T2
xrdfs root://xrootd.cmsaf.mit.edu/ ls -R /store/user/$USER/$ANALYSIS_DIR/ >> temp_xrd_delete.txt
while read -r LINE; do
  if [[ "$LINE" =~ \.*$ ]]; then
    xrdfs root://xrootd.cmsaf.mit.edu/ rm $LINE
  fi
done < temp_xrd_delete.txt
rm temp_xrd_delete.txt
xrdfs root://xrootd.cmsaf.mit.edu/ rmdir /store/user/$USER/$ANALYSIS_DIR/

# Copy over current state of Condor dir
xrdfs root://xrootd.cmsaf.mit.edu/ mkdir -p /store/user/$USER/$ANALYSIS_DIR/SampleGeneration/$SKIM_DIR
xrdcp -r -f ../../CommonCode    root://xrootd.cmsaf.mit.edu//store/user/$USER/$ANALYSIS_DIR/
xrdcp -f ../../SetupAnalysis.sh root://xrootd.cmsaf.mit.edu//store/user/$USER/$ANALYSIS_DIR/
xrdcp -r -f include       	root://xrootd.cmsaf.mit.edu//store/user/$USER/$ANALYSIS_DIR/SampleGeneration/$SKIM_DIR/
xrdcp -f ReduceForest.cpp 	root://xrootd.cmsaf.mit.edu//store/user/$USER/$ANALYSIS_DIR/SampleGeneration/$SKIM_DIR/
xrdcp -f makefile         	root://xrootd.cmsaf.mit.edu//store/user/$USER/$ANALYSIS_DIR/SampleGeneration/$SKIM_DIR/
