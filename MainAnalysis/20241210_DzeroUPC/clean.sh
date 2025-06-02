rm CorrectedYields
rm ExecuteDzeroUPC
rm MassFit
rm PlotCrossSection
#rm PlotHFDist
#rm PlotRapGapScan
rm -rf ../../CommonCode/binary/
rm -rf ../../CommonCode/library/
rm -rf Output
rm SkimReco.root
rm list.txt
rm -rf output
#rm *.txt*
rm SkimReco.root
rm .DS_Store

#cd /home/$USER/CMSSW_13_2_13/src
cd /home/$USER/CMSSW_14_1_7/src
cmsenv

cd -
echo "CMSSW environment is set up"
cd ../../
source SetupAnalysis.sh
cd CommonCode/
make
cd ..
cd MainAnalysis/20241210_DzeroUPC/
make
rm Skim*.root
