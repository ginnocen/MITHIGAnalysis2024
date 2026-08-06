#include "Messenger.h"
#include "EventSelection.h"
#include <algorithm>

using namespace std;
bool eventSelection(ChargedHadronRAATreeMessenger *ch, const Parameters &par) {

  // Z Vertex cut 
  if(fabs(ch->VZ) >= par.VzMax){
    return false;
  }

  // NVtx cut
  if(ch->nVtx < par.NVtxMin) {
    return false;
  }

  // Cluster Compatibility Filter
  if(ch->ClusterCompatibilityFilter < par.CCFilter) {
    return false;
  }

  // PV selection
  if(ch->PVFilter < par.PVFilter) {
    return false;
  }
  
  //Offline HFE energy cuts
  float hiHF = max(ch->HFEMaxPlus, ch->HFEMaxMinus);
  float loHF = min(ch->HFEMaxPlus, ch->HFEMaxMinus);
  if(hiHF <= par.HFEmax_Offline_min1 || loHF <= par.HFEmax_Offline_min2) {
    return false;
  }

  //Online HFE energy cuts
  float hiHFOnline = max(ch->mMaxL1HFAdcPlus, ch->mMaxL1HFAdcMinus);
  float loHFOnline = min(ch->mMaxL1HFAdcPlus, ch->mMaxL1HFAdcMinus);
  if(hiHFOnline <= par.HFEmax_Online_min1 || loHFOnline <= par.HFEmax_Online_min2) {
    return false;
  }

  return true;
}
