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

echo ">>> Starting CopyToT2.sh"

# Remove existing files on T2
echo "--- Removing files from $ANALYSIS_DIR..."
xrdfs $XROOTD_SERVER ls -R $ANALYSIS_DIR/ >> temp_xrd_delete.txt
while read -r LINE; do
  if [[ $(basename "$LINE") == *.* ]]; then
    echo "    Deleting $LINE ..."
    xrdfs $XROOTD_SERVER rm $LINE
  fi
done < temp_xrd_delete.txt
rm temp_xrd_delete.txt
echo "    Deleting $ANALYSIS_DIR ..."
xrdfs $XROOTD_SERVER rmdir $ANALYSIS_DIR

# Copy over current state of Condor dir
echo "--- Remaking $ANALYSIS_DIR..."
xrdfs $XROOTD_SERVER mkdir -p $ANALYSIS_DIR
echo "--- Copying files in MITHIGAnalysis2024/CommonCode..."
xrdcp --parallel 5 --recursive --retry 2 --retry-policy continue ../../CommonCode $XROOTD_SERVER/$ANALYSIS_DIR/
xrdcp --retry 2 --retry-policy continue ../../SetupAnalysis.sh $XROOTD_SERVER/$ANALYSIS_DIR/
for ANALYSIS_SUBDIR in ${ANALYSIS_SUBDIRS[@]}; do
  echo "--- Copying files in MITHIGAnalysis2024/$ANALYSIS_SUBDIR..."
  xrdfs root://xrootd.cmsaf.mit.edu/ mkdir -p $ANALYSIS_DIR/$ANALYSIS_SUBDIR
  xrdcp --parallel 5 --recursive --retry 2 --retry-policy continue --parallel 5 $ProjectBase/$ANALYSIS_SUBDIR/include $XROOTD_SERVER/$ANALYSIS_DIR/$ANALYSIS_SUBDIR/
  xrdcp --retry 2 --retry-policy continue $ProjectBase/$ANALYSIS_SUBDIR/ReduceForest.cpp $XROOTD_SERVER/$ANALYSIS_DIR/$ANALYSIS_SUBDIR/
  xrdcp --retry 2 --retry-policy continue $ProjectBase/$ANALYSIS_SUBDIR/makefile $XROOTD_SERVER/$ANALYSIS_DIR/$ANALYSIS_SUBDIR/
done

echo ">>> Done!"
