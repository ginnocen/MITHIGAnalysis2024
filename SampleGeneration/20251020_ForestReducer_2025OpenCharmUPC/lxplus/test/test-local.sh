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
inputname=HiForest_2025-HIForward0-6014.root
[[ -f $inputname ]] || xrdcp root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2025_PromptReco/HIForward0/crab_PbPbUPC_HIForward0/251227_162520/0006/HiForest_2025PbPbUPC_6014.root $inputname
./Execute_Dzero --Input $inputname \
                    --Output ${inputname/HiForest_/skim_} --Year 2025 --IsData true \
                    --ApplyTriggerRejection 0 \
                    --ApplyEventRejection false \
                    --ApplyZDCGapRejection 0 \
                    --ApplyDRejection no \
                    --DptThreshold 0 \
                    --Fraction 1.0 \
                    --HideProgressBar false

### 2023 reForest ###
# [[ -f HiForest_2023-HIForward0-Jan24Reco-20260212Forest_1100.root ]] || xrdcp root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward0/crab_2023PbPbUPC_Jan2024ReReco_20260212Forest_HIForward0/260212_170406/0001/HiForest_2023PbPbUPC_Jan24Reco_1100.root HiForest_2023-HIForward0-Jan24Reco-20260212Forest_1100.root
# ./Execute_Dzero --Input HiForest_2023-HIForward0-Jan24Reco-20260212Forest_1100.root \
#                 --Output skim_2023-HIForward0-Jan24Reco-20260212Forest_1100_Dzero_DRejection-pasor_TriggerRejection-4.root --Year 2023 --IsData true \
#                 --ApplyTriggerRejection 4 \
#                 --ApplyEventRejection false \
#                 --ApplyZDCGapRejection 0 \
#                 --ApplyDRejection pasor \
#                 --Fraction 1.0 \
#                 --HideProgressBar false

# [[ -f HiForest_2023-HIForward0-Jan24Reco-260201Forest_1222.root ]] || xrdcp root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward0/crab_2023PbPbUPC_Jan2024ReReco_20260201Forest_HIForward0/260201_192238/0001/HiForest_2023PbPbUPC_Jan24Reco_1222.root HiForest_2023-HIForward0-Jan24Reco-260201Forest_1222.root
# ./Execute_Dzero --Input HiForest_2023-HIForward0-Jan24Reco-260201Forest_1222.root \
#                 --Output skim_2023-HIForward0-Jan24Reco-260201Forest_1222_Dzero_DRejection-pasor_TriggerRejection-4.root --Year 2023 --IsData true \
#                 --ApplyTriggerRejection 4 \
#                 --ApplyEventRejection false \
#                 --ApplyZDCGapRejection 0 \
#                 --ApplyDRejection pasor \
#                 --Fraction 1.0 \
#                 --HideProgressBar false

### 2023 ###
# [[ -f HiForest_2023-HIForward2-Jan24Reco-375064-710.root ]] || xrdcp root://xrootd.cmsaf.mit.edu//store/user/jdlanfg/public/run3_2023Data_Jan2024ReReco/Run3_2023UPC_375064/HIForward2/crab_Run3_2023UPC_Jan2024ReReco_375064_HIForward2/250212_172509/0000/HiForestMiniAOD_710.root HiForest_2023-HIForward2-Jan24Reco-375064-710.root
# ./Execute_Dzero --Input HiForest_2023-HIForward2-Jan24Reco-375064-710.root \
    # --Output skim_2023-HIForward2-Jan24Reco-375064-710_DRejection-pasor_TriggerRejection-2.root --Year 2023 --IsData true \
    # --ApplyTriggerRejection 2 \
    # --ApplyEventRejection false \
    # --ApplyZDCGapRejection 0 \
    # --ApplyDRejection pasor \
    # --Fraction 1.0 \
    # --HideProgressBar false

### 2024 MC ###
# [[ -f HiForest_2024-MC-126.root ]] || xrdcp root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/wangj/prompt-GNucleusToD0-PhotonBeamA_Bin-Pthat0_Fil-Kpi_UPC_5p36TeV_pythia8-evtgen/crab_HiForest_260120_prompt_GNucleusToD0-PhotonBeamA_Bin-Pthat0_Kpi_Dpt1_PF0p1/260120_232519/0000/HiForestMiniAOD_126.root HiForest_2024-MC-126.root
# ./Execute_Dzero --Input HiForest_2024-MC-126.root \
    #         --Output skim_2024-MC.root --Year 2025 --IsData false \
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
