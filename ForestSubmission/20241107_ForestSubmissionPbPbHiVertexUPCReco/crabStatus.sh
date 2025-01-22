#!/bin/bash
# To use:
# bash crabCheck.sh <run_number(opt)> <PD(opt)>
# If no args are given, this will start from the first run in RUNLIST

ARGRUN=$1
ARGPD=$2

RUNLIST=(
### 2023 Valid Runs
  374804 374810 374828 374833 374925
#  374950 374951 374953 374961 374970
#  374997 374998 375001 375002 375007
#  375013 375055 375058 375060 375061
#  375064 375110 375145 375164 375195
#  375202 375245 375252 375256 375259
#  375300 375317 375391 375413 375415
#  375440 375441 375448 375455 375463
#  375465 375483 375491 375507 375513
#  375530 375531 375545 375549 375658
#  375659 375665 375666 375695 375696
#  375697 375703 375738 375739 375740
#  375744 375746
### 2024 Valid Runs
#  388000 388004 388005 388006 388020
#  388021 388037 388038 388039 388048
#  388049 388050 388056 388090 388091 
#  388092 388095 388121 388122 388168
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

WORKAREA="WorkArea"
MAXMEMORY=5000 # [MB]
MAXRUNTIME=450 # [minutes]

# CHECK JOB STATUS #############################################################

# Iterate through each run in the RUNLIST
for RUN in ${RUNLIST[@]}; do
  # If a run number is provided, skip to that run
  RUNYEAR=0
  if ! [ -z "$ARGRUN" ]; then
    if [[ "$RUN" == "${ARGRUN}" ]]; then
        unset ARGRUN
    else
      continue
    fi
  fi
  if (( $RUN < 375800 )); then
    RUNYEAR=2023
  elif (( $RUN < 388800 )); then
    RUNYEAR=2024
  fi
  # Iterate through each PD
  for (( PD=$PDMIN; PD<=PDMAX; PD++ )); do
    # If a PD is provided, skip to that PD
    if ! [ -z "$ARGPD" ]; then
      if [[ "$PD" == "${ARGPD}" ]]; then
        unset ARGPD
      else
        continue
      fi
    fi
    # Set run/PD dependent parameters
    PDNAME="HIForward${PD}"
    JOBTAG=""
    if [[ $RUNYEAR -eq 2023 ]]; then
      JOBTAG="Run3_2023UPC_2025ReReco_${RUN}_${PDNAME}"
      DATASET="/${PDNAME}/HIRun2023A-1Dec2024-v1/MINIAOD"
      cp forestCRABConfig_DataTemplate_Run3UPC2023.py forestCRABConfig_Data_Run3UPC.py
    elif [[ $RUNYEAR -eq 2024 ]]; then
      JOBTAG="Run3_2024UPC_2024PromptReco_${RUN}_${PDNAME}"
      DATASET="/${PDNAME}/HIRun2024A-PromptReco-v1/MINIAOD"
      cp forestCRABConfig_DataTemplate_Run3UPC2024.py forestCRABConfig_Data_Run3UPC.py
    fi
    
    echo "--------------------"
    echo ""
    echo "crab status -d ${WORKAREA}/crab_${JOBTAG} --verboseErrors"
    crab status -d $WORKAREA/crab_$JOBTAG --verboseErrors
  
    # Prompt for user input
    VALIDCHOICE=0
    while [ 1 ]; do
      echo ""
      echo "Status for run ${RUN} ${PDNAME}. What would you like to do?"
      echo "Check next job [n], Resubmit failed jobs [r], Kill jobs [k], Quit [q]: "
      read -r CHOICE
      
      if [[ "$CHOICE" == "n" ]]; then
        break
        
      elif [[ "$CHOICE" == "r" ]]; then
        echo "Set maxmemory (default is ${MAXMEMORY}): "
        read -r ARGMEMORY
        echo "Set maxjobruntime (default is ${MAXRUNTIME}): "
        read -r ARGRUNTIME
        RESUBMEMORY=$MAXMEMORY
        RESUBRUNTIME=$MAXRUNTIME
        if ! [ -z "$ARGMEMORY" ]; then
          RESUBMEMORY=$ARGMEMORY
        fi
        if ! [ -z "$ARGRUNTIME" ]; then
          RESUBRUNTIME=$ARGRUNTIME
        fi
        echo "crab resubmit --maxmemory ${RESUBMEMORY} --maxjobruntime ${RESUBRUNTIME} -d ${WORKAREA}/crab_${JOBTAG}"
        crab resubmit --maxmemory $RESUBMEMORY --maxjobruntime $RESUBRUNTIME -d $WORKAREA/crab_$JOBTAG
        wait
        break
        
      elif [[ "$CHOICE" == "k" ]]; then
        echo "Are you sure you want to kill this job [y/n]? "
        read -r CONFIRM
        if [[ "$CONFIRM" == "y" ]]; then
          echo "crab kill -d ${WORKAREA}/crab_${JOBTAG}"
          crab kill -d $WORKAREA/crab_$JOBTAG
        elif [[ "$CONFIRM" == "n" ]]; then
          echo "Job will NOT be killed."
        else
          echo "Invalid response. Job will NOT be killed."
        fi
        
      elif [[ "$CHOICE" == "q" ]]; then
        exit 0
        
      else
        "Entry was not a valid option."
      fi
      
      echo ""
    done
  done
done
