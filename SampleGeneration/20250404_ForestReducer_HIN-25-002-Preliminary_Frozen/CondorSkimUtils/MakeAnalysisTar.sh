#!/bin/bash
# MakeAnalysisTar.sh
#
# Call this from a ForestReducer directory:
#     bash ../20231203_CondorSkimUtils/MakeAnalysisTar.sh

TAR_DIR="MITHIGAnalysis2024"

rm -rf $TAR_DIR

mkdir -p $TAR_DIR
cp $ProjectBase/SetupAnalysis.sh $TAR_DIR/
cp -r $ProjectBase/CommonCode $TAR_DIR/

REDUCE_FOREST_DIR=SampleGeneration/$(basename $PWD)
mkdir -p $TAR_DIR/$REDUCE_FOREST_DIR

cp ReduceForest.cpp $TAR_DIR/$REDUCE_FOREST_DIR/
cp makefile $TAR_DIR/$REDUCE_FOREST_DIR/
cp -r include $TAR_DIR/$REDUCE_FOREST_DIR/

tar cf MITHIGAnalysis2024.tar $TAR_DIR

rm -rf $TAR_DIR
