CMSFOLDER=$HOME/CMSSW_14_1_4_patch5/src
WORKFOLDER=$PWD

if [ ! -d $CMSFOLDER ]; then
    echo "You need to define the folder where the CMSSW environment is located"
    exit 1
else
    echo "CMSSW environment is located at $CMSFOLDER"
fi

rm Execute
rm -rf ../../CommonCode/binary/
rm -rf ../../CommonCode/library/
rm MergedOutput.root
rm -rf Output
rm SkimReco.root
rm list.txt
rm -rf output
rm *.txt*
rm SkimReco.root
rm .DS_Store

source /cvmfs/cms.cern.ch/cmsset_default.sh
cd $CMSFOLDER
cmsenv
echo "CMSSW environment is set up"

cd $WORKFOLDER
cd ../../
source SetupAnalysis.sh
cd CommonCode/
make
cd ..
cd $WORKFOLDER
make
rm skim*.root
