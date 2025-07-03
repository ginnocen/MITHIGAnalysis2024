THISFOLDER=$(pwd)
rm ChargedHadronRAA
rm ExecuteChargedHadronRAA
rm -rf ../../CommonCode/binary/
rm -rf ../../CommonCode/library/
rm *.root

cd /home/$USER/CMSSW_15_0_6_patch1/src
cmsenv
cd ${THISFOLDER}

cd ../../
source SetupAnalysis.sh
cd CommonCode/
make
echo "Cleaning CommonCode"
cd ..
cd ${THISFOLDER}
make
