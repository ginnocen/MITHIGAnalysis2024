#!/bin/bash

# https://batchdocs.web.cern.ch/local/submit.html

if [[ $# -ne 6 ]]; then
    echo "usage: ./tt-condor-checkfile.sh [executable file] [input dir] [output dir] [max jobs] [log dir] [IsData] " 
    exit 1
fi

EXEFILE=$1
FILELIST=$2
DESTINATION=$3
MAXFILES=$4
LOGDIR=$5
IsData=$6
SCRVERSION=${SCRAM_ARCH%%_*}
runtimelimit="espresso" # espresso = 20 min, microcentury = 1 hour, longlunch = 2 hours

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
Arguments    = $EXEFILE $inputname $DEST_CONDOR ${outputfile} $CMSSW_VERSION $IsData 
Output       = $LOGDIR/log-${infn}.out
Error        = $LOGDIR/log-${infn}.err
Log          = $LOGDIR/log-${infn}.log
# +AccountingGroup = "group_u_CMST3.all"
# +AccountingGroup = "group_u_CMS.u_zh.priority"
+JobFlavour = "$runtimelimit"
MY.WantOS = "$SCRVERSION"
should_transfer_files = YES
use_x509userproxy = True
x509userproxy = $PROXYFILE
transfer_input_files = ${EXEFILE},DzeroUPC_dedxMap.root
Queue 
EOF

condor_submit tt-${tag}.condor
mv tt-${tag}.condor $LOGDIR/log-${infn}.condor
counter=$(($counter+1))
    fi
done

echo -e "Submitted \033[1;36m$counter\033[0m jobs to Condor."
