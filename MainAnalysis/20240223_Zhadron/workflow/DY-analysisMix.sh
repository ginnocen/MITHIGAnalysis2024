./finalAnalysis.sh output/DYMix0_30 1_2  --MinTrackPT 1 --MaxTrackPT 2  --MinHiBin 0 --MaxHiBin 60  --Input mergedSample/DY-v7.root --MixFile mergedSample/hydjet-v6.root  --IsPP false --IsGenZ false --nMix 1 --IsSelfMixing false --Shift 680
./finalAnalysis.sh output/DYMix0_30 2_4  --MinTrackPT 2 --MaxTrackPT 4  --MinHiBin 0 --MaxHiBin 60  --Input mergedSample/DY-v7.root --MixFile mergedSample/hydjet-v6.root  --IsPP false --IsGenZ false --nMix 1 --IsSelfMixing false --Shift 680
./finalAnalysis.sh output/DYMix0_30 4_10 --MinTrackPT 4 --MaxTrackPT 10 --MinHiBin 0 --MaxHiBin 60  --Input mergedSample/DY-v7.root --MixFile mergedSample/hydjet-v6.root  --IsPP false --IsGenZ false --nMix 1 --IsSelfMixing false --Shift 680

hadd -f plots/DYMix0_30-result.root output/DYMix0_30-1_2-result.root output/DYMix0_30-2_4-result.root output/DYMix0_30-4_10-result.root

./finalAnalysis.sh output/DYMix30_50 1_2  --MinTrackPT 1 --MaxTrackPT 2  --MinHiBin 60 --MaxHiBin 100  --Input mergedSample/DY-v7.root --MixFile mergedSample/hydjet-v6.root  --IsPP false --IsGenZ false --nMix 1 --IsSelfMixing false --Shift 680
./finalAnalysis.sh output/DYMix30_50 2_4  --MinTrackPT 2 --MaxTrackPT 4  --MinHiBin 60 --MaxHiBin 100  --Input mergedSample/DY-v7.root --MixFile mergedSample/hydjet-v6.root  --IsPP false --IsGenZ false --nMix 1 --IsSelfMixing false --Shift 680
./finalAnalysis.sh output/DYMix30_50 4_10 --MinTrackPT 4 --MaxTrackPT 10 --MinHiBin 60 --MaxHiBin 100  --Input mergedSample/DY-v7.root --MixFile mergedSample/hydjet-v6.root  --IsPP false --IsGenZ false --nMix 1 --IsSelfMixing false --Shift 680

hadd -f plots/DYMix30_50-result.root output/DYMix30_50-1_2-result.root output/DYMix30_50-2_4-result.root output/DYMix30_50-4_10-result.root

./finalAnalysis.sh output/DYMix50_90 1_2  --MinTrackPT 1 --MaxTrackPT 2  --MinHiBin 100 --MaxHiBin 180  --Input mergedSample/DY-v7.root --MixFile mergedSample/hydjet-v6.root  --IsPP false --IsGenZ false --nMix 1 --IsSelfMixing false --Shift 680
./finalAnalysis.sh output/DYMix50_90 2_4  --MinTrackPT 2 --MaxTrackPT 4  --MinHiBin 100 --MaxHiBin 180  --Input mergedSample/DY-v7.root --MixFile mergedSample/hydjet-v6.root  --IsPP false --IsGenZ false --nMix 1 --IsSelfMixing false --Shift 680
./finalAnalysis.sh output/DYMix50_90 4_10 --MinTrackPT 4 --MaxTrackPT 10 --MinHiBin 100 --MaxHiBin 180  --Input mergedSample/DY-v7.root --MixFile mergedSample/hydjet-v6.root  --IsPP false --IsGenZ false --nMix 1 --IsSelfMixing false --Shift 680

hadd -f plots/DYMix50_90-result.root output/DYMix50_90-1_2-result.root output/DYMix50_90-2_4-result.root output/DYMix50_90-4_10-result.root

./finalAnalysis.sh output/DYMix0_90 1_2  --MinTrackPT 1 --MaxTrackPT 2  --MinHiBin 0 --MaxHiBin 180  --Input mergedSample/DY-v7.root --MixFile mergedSample/hydjet-v6.root  --IsPP false --IsGenZ false --nMix 1 --IsSelfMixing false --Shift 680
./finalAnalysis.sh output/DYMix0_90 2_4  --MinTrackPT 2 --MaxTrackPT 4  --MinHiBin 0 --MaxHiBin 180  --Input mergedSample/DY-v7.root --MixFile mergedSample/hydjet-v6.root  --IsPP false --IsGenZ false --nMix 1 --IsSelfMixing false --Shift 680
./finalAnalysis.sh output/DYMix0_90 4_10 --MinTrackPT 4 --MaxTrackPT 10 --MinHiBin 0 --MaxHiBin 180  --Input mergedSample/DY-v7.root --MixFile mergedSample/hydjet-v6.root  --IsPP false --IsGenZ false --nMix 1 --IsSelfMixing false --Shift 680

hadd -f plots/DYMix0_90-result.root output/DYMix0_90-1_2-result.root output/DYMix0_90-2_4-result.root output/DYMix0_90-4_10-result.root




