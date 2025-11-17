#!/bin/bash

# Clean up
source clean.sh
rm *.root
rm *.pdf

OUTPUTDIR="data_default"
mkdir -p ${OUTPUTDIR}

# Define arrays for JETPTMIN and JETPTMAX
JETPTMIN_VALUES=(60 80 100 120 160 200 240 280)
JETPTMAX_VALUES=(80 100 120 160 200 240 280 360)

# Ensure arrays have the same length
if [ "${#JETPTMIN_VALUES[@]}" -ne "${#JETPTMAX_VALUES[@]}" ]; then
    echo "Error: JETPTMIN and JETPTMAX arrays must have the same length."
    exit 1
fi

# Input ROOT file
#INPUT=/data00/g2ccbar/mc2018/skim_102725_all/mergedfile.root
INPUT=/data00/g2ccbar/data2018/skim_110525_0/mergedfile.root

# Loop over indices of the arrays
for i in "${!JETPTMIN_VALUES[@]}"; do
    # Get JETPTMIN and JETPTMAX for the current index
    JETPTMIN=${JETPTMIN_VALUES[i]}
    JETPTMAX=${JETPTMAX_VALUES[i]}

    # Set output filenames and labels
    OUTPUT="${OUTPUTDIR}/output_DoubleHQtagging_${JETPTMIN}_${JETPTMAX}.root"
    
    # Run the entire job block in background - this ensures proper sequencing within each job
    {
        echo "Starting job for pT range: ${JETPTMIN}-${JETPTMAX} GeV"
        
        ./ExecuteDoubleHQtagging \
         --Input $INPUT \
         --IsData 1 \
         --IsPP 1 \
         --MinJetPT $JETPTMIN \
         --MaxJetPT $JETPTMAX \
         --ChargeSelection -1 \
         --DCAString "abs(muDiDxy1 / muDiDxy1Err) > 2.78 && abs(muDiDxy2 / muDiDxy2Err) > 2.78" \
         --Output ${OUTPUT}

        echo "Finished ExecuteDoubleHQtagging for pT range: ${JETPTMIN}-${JETPTMAX} GeV"

        ./MassFit --Input ${OUTPUT} --Output ${OUTPUTDIR}/massfit_${JETPTMIN}_${JETPTMAX}.root --Label ${OUTPUTDIR}/massfit_${JETPTMIN}_${JETPTMAX} --MinJetPT $JETPTMIN --MaxJetPT $JETPTMAX
        echo "Finished MassFit for pT range: ${JETPTMIN}-${JETPTMAX} GeV"
        
        root -l -b -q "plotter.C(\"${OUTPUT}\",\"${OUTPUTDIR}\")"
        echo "Completed all tasks for pT range: ${JETPTMIN}-${JETPTMAX} GeV"
        
    } &

done
wait

root -l -b -q "result.C(\"${OUTPUTDIR}\",\"${OUTPUT}\")"

cd ${OUTPUTDIR}
mkdir -p plots
mv *.pdf plots/
cd ..

echo "All jobs are completed!"
