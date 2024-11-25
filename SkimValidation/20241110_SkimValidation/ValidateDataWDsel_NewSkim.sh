#! /bin/bash

cd ${ProjectBase}/SkimValidation/20241110_SkimValidation/

Input1=${1}
Input2=${2}

make MkdirOutput DumpNewSkim Validate
./DumpNewSkim --Input ${1} \
			  --Output out/DumpNewSkim1 --IsData true --ApplyDSelection true

./DumpNewSkim --Input ${2} \
			  --Output out/DumpNewSkim2 --IsData true --ApplyDSelection true

./Validate --Input1 out/DumpNewSkim1.root --Input2 out/DumpNewSkim2.root \
					 --LegendName1 Input1 --LegendName2 Input2 \
					 --PlotName out/ValidateDataWDSel_Input1_vs_Input2_Skim.pdf

echo "Diff between old file (<) and new file (>)" > out/DumpSkims.diff
echo "Quit 'less out/DumpSkims.diff' by entering 'q'" >> out/DumpSkims.diff
diff out/DumpNewSkim1.txt out/DumpNewSkim2.txt >> out/DumpSkims.diff || echo 'Files differ'
less out/DumpSkims.diff

cd -