USEONMITHI03=true
CMSFOLDER=/home/$USER/CMSSW_13_2_4/src
CMSSUBMIT=/home/submit/$USER/CMSSW_14_1_7/src

if [ -d "$CMSSUBMIT" ]; then
    CMSFOLDER=$CMSSUBMIT
fi

CURRENTFOLDER=$(pwd)

if [ "$USEONMITHI03" = true ]; then
    if [ ! -d "$CMSFOLDER" ]; then
        echo "You need to define the folder where the CMSSW environment is located"
        kill -INT $$
    else
        echo "CMSSW environment is located at $CMSFOLDER"
    fi
fi

rm -f Execute
rm -rf ../../CommonCode/binary/
rm -rf ../../CommonCode/library/
rm -f MergedOutput.root
rm -rf Output
rm -f skim_*.root
rm -f list.txt
rm -rf output
#rm *.txt*  # You may want to uncomment this or handle separately
rm -f SkimReco.root
rm -f .DS_Store

if [ "$USEONMITHI03" = true ]; then
    cd "$CMSFOLDER"
    cmsenv
    echo "CMSSW environment is set up"
cd -
fi


cd ../../
source SetupAnalysis.sh
cd CommonCode/
make
cd ..
cd "$CURRENTFOLDER"
make
rm -f skim_*.root
