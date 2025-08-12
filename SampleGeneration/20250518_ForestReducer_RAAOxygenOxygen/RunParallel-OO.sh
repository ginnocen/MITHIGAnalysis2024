#!/bin/bash
DATE=$(date +%Y%m%d)

source clean.sh

MAXCORES=40  # too many parallel cores can cause event loss, increase with caution!
NFILES=1 # number of files to cap the processing at, if -1 processess all files
ISDATA=1
APPLYTRIGGERREJECTION=0 #  trigger = 0 for no rejection, 1 for ZeroBias, 2 for MinBias
APPLYEVENTREJECTION=0
APPLYTRACKREJECTION=0
REJECTTRACKSBELOWPT=0.4
SAMPLETYPE=-1 # 0 for HIJING 00, 1 for Starlight SD, 2 for Starlight DD, 4 for HIJING alpha-O, -1 for data
INPUT_ON_XRD=1 # 0 for local files, 1 for xrd files
XRDSERV="root://eoscms.cern.ch/" # eos xrootd server, path should start /store/group...

# ============================================================
# OO data, low pT PD
# ============================================================
#NAME="${DATE}_Skim_OO_IonPhysics0_LowPtV2_250711_104114_test"
#PATHSAMPLE="/store/group/phys_heavyions/jdlang/Run3_OxygenRAA/PromptForest/IonPhysics0/crab_OO_IonPhysics0_LowPtV2/250711_104114/0000"

# set your output directory here
#OUTPUT="/data00/$USER/OOsamples/Skims/output_$NAME/0004"


# ============================================================
# OO data, high pT PD
# ============================================================
#NAME="${DATE}_Skim_OO_IonPhysics5_HighPtV2_250711_104159_40files"
#PATHSAMPLE="/store/group/phys_heavyions/jdlang/Run3_OxygenRAA/PromptForest/IonPhysics5/crab_OO_IonPhysics5_HighPtV2/250711_104159/0000"

# set your output directory here
#OUTPUT="/data00/$USER/OOsamples/Skims/output_$NAME/0000"


# ============================================================
# OO HIJING official
# ============================================================
#NAME="${DATE}_Skim_MinBias_OO_5p36TeV_hijing"
#PATHSAMPLE="/eos/cms/store/group/phys_heavyions/xirong/Run3_OxygenRAA/MCForest/MinBias_OO_5p36TeV_hijing/crab_OO_HIJING_5362GeV_new2/250724_211743/0000"

# set your output directory here
#OUTPUT="/data00/$USER/OOsamples/Skims/output_$NAME/0000"


# ============================================================
# OO PYTIHA+HIJING embeded official
# ============================================================
#NAME="${DATE}_Skim_MinBias_OO_5p36TeV_hijingpythia"
#PATHSAMPLE="/eos/cms/store/group/phys_heavyions/kdeverea/Run3_OO_2025MC/QCD-dijet_Pthat-15_TuneCP5_OO_5p36TeV_pythia8/crab_Run3_OO_pythiahijing_official/250807_164006/0001"

# set your output directory here
#OUTPUT="/data00/$USER/OOsamples/Skims/output_$NAME/0001"


# ============================================================
# OO Data, EmptyBX
# ============================================================
NAME="${DATE}_Skim_OO_5p36TeV_EmptyBX"
PATHSAMPLE="/eos/cms/store/group/phys_heavyions/jdlang/Run3_OxygenRAA/PromptForest_EmptyBX/EmptyBX/crab_OO_EmptyBX/250807_200807/0000"

# set your output directory here
OUTPUT="/data00/$USER/OOsamples/Skims/output_$NAME/0000"



# Function to monitor active processes
wait_for_slot() {
    while (( $(jobs -r | wc -l) >= MAXCORES )); do
        # Wait a bit before checking again
        sleep 1
    done
}

rm -rf $OUTPUT &> /dev/null
mkdir -p $OUTPUT

# Loop through each file in the file list
COUNTER=0
for FILEPATH in $(xrdfs $XRDSERV ls $PATHSAMPLE | grep 'HiForest'); do

    if [ $NFILES -gt 0 ] && [ $COUNTER -ge $NFILES ]; then
        break
    fi

    if (( $INPUT_ON_XRD == 1 )); then
        echo ./ProcessSingleFile-OO-xrd.sh $FILEPATH $COUNTER $OUTPUT $ISDATA $APPLYTRIGGERREJECTION $APPLYEVENTREJECTION $APPLYTRACKREJECTION $REJECTTRACKSBELOWPT $SAMPLETYPE $XRDSERV $MAXCORES &
        ./ProcessSingleFile-OO-xrd.sh $FILEPATH $COUNTER $OUTPUT $ISDATA $APPLYTRIGGERREJECTION $APPLYEVENTREJECTION $APPLYTRACKREJECTION $REJECTTRACKSBELOWPT $SAMPLETYPE $XRDSERV $MAXCORES &
    else
        echo ./ProcessSingleFile-OO.sh $FILEPATH $COUNTER $OUTPUT $ISDATA $APPLYTRIGGERREJECTION $APPLYEVENTREJECTION $APPLYTRACKREJECTION $REJECTTRACKSBELOWPT $SAMPLETYPE &
        ./ProcessSingleFile-OO.sh $FILEPATH $COUNTER $OUTPUT $ISDATA $APPLYTRIGGERREJECTION $APPLYEVENTREJECTION $APPLYTRACKREJECTION $REJECTTRACKSBELOWPT $SAMPLETYPE &
    fi

    wait_for_slot
    ((COUNTER++))
done
wait

echo "Processing COMPLETE"
