#!/bin/bash

FitSettingCard=${1}
FitDir=$(jq -r '.FitDir' $FitSettingCard)

# Extract the directory path for fullAnalysis
MicroTreeDir=$(jq -r '.MicroTrees[0].dataInput' $FitSettingCard | grep -o '^[^/]*')
mkdir -p $MicroTreeDir
if [[ $FitSettingCard == *"fitSettings/"* ]]; then
	cp $FitSettingCard $MicroTreeDir/${FitDir}.json
fi

jq -c '.MicroTrees[]' $FitSettingCard | while read MicroTree; do
	dataInput=$(echo $MicroTree | jq -r '.dataInput')
	fitmcInputs=$(echo $MicroTree | jq -r '.fitmcInputs')
	sigswpInputs=$(echo $MicroTree | jq -r '.sigswpInputs')
	KKmcInputs=$(echo $MicroTree | jq -r '.KKmcInputs')
	pipimcInputs=$(echo $MicroTree | jq -r '.pipimcInputs')
	neventsInput=$(echo $MicroTree | jq -r '.neventsInput')
	effmcInput=$(echo $MicroTree | jq -r '.effmcInput')
	doSyst_sig=$(echo $MicroTree | jq -r '.doSyst_sig')
	doSyst_comb=$(echo $MicroTree | jq -r '.doSyst_comb')
	doPkkk=$(echo $MicroTree | jq -r '.doPkkk')
	doPkpp=$(echo $MicroTree | jq -r '.doPkpp')
	sigMeanRange=$(echo $MicroTree | jq -r '.sigMeanRange')
	sigAlphaRange=$(echo $MicroTree | jq -r '.sigAlphaRange')
	systMassWin=$(echo $MicroTree | jq -r '.systMassWin')
	RstDir=$(echo $MicroTree | jq -r '.RstDir')
	if [[ $RstDir == "null" ]]; then
			RstDir=$(dirname "$dataInput")
			RstDir=${RstDir}/${FitDir}/
	fi
	mkdir -p $RstDir
	if [[ $FitSettingCard == *"fitSettings/"* ]]; then
		# Create a new JSON with FitDir and the current MicroTree
		newJson=$(jq -n --arg FitDir "$FitDir" --argjson MicroTree "$MicroTree" \
				'{FitDir: $FitDir, MicroTrees: [$MicroTree]}')
		echo "$newJson" > "${RstDir}/fitConfig.json"
		echo "Created JSON: ${RstDir}/fitConfig.json"
		# cp $FitSettingCard $RstDir/fitConfig.json
	fi

	# Optional flags shared by data and MC_inclusive MassFit jobs
	appendOpts=""
	[ "$sigswpInputs" != "null" ] && appendOpts="$appendOpts --sigswpInputs $sigswpInputs"
	[ "$KKmcInputs" != "null" ] && appendOpts="$appendOpts --KKmcInputs $KKmcInputs"
	[ "$pipimcInputs" != "null" ] && appendOpts="$appendOpts --pipimcInputs $pipimcInputs"
	[ "$neventsInput" != "null" ] && appendOpts="$appendOpts --neventsInput $neventsInput"
	[ "$doSyst_sig" != "null" ] && appendOpts="$appendOpts --doSyst_sig $doSyst_sig"
	[ "$doSyst_comb" != "null" ] && appendOpts="$appendOpts --doSyst_comb $doSyst_comb"
	[ "$doPkkk" != "null" ] && appendOpts="$appendOpts --doPkkk $doPkkk"
	[ "$doPkpp" != "null" ] && appendOpts="$appendOpts --doPkpp $doPkpp"
	[ "$sigMeanRange" != "null" ] && appendOpts="$appendOpts --sigMeanRange $sigMeanRange"
	[ "$sigAlphaRange" != "null" ] && appendOpts="$appendOpts --sigAlphaRange $sigAlphaRange"
	[ "$systMassWin" != "null" ] && appendOpts="$appendOpts --systMassWin $systMassWin"

	# ----- Data MassFit (same bin as dataInput) -----
	cmd="./MassFit --dataInput $dataInput --mcInputs $fitmcInputs${appendOpts}"
	cmd="$cmd --Output fit.root --RstDir $RstDir"

	echo "Executing >>>>>>"
	echo $cmd
	echo "Executing >>>>>>" > $RstDir/fit.log
	echo $cmd >> $RstDir/fit.log
	$cmd >> $RstDir/fit.log

	# ----- MC MassFit: same treatment as data, but Data.root -> MC_inclusive.root for both data and mc chain -----
	binDir=$(dirname "$dataInput")
	mcInclusive="${binDir}/MC_inclusive.root"
	rstDirMC="${binDir}/${FitDir}_MC/"
	mkdir -p "$rstDirMC"
	if [[ $FitSettingCard == *"fitSettings/"* ]]; then
		newJsonMC=$(jq -n --arg FitDir "$FitDir" --argjson MicroTree "$MicroTree" --arg mc "$mcInclusive" \
			'{FitDir: $FitDir, MicroTrees: [($MicroTree | .dataInput = $mc | .fitmcInputs = $mc)]}')
		echo "$newJsonMC" > "${rstDirMC}/fitConfig.json"
		echo "Created JSON: ${rstDirMC}/fitConfig.json"
	fi

	cmdMC="./MassFit --dataInput $mcInclusive --mcInputs $mcInclusive${appendOpts}"
	cmdMC="$cmdMC --Output fit.root --RstDir $rstDirMC"

	echo "Executing >>>>>>"
	echo $cmdMC
	echo "Executing >>>>>>" > $rstDirMC/fit.log
	echo $cmdMC >> $rstDirMC/fit.log
	$cmdMC >> $rstDirMC/fit.log
done
wait
