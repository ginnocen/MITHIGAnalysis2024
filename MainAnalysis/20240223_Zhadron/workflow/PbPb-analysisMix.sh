./finalAnalysis.sh output/PbPbMix0_30 1_2  --MinTrackPT 1 --MaxTrackPT 2  --MinHiBin 0 --MaxHiBin 60  --Input mergedSample/HISingleMuon-v6.root --MixFile mergedSample/MB-v6.root  --IsPP false --IsGenZ false --nMix 20 --IsSelfMixing false --Shift 645
./finalAnalysis.sh output/PbPbMix0_30 2_4  --MinTrackPT 2 --MaxTrackPT 4  --MinHiBin 0 --MaxHiBin 60  --Input mergedSample/HISingleMuon-v6.root --MixFile mergedSample/MB-v6.root  --IsPP false --IsGenZ false --nMix 20 --IsSelfMixing false --Shift 645
./finalAnalysis.sh output/PbPbMix0_30 4_10 --MinTrackPT 4 --MaxTrackPT 10 --MinHiBin 0 --MaxHiBin 60  --Input mergedSample/HISingleMuon-v6.root --MixFile mergedSample/MB-v6.root  --IsPP false --IsGenZ false --nMix 20 --IsSelfMixing false --Shift 645

hadd -f plots/PbPbMix0_30-result.root output/PbPbMix0_30-1_2-result.root output/PbPbMix0_30-2_4-result.root output/PbPbMix0_30-4_10-result.root

./finalAnalysis.sh output/PbPbMix30_50 1_2  --MinTrackPT 1 --MaxTrackPT 2  --MinHiBin 60 --MaxHiBin 100  --Input mergedSample/HISingleMuon-v6.root --MixFile mergedSample/MB-v6.root  --IsPP false --IsGenZ false --nMix 20 --IsSelfMixing false --Shift 645
./finalAnalysis.sh output/PbPbMix30_50 2_4  --MinTrackPT 2 --MaxTrackPT 4  --MinHiBin 60 --MaxHiBin 100  --Input mergedSample/HISingleMuon-v6.root --MixFile mergedSample/MB-v6.root  --IsPP false --IsGenZ false --nMix 20 --IsSelfMixing false --Shift 645
./finalAnalysis.sh output/PbPbMix30_50 4_10 --MinTrackPT 4 --MaxTrackPT 10 --MinHiBin 60 --MaxHiBin 100  --Input mergedSample/HISingleMuon-v6.root --MixFile mergedSample/MB-v6.root  --IsPP false --IsGenZ false --nMix 20 --IsSelfMixing false --Shift 645

hadd -f plots/PbPbMix30_50-result.root output/PbPbMix30_50-1_2-result.root output/PbPbMix30_50-2_4-result.root output/PbPbMix30_50-4_10-result.root

./finalAnalysis.sh output/PbPbMix50_90 1_2  --MinTrackPT 1 --MaxTrackPT 2  --MinHiBin 100 --MaxHiBin 180  --Input mergedSample/HISingleMuon-v6.root --MixFile mergedSample/MB-v6.root  --IsPP false --IsGenZ false --nMix 20 --IsSelfMixing false --Shift 645
./finalAnalysis.sh output/PbPbMix50_90 2_4  --MinTrackPT 2 --MaxTrackPT 4  --MinHiBin 100 --MaxHiBin 180  --Input mergedSample/HISingleMuon-v6.root --MixFile mergedSample/MB-v6.root  --IsPP false --IsGenZ false --nMix 20 --IsSelfMixing false --Shift 645
./finalAnalysis.sh output/PbPbMix50_90 4_10 --MinTrackPT 4 --MaxTrackPT 10 --MinHiBin 100 --MaxHiBin 180  --Input mergedSample/HISingleMuon-v6.root --MixFile mergedSample/MB-v6.root  --IsPP false --IsGenZ false --nMix 20 --IsSelfMixing false --Shift 645

hadd -f plots/PbPbMix50_90-result.root output/PbPbMix50_90-1_2-result.root output/PbPbMix50_90-2_4-result.root output/PbPbMix50_90-4_10-result.root

./finalAnalysis.sh output/PbPbMix0_90 1_2  --MinTrackPT 1 --MaxTrackPT 2  --MinHiBin 0 --MaxHiBin 180  --Input mergedSample/HISingleMuon-v6.root --MixFile mergedSample/MB-v6.root  --IsPP false --IsGenZ false --nMix 20 --IsSelfMixing false --Shift 645
./finalAnalysis.sh output/PbPbMix0_90 2_4  --MinTrackPT 2 --MaxTrackPT 4  --MinHiBin 0 --MaxHiBin 180  --Input mergedSample/HISingleMuon-v6.root --MixFile mergedSample/MB-v6.root  --IsPP false --IsGenZ false --nMix 20 --IsSelfMixing false --Shift 645
./finalAnalysis.sh output/PbPbMix0_90 4_10 --MinTrackPT 4 --MaxTrackPT 10 --MinHiBin 0 --MaxHiBin 180  --Input mergedSample/HISingleMuon-v6.root --MixFile mergedSample/MB-v6.root  --IsPP false --IsGenZ false --nMix 20 --IsSelfMixing false --Shift 645

hadd -f plots/PbPbMix0_90-result.root output/PbPbMix0_90-1_2-result.root output/PbPbMix0_90-2_4-result.root output/PbPbMix0_90-4_10-result.root




