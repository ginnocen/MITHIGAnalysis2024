source clean.sh

PATHSKIM="Data/EfficiencyData"

INPUTOFFLINEAND="$PATHSKIM/EfficiencyPurityData_N49_trkpt-1_OfflineAND"
INPUTOFFLINEOR="$PATHSKIM/EfficiencyPurityData_N49_trkpt-1_OfflineOR"
INPUTONLINEAND="$PATHSKIM/EfficiencyPurityData_N49_trkpt-1_OnlineAND"
INPUTONLINEOR="$PATHSKIM/EfficiencyPurityData_N49_trkpt-1_OnlineOR"

./PlotExecutable
    --InputOfflineAND $INPUTOFFLINEAND \
    --InputOfflineOR $INPUTOFFLINEOR \
    --InputOnlineAND $INPUTONLINEAND \
    --InputOnlineOR $INPUTONLINEOR \