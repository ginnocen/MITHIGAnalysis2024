#!/bin/bash

BASENAME=${1}
JOB_LIST=${2}
CONFIG_DIR=${3}
OUTPUT_SERVER=${4}
OUTPUT_PATH=${5}
PROXYFILE=${6}
JOB_MEMORY=${7}
JOB_STORAGE=${8}
CMSSW_VERSION=${9}
ANALYSIS_SUBDIR=${10}

SCRIPT="${CONFIG_DIR}/${BASENAME}_script.sh"
CONFIG="${CONFIG_DIR}/${BASENAME}_config.condor"
JOB_LIST_NAME="${JOB_LIST##*/}"
PROXYFILE_NAME="${PROXYFILE##*/}"



# MAKE CONDOR SCRIPT ==========================================================
rm $SCRIPT
sleep 0.5
cat > $SCRIPT <<EOF1
#!/bin/bash

CLUSTER=\$1
PROC=\$2

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
echo ""
echo ">>> Setting up ${CMSSW_VERSION}"
source /cvmfs/cms.cern.ch/cmsset_default.sh
wait
cmsrel $CMSSW_VERSION
wait
cd ${CMSSW_VERSION}/src/
cmsenv
wait
cd ../../
which root
which hadd
echo ""
echo ">>> Setting up directory"
xrdcp -r -f --notlsok root://xrootd.cmsaf.mit.edu//store/user/$USER/MITHIGAnalysis2024 .
cd MITHIGAnalysis2024
source SetupAnalysis.sh
wait
cd CommonCode/
make
wait
cd ../$ANALYSIS_SUBDIR
cp ../../../$JOB_LIST_NAME .
cp ../../../$PROXYFILE_NAME .
ls -l
echo ""
echo ">>> Compiling skimmer"
# Must manually compile to avoid error from missing test file
g++ ReduceForest.cpp -o Execute \\
  \$(root-config --cflags --glibs) \\
  -I\$ProjectBase/CommonCode/include \$ProjectBase/CommonCode/library/Messenger.o
wait
sleep 1
if ! [ -f "Execute" ]; then
  echo "ERROR: Unable to compile executable!"
  exit 2
fi

# Skimming
echo ""
echo ">>> Running skimmer"
mkdir -p "output"
COUNTER=0
while read -r ROOT_IN_T2; do
  ROOT_IN_LOCAL="forest_\${COUNTER}.root"
  ROOT_OUT="output/${BASENAME}_\${COUNTER}.root"
  xrdcp -f -N -s --notlsok \$ROOT_IN_T2 \$ROOT_IN_LOCAL
  wait
  if ! [ -f "\$ROOT_IN_LOCAL" ]; then
    echo "--- ERROR! Missing root file: \$ROOT_IN_LOCAL"
    exit 1
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
    --PFTree particleFlowAnalyser/pftree &
  wait
  sleep 1
  rm \$ROOT_IN_LOCAL
  ((COUNTER++))
done < $JOB_LIST_NAME
wait
echo ""
echo ">>> Completed \$COUNTER jobs!"

# Merge and transfer
echo ""
echo ">>> Merging root files"
hadd -ff ${BASENAME}_merged.root output/${BASENAME}_*.root
wait
echo ""
echo ">>> Transferring merged root file to T2"
xrdcp -f --notlsok ${BASENAME}_merged.root ${OUTPUT_SERVER}${OUTPUT_PATH}
wait
echo ""
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
transfer_input_files    = $JOB_LIST
MAX_TRANSFER_INPUT_MB   = 400
#on_exit_remove          = (ExitBySignal == False) && (ExitCode == 0)

### Logging
notification            = Error
output                  = ${CONFIG_DIR}/${BASENAME}_out_\$(ClusterId)_\$(ProcId).txt
error                   = ${CONFIG_DIR}/${BASENAME}_err_\$(ClusterId)_\$(ProcId).txt
log                     = ${CONFIG_DIR}/${BASENAME}_log_\$(ClusterId)_\$(ProcId).txt

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
