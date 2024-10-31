#./finalAnalysis.sh output/$1hybridPP 1_2   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11}   --MinZPT 40 --MaxZPT 350 --MinTrackPT 1 --MaxTrackPT 2  --Input mergedSample/hybrid_pp-v8.root --MixFile mergedSample/hybrid_pp-v8.root  --IsPP false --IsGenZ true --nMix 1  --MinHiBin 0 --MaxHiBin 600000  
#./finalAnalysis.sh output/$1hybridPP 2_4   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11}   --MinZPT 40 --MaxZPT 350 --MinTrackPT 2 --MaxTrackPT 4  --Input mergedSample/hybrid_pp-v8.root --MixFile mergedSample/hybrid_pp-v8.root  --IsPP false --IsGenZ true --nMix 1  --MinHiBin 0 --MaxHiBin 600000  
#./finalAnalysis.sh output/$1hybridPP 4_10  $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11}   --MinZPT 40 --MaxZPT 350 --MinTrackPT 4 --MaxTrackPT 10 --Input mergedSample/hybrid_pp-v8.root --MixFile mergedSample/hybrid_pp-v8.root  --IsPP false --IsGenZ true --nMix 1  --MinHiBin 0 --MaxHiBin 600000  
#hadd -f plots/$1hybridPP-result.root output/$1hybridPP-1_2-result.root output/$1hybridPP-2_4-result.root output/$1hybridPP-4_10-result.root
#hadd -f plots/$1hybridPP-nosub.root output/$1hybridPP-1_2-nosub.root output/$1hybridPP-2_4-nosub.root output/$1hybridPP-4_10-nosub.root


#./finalAnalysis.sh output/$1hybridPbPb030 1_2   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11}   --MinZPT 40 --MaxZPT 350 --MinTrackPT 1 --MaxTrackPT 2  --Input mergedSample/hybrid_PbPb030-v8.root --MixFile mergedSample/hybrid_PbPb030-v8.root  --IsPP false --IsGenZ true --nMix 1  --MinHiBin 0 --MaxHiBin 600000  
#./finalAnalysis.sh output/$1hybridPbPb030 2_4   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11}   --MinZPT 40 --MaxZPT 350 --MinTrackPT 2 --MaxTrackPT 4  --Input mergedSample/hybrid_PbPb030-v8.root --MixFile mergedSample/hybrid_PbPb030-v8.root  --IsPP false --IsGenZ true --nMix 1  --MinHiBin 0 --MaxHiBin 600000  
#./finalAnalysis.sh output/$1hybridPbPb030 4_10  $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11}   --MinZPT 40 --MaxZPT 350 --MinTrackPT 4 --MaxTrackPT 10 --Input mergedSample/hybrid_PbPb030-v8.root --MixFile mergedSample/hybrid_PbPb030-v8.root  --IsPP false --IsGenZ true --nMix 1  --MinHiBin 0 --MaxHiBin 600000  
#hadd -f plots/$1hybridPbPb030-result.root output/$1hybridPbPb030-1_2-result.root output/$1hybridPbPb030-2_4-result.root output/$1hybridPbPb030-4_10-result.root
#hadd -f plots/$1hybridPbPb030-nosub.root output/$1hybridPbPb030-1_2-nosub.root output/$1hybridPbPb030-2_4-nosub.root output/$1hybridPbPb030-4_10-nosub.root

#./finalAnalysis.sh output/$1hybridPbPb3050 1_2   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11}   --MinZPT 40 --MaxZPT 350 --MinTrackPT 1 --MaxTrackPT 2  --Input mergedSample/hybrid_PbPb3050-v8.root --MixFile mergedSample/hybrid_PbPb3050-v8.root  --IsPP false --IsGenZ true --nMix 1  --MinHiBin 0 --MaxHiBin 600000  
#./finalAnalysis.sh output/$1hybridPbPb3050 2_4   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11}   --MinZPT 40 --MaxZPT 350 --MinTrackPT 2 --MaxTrackPT 4  --Input mergedSample/hybrid_PbPb3050-v8.root --MixFile mergedSample/hybrid_PbPb3050-v8.root  --IsPP false --IsGenZ true --nMix 1  --MinHiBin 0 --MaxHiBin 600000  
#./finalAnalysis.sh output/$1hybridPbPb3050 4_10  $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11}   --MinZPT 40 --MaxZPT 350 --MinTrackPT 4 --MaxTrackPT 10 --Input mergedSample/hybrid_PbPb3050-v8.root --MixFile mergedSample/hybrid_PbPb3050-v8.root  --IsPP false --IsGenZ true --nMix 1  --MinHiBin 0 --MaxHiBin 600000  
#hadd -f plots/$1hybridPbPb3050-result.root output/$1hybridPbPb3050-1_2-result.root output/$1hybridPbPb3050-2_4-result.root output/$1hybridPbPb3050-4_10-result.root
#hadd -f plots/$1hybridPbPb3050-nosub.root output/$1hybridPbPb3050-1_2-nosub.root output/$1hybridPbPb3050-2_4-nosub.root output/$1hybridPbPb3050-4_10-nosub.root


#./finalAnalysis.sh output/$1hybridPbPbPositiveOnly030 1_2   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11}   --MinZPT 40 --MaxZPT 350 --MinTrackPT 1 --MaxTrackPT 2  --Input mergedSample/hybrid_PbPb030-v8.root --MixFile mergedSample/hybrid_PbPb030-v8.root  --IsPP false --IsGenZ true --nMix 1  --MinHiBin 0 --MaxHiBin 600000   --includeHole 0
#./finalAnalysis.sh output/$1hybridPbPbPositiveOnly030 2_4   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11}   --MinZPT 40 --MaxZPT 350 --MinTrackPT 2 --MaxTrackPT 4  --Input mergedSample/hybrid_PbPb030-v8.root --MixFile mergedSample/hybrid_PbPb030-v8.root  --IsPP false --IsGenZ true --nMix 1  --MinHiBin 0 --MaxHiBin 600000   --includeHole 0
#./finalAnalysis.sh output/$1hybridPbPbPositiveOnly030 4_10  $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11}   --MinZPT 40 --MaxZPT 350 --MinTrackPT 4 --MaxTrackPT 10 --Input mergedSample/hybrid_PbPb030-v8.root --MixFile mergedSample/hybrid_PbPb030-v8.root  --IsPP false --IsGenZ true --nMix 1  --MinHiBin 0 --MaxHiBin 600000   --includeHole 0
#hadd -f plots/$1hybridPbPbPositiveOnly030-result.root output/$1hybridPbPbPositiveOnly030-1_2-result.root output/$1hybridPbPbPositiveOnly030-2_4-result.root output/$1hybridPbPbPositiveOnly030-4_10-result.root
#hadd -f plots/$1hybridPbPbPositiveOnly030-nosub.root output/$1hybridPbPbPositiveOnly030-1_2-nosub.root output/$1hybridPbPbPositiveOnly030-2_4-nosub.root output/$1hybridPbPbPositiveOnly030-4_10-nosub.root


./finalAnalysis.sh output/$1hybridPbPbNoWake030 1_2   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11}   --MinZPT 40 --MaxZPT 350 --MinTrackPT 1 --MaxTrackPT 2  --Input mergedSample/hybrid_WakeZeroWeight_PbPb030-v8.root --MixFile mergedSample/hybrid_WakeZeroWeight_PbPb030-v8.root  --IsPP false --IsGenZ true --nMix 1  --MinHiBin 0 --MaxHiBin 600000   --includeHole 0
./finalAnalysis.sh output/$1hybridPbPbNoWake030 2_4   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11}   --MinZPT 40 --MaxZPT 350 --MinTrackPT 2 --MaxTrackPT 4  --Input mergedSample/hybrid_WakeZeroWeight_PbPb030-v8.root --MixFile mergedSample/hybrid_WakeZeroWeight_PbPb030-v8.root  --IsPP false --IsGenZ true --nMix 1  --MinHiBin 0 --MaxHiBin 600000   --includeHole 0
./finalAnalysis.sh output/$1hybridPbPbNoWake030 4_10  $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11}   --MinZPT 40 --MaxZPT 350 --MinTrackPT 4 --MaxTrackPT 10 --Input mergedSample/hybrid_WakeZeroWeight_PbPb030-v8.root --MixFile mergedSample/hybrid_WakeZeroWeight_PbPb030-v8.root  --IsPP false --IsGenZ true --nMix 1  --MinHiBin 0 --MaxHiBin 600000   --includeHole 0
hadd -f plots/$1hybridPbPbNoWake030-result.root output/$1hybridPbPbNoWake030-1_2-result.root output/$1hybridPbPbNoWake030-2_4-result.root output/$1hybridPbPbNoWake030-4_10-result.root
hadd -f plots/$1hybridPbPbNoWake030-nosub.root output/$1hybridPbPbNoWake030-1_2-nosub.root output/$1hybridPbPbNoWake030-2_4-nosub.root output/$1hybridPbPbNoWake030-4_10-nosub.root


#./finalAnalysis.sh output/pp_y0_1 1_2   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} --MinZY 0 --MaxZY 1  --MinZPT 40 --MaxZPT 350 --MinTrackPT 1 --MaxTrackPT 2  --Input mergedSample/hybrid_PbPb3050-v8.root --MixFile mergedSample/hybrid_PbPb3050-v8.root  --IsPP true --IsGenZ true --nMix 1
#./finalAnalysis.sh output/pp_y0_1 2_4   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} --MinZY 0 --MaxZY 1  --MinZPT 40 --MaxZPT 350 --MinTrackPT 2 --MaxTrackPT 4  --Input mergedSample/hybrid_PbPb3050-v8.root --MixFile mergedSample/hybrid_PbPb3050-v8.root  --IsPP true --IsGenZ true --nMix 1
#./finalAnalysis.sh output/pp_y0_1 4_10  $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} --MinZY 0 --MaxZY 1  --MinZPT 40 --MaxZPT 350 --MinTrackPT 4 --MaxTrackPT 10 --Input mergedSample/hybrid_PbPb3050-v8.root --MixFile mergedSample/hybrid_PbPb3050-v8.root  --IsPP true --IsGenZ true --nMix 1
#hadd -f plots/$1pp_y0_1-result.root output/$1pp_y0_1-1_2-result.root output/$1pp_y0_1-2_4-result.root output/$1pp_y0_1-4_10-result.root
#hadd -f plots/$1pp_y0_1-nosub.root output/$1pp_y0_1-1_2-nosub.root output/$1pp_y0_1-2_4-nosub.root output/$1pp_y0_1-4_10-nosub.root


#./finalAnalysis.sh output/pp_y1_10 1_2   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} --MinZY 1 --MaxZY 10  --MinZPT 40 --MaxZPT 350 --MinTrackPT 1 --MaxTrackPT 2  --Input mergedSample/hybrid_PbPb3050-v8.root --MixFile mergedSample/hybrid_PbPb3050-v8.root  --IsPP true --IsGenZ true --nMix 1
#./finalAnalysis.sh output/pp_y1_10 2_4   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} --MinZY 1 --MaxZY 10  --MinZPT 40 --MaxZPT 350 --MinTrackPT 2 --MaxTrackPT 4  --Input mergedSample/hybrid_PbPb3050-v8.root --MixFile mergedSample/hybrid_PbPb3050-v8.root  --IsPP true --IsGenZ true --nMix 1
#./finalAnalysis.sh output/pp_y1_10 4_10  $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} --MinZY 1 --MaxZY 10  --MinZPT 40 --MaxZPT 350 --MinTrackPT 4 --MaxTrackPT 10 --Input mergedSample/hybrid_PbPb3050-v8.root --MixFile mergedSample/hybrid_PbPb3050-v8.root  --IsPP true --IsGenZ true --nMix 1
#hadd -f plots/$1pp_y1_10-result.root output/$1pp_y1_10-1_2-result.root output/$1pp_y1_10-2_4-result.root output/$1pp_y1_10-4_10-result.root
#hadd -f plots/$1pp_y1_10-nosub.root output/$1pp_y1_10-1_2-nosub.root output/$1pp_y1_10-2_4-nosub.root output/$1pp_y1_10-4_10-nosub.root

