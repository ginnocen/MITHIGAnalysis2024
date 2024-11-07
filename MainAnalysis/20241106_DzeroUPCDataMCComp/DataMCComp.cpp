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

  //Pt histograms in y bins
  for(unsigned int yI = 0; yI < nDYBins; ++yI){
    std::string histName = Form("rawDPt_YBin%d_h", yI);
    std::string histTitle = Form(";D^{0} cand. p_{T} (GeV), %.1f < y < %.1f; Counts", dYBins[yI], dYBins[yI+1]);
    
    rawDPt_h[yI] = new TH1D(histName.c_str(), histTitle.c_str(), dPtBins.size()-1, dPtBins.data());  
  }
  std::string histName = "rawDPt_YBinAll_h";
  std::string histTitle = Form(";D^{0} cand. p_{T} (GeV), %.1f < y < %.1f; Counts", dYBins[0], dYBins[nDYBins]);
  rawDPt_h[nDYBins] = new TH1D(histName.c_str(), histTitle.c_str(), dPtBins.size()-1, dPtBins.data());  

  //Y histograms in pt bins
  for(unsigned int pI = 0; pI < nDPtBins; ++pI){
    histName = Form("rawDY_PtBin%d_h", pI);
    histTitle = Form(";D^{0} cand. y, %.1f < p_{T} < %.1f (GeV); Counts", dPtBins[pI], dPtBins[pI+1]);
    
    rawDY_h[pI] = new TH1D(histName.c_str(), histTitle.c_str(), dYBins.size()-1, dYBins.data());  
  }
  histName = "rawDY_PtBinAll_h";
  histTitle = Form(";D^{0} cand. y, %.1f < p_{T} < %.1f (GeV); Counts", dPtBins[0], dPtBins[nDPtBins]);
  rawDY_h[nDPtBins] = new TH1D(histName.c_str(), histTitle.c_str(), dYBins.size()-1, dYBins.data());  

  
  //Done handling config input

  //Process inputs
  std::cout << "Processing " << skimFileNames.size() << " skim files...." << std::endl;
  for(unsigned int fI = 0; fI < skimFileNames.size(); ++fI){
    std::cout << " File " << fI << "/" << skimFileNames.size() << " (" << ((Double_t)fI)/(Double_t)skimFileNames.size() << "): " << skimFileNames[fI] << std::endl;

    //Define the variables to be used w/ skim tree
    std::vector<float>* Dpt_p=nullptr;
    std::vector<float>* Dy_p=nullptr;
    
    //Grab the input file and Tree
    TFile* inFile_p = new TFile(skimFileNames[fI].c_str(), "READ");
    //Use UPC DZeroTree
    DzeroUPCTreeMessenger *MDzeroUPC = new DzeroUPCTreeMessenger(inFile_p, "Tree");

    //Loop over entries
    ULong64_t nEntries = MDzeroUPC->GetEntries();
    for(ULong64_t entry = 0; entry < nEntries; ++entry){   
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
