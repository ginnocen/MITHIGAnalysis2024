#!/bin/bash

XROOTD_SERVER="root://xrootd.cmsaf.mit.edu/"
ANALYSIS_DIR="/store/user/$USER/MITHIGAnalysis2024"
ANALYSIS_SUBDIRS=(
  'SampleGeneration/20241204_ForestReducer_DzeroUPC_2024PromptReco'
  'SampleGeneration/20241214_ForestReducer_DzeroUPC_2023OldReco'
  'SampleGeneration/20241220_ForestReducer_MuMuJet'
)

# Overwrite default paths with arguments
if [ $# -eq 1 ]; then
  ANALYSIS_DIR="$1"
elif [ $# -eq 2 ]; then
  ANALYSIS_DIR="$1"
  ANALYSIS_SUBDIRS=( "$2" )
fi

# Remove existing files on T2
xrdfs $XROOTD_SERVER ls -R $ANALYSIS_DIR/ >> temp_xrd_delete.txt
while read -r LINE; do
  if [[ $(basename "$LINE") =~ \.*$ ]]; then
    xrdfs $XROOTD_SERVER rm $LINE
  fi
done < temp_xrd_delete.txt
rm temp_xrd_delete.txt
xrdfs $XROOTD_SERVER rmdir $ANALYSIS_DIR

xrdfs $XROOTD_SERVER mkdir -p $ANALYSIS_DIR
xrdcp -r -f -t 3 ../../CommonCode    $XROOTD_SERVER/$ANALYSIS_DIR/
xrdcp -f -t 3 ../../SetupAnalysis.sh $XROOTD_SERVER/$ANALYSIS_DIR/

for ANALYSIS_SUBDIR in ${ANALYSIS_SUBDIRS[@]}; do
  # Copy over current state of Condor dir
  xrdfs root://xrootd.cmsaf.mit.edu/ mkdir -p $ANALYSIS_DIR/$ANALYSIS_SUBDIR
  xrdcp -r -f -t 3 $ProjectBase/$ANALYSIS_SUBDIR/include $XROOTD_SERVER/$ANALYSIS_DIR/$ANALYSIS_SUBDIR/
  xrdcp -f -t 3 $ProjectBase/$ANALYSIS_SUBDIR/ReduceForest.cpp $XROOTD_SERVER/$ANALYSIS_DIR/$ANALYSIS_SUBDIR/
  xrdcp -f -t 3 $ProjectBase/$ANALYSIS_SUBDIR/makefile $XROOTD_SERVER/$ANALYSIS_DIR/$ANALYSIS_SUBDIR/
done
