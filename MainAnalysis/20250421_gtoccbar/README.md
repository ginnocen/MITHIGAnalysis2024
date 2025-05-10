# Instructions
This area contains all the essential ingredients to obtain the central value of the measurement.
Setup the analysis environment
```bash
cmsrel CMSSW_13_2_12
cd CMSSW_13_2_12/src
cmsenv
cd ../../
```

# Run the complete analysis

`source runMain.sh`

`runMain.sh` computes, sequentially, the following steps:

1. Muon acceptance and efficiency correction maps from MC
2. Read data, apply corrections from step 1 as a weight for the MuMuJet events. Inclusive jets are filled with no weights.
3. Filling the RooUnfold response matrices.
4. Unfold the MuMuJet and inclusive jet yields, and compute the ratio of the two.
5. The final file contains the central values for the ratio of muMuJet/inclusive jet.
