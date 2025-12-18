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

start_time=$(date +%s)

set -x

cd ../

./Execute --Input root://eoscms.cern.ch//eos/cms/store/group/phys_heavyions/aholterm/g2qqbar/HighEGJet/crab_btagged_and_svtagged_jets_DATA_HFfindersA/251202_223354/0000/HiForestMiniAOD_1.root \
            --Output skim_HiForestMINIAOD_local-test.root \
            --IsData true \
            --IsPP true \
            --svtx true \
            --PFJetCollection ak3PFJetAnalyzer/t \
            --MinJetPT 0 \
            --Fraction 1.0

cd -

set +x 

end_time=$(date +%s)
print_duration "$start_time" "$end_time"
