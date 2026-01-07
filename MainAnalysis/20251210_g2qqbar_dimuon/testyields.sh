source clean.sh

./ExecuteYield \
    --Input "testdistros_soft.root" \
    --Templates "testdistros_soft.root" \
    --Output "testyields_soft.root" \
    --ptBins 60,80,100,120,160,200,250,300 \
    --doJpsi true \
    --doLF true \
    --makeplots true \

