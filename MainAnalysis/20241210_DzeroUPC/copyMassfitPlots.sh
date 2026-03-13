#!/bin/bash

#filename=first_second_third_requiredText_fourth_fifth_sixth
#removed_first_part=${filename#*_*_*_}
#finalText=${removed_first_part%_*_*_*}

ls fullAnalysis/pt*_y*_IsGammaN*/MassFit/fit_result_full_param.pdf > temp_MassFitPlotList.txt
mkdir -p plot/massFit/full_param

while IFS= read -r FilePath; do
  Kinematics=${FilePath#fullAnalysis/}
  Kinematics=${Kinematics%/MassFit*}
  NewFilePath=plot/massFit/full_param/${Kinematics}_$(basename $FilePath)
  cp $FilePath $NewFilePath
done < temp_MassFitPlotList.txt

ls fullAnalysis/pt*_y*_IsGammaN*/MassFit/fit_result_full_param_stats.pdf > temp_MassFitPlotList.txt
mkdir -p plot/massFit/full_param_stats

while IFS= read -r FilePath; do
  Kinematics=${FilePath#fullAnalysis/}
  Kinematics=${Kinematics%/MassFit*}
  NewFilePath=plot/massFit/full_param_stats/${Kinematics}_$(basename $FilePath)
  cp $FilePath $NewFilePath
done < temp_MassFitPlotList.txt

ls fullAnalysis/pt*_y*_IsGammaN*/MassFit/fit_result_full_param_legend.pdf > temp_MassFitPlotList.txt
mkdir -p plot/massFit/full_param_legend

while IFS= read -r FilePath; do
  Kinematics=${FilePath#fullAnalysis/}
  Kinematics=${Kinematics%/MassFit*}
  NewFilePath=plot/massFit/full_param_legend/${Kinematics}_$(basename $FilePath)
  cp $FilePath $NewFilePath
done < temp_MassFitPlotList.txt

rm temp_MassFitPlotList.txt
