#ifndef TRKEFF2024PPREF
#define TRKEFF2024PPREF

#include "TFile.h"
#include "TH2D.h"
#include "TH3F.h"
#include "TMath.h"
#include <iostream>
#include <string>

class TrkEff2024ppref{
public:

  TrkEff2024ppref( bool isQuiet_ = false ,std::string filePath = "");
  ~TrkEff2024ppref();

  float getCorrection(float pt, float eta);
  float getEfficiency( float pt, float eta, bool passesCheck = false);
  float getFake( float pt, float eta, bool passesCheck = false);
  float getSecondary( float pt, float eta, bool passesCheck = false);
  float getMultipleReco( float pt, float eta, bool passesCheck = false);


private:

  inline bool checkBounds(float pt, float eta);

  bool isQuiet;

  TFile * trkEff;
  TH2F * eff;
  TH2F * fake;
  TH2F * sec;
  TH2F * mul;

};

inline bool TrkEff2024ppref::checkBounds(float pt, float eta){
  if (!eff) {
    if (!isQuiet) std::cout << "TrkEff2024ppref: Efficiency histogram not loaded. Returning false." << std::endl;
    return false;
  }

  // Get eta axis range
  double etaMin = eff->GetXaxis()->GetBinLowEdge(1);
  double etaMax = eff->GetXaxis()->GetBinUpEdge(eff->GetXaxis()->GetNbins());

  // Get pt axis range
  double ptMin = eff->GetYaxis()->GetBinLowEdge(1);
  double ptMax = eff->GetYaxis()->GetBinUpEdge(eff->GetYaxis()->GetNbins());

  if (eta < etaMin || eta > etaMax) {
    if (!isQuiet) std::cout << "TrkEff2024ppref: track outside eta range [" << etaMin << "," << etaMax << "], returning correction factor of 0." << std::endl;
    return false;
  }

  if (pt < ptMin || pt > ptMax) {
    if (!isQuiet) std::cout << "TrkEff2024ppref: pT outside range [" << ptMin << "," << ptMax << "], returning correction factor of 0." << std::endl;
    return false;
  }

  return true;
}

float TrkEff2024ppref::getCorrection(float pt, float eta){
  if( !checkBounds(pt, eta) ) return 0;
  
  float efficiency = getEfficiency(pt, eta, true);
  float fake = getFake(pt, eta, true);
  float secondary = getSecondary(pt, eta, true);
  float multipleReco = getMultipleReco(pt, eta, true);

  //protect against dividing by 0
  if(efficiency > 0.001){
    return (1 - fake) * (1 - secondary) / (efficiency * (1 + multipleReco));
  } else {
    if( ! isQuiet ) std::cout << "TrkEff2024ppref: Warning! Tracking efficiency is very low for this track (close to dividing by 0).  Returning correction factor of 0 for this track for now." << std::endl;
    return 0;
  }
}

float TrkEff2024ppref::getEfficiency( float pt, float eta, bool passesCheck){
  if( !passesCheck){
    if(  !checkBounds(pt, eta) ) return 0;
  }

  return eff->GetBinContent( eff->FindBin(eta, pt) );
}

float TrkEff2024ppref::getFake( float pt, float eta,  bool passesCheck){
  if( !passesCheck){
    if(  !checkBounds(pt, eta) ) return 0;
  }

  return fake->GetBinContent( fake->FindBin(eta, pt) );
}

float TrkEff2024ppref::getSecondary( float pt, float eta, bool passesCheck){
  if( !passesCheck){
    if(  !checkBounds(pt, eta) ) return 0;
  }

  return sec->GetBinContent( sec->FindBin(eta, pt) );
}

float TrkEff2024ppref::getMultipleReco( float pt, float eta, bool passesCheck){
  if( !passesCheck){
    if(  !checkBounds(pt, eta) ) return 0;
  }

  return mul->GetBinContent( mul->FindBin(eta, pt) );
}


TrkEff2024ppref::TrkEff2024ppref(bool isQuiet_, std::string filePath){
  isQuiet = isQuiet_;
    if(!isQuiet) std::cout << "TrkEff2024ppref class opening in general tracks mode!" << std::endl;
    
    trkEff = TFile::Open(filePath.c_str(),"open");
    
    if( !(trkEff->IsOpen() ) ){
      std::cout << "WARNING, COULD NOT FIND TRACK EFFICIENCY FILE FOR GENERAL TRACKS!" << std::endl;
    } else {
      eff = (TH2F*) trkEff->Get("hEff_2D");
      fake = (TH2F*) trkEff->Get("hFak_2D");
      sec = (TH2F*) trkEff->Get("hSec_2D");
      mul = (TH2F*) trkEff->Get("hMul_2D");
    }

}

TrkEff2024ppref::~TrkEff2024ppref(){
  trkEff->Close();
}

#endif
