#!/bin/bash

if [[ $0 != *.sh ]]
then
    echo -e "\e[31;1merror:\e[0m use \e[32;1m./script.sh\e[0m instead of \e[32;1msource script.sh\e[0m"
    return 1
fi

#
MAXFILENO=5

# parameters
CORRPATH=/afs/cern.ch/work/w/wangj/RECO2025/CMSSW_15_0_6/src/MITHIGAnalysis2024/CommonCode/root
IsData=True
CollisionSystem=NeNe
Fraction=1
ApplyTriggerRejection=0
ApplyEventRejection=false
ApplyTrackRejection=false
sampleType=-1
DebugMode=false
includeL1EMU=false

#
movetosubmit=${1:-0}
runjobs=${2:-0}

PRIMARY="skim_250805"

INPUTS=(
    "/eos/cms/store/group/phys_heavyions/xirong/Run3_NeonRAA/PromptForest/IonPhysics0/crab_NeNe_IonPhysics0_AllPtV1_0716LynnTest/250716_180522/000*/" # quote is nessary
)

OUTPUTPRIDIR="/eos/cms/store/group/phys_heavyions/$USER/NeNe2025"
WORKDIR="$PWD"

CORRFILES=$(ls $CORRPATH/* | grep -E 'OO|pp|NeNe') # I assume all the useful correction files have the name including OO, ppref or NeNe

# don't need to change things below if you are just user

[[ x$CORRFILES == x ]] && { echo "error: no correction files." ; exit 1; }
[[ x$CMSSW_VERSION == x ]] && { echo "error: do cmsenv first." ; exit 1; }

#
mkdir -p $WORKDIR filelists

for INPUTDIR in "${INPUTS[@]}"
do
    echo -e "\e[32m$INPUTDIR\e[0m"
    
    if [[ $INPUTDIR == *.txt ]]
    then
        INPUTFILELIST=$INPUTDIR 
    else
        CRABNAME=${INPUTDIR##*crab_} ; CRABNAME=${CRABNAME%%/*} ;
        INPUTFILELIST="$WORKDIR/filelists/filelist_"$CRABNAME".txt"
        if [[ $INPUTDIR == /mnt/T2_US_MIT/* ]] ; then
            ls --color=no $INPUTDIR/* -d | sed -e "s/\/mnt\/T2_US_MIT\/hadoop\/cms/root:\/\/xrootd.cmsaf.mit.edu\//g" > $INPUTFILELIST
        fi
        if [[ $INPUTDIR == /eos* ]] ; then
            ls --color=no $INPUTDIR/* -d | sed -e "s/\/eos/root:\/\/eoscms.cern.ch\/\/eos/g" > $INPUTFILELIST
        fi
    fi
    echo "$INPUTFILELIST"
    REQUESTNAME=${INPUTFILELIST##*filelist_} ; REQUESTNAME=${REQUESTNAME%%.txt} ;
    OUTPUTSUBDIR="${PRIMARY}_${REQUESTNAME}"

    ##
    OUTPUTDIR="${OUTPUTPRIDIR}/${OUTPUTSUBDIR}"
    LOGDIR="logs/log_${OUTPUTSUBDIR}"

    echo "$OUTPUTDIR"
    ##

    if [ "$runjobs" -eq 1 ]
    then
        [[ $(ls -lt /tmp/ | grep --color=no "$USER " | grep --color=no -m 1 x509)x == x ]] && voms-proxy-init --voms cms --valid 168:00 ;
        EXISTPROXY=$(ls /tmp/ -lt | grep $USER | grep -m 1 x509 | awk '{print $NF}') ;
        cp -v /tmp/$EXISTPROXY $HOME/ ;
        
        set -x
        ./tt-condor-checkfile.sh "$INPUTFILELIST" $OUTPUTDIR $MAXFILENO $LOGDIR $IsData $CollisionSystem $Fraction $ApplyTriggerRejection $ApplyEventRejection $ApplyTrackRejection $sampleType $DebugMode $includeL1EMU corr.tar.gz
        set +x
    fi

done

if [[ "$movetosubmit" -eq 1 ]]
then
    mkdir -p root
    cp $CORRFILES root/
    tar -czvf corr.tar.gz ./root
    rm -r root
    # tar -czvf corr.tar.gz $CORRPATH

    cd ../    
    . clean.sh
    cd lxplus/    
    mv -v ../Execute $WORKDIR/

    # cd ../
    # tar -czvf tracklet.tar.gz transmute_trees.C include
    # cd lxplus
    
    # mv ../tracklet.tar.gz .
    
fi

# CORRFILES=
# case $CollisionSystem in
#     "pp")
#         CORRFILES=$CORRPATH/Eff_ppref_2024_Pythia_minBias_NopU_2D_Nominal_Official.root,$CORRPATH/Eff_ppref_2024_Pythia_minBias_NopU_2D_Loose_Official.root,$CORRPATH/Eff_ppref_2024_Pythia_minBias_NopU_2D_Tight_Official.root
#         CORRFILES=$CORRFILES,$CORRPATH/ParticleSpeciesCorrectionFactorsOO.root
#         ;;
#     "OO")
#         CORRFILES=$CORRPATH/Eff_OO_2025_PythiaHijing_QCD_pThat15_NoPU_pThatweight_2D_Nominal_Official.root,$CORRPATH/Eff_OO_2025_Hijing_MB_NoPU_2D_Nominal_Official.root
#         CORRFILES=$CORRFILES,$CORRPATH/Eff_OO_2025_PythiaHijing_QCD_pThat15_NoPU_pThatweight_2D_Loose_Official.root,$CORRPATH/Eff_OO_2025_Hijing_MB_NoPU_2D_Loose_Official.root
#         CORRFILES=$CORRFILES,$CORRPATH/Eff_OO_2025_PythiaHijing_QCD_pThat15_NoPU_pThatweight_2D_Tight_Official.root,$CORRPATH/Eff_OO_2025_Hijing_MB_NoPU_2D_Tight_Official.root
#         CORRFILES=$CORRFILES,$CORRPATH/OORAA_MULT_EFFICIENCY_HIJING_HF13AND.root,$CORRPATH/OORAA_MULT_EFFICIENCY_HIJING_HF10AND.root,$CORRPATH/OORAA_MULT_EFFICIENCY_HIJING_HF19AND.root
#         CORRFILES=$CORRFILES,$CORRPATH/ParticleSpeciesCorrectionFactorsOO.root
#         ;;
#     "NeNe")
#         CORRFILES=$CORRPATH/Eff_OO_2025_PythiaHijing_QCD_pThat15_NoPU_pThatweight_2D_Nominal_Official.root,$CORRPATH/Eff_OO_2025_Hijing_MB_NoPU_2D_Nominal_Official.root
#         CORRFILES=$CORRFILES,$CORRPATH/Eff_OO_2025_PythiaHijing_QCD_pThat15_NoPU_pThatweight_2D_Loose_Official.root,$CORRPATH/Eff_OO_2025_Hijing_MB_NoPU_2D_Loose_Official.root
#         CORRFILES=$CORRFILES,$CORRPATH/Eff_OO_2025_PythiaHijing_QCD_pThat15_NoPU_pThatweight_2D_Tight_Official.root,$CORRPATH/Eff_OO_2025_Hijing_MB_NoPU_2D_Tight_Official.root
#         CORRFILES=$CORRFILES,$CORRPATH/NeNeRAA_MULT_EFFICIENCY_HIJING_HF13AND.root,$CORRPATH/NeNeRAA_MULT_EFFICIENCY_HIJING_HF10AND.root,$CORRPATH/NeNeRAA_MULT_EFFICIENCY_HIJING_HF19AND.root
#         CORRFILES=$CORRFILES,$CORRPATH/ParticleSpeciesCorrectionFactorsOO.root
#         ;;
# esac
# CORRFILES=${CORRFILES//","/" "}
# bad=0
# for i in ${CHECKCORR[@]} ; do
#     [[ -f $i ]] || {
#         echo -e "\e[31m(x) $i\e[0m" ;
#         bad=$(($bad+1)) ; }
# done
# [[ $bad -gt 0 ]] && { echo "error: has bad correction files." ; exit 1; }
