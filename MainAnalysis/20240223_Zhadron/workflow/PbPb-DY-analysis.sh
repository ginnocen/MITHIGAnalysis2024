./finalAnalysis.sh output/$1DY0_30 1_2  $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} --MinZPT 40 --MaxZPT 350 -MinTrackPT 1 --MaxTrackPT 2  --MinHiBin 0 --MaxHiBin 60  --Input mergedSample/DY-v6.root --MixFile mergedSample/DY-v6.root  --IsPP false --IsGenZ false --nMix 10
./finalAnalysis.sh output/$1DY0_30 2_4  $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} --MinZPT 40 --MaxZPT 350 -MinTrackPT 2 --MaxTrackPT 4  --MinHiBin 0 --MaxHiBin 60  --Input mergedSample/DY-v6.root --MixFile mergedSample/DY-v6.root  --IsPP false --IsGenZ false --nMix 10
./finalAnalysis.sh output/$1DY0_30 4_10 $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} --MinZPT 40 --MaxZPT 350 -MinTrackPT 4 --MaxTrackPT 10 --MinHiBin 0 --MaxHiBin 60  --Input mergedSample/DY-v6.root --MixFile mergedSample/DY-v6.root  --IsPP false --IsGenZ false --nMix 10

hadd -f plots/$1DY0_30-result.root output/$1DY0_30-1_2-result.root output/$1DY0_30-2_4-result.root output/$1DY0_30-4_10-result.root

./finalAnalysis.sh output/$1DY30_50 1_2  $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} --MinZPT 40 --MaxZPT 350   -MinTrackPT 1 --MaxTrackPT 2  --MinHiBin 60 --MaxHiBin 100  --Input mergedSample/DY-v6.root --MixFile mergedSample/DY-v6.root  --IsPP false --IsGenZ false --nMix 10
./finalAnalysis.sh output/$1DY30_50 2_4  $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} --MinZPT 40 --MaxZPT 350  --MinTrackPT 2 --MaxTrackPT 4  --MinHiBin 60 --MaxHiBin 100  --Input mergedSample/DY-v6.root --MixFile mergedSample/DY-v6.root  --IsPP false --IsGenZ false --nMix 10
./finalAnalysis.sh output/$1DY30_50 4_10 $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} --MinZPT 40 --MaxZPT 350  --MinTrackPT 4 --MaxTrackPT 10 --MinHiBin 60 --MaxHiBin 100  --Input mergedSample/DY-v6.root --MixFile mergedSample/DY-v6.root  --IsPP false --IsGenZ false --nMix 10

hadd -f plots/$1DY30_50-result.root output/$1DY30_50-1_2-result.root output/$1DY30_50-2_4-result.root output/$1DY30_50-4_10-result.root

./finalAnalysis.sh output/$1DY50_90 1_2  $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11}  --MinZPT 40 --MaxZPT 350 -MinTrackPT 1 --MaxTrackPT 2  --MinHiBin 100 --MaxHiBin 180  --Input mergedSample/DY-v6.root --MixFile mergedSample/DY-v6.root  --IsPP false --IsGenZ false --nMix 10
./finalAnalysis.sh output/$1DY50_90 2_4  $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11}  --MinZPT 40 --MaxZPT 350 -MinTrackPT 2 --MaxTrackPT 4  --MinHiBin 100 --MaxHiBin 180  --Input mergedSample/DY-v6.root --MixFile mergedSample/DY-v6.root  --IsPP false --IsGenZ false --nMix 10
./finalAnalysis.sh output/$1DY50_90 4_10 $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11}  --MinZPT 40 --MaxZPT 350 -MinTrackPT 4 --MaxTrackPT 10 --MinHiBin 100 --MaxHiBin 180  --Input mergedSample/DY-v6.root --MixFile mergedSample/DY-v6.root  --IsPP false --IsGenZ false --nMix 10

hadd -f plots/$1DY50_90-result.root output/$1DY50_90-1_2-result.root output/$1DY50_90-2_4-result.root output/$1DY50_90-4_10-result.root

./finalAnalysis.sh output/$1DY0_90 1_2   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} --MinZPT 40 --MaxZPT 350 -MinTrackPT 1 --MaxTrackPT 2  --MinHiBin 0 --MaxHiBin 180  --Input mergedSample/DY-v6.root --MixFile mergedSample/DY-v6.root  --IsPP false --IsGenZ false --nMix 10
./finalAnalysis.sh output/$1DY0_90 2_4   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} --MinZPT 40 --MaxZPT 350 -MinTrackPT 2 --MaxTrackPT 4  --MinHiBin 0 --MaxHiBin 180  --Input mergedSample/DY-v6.root --MixFile mergedSample/DY-v6.root  --IsPP false --IsGenZ false --nMix 10
./finalAnalysis.sh output/$1DY0_90 4_10  $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} --MinZPT 40 --MaxZPT 350 -MinTrackPT 4 --MaxTrackPT 10 --MinHiBin 0 --MaxHiBin 180  --Input mergedSample/DY-v6.root --MixFile mergedSample/DY-v6.root  --IsPP false --IsGenZ false --nMix 10

hadd -f plots/$1DY0_90-result.root output/$1DY0_90-1_2-result.root output/$1DY0_90-2_4-result.root output/$1DY0_90-4_10-result.root


./finalAnalysis.sh output/$1DY0_90_y0_1 1_2   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} --MinZY 0 --MaxZY 1 --MinZPT 40 --MaxZPT 350 -MinTrackPT 1 --MaxTrackPT 2  --MinHiBin 0 --MaxHiBin 180  --Input mergedSample/DY-v6.root --MixFile mergedSample/DY-v6.root  --IsPP false --IsGenZ false --nMix 10
./finalAnalysis.sh output/$1DY0_90_y0_1 2_4   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} --MinZY 0 --MaxZY 1 --MinZPT 40 --MaxZPT 350 -MinTrackPT 2 --MaxTrackPT 4  --MinHiBin 0 --MaxHiBin 180  --Input mergedSample/DY-v6.root --MixFile mergedSample/DY-v6.root  --IsPP false --IsGenZ false --nMix 10
./finalAnalysis.sh output/$1DY0_90_y0_1 4_10  $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} --MinZY 0 --MaxZY 1 --MinZPT 40 --MaxZPT 350 -MinTrackPT 4 --MaxTrackPT 10 --MinHiBin 0 --MaxHiBin 180  --Input mergedSample/DY-v6.root --MixFile mergedSample/DY-v6.root  --IsPP false --IsGenZ false --nMix 10

hadd -f plots/$1DY0_90_y0_1-result.root output/$1DY0_90_y0_1-1_2-result.root output/$1DY0_90_y0_1-2_4-result.root output/$1DY0_90_y0_1-4_10-result.root

./finalAnalysis.sh output/$1DY0_90_y1_10 1_2   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} --MinZY 1 --MaxZY 10 --MinZPT 40 --MaxZPT 350 -MinTrackPT 1 --MaxTrackPT 2  --MinHiBin 0 --MaxHiBin 180  --Input mergedSample/DY-v6.root --MixFile mergedSample/DY-v6.root  --IsPP false --IsGenZ false --nMix 10
./finalAnalysis.sh output/$1DY0_90_y1_10 2_4   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} --MinZY 1 --MaxZY 10 --MinZPT 40 --MaxZPT 350 -MinTrackPT 2 --MaxTrackPT 4  --MinHiBin 0 --MaxHiBin 180  --Input mergedSample/DY-v6.root --MixFile mergedSample/DY-v6.root  --IsPP false --IsGenZ false --nMix 10
./finalAnalysis.sh output/$1DY0_90_y1_10 4_10  $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} --MinZY 1 --MaxZY 10 --MinZPT 40 --MaxZPT 350 -MinTrackPT 4 --MaxTrackPT 10 --MinHiBin 0 --MaxHiBin 180  --Input mergedSample/DY-v6.root --MixFile mergedSample/DY-v6.root  --IsPP false --IsGenZ false --nMix 10

hadd -f plots/$1DY0_90_y1_10-result.root output/$1DY0_90_y1_10-1_2-result.root output/$1DY0_90_y1_10-2_4-result.root output/$1DY0_90_y1_10-4_10-result.root



