###### PLEASE CONFIGURE THESE PARAMETERS #####
SETUPFILE=/home/yuchenc/pythia8311/setup-pythia8.sh #contains env. variables
COMPILER=compile_pythia_janice0314.sh
echo "----------------------------------"
echo "----------------------------------"
echo "----------------------------------"
echo $CASE
echo "----------------------------------"
echo "----------------------------------"
echo "----------------------------------"

###### 
source clean.sh
rm *.root *.exe
cp ../../codeforskim/paramfolder/param.h .
source $SETUPFILE
./$COMPILER test70
./test70.exe

