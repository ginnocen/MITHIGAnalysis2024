//Created C. McGinn, 2024.11.06
//contact cfmcginn@mit.edu/cffionn@skype/cfmcginn@github

//c and cpp dependencies
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

//ROOT
#include "TEnv.h"
#include "TFile.h"
#include "TH1D.h"
#include "TTree.h"

//Local CommonCode
#include "Messenger.h"

std::vector<double> commaSepStrToDVect(std::string inStr)
{
   std::vector<double> vectD = {};
  //Test the vector input
  if(inStr.size() == 0){
    std::cout << __PRETTY_FUNCTION__ << " WARNING: Passed empty string, returning empty vector. check input" << std::endl;
    return vectD;
  }

  if(inStr.substr(inStr.size()-1, 1).find(",") == std::string::npos){
    inStr = inStr + ",";
  }
  
  if(inStr.find(",") == std::string::npos){
    std::cout << __PRETTY_FUNCTION__ << " WARNING: Not a comma separated list, returning empty vector. check input" << std::endl;
    return vectD;
  }

  while(inStr.find(",") != std::string::npos){
    double value = std::stod(inStr.substr(0, inStr.find(",")));
    vectD.push_back(value);
    inStr.replace(0, inStr.find(",")+1, "");
  }
  
  return vectD;
}

int getPosFromBins(double val, std::vector<double> bins)
{
  int retPos = -1;
  if(bins.size() <= 1){
    std::cout << __PRETTY_FUNCTION__ << " WARNING: Given bins.size() = " << bins.size() << ", return -1" << std::endl;
    return retPos;
  }
  
  for(unsigned int bI = 0; bI < bins.size()-1; ++bI){
    if(val >= bins[bI] && val < bins[bI+1]){
      retPos = bI;
      break;
    }
  }

  return retPos;
}

bool binsMaxCheck(std::string binStr, int nBins, int nMaxBins)
{
  if(nBins > nMaxBins){
    std::cout << __PRETTY_FUNCTION__ << " WARNING: Given nBins=" << nBins << " for '' exceeds max. return false" << std::endl;
    return false;
  }

  return true;
}

void bookHist1D(std::string nameStr, std::string titleStr, std::vector<double> histBins, std::vector<double> axisBins, TH1D* hist[])
{
  for(unsigned int bI = 0; bI < histBins.size()-1; ++bI){
    std::string localName = nameStr + std::to_string(bI) + "_h";
    std::string localTitle = Form(titleStr.c_str(), histBins[bI], histBins[bI+1]);

    hist[bI] = new TH1D(localName.c_str(), localTitle.c_str(), axisBins.size()-1, axisBins.data());
  }
  std::string localName = nameStr + "All_h";
  std::string localTitle = Form(titleStr.c_str(), histBins[0], histBins[histBins.size()-1]);
  hist[histBins.size()-1] = new TH1D(localName.c_str(), localTitle.c_str(), axisBins.size()-1, axisBins.data());

  return;
}

std::vector<double> getLinBins(int nbins, double low, double high)
{
  std::vector<double> vect;
  double binWidth = (high - low)/(double)nbins;
  vect.push_back(low);
  for(int bI = 1; bI < nbins; ++bI){
    vect.push_back(low+binWidth*(double)bI);
  }
  vect.push_back(high);
  
  return vect;
}

int DataMCComp(std::string inConfigName)
{
  //config handler - 
  TEnv* config_p = new TEnv(inConfigName.c_str());
  //Grab the input skim file
  std::string skimFileName = config_p->GetValue("SKIMFILENAME", "");
  std::vector<std::string> skimFileNames;

  //Process if its a root or a txt list of root files
  if(skimFileName.find(".root") != std::string::npos) skimFileNames.push_back(skimFileName);
  else if(skimFileName.find(".txt") != std::string::npos){
    std::ifstream inFile(skimFileName);
    std::string tempStr;
    while(std::getline(inFile, tempStr)){
      if(tempStr.find(".root") != std::string::npos) skimFileNames.push_back(skimFileName);
    }
    inFile.close();
  }
  
  //grab dPtBins
  std::vector<double> dPtBins = commaSepStrToDVect(config_p->GetValue("DPTBINS", ""));
  if(dPtBins.size() == 0) return 1;

  //grab dYBins
  std::vector<double> dYBins = commaSepStrToDVect(config_p->GetValue("DYBINS", ""));
  if(dYBins.size() == 0) return 1;

  //Grab dMBins
  Int_t nDMBins = config_p->GetValue("NDMBINS", -1);//nonsense defaults for auto-failure
  Double_t dMBinsLow = config_p->GetValue("DMBINSLOW", -1.0);
  Double_t dMBinsHigh = config_p->GetValue("DMBINSHIGH", -9999.0);
  std::vector<double> dMBins = getLinBins(nDMBins, dMBinsLow, dMBinsHigh);
  
  //Grab the outputfile name, if it exist; else default name
  std::string outFileName = config_p->GetValue("OUTFILENAME", "out.root");
  TFile* outFile_p = new TFile(outFileName.c_str(), "RECREATE");

  const Int_t nMaxBins = 20;
  const Int_t nDPtBins = dPtBins.size()-1;
  const Int_t nDYBins = dYBins.size()-1;

  if(!binsMaxCheck("DPt", nDPtBins, nMaxBins)) return 1;
  if(!binsMaxCheck("DY", nDYBins, nMaxBins)) return 1;
  
  TH1D* rawDPt_h[nMaxBins];
  TH1D* rawDY_h[nMaxBins];
  TH1D* rawDM_h[nMaxBins][nMaxBins];

  bookHist1D("rawDPt_YBin", ";D^{0} cand. p_{T} (GeV), %.1f < y < %.1f; Counts", dYBins, dPtBins, rawDPt_h); 
  bookHist1D("rawDY_PtBin", ";D^{0} cand. y, %.1f < p_{T} < %.1f (GeV); Counts", dPtBins, dYBins, rawDY_h);   

  //For mass, double binned, so iterate over Pt, then Y
  for(Int_t pI = 0; pI < nDPtBins+1; ++pI){
    std::string histName = "rawDM_PtBinAll";
    std::string histTitle = Form(";D^{0} cand. Mass (GeV), %.1f < p_{T} < %.1f (GeV),", dPtBins[0], dPtBins[dPtBins.size()-1]);
    if(pI < nDPtBins){
      histName = "rawDM_PtBin" + std::to_string(pI);
      histTitle = Form(";D^{0} cand. Mass (GeV), %.1f < p_{T} < %.1f (GeV),", dPtBins[pI], dPtBins[pI+1]);
    }

    histName = histName + "_Y";
    histTitle = histTitle + " %.1f < y < %.1f;Counts";
    bookHist1D(histName, histTitle, dYBins, dMBins, rawDM_h[pI]);
  }

  const ULong64_t nEntriesToPrint = TMath::Max(1, config_p->GetValue("NENTRIESTOPRINT", 10000));
  
  //Done handling config input

  ULong64_t totalNEntries = 0;
  
  std::cout << "Pre-processing " << skimFileNames.size() << " skim files...." << std::endl;
  for(unsigned int fI = 0; fI < skimFileNames.size(); ++fI){
    TFile* inFile_p = new TFile(skimFileNames[fI].c_str(), "READ");
    DzeroUPCTreeMessenger *MDzeroUPC = new DzeroUPCTreeMessenger(inFile_p, "Tree");
    totalNEntries += MDzeroUPC->GetEntries();
    delete MDzeroUPC;
    inFile_p->Close();
    delete inFile_p;
  }  
  
  //Process inputs
  std::cout << "Processing " << skimFileNames.size() << " skim files...." << std::endl;
  std::cout << "Total entries: " << totalNEntries << "..." << std::endl; 

  ULong64_t nCurrEntries = 0;
  
  
  for(unsigned int fI = 0; fI < skimFileNames.size(); ++fI){
    //Grab the input file and Tree
    TFile* inFile_p = new TFile(skimFileNames[fI].c_str(), "READ");
    //Use UPC DZeroTree
    DzeroUPCTreeMessenger *MDzeroUPC = new DzeroUPCTreeMessenger(inFile_p, "Tree");

    //Loop over entries
    ULong64_t nEntries = MDzeroUPC->GetEntries();
    for(ULong64_t entry = 0; entry < nEntries; ++entry){   
      if(nCurrEntries%nEntriesToPrint == 0) std::cout << " Entry " << nCurrEntries << "/" << totalNEntries << " = " << Form("%.1f", ((Double_t)100*nCurrEntries)/((Double_t)totalNEntries)) << "%, File " << fI << "/" << skimFileNames.size() << std::endl;
      ++nCurrEntries;
      
      MDzeroUPC->GetEntry(entry);

      for(unsigned int dI = 0; dI < MDzeroUPC->Dpt->size(); ++dI){
	int yPos = getPosFromBins(MDzeroUPC->Dy->at(dI), dYBins);
	int ptPos = getPosFromBins(MDzeroUPC->Dpt->at(dI), dPtBins);
	
	if(yPos >= 0){
	  rawDPt_h[yPos]->Fill(MDzeroUPC->Dpt->at(dI));
	  rawDPt_h[nDYBins]->Fill(MDzeroUPC->Dpt->at(dI));
	}

	if(ptPos >= 0){
	  rawDY_h[ptPos]->Fill(MDzeroUPC->Dy->at(dI));
	  rawDY_h[nDPtBins]->Fill(MDzeroUPC->Dy->at(dI));
	}

	if(ptPos >= 0 && yPos >= 0){
	  rawDM_h[ptPos][yPos]->Fill(MDzeroUPC->Dmass->at(dI));
	  rawDM_h[nDPtBins][yPos]->Fill(MDzeroUPC->Dmass->at(dI));
	  rawDM_h[ptPos][nDYBins]->Fill(MDzeroUPC->Dmass->at(dI));
	  rawDM_h[nDPtBins][nDYBins]->Fill(MDzeroUPC->Dmass->at(dI));
	}
      }
    }

    delete MDzeroUPC;
    
    inFile_p->Close();
    delete inFile_p;
  }//complete file loop

  //outfile handling
  outFile_p->cd();

  for(unsigned int yI = 0; yI < nDYBins+1; ++yI){
    rawDPt_h[yI]->Write("", TObject::kOverwrite);
    delete rawDPt_h[yI];
  }

  for(unsigned int pI = 0; pI < nDPtBins+1; ++pI){
    rawDY_h[pI]->Write("", TObject::kOverwrite);
    delete rawDY_h[pI];    
  }

  for(Int_t pI = 0; pI < nDPtBins+1; ++pI){
    for(unsigned int yI = 0; yI < nDYBins+1; ++yI){
      rawDM_h[pI][yI]->Write("", TObject::kOverwrite);
      delete rawDM_h[pI][yI];    
    }
  }
    
  //Write out the config so there is a record of production
  config_p->Write("config", TObject::kOverwrite);
  delete config_p;
  
  outFile_p->Close();
  delete outFile_p;
  
  std::cout << "DataMCComp complete! return 0" << std::endl;
  return 0;
}

int main(int argc, char* argv[])
{
  if(argc != 2){
    std::cout << "Usage: ./DataMCComp <inConfigFile>. return 1" << std::endl;
    return 1;
  }

  int retVal = 0;
  retVal += DataMCComp(argv[1]);
  return retVal;
}
