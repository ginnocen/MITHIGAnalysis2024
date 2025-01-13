#!/bin/bash

SAMPLEGEN_DIRS=(
  '20241214_ForestReducer_DzeroUPC_2023OldReco'
  '20241204_ForestReducer_DzeroUPC_2024PromptReco'
  '20241220_ForestReducer_MuMuJet'
)

# Remove existing files on T2
xrdfs root://xrootd.cmsaf.mit.edu/ ls -R /store/user/$USER/MITHIGAnalysis2024/ >> temp_xrd_delete.txt
while read -r LINE; do
  if [[ "${LINE##*/}" =~ \.*$ ]]; then
    xrdfs root://xrootd.cmsaf.mit.edu/ rm $LINE
  fi
done < temp_xrd_delete.txt
rm temp_xrd_delete.txt
xrdfs root://xrootd.cmsaf.mit.edu/ rmdir /store/user/$USER/MITHIGAnalysis2024/

xrdfs root://xrootd.cmsaf.mit.edu/ mkdir -p /store/user/$USER/MITHIGAnalysis2024
xrdcp -r -f ../../CommonCode    root://xrootd.cmsaf.mit.edu//store/user/$USER/MITHIGAnalysis2024/
xrdcp -f ../../SetupAnalysis.sh root://xrootd.cmsaf.mit.edu//store/user/$USER/MITHIGAnalysis2024/

for SAMPLEGEN_DIR in ${SAMPLEGEN_DIRS[@]}; do
  # Copy over current state of Condor dir
  xrdfs root://xrootd.cmsaf.mit.edu/ mkdir -p /store/user/$USER/MITHIGAnalysis2024/SampleGeneration/$SAMPLEGEN_DIR
  xrdcp -r -f $ProjectBase/SampleGeneration/$SAMPLEGEN_DIR/include       	root://xrootd.cmsaf.mit.edu//store/user/$USER/MITHIGAnalysis2024/SampleGeneration/$SAMPLEGEN_DIR/
  xrdcp -f $ProjectBase/SampleGeneration/$SAMPLEGEN_DIR/ReduceForest.cpp 	root://xrootd.cmsaf.mit.edu//store/user/$USER/MITHIGAnalysis2024/SampleGeneration/$SAMPLEGEN_DIR/
  xrdcp -f $ProjectBase/SampleGeneration/$SAMPLEGEN_DIR/makefile         	root://xrootd.cmsaf.mit.edu//store/user/$USER/MITHIGAnalysis2024/SampleGeneration/$SAMPLEGEN_DIR/
done
