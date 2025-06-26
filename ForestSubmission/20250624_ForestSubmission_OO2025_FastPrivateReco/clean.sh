#!/bin/bash

echo ">>> Setting CMSSW environment"
_150X_LIST=( $( ls -d ../../../CMSSW_15_0_* ) )
_WORKING_DIR=$PWD

if [ ! -e ${_150X_LIST[0]} ]; then
  echo "[ERROR!] No CMSSW_15_0_X version was found in the same directory as"
  echo "your MITHIGAnalysis repo!"
  return
else
  echo "Setting $( basename ${_150X_LIST[0]} )"
  cd ${_150X_LIST[0]}/src
  cmsenv
fi

echo ">>> Returning to working directory"
cd $_WORKING_DIR
