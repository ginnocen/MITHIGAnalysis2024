#!/bin/bash
DATE=$(date +%Y%m%d)

# =============================================================================
# SKIM FOR: LAMBDA_C -> P + K + PI

# Input settings:
YEAR=2023
ISDATA=false
TAG="LambdaCpkpi_2023MC_gN-PhotonA_Pthat2"
FILELIST="filelist_${TAG}.txt"
INPUT_ON_XRD=0
#XRDSERV="root://eoscms.cern.ch/"
XRDSERV="root://xrootd.cmsaf.mit.edu/"
MAXCORES=40

# Output Settings:
OUTPUT="/data00/jdlang/UPCLambdaCAnalysis/SkimsMC/output_${TAG}_${DATE}"
MERGEDOUTPUT="/data00/jdlang/UPCLambdaCAnalysis/SkimsMC/${TAG}_${DATE}.root"

# Skim/Rejection Settings:
TRIG_REJECT=2
EVENT_REJECT=true
ZDCGAP_REJECT=true
D_REJECT=no

# =============================================================================

# Automatically set ZDC thresholds by year
if [[ $YEAR -eq 2023 ]]; then
    ZDCM_THRESH=1000
    ZDCP_THRESH=1100
elif [[ $YEAR -eq 2024 ]]; then
    ZDCM_THRESH=1000
    ZDCP_THRESH=1100
elif [[ $YEAR -eq 2025 ]]; then
    ZDCM_THRESH=1000
    ZDCP_THRESH=1100
fi

# Function to monitor active processes
wait_for_slot() {
    while (( $(jobs -r | wc -l) >= MAXCORES )); do
        # Wait a bit before checking again
        sleep 1
    done
}

# Check if the filelist is empty
if [[ ! -s "$FILELIST" ]]; then
    echo "No matching files found in Samples directory."
    exit 1
fi

echo "File list: $FILELIST"
rm $MERGEDOUTPUT &> /dev/null
rm -rf $OUTPUT &> /dev/null
mkdir -p $OUTPUT
# Loop through each file in the file list
COUNTER=1
while IFS= read -r FILEPATH; do
    if (( $INPUT_ON_XRD == 1 )); then
        ./ProcessSkim_XRD.sh Execute_Lcpkpi $SERVER \
            $FILEPATH $COUNTER $OUTPUT $YEAR $ISDATA \
            $TRIG_REJECT $EVENT_REJECT $ZDCGAP_REJECT $D_REJECT \
            $ZDCM_THRESH $ZDCP_THRESH $MAXCORES &
    else
        ./ProcessSkim_Local.sh Execute_Lcpkpi \
            $FILEPATH $COUNTER $OUTPUT $YEAR $ISDATA \
            $TRIG_REJECT $EVENT_REJECT $ZDCGAP_REJECT $D_REJECT \
            $ZDCM_THRESH $ZDCP_THRESH &
    fi
    ((COUNTER++))
    wait_for_slot
done < "$FILELIST"
wait 

hadd $MERGEDOUTPUT $OUTPUT/output_*.root
echo "All done!"
echo "Merged output file: $MERGEDOUTPUT"
