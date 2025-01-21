#!/bin/bash

JOB_NAME=${1}
JOB_LIST=${2}
CONFIG_DIR=${3}
OUTPUT_SERVER=${4}
OUTPUT_PATH=${5}
PROXYFILE=${6}
JOB_MEMORY=${7}
JOB_STORAGE=${8}
CMSSW_VERSION=${9}
ANALYSIS_SUBDIR=${10}
YEAR=${11}

SCRIPT="${CONFIG_DIR}/${JOB_NAME}_script.sh"
CONFIG="${CONFIG_DIR}/${JOB_NAME}_config.condor"
JOB_LIST_NAME=$(basename "$JOB_LIST")
PROXYFILE_NAME=$(basename "$PROXYFILE")
OUTPUT_DIR=$(dirname "$OUTPUT_PATH")

ZDCM_THRESHOLD=1000
ZDCP_THRESHOLD=1100
if [[ $YEAR -eq 2024 ]]; then
  ZDCM_THRESHOLD=900
  ZDCP_THRESHOLD=900
fi

# MAKE CONDOR SCRIPT ==========================================================
rm $SCRIPT
sleep 0.5
cat > $SCRIPT <<EOF1
#!/bin/bash

CLUSTER=\$1
PROC=\$2
SAVE_IO_LISTS=1

# OS config checks
echo ""
echo ">>> Host info"
hostname
uname -a
echo ""
echo ">>> VOMS info"
unset  X509_USER_KEY
export X509_USER_KEY=$PROXYFILE_NAME
voms-proxy-info

# Setup
echo ">>> Setting up ${CMSSW_VERSION}"
source /cvmfs/cms.cern.ch/cmsset_default.sh
cmsrel $CMSSW_VERSION
cd ${CMSSW_VERSION}/src/
cmsenv
cd ../../
which root
which hadd
echo ">>> Setting up directory"
tar -xf MITHIGAnalysis2024.tar
cd MITHIGAnalysis2024
source SetupAnalysis.sh
cd CommonCode/
make
cd ../$ANALYSIS_SUBDIR
cp ../../../$JOB_LIST_NAME .
cp ../../../$PROXYFILE_NAME .
ls -l
echo ">>> Compiling skimmer"
# Must manually compile to avoid error from missing test file
g++ ReduceForest.cpp -o Execute \\
  \$(root-config --cflags --glibs) \\
  -I\$ProjectBase/CommonCode/include \$ProjectBase/CommonCode/library/Messenger.o
sleep 1
if ! [ -f "Execute" ]; then
  echo "ERROR: Unable to compile executable!"
  exit 2
fi

# Skimming
echo ">>> Running skimmer"
mkdir -p "output"
COUNTER=0
ROOT_IN_LIST="${JOB_NAME}_rootIn.txt"
ROOT_OUT_LIST="${JOB_NAME}_rootOut.txt"
while read -r ROOT_IN_T2; do
  ROOT_IN_LOCAL="forest_\${COUNTER}.root"
  ROOT_OUT="output/${JOB_NAME}_\${COUNTER}.root"
  xrdcp -N -S 4 --retry 2 --retry-policy continue --notlsok \$ROOT_IN_T2 \$ROOT_IN_LOCAL
  wait
  echo \$(ls -lh \$ROOT_IN_LOCAL) >> \$ROOT_IN_LIST
  if ! [ -f "\$ROOT_IN_LOCAL" ]; then
    echo "--- ERROR! Missing root file: \$ROOT_IN_LOCAL"
    continue
  fi
  echo "--- Processing file: \$ROOT_IN_LOCAL"
  ./Execute --Input \$ROOT_IN_LOCAL \\
    --Output \$ROOT_OUT \\
    --Year 2023 \\
    --ApplyTriggerRejection 2 \\
    --ApplyEventRejection true \\
    --ApplyZDCGapRejection true \\
    --ApplyDRejection 2 \\
    --ZDCMinus1nThreshold 1000 \\
    --ZDCPlus1nThreshold 1100 \\
    --IsData true \\
    --PFTree particleFlowAnalyser/pftree \\
    --HideProgressBar true &
  wait
  echo \$(ls -lh \$ROOT_OUT) >> \$ROOT_OUT_LIST
  rm \$ROOT_IN_LOCAL
  ((COUNTER++))
done < $JOB_LIST_NAME
echo ">>> Completed \$COUNTER jobs!"

# Merge and transfer
echo ">>> Merging root files"
hadd -ff -k ${JOB_NAME}_merged.root output/${JOB_NAME}_*.root
echo \$(ls -lh \{JOB_NAME}_merged.root) >> \$ROOT_OUT_LIST
echo ">>> Transferring merged root file to T2"
xrdcp -N -S 4 --retry 2 --retry-policy continue --notlsok ${JOB_NAME}_merged.root ${OUTPUT_SERVER}${OUTPUT_PATH}
if [ \$SAVE_IO_LISTS -eq 1 ]; then
  xrdfs ${OUTPUT_SERVER} mkdir -p ${OUTPUT_DIR}/file_lists
  xrdcp -N --retry 2 --retry-policy continue --notlsok \$ROOT_IN_LIST ${OUTPUT_SERVER}${OUTPUT_DIR}/file_lists/\$ROOT_IN_LIST
  xrdcp -N --retry 2 --retry-policy continue --notlsok \$ROOT_OUT_LIST ${OUTPUT_SERVER}${OUTPUT_DIR}/file_lists/\$ROOT_OUT_LIST
fi
echo ">>> Done!"

EOF1
# -----------------------------------------------------------------------------
echo "Made script: $SCRIPT"



# MAKE CONDOR CONFIG ==========================================================
rm $CONFIG
sleep 0.5
cat > $CONFIG <<EOF2
### Job settings
Universe                = vanilla
request_disk            = ${JOB_STORAGE}GB
request_memory          = ${JOB_MEMORY}GB
initialdir              = $PWD/
executable              = $PWD/$SCRIPT
arguments               = \$(ClusterId) \$(ProcId)
use_x509userproxy       = True
x509userproxy           = $PROXYFILE
+AccountingGroup        = "analysis.$USER"
max_retries             = 1

### File transfer
should_transfer_files   = YES
transfer_input_files    = $JOB_LIST,MITHIGAnalysis2024.tar
MAX_TRANSFER_INPUT_MB   = 400
#on_exit_remove          = (ExitBySignal == False) && (ExitCode == 0)

### Logging
notification            = Error
output                  = ${CONFIG_DIR}/${JOB_NAME}_out_\$(ClusterId)_\$(ProcId).txt
error                   = ${CONFIG_DIR}/${JOB_NAME}_err_\$(ClusterId)_\$(ProcId).txt
log                     = ${CONFIG_DIR}/${JOB_NAME}_log_\$(ClusterId)_\$(ProcId).txt

### Server settings
MY.WantOS               = "el9"
Requirements            = ( (OpSysAndVer =?= "AlmaLinux9" || OpSysAndVer =?= "CentOS9") && (Arch =?= "X86_64") && (BOSCOCluster =!= "t3serv008.mit.edu") && (BOSCOCluster =!= "ce03.cmsaf.mit.edu") && (BOSCOCluster =!= "eofe8.mit.edu") )
+SingularityImage       = "/cvmfs/unpacked.cern.ch/registry.hub.docker.com/cmssw/el9:x86_64-d20241130"
+DESIRED_Sites          = "mit_tier2,mit_tier3,T2_AT_Vienna,T2_BE_IIHE,T2_BE_UCL,T2_BR_SPRACE,T2_BR_UERJ,T2_CH_CERN,T2_CH_CERN_AI,T2_CH_CERN_HLT,T2_CH_CERN_Wigner,T2_CH_CSCS,T2_CH_CSCS_HPC,T2_CN_Beijing,T2_DE_DESY,T2_DE_RWTH,T2_EE_Estonia,T2_ES_CIEMAT,T2_ES_IFCA,T2_FI_HIP,T2_FR_CCIN2P3,T2_FR_GRIF_IRFU,T2_FR_GRIF_LLR,T2_FR_IPHC,T2_GR_Ioannina,T2_HU_Budapest,T2_IN_TIFR,T2_IT_Bari,T2_IT_Legnaro,T2_IT_Pisa,T2_IT_Rome,T2_KR_KISTI,T2_MY_SIFIR,T2_MY_UPM_BIRUNI,T2_PK_NCP,T2_PL_Swierk,T2_PL_Warsaw,T2_PT_NCG_Lisbon,T2_RU_IHEP,T2_RU_INR,T2_RU_ITEP,T2_RU_JINR,T2_RU_PNPI,T2_RU_SINP,T2_TH_CUNSTDA,T2_TR_METU,T2_TW_NCHC,T2_UA_KIPT,T2_UK_London_IC,T2_UK_SGrid_Bristol,T2_UK_SGrid_RALPP,T2_US_Caltech,T2_US_Florida,T2_US_Nebraska,T2_US_Purdue,T2_US_UCSD,T2_US_Vanderbilt,T2_US_Wisconsin,T3_CH_CERN_CAF,T3_CH_CERN_DOMA,T3_CH_CERN_HelixNebula,T3_CH_CERN_HelixNebula_REHA,T3_CH_CMSAtHome,T3_CH_Volunteer,T3_US_HEPCloud,T3_US_NERSC,T3_US_OSG,T3_US_PSC,T3_US_SDSC,T3_US_MIT"

queue 1

EOF2
# -----------------------------------------------------------------------------
echo "Made config: $CONFIG"

sleep 0.5
condor_submit $CONFIG
wait
