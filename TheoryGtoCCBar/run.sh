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
NJOBS=1
###### 
source clean.sh
#cp ../../param.h .
rm *.root *.exe
source $SETUPFILE
./$COMPILER bevents

OUTPUTFOLDERDIR=output
rm -rf $OUTPUTFOLDERDIR
mkdir $OUTPUTFOLDERDIR
cd $OUTPUTFOLDERDIR
RANDOM=640

for i in $( eval echo {1..$NJOBS} )
do
   echo "Running job $i"
   mkdir file_$i
   cd file_$i
   cp ../../bevents.exe .
   echo $RANDOM
   ./bevents.exe $RANDOM &
   cd ..
done
echo "Waiting for all jobs to finish..."

wait
cd ../

#rm output.root
#hadd output.root $OUTPUTFOLDERDIR/file_*/output.root
