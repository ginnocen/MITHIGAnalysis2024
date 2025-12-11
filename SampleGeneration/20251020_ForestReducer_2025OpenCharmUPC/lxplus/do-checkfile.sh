#!/bin/bash

if [[ $0 != *.sh ]]
then
    echo -e "\e[31;1merror:\e[0m use \e[32;1m./script.sh\e[0m instead of \e[32;1msource script.sh\e[0m"
    return 1
fi

#
MAXFILENO=10000000

# parameters
IsData=False
exefile=Execute_Lcpks
#
movetosubmit=${1:-0}
runjobs=${2:-0}


PRIMARY="Lcpks_251125"
# LABELTAG="_Dpt2"
LABELTAG=""

INPUTS=(
    "/eos/cms/store/group/phys_heavyions/wangj/Forest2023/Pythia8_LcToKsPr_prompt_gN-PhotonA_Pthat2-pt0p9_PbPb_5362GeV/crab_HiForest_251030_LcToKsPr_prompt_gN-PhotonA_Pthat2_v0/251031_005700/0000"
    # "/eos/cms/store/group/phys_heavyions/wangj/Forest2023/HIForward0/crab_HiForest_251110_HIForward0_HIRun2023A_14Feb2025_Dzero0-Lc0p9/251111_023506/000*"
    # "/eos/cms/store/group/phys_heavyions/wangj/Forest2023/HIForward1/crab_HiForest_251110_HIForward1_HIRun2023A_14Feb2025_Dzero0-Lc0p9/251111_024646/000*"
    # "/eos/cms/store/group/phys_heavyions/wangj/Forest2023/HIForward10/crab_HiForest_251110_HIForward10_HIRun2023A_14Feb2025_Dzero0-Lc0p9/251113_030313/000*"
    # "/eos/cms/store/group/phys_heavyions/wangj/Forest2023/HIForward11/crab_HiForest_251110_HIForward11_HIRun2023A_14Feb2025_Dzero0-Lc0p9/251113_030345/000*"
    # "/eos/cms/store/group/phys_heavyions/wangj/Forest2023/HIForward12/crab_HiForest_251110_HIForward12_HIRun2023A_14Feb2025_Dzero0-Lc0p9/251113_030431/000*"
    # "/eos/cms/store/group/phys_heavyions/wangj/Forest2023/HIForward13/crab_HiForest_251110_HIForward13_HIRun2023A_14Feb2025_Dzero0-Lc0p9/251113_030528/000*"
    # "/eos/cms/store/group/phys_heavyions/wangj/Forest2023/HIForward14/crab_HiForest_251110_HIForward14_HIRun2023A_14Feb2025_Dzero0-Lc0p9/251113_030627/000*"
    # "/eos/cms/store/group/phys_heavyions/wangj/Forest2023/HIForward15/crab_HiForest_251110_HIForward15_HIRun2023A_14Feb2025_Dzero0-Lc0p9/251113_030720/000*"
    # "/eos/cms/store/group/phys_heavyions/wangj/Forest2023/HIForward16/crab_HiForest_251110_HIForward16_HIRun2023A_14Feb2025_Dzero0-Lc0p9/251113_030817/000*"
    # "/eos/cms/store/group/phys_heavyions/wangj/Forest2023/HIForward17/crab_HiForest_251110_HIForward17_HIRun2023A_14Feb2025_Dzero0-Lc0p9/251113_030853/000*"
    # "/eos/cms/store/group/phys_heavyions/wangj/Forest2023/HIForward18/crab_HiForest_251110_HIForward18_HIRun2023A_14Feb2025_Dzero0-Lc0p9/251113_030933/000*"
    # "/eos/cms/store/group/phys_heavyions/wangj/Forest2023/HIForward19/crab_HiForest_251110_HIForward19_HIRun2023A_14Feb2025_Dzero0-Lc0p9/251113_031057/000*"
    # "/eos/cms/store/group/phys_heavyions/wangj/Forest2023/HIForward2/crab_HiForest_251110_HIForward2_HIRun2023A_14Feb2025_Dzero0-Lc0p9/251111_024717/000*"
    # "/eos/cms/store/group/phys_heavyions/wangj/Forest2023/HIForward3/crab_HiForest_251110_HIForward3_HIRun2023A_14Feb2025_Dzero0-Lc0p9/251111_024757/000*"
    # "/eos/cms/store/group/phys_heavyions/wangj/Forest2023/HIForward4/crab_HiForest_251110_HIForward4_HIRun2023A_14Feb2025_Dzero0-Lc0p9/251111_025145/000*"
    # "/eos/cms/store/group/phys_heavyions/wangj/Forest2023/HIForward5/crab_HiForest_251110_HIForward5_HIRun2023A_14Feb2025_Dzero0-Lc0p9/251111_025220/000*"
    # "/eos/cms/store/group/phys_heavyions/wangj/Forest2023/HIForward6/crab_HiForest_251110_HIForward6_HIRun2023A_14Feb2025_Dzero0-Lc0p9/251111_025311/000*"
    # "/eos/cms/store/group/phys_heavyions/wangj/Forest2023/HIForward7/crab_HiForest_251110_HIForward7_HIRun2023A_14Feb2025_Dzero0-Lc0p9/251111_025344/000*"
    # "/eos/cms/store/group/phys_heavyions/wangj/Forest2023/HIForward8/crab_HiForest_251110_HIForward8_HIRun2023A_14Feb2025_Dzero0-Lc0p9/251111_025419/000*"
    # "/eos/cms/store/group/phys_heavyions/wangj/Forest2023/HIForward9/crab_HiForest_251110_HIForward9_HIRun2023A_14Feb2025_Dzero0-Lc0p9/251111_025601/000*"
)

OUTPUTPRIDIR="/eos/cms/store/group/phys_heavyions/wangj/Forest2023"
WORKDIR="$PWD"

# CORRFILES=$(ls $CORRPATH/* | grep -E 'OO|ppref|NeNe|NENE') # I assume all the useful correction files have the name including OO, ppref or NeNe

##############################################################
### don't need to change things below if you are just user ###
##############################################################

[[ x$CMSSW_VERSION == x ]] && { echo "error: do cmsenv first." ; exit 1; }

#
mkdir -p $WORKDIR filelists

for INPUTDIR in "${INPUTS[@]}"
do
    echo -e "\e[2mInput files\e[0m \e[32m$INPUTDIR\e[0m"
    
    if [[ $INPUTDIR == *.txt ]]
    then
        INPUTFILELIST=$INPUTDIR 
    else
        CRABNAME=${INPUTDIR##*crab_} ; CRABNAME=${CRABNAME%%/*} ;
        INPUTFILELIST="$WORKDIR/filelists/filelist_"$CRABNAME".txt"
        if [[ $INPUTDIR == /mnt/T2_US_MIT/* ]] ; then
            ls --color=no $INPUTDIR/*.root -d | sed -e "s/\/mnt\/T2_US_MIT\/hadoop\/cms/root:\/\/xrootd.cmsaf.mit.edu\//g" > $INPUTFILELIST
        fi
        if [[ $INPUTDIR == /eos* ]] ; then
            ls --color=no $INPUTDIR/*.root -d | sed -e "s/\/eos/root:\/\/eoscms.cern.ch\/\/eos/g" > $INPUTFILELIST
        fi
    fi
    echo -e "\e[2mInjected files in\e[0m $INPUTFILELIST"
    REQUESTNAME=${INPUTFILELIST##*filelist_} ; REQUESTNAME=${REQUESTNAME%%.txt} ;
    OUTPUTSUBDIR="${PRIMARY}_${REQUESTNAME}${LABELTAG}"

    ##
    OUTPUTDIR="${OUTPUTPRIDIR}/${OUTPUTSUBDIR}"
    LOGDIR="logs/log_${OUTPUTSUBDIR}"

    echo -e "\e[2mOutput to\e[0m $OUTPUTDIR"
    ##

    if [ "$runjobs" -eq 1 ]
    then
        [[ $(ls -lt /tmp/ | grep --color=no "$USER " | grep --color=no -m 1 x509)x == x ]] && voms-proxy-init --voms cms --valid 168:00 ;
        EXISTPROXY=$(ls /tmp/ -lt | grep $USER | grep -m 1 x509 | awk '{print $NF}') ;
        timeleft=$(voms-proxy-info | grep timeleft)
        [[ x$EXISTPROXY == x || "$timeleft" == *"00:00"* ]] && {
            echo "error: bad voms proxy."
            exit
        }
        cp -v /tmp/$EXISTPROXY $HOME/ ;
        
        set -x
        ./tt-condor-checkfile.sh $exefile "$INPUTFILELIST" $OUTPUTDIR $MAXFILENO $LOGDIR $IsData 
        set +x
    fi

done

if [[ "$movetosubmit" -eq 1 ]]
then
    cd ../    
    . clean.sh
    cd lxplus/    
    mv -v ../$exefile $WORKDIR/
    cp -v ../DzeroUPC_dedxMap.root $WORKDIR/

    # cd ../
    # tar -czvf tracklet.tar.gz transmute_trees.C include
    # cd lxplus
    
    # mv ../tracklet.tar.gz .
    
fi
