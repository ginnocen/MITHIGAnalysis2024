#source clean.sh
fPromptSettingCard=${1}
FitDir=$(jq -r '.FitDir' $fPromptSettingCard)

# Extract the directory path for fullAnalysis
MicroTreeDir=$(jq -r '.fPromptTasks[0].samples.Data.sampleInput' $fPromptSettingCard | grep -o '^[^/]*')
mkdir -p $MicroTreeDir
if [[ $fPromptSettingCard == *"fPromptSettings/"* ]]; then
	cp $fPromptSettingCard $MicroTreeDir/${FitDir}.json
fi

jq -c '.fPromptTasks[]' $fPromptSettingCard | while read Task; do
	### sidebandSubtraction.C
	sideBand=$(echo $Task | jq -r '.sideBand')
	sideBandEdge=$(echo $Task | jq -r '.sideBandEdge')
	sampleInput_Data=$(echo $Task | jq -r '.samples.Data.sampleInput')
	massFitResult_Data=$(echo $Task | jq -r '.samples.Data.massFitResult')
	sampleInput_MC=$(echo $Task | jq -r '.samples.MC.sampleInput')
	massFitResult_MC=$(echo $Task | jq -r '.samples.MC.massFitResult')
	RstDir_Data=$(dirname "$sampleInput_Data")
	RstDir_Data=${RstDir_Data}/${FitDir}/
	RstDir_MC=$(dirname "$sampleInput_MC")
	RstDir_MC=${RstDir_MC}/${FitDir}_MC/
	sidebandSubtractionOutput_Data=${RstDir_Data}/sidebandSubtraction.root
	sidebandSubtractionOutput_MC=${RstDir_MC}/sidebandSubtraction.root
	
	### genMatchedTemplate.C
	promptInput=$(echo $Task | jq -r '.promptInput')
	nonPromptInput=$(echo $Task | jq -r '.nonPromptInput')
	genMatchedTemplateOutput_prompt=${RstDir_Data}/genMatchedTemplate_prompt.root
	genMatchedTemplateOutput_nonPrompt=${RstDir_Data}/genMatchedTemplate_nonPrompt.root
	
	### charmFractionExtraction.C
	fitting=$(echo $Task | jq -r '.fitting')

	mkdir -p $RstDir_Data $RstDir_MC

	# if [[ $fPromptSettingCard == *"fPromptSettings/"* ]]; then
	# 	# Create a new JSON with FitDir and the current Task
	# 	newJson=$(jq -n --arg FitDir "$FitDir" --argjson Task "$Task" \
	# 			'{FitDir: $FitDir, fPromptTasks: [$Task]}')
	# 	echo "$newJson" > "${RstDir}/fitConfig.json"
	# 	echo "Created JSON: ${RstDir}/fitConfig.json"
	# 	# cp $fPromptSettingCard $RstDir/fitConfig.json
	# fi

  # Build the command dynamically
  cmd="./SidebandSubtraction --sampleInput $sampleInput_Data --massFitResult $massFitResult_Data"
  [ "$sideBand" != "null" ] && cmd="$cmd --sideBand $sideBand"
  [ "$sideBandEdge" != "null" ] && cmd="$cmd --sideBandEdge $sideBandEdge"
  cmd="$cmd --Output $sidebandSubtractionOutput_Data --plotPrefix $RstDir_Data"
  echo "Executing >>>>>>"
  echo $cmd
  echo "Executing >>>>>>" > $RstDir_Data/sidebandSubtraction.log
  echo $cmd >> $RstDir_Data/sidebandSubtraction.log
  $cmd >> $RstDir_Data/sidebandSubtraction.log


  cmd="./SidebandSubtraction --sampleInput $sampleInput_MC --massFitResult $massFitResult_Data"
  [ "$sideBand" != "null" ] && cmd="$cmd --sideBand $sideBand"
  [ "$sideBandEdge" != "null" ] && cmd="$cmd --sideBandEdge $sideBandEdge"
  cmd="$cmd --Output $sidebandSubtractionOutput_MC --plotPrefix $RstDir_MC"
  echo "Executing >>>>>>"
  echo $cmd
  echo "Executing >>>>>>" > $RstDir_MC/sidebandSubtraction.log
  echo $cmd >> $RstDir_MC/sidebandSubtraction.log
  $cmd >> $RstDir_MC/sidebandSubtraction.log


  cmd="./GenMatchedTemplate --sampleInput $promptInput --massFitResult $massFitResult_Data --Output $genMatchedTemplateOutput_prompt"
  echo "Executing >>>>>>"
  echo $cmd
  echo "Executing >>>>>>" > $RstDir_Data/genMatchedTemplateOutput_prompt.log
  echo $cmd >> $RstDir_Data/genMatchedTemplateOutput_prompt.log
  $cmd >> $RstDir_Data/genMatchedTemplateOutput_prompt.log


  cmd="./GenMatchedTemplate --sampleInput $nonPromptInput --massFitResult $massFitResult_Data --Output $genMatchedTemplateOutput_nonPrompt"
  echo "Executing >>>>>>"
  echo $cmd
  echo "Executing >>>>>>" > $RstDir_Data/genMatchedTemplateOutput_nonPrompt.log
  echo $cmd >> $RstDir_Data/genMatchedTemplateOutput_nonPrompt.log
  $cmd >> $RstDir_Data/genMatchedTemplateOutput_nonPrompt.log


  cmd="./CharmFractionExtraction --dataInput $sidebandSubtractionOutput_Data --promptTemplInput $genMatchedTemplateOutput_prompt --nonPromptTemplInput $genMatchedTemplateOutput_nonPrompt"
  [ "$fitting" != "null" ] && cmd="$cmd --fitting $fitting"
  cmd="$cmd --Output $RstDir_Data/promptFraction.md --plotPrefix $RstDir_Data"
  echo "Executing >>>>>>"
  echo $cmd
  echo "Executing >>>>>>" > $RstDir_Data/charmFractionExtraction.log
  echo $cmd >> $RstDir_Data/charmFractionExtraction.log
  $cmd >> $RstDir_Data/charmFractionExtraction.log


  cmd="./CharmFractionExtraction --dataInput $sidebandSubtractionOutput_MC --promptTemplInput $genMatchedTemplateOutput_prompt --nonPromptTemplInput $genMatchedTemplateOutput_nonPrompt"
  [ "$fitting" != "null" ] && cmd="$cmd --fitting $fitting"
  cmd="$cmd --Output $RstDir_MC/promptFraction.md --plotPrefix $RstDir_MC"
  echo "Executing >>>>>>"
  echo $cmd
  echo "Executing >>>>>>" > $RstDir_MC/charmFractionExtraction.log
  echo $cmd >> $RstDir_MC/charmFractionExtraction.log
  $cmd >> $RstDir_MC/charmFractionExtraction.log
done
