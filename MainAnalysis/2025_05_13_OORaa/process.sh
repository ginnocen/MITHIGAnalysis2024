#!/bin/bash
#

GREEN='\033[0;32m'
RED='\033[0;31m'
RESET='\033[0m'

#to clang: clang-format -style=file -i cross.C
# File containing list of files
#file_list="list_skim_03JuneData.txt"
current_dir=$(pwd)

g++ process.C -o process.exe `root-config --cflags --glibs`
./process.exe "inputs/crab_HiForest_250513_Hijing_ppreco_1500pre2_250507.root" "outputHijing.root"
./process.exe "inputs/crab_HiForest_250513_Starlight_SingleDiffraction_OO_5362GeV_1505PR47944.root" "outputSLSD.root"
