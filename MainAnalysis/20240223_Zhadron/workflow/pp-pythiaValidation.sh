# Gen level 
./finalAnalysis.sh output/pythiaGen --Input mergedSample/pythia-gen-v6.root --MixFile mergedSample/pythia-gen-v6.root --IsPP true --IsGenZ true --nMix 1

# Reco level 
./finalAnalysis.sh output/pythia    --Input mergedSample/pythia-v6.root     --MixFile mergedSample/pythia-v6.root     --IsPP true --IsGenZ false --nMix 1

# pp data
./finalAnalysis.sh output/pp ""      --Input mergedSample/pp-v6.root         --MixFile mergedSample/pp-v6.root         --IsPP true --IsGenZ false --nMix 1

