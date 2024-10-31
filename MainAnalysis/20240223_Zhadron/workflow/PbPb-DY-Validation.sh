# Gen level 
./finalAnalysis.sh output/DY-Gen MB --Input mergedSample/DY-gen-v6.root --MixFile mergedSample/DY-gen-v6.root --IsPP false --IsGenZ true --nMix 10

# Reco level 
./finalAnalysis.sh output/DY  MB   --Input mergedSample/DY-v6.root     --MixFile mergedSample/DY-v6.root     --IsPP false --IsGenZ false --nMix 10

