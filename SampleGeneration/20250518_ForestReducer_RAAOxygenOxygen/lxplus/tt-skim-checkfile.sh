#!/bin/bash

if [[ $# -ne 13 ]]; then
    echo "usage: ./tt-skim-checkfile.sh [input file] [output dir] [output filename] [release] [IsData] [CollisionSystem] [Fraction] [ApplyTriggerRejection] [ApplyEventRejection] [ApplyTrackRejection] [sampleType] [DebugMode] [includeL1EMU]"
    exit 1
fi

INFILE=$1
DESTINATION=$2
OUTFILE=$3
CRELEASE=$4
IsData=$5
CollisionSystem=$6
Fraction=$7
ApplyTriggerRejection=$8
ApplyEventRejection=$9
ApplyTrackRejection=${10}
sampleType=${11}
DebugMode=${12}
includeL1EMU=${13}

echo $SCRAM_ARCH

tar -xzvf corr.tar.gz
source /cvmfs/cms.cern.ch/cmsset_default.sh
scramv1 project CMSSW $CRELEASE
# g++ transmute_trees.C $(root-config --libs --cflags) -g -o transmute_trees

[[ -d $CRELEASE/src ]] && {
    cd $CRELEASE/src
    eval `scram runtime -sh`
    cd ../../

    set -x

    root --version

    xrdcp $INFILE .

    input_file=$INFILE
    [[ -f $PWD/${INFILE##*/} ]] && input_file=$PWD/${INFILE##*/} || echo "xrdcp failed."

    ./Execute --Input $input_file \
              --Output $OUTFILE \
              --IsData $IsData \
              --CollisionSystem $CollisionSystem \
              --Fraction $Fraction \
              --ApplyTriggerRejection $ApplyTriggerRejection \
              --ApplyEventRejection $ApplyEventRejection \
              --ApplyTrackRejection $ApplyTrackRejection \
              --sampleType $sampleType \
              --DebugMode $DebugMode \
              --includeL1EMU $includeL1EMU \
              --CorrectionPath ./root 

    if [[ $(wc -c $OUTFILE | awk '{print $1}') -gt 700 ]] ; then
        # xrdcp
        SRM_PREFIX="/eos/cms/" ; SRM_PATH=${DESTINATION#${SRM_PREFIX}} ;
        xrdcp ${OUTFILE} root://eoscms.cern.ch//${SRM_PATH}/$OUTFILE
    fi
    set +x
}

rm -rf Execute root/ corr.tar.gz $CRELEASE
rm $PWD/${INFILE##*/}
rm $OUTFILE
