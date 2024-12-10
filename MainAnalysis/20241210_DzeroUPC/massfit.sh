#source clean.sh
FitSettingCard=${1}
FitDir=$(jq -r '.FitDir' $FitSettingCard)

jq -c '.MicroTrees[]' $FitSettingCard | while read MicroTree; do
	dataInput=$(echo $MicroTree | jq -r '.dataInput')
	mcInput=$(echo $MicroTree | jq -r '.mcInput')
	sigswpInput=$(echo $MicroTree | jq -r '.sigswpInput')
	KKmcInput=$(echo $MicroTree | jq -r '.KKmcInput')
	pipimcInput=$(echo $MicroTree | jq -r '.pipimcInput')
	neventsInput=$(echo $MicroTree | jq -r '.neventsInput')
	doSyst_comb=$(echo $MicroTree | jq -r '.doSyst_comb')
	RstDir=$(dirname "$dataInput")
	RstDir=${RstDir}/${FitDir}/
	mkdir -p $RstDir
	cp $FitSettingCard $RstDir/fitConfig.json

  # Build the command dynamically
  cmd="./MassFit --dataInput $dataInput --mcInput $mcInput"
  [ "$sigswpInput" != "null" ] && cmd="$cmd --sigswpInput $sigswpInput"
  [ "$KKmcInput" != "null" ] && cmd="$cmd --KKmcInput $KKmcInput"
  [ "$pipimcInput" != "null" ] && cmd="$cmd --pipimcInput $pipimcInput"
  [ "$neventsInput" != "null" ] && cmd="$cmd --neventsInput $neventsInput"
  [ "$doSyst_comb" != "null" ] && cmd="$cmd --doSyst_comb $doSyst_comb"
  cmd="$cmd --Output fit.root --RstDir $RstDir"

  # Execute the command and log output
  $cmd > $RstDir/fit.log

done