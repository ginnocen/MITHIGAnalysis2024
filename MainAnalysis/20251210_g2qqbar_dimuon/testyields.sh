source clean.sh

./ExecuteYield \
    --Input "testdistros.root" \
    --Templates "testdistros.root" \
    --Output "testyields.root" \
    --ptBins 60,80,100,120,160,200,250,300 \
    --doLF_DCA true \
    --doLF_invMass true \
    --makeplots true \

echo "DONE WITH YIELD FITTING"

