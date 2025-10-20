#ifndef REWEIGHTING_H_ASDFASDF
#define REWEIGHTING_H_ASDFASDF

#include "TFile.h"
#include "TH2D.h"
#include "TH3F.h"
#include "TMath.h"
#include <iostream>
#include <string>

//// GENERALIZED MACRO TO REWEIGHT A COLUMN OF FLOATS BY A TH1D, ASSIGNS A FLOAT WEIGHT TO EACH BRANCH
class MCReweighting {
public:
    MCReweighting(bool isQuiet_ = false, std::string filePath = "", std::string histName = "reweightHist");
    ~MCReweighting();

    float getCorrection(float value);

private: 

    inline bool checkBounds(float value);

    bool isQuiet;
    std::string filePath;

    TFile* reweightFile;
    TH1D* reweightHist;
};

inline bool MCReweighting::checkBounds(float value){

    if(!reweightHist){
        if(!isQuiet) std::cout << "MCReweighting: Histogram not loaded. Returning false." << std::endl;
        return false;
    }

    double minEdge = reweightHist->GetBinLowEdge(1);
    double maxEdge = reweightHist->GetBinLowEdge(reweightHist->GetNbinsX() + 1);

    if (value < minEdge || value > maxEdge) {
        if (!isQuiet) std::cout << "Reweighting: value is outside the range [" << minEdge << "," << maxEdge << "]. Returning correction factor of 0 for this event for now." << std::endl;
        return false;
    }

    return true;
}

float MCReweighting::getCorrection(float value) {
    // Return 1 if file path is empty string
    if(filePath == ""){
        return 1;
    }
    
    if (!checkBounds(value)) return 0;

    float weight = reweightHist->GetBinContent(reweightHist->FindBin(value));
    //cout << "MCReweighting: value = " << value << ", weight = " << weight << std::endl;
    return weight;
}

MCReweighting::MCReweighting(bool isQuiet_, std::string filePath_, std::string histName) {

    isQuiet = isQuiet_;
    filePath = filePath_;
    reweightFile = nullptr;
    reweightHist = nullptr;

    // Don't try to open file if path is empty
    if(filePath.empty()) {
        if(!isQuiet) std::cout << "MCReweighting: Empty file path provided, no reweighting file will be loaded." << std::endl;
        return;
    }

    reweightFile = TFile::Open(filePath.c_str());

    if( !reweightFile || !(reweightFile->IsOpen()) ){
      std::cout << "WARNING, COULD NOT FIND REWEIGHTING FILE FOR GENERAL EVENTS!" << std::endl;
    } else {
      reweightHist = (TH1D*) reweightFile->Get(histName.c_str());
      if(!reweightHist){
          std::cout << "WARNING, COULD NOT FIND REWEIGHTING HISTOGRAM '" << histName << "' FOR GENERAL EVENTS!" << std::endl;
      } else {
          if(!isQuiet) std::cout << "Histogram '" << histName << "' loaded successfully with " << reweightHist->GetNbinsX() << " bins" << std::endl;
      }
    }

}

MCReweighting::~MCReweighting(){
    reweightFile->Close();
}

#endif