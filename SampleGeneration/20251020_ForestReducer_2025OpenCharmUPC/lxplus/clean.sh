set -x
CURRENTFOLDER=$PWD

# rm Execute_*
# rm MergedOutput.root
# rm -rf Output
# rm SkimReco.root
# rm list.txt
# rm -rf output
# rm SkimReco.root
# rm .DS_Store

cd ../../
[[ x$ProjectBase == x ]] && source SetupAnalysis.sh
cd CommonCode/
rm -rf binary library
make
cd $CURRENTFOLDER
make

set +x
