THISFOLDER=$(pwd)
rm ChargedHadronRAA
rm ExecuteChargedHadronRAA
rm -rf ../../CommonCode/binary/
rm -rf ../../CommonCode/library/

cd ../../
source SetupAnalysis.sh
cd CommonCode/
make
echo "Cleaning CommonCode"
cd ..
cd ${THISFOLDER}
make
