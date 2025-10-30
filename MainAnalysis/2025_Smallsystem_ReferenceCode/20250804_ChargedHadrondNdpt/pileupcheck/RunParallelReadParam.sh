#!/bin/bash

# --- Default parameters ---
MAXCORES=30
COLLISIONSYSTEM=true
OUTPUTDIR="output_OOReference"
INPUTLIST="OO_InputFileList_Reference.txt"
DOAPPLYEVENTSELECTION=true
DOUSEEVENTWEIGHT=false
DOUSESPECIESWEIGHT=false
DOUSETRACKWEIGHT=true
TRACKWEIGHTSELECTION=2
TRIGGERCHOICE=1
EVENTSELECTIONOPTION=1
SPECIESCORRECTIONOPTION=0
SCALEFACTOR=1

# --- Manual long option parsing ---
while [[ $# -gt 0 ]]; do
  case "$1" in
    --Output) OUTPUTDIR="$2"; shift 2 ;;
    --Input) INPUTLIST="$2"; shift 2 ;;
    --CollisionSystem) COLLISIONSYSTEM="$2"; shift 2 ;;
    --ApplyEventSelection) DOAPPLYEVENTSELECTION="$2"; shift 2 ;;
    --UseEventWeight) DOUSEEVENTWEIGHT="$2"; shift 2 ;;
    --UseSpeciesWeight) DOUSESPECIESWEIGHT="$2"; shift 2 ;;
    --UseTrackWeight) DOUSETRACKWEIGHT="$2"; shift 2 ;;
    --TrackWeightSelection) TRACKWEIGHTSELECTION="$2"; shift 2 ;;
    --TriggerChoice) TRIGGERCHOICE="$2"; shift 2 ;;
    --EventSelectionOption) EVENTSELECTIONOPTION="$2"; shift 2 ;;
    --SpeciesCorrectionOption) SPECIESCORRECTIONOPTION="$2"; shift 2 ;;
    --ScaleFactor) SCALEFACTOR="$2"; shift 2 ;;
    --help)
      echo "Usage: $0 [--Output DIR] [--Input FILE] [--ApplyEventSelection true|false] ..."
      exit 0
      ;;
    *)
      echo "Unknown option: $1" >&2
      exit 1
      ;;
  esac
done

# --- Print configuration summary ---
echo "=== RunParallel Configuration ==="
echo "OUTPUTDIR              = $OUTPUTDIR"
echo "INPUTLIST              = $INPUTLIST"
echo "COLLISIONSYSTEM        = $COLLISIONSYSTEM"
echo "DOAPPLYEVENTSELECTION  = $DOAPPLYEVENTSELECTION"
echo "DOUSEEVENTWEIGHT       = $DOUSEEVENTWEIGHT"
echo "DOUSESPECIESWEIGHT     = $DOUSESPECIESWEIGHT"
echo "DOUSETRACKWEIGHT       = $DOUSETRACKWEIGHT"
echo "TRACKWEIGHTSELECTION   = $TRACKWEIGHTSELECTION"
echo "TRIGGERCHOICE          = $TRIGGERCHOICE"
echo "EVENTSELECTIONOPTION   = $EVENTSELECTIONOPTION"
echo "SPECIESCORRECTIONOPTION= $SPECIESCORRECTIONOPTION"
echo "SCALEFACTOR            = $SCALEFACTOR"
echo "MAXCORES               = $MAXCORES"
echo "================================="

# --- Setup output directory ---
MERGEDOUTPUT="${OUTPUTDIR}/MergedOutput.root"
rm -f "$MERGEDOUTPUT"

source clean.sh
rm -rf "$OUTPUTDIR"
mkdir -p "$OUTPUTDIR"

# --- Function to limit number of concurrent jobs ---
wait_for_slot() {
  while (( $(jobs -r | wc -l) >= MAXCORES )); do
    sleep 1
  done
}

# --- Input file validation ---
if [[ ! -s "$INPUTLIST" ]]; then
  echo "Error: Input list '$INPUTLIST' is empty or missing."
  exit 1
fi

# --- Launch jobs ---
counter=0
while IFS= read -r INPUT; do
  OUTPUTFILE="${OUTPUTDIR}/output_${counter}.root"
  echo "Launching job for $INPUT -> $OUTPUTFILE"

  ./ExecuteChargedHadrondNdpt \
    --TriggerChoice "$TRIGGERCHOICE" \
    --ScaleFactor "$SCALEFACTOR" \
    --Input "$INPUT" \
    --Output "$OUTPUTFILE" \
    --CollisionSystem "$COLLISIONSYSTEM" \
    --ApplyEventSelection "$DOAPPLYEVENTSELECTION" \
    --UseSpeciesWeight "$DOUSESPECIESWEIGHT" \
    --UseEventWeight "$DOUSEEVENTWEIGHT" \
    --UseTrackWeight "$DOUSETRACKWEIGHT" \
    --TrackWeightSelection "$TRACKWEIGHTSELECTION" \
    --EventSelectionOption "$EVENTSELECTIONOPTION" \
    --SpeciesCorrectionOption "$SPECIESCORRECTIONOPTION" &

  ((counter++))
  wait_for_slot
done < "$INPUTLIST"

# --- Wait for jobs to complete and merge ---
wait
echo "Merging outputs to $MERGEDOUTPUT..."
hadd "$MERGEDOUTPUT" "$OUTPUTDIR"/output_*.root

echo "All done!"
echo "Merged output file: $MERGEDOUTPUT"


