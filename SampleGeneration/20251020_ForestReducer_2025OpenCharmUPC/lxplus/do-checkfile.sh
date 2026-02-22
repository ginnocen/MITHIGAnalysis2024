#!/bin/bash

if [[ $0 != *.sh ]] ; then
    echo -e "\e[31;1merror:\e[0m use \e[32;1m./script.sh\e[0m instead of \e[32;1msource script.sh\e[0m"
    return 1
fi

# Max number of files to submit for each input
MAXFILENO=1000000

# Exe parameters
Year=2025 ; IsData=true ; ApplyDRejection=no ; ApplyTriggerRejection=0 ; DptThreshold=0 ; ApplyZDCGapRejection=0 ; # Data 2025
# Year=2023 ; IsData=true ; ApplyDRejection=no ; ApplyTriggerRejection=0 ; DptThreshold=0 ; ApplyZDCGapRejection=0 ; # Data 2023
# Year=2024 ; IsData=false ; ApplyDRejection=no ; ApplyTriggerRejection=0 ; DptThreshold=0 ; ApplyZDCGapRejection=0 ; # MC 2024
IsGammaNMCtype=true
#
PRIMARY="Dzero_260212-hfle"
LABELTAG="" # e.g. versions or selections
#
[[ $ApplyDRejection != "no" ]] && LABELTAG+="_Drej-"$ApplyDRejection 
[[ $DptThreshold -gt 0 ]] && LABELTAG+="_Dpt-"$DptThreshold
[[ $ApplyTriggerRejection -gt 0 ]] && LABELTAG+="_Trig-"$ApplyTriggerRejection 
[[ $ApplyZDCGapRejection -gt 0 ]] && LABELTAG+="_ZDCgap-"$ApplyZDCGapRejection

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
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/wangj/prompt-GNucleusToD0-PhotonBeamA_Bin-Pthat0_Fil-Kpi_UPC_5p36TeV_pythia8-evtgen/crab_HiForest_260218_prompt_GNucleusToD0-PhotonBeamA_Bin-Pthat0_Kpi_t2/260218_200449/0000
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/wangj/nonprompt-GNucleusToD0-PhotonBeamA_Bin-Pthat0_Fil-Kpi_UPC_5p36TeV_pythia8-evtgen/crab_HiForest_260218_nonprompt_GNucleusToD0-PhotonBeamA_Bin-Pthat0_Kpi_t2/260219_200038/0000
    # ------ Ngamma -> IsGammaNMCtype=false
    # # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/wangj/prompt-GNucleusToD0-PhotonBeamB_Bin-Pthat0_Fil-Kpi_UPC_5p36TeV_pythia8-evtgen/crab_HiForest_260120_prompt_GNucleusToD0-PhotonBeamB_Bin-Pthat0_Kpi_Dpt1_PF0p1/260120_233803/0000
    # # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/wangj/nonprompt-GNucleusToD0-PhotonBeamB_Bin-Pthat0_Fil-Kpi_UPC_5p36TeV_pythia8-evtgen/crab_HiForest_260120_nonprompt_GNucleusToD0-PhotonBeamB_Bin-Pthat0_Kpi_Dpt1_PF0p1/260121_000604/0000

    # ------ Data -> 2025
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2025_PromptReco/HIForward0/crab_PbPbUPC_HIForward0/251227_162520/000[0-6]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2025_PromptReco/HIForward2/crab_PbPbUPC_HIForward2/251227_171556/000[0-6]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2025_PromptReco/HIForward4/crab_PbPbUPC_HIForward4/251227_171633/000[0-6]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2025_PromptReco/HIForward6/crab_PbPbUPC_HIForward6/251227_171846/000[0-6]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2025_PromptReco/HIForward8/crab_PbPbUPC_HIForward8/251227_171942/000[0-6]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2025_PromptReco/HIForward10/crab_PbPbUPC_HIForward10/251227_172110/000[0-6]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2025_PromptReco/HIForward12/crab_PbPbUPC_HIForward12/251228_175230/000[0-6]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2025_PromptReco/HIForward14/crab_PbPbUPC_HIForward14/251228_175327/000[0-6]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2025_PromptReco/HIForward16/crab_PbPbUPC_HIForward16/251228_175418/000[0-6]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2025_PromptReco/HIForward18/crab_PbPbUPC_HIForward18/251228_175501/000[0-6]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2025_PromptReco/HIForward20/crab_PbPbUPC_HIForward20/251228_175537/000[0-6]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2025_PromptReco/HIForward22/crab_PbPbUPC_HIForward22/251228_175617/000[0-6]

    # ------ Data -> 2023
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward0/crab_2023PbPbUPC_Jan2024ReReco_20260212Forest_HIForward0/260212_170406/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward1/crab_2023PbPbUPC_Jan2024ReReco_20260212Forest_HIForward1/260212_222917/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward2/crab_2023PbPbUPC_Jan2024ReReco_20260212Forest_HIForward2/260212_222948/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward3/crab_2023PbPbUPC_Jan2024ReReco_20260212Forest_HIForward3/260212_223021/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward4/crab_2023PbPbUPC_Jan2024ReReco_20260212Forest_HIForward4/260213_014237/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward5/crab_2023PbPbUPC_Jan2024ReReco_20260212Forest_HIForward5/260213_014922/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward6/crab_2023PbPbUPC_Jan2024ReReco_20260212Forest_HIForward6/260213_014951/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward7/crab_2023PbPbUPC_Jan2024ReReco_20260212Forest_HIForward7/260213_015019/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward8/crab_2023PbPbUPC_Jan2024ReReco_20260212Forest_HIForward8/260213_052504/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward9/crab_2023PbPbUPC_Jan2024ReReco_20260212Forest_HIForward9/260213_052532/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward10/crab_2023PbPbUPC_Jan2024ReReco_20260212Forest_HIForward10/260213_052645/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward11/crab_2023PbPbUPC_Jan2024ReReco_20260212Forest_HIForward11/260213_052909/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward12/crab_2023PbPbUPC_Jan2024ReReco_20260212Forest_HIForward12/260213_161418/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward13/crab_2023PbPbUPC_Jan2024ReReco_20260212Forest_HIForward13/260213_161513/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward14/crab_2023PbPbUPC_Jan2024ReReco_20260212Forest_HIForward14/260213_163821/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward15/crab_2023PbPbUPC_Jan2024ReReco_20260212Forest_HIForward15/260213_163904/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward16/crab_2023PbPbUPC_Jan2024ReReco_20260212Forest_HIForward16/260213_193217/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward17/crab_2023PbPbUPC_Jan2024ReReco_20260212Forest_HIForward17/260213_193432/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward18/crab_2023PbPbUPC_Jan2024ReReco_20260212Forest_HIForward18/260213_222303/000[0-1]
    # root://xrootd-vanderbilt.sites.opensciencegrid.org//store/user/jdlang/Run3_PbPbUPC/Forest_2023_Jan2024ReReco_2025Reforest/HIForward19/crab_2023PbPbUPC_Jan2024ReReco_20260212Forest_HIForward19/260213_222333/000[0-1]
    
    # ------ Data -> clusComp
    # 2025 EmptyBX
    # /eos/cms/store/group/phys_heavyions/wangj/Forest2025PbPb/forest/crab_HiForest_260218_HIEmptyBX_HIRun2025A_PromptReco_v1/260219_194030/0000
    # 2023 EmptyBX
    # /eos/cms/store/group/phys_heavyions/wangj/Forest2023PbPb/forest/crab_HiForest_260218_HIEmptyBX_HIRun2023A_PromptReco_v2/260219_210631/0000
    # 2025 high rate ZB
    # /eos/cms/store/group/phys_heavyions/wangj/Forest2025PbPb/forest/crab_HiForest_260218_HIPhysicsRawPrime0_HIRun2025A_highrZB_399766/260219_195114/0000
    # /eos/cms/store/group/phys_heavyions/wangj/Forest2025PbPb/forest/crab_HiForest_260218_HIPhysicsRawPrime1_HIRun2025A_highrZB_399766/260219_201619/0000
    # /eos/cms/store/group/phys_heavyions/wangj/Forest2025PbPb/forest/crab_HiForest_260218_HIPhysicsRawPrime2_HIRun2025A_highrZB_399766/260219_203507/0000
    # /eos/cms/store/group/phys_heavyions/wangj/Forest2025PbPb/forest/crab_HiForest_260218_HIPhysicsRawPrime3_HIRun2025A_highrZB_399766/260219_203544/0000
    # /eos/cms/store/group/phys_heavyions/wangj/Forest2025PbPb/forest/crab_HiForest_260218_HIPhysicsRawPrime4_HIRun2025A_highrZB_399766/260219_203628/0000
    # /eos/cms/store/group/phys_heavyions/wangj/Forest2025PbPb/forest/crab_HiForest_260218_HIPhysicsRawPrime5_HIRun2025A_highrZB_399766/260219_203907/0000
    # /eos/cms/store/group/phys_heavyions/wangj/Forest2025PbPb/forest/crab_HiForest_260218_HIPhysicsRawPrime6_HIRun2025A_highrZB_399766/260219_225853/0000
    # /eos/cms/store/group/phys_heavyions/wangj/Forest2025PbPb/forest/crab_HiForest_260218_HIPhysicsRawPrime7_HIRun2025A_highrZB_399766/260219_225941/0000
    # /eos/cms/store/group/phys_heavyions/wangj/Forest2025PbPb/forest/crab_HiForest_260218_HIPhysicsRawPrime8_HIRun2025A_highrZB_399766/260219_230030/0000
    # /eos/cms/store/group/phys_heavyions/wangj/Forest2025PbPb/forest/crab_HiForest_260218_HIPhysicsRawPrime9_HIRun2025A_highrZB_399766/260219_230144/0000
    # /eos/cms/store/group/phys_heavyions/wangj/Forest2025PbPb/forest/crab_HiForest_260218_HIPhysicsRawPrime10_HIRun2025A_highrZB_399766/260219_230232/0000
    # 2023 high rate ZB
    # /eos/cms/store/group/phys_heavyions/wangj/Forest2023PbPb/forest/crab_HiForest_260218_HIPhysicsRawPrime0_HIRun2023A_ZB_374970/260219_211205/0000
    # /eos/cms/store/group/phys_heavyions/wangj/Forest2023PbPb/forest/crab_HiForest_260218_HIPhysicsRawPrime1_HIRun2023A_ZB_374970/260219_211240/0000
    # /eos/cms/store/group/phys_heavyions/wangj/Forest2023PbPb/forest/crab_HiForest_260218_HIPhysicsRawPrime2_HIRun2023A_ZB_374970/260219_211409/0000
    # /eos/cms/store/group/phys_heavyions/wangj/Forest2023PbPb/forest/crab_HiForest_260218_HIPhysicsRawPrime3_HIRun2023A_ZB_374970/260219_211435/0000
    # /eos/cms/store/group/phys_heavyions/wangj/Forest2023PbPb/forest/crab_HiForest_260218_HIPhysicsRawPrime4_HIRun2023A_ZB_374970/260219_211507/0000
    # /eos/cms/store/group/phys_heavyions/wangj/Forest2023PbPb/forest/crab_HiForest_260218_HIPhysicsRawPrime5_HIRun2023A_ZB_374970/260219_211609/0000

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
        ./tt-condor-checkfile.sh $EXEFILE "$INPUTFILELIST" $OUTPUTDIR $MAXFILENO $LOGDIR $IsData $ApplyDRejection $IsGammaNMCtype $Year $ApplyTriggerRejection $DptThreshold $ApplyZDCGapRejection
        set +x
    fi

done

if [[ "$prep_jobs" -gt 0 ]] ; then
    echo
    cp -v ../$EXEFILE .
    cp -v $PIDfile .
fi

