#!/bin/bash
DATE=$(date +%Y%m%d)

source clean.sh

MAXCORES=40  # too many parallel cores can cause event loss, increase with caution!
NFILES=1 # number of files to cap the processing at, if -1 processess all files
DOGENLEVEL=0
ISDATA=1
SAMPLETYPE=-1 # 0 for HIJING 00, 1 for Starlight SD, 2 for Starlight DD, 4 for HIJING alpha-O, -1 for data
SAVETRIGGERBITS=1 # 0 for not HLT saved, 1 for HLT OO, 2 for HLT pO
DEBUGMODE=1
INCLUDEPPSANDFSC=0
INCLUDEPF=0

INPUT_ON_XRD=1 # set to 1 if input files are on xrd, 0 if they are local
#XRDSERV="root://xrootd.cmsaf.mit.edu/" # mit t2 server
XRDSERV="root://eoscms.cern.ch/" # eos xrootd server, path should start /store/group...

# ============================================================
# OO data, low pT PD
# ============================================================
NAME="${DATE}_Skim_OO_IonPhysics0_LowPtV2_250711_104114_test"
PATHSAMPLE="/store/group/phys_heavyions/jdlang/Run3_OxygenRAA/PromptForest/IonPhysics0/crab_OO_IonPhysics0_LowPtV2/250711_104114/0000"

# set your output directory here
OUTPUT="/data00/kdeverea/OOsamples/Skims/output_$NAME/0004"
MERGEDOUTPUT="/data00/kdeverea/OOsamples/Skims/output_$NAME/${NAME}_0004.root"


# ============================================================
# OO data, high pT PD
# ============================================================
#NAME="${DATE}_Skim_OO_IonPhysics5_HighPtV2_250711_104159_40files"
#PATHSAMPLE="/store/group/phys_heavyions/jdlang/Run3_OxygenRAA/PromptForest/IonPhysics5/crab_OO_IonPhysics5_HighPtV2/250711_104159/0000"

# set your output directory here
#OUTPUT="/data00/kdeverea/OOsamples/Skims/output_$NAME/0000"
#MERGEDOUTPUT="/data00/kdeverea/OOsamples/Skims/output_$NAME/0000.root"



rm $MERGEDOUTPUT &> /dev/null

# Function to monitor active processes
wait_for_slot() {
    while (( $(jobs -r | wc -l) >= MAXCORES )); do
        # Wait a bit before checking again
        sleep 1
    done
}

echo "Forest sample path: $PATHSAMPLE"
rm -rf $OUTPUT &> /dev/null
mkdir -p $OUTPUT

# Loop through each file in the file list
COUNTER=0
for FILEPATH in $(xrdfs $XRDSERV ls $PATHSAMPLE | grep 'HiForest'); do

    if [ $NFILES -gt 0 ] && [ $COUNTER -ge $NFILES ]; then
        break
    fi

    if (( $INPUT_ON_XRD == 1 )); then
        echo ./ProcessSingleFile-OO-xrd.sh "$FILEPATH" $COUNTER $OUTPUT $DOGENLEVEL $ISDATA $SAMPLETYPE $SAVETRIGGERBITS $DEBUGMODE $INCLUDEPPSANDFSC $INCLUDEPF $XRDSERV $MAXCORES &
        ./ProcessSingleFile-OO-xrd.sh "$FILEPATH" $COUNTER $OUTPUT $DOGENLEVEL $ISDATA $SAMPLETYPE $SAVETRIGGERBITS $DEBUGMODE $INCLUDEPPSANDFSC $INCLUDEPF $XRDSERV $MAXCORES &
    else
        echo ./ProcessSingleFile-OO.sh "$FILEPATH" $COUNTER $OUTPUT $DOGENLEVEL $ISDATA $SAMPLETYPE $SAVETRIGGERBITS $DEBUGMODE $INCLUDEPPSANDFSC $INCLUDEPF &
        ./ProcessSingleFile-OO.sh "$FILEPATH" $COUNTER $OUTPUT $DOGENLEVEL $ISDATA $SAMPLETYPE $SAVETRIGGERBITS $DEBUGMODE $INCLUDEPPSANDFSC $INCLUDEPF &
    fi

    wait_for_slot
    ((COUNTER++))
done
wait

hadd $MERGEDOUTPUT $OUTPUT/output_*.root
echo "All done!"
echo "Merged output file: $MERGEDOUTPUT"
