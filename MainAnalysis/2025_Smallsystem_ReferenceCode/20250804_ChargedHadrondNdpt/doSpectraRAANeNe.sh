#!/bin/bash

LUMIAAtoPP=806882.576415303 #lumi for golden json, NeNe, meant to be used for 60 PDs normalization.

#Lumi per PD computed with the following commands in lxplus9:

#source /cvmfs/cms-bril.cern.ch/cms-lumi-pog/brilws-docker/brilws-env
#brilcalc lumi -b "STABLE BEAMS" --normtag hfetLightIons25v01 --hltpath "HLT_MinimumBiasHF_OR_BptxAND_v1" -u /nb -i 'NeNe.json'
#Where NeNe.json has this:
#{
#  "394271": [[1, 106], [172, 306]],
#  "394272": [[1, 357], [663, 1135]]
#}



# lumi in mb-1, matches the golden JSON file overlap with Lynn's JSON, divided by 60 (per PD lumi).
#85743.91 OO lumi, Jordan JSON

echo "Running spectra macro..."
g++ -o Spectra Spectra.cpp $(root-config --cflags --glibs) -lASImage

# Run the spectra macro for NeNe
# Inputs:
# 1. Collision system: e.g. "NeNe"
# 2. Input file for AA spectra
# 3. Histogram name for AA spectra
# 4. Input file for AA systematics
# 5. Output histogram name for AA systematics
# 6. Lumi scaling for NeNe
# 7. Output directory for results

./Spectra "NeNe" "output_NeNeReferenceCentralValue/MergedOutput.root" "hTrkPt" "systematic_variations/nenesystematics.root" "hSystematic_total" $LUMIAAtoPP "output_NeNeReferenceCentralValue"

root -l RAA_NeNe.C
