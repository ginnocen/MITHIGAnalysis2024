rm Execute*
rm -rf ../../CommonCode/binary/
rm -rf ../../CommonCode/library/
rm -rf Output
rm SkimReco.root
rm list.txt
rm -rf output
rm *.txt*
rm SkimReco.root
rm .DS_Store

cd /home/aholterm/CMSSW_13_2_4/src
cmsenv

cd -
echo "CMSSW environment is set up"
cd ../../
source SetupAnalysis.sh
cd CommonCode/
pwd
make
cd ..
cd /home/aholterm/MITHIGAnalysis2024/MainAnalysis/20250226_gccbarMCData
make
rm Skim*.root
