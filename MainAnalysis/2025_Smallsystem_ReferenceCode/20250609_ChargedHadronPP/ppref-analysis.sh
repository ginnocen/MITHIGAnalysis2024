#!/bin/bash

# ============================================================
# OO data, parallelized main analysis
# ============================================================
INPUT=/data00/kdeverea/OOsamples/Skims/output_20250723_Skim_ppref2024_Data_noEvtSel/0000
OUTPUTANALYSIS=output/output_20250723_Skim_ppref2024_Data_noEvtSel_MCDrivenCorrection
MERGEDOUTPUT=output/output_20250723_Skim_ppref2024_Data_noEvtSel_MCDrivenCorrection.root

MAXCORES=30 # too many parallel cores can cause event loss, increase with caution!
NFILES=-1 # number of files to cap the processing at, if -1 processess all files
# set =1 for just first file, =2 for first two files, etc.

source clean.sh
mkdir -p $OUTPUTANALYSIS

# Function to monitor active processes
wait_for_slot() {
    while (( $(jobs -r | wc -l) >= MAXCORES )); do
        # Wait a bit before checking again
        sleep 1
    done
}

# Loop through each file in the file list
COUNTER=0
for FILENAME in $(ls $INPUT | grep 'output'); do

  if [ $NFILES -gt 0 ] && [ $COUNTER -ge $NFILES ]; then
    break
  fi

  FILEPATH="$INPUT/$FILENAME"
  outname=$(basename "$FILEPATH")

  ./ExecuteChargedHadronRAA \
    --Input $FILEPATH \
    --Output $OUTPUTANALYSIS/$outname \
    --IsData true \
    --IsPP true \
    --UseTrackWeight false \
    --ApplyEventSelection true \
    --TrackWeightSelection 2 \
    --MinTrackPt 0.4 \
    --MinLeadingTrackPt -1 \
    --ScaleFactor 1 \
    --UseEventWeight true \
    --EventCorrectionFile plots/20250723_Skim_ppref2024_Official_noEvtSel_EventCorrection.root \
    &

  wait_for_slot
  ((COUNTER++))
done
wait

hadd -f $MERGEDOUTPUT $OUTPUTANALYSIS/output_*.root
echo "All done!"
echo "Merged output file: $MERGEDOUTPUT"
