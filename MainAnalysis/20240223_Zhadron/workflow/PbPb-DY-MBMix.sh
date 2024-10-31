# Gen level 
./finalAnalysis.sh output/DY-Gen       0-30      --Input mergedSample/DY-gen-v6.root --MixFile mergedSample/hydjet-gen-v6.root --IsPP false --IsGenZ true --nMix 10 --IsSelfMixing false --MaxHiBin 60
./finalAnalysis.sh output/DY-Gen-RecoZ 0-30      --Input mergedSample/DY-gen-v6.root --MixFile mergedSample/hydjet-gen-v6.root --IsPP false --IsGenZ false --nMix 10 --IsSelfMixing false --MaxHiBin 60

# Reco level 
#./finalAnalysis.sh output/DY     --Input mergedSample/DY-v6.root     --MixFile mergedSample/hydjet-v6.root     --IsPP false --IsGenZ false --nMix 10 --IsSelfMixing false --Shift 696.5
./finalAnalysis.sh output/DY4 		0-30     --Input mergedSample/DY-v6.root     --MixFile mergedSample/hydjet-v6.root     --IsPP false --IsGenZ false --nMix 1 --IsSelfMixing false --Shift 740 --MaxHiBin 60
./finalAnalysis.sh output/DY4_680   	0-30     --Input mergedSample/DY-v6.root     --MixFile mergedSample/hydjet-v6.root     --IsPP false --IsGenZ false --nMix 1 --IsSelfMixing false --Shift 680 --MaxHiBin 60
./finalAnalysis.sh output/DY4_800   	0-30     --Input mergedSample/DY-v6.root     --MixFile mergedSample/hydjet-v6.root     --IsPP false --IsGenZ false --nMix 1 --IsSelfMixing false --Shift 800 --MaxHiBin 60

./finalAnalysis.sh output/DY4 		30-50     --Input mergedSample/DY-v6.root     --MixFile mergedSample/hydjet-v6.root     --IsPP false --IsGenZ false --nMix 1 --IsSelfMixing false --Shift 740 --MaxHiBin 100 --MinHiBin 60
./finalAnalysis.sh output/DY4_680   	30-50     --Input mergedSample/DY-v6.root     --MixFile mergedSample/hydjet-v6.root     --IsPP false --IsGenZ false --nMix 1 --IsSelfMixing false --Shift 680 --MaxHiBin 100 --MinHiBin 60
./finalAnalysis.sh output/DY4_800   	30-50     --Input mergedSample/DY-v6.root     --MixFile mergedSample/hydjet-v6.root     --IsPP false --IsGenZ false --nMix 1 --IsSelfMixing false --Shift 800 --MaxHiBin 100 --MinHiBin 60

./finalAnalysis.sh output/DY4 		50-90     --Input mergedSample/DY-v6.root     --MixFile mergedSample/hydjet-v6.root     --IsPP false --IsGenZ false --nMix 1 --IsSelfMixing false --Shift 740 --MaxHiBin 180 --MinHiBin 100
./finalAnalysis.sh output/DY4_680   	50-90     --Input mergedSample/DY-v6.root     --MixFile mergedSample/hydjet-v6.root     --IsPP false --IsGenZ false --nMix 1 --IsSelfMixing false --Shift 680 --MaxHiBin 180 --MinHiBin 100
./finalAnalysis.sh output/DY4_800   	50-90     --Input mergedSample/DY-v6.root     --MixFile mergedSample/hydjet-v6.root     --IsPP false --IsGenZ false --nMix 1 --IsSelfMixing false --Shift 800 --MaxHiBin 180 --MinHiBin 100


./finalAnalysis.sh output/DY4 		0-90     --Input mergedSample/DY-v6.root     --MixFile mergedSample/hydjet-v6.root     --IsPP false --IsGenZ false --nMix 1 --IsSelfMixing false --Shift 740 --MaxHiBin 180 --MinHiBin 0
./finalAnalysis.sh output/DY4_680   	0-90     --Input mergedSample/DY-v6.root     --MixFile mergedSample/hydjet-v6.root     --IsPP false --IsGenZ false --nMix 1 --IsSelfMixing false --Shift 680 --MaxHiBin 180 --MinHiBin 0
./finalAnalysis.sh output/DY4_800   	0-90     --Input mergedSample/DY-v6.root     --MixFile mergedSample/hydjet-v6.root     --IsPP false --IsGenZ false --nMix 1 --IsSelfMixing false --Shift 800 --MaxHiBin 180 --MinHiBin 0
