#!/bin/bash
DATE=$(date +%Y%m%d)

source clean.sh

MAXCORES=40  # too many parallel cores can cause event loss, increase with caution!
NFILES=-1 # number of files to cap the processing at, if -1 processess all files
ISDATA=1
APPLYTRIGGERREJECTION=1 #  trigger = 0 for no rejection, 1 for ZeroBias, 2 for MinBias
APPLYEVENTREJECTION=1
APPLYTRACKREJECTION=1
REJECTTRACKSBELOWPT=3.0
SAMPLETYPE=-1 # 0 for HIJING 00, 1 for Starlight SD, 2 for Starlight DD, 4 for HIJING alpha-O, -1 for data
INPUT_ON_XRD=1 # 0 for local files, 1 for xrd files
XRDSERV="root://eoscms.cern.ch/" # eos xrootd server, path should start /store/group...

# ============================================================
# NeNe data, PD1
# ============================================================
NAME="${DATE}_Skim_NeNe_IonPhysics0_AllPtV1_0716LynnTest_ZeroBias"
PATHSAMPLE="/eos/cms/store/group/phys_heavyions/xirong/Run3_NeonRAA/PromptForest/IonPhysics0/crab_NeNe_IonPhysics0_AllPtV1_0716LynnTest/250716_180522/0001"

# set your output directory here
OUTPUT="/data00/$USER/OOsamples/Skims/output_$NAME/0001"


# ============================================================
# NeNe MC, private HIJING
# ============================================================
#NAME="${DATE}_Skim_MinBias_Hijing_NeNe_5362GeV"
#PATHSAMPLE="/eos/cms/store/group/phys_heavyions/xirong/Run3_NeonRAA/MCForest/MinBias_Hijing_NeNe_5362GeV/crab_Nene_HIJING_5362GeV_v2/250727_012338/0000"

# set your output directory here
#OUTPUT="/data00/$USER/OOsamples/Skims/output_$NAME/0000"



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
        echo ./ProcessSingleFile-NeNe-xrd.sh $FILEPATH $COUNTER $OUTPUT $ISDATA $APPLYTRIGGERREJECTION $APPLYEVENTREJECTION $APPLYTRACKREJECTION $REJECTTRACKSBELOWPT $SAMPLETYPE $XRDSERV $MAXCORES &
        ./ProcessSingleFile-NeNe-xrd.sh $FILEPATH $COUNTER $OUTPUT $ISDATA $APPLYTRIGGERREJECTION $APPLYEVENTREJECTION $APPLYTRACKREJECTION $REJECTTRACKSBELOWPT $SAMPLETYPE $XRDSERV $MAXCORES &
    else
        echo ./ProcessSingleFile-NeNe.sh $FILEPATH $COUNTER $OUTPUT $ISDATA $APPLYTRIGGERREJECTION $APPLYEVENTREJECTION $APPLYTRACKREJECTION $REJECTTRACKSBELOWPT $SAMPLETYPE &
        ./ProcessSingleFile-NeNe.sh $FILEPATH $COUNTER $OUTPUT $ISDATA $APPLYTRIGGERREJECTION $APPLYEVENTREJECTION $APPLYTRACKREJECTION $REJECTTRACKSBELOWPT $SAMPLETYPE &
    fi

    wait_for_slot
    ((COUNTER++))
done
wait

echo "Processing COMPLETE"
