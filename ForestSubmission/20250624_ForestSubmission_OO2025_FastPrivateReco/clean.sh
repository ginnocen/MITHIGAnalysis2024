#!/bin/bash

echo ">>> Setting CMSSW environment"
_150X_LIST=( $( ls -d ../../../CMSSW_15_0_* ) )
_CMSSW_DIR=${_150X_LIST[0]}
_WORKING_DIR=$PWD

if [ ! -e $_CMSSW_DIR ]; then
  echo "[ERROR!] No CMSSW_15_0_X version was found in the same directory as"
  echo "your MITHIGAnalysis repo!"
  return
else
  echo "Setting $( basename $_CMSSW_DIR )"
  cd $_CMSSW_DIR/src
  cmsenv
fi

echo ">>> Returning to working directory"
cd $_WORKING_DIR

echo ">>> Updating ZDC emap"
rm emap_2025_full.txt
wget https://raw.githubusercontent.com/hjbossi/ZDCOnlineMonitoring/refs/heads/main/Conditions/emap/emap_2025_full.txt
cp emap_2025_full.txt $_CMSSW_DIR/src/
