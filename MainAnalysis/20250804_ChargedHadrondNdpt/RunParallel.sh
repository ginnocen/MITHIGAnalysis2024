#!/bin/bash
MAXCORES=40

# Set output directory and input list
OUTPUTDIR="output_parallel"
INPUTLIST="OO_InputFileList.txt"  # You must create this list file
MERGEDOUTPUT="${OUTPUTDIR}/MergedOutput.root"
rm -f "$MERGEDOUTPUT"

# Clean output directory
rm -rf "$OUTPUTDIR"
mkdir -p "$OUTPUTDIR"

# Function to limit number of concurrent jobs
wait_for_slot() {
    while (( $(jobs -r | wc -l) >= MAXCORES )); do
        sleep 1
    done
}

# Check input file list
if [[ ! -s "$INPUTLIST" ]]; then
    echo "Error: Input list '$INPUTLIST' is empty or missing."
    exit 1
fi

# Loop over files and launch parallel jobs
counter=0
while IFS= read -r INPUT; do
    OUTPUTFILE="${OUTPUTDIR}/output_${counter}.root"
    echo "Launching job for $INPUT -> $OUTPUTFILE"

    ./ExecuteChargedHadrondNdpt \
      --Input "$INPUT" \
      --Output "$OUTPUTFILE" \
      --IsData true \
      --ApplyEventSelection true \
      --UseSpeciesWeight false \
      --UseEventWeight false \
      --UseTrackWeight true \
      --TrackWeightSelection 2 \
      --MinTrackPt 0.4 \
      --MinLeadingTrackPt -1 \
      --TriggerChoice 1 \
      --CollisionSystem true \
      --EventSelectionOption 1 \
      --SpeciesCorrectionOption 0 \
      --ScaleFactor 1 &

    ((counter++))
    wait_for_slot
done < "$INPUTLIST"

# Wait for all jobs to complete
wait

# Merge outputs
echo "Merging outputs to $MERGEDOUTPUT..."
hadd "$MERGEDOUTPUT" "$OUTPUTDIR"/output_*.root

echo "All done!"
echo "Merged output file: $MERGEDOUTPUT"

