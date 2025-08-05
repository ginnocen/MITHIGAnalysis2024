#!/bin/bash

# https://batchdocs.web.cern.ch/local/submit.html

if [[ $# -ne 14 ]]; then
    echo "usage: ./tt-condor-checkfile.sh [input dir] [output dir] [max jobs] [log dir] [IsData] [CollisionSystem] [Fraction] [ApplyTriggerRejection] [ApplyEventRejection] [ApplyTrackRejection] [sampleType] [DebugMode] [includeL1EMU]"
    exit 1
fi

FILELIST=$1
DESTINATION=$2
MAXFILES=$3
LOGDIR=$4
IsData=$5
CollisionSystem=$6
Fraction=$7
ApplyTriggerRejection=$8
ApplyEventRejection=$9
ApplyTrackRejection=${10}
sampleType=${11}
DebugMode=${12}
includeL1EMU=${13}
CORRFILES=${14}

# PROXYFILE=$(ls /tmp/ -lt | grep $USER | grep -m 1 x509 | awk '{print $NF}')
PROXYFILE=$HOME/$(ls $HOME -lt | grep $USER | grep -m 1 x509 | awk '{print $NF}')

tag="skim"

DEST_CONDOR=${DESTINATION}

if [ ! -d $DESTINATION ]
then
    mkdir -p $DESTINATION
fi

mkdir -p $LOGDIR

counter=0
for i in `cat $FILELIST`
do
    if [ $counter -ge $MAXFILES ]
    then
        break
    fi
    inputname=${i}
    infn=${inputname##*/}
    infn=${infn%%.*} # no .root
    outputfile=${tag}_${infn}.root
    if [ ! -f ${DESTINATION}/${outputfile} ]
    then
        echo -e "\033[38;5;242mSubmitting a job for output\033[0m ${DESTINATION}/${outputfile}"
        
        cat > tt-${tag}.condor <<EOF

Universe     = vanilla
Initialdir   = $PWD/
Notification = Error
Executable   = $PWD/tt-${tag}-checkfile.sh
Arguments    = $inputname $DEST_CONDOR ${outputfile} $CMSSW_VERSION $IsData $CollisionSystem $Fraction $ApplyTriggerRejection $ApplyEventRejection $ApplyTrackRejection $sampleType $DebugMode $includeL1EMU
Output       = $LOGDIR/log-${infn}.out
Error        = $LOGDIR/log-${infn}.err
Log          = $LOGDIR/log-${infn}.log
# Rank         = Mips
+AccountingGroup = "group_u_CMST3.all"
+JobFlavour = "longlunch"
# Requirements = ( OpSysAndVer =?= "CentOS8" )
MY.WantOS = "el8"
should_transfer_files = YES
use_x509userproxy = True
x509userproxy = $PROXYFILE
transfer_input_files = Execute,$CORRFILES
Queue 
EOF

condor_submit tt-${tag}.condor
mv tt-${tag}.condor $LOGDIR/log-${infn}.condor
counter=$(($counter+1))
    fi
done

echo -e "Submitted \033[1;36m$counter\033[0m jobs to Condor."
