#!/bin/bash

if [[ $# -ne 6 ]]; then
    echo "usage: ./tt-skim-checkfile.sh [executable file] [input file] [output dir] [output filename] [release] [IsData]" 
    exit 1
fi

EXEFILE=$1
INFILE=$2
DESTINATION=$3
OUTFILE=$4
CRELEASE=$5
IsData=$6

echo "SCRAM_ARCH:          "$SCRAM_ARCH
echo "PWD:                 "$PWD
echo "_CONDOR_SCRATCH_DIR: "$_CONDOR_SCRATCH_DIR
echo "hostname:            "$(hostname)
echo "INFILE:              "$INFILE
echo "DESTINATION:         "$DESTINATION

# tar -xzvf corr.tar.gz
source /cvmfs/cms.cern.ch/cmsset_default.sh
scramv1 project CMSSW $CRELEASE

[[ -d $CRELEASE/src ]] && {
    cd $CRELEASE/src
    eval `scram runtime -sh`
    cd ../../

    root --version

    input_file=$INFILE
    xrdcp $INFILE .
    [[ -f $PWD/${INFILE##*/} ]] && input_file=$PWD/${INFILE##*/} || echo "xrdcp failed."

    set -x
    
    ./$EXEFILE --Input $input_file \
               --Output $OUTFILE \
               --RootPID DzeroUPC_dedxMap.root \
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
