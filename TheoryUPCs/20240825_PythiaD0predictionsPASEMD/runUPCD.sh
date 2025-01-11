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
NJOBS=100
###### 
source clean.sh
cp ../../param.h .
rm *.root *.exe
source $SETUPFILE
./$COMPILER dmesonUPC

OUTPUTFOLDERDIR=outputnew
rm -rf $OUTPUTFOLDERDIR
mkdir $OUTPUTFOLDERDIR
cd $OUTPUTFOLDERDIR
RANDOM=640

for i in $( eval echo {1..$NJOBS} )
do
   echo "Running job $i"
   mkdir file_$i
   cd file_$i
   cp ../../dmesonUPC.exe .
   echo $RANDOM
   ./dmesonUPC.exe $RANDOM &
   cd ..
done
echo "Waiting for all jobs to finish..."

wait
cd ../

rm output.root
hadd output.root $OUTPUTFOLDERDIR/file_*/output.root
git checkout crosssection_ref_pp.root
git checkout crosssection_ref.root	
root -l -b -q 'plotterdmesonUPC.cc("output.root", "correctedcross.root")'
