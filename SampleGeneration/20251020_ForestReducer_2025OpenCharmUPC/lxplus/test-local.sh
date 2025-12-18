#!/bin/bash

print_duration() {
  local start_time=$1
  local end_time=$2
  local elapsed=$(( end_time - start_time ))

  local hours=$(( elapsed / 3600 ))
  local minutes=$(( (elapsed % 3600) / 60 ))
  local seconds=$(( elapsed % 60 ))

  echo -ne "\e[33mExecution took "
  if [[ $hours -gt 0 ]]; then
    echo -ne "${hours} h ${minutes} m"
  elif [[ $minutes -gt 0 ]]; then
    echo -ne "${minutes} min ${seconds} s"
  else
    echo -ne "${seconds} s"
  fi
  echo -e "\e[0m"
}

set -x

cd ../

# 800 events
./Execute_Lcpks --Input /eos/cms/store/group/phys_heavyions/jdlang/Run3_PbPbUPC/Forest_2025_PromptReco/HIForward23/crab_PbPbUPC_HIForward23_399966-399989_QuickAnalysis/251204_184915/0000/HiForest_2025PbPbUPC_100.root \
                --Output skim_HiForestMINIAOD_local-test_Lcpks.root --Year 2025 --IsData true \
                --ApplyTriggerRejection 0 \
                --ApplyEventRejection false \
                --ApplyZDCGapRejection false \
                --ApplyDRejection no \
                --PFTree particleFlowAnalyser/pftree \
                --HideProgressBar false

start_time=$(date +%s)

./Execute_Dzero --Input root://xrootd-se31-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2025_PromptReco/HIForward0/crab_PbPbUPC_HIForward0_400098-400243_QuickAnalysis/251207_153448/0000/HiForest_2025PbPbUPC_100.root \
                --Output skim_HiForestMINIAOD_local-test_Dzero.root --Year 2025 --IsData true \
                --ApplyTriggerRejection 0 \
                --ApplyEventRejection false \
                --ApplyZDCGapRejection false \
                --ApplyDRejection no \
                --PFTree particleFlowAnalyser/pftree \
                --Fraction 0.0257 \
                --HideProgressBar false

cd -

set +x 

end_time=$(date +%s)
print_duration "$start_time" "$end_time"
