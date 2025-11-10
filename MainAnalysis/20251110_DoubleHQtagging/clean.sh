rm ExecuteDoubleHQtagging
rm -rf ../../CommonCode/binary/
rm -rf ../../CommonCode/library/
rm -rf Output
rm SkimReco.root
rm list.txt
rm -rf output
rm *.txt*
rm SkimReco.root
rm .DS_Store
CURRENTDIR=$PWD
cd /home/ginnocen/CMSSW_13_2_4/src
cmsenv

cd -
echo "CMSSW environment is set up"
cd ../../
source SetupAnalysis.sh
cd CommonCode/
make
cd $CURRENTDIR
make
rm Skim*.root
