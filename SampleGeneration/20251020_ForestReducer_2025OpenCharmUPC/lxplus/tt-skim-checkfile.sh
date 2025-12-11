#!/bin/bash

if [[ $# -ne 6 ]]; then
    echo "usage: ./tt-skim-checkfile.sh [executable file] [input file] [output dir] [output filename] [release] [IsData]" # [CollisionSystem] [Fraction] [ApplyTriggerRejection] [ApplyEventRejection] [ApplyTrackRejection] [sampleType] [DebugMode] [includeL1EMU]"
    exit 1
fi

EXEFILE=$1
INFILE=$2
DESTINATION=$3
OUTFILE=$4
CRELEASE=$5
IsData=$6
# CollisionSystem=$6
# Fraction=$7
# ApplyTriggerRejection=$8
# ApplyEventRejection=$9
# ApplyTrackRejection=${10}
# sampleType=${11}
# DebugMode=${12}
# includeL1EMU=${13}

echo $SCRAM_ARCH

# tar -xzvf corr.tar.gz
source /cvmfs/cms.cern.ch/cmsset_default.sh
scramv1 project CMSSW $CRELEASE

[[ -d $CRELEASE/src ]] && {
    cd $CRELEASE/src
    eval `scram runtime -sh`
    cd ../../

    root --version

    xrdcp $INFILE .

    set -x
    input_file=$INFILE
    # [[ -f $PWD/${INFILE##*/} ]] && input_file=$PWD/${INFILE##*/} || echo "xrdcp failed."

    ./$EXEFILE --Input $input_file \
               --Output $OUTFILE \
                --ApplyTriggerRejection 0 \
                --ApplyEventRejection false \
                --ApplyZDCGapRejection false \
                --ApplyDRejection no \
                --PFTree particleFlowAnalyser/pftree \
                --IsData $IsData

    ls
    
    if [[ $(wc -c $OUTFILE | awk '{print $1}') -gt 700 ]] ; then
        # xrdcp
        SRM_PREFIX="/eos/cms/" ; SRM_PATH=${DESTINATION#${SRM_PREFIX}} ;
        xrdcp ${OUTFILE} root://eoscms.cern.ch//${SRM_PATH}/$OUTFILE
    fi
    set +x
}

rm -rf $EXEFILE $CRELEASE
rm DzeroUPC_dedxMap.root
rm $PWD/${INFILE##*/}
rm $OUTFILE
