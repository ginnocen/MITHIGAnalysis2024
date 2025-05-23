rm Execute
rm filterEvents
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
CURRENTDIR=$PWD
cd /home/aholterm/CMSSW_13_2_4/src
cmsenv

cd -
echo "CMSSW environment is set up"
cd ../../
source SetupAnalysis.sh
cd CommonCode/
make
cd ..
cd $CURRENTDIR
make
rm Skim*.root

rootcling -f DictDict.cxx -c DictTypes.h LinkDef.h
g++ -fPIC -shared DictDict.cxx `root-config --cflags --libs` -o libDict.so

