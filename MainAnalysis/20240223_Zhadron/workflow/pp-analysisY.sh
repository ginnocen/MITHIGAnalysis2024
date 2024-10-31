./finalAnalysis.sh output/pp_y0_1 1_2   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} --MinZY 0 --MaxZY 1  --MinZPT 40 --MaxZPT 350 --MinTrackPT 1 --MaxTrackPT 2  --Input mergedSample/pp-v6.root --MixFile mergedSample/pp-v6.root  --IsPP true --IsGenZ false --nMix 10
./finalAnalysis.sh output/pp_y0_1 2_4   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} --MinZY 0 --MaxZY 1  --MinZPT 40 --MaxZPT 350 --MinTrackPT 2 --MaxTrackPT 4  --Input mergedSample/pp-v6.root --MixFile mergedSample/pp-v6.root  --IsPP true --IsGenZ false --nMix 10
./finalAnalysis.sh output/pp_y0_1 4_10  $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} --MinZY 0 --MaxZY 1  --MinZPT 40 --MaxZPT 350 --MinTrackPT 4 --MaxTrackPT 10 --Input mergedSample/pp-v6.root --MixFile mergedSample/pp-v6.root  --IsPP true --IsGenZ false --nMix 10
hadd -f plots/pp_y0_1-result.root output/pp_y0_1-1_2-result.root output/pp_y0_1-2_4-result.root output/pp_y0_1-4_10-result.root

./finalAnalysis.sh output/pp_y1_10 1_2   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} --MinZY 0 --MaxZY 1  --MinZPT 40 --MaxZPT 350 --MinTrackPT 1 --MaxTrackPT 2  --Input mergedSample/pp-v6.root --MixFile mergedSample/pp-v6.root  --IsPP true --IsGenZ false --nMix 10
./finalAnalysis.sh output/pp_y1_10 2_4   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} --MinZY 0 --MaxZY 1  --MinZPT 40 --MaxZPT 350 --MinTrackPT 2 --MaxTrackPT 4  --Input mergedSample/pp-v6.root --MixFile mergedSample/pp-v6.root  --IsPP true --IsGenZ false --nMix 10
./finalAnalysis.sh output/pp_y1_10 4_10  $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} --MinZY 0 --MaxZY 1  --MinZPT 40 --MaxZPT 350 --MinTrackPT 4 --MaxTrackPT 10 --Input mergedSample/pp-v6.root --MixFile mergedSample/pp-v6.root  --IsPP true --IsGenZ false --nMix 10
hadd -f plots/pp_y1_10-result.root output/pp_y1_10-1_2-result.root output/pp_y1_10-2_4-result.root output/pp_y1_10-4_10-result.root

