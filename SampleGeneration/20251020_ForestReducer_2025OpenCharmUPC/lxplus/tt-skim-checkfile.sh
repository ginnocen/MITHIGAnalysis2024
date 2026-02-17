#!/bin/bash

if [[ $# -ne 12 ]]; then
    echo "usage: ./tt-skim-checkfile.sh [executable file] [input file] [output dir] [output filename] [release] [IsData] [ApplyDRejection] [IsGammaNMCtype] [Year] [ApplyTriggerRejection] [DptThreshold] [ApplyZDCGapRejection]" 
    exit 1
fi

EXEFILE=$1
INFILE=$2
DESTINATION=$3
OUTFILE=$4
CRELEASE=$5
IsData=$6
ApplyDRejection=$7
IsGammaNMCtype=$8
Year=$9
ApplyTriggerRejection=${10}
DptThreshold=${11}
ApplyZDCGapRejection=${12}

echo "SCRAM_ARCH:          "$SCRAM_ARCH
echo "PWD:                 "$PWD
echo "_CONDOR_SCRATCH_DIR: "$_CONDOR_SCRATCH_DIR
echo "hostname:            "$(hostname)
echo "INFILE:              "$INFILE
echo "DESTINATION:         "$DESTINATION

# tar -xzvf corr.tar.gz
source /cvmfs/cms.cern.ch/cmsset_default.sh
scramv1 project CMSSW $CRELEASE # cmsrel

INFILE_NAME=$PWD/${INFILE##*/}

[[ -d $CRELEASE/src ]] && {
    cd $CRELEASE/src
    eval `scram runtime -sh` # cmsenv
    cd ../../

    root --version

    input_file=$INFILE
    rm -f $INFILE_NAME
    xrdcp $INFILE .
    [[ -f $INFILE_NAME ]] && input_file=$INFILE_NAME || echo "xrdcp failed."

    set -x
    
    ./$EXEFILE --Input $input_file \
               --Output $OUTFILE \
               --RootPID DzeroUPC_dedxMap.root \
               --ApplyTriggerRejection $ApplyTriggerRejection \
               --ApplyEventRejection false \
               --ApplyZDCGapRejection $ApplyZDCGapRejection \
               --ApplyDRejection $ApplyDRejection \
               --DptThreshold $DptThreshold \
               --IsGammaNMCtype $IsGammaNMCtype \
               --Year $Year \
               --IsData $IsData \
               --HideProgressBar true

    ls
    
    if [[ $(wc -c $OUTFILE | awk '{print $1}') -gt 700 ]] ; then # check output file size reasonable
        # xrdcp
        SRM_PREFIX="/eos/cms/" ; SRM_PATH=${DESTINATION#${SRM_PREFIX}} ;
        xrdcp ${OUTFILE} root://eoscms.cern.ch//${SRM_PATH}/$OUTFILE
    fi
    set +x
}

rm -rf $EXEFILE $CRELEASE
rm DzeroUPC_dedxMap.root
rm $INFILE_NAME
rm $OUTFILE
rm -v x509*
