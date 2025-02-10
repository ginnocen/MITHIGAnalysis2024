#!/bin/bash

# Move the emap to FileInPath search path
mv ZDCemap_PbPbUPC2023.txt CMSSW_13_2_13/src

# Run the code
cmsRun -j FrameworkJobReport.xml -p PSet.py
