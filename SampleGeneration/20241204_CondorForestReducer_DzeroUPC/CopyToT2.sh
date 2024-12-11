#!/bin/bash

# Remove existing files on T2
xrdfs root://xrootd5.cmsaf.mit.edu/ ls -R /store/user/$USER/MITHIGAnalysis2024/ >> temp_xrd_delete.txt
while read -r LINE; do
  if [[ "$LINE" =~ \.*$ ]]; then
    xrdfs root://xrootd5.cmsaf.mit.edu/ rm $LINE
  fi
done < temp_xrd_delete.txt
rm temp_xrd_delete.txt
xrdfs root://xrootd5.cmsaf.mit.edu/ rmdir /store/user/$USER/MITHIGAnalysis2024/

# Copy over current state of Condor dir
CONDOR_SKIM_DIR=CondorForestReducer_DzeroUPC
xrdfs root://xrootd5.cmsaf.mit.edu/ mkdir -p /store/user/$USER/MITHIGAnalysis2024/SampleGeneration/$CONDOR_SKIM_DIR
xrdcp -r -f ../../CommonCode root://xrootd5.cmsaf.mit.edu//store/user/$USER/MITHIGAnalysis2024/
xrdcp -f ../../SetupAnalysis.sh root://xrootd5.cmsaf.mit.edu//store/user/$USER/MITHIGAnalysis2024/
xrdcp -r -f include root://xrootd5.cmsaf.mit.edu//store/user/$USER/MITHIGAnalysis2024/SampleGeneration/$CONDOR_SKIM_DIR/
xrdcp -f ReduceForest.cpp root://xrootd5.cmsaf.mit.edu//store/user/$USER/MITHIGAnalysis2024/SampleGeneration/$CONDOR_SKIM_DIR/
xrdcp -f makefile root://xrootd5.cmsaf.mit.edu//store/user/$USER/MITHIGAnalysis2024/SampleGeneration/$CONDOR_SKIM_DIR/
#xrdcp -f RunSkimCondor.sh root://xrootd5.cmsaf.mit.edu//store/user/$USER/MITHIGAnalysis2024/SampleGeneration/$CONDOR_SKIM_DIR/
#xrdcp -f MakeSkimFileList.sh root://xrootd5.cmsaf.mit.edu//store/user/$USER/MITHIGAnalysis2024/SampleGeneration/$CONDOR_SKIM_DIR/
#xrdcp -f MakeSkimCondor.sh root://xrootd5.cmsaf.mit.edu//store/user/$USER/MITHIGAnalysis2024/SampleGeneration/$CONDOR_SKIM_DIR/
