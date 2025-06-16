#include <iostream>
#include <fstream>
#include <iomanip> // For setting precision

#include "TFile.h"
#include "TDirectoryFile.h"
#include "TH1D.h"
#include "TMath.h"
#include "RooWorkspace.h"
#include "RooFitResult.h"
#include "RooArgList.h"
#include "RooRealVar.h"

#include "CommandLine.h" // Yi's Commandline bundle

using namespace std;
using namespace RooFit;

int getCorrectedYields(string rawYieldInput, string effInput, string outputMD)
{

  TFile rawYieldFile(rawYieldInput.c_str());
  RooWorkspace* ws = dynamic_cast<RooWorkspace*>(rawYieldFile.Get("ws"));

  if (!ws)
  {
    std::cerr << "RooWorkspace ws not found!" << std::endl;
    return 1;
  }

  RooFitResult* result = dynamic_cast<RooFitResult*>(ws->obj("FitResult"));
  if (result) {
    result->Print("v"); // Print detailed information
  } else {
    std::cerr << "RooFitResult FitResult not found!" << std::endl;
    return 1;
  }

  // Access the final fitted parameters
  const RooArgList& params = result->floatParsFinal();
  // Retrieve the parameter by name
  RooRealVar* nsigVar = dynamic_cast<RooRealVar*>(params.find("nsig"));
  double yield = nsigVar->getVal();
  double yieldErr = nsigVar->getError();
    
  rawYieldFile.Close();


  TFile effFile(effInput.c_str());
  TH1D* hNumEvtEff = (TH1D*) effFile.Get("hNumEvtEff");
  TH1D* hDenEvtEff = (TH1D*) effFile.Get("hDenEvtEff");
  TH1D* hRatioEvtEff = (TH1D*) effFile.Get("hRatioEvtEff");
  TH1D* hNumDEff = (TH1D*) effFile.Get("hNumDEff");
  TH1D* hDenDEff = (TH1D*) effFile.Get("hDenDEff");
  TH1D* hRatioDEff = (TH1D*) effFile.Get("hRatioDEff");

  std::cout << "eff(evt), eff(D): " << std::endl;
  hRatioEvtEff->Print("all");
  hRatioDEff->Print("all");

  double effEvt = hRatioEvtEff->GetBinContent(1);
  double effD   = hRatioDEff->GetBinContent(1);
  double eff    = effEvt * effD;
  double effErr = eff * TMath::Sqrt(
                    TMath::Power(hRatioEvtEff->GetBinError(1)/effEvt, 2) +
                    TMath::Power(hRatioDEff->GetBinError(1)/effD, 2)
                  );
  printf("eff(evt) x eff(D) = %f +/- %f\n", eff, effErr);

  TDirectoryFile* effDir = dynamic_cast<TDirectoryFile*>(effFile.Get("par"));
  if (!effDir) {
    std::cerr << "Error: Directory par not found in the file!" << std::endl;
    effFile.Close();
    return 1;
  }
  double parMinDzeroPT = -999.;
  double parMaxDzeroPT = -999.;
  double parMinDzeroY = -999.;
  double parMaxDzeroY = -999.;
  double parIsGammaN = -999.;
  double parTriggerChoice = -999.;
  double parIsData = -999.;
  double parScaleFactor = -999.;

  TH1D* hist = nullptr;

  hist = dynamic_cast<TH1D*>(effDir->Get("parMinDzeroPT"));
  if (hist) parMinDzeroPT = hist->GetBinContent(1);
  hist = dynamic_cast<TH1D*>(effDir->Get("parMaxDzeroPT"));
  if (hist) parMaxDzeroPT = hist->GetBinContent(1);
  hist = dynamic_cast<TH1D*>(effDir->Get("parMinDzeroY"));
  if (hist) parMinDzeroY = hist->GetBinContent(1);
  hist = dynamic_cast<TH1D*>(effDir->Get("parMaxDzeroY"));
  if (hist) parMaxDzeroY = hist->GetBinContent(1);
  hist = dynamic_cast<TH1D*>(effDir->Get("parIsGammaN"));
  if (hist) parIsGammaN = hist->GetBinContent(1);
  hist = dynamic_cast<TH1D*>(effDir->Get("parTriggerChoice"));
  if (hist) parTriggerChoice = hist->GetBinContent(1);
  hist = dynamic_cast<TH1D*>(effDir->Get("parIsData"));
  if (hist) parIsData = hist->GetBinContent(1);
  hist = dynamic_cast<TH1D*>(effDir->Get("parScaleFactor"));
  if (hist) parScaleFactor = hist->GetBinContent(1);
  // Output the results
  // std::cout << "parMinDzeroPT: " << parMinDzeroPT << "\n";
  // std::cout << "parMaxDzeroPT: " << parMaxDzeroPT << "\n";
  // std::cout << "parMinDzeroY: " << parMinDzeroY << "\n";
  // std::cout << "parMaxDzeroY: " << parMaxDzeroY << "\n";
  // std::cout << "parIsGammaN: " << parIsGammaN << "\n";
  // std::cout << "parTriggerChoice: " << parTriggerChoice << "\n";
  // std::cout << "parIsData: " << parIsData << "\n";
  // std::cout << "parScaleFactor: " << parScaleFactor << "\n";

  const float particle_antiparticlefactor = 2.;
  const float BR = 0.03947;
  const float lumipathinvnbZDC = 1.379766654 / 1.11;
  const float lumipathinvnbNotHFAND = 0.74130;
  const float lumipathinvnbZDCOR = 0.007802890;
  float lumitrigger = (parTriggerChoice==1)? lumipathinvnbZDCOR : // ZDCOR
                      (parTriggerChoice==2)? lumipathinvnbZDC   : // ZDCXORJet8
                      -999; // [WARN] Check this
  float triggereff = 1.; // [WARN] Change this
  double cross = yield / (eff * lumitrigger * triggereff *
                          particle_antiparticlefactor * BR *
                          (parMaxDzeroPT - parMinDzeroPT) *
                          (parMaxDzeroY - parMinDzeroY));

  double crossErr = yieldErr / (eff * lumitrigger * triggereff *
                                particle_antiparticlefactor * BR *
                                (parMaxDzeroPT - parMinDzeroPT) *
                                (parMaxDzeroY - parMinDzeroY));

  // this error calculation ignores efficiency errors!!
  std::cout << "pt: " << parMinDzeroPT << "," << parMaxDzeroPT
            << ", y: " << parMinDzeroY << "," << parMaxDzeroY;
  std::cout << ", eff: " << eff << " +/- " << effErr;
  std::cout << ", yield: " << yield << " +/- " << yieldErr;
  std::cout << ", pt bin width: " << parMaxDzeroPT - parMinDzeroPT;
  std::cout << ", y bin width: " << parMaxDzeroY - parMinDzeroY;
  std::cout << ", BR: " << BR;
  std::cout << ", lumitrigger: " << lumitrigger;
  std::cout << ", triggereff: " << triggereff;
  std::cout << ", cross: " << cross << " +/- " << crossErr << std::endl;

  // Open the file in overwrite mode (std::ios::out)
  std::ofstream outFile(outputMD.c_str(), std::ios::out);
  if (!outFile) {
    std::cerr << "Error: Cannot open output.md for writing!" << std::endl;
    return 1;
  }

  // Write headers if the file is empty
  if (outFile.tellp() == 0) {
    outFile << "| ptmin | ptmax | ymin | ymax |" << std::endl;
    outFile << "|-------|-------|------|------|" << std::endl;
  }

  outFile << "| " << std::fixed << std::setprecision(4)
          << parMinDzeroPT << " | " << parMaxDzeroPT
          << " | " << parMinDzeroY << " | " << parMaxDzeroY
          << " |" << std::endl;

  outFile << "| effEvt | effD | raw yield | corrected yield (mb) |" << std::endl;

  outFile << "| " << std::fixed << std::setprecision(4)
          << effEvt << " +/- " << hRatioEvtEff->GetBinError(1) // Error for effEvt
          << " | " << effD << " +/- " << hRatioDEff->GetBinError(1) // Error for effD
          << " | " << yield << " +/- " << yieldErr // Error for raw yield
          << " | " << cross/1e6 << " +/- " << crossErr/1e6 // Error for corrected yield
          << " |" << std::endl;

  outFile << "| Num #(Evt) | Den #(Evt) | Num #(D) | Den #(D) |" << std::endl;
  outFile << "| " << hNumEvtEff->GetBinContent(1)
          << " | " << hDenEvtEff->GetBinContent(1)
          << " | " << hNumDEff->GetBinContent(1)
          << " | " << hDenDEff->GetBinContent(1)
          << " |" << std::endl;

  // Close the file
  outFile.close();

  return 0;

}



int main(int argc, char *argv[]) {
  CommandLine CL(argc, argv);
  string rawYieldInput    = CL.Get      ("rawYieldInput",    "output.root"); // Input raw yield file from MassFit
  string effInput         = CL.Get      ("effInput",      "output.root"); // Input eff file from ExecuteDzeroUPC
  string outputMD         = CL.Get      ("Output", "correctedYields.md");     // Output file
 
  int retVal = getCorrectedYields(rawYieldInput, effInput, outputMD);

  return retVal;
}