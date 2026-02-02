source clean.sh

./ExecuteYield \
    --Input "mcdistros_nopthat.root" \
    --Templates "mcdistros_nopthat.root" \
    --Output "testyields2.root" \
    --ptBins 100,120,160,200,250 \
    --variables muDR,mumuZ \
    --kde 1.3,1.1 \
    --fitRangeMin 0.0,0.0 \
    --fitRangeMax 0.6,1.0 \
    --chargeSelection 0 \
    --makeplots true \

echo "DONE WITH YIELD FITTING"

