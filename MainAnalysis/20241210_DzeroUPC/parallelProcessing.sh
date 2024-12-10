#!/bin/bash

# Number of threads per instance
nThread=10

# Array to hold the names of the output files
declare -a outputFileNames

# Loop over the number of chunks
for (( chunk=1; chunk<=nThread; chunk++ ))
do
    outputFileName="$1-$2_${chunk}.root"
    outputFileNames+=($outputFileName)
    echo "Starting analysis of chunk $chunk"
    echo ./ExecuteDzeroUPC $3 $4 $5 $6 $7 $8 $9 ${10} ${11} ${12} ${13} ${14} ${15} ${16} ${17} ${18} ${19} ${20} ${21} ${22} ${23} ${24} ${25} ${26} ${27} ${28} ${29} ${30} --nThread $nThread --nChunk $chunk --Output $outputFileName &
    ./ExecuteDzeroUPC $3 $4 $5 $6 $7 $8 $9 ${10} ${11} ${12} ${13} ${14} ${15} ${16} ${17} ${18} ${19} ${20} ${21} ${22} ${23} ${24} ${25} ${26} ${27} ${28} ${29} ${30} --nThread $nThread --nChunk $chunk --Output $outputFileName &
done

# Wait for all background processes to finish
wait

# Merge the output files into a single file
echo "Merging output files into $1-$2.root"
hadd -f $1-$2.root "${outputFileNames[@]}"
# Remove the individual chunk files
echo "Cleaning up individual chunk files..."
for fileName in "${outputFileNames[@]}"
do
    rm -f $fileName
done
