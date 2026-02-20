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

cd ../

set -x

### 2025 ###
# inputname=HiForest_2025-HIForward0_6014.root
# [[ -f $inputname ]] || xrdcp root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2025_PromptReco/HIForward0/crab_PbPbUPC_HIForward0/251227_162520/0006/HiForest_2025PbPbUPC_6014.root $inputname
# ./Execute_Dzero --Input $inputname \
#                     --Output ${inputname/HiForest_/skim_} --Year 2025 --IsData true \
#                     --ApplyTriggerRejection 0 \
#                     --ApplyEventRejection false \
#                     --ApplyZDCGapRejection 0 \
#                     --ApplyDRejection no \
#                     --DptThreshold 0 \
#                     --Fraction 1.0 \
#                     --HideProgressBar false

### 2025 EmptyBX ###
# inputname=HiForest_2025-HIEmptyBX_11.root
# [[ -f $inputname ]] || cp /eos/cms/store/group/phys_heavyions/wangj/Forest2025PbPb/forest/crab_HiForest_260206_HIEmptyBX_HIRun2025A_PromptReco_v1_PF0p1/260206_025751/0000/HiForestMiniAOD_11.root $inputname
# ./Execute_Dzero --Input $inputname \
#                     --Output ${inputname/HiForest_/skim_} --Year 2025 --IsData true \
#                     --ApplyTriggerRejection 0 \
#                     --ApplyEventRejection false \
#                     --ApplyZDCGapRejection 0 \
#                     --ApplyDRejection no \
#                     --DptThreshold 0 \
#                     --Fraction 1.0 \
#                     --HideProgressBar false

### 2023 reForest ###
# inputname=HiForest_2023-HIForward0-260212Forest_1100.root
# [[ -f $inputname ]] || xrdcp root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward0/crab_2023PbPbUPC_Jan2024ReReco_20260212Forest_HIForward0/260212_170406/0001/HiForest_2023PbPbUPC_Jan24Reco_1100.root $inputname
# ./Execute_Dzero --Input $inputname \
#                 --Output ${inputname/HiForest_/skim_} --Year 2023 --IsData true \
#                 --ApplyTriggerRejection 2 \
#                 --ApplyEventRejection false \
#                 --ApplyZDCGapRejection 0 \
#                 --ApplyDRejection no \
#                 --DptThreshold 2 \
#                 --Fraction 1.0 \
#                 --HideProgressBar false

### 2023 ###
# inputname=HiForest_2023-HIForward2-375064-25002_710.root
# [[ -f $inputname ]] || xrdcp root://xrootd.cmsaf.mit.edu//store/user/jdlanfg/public/run3_2023Data_Jan2024ReReco/Run3_2023UPC_375064/HIForward2/crab_Run3_2023UPC_Jan2024ReReco_375064_HIForward2/250212_172509/0000/HiForestMiniAOD_710.root $inputname
# ./Execute_Dzero --Input $inputname \
    # --Output ${inputname/HiForest_/skim_} --Year 2023 --IsData true \
    # --ApplyTriggerRejection 2 \
    # --ApplyEventRejection false \
    # --ApplyZDCGapRejection 0 \
    # --ApplyDRejection pasor \
    # --Fraction 1.0 \
    # --HideProgressBar false

### 2023 EmptyBX ###
inputname=HiForest_2023-HIEmptyBX_101.root
[[ -f $inputname ]] || cp /eos/cms/store/group/phys_heavyions/wangj/Forest2023PbPb/forest/crab_HiForest_260209_HIEmptyBX_HIRun2023A_PromptReco_v2/260210_174311/0000/HiForestMiniAOD_101.root $inputname
./Execute_Dzero --Input $inputname \
                    --Output ${inputname/HiForest_/skim_} --Year 2023 --IsData true \
                    --ApplyTriggerRejection 0 \
                    --ApplyEventRejection false \
                    --ApplyZDCGapRejection 0 \
                    --ApplyDRejection no \
                    --DptThreshold 0 \
                    --Fraction 1.0 \
                    --HideProgressBar false

### 2024 MC ###
# inputname=HiForest_2024-MC_126.root
# [[ -f $inputname ]] || xrdcp root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/wangj/prompt-GNucleusToD0-PhotonBeamA_Bin-Pthat0_Fil-Kpi_UPC_5p36TeV_pythia8-evtgen/crab_HiForest_260120_prompt_GNucleusToD0-PhotonBeamA_Bin-Pthat0_Kpi_Dpt1_PF0p1/260120_232519/0000/HiForestMiniAOD_126.root $inputname
# ./Execute_Dzero --Input $inputname \
    #         --Output ${inputname/HiForest_/skim_} --Year 2025 --IsData false \
    #         --ApplyTriggerRejection 0 \
    #         --ApplyEventRejection false \
    #         --ApplyZDCGapRejection 0 \
    #         --ApplyDRejection no \
    #         --Fraction 1.0 \
    #         --HideProgressBar false

set +x 

cd -

end_time=$(date +%s)
print_duration "$start_time" "$end_time"
