#!/bin/bash

T2PATH=$1

echo ">>> hostname"
hostname

echo ">>> OS info"
uname -a

echo ">>> VOMS info"
unset  X509_USER_KEY
voms-proxy-info --identity -p -vo

echo ">>> Setting up ROOT through LCG"
source /cvmfs/sft.cern.ch/lcg/releases/LCG_106/ROOT/6.32.02/x86_64-el9-gcc13-opt/ROOT-env.sh
export PATH=$PATH:$ROOTSYS/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ROOTSYS/lib

echo ">>> Running cpptest.cc"
root -x -q cpptest.cc
wait
ls cpptest_output.root

echo ">>> Transferring output to T2_MIT"
xrdcp --verbose cpptest_output.root root://xrootd.cmsaf.mit.edu/$T2PATH/cpptest_output.root
wait
xrdfs root://xrootd5.cmsaf.mit.edu/ ls $T2PATH/

echo ">>> Done!"
