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

[[ -f HiForest_2025PbPbUPC_6014.root ]] || xrdcp root://xrootd-se31-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2025_PromptReco/HIForward0/crab_PbPbUPC_HIForward0/251227_162520/0006/HiForest_2025PbPbUPC_6014.root .
[[ -f HiForestMiniAOD_126.root ]] || xrdcp root://xrootd-se31-vanderbilt.sites.opensciencegrid.org//store/user/wangj/prompt-GNucleusToD0-PhotonBeamA_Bin-Pthat0_Fil-Kpi_UPC_5p36TeV_pythia8-evtgen/crab_HiForest_250112_GNucleusToD0-PhotonBeamA_Bin-Pthat0_Kpi_Dpt1/260113_042410/0000/HiForestMiniAOD_126.root .

set -x
./Execute_Dzero --Input HiForest_2025PbPbUPC_6014.root \
                --Output skim_HiForestMINIAOD_local_Dzero.root --Year 2025 --IsData true \
                --ApplyTriggerRejection 0 \
                --ApplyEventRejection false \
                --ApplyZDCGapRejection false \
                --ApplyDRejection no \
                --PFTree particleFlowAnalyser/pftree \
                --Fraction 1.0 \
                --HideProgressBar false

./Execute_Dzero --Input HiForest_2025PbPbUPC_6014.root \
                --Output skim_HiForestMINIAOD_local_Dzero_ApplyDRejection-pasor.root --Year 2025 --IsData true \
                --ApplyTriggerRejection 0 \
                --ApplyEventRejection false \
                --ApplyZDCGapRejection false \
                --ApplyDRejection pasor \
                --PFTree particleFlowAnalyser/pftree \
                --Fraction 1.0 \
                --HideProgressBar false

./Execute_Dzero --Input HiForest_2025PbPbUPC_6014.root \
                --Output skim_HiForestMINIAOD_local_Dzero_ApplyDRejection-pasor_DptThreshold-1.root --Year 2025 --IsData true \
                --ApplyTriggerRejection 0 \
                --ApplyEventRejection false \
                --ApplyZDCGapRejection false \
                --ApplyDRejection pasor \
                --PFTree particleFlowAnalyser/pftree \
                --Fraction 1.0 \
                --DptThreshold 1 \
                --HideProgressBar false

# ./Execute_Dzero --Input HiForestMiniAOD_126.root \
#                 --Output skim_HiForestMINIAOD_localmc_Dzero.root --Year 2025 --IsData false \
#                 --ApplyTriggerRejection 0 \
#                 --ApplyEventRejection false \
#                 --ApplyZDCGapRejection false \
#                 --ApplyDRejection no \
#                 --PFTree particleFlowAnalyser/pftree \
#                 --Fraction 1.0 \
#                 --HideProgressBar false

set +x 

cd -

end_time=$(date +%s)
print_duration "$start_time" "$end_time"
