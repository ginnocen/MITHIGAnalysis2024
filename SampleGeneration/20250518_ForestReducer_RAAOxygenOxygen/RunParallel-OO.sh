#!/bin/bash
DATE=$(date +%Y%m%d)

#source clean.sh

MAXCORES=40 # too many parallel cores can cause event loss, increase with caution!
NFILES=50 # files for ppref go 1-999
DOGENLEVEL=0
ISDATA=0
SAMPLETYPE=1 # 0 for HIJING 00, 1 for Starlight SD, 2 for Starlight DD, 4 for HIJING alpha-O, -1 for data
DEBUGMODE=1


NAME="${DATE}_OOparallel_2025"
PATHSAMPLE="/home/bakovacs/UPCAnalysisMIT2024/MITHIGAnalysis2024/SampleGeneration/20250518_ForestReducer_RAAOxygenOxygen/test_paralell"
# set your output directory here
OUTPUT="/home/bakovacs/UPCAnalysisMIT2024/MITHIGAnalysis2024/SampleGeneration/20250518_ForestReducer_RAAOxygenOxygen/test_parallel_out_$NAME"
MERGEDOUTPUT="/home/bakovacs/UPCAnalysisMIT2024/MITHIGAnalysis2024/SampleGeneration/20250518_ForestReducer_RAAOxygenOxygen/test_parallel_out_$NAME.root"
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
for COUNTER in $(seq 1 $NFILES); do
    FILEPATH="${PATHSAMPLE}/SD_parallel_test_${COUNTER}.root"

    echo ./ProcessSingleOOFile.sh $FILEPATH $COUNTER $OUTPUT $DOGENLEVEL $ISDATA $SAMPLETYPE $DEBUGMODE &
    ./ProcessSingleOOFile.sh $FILEPATH $COUNTER $OUTPUT $DOGENLEVEL $ISDATA $SAMPLETYPE $DEBUGMODE &

    wait_for_slot
done
wait

hadd $MERGEDOUTPUT $OUTPUT/output_*.root
echo "All done!"
echo "Merged output file: $MERGEDOUTPUT"
