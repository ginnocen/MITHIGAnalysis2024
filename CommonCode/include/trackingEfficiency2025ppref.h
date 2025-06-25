#ifndef TRKEFF2025PPREF
#define TRKEFF2025PPREF

#include "TFile.h"
#include "TH2D.h"
#include "TH3F.h"
#include "TMath.h"
#include <iostream>
#include <string>

class TrkEff2025ppref{
public:

  TrkEff2025ppref( bool isQuiet_ = false ,std::string filePath = "");
  ~TrkEff2025ppref();

  float getCorrection(float pt, float eta);
  float getEfficiency( float pt, float eta, bool passesCheck = false);
  float getFake( float pt, float eta, bool passesCheck = false);
  float getSecondary( float pt, float eta, bool passesCheck = false);


private:

  inline bool checkBounds(float pt, float eta);

  bool isQuiet;

  TFile * trkEff;
  TH2F * eff;
  TH2F * fake;
  TH2F * sec;

};

inline bool TrkEff2025ppref::checkBounds(float pt, float eta){
  if( TMath::Abs(eta) > 2.4 ){
    if( ! isQuiet) std::cout << "TrkEff2025ppref: track outside |eta|<2.4, please apply this cut!  I am returning a correction factor of 0 for this track for now." << std::endl;
    return false;
  }
  
  if( pt< 0 || pt > 500 ){
    if( ! isQuiet) std::cout << "TrkEff2025ppref: pT is outside the range [0,500].  I am returning a correction factor of 0 for this track for now." << std::endl;
    return false;
  }

  return true;
}

float TrkEff2025ppref::getCorrection(float pt, float eta){
  if( !checkBounds(pt, eta) ) return 0;
  
  float efficiency = getEfficiency(pt, eta, true);
  float fake = getFake(pt, eta, true);
  float secondary = getSecondary(pt, eta, true);

  //protect against dividing by 0
  if(efficiency > 0.001){
    return (1-fake)*(1-secondary)/efficiency;
  } else {
    if( ! isQuiet ) std::cout << "TrkEff2025ppref: Warning! Tracking efficiency is very low for this track (close to dividing by 0).  Returning correction factor of 0 for this track for now." << std::endl;
    return 0;
  }
}

float TrkEff2025ppref::getEfficiency( float pt, float eta, bool passesCheck){
  if( !passesCheck){
    if(  !checkBounds(pt, eta) ) return 0;
  }

  return eff->GetBinContent( eff->FindBin(eta, pt) ) * 0.979;//0.979 is scale factor from D mesons
}

float TrkEff2025ppref::getFake( float pt, float eta,  bool passesCheck){
  if( !passesCheck){
    if(  !checkBounds(pt, eta) ) return 0;
  }

  return fake->GetBinContent( fake->FindBin(eta, pt) );
}

float TrkEff2025ppref::getSecondary( float pt, float eta, bool passesCheck){
  if( !passesCheck){
    if(  !checkBounds(pt, eta) ) return 0;
  }

  return sec->GetBinContent( sec->FindBin(eta, pt) );
}


TrkEff2025ppref::TrkEff2025ppref(bool isQuiet_, std::string filePath){
  isQuiet = isQuiet_;
    if(!isQuiet) std::cout << "TrkEff2025ppref class opening in general tracks mode!" << std::endl;

    std::cout<<"maees it here"<<std::endl;
    
    trkEff = TFile::Open( (filePath + "Eff_ppref_2024_Pythia_QCDptHat15_NopU_2D_vzpthatWeight_Nominal_10thJune2025.root").c_str(),"open");
    
    if( !(trkEff->IsOpen() ) ){
      std::cout << "WARNING, COULD NOT FIND TRACK EFFICIENCY FILE FOR GENERAL TRACKS!" << std::endl;
    } else {
      eff = (TH2F*) trkEff->Get("hEff_2D");
      fake = (TH2F*) trkEff->Get("hFak_2D");
      sec = (TH2F*) trkEff->Get("hSec_2D");
    }

}

TrkEff2025ppref::~TrkEff2025ppref(){
  trkEff->Close();
}

#endif
