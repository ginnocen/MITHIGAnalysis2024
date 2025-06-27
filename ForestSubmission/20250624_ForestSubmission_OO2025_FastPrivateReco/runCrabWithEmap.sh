#!/bin/bash

# Move the emap to FileInPath search path
mv emap_2025_full.txt CMSSW_15_0_9/src

# Run the code
cmsRun -j FrameworkJobReport.xml -p PSet.py
