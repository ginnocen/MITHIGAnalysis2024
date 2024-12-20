#source clean.sh
FitSettingCard=${1}
FitDir=$(jq -r '.FitDir' $FitSettingCard)

jq -c '.MicroTrees[]' $FitSettingCard | while read MicroTree; do
	dataInput=$(echo $MicroTree | jq -r '.dataInput')
	fitmcInputs=$(echo $MicroTree | jq -r '.fitmcInputs')
	sigswpInputs=$(echo $MicroTree | jq -r '.sigswpInputs')
	KKmcInputs=$(echo $MicroTree | jq -r '.KKmcInputs')
	pipimcInputs=$(echo $MicroTree | jq -r '.pipimcInputs')
	neventsInput=$(echo $MicroTree | jq -r '.neventsInput')
	effmcInput=$(echo $MicroTree | jq -r '.effmcInput')
	doSyst_comb=$(echo $MicroTree | jq -r '.doSyst_comb')
	RstDir=$(dirname "$dataInput")
	RstDir=${RstDir}/${FitDir}/
	mkdir -p $RstDir
	cp $FitSettingCard $RstDir/fitConfig.json

  # Build the command dynamically
  cmd="./MassFit --dataInput $dataInput --mcInputs $fitmcInputs"
  [ "$sigswpInputs" != "null" ] && cmd="$cmd --sigswpInputs $sigswpInputs"
  [ "$KKmcInputs" != "null" ] && cmd="$cmd --KKmcInputs $KKmcInputs"
  [ "$pipimcInputs" != "null" ] && cmd="$cmd --pipimcInputs $pipimcInputs"
  [ "$neventsInput" != "null" ] && cmd="$cmd --neventsInput $neventsInput"
  [ "$doSyst_comb" != "null" ] && cmd="$cmd --doSyst_comb $doSyst_comb"
  cmd="$cmd --Output fit.root --RstDir $RstDir"

  # Execute the command and log output
  $cmd > $RstDir/fit.log

  echo '================================================' >> $RstDir/fit.log
  echo '= Getting corrected yields:' >> $RstDir/fit.log
  echo '================================================' >> $RstDir/fit.log
  cmd="./CorrectedYields --rawYieldInput $RstDir/fit.root --effInput $effmcInput --Output $RstDir/correctedYields.md"
  $cmd >> $RstDir/fit.log
done