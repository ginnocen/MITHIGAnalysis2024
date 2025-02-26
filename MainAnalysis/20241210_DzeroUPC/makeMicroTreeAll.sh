#!/bin/bash

source clean.sh

#./makeMicroTree.sh sampleSettings/fullAnalysis_skimV4.json
#sleep 1
#./makeMicroTree.sh sampleSettings/systDsvpv_skimV4.json
#sleep 1
./makeMicroTree.sh sampleSettings/systDtrkPt_skimV4.json
sleep 1
./makeMicroTree.sh sampleSettings/systRapGapLoose_skimV4.json
sleep 1
./makeMicroTree.sh sampleSettings/systRapGapTight_skimV4.json
sleep 1
./makeMicroTree.sh sampleSettings/systDalpha_skimV4.json
sleep 1
./makeMicroTree.sh sampleSettings/systDchi2cl_skimV4.json
sleep 1
