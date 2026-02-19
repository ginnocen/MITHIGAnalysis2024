G2QQBar Skimmer 

The skimmer code occupies ReduceForest.cpp. To compile this code, run clean.sh. To quickly test the skimmer, run ```./testskim_forGM.sh```


The skimmer takes arguments to specify whether the dataset is Data/MC, ppref/PbPb, to include secondary vertex and track properties in the output skim or not, the minimum Jet pT, the fraction of events, and the jet collection name (as this changes between PbPb and pp datasets), alongside the input and output files. 

```
./Execute \
    --Input HiForestMiniAOD_123.root \
    --IsData false \
    --IsPP true \
    --svtx true \
    --Output tested2.root \
    --MinJetPT 30 \
    --Fraction 1.00 \
    --PFJetCollection ak3PFJetAnalyzer/t
```

The skimmer relies on two source files: 
```
/MITHIGAnalysis/CommonCode/source/Messenger.cpp
/MITHIGAnalysis/CommonCode/source/DimuonMessenger.cpp
```
and the corresponding header files in Commoncode/include.

The skimmer loops over each event, and then over each reco jet. For each jet above the minimum jet pT threshold and within some kinematic cuts, it will fill basic vertex and jet information. If the "--svtx" flag is set to "true," vectors of track and secondary vertex information will be written to the output TTree via the DimuonMessenger object. For each reco jet, the skimmer then loops over all reco muons in the event. If there is a pair (or more) of reco muons within R=0.3 of the jet, satisfying selection quality criteria, the skimmer will write dimuon kinematic properties to the skimmer (regarding the pair of muons with the highest dimuon pT). Then, if we are running over MC, the skimmer passes over all gen level muons. A similar selection is applied, and Gen-matching booleans are filled to reflect if identified reco dimuon jets match identified gen dimuon jets. Finally, tag and probe weights for any selected dimuons are calculated and written to the output tree. Finally, the skimmer does a loop over all Gen-jets within the sample and records their kinematic and matching properties (for use with unfolding). 

The skim is formatted such that there are two trees outputted "Tree" and "GenTree". Each entry in one of these trees corresponds to a single jet. Event information (i.e. Vertex position, pThat) is duplicated and stored for each jet within an event. 

The skimming is typically run using ```RunParallelData_xrdcp.sh``` or ```RunParallelMC_xrdcp.sh```. These will use up to 50 cores to copy forest files from /eos/cms/store/ and skim them locally. They are configured to take an input file (within which they run over all files named "HiForestMiniAOD_*.root)", copy it locally, send all skims to a user-specified output directory, and Hadd them. To edit the arguments used for skimming, you must directly edit the submission scripts. For faster skimming, Jing's condor-foresting setup is included within the "lxplus" directory. Directions to submit large batches of skims using condor can be found here: https://www.dropbox.com/scl/fi/qtnidvm8jujrd5vjvyzdi/Condor-tutorial-for-MITHIG.paper?rlkey=vf02ayehvera4i2zbp7n69yfz&dl=0. Note that this will only work if used in AFS.



Please let me (Abraham) know if there are any issues. Thank you!