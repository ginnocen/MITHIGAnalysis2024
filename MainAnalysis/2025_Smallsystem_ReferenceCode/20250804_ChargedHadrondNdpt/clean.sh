THISFOLDER=$(pwd)
rm ChargedHadrondNdpt
rm ExecuteChargedHadrondNdpt
rm -rf ../../CommonCode/binary/
rm -rf ../../CommonCode/library/
rm -rf output
mkdir output
cd ../../
source SetupAnalysis.sh
cd CommonCode/
make
echo "Cleaning CommonCode"
cd ..
cd ${THISFOLDER}
make
