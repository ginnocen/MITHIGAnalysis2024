#!/bin/bash

# Set 1 if data, 0 if MC
ISDATA=1
RUNYEAR=2024

RUNLIST=(
### 2024 Valid Runs
  388000 388004 388005 388006 388020
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
### 2023 Valid Runs
#  374804 374810 374828 374833
#  374834 374925 374950 374951 374953
#  374961 374970 374997 374998 375001
#  375002 375007 375013 375055 375058
#  375060 375061 375064 375110 375145
#  375164 375195 375202 375245 375252
#  375256 375259 375300 375317 375391
#  375413 375415 375440 375441 375448
#  375455 375463 375465 375483 375491
#  375507 375513 375530 375531 375545
#  375549 375658 375659 375665 375666
#  375695 375696 375697 375703 375738
#  375739 375740 375744 375746
)
PDMIN=0
PDMAX=19

if ! [ -z "$1" ]; then
  RUNLIST=($1)
fi
if ! [ -z "$2" ]; then
  PDMIN=$2
  PDMAX=$2
fi

# SUBMISSION ###################################################################

if [[ $ISDATA -eq 1 ]]; then
  echo ">>>>> Submitting Data <<<<<"
  # Update variables in CMSSW config:
  sed -i -e 's|isData=0|isData=1|g; s|runYear=2023|runYear='$RUNYEAR'|g; s|runYear=2024|runYear='$RUNYEAR'|g' forestCMSSWConfig_miniAOD_Run3UPC.py
  wait
  # Iterate through each Run and PD
  for RUN in ${RUNLIST[@]}; do
    for (( PD=$PDMIN; PD<=PDMAX; PD++ )); do
      # Set run/PD dependent parameters
      PDNAME="HIForward${PD}"
      JOBTAG="Run3UPC2024_PromptReco_${RUN}_${PDNAME}"
      DATASET="/${PDNAME}/HIRun2024A-PromptReco-v1/MINIAOD"
      DATABASE="global"
      
      # Copy CRAB config from template
      rm forestCRABConfig_Data_Run3UPC2024.py
      cp forestCRABConfig_DataTemplate_Run3UPC2024.py forestCRABConfig_Data_Run3UPC2024.py
      # Insert variables in CRAB config:
      sed -i -e 's|$RUN|'$RUN'|g; s|$JOBTAG|'$JOBTAG'|g; s|$DATASET|'$DATASET'|g; s|$DATABASE|'$DATABASE'|g' forestCRABConfig_Data_Run3UPC2024.py
      wait
      
      # Submit filled temp file
      crab submit -c forestCRABConfig_Data_Run3UPC2024.py
      wait
    done
  done
fi

rm forestCRABConfig_Data_Run3UPC2024.py
