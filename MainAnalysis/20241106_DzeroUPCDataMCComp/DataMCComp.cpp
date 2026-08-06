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

  //Bin handling
  const Int_t nMaxBins = 20;
  const Int_t nGammaBins = 2;
  const Int_t nDPtBins = dPtBins.size()-1;
  const Int_t nDYBins = dYBins.size()-1;

  //bin check, return if failed
  if(!binsMaxCheck("DPt", nDPtBins, nMaxBins)) return 1;
  if(!binsMaxCheck("DY", nDYBins, nMaxBins)) return 1;

  //Define gamma n and n gamma bins
  std::map<std::string, int> nGammaBinsStrMap;
  nGammaBinsStrMap["NGamma"] = 0;
  nGammaBinsStrMap["GammaN"] = 1;

  std::map<std::string, std::string> nGammaBinsLabelMap;
  nGammaBinsLabelMap["NGamma"] = "N+#gamma";
  nGammaBinsLabelMap["GammaN"] = "#gamma+N";

  //Add info to config file
  config_p->SetValue("NGAMMABINS", nGammaBins);
  for(auto const& nGammaBinStr : nGammaBinsStrMap){
    std::string keyStr = "GAMMABIN." + std::to_string(nGammaBinStr.second);
    std::string valStr = nGammaBinStr.first;
    config_p->SetValue(keyStr.c_str(), valStr.c_str());

    keyStr = "GAMMALABEL." + std::to_string(nGammaBinStr.second);
    valStr = nGammaBinsLabelMap[nGammaBinStr.first];
    config_p->SetValue(keyStr.c_str(), valStr.c_str());
  }

  //CutPass handler
  TH1D* cutPass_h = new TH1D("cutPass_h", ";Cuts;Events", 3, -0.5, 2.5);
  cutPass_h->GetXaxis()->SetBinLabel(1, "No Cut");
  cutPass_h->GetXaxis()->SetBinLabel(2, "ZDC XOR");
  cutPass_h->GetXaxis()->SetBinLabel(3, "Gap Requirement");

  //D kinematics
  TH1D* rawDPt_h[nGammaBins][nMaxBins];
  TH1D* rawDY_h[nGammaBins][nMaxBins];
  TH1D* rawDM_h[nGammaBins][nMaxBins][nMaxBins];

  for(auto const& nGammaStr: nGammaBinsStrMap){
    std::string histName = "rawDPt_" + nGammaStr.first + "_YBin";
    std::string histTitle = ";D^{0} cand. p_{T} (GeV), %.1f < y < %.1f; Counts (" + nGammaBinsLabelMap[nGammaStr.first] + ")";
    bookHist1D(histName.c_str(), histTitle.c_str(), dYBins, dPtBins, rawDPt_h[nGammaStr.second]);

    histName = "rawDY_" + nGammaStr.first + "_PtBin";
    histTitle = ";D^{0} cand. y, %.1f < p_{T} < %.1f (GeV); Counts (" + nGammaBinsLabelMap[nGammaStr.first] + ")";
    bookHist1D(histName.c_str(), histTitle.c_str(), dPtBins, dYBins, rawDY_h[nGammaStr.second]);

    //For mass, double binned, so iterate over Pt, then Y
    for(Int_t pI = 0; pI < nDPtBins+1; ++pI){
      histName = "rawDM_" + nGammaStr.first + "_PtBinAll";
      histTitle = Form(";D^{0} cand. Mass (GeV), %.1f < p_{T} < %.1f (GeV),", dPtBins[0], dPtBins[dPtBins.size()-1]);
      if(pI < nDPtBins){
	histName = "rawDM_" + nGammaStr.first + "_PtBin" + std::to_string(pI);
	histTitle = Form(";D^{0} cand. Mass (GeV), %.1f < p_{T} < %.1f (GeV),", dPtBins[pI], dPtBins[pI+1]);
      }

      histName = histName + "_Y";
      histTitle = histTitle + " %.1f < y < %.1f;Counts (" + nGammaBinsLabelMap[nGammaStr.first] + ")";
      bookHist1D(histName, histTitle, dYBins, dMBins, rawDM_h[nGammaStr.second][pI]);
    }
  }

  const ULong64_t nEntriesToPrint = TMath::Max(1, config_p->GetValue("NENTRIESTOPRINT", 10000));
  const ULong64_t nEntriesToProcess = config_p->GetValue("NENTRIESTOPROCESS", -1);

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

  //Check if the nentries override is in place
  if(nEntriesToProcess >= 0){
    if(nEntriesToProcess >= totalNEntries){
      std::cout << "Requested nEntriesToProcess=" << nEntriesToProcess << " exceeds total number, " << totalNEntries << ". Will process total instead." << std::endl;
    }
    else totalNEntries = nEntriesToProcess;
  }

  //Process inputs
  std::cout << "Processing " << skimFileNames.size() << " skim files...." << std::endl;
  std::cout << "Total entries to be processed: " << totalNEntries << "..." << std::endl;

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

      if(nCurrEntries > totalNEntries) break;

      MDzeroUPC->GetEntry(entry);

      cutPass_h->Fill(0);

      //event selection handling
      bool zdcGammaN = MDzeroUPC->ZDCgammaN;
      bool zdcNGamma = MDzeroUPC->ZDCNgamma;

      //Impose XOR
      if(zdcGammaN && zdcNGamma) continue;
      if(!zdcGammaN && !zdcNGamma) continue;

      cutPass_h->Fill(1);

      //Impose gap requirement
      if(zdcGammaN && !MDzeroUPC->gapgammaN) continue;
      if(zdcNGamma && !MDzeroUPC->gapNgamma) continue;

      cutPass_h->Fill(2);

      std::string gammaNStr = zdcGammaN ? "GammaN" : "NGamma";
      int gammaNPos = nGammaBinsStrMap[gammaNStr];

      //D handling
      for(unsigned int dI = 0; dI < MDzeroUPC->Dpt->size(); ++dI){
	int yPos = getPosFromBins(MDzeroUPC->Dy->at(dI), dYBins);
	int ptPos = getPosFromBins(MDzeroUPC->Dpt->at(dI), dPtBins);

	if(yPos >= 0){
	  rawDPt_h[gammaNPos][yPos]->Fill(MDzeroUPC->Dpt->at(dI));
	  rawDPt_h[gammaNPos][nDYBins]->Fill(MDzeroUPC->Dpt->at(dI));
	}

	if(ptPos >= 0){
	  rawDY_h[gammaNPos][ptPos]->Fill(MDzeroUPC->Dy->at(dI));
	  rawDY_h[gammaNPos][nDPtBins]->Fill(MDzeroUPC->Dy->at(dI));
	}

	if(ptPos >= 0 && yPos >= 0){
	  rawDM_h[gammaNPos][ptPos][yPos]->Fill(MDzeroUPC->Dmass->at(dI));
	  rawDM_h[gammaNPos][nDPtBins][yPos]->Fill(MDzeroUPC->Dmass->at(dI));
	  rawDM_h[gammaNPos][ptPos][nDYBins]->Fill(MDzeroUPC->Dmass->at(dI));
	  rawDM_h[gammaNPos][nDPtBins][nDYBins]->Fill(MDzeroUPC->Dmass->at(dI));
	}
      }
    }

    delete MDzeroUPC;

    inFile_p->Close();
    delete inFile_p;


    if(nCurrEntries > totalNEntries) break;
  }//complete file loop

  //outfile handling
  outFile_p->cd();
  cutPass_h->Write("", TObject::kOverwrite);
  delete cutPass_h;

  for(auto const& gammaNVal : nGammaBinsStrMap){
    for(unsigned int yI = 0; yI < nDYBins+1; ++yI){
      rawDPt_h[gammaNVal.second][yI]->Write("", TObject::kOverwrite);
      delete rawDPt_h[gammaNVal.second][yI];
    }

    for(unsigned int pI = 0; pI < nDPtBins+1; ++pI){
      rawDY_h[gammaNVal.second][pI]->Write("", TObject::kOverwrite);
      delete rawDY_h[gammaNVal.second][pI];
    }

    for(Int_t pI = 0; pI < nDPtBins+1; ++pI){
      for(unsigned int yI = 0; yI < nDYBins+1; ++yI){
	rawDM_h[gammaNVal.second][pI][yI]->Write("", TObject::kOverwrite);
	delete rawDM_h[gammaNVal.second][pI][yI];
      }
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
