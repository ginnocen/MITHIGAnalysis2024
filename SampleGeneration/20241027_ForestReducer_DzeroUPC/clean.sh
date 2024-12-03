CMSFOLDER=/home/$USER/CMSSW_13_2_4/src

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

cd $CMSFOLDER
cmsenv

cd -
echo "CMSSW environment is set up"
cd ../../
source SetupAnalysis.sh
cd CommonCode/
make
cd ..
cd SampleGeneration/20241027_ForestReducer_DzeroUPC
make
rm Skim*.root
