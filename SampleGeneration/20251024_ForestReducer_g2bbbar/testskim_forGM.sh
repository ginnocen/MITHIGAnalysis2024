source clean.sh

mkdir -p testSkimMC
nohup ./RunParallelMC_xrdcp.sh /eos/cms/store/group/phys_heavyions/aholterm/g2qqbar/testforest testSkimMC > logs/testSkimMC.log 2>&1 &
