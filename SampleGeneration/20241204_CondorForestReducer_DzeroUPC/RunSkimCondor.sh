#!/bin/bash

XROOTD_SERVER="root://xrootd5.cmsaf.mit.edu/"
T2_PARENT_DIR="/store/user/jdlang/run3_2024PromptReco"
T2_OUTPUT_DIR="/store/user/jdlang/run3_2024PromptRecoSkims_HIForward_20241211"
RUNLIST=(
  388000
#  388004 388005 388006 388020
#  388021 388037 388038 388039 388048
#  388049 388050 388056 388090 388091
#  388092 388095 388121 388122 388168
### Runs below are not processed!
#  388192 388305 388306 388317 388349
#  388350 388368 388369 388384 388389 
#  388390 388401 388402 388418 388419 
#  388425 388450 388468 388475 388476
#  388574 388575 388576 388577 388578
#  388579 388582 388583 388613 388620
#  388622 388624 388710 388711 388713
)
PDMIN=0
PDMAX=19
#FILES_PER_JOB=100

DATE=$(date +%Y%m%d)

xrdfs $XROOTD_SERVER mkdir -p $T2_OUTPUT_DIR

submit_condor_jobs() {
  local BASENAME=$1
  local JOBLIST=$2
  local CONFIGDIR=$3
  T2_OUTPUT="${T2_OUTPUT_DIR}/run${RUN}_HIForward${PD}.root"
  ./MakeSkimCondor.sh $BASENAME $JOBLIST $CONFIGDIR $XROOTD_SERVER $T2_OUTPUT
  wait
  sleep 0.2
  return 0
}

for RUN in ${RUNLIST[@]}; do
  for (( PD=$PDMIN; PD<=PDMAX; PD++ )); do
    CONFIGDIR="condorConfigs/${DATE}_${RUN}_${PD}"
    mkdir -p $CONFIGDIR
    T2_INPUT_DIR="${T2_PARENT_DIR}/Run3UPC2024_PromptReco_${RUN}_HIForward${PD}/"
    FILELIST="filelist_${RUN}_${PD}.txt"
    ./MakeSkimFileList.sh $XROOTD_SERVER $T2_INPUT_DIR $FILELIST
    # Iterate through list to make individual jobs
#    PART=1
    BASENAME="${RUN}_HIForward${PD}"
    JOBLIST="${CONFIGDIR}/${BASENAME}_joblist.txt"
    rm $JOBLIST
    FILE_COUNTER=0
    while IFS= read -r LINE; do
      echo "$LINE" >> "$JOBLIST"
      FILE_COUNTER=$((FILE_COUNTER + 1))
#      if ! (( $FILE_COUNTER % FILES_PER_JOB )); then
#        submit_condor_jobs $BASENAME $JOBLIST $CONFIGDIR
#        wait
#        # Make new job file
#        PART=$((PART + 1))
#        BASENAME="${RUN}_HIForward${PD}_${PART}"
#        JOBLIST="${CONFIGDIR}/${BASENAME}_joblist.txt"
#        rm $JOBLIST
#      fi
    done < $FILELIST
    # Submit final job file list
    submit_condor_jobs $BASENAME $JOBLIST $CONFIGDIR
    sleep 10 # add 10 second pause between PDs to offset streams from T2_MIT
  done # end PD loop
  sleep 60 # add 1 minute pause between runs to offset streams from T2_MIT
done # end RUN loop
