#source clean.sh
dataMCCompSettingCard=${1}
AnaDir="dataMCComp"

# Extract the directory path for fullAnalysis
MicroTreeDir=$(jq -r '.MicroTreeDir' $dataMCCompSettingCard)
mkdir -p $MicroTreeDir
if [[ $dataMCCompSettingCard == *"dataMCCompSettings/"* ]]; then
  cp $dataMCCompSettingCard $MicroTreeDir/${AnaDir}.json
fi

### global method
sideBand=$(jq -r '.sideBand' $dataMCCompSettingCard)
sideBandEdge=$(jq -r '.sideBandEdge' $dataMCCompSettingCard)

jq -c '.vars[]' $dataMCCompSettingCard | while read VarBlock; do
  var=$(echo $VarBlock | jq -r '.var')
  xTitle=$(echo $VarBlock | jq -r '.xTitle')
  bins=$(echo $VarBlock | jq -r '.bins')
  nbins=$(echo $VarBlock | jq -r '.nbins')
  xmin=$(echo $VarBlock | jq -r '.xmin')
  xmax=$(echo $VarBlock | jq -r '.xmax')
  filesuffix=$(echo $VarBlock | jq -r '.filesuffix')
  eventBased=$(echo $VarBlock | jq -r '.eventBased // false')
  eventTree=$(echo $VarBlock | jq -r '.eventTree // "nt_evt"')

  jq -c '.tasks[]' $dataMCCompSettingCard | while read Task; do
    sampleInput_Data=${MicroTreeDir}/$(echo $Task | jq -r '.Data.sampleInput')
    massFitResult_Data=${MicroTreeDir}/$(echo $Task | jq -r '.Data.massFitResult')
    sampleInput_MC=${MicroTreeDir}/$(echo $Task | jq -r '.MC.sampleInput')
    massFitResult_MC=${MicroTreeDir}/$(echo $Task | jq -r '.MC.massFitResult')
    RstDir_Data=$(dirname "$sampleInput_Data")
    RstDir_Data=${RstDir_Data}/${AnaDir}/
    RstDir_MC=$(dirname "$sampleInput_MC")
    RstDir_MC=${RstDir_MC}/${AnaDir}_MC/
    sidebandSubtractionOutput_Data=${RstDir_Data}/sidebandSubtraction_${filesuffix}.root
    sidebandSubtractionOutput_MC=${RstDir_MC}/sidebandSubtraction_${filesuffix}.root
    dataMCCompOutPdf=${RstDir_Data}/sidebandSubtraction_${filesuffix}_comp.pdf

    mkdir -p $RstDir_Data $RstDir_MC

    # Build the command dynamically (event-level vars: no mass fit / sideband subtraction)
    if [[ "$eventBased" == "true" ]]; then
      cmd="./SidebandSubtraction --eventOnly 1 --sampleInput $sampleInput_Data"
      [ "$eventTree" != "null" ] && [ -n "$eventTree" ] && cmd="$cmd --eventTree $eventTree"
    else
      cmd="./SidebandSubtraction --sampleInput $sampleInput_Data --massFitResult $massFitResult_Data"
      [ "$sideBand" != "null" ] && cmd="$cmd --sideBand $sideBand"
      [ "$sideBandEdge" != "null" ] && cmd="$cmd --sideBandEdge $sideBandEdge"
    fi
    # Quote --var: expressions like TMath::Abs(DsvpvDistance_2D)/... contain '(' which breaks eval if unquoted
    [ "$var" != "null" ] && cmd="$cmd --var \"$var\""
    [ "$xTitle" != "null" ] && cmd="$cmd --xTitle \"$xTitle\""
    [ "$bins" != "null" ] && cmd="$cmd --bins $bins"
    [ "$nbins" != "null" ] && cmd="$cmd --nbins $nbins"
    [ "$xmin" != "null" ] && cmd="$cmd --xmin $xmin"
    [ "$xmax" != "null" ] && cmd="$cmd --xmax $xmax"
    [ "$filesuffix" != "null" ] && [ -n "$filesuffix" ] && cmd="$cmd --filesuffix \"$filesuffix\""

    cmd="$cmd --Output $sidebandSubtractionOutput_Data --plotPrefix $RstDir_Data"
    echo "Executing >>>>>>"
    echo $cmd
    echo "Executing >>>>>>" > $RstDir_Data/sidebandSubtraction.log
    echo $cmd >> $RstDir_Data/sidebandSubtraction.log
    eval $cmd >> $RstDir_Data/sidebandSubtraction.log


    if [[ "$eventBased" == "true" ]]; then
      cmd="./SidebandSubtraction --eventOnly 1 --sampleInput $sampleInput_MC"
      [ "$eventTree" != "null" ] && [ -n "$eventTree" ] && cmd="$cmd --eventTree $eventTree"
    else
      cmd="./SidebandSubtraction --sampleInput $sampleInput_MC --massFitResult $massFitResult_MC"
      [ "$sideBand" != "null" ] && cmd="$cmd --sideBand $sideBand"
      [ "$sideBandEdge" != "null" ] && cmd="$cmd --sideBandEdge $sideBandEdge"
    fi
    [ "$var" != "null" ] && cmd="$cmd --var \"$var\""
    [ "$xTitle" != "null" ] && cmd="$cmd --xTitle \"$xTitle\""
    [ "$bins" != "null" ] && cmd="$cmd --bins $bins"
    [ "$nbins" != "null" ] && cmd="$cmd --nbins $nbins"
    [ "$xmin" != "null" ] && cmd="$cmd --xmin $xmin"
    [ "$xmax" != "null" ] && cmd="$cmd --xmax $xmax"
    [ "$filesuffix" != "null" ] && [ -n "$filesuffix" ] && cmd="$cmd --filesuffix \"$filesuffix\""

    cmd="$cmd --Output $sidebandSubtractionOutput_MC --plotPrefix $RstDir_MC"
    echo "Executing >>>>>>"
    echo $cmd
    echo "Executing >>>>>>" > $RstDir_MC/sidebandSubtraction.log
    echo $cmd >> $RstDir_MC/sidebandSubtraction.log
    eval $cmd >> $RstDir_MC/sidebandSubtraction.log

    cmd="./DataMCComp --Data $sidebandSubtractionOutput_Data --MC $sidebandSubtractionOutput_MC --out $dataMCCompOutPdf"
    echo "Executing >>>>>>"
    echo $cmd
    echo "Executing >>>>>>" > $RstDir_Data/dataMCComp.log
    echo $cmd >> $RstDir_Data/dataMCComp.log
    eval $cmd >> $RstDir_Data/dataMCComp.log

    /afs/cern.ch/user/y/yuchenc/Dropbox-Uploader/dropbox_uploader.sh upload ${RstDir_Data}/*${filesuffix}*pdf /tmp/${RstDir_Data}/
  done
done
