#!/bin/bash

CPPFILE=$PWD/cpptest.cc

# Output location on EOS
T2PATH=/store/$USER/

PROXYFILE=$HOME/$(ls $HOME -lt | grep $USER | grep -m 1 x509 | awk '{print $NF}')

LOGDIR=$PWD/logs
mkdir -p $LOGDIR

cat > cpptest.condor <<EOF

### Job settings
Universe                = vanilla
request_disk            = 3GB
request_memory          = 3GB
initialdir              = $PWD/
executable              = $PWD/cpptest.sh
arguments               = $T2PATH
use_x509userproxy       = True
x509userproxy           = $PROXYFILE
+AccountingGroup        = "analysis.$USER"
max_retries             = 1

### File transfer
should_transfer_files   = YES
transfer_input_files    = $CPPFILE
when_to_transfer_output = ON_EXIT_OR_EVICT
MAX_TRANSFER_INPUT_MB   = 400
on_exit_remove          = (ExitBySignal == False) && (ExitCode == 0)

### Logging
notification            = Error
output                  = $LOGDIR/\$(ClusterId).\$(ProcId).out
error                   = $LOGDIR/\$(ClusterId).\$(ProcId).err
log                     = $LOGDIR/\$(ClusterId).\$(ProcId).log

### Server settings
Requirements            = ( BOSCOCluster =!= "t3serv008.mit.edu" && BOSCOCluster =!= "ce03.cmsaf.mit.edu" && BOSCOCluster =!= "eofe8.mit.edu")
+SingularityImage       = "/cvmfs/unpacked.cern.ch/registry.hub.docker.com/cmssw/el9:x86_64-d20241130"
+DESIRED_Sites          = "mit_tier2,mit_tier3,T2_AT_Vienna,T2_BE_IIHE,T2_BE_UCL,T2_BR_SPRACE,T2_BR_UERJ,T2_CH_CERN,T2_CH_CERN_AI,T2_CH_CERN_HLT,T2_CH_CERN_Wigner,T2_CH_CSCS,T2_CH_CSCS_HPC,T2_CN_Beijing,T2_DE_DESY,T2_DE_RWTH,T2_EE_Estonia,T2_ES_CIEMAT,T2_ES_IFCA,T2_FI_HIP,T2_FR_CCIN2P3,T2_FR_GRIF_IRFU,T2_FR_GRIF_LLR,T2_FR_IPHC,T2_GR_Ioannina,T2_HU_Budapest,T2_IN_TIFR,T2_IT_Bari,T2_IT_Legnaro,T2_IT_Pisa,T2_IT_Rome,T2_KR_KISTI,T2_MY_SIFIR,T2_MY_UPM_BIRUNI,T2_PK_NCP,T2_PL_Swierk,T2_PL_Warsaw,T2_PT_NCG_Lisbon,T2_RU_IHEP,T2_RU_INR,T2_RU_ITEP,T2_RU_JINR,T2_RU_PNPI,T2_RU_SINP,T2_TH_CUNSTDA,T2_TR_METU,T2_TW_NCHC,T2_UA_KIPT,T2_UK_London_IC,T2_UK_SGrid_Bristol,T2_UK_SGrid_RALPP,T2_US_Caltech,T2_US_Florida,T2_US_Nebraska,T2_US_Purdue,T2_US_UCSD,T2_US_Vanderbilt,T2_US_Wisconsin,T3_CH_CERN_CAF,T3_CH_CERN_DOMA,T3_CH_CERN_HelixNebula,T3_CH_CERN_HelixNebula_REHA,T3_CH_CMSAtHome,T3_CH_Volunteer,T3_US_HEPCloud,T3_US_NERSC,T3_US_OSG,T3_US_PSC,T3_US_SDSC,T3_US_MIT"

queue 1
EOF

condor_submit cpptest.condor
