#!/bin/bash
DATE=$(date +%Y%m%d)

source clean.sh

MAXCORES=1 # too many parallel cores can cause event loss, increase with caution!
NFILES=1 # number of files to cap the processing at, if -1 processess all files
# set =1 for agreed upon debug file for Vipul comparison (we just used his first file in the directory HiForestMiniAOD_1.root), set =999 for full production
INPUT_ON_XRD=1 # set to 1 if input files are on xrd, 0 if they are local
#XRDSERV="root://xrootd.cmsaf.mit.edu/" # mit t2 server
XRDSERV="root://eoscms.cern.ch/" # eos xrootd server, path should start /store/group...

NAME="${DATE}_Skim_ppref2024_all"
PATHSAMPLE="/store/group/phys_heavyions/vpant/ppref2024output/PPRefZeroBiasPlusForward4/crab_ppref2024/250324_080237/0000"
# set your output directory here
OUTPUT="/data00/kdeverea/OOsamples/Skims/output_$NAME"
MERGEDOUTPUT="/data00/kdeverea/OOsamples/Skims/$NAME.root"
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
for FILEPATH in $(xrdfs $XRDSERV ls $PATHSAMPLE | grep 'HiForestMiniAOD'); do

    if [ $NFILES -gt 0 ] && [ $COUNTER -ge $NFILES ]; then
        break
    fi

    if (( $INPUT_ON_XRD == 1 )); then
        echo ./ProcessSingleFile-ppref-xrd.sh $XRDSERV $FILEPATH $COUNTER $OUTPUT $MAXCORES
        ./ProcessSingleFile-ppref-xrd.sh $XRDSERV $FILEPATH $COUNTER $OUTPUT $MAXCORES &
    else
        echo ./ProcessSingleFile-ppref.sh $FILEPATH $COUNTER $OUTPUT
        ./ProcessSingleFile-ppref.sh $FILEPATH $COUNTER $OUTPUT &
    fi

    wait_for_slot
    ((COUNTER++))
done
wait

hadd $MERGEDOUTPUT $OUTPUT/output_*.root
echo "All done!"
echo "Merged output file: $MERGEDOUTPUT"
