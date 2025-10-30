#!/bin/bash

echo "Running RunParallelData.sh for 20 different samples..."

# Loop through samples 0 to 19
for i in {0..19}; do
    echo "========================================="
    echo "Running sample $i..."
    echo "========================================="
    
    # Create a temporary modified version of RunParallelData.sh
    cp RunParallelData.sh RunParallelData_temp.sh
    
    # Replace SAMPLEID=0 with current sample number
    sed -i "s/SAMPLEID=0/SAMPLEID=$i/" RunParallelData_temp.sh
    
    # Replace the if condition to handle all sample IDs
    sed -i "s/if \[ \"\$SAMPLEID\" -eq 0 \]; then/if [ \"\$SAMPLEID\" -eq $i ]; then/" RunParallelData_temp.sh
    
    # Replace the NAMEData path with current sample number
    sed -i "s|NAMEData=\"/data00/g2ccbar/data2018/skim_09232025_DiJet_pThat-15_TuneCP5_HydjetDrumMB_5p02TeV_Pythia8_0/\"|NAMEData=\"/data00/g2ccbar/data2018/skim_09232025_DiJet_pThat-15_TuneCP5_HydjetDrumMB_5p02TeV_Pythia8_${i}/\"|" RunParallelData_temp.sh
    
    # Replace the FOLDER path with current sample number
    sed -i "s|FOLDER=\"/data00/g2ccbar/data2018/minBiasForest_02022025/mb0\"|FOLDER=\"/data00/g2ccbar/data2018/minBiasForest_02022025/mb${i}\"|" RunParallelData_temp.sh
    
    # Run the modified script
    bash RunParallelData_temp.sh
    
    # Check if the run was successful
    if [ $? -eq 0 ]; then
        echo "Sample $i completed successfully"
    else
        echo "ERROR: Sample $i failed!"
    fi
    
    # Clean up temporary file
    rm RunParallelData_temp.sh
    
    echo "Finished sample $i"
    echo ""
done

echo "All 20 samples completed!"
