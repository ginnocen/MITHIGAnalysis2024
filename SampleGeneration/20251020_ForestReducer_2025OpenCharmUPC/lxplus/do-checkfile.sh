#!/bin/bash

if [[ $0 != *.sh ]]
then
    echo -e "\e[31;1merror:\e[0m use \e[32;1m./script.sh\e[0m instead of \e[32;1msource script.sh\e[0m"
    return 1
fi

# Max number of files to submit for each input
MAXFILENO=2

# Exe parameters
IsData=False

# 
EXEFILE=Execute_Lcpks
PIDfile=../../../CommonCode/root/DzeroUPC_dedxMap.root # wrt lxplus/.
#
PRIMARY="Lcpks_251218"
LABELTAG="_tutorial" # e.g. versions or selections

###############################################################################
## IMPORTANT:
## Ensure your input path contains `crab_`
## This script automatically generates a label based on the `*/crab_*` pattern.
## If your path does not include `crab_`, the script must be adjusted.
###############################################################################
INPUTS=(
    ## OPTION 1: /eos/cms/..
    ## - The double quotes below are required when asterisks (*) are used.
    "/eos/cms/store/group/phys_heavyions/jdlang/Run3_PbPbUPC/Forest_2025_PromptReco/HIForward23/crab_PbPbUPC_HIForward23_399966-399989_QuickAnalysis/251204_184915/000*"

    ## OPTION 2: root:/redirector//store/...
    ## - Do not use asterisks (*) for xrootd directory path
    root://xrootd-se31-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2025_PromptReco/HIForward0/crab_PbPbUPC_HIForward0_400098-400243_QuickAnalysis/251207_153448/0000
    # root://xrootd-se31-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2025_PromptReco/HIForward0/crab_PbPbUPC_HIForward0_400098-400243_QuickAnalysis/251207_153448/0001

    ## OPTION 3: filelist.txt
    ## - Must be .txt; One file per line ; No asterisks (*)
    filelists/example.txt
)

OUTPUTPRIDIR="/eos/cms/store/group/phys_heavyions/"$USER"/test_condor"

######################################################
### don't change things below if you are just user ###
######################################################

prep_jobs=${1:-0}
submit_jobs=${2:-0}

# Check environment
[[ x$CMSSW_VERSION == x ]] && { echo "error: do cmsenv first." ; exit 1; }

[[ $(ls -lt /tmp/ | grep --color=no "$USER " | grep --color=no -m 1 x509)x == x ]] && voms-proxy-init --voms cms --valid 168:00 ;
EXISTPROXY=$(ls /tmp/ -lt | grep $USER | grep -m 1 x509 | awk '{print $NF}') ;
timeleft=$(voms-proxy-info | grep timeleft)
[[ x$EXISTPROXY == x || "$timeleft" == *"00:00"* ]] && {
    echo "error: bad voms proxy."
    exit 2
}
cp -v /tmp/$EXISTPROXY $HOME/ 

#
mkdir -p filelists

for INPUTDIR in "${INPUTS[@]}"
do
    echo
    echo -e "\e[2mInput files\e[0m \e[32m$INPUTDIR\e[0m"
    
    ## Generate file list ##
    if [[ $INPUTDIR == *.txt ]] ; then
        INPUTFILELIST=$INPUTDIR 
    else
        CRABNAME=${INPUTDIR##*crab_} ; CRABNAME=${CRABNAME%%/*} ;
        INPUTFILELIST="./filelists/filelist_"$CRABNAME".txt"

        if [[ $INPUTDIR == /mnt/T2_US_MIT/* ]] ; then
            ls --color=no $INPUTDIR/*.root -d | sed -e "s|/mnt/T2_US_MIT/hadoop/cms|root://xrootd.cmsaf.mit.edu/|g" > $INPUTFILELIST
        elif [[ $INPUTDIR == /eos* ]] ; then
            ls --color=no $INPUTDIR/*.root -d | sed -e "s|/eos|root://eoscms.cern.ch//eos|g" > $INPUTFILELIST
        elif [[ $INPUTDIR == root:/*/store* ]] ; then
            REDIRECTOR=${INPUTDIR%%/store*}
            SUBPATH=${INPUTDIR/$REDIRECTOR/}
            REDIRECTOR=${REDIRECTOR%/}"/"
            xrdfs $REDIRECTOR ls $SUBPATH | sed -e "s|^|$REDIRECTOR|1" > $INPUTFILELIST
        fi
    fi
    echo -e "\e[2mInjected files in\e[0m $INPUTFILELIST"
    REQUESTNAME=${INPUTFILELIST##*/} ; REQUESTNAME=${REQUESTNAME##*filelist_} ; REQUESTNAME=${REQUESTNAME%%.txt} ;
    OUTPUTSUBDIR="${PRIMARY}_${REQUESTNAME}${LABELTAG}"

    ##
    OUTPUTDIR="${OUTPUTPRIDIR}/${OUTPUTSUBDIR}"
    LOGDIR="logs/log_${OUTPUTSUBDIR}"

    echo -e "\e[2mOutput to\e[0m $OUTPUTDIR"
    ##

    if [ "$submit_jobs" -eq 1 ]
    then
        set -x
        ./tt-condor-checkfile.sh $EXEFILE "$INPUTFILELIST" $OUTPUTDIR $MAXFILENO $LOGDIR $IsData 
        set +x
    fi

done

if [[ "$prep_jobs" -gt 0 ]]
then
    echo
    cp -v ../$EXEFILE .
    cp -v $PIDfile .

    # cd ../
    # tar -czvf tracklet.tar.gz transmute_trees.C include
    # cd lxplus
    # mv ../tracklet.tar.gz .
fi
