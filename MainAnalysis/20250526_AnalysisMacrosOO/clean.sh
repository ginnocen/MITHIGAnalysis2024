THISFOLDER=$(pwd)
rm MainEfficiencyExecutable
rm MainExecutable
rm Execute2DPlotExecutable
rm Executable
rm -rf ../../CommonCode/binary/
rm -rf ../../CommonCode/library/
#rm *.root

cd ../../
source SetupAnalysis.sh
cd CommonCode/
make
echo "Cleaning CommonCode"
cd ..
cd ${THISFOLDER}
make