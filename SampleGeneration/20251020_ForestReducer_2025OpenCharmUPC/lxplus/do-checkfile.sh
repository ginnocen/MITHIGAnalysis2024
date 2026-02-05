#!/bin/bash

if [[ $0 != *.sh ]] ; then
    echo -e "\e[31;1merror:\e[0m use \e[32;1m./script.sh\e[0m instead of \e[32;1msource script.sh\e[0m"
    return 1
fi

# Max number of files to submit for each input
MAXFILENO=1000000

# Exe parameters
# Year=2025 ; IsData=true ; ApplyDRejection=pasor ; ApplyTriggerRejection=0 ; # Data 2025
Year=2023 ; IsData=true ; ApplyDRejection=pasor ; ApplyTriggerRejection=2 ; # Data 2023
# Year=2024 ; IsData=false ; ApplyDRejection=no ; ApplyTriggerRejection=0 ; # MC 2024
IsGammaNMCtype=false
#
PRIMARY="Dzero_260203"
LABELTAG="" # e.g. versions or selections
#
[[ $ApplyDRejection != "no" ]] && LABELTAG+="_Drej-"$ApplyDRejection # e.g. versions or selections
[[ $ApplyTriggerRejection != 0 ]] && LABELTAG+="_Trig-"$ApplyTriggerRejection # e.g. versions or selections

# 
EXEFILE=Execute_Dzero
PIDfile=../../../CommonCode/root/DzeroUPC_dedxMap.root # wrt lxplus/

###############################################################################
## IMPORTANT:
## Ensure your input path contains `crab_`
## This script automatically generates a label based on the `*/crab_*` pattern.
## If your path does not include `crab_`, the script must be adjusted.
###############################################################################
INPUTS=(
    # ------ gammaN -> IsGammaNMCtype=true
    # root://xrootd-se31-vanderbilt.sites.opensciencegrid.org//store/user/wangj/prompt-GNucleusToD0-PhotonBeamA_Bin-Pthat0_Fil-Kpi_UPC_5p36TeV_pythia8-evtgen/crab_HiForest_260120_prompt_GNucleusToD0-PhotonBeamA_Bin-Pthat0_Kpi_Dpt1_PF0p1/260120_232519/0000
    # root://xrootd-se31-vanderbilt.sites.opensciencegrid.org//store/user/wangj/nonprompt-GNucleusToD0-PhotonBeamA_Bin-Pthat0_Fil-Kpi_UPC_5p36TeV_pythia8-evtgen/crab_HiForest_260120_nonprompt_GNucleusToD0-PhotonBeamA_Bin-Pthat0_Kpi_Dpt1_PF0p1/260121_000439/0000
    # ------ Ngamma -> IsGammaNMCtype=false
    # root://xrootd-se31-vanderbilt.sites.opensciencegrid.org//store/user/wangj/prompt-GNucleusToD0-PhotonBeamB_Bin-Pthat0_Fil-Kpi_UPC_5p36TeV_pythia8-evtgen/crab_HiForest_260120_prompt_GNucleusToD0-PhotonBeamB_Bin-Pthat0_Kpi_Dpt1_PF0p1/260120_233803/0000
    # root://xrootd-se31-vanderbilt.sites.opensciencegrid.org//store/user/wangj/nonprompt-GNucleusToD0-PhotonBeamB_Bin-Pthat0_Fil-Kpi_UPC_5p36TeV_pythia8-evtgen/crab_HiForest_260120_nonprompt_GNucleusToD0-PhotonBeamB_Bin-Pthat0_Kpi_Dpt1_PF0p1/260121_000604/0000

    # ------ Data -> 2025
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

    # ------ Data -> 2023
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward0/crab_2023PbPbUPC_Jan2024ReReco_20260201Forest_HIForward0/260201_192238/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward1/crab_2023PbPbUPC_Jan2024ReReco_20260201Forest_HIForward1/260202_194045/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward2/crab_2023PbPbUPC_Jan2024ReReco_20260201Forest_HIForward2/260201_192851/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward3/crab_2023PbPbUPC_Jan2024ReReco_20260201Forest_HIForward3/260202_194110/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward4/crab_2023PbPbUPC_Jan2024ReReco_20260201Forest_HIForward4/260201_192918/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward5/crab_2023PbPbUPC_Jan2024ReReco_20260201Forest_HIForward5/260202_194146/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward6/crab_2023PbPbUPC_Jan2024ReReco_20260201Forest_HIForward6/260201_192943/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward7/crab_2023PbPbUPC_Jan2024ReReco_20260201Forest_HIForward7/260202_194217/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward8/crab_2023PbPbUPC_Jan2024ReReco_20260201Forest_HIForward8/260201_193011/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward9/crab_2023PbPbUPC_Jan2024ReReco_20260201Forest_HIForward9/260202_194238/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward10/crab_2023PbPbUPC_Jan2024ReReco_20260201Forest_HIForward10/260203_163709/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward11/crab_2023PbPbUPC_Jan2024ReReco_20260201Forest_HIForward11/260203_163918/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward12/crab_2023PbPbUPC_Jan2024ReReco_20260201Forest_HIForward12/260203_164016/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward13/crab_2023PbPbUPC_Jan2024ReReco_20260201Forest_HIForward13/260203_164103/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward14/crab_2023PbPbUPC_Jan2024ReReco_20260201Forest_HIForward14/260203_164150/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward15/crab_2023PbPbUPC_Jan2024ReReco_20260201Forest_HIForward15/260203_164240/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward16/crab_2023PbPbUPC_Jan2024ReReco_20260201Forest_HIForward16/260203_164315/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward17/crab_2023PbPbUPC_Jan2024ReReco_20260201Forest_HIForward17/260203_170542/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward18/crab_2023PbPbUPC_Jan2024ReReco_20260201Forest_HIForward18/260203_170619/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward19/crab_2023PbPbUPC_Jan2024ReReco_20260201Forest_HIForward19/260204_170052/000[0-1]
)

OUTPUTPRIDIR="/eos/cms/store/group/phys_heavyions/"$USER"/Forest"${Year}"PbPb"
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
                rm -f $INPUTFILELIST
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

    [[ ($INPUTDIR == *BeamA* && $IsGammaNMCtype == false) || ($INPUTDIR == *BeamB* && $IsGammaNMCtype == true) ]] && { echo -e "\e[31merror:\e[0m mismatching between IsGammaNMCtype ("$IsGammaNMCtype") and Beam for MC." ; continue ; }
    [[ ($INPUTDIR == *ythia* && $IsData == true) || ($INPUTDIR == *HIForward* && $IsData == false) ]] && { echo -e "\e[31merror:\[0m mismatching between IsData ("$IsData") and INPUTDIR "$INPUTDIR ; continue ; }

    if [ "$submit_jobs" -eq 1 ] ; then
        set -x
        ./tt-condor-checkfile.sh $EXEFILE "$INPUTFILELIST" $OUTPUTDIR $MAXFILENO $LOGDIR $IsData $ApplyDRejection $IsGammaNMCtype $Year $ApplyTriggerRejection
        set +x
    fi

done

if [[ "$prep_jobs" -gt 0 ]] ; then
    echo
    cp -v ../$EXEFILE .
    cp -v $PIDfile .
fi


# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_374804_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_374810_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_374828_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_374833_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_374925_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_374950_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_374951_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_374953_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_374961_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_374970_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_374997_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_374998_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375001_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375002_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375007_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375013_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375055_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375058_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375060_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375061_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375064_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375110_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375145_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375164_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375195_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375202_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375245_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375252_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375256_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375259_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375300_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375317_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375391_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375413_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375415_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375440_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375441_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375448_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375455_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375463_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375465_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375483_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375491_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375507_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375513_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375530_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375531_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375545_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375549_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375658_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375659_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375665_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375666_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375695_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375696_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375697_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375703_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375738_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375739_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375740_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375744_HIForward1.txt
# filelists/HIForward1/filelist_Run3_2023UPC_Jan2024ReReco_375746_HIForward1.txt
