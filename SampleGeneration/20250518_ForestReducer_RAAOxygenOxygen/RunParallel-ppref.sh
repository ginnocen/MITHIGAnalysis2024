#!/bin/bash
DATE=$(date +%Y%m%d)


# NOT FULLY TESTED, USE WITH CAUTION!

source clean.sh

MAXCORES=40  # too many parallel cores can cause event loss, increase with caution!
NFILES=1 # number of files to cap the processing at, if -1 processess all files
ISDATA=1
APPLYTRIGGERREJECTION=1 #  trigger = 0 for no rejection, 1 for ZeroBias, 2 for MinBias
APPLYEVENTREJECTION=1
APPLYTRACKREJECTION=1
SAMPLETYPE=-1 # 0 for HIJING 00, 1 for Starlight SD, 2 for Starlight DD, 4 for HIJING alpha-O, -1 for data
XRDSERV="root://eoscms.cern.ch/" # eos xrootd server, path should start /store/group...


# ============================================================
# pp data, low pT PD
# ============================================================
NAME="${DATE}_Skim_ppref2024_data"
PATHSAMPLE="/store/group/phys_heavyions/vpant/ppref2024output/PPRefZeroBiasPlusForward4/crab_ppref2024/250324_080237/0001"

# set your output directory here
OUTPUT="/data00/$USER/OOsamples/Skims/output_$NAME/0001"

# ============================================================
# pp MC, official minbias
# ============================================================
#NAME="${DATE}_Skim_ppref2024_MinBias_TuneCP5_5p36TeV-pythia8_noEvtSel"
#PATHSAMPLE="/eos/cms/store/group/phys_heavyions/kdeverea/MinBias_TuneCP5_5p36TeV-pythia8/MinBias_TuneCP5_5p36TeV-pythia8/crab_MinBias_TuneCP5_5p36TeV-pythia8/250726_203015/0001"

# set your output directory here
#OUTPUT="/data00/$USER/OOsamples/Skims/output_$NAME/0001"



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
for FILEPATH in $(xrdfs $XRDSERV ls $PATHSAMPLE | grep 'HiForestMiniAOD'); do

    if [ $NFILES -gt 0 ] && [ $COUNTER -ge $NFILES ]; then
        break
    fi

    echo ./ProcessSingleFile-ppref-xrd.sh $FILEPATH $COUNTER $OUTPUT $ISDATA $APPLYTRIGGERREJECTION $APPLYEVENTREJECTION $APPLYTRACKREJECTION $SAMPLETYPE $XRDSERV $MAXCORES &
    ./ProcessSingleFile-ppref-xrd.sh $FILEPATH $COUNTER $OUTPUT $ISDATA $APPLYTRIGGERREJECTION $APPLYEVENTREJECTION $APPLYTRACKREJECTION $SAMPLETYPE $XRDSERV $MAXCORES &

    wait_for_slot
    ((COUNTER++))
done
wait

echo "Processing COMPLETE"
