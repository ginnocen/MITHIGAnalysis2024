#ifndef EVTSELCORRECTION
#define EVTSELCORRECTION

#include "TFile.h"
#include "TH2D.h"
#include "TH3F.h"
#include "TMath.h"
#include <iostream>
#include <string>

class EvtSelCorrection{
public:

  EvtSelCorrection( bool isQuiet_ = false ,std::string filePath = "");
  ~EvtSelCorrection();

  float getCorrection(float mult);
  float getEfficiency( float mult, bool passesCheck = false);

private:

  inline bool checkBounds(float mult);

  bool isQuiet;
  std::string filePath;

  TFile * eventEfficiencyFile;
  TH1D * eff;

};

inline bool EvtSelCorrection::checkBounds(float mult){
  if (!eff) {
    if (!isQuiet) std::cout << "EvtSelCorrection: Efficiency histogram not loaded. Returning false." << std::endl;
    return false;
  }

  double minEdge = eff->GetBinLowEdge(1);
  double maxEdge = eff->GetBinLowEdge(eff->GetNbinsX() + 1);

  if (mult < minEdge || mult > maxEdge) {
    if (!isQuiet) std::cout << "EvtSelCorrection: multiplicity is outside the range [" << minEdge << "," << maxEdge << "]. Returning correction factor of 0 for this event for now." << std::endl;
    return false;
  }

  return true;
}

float EvtSelCorrection::getCorrection(float mult){
  // Return 1 if file path is empty string
  if(filePath == ""){
    return -1;
  }
  
  if( !checkBounds(mult) ) return 0;
  
  float efficiency = getEfficiency(mult, true);

  //protect against dividing by 0
  if(efficiency > 0.001){
    return 1 / efficiency;
  } else {
    if( ! isQuiet ) std::cout << "EvtSelCorrection: Warning! Event efficiency is very low for this event (close to dividing by 0).  Returning correction factor of 0 for this event for now." << std::endl;
    return 0;
  }
}

float EvtSelCorrection::getEfficiency(float mult, bool passesCheck){
  if( !passesCheck){
    if(  !checkBounds(mult) ) return 0;
  }

  return eff->GetBinContent( eff->FindBin(mult) );
}

EvtSelCorrection::EvtSelCorrection(bool isQuiet_, std::string filePath_){
  isQuiet = isQuiet_;
  filePath = filePath_;
  eventEfficiencyFile = nullptr;
  eff = nullptr;
  
  if(!isQuiet) std::cout << "EvtSelCorrection class opening in general events mode!" << std::endl;
  
  // Don't try to open file if path is empty
  if(filePath.empty()) {
    if(!isQuiet) std::cout << "EvtSelCorrection: Empty file path provided, no efficiency file will be loaded." << std::endl;
    return;
  }
  
  eventEfficiencyFile = TFile::Open(filePath.c_str());

  if( !eventEfficiencyFile || !(eventEfficiencyFile->IsOpen()) ){
    std::cout << "WARNING, COULD NOT FIND EVENT EFFICIENCY FILE FOR GENERAL EVENTS!" << std::endl;
  } else {
    eff = (TH1D*) eventEfficiencyFile->Get("hEff");
  }

}

EvtSelCorrection::~EvtSelCorrection(){
  eventEfficiencyFile->Close();
}

#endif
