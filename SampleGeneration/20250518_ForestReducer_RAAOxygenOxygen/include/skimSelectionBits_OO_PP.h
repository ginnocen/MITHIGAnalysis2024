bool getDefaultOOEventSel_WithoutHF(ChargedHadronRAATreeMessenger &ch) {
    // zVtx cut
    if (fabs(ch.VZ) >= 15.){ return false; }

    // Cluster Compatibility Filter
    if (ch.ClusterCompatibilityFilter == false) { return false; }

    // PV selection
    if (ch.PVFilter == false) { return false; }

    return true;
}

bool getDefaultPPEventSel(ChargedHadronRAATreeMessenger &ch) {
    // zVtx cut
    if (fabs(ch.VZ) >= 15.0) { return false; }
  
    // Cluster Compatibility Filter
    if (ch.ClusterCompatibilityFilter == false) { return false; }

    // PV selection
    if (ch.PVFilter == false) { return false;}

    if (ch.isFakeVtx) { return false; }


    if (ch.nTracksVtx < 0) { return false; }

    return true;
}

bool checkHFCondition(ChargedHadronRAATreeMessenger &ch, float HFE_min1, float HFE_min2, bool useOnlineHFE) {
    if (HFE_min1 < HFE_min2) {
        std::cout << "ERROR: HFE_min1 < HFE_min2 in HF selection bit check, please make sure that (HFE_min1 >= HFE_min2) !" << std::endl;
        return false;
    }

    if (useOnlineHFE) {
        int ohiHF = max(ch.mMaxL1HFAdcPlus, ch.mMaxL1HFAdcMinus);
        int oloHF = min(ch.mMaxL1HFAdcPlus, ch.mMaxL1HFAdcMinus);
        if(ohiHF <= HFE_min1 || oloHF <= HFE_min2) {
            return false;
        }
    } else {
        float hiHF = max(ch.HFEMaxPlus, ch.HFEMaxMinus);
        float loHF = min(ch.HFEMaxPlus, ch.HFEMaxMinus);
        if(hiHF <= HFE_min1 || loHF <= HFE_min2) {
            return false;
        }
    }

    return true;
}