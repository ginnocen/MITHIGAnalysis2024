#ifndef __PHOD_DTREE__
#define __PHOD_DTREE__

#include <TTree.h>
#include <TDirectory.h>
#include <TFile.h>
#include <TH2F.h>

#include <vector>
#include <map>
#include <string>
#include <type_traits>

#include "xjjcuti.h"

namespace hfupc
{
  class dtree
  {
  public:
    dtree(DzeroTreeMessenger* nt);
    dtree(TTree* nt) { ; }
    DzeroTreeMessenger* nt() { return nt_; }

    // read
    template<typename T = float> T val(const std::string& br, int j);
    bool has(const std::string& br);

  private:
    //Members set in constructor
    DzeroTreeMessenger* nt_;

    void setbranchaddress();

    std::map<std::string, float*> bvf_;
    std::map<std::string, int*> bvi_;
    std::map<std::string, bool*> bvo_;
    
    // status
    std::map<std::string, bool> bvs_; //

  };
}

hfupc::dtree::dtree(DzeroTreeMessenger* nt) :
  nt_(nt) {

  setbranchaddress();
  for (auto& [v, dptr] : bvf_) { bvs_[v] = (bool)dptr; }
  for (auto& [v, dptr] : bvi_) { bvs_[v] = (bool)dptr; }
  for (auto& [v, dptr] : bvo_) { bvs_[v] = (bool)dptr; }

  // xjjc::print_tab(bvs_, 0);
}

bool hfupc::dtree::has(const std::string& br) {
  bool r = false;
  if (bvs_.find(br) != bvs_.end()) {
    r = bvs_.at(br);
  }
  return r;
}

void hfupc::dtree::setbranchaddress() {
  
  bvf_["Dmass"] = nt_->Dmass;
  bvf_["Dpt"] = nt_->Dpt;
  bvf_["Dphi"] = nt_->Dphi;
  bvf_["Dy"] = nt_->Dy;
  bvf_["Dchi2cl"] = nt_->Dchi2cl;
  bvf_["Ddtheta"] = nt_->Ddtheta;
  bvf_["Dalpha"] = nt_->Dalpha;
  bvf_["DsvpvDistance"] = nt_->DsvpvDistance;
  bvf_["DsvpvDisErr"] = nt_->DsvpvDisErr;
  bvf_["DsvpvDistance_2D"] = nt_->DsvpvDistance_2D;
  bvf_["DsvpvDisErr_2D"] = nt_->DsvpvDisErr_2D;
  bvf_["Dtrk1Pt"] = nt_->Dtrk1Pt;
  bvf_["Dtrk2Pt"] = nt_->Dtrk2Pt;
  bvf_["Dtrk1PtErr"] = nt_->Dtrk1PtErr;
  bvf_["Dtrk2PtErr"] = nt_->Dtrk2PtErr;
  bvf_["Dtrk1Eta"] = nt_->Dtrk1Eta;
  bvf_["Dtrk2Eta"] = nt_->Dtrk2Eta;
  bvf_["Dtrk1PixelHit"] = nt_->Dtrk1PixelHit;
  bvf_["Dtrk2PixelHit"] = nt_->Dtrk2PixelHit;
  bvf_["Dtrk1StripHit"] = nt_->Dtrk1StripHit;
  bvf_["Dtrk2StripHit"] = nt_->Dtrk2StripHit;
  bvi_["Dgen"] = nt_->Dgen;

}

template<typename T> T hfupc::dtree::val(const std::string& br, int j) {
  if (!has(br)) {
    std::cout<<__FUNCTION__<<" error: bad branch ["<<br<<"]."<<std::endl;
    return static_cast<T>(0);
  } 
  if (std::is_same<T, float>::value) { return bvf_.at(br)[j]; }
  if (std::is_same<T, int>::value) { return bvi_.at(br)[j]; }
  if (std::is_same<T, bool>::value) { return bvo_.at(br)[j]; }

  std::cout<<__FUNCTION__<<" error: bad type for branch ["<<br<<"]."<<std::endl; 
  return static_cast<T>(0);
}

#endif
