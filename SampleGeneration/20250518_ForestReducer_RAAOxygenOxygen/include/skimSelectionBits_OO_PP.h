#ifndef CHARGEDHADRONRAAOOSKIMSELECTIONBITFUNCS
#define CHARGEDHADRONRAAOOSKIMSELECTIONBITFUNCS

bool getBaselineOOEventSel(ChargedHadronRAATreeMessenger &ch) {
    // zVtx cut
    if (fabs(ch.VZ) >= 15.){ return false; }

    // Cluster Compatibility Filter
    if (ch.ClusterCompatibilityFilter == false) { return false; }

    // PV selection
    if (ch.PVFilter == false) { return false; }

    return true;
}

bool getBaselinePPEventSel(ChargedHadronRAATreeMessenger &ch) {
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

bool checkHFANDCondition(ChargedHadronRAATreeMessenger &ch, float HFE_min1, float HFE_min2, bool useOnlineHFE) {
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
        if (ch.HFEMaxPlus < -9998. || ch.HFEMaxMinus < -9998.) {
            std::cout << "ERROR: Skimmer detected nullptr PF Tree or PF Messenger!" << std::endl;
            return false;
        }
        float hiHF = max(ch.HFEMaxPlus, ch.HFEMaxMinus);
        float loHF = min(ch.HFEMaxPlus, ch.HFEMaxMinus);
        if(hiHF <= HFE_min1 || loHF <= HFE_min2) {
            return false;
        }
    }

    return true;
}

bool checkHFORCondition(ChargedHadronRAATreeMessenger &ch, float HFE_min, bool useOnlineHFE) {
    if (useOnlineHFE) {
        int ohiHF = max(ch.mMaxL1HFAdcPlus, ch.mMaxL1HFAdcMinus);
        if (ohiHF <= HFE_min) {
            return false;
        }
    } else {
        if (ch.HFEMaxPlus < -9998. || ch.HFEMaxMinus < -9998.) {
            std::cout << "ERROR: Skimmer detected nullptr PF Tree or PF Messenger!" << std::endl;
            return false;
        }
        float hiHF = max(ch.HFEMaxPlus, ch.HFEMaxMinus);
        if(hiHF <= HFE_min) {
            return false;
        }
    }

    return true;
}

#endif