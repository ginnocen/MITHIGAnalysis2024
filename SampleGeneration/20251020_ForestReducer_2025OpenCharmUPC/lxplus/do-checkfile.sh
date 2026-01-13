#!/bin/bash

if [[ $0 != *.sh ]]
then
    echo -e "\e[31;1merror:\e[0m use \e[32;1m./script.sh\e[0m instead of \e[32;1msource script.sh\e[0m"
    return 1
fi

# Max number of files to submit for each input
MAXFILENO=1000000

# Exe parameters
IsData=True
ApplyDRejection=pasor # no pasor 
# 
EXEFILE=Execute_Dzero
PIDfile=../../../CommonCode/root/DzeroUPC_dedxMap.root # wrt lxplus/
#
PRIMARY="Dzero_260113"
LABELTAG="_Drej-pasor" # e.g. versions or selections

###############################################################################
## IMPORTANT:
## Ensure your input path contains `crab_`
## This script automatically generates a label based on the `*/crab_*` pattern.
## If your path does not include `crab_`, the script must be adjusted.
###############################################################################
INPUTS=(
    # root://xrootd-se31-vanderbilt.sites.opensciencegrid.org//store/user/wangj/prompt-GNucleusToD0-PhotonBeamA_Bin-Pthat0_Fil-Kpi_UPC_5p36TeV_pythia8-evtgen/crab_HiForest_250112_GNucleusToD0-PhotonBeamA_Bin-Pthat0_Kpi_Dpt1/260113_042410/0000
    # root://xrootd-se31-vanderbilt.sites.opensciencegrid.org//store/user/wangj/prompt-GNucleusToD0-PhotonBeamB_Bin-Pthat0_Fil-Kpi_UPC_5p36TeV_pythia8-evtgen/crab_HiForest_250112_GNucleusToD0-PhotonBeamB_Bin-Pthat0_Kpi_Dpt1/260113_042623/0000

    # root://xrootd-se31-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2025_PromptReco/HIForward0/crab_PbPbUPC_HIForward0/251227_162520/000[0-6]
    # root://xrootd-se31-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2025_PromptReco/HIForward2/crab_PbPbUPC_HIForward2/251227_171556/000[0-6]
    # root://xrootd-se31-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2025_PromptReco/HIForward4/crab_PbPbUPC_HIForward4/251227_171633/000[0-6]
    # root://xrootd-se31-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2025_PromptReco/HIForward6/crab_PbPbUPC_HIForward6/251227_171846/000[0-6]
    # root://xrootd-se31-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2025_PromptReco/HIForward8/crab_PbPbUPC_HIForward8/251227_171942/000[0-6]
    # root://xrootd-se31-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2025_PromptReco/HIForward10/crab_PbPbUPC_HIForward10/251227_172110/000[0-6]
    # root://xrootd-se31-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2025_PromptReco/HIForward12/crab_PbPbUPC_HIForward12/251228_175230/000[0-6]
    # root://xrootd-se31-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2025_PromptReco/HIForward14/crab_PbPbUPC_HIForward14/251228_175327/000[0-6]
    # root://xrootd-se31-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2025_PromptReco/HIForward16/crab_PbPbUPC_HIForward16/251228_175418/000[0-6]
    # root://xrootd-se31-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2025_PromptReco/HIForward18/crab_PbPbUPC_HIForward18/251228_175501/000[0-6]
    # root://xrootd-se31-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2025_PromptReco/HIForward20/crab_PbPbUPC_HIForward20/251228_175537/000[0-6]
    # root://xrootd-se31-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2025_PromptReco/HIForward22/crab_PbPbUPC_HIForward22/251228_175617/000[0-6]
)

OUTPUTPRIDIR="/eos/cms/store/group/phys_heavyions/"$USER"/Forest2025PbPb"
LOGBASEDIR=$PWD # eos does not work for logs

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
            if [[ "$SUBPATH" =~ \[[0-9]+-[0-9]+\] && ! "$CRABNAME" =~ \[[0-9]+-[0-9]+\] ]]; then
                range="${SUBPATH##*[}" ; range="${range%%]*}"
                low="${range%-*}" ; high="${range#*-}"
                if (( low > high )) ; then rtmp=high ; high=low ; low=rtmp ; fi ;
                rm $INPUTFILELIST
                for ((i = low; i <= high; i++)); do
                    xrdfs $REDIRECTOR ls ${SUBPATH/\[$range\]/$i} | sed -e "s|^|$REDIRECTOR|1" >> $INPUTFILELIST
                done
            else
                xrdfs $REDIRECTOR ls $SUBPATH | sed -e "s|^|$REDIRECTOR|1" > $INPUTFILELIST
            fi
        fi
    fi
    echo -e "\e[2mInjected files in\e[0m $INPUTFILELIST"
    REQUESTNAME=${INPUTFILELIST##*/} ; REQUESTNAME=${REQUESTNAME##*filelist_} ; REQUESTNAME=${REQUESTNAME%%.txt} ;
    OUTPUTSUBDIR="${PRIMARY}_${REQUESTNAME}${LABELTAG}"

    ##
    OUTPUTDIR="${OUTPUTPRIDIR}/${OUTPUTSUBDIR}"
    LOGDIR=$LOGBASEDIR"/logs/log_${OUTPUTSUBDIR}"

    echo -e "\e[2mOutput to\e[0m $OUTPUTDIR"
    ##

    if [ "$submit_jobs" -eq 1 ]
    then
        set -x
        ./tt-condor-checkfile.sh $EXEFILE "$INPUTFILELIST" $OUTPUTDIR $MAXFILENO $LOGDIR $IsData $ApplyDRejection 
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
