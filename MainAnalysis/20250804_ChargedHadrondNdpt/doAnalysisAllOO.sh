#!/bin/bash

./RunParallelReadParam.sh --Output output_OOReferenceCentralValue --Input OO_InputFileList_Reference.txt --CollisionSystem true --ApplyEventSelection true --UseEventWeight false --UseSpeciesWeight false --UseTrackWeight true --TrackWeightSelection 2 --MinTrackPt 0.4 --MinLeadingTrackPt -1 --TriggerChoice 1 --EventSelectionOption 1 --SpeciesCorrectionOption 0 --ScaleFactor 1

