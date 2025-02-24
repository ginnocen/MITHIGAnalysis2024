#!/bin/bash

# Clean up
source clean.sh
rm *.root
rm *.pdf
rm -rf output/
mkdir output/

# Define arrays for JETPTMIN and JETPTMAX
JETPTMIN_VALUES=(40 50 60 70 80 120)
JETPTMAX_VALUES=(50 60 70 80 120 160)

# Ensure arrays have the same length
if [ "${#JETPTMIN_VALUES[@]}" -ne "${#JETPTMAX_VALUES[@]}" ]; then
    echo "Error: JETPTMIN and JETPTMAX arrays must have the same length."
    exit 1
fi

# Input ROOT file
INPUT=/data/NewSkims_gtoccbar/20241113_DataPbPb2018gtoccbar_v2_TestV0.root

# Loop over indices of the arrays
for i in "${!JETPTMIN_VALUES[@]}"; do
    # Get JETPTMIN and JETPTMAX for the current index
    JETPTMIN=${JETPTMIN_VALUES[i]}
    JETPTMAX=${JETPTMAX_VALUES[i]}

    # Set output filenames and labels
    OUTPUT="output/output_analysis_${JETPTMIN}_${JETPTMAX}.root"
    OUTPUTFIT="output/output_fit_${JETPTMIN}_${JETPTMAX}.root"
    LABEL="JetPtMin${JETPTMIN}JetPtMax${JETPTMAX}"

    # Print the output file being processed
    echo "Processing JetPtMin=$JETPTMIN and JetPtMax=$JETPTMAX"
    echo "Output File: $OUTPUT"

    # Run the main analysis
    ./ExecuteMuMuJet --Input "$INPUT" --MinJetPT "$JETPTMIN" --MaxJetPT "$JETPTMAX" --IsData false --Output "$OUTPUT" &
done
wait

for i in "${!JETPTMIN_VALUES[@]}"; do
    # Get JETPTMIN and JETPTMAX for the current index
    JETPTMIN=${JETPTMIN_VALUES[i]}
    JETPTMAX=${JETPTMAX_VALUES[i]}

    # Set output filenames and labels
    OUTPUT="output/output_analysis_${JETPTMIN}_${JETPTMAX}.root"
    OUTPUTFIT="output/output_fit_${JETPTMIN}_${JETPTMAX}.root"
    LABEL="JetPtMin${JETPTMIN}JetPtMax${JETPTMAX}"

    # Print the output file being processed
    echo "Processing JetPtMin=$JETPTMIN and JetPtMax=$JETPTMAX"
    echo "Output File: $OUTPUT"
    
    # Run the mass fit
    ./MassFit --MinJetPT $JETPTMIN --MaxJetPT $JETPTMAX --Input $OUTPUT --Output $OUTPUTFIT --Label $LABEL
    wait
done
