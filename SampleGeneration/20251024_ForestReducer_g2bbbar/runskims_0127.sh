#!/bin/bash

doMC=false
doHighEG=false
doLowEG=false

if [ $doMC = true ]; then
    echo "Starting MC processing"

    # DO THE MC
    nohup ./RunParallelMC_xrdcp.sh /eos/cms/store/group/phys_heavyions/aholterm/g2qqbar/QCD_pThat-15_Dijet_TuneCP5_5p02TeV-pythia8 /data00/g2ccbar/mc2018/skim_0127 > logs/process_mc.log 2>&1 &
    wait
    echo "done with MC"

else
    echo "Skipping MC processing"
fi

if [ $doHighEG = true ]; then
    echo "Starting HIGH EG DATA processing"

    # DO THE HIGH EG DATA

    nohup ./RunParallelData_xrdcp.sh /eos/cms/store/group/phys_heavyions/aholterm/g2qqbar/HighEGJet/crab_btagged_and_svtagged_jets_DATA_HFfindersA /data00/g2ccbar/data2018/highEG_A/skim_0127 > logs/process_data_highEG.log 2>&1 &
    wait 
    nohup ./RunParallelData_xrdcp.sh /eos/cms/store/group/phys_heavyions/aholterm/g2qqbar/HighEGJet/crab_btagged_and_svtagged_jets_DATA_HFfindersB /data00/g2ccbar/data2018/highEG_B/skim_0127 > logs/process_data_highEGB.log 2>&1 &
    wait
    nohup ./RunParallelData_xrdcp.sh /eos/cms/store/group/phys_heavyions/aholterm/g2qqbar/HighEGJet/crab_btagged_and_svtagged_jets_DATA_HFfindersC /data00/g2ccbar/data2018/highEG_C/skim_0127 > logs/process_data_highEGC.log 2>&1 &
    wait
    echo "done with HIGH EG DATA"

else
    echo "Skipping HIGH EG DATA processing"
fi

if [ $doLowEG = true ]; then
    echo "Starting LOW EG DATA processing"

    # DO THE LOW EG DATA

    nohup ./RunParallelData_xrdcp.sh /eos/cms/store/group/phys_heavyions/aholterm/g2qqbar/LowEGJet/crab_g2bbbars_DATA_HFfindersA /data00/g2ccbar/data2018/lowEG_A/skim_0127 > logs/process_data_lowEGA.log 2>&1 &
    wait 
    nohup ./RunParallelData_xrdcp.sh /eos/cms/store/group/phys_heavyions/aholterm/g2qqbar/LowEGJet/crab_g2bbbars_DATA_HFfindersB /data00/g2ccbar/data2018/lowEG_B/skim_0127 > logs/process_data_lowEGB.log 2>&1 &
    wait
    nohup ./RunParallelData_xrdcp.sh /eos/cms/store/group/phys_heavyions/aholterm/g2qqbar/LowEGJet/crab_g2bbbars_DATA_HFfindersC /data00/g2ccbar/data2018/lowEG_C/skim_0127 > logs/process_data_lowEGC.log 2>&1 &
    wait
    echo "done with LOW EG DATA"

else
    echo "Skipping LOW EG DATA processing"
fi

