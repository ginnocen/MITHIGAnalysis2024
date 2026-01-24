source clean.sh

./ExecuteYield_DRS \
    --Input "../datadistros.root" \
    --Templates "../mcdistros_w.root" \
    --Output "testyields.root" \
    --ptBins 60,80,100,120,160,200,250,300 \
    --doYields_DCA true \
    --doYields_invMass true \
    --doYields_DR true \
    --makeplots true \

echo "DONE WITH YIELD FITTING"

