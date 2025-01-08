#!/bin/bash

BASENAME=$1
JOBLIST=$2
CONFIGDIR=$3
XROOTD_SERVER=$4
T2_OUTPUT=$5

SCRIPT="${CONFIGDIR}/${BASENAME}_script.sh"
CONFIG="${CONFIGDIR}/${BASENAME}_config.condor"

JOBLIST_NAME="${JOBLIST##*/}"
PROXYFILE=$HOME/$(ls $HOME -lt | grep $USER | grep -m 1 x509 | awk '{print $NF}')
PROXYFILE_NAME="${PROXYFILE##*/}"



# MAKE CONDOR SCRIPT ==========================================================
rm $SCRIPT
sleep 0.5
cat > $SCRIPT <<EOF1
#!/bin/bash

CLUSTER=\$1
PROC=\$2
ROOT_LIST="rootList_\${CLUSTER}_\${PROC}.txt"
MERGED_ROOT="merged_\${CLUSTER}_\${PROC}.root"

echo ""
echo ">>> Host info"
hostname
uname -a

echo ""
echo ">>> VOMS info"
unset  X509_USER_KEY
export X509_USER_KEY=$PROXYFILE_NAME
voms-proxy-info

echo ""
echo ">>> Setting up CMSSW_14_1_4_patch5"
source /cvmfs/cms.cern.ch/cmsset_default.sh
wait
cmsrel CMSSW_14_1_4_patch5
wait
cd CMSSW_14_1_4_patch5/src/
cmsenv
git cms-merge-topic CmsHI:forest_CMSSW_14_1_X
wait
cd ../../
which root
which hadd

echo ""
echo ">>> Setting up directory"
xrdcp -r -f  root://xrootd.cmsaf.mit.edu//store/user/jdlang/MITHIGAnalysis2024 .
wait
ls -l
cd MITHIGAnalysis2024/SampleGeneration/CondorForestReducer_DzeroUPC/
export ProjectBase=../../
export PATH=$ProjectBase/CommonCode/binary/:$PATH
mv ../../../$JOBLIST_NAME .
ls -l

echo ""
echo ">>> Compiling skimmer"
make
wait
sleep 1
if ! [ -f "Execute" ]; then
  echo "ERROR: Unable to compile executable!"
  exit 2
fi

echo ""
echo ">>> Running skimmer"
COUNTER=0
HADD_LIST="${BASENAME}_haddlist.txt"
while read -r ROOT_T2; do
  ROOT_IN="\${ROOT_T2##*/}"
#  ROOT_OUT="temp_skim_\${CLUSTER}\${PROC}_\${COUNTER}.root"
  ROOT_OUT="tempskim_\${CLUSTER}\${PROC}_\${ROOT_IN}"
  xrdcp -f \$ROOT_T2 .
  wait
  if ! [ -f "\$ROOT_IN" ]; then
    echo "--- ERROR! Missing root file: \$ROOT_IN"
    continue
  fi
  echo "--- Processing input \${COUNTER}: \$ROOT_IN"
  ./Execute --Input \$ROOT_IN \\
    --Output \$ROOT_OUT \\
    --Year 2024 \\
    --IsData true \\
    --ZDCTree zdcanalyzer/zdcrechit \\
    --ApplyTriggerRejection false \\
    --ApplyEventRejection true \\
    --ApplyZDCGapRejection true \\
    --ApplyDRejection true \\
    --ZDCMinus1nThreshold 900 \\
    --ZDCPlus1nThreshold 900 \\
    --PFTree particleFlowAnalyser/pftree
  wait
  
  echo \$ROOT_OUT >> \$HADD_LIST
  COUNTER=\$((COUNTER + 1))
  rm \$ROOT_IN
  wait
done < $JOBLIST_NAME

echo ""
echo ">>> Completed \$COUNTER jobs!"
ls -lh

echo ""
echo ">>> Merging root files"
hadd -ff \$MERGED_ROOT @\$HADD_LIST

echo ""
echo ">>> Transferring merged root file to T2"
xrdfs $XROOTD_SERVER rm $T2_OUTPUT
sleep 1
xrdcp -f \$MERGED_ROOT $XROOTD_SERVER/$T2_OUTPUT
wait

# Check if copying over worked. If not, then try again
T2SIZE=$(xrdfs $XROOTD_SERVER ls -l $T2_OUTPUT | awk '{print $4}')
wait
LOCALSIZE=$(ls -l \$MERGED_ROOT | awk '{print $5}')
if ! (( \$T2SIZE - \$LOCALSIZE )); then
  xrdcp -f \$MERGED_ROOT $XROOTD_SERVER/$T2_OUTPUT
  wait
  sleep 300
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
request_disk            = 10GB
request_memory          = 5GB
initialdir              = $PWD/
executable              = $PWD/$SCRIPT
arguments               = \$(ClusterId) \$(ProcId)
use_x509userproxy       = True
x509userproxy           = $PROXYFILE
+AccountingGroup        = "analysis.$USER"
max_retries             = 1

### File transfer
should_transfer_files   = YES
transfer_input_files    = $JOBLIST
#when_to_transfer_output = ON_EXIT_OR_EVICT
MAX_TRANSFER_INPUT_MB   = 400
#on_exit_remove          = (ExitBySignal == False) && (ExitCode == 0)

### Logging
notification            = Error
#output                  = ${CONFIGDIR}/${BASENAME}.out
#error                   = ${CONFIGDIR}/${BASENAME}.err
#log                     = ${CONFIGDIR}/${BASENAME}.log
output                  = ${CONFIGDIR}/${BASENAME}_out_\$(ClusterId)_\$(ProcId).txt
error                   = ${CONFIGDIR}/${BASENAME}_err_\$(ClusterId)_\$(ProcId).txt
log                     = ${CONFIGDIR}/${BASENAME}_log_\$(ClusterId)_\$(ProcId).txt

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
