
This is an alternative to the 20250409_ForestReducerMuMuJet_svtx skimmer configuration. It is meant to be identical in function, and simply to organize the skimmed information in a slightly different way. 

In the older skimmer, each Tree entry was an event, so Jet (and mumu-tagged Jet) information was entirely stored in std::vectors. Consequently, all of the tracking and secondary vertex information was stored in "std::vector<std::vector<type>>" form. This required the use of additional libraries to read, and resulted in a very slow processing time - as each of these vectors could have varying lengths. 

To complement the old approach, this skimmer produces two trees, "Tree" which contains each reco-level jet as an entry (the previous skim tree has been flattened, so all event level information is still accessible for this object), and for example secondary vertex or track information is stored now as std:vectors instead of vector<vector<>> form. This makes reading the array and filling histograms faster, and more straightforward. As Gen-Level gets can be decoupled from the reco-level jets, I also include a "GenTree" written to the output files which allows you to access Gen-Jet information in the exact same fashion. 

Each Tree here is generated with an "DimuonJetMessenger" object defined in the "DimuonJetMessenger.h" file. I keep it locally here to avoid confusion with the real Messenger.h file in CommonCode.

I have included an extremely detailed "debug.C" macro within this folder to compare the outputs of the older skimmer and this version. They are 100% identical in their information. 

The skim generation workflow in this directory is identical to the previous skim folders. The only necessary changes one would need to make are the input / output files as well as arguments in the skim submission scripts (e.g RunParallelMC.sh), and then they can run out of the box. 

Please let me (Abraham) know if there are any issues. Thank you!