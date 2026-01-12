rm ExecuteEfficiencies
rm -rf ../../CommonCode/binary/
rm -rf ../../CommonCode/library/
rm -rf Output
rm -rf output
rm *.txt*
rm .DS_Store
mkdir -p plots
CURRENTDIR=$PWD
cd /home/$USER/CMSSW_13_2_4/src
cmsenv

cd -
echo "CMSSW environment is set up"
cd ../../
source SetupAnalysis.sh
cd CommonCode/
make
cd $CURRENTDIR
make

