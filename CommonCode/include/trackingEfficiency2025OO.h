#ifndef TRKEFF2025OO
#define TRKEFF2025OO

#include "TFile.h"
#include "TH2D.h"
#include "TH3F.h"
#include "TMath.h"
#include <iostream>
#include <string>

class TrkEff2025OO{
public:

  TrkEff2025OO( bool isQuiet_ = false ,std::string filePath_pythiahijing = "", std::string filePath_hijing = "");
  ~TrkEff2025OO();

  float getCorrection(float pt, float eta);
  float getEfficiency( float pt, float eta, bool passesCheck = false);
  float getFake( float pt, float eta, bool passesCheck = false);
  float getSecondary( float pt, float eta, bool passesCheck = false);
  float getMultipleReco( float pt, float eta, bool passesCheck = false);


private:

  inline bool checkBounds(float pt, float eta);

  bool isQuiet;

  TFile * trkEffSecMul;
  TFile * trkFak;
  TH2F * eff;
  TH2F * fake;
  TH2F * sec;
  TH2F * mul;

};

inline bool TrkEff2025OO::checkBounds(float pt, float eta){
  if (!eff) {
    if (!isQuiet) std::cout << "TrkEff2025OO: Efficiency histogram not loaded. Returning false." << std::endl;
    return false;
  }

  // Get eta axis range
  double etaMin = eff->GetXaxis()->GetBinLowEdge(1);
  double etaMax = eff->GetXaxis()->GetBinUpEdge(eff->GetXaxis()->GetNbins());

  // Get pt axis range
  double ptMin = eff->GetYaxis()->GetBinLowEdge(1);
  double ptMax = eff->GetYaxis()->GetBinUpEdge(eff->GetYaxis()->GetNbins());

  if (eta < etaMin || eta > etaMax) {
    if (!isQuiet) std::cout << "TrkEff2025OO: track outside eta range [" << etaMin << "," << etaMax << "], returning correction factor of 0." << std::endl;
    return false;
  }

  if (pt < ptMin || pt > ptMax) {
    if (!isQuiet) std::cout << "TrkEff2025OO: pT outside range [" << ptMin << "," << ptMax << "], returning correction factor of 0." << std::endl;
    return false;
  }

  return true;
}

float TrkEff2025OO::getCorrection(float pt, float eta){
  if( !checkBounds(pt, eta) ) return 0;
  
  float efficiency = getEfficiency(pt, eta, true);
  float fake = getFake(pt, eta, true);
  float secondary = getSecondary(pt, eta, true);
  float multipleReco = getMultipleReco(pt, eta, true);

  //protect against dividing by 0
  if(efficiency > 0.001){
    return (1 - fake) * (1 - secondary) / (efficiency * (1 + multipleReco));
  } else {
    if( ! isQuiet ) std::cout << "TrkEff2025OO: Warning! Tracking efficiency is very low for this track (close to dividing by 0).  Returning correction factor of 0 for this track for now." << std::endl;
    return 0;
  }
}

float TrkEff2025OO::getEfficiency( float pt, float eta, bool passesCheck){
  if( !passesCheck){
    if(  !checkBounds(pt, eta) ) return 0;
  }

  return eff->GetBinContent( eff->FindBin(eta, pt) );
}

float TrkEff2025OO::getFake( float pt, float eta,  bool passesCheck){
  if( !passesCheck){
    if(  !checkBounds(pt, eta) ) return 0;
  }

  return fake->GetBinContent( fake->FindBin(eta, pt) );
}

float TrkEff2025OO::getSecondary( float pt, float eta, bool passesCheck){
  if( !passesCheck){
    if(  !checkBounds(pt, eta) ) return 0;
  }

  return sec->GetBinContent( sec->FindBin(eta, pt) );
}

float TrkEff2025OO::getMultipleReco( float pt, float eta, bool passesCheck){
  if( !passesCheck){
    if(  !checkBounds(pt, eta) ) return 0;
  }

  return mul->GetBinContent( mul->FindBin(eta, pt) );
}


TrkEff2025OO::TrkEff2025OO(bool isQuiet_, std::string filePath_pythiahijing, std::string filePath_hijing){
  isQuiet = isQuiet_;
    if(!isQuiet) std::cout << "TrkEff2025OO class opening in general tracks mode!" << std::endl;

    // Effeciency, Secondary and multpleReco histograms are imported from Pythia+HIJING simulation, but since Pythia+HIJING has unrealistic fake rates, we use the fake rate from HIJIING alone, two input files are needed.
    
    trkEffSecMul = TFile::Open(filePath_pythiahijing.c_str(),"open");

    trkFak = TFile::Open(filePath_hijing.c_str(),"open");
    
    if( !(trkEffSecMul->IsOpen() ) ){
      std::cout << "WARNING, COULD NOT FIND TRACK EFFICIENCY FILE FOR GENERAL TRACKS!" << std::endl;
    } else if( !(trkFak->IsOpen() ) ){
      std::cout << "WARNING, COULD NOT FIND TRACK FAKE RATE FILE FOR GENERAL TRACKS!" << std::endl;
    } else {
      eff = (TH2F*) trkEffSecMul->Get("hEff_2D");
      fake = (TH2F*) trkFak->Get("hFak_2D");
      sec = (TH2F*) trkEffSecMul->Get("hSec_2D");
      mul = (TH2F*) trkEffSecMul->Get("hMul_2D");
    }

}

TrkEff2025OO::~TrkEff2025OO(){
  trkEffSecMul->Close();
  trkFak->Close();
}

#endif
