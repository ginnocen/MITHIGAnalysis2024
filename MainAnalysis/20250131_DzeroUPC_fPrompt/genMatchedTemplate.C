#include <TTree.h>
#include <TFile.h>
#include <TDirectoryFile.h>
#include <TChain.h>
#include <TH1D.h>
#include <TMath.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TLegend.h>

#include <RooAddPdf.h>
#include <RooAbsPdf.h>
#include <RooGaussian.h>
#include <RooCBShape.h>
#include <RooExponential.h>
#include <RooChebychev.h>
#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooFitResult.h>
#include <RooPlot.h>
#include <RooWorkspace.h>

#include "CommandLine.h" // Yi's Commandline bundle
#include "utilities.h"


using namespace std;
using namespace RooFit;
#include <RooArgSet.h>
#include <RooRealVar.h>
#include <RooConstVar.h>
#include <RooFormulaVar.h>
#include <vector>
#include <string>
#include <iostream>
#include <map>

#define DMASSMIN 1.68
#define DMASSMAX 2.05

int genMatchedTemplate(string sampleInput, 
				string massFitResult, 
				string output)
{
  //////// Step 0: Get the two sigma signal region
  TFile massFitFile(massFitResult.c_str());
  RooWorkspace* ws = dynamic_cast<RooWorkspace*>(massFitFile.Get("ws"));

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
  const RooArgList& floatParams = result->floatParsFinal();
  const RooArgList& constParams = result->constPars();
  RooRealVar* sig_meanVar 		= dynamic_cast<RooRealVar*>(floatParams.find("sig_mean"));
  RooRealVar* sig_frac1Var 		= dynamic_cast<RooRealVar*>(constParams.find("sig_frac1"));
  RooRealVar* sig_sigma1Var 	= dynamic_cast<RooRealVar*>(constParams.find("sig_sigma1"));
  RooRealVar* sig_sigma2Var 	= dynamic_cast<RooRealVar*>(constParams.find("sig_sigma2"));

  double centerVal  =  sig_meanVar->getVal();
  double sigmaVal   = (sig_frac1Var->getVal()>0.5)? sig_sigma1Var->getVal():
                                                    sig_sigma2Var->getVal();

  printf("Mean of the double Gaus: %.3f\n", centerVal);
  printf("Width of the 1st Gaus: %.3f, amplitude: %.3f\n", sig_sigma1Var->getVal(),   sig_frac1Var->getVal());
  printf("Width of the 2nd Gaus: %.3f, amplitude: %.3f\n", sig_sigma2Var->getVal(), 1-sig_frac1Var->getVal());
  printf("2-sigma band: [%.3f, %.3f]\n", centerVal-2*sigmaVal, centerVal+2*sigmaVal);

  //////// Step 1: Get DCA distributions in mass SR windows
  TFile* infile = TFile::Open(sampleInput.c_str());
  TTree *nt = (TTree*) infile->Get("nt");

  // DCA with variable histogram bins
  double _xBins[] = { 0.0, 0.0025, 0.005, 0.0075, 0.01, 0.015, 0.02, // 0.028, 
                      0.036, // 0.045, 
                      0.06 , // 0.08, 
                      0.10};
  const int _nXBins = sizeof(_xBins)/sizeof(double) - 1;
  
  TH1D* hDCA_SR  = new TH1D("hDCA_SR" , ";DCA (cm);Entries", _nXBins, _xBins);
  hDCA_SR ->Sumw2();

  float DgenVal, DmassVal, DCAVal;
  nt->SetBranchAddress("Dgen", &DgenVal);
  nt->SetBranchAddress("Dmass", &DmassVal);
  nt->SetBranchAddress("DCA", &DCAVal);

  for (Long64_t iEntry = 0; iEntry < nt->GetEntries(); iEntry++)
  {
    nt->GetEntry(iEntry);

    if (((int) DgenVal)!=23333 && ((int) DgenVal)!=23344) continue;

    if (DmassVal >= centerVal-2*sigmaVal &&
        DmassVal <= centerVal+2*sigmaVal)
    {
      hDCA_SR ->Fill(DCAVal);
    }
  }

  normalizeHistoBinWidth(hDCA_SR);
  hDCA_SR->GetYaxis()->SetTitle("Yield per unit");

  //////// Step 2: Saving things to output file
  TFile* outFile = new TFile(output.c_str(), "recreate");

  TH1D* hDCA_SR_norm    = (TH1D*) hDCA_SR->Clone(Form("%s_norm",hDCA_SR->GetName()));
  hDCA_SR_norm->Sumw2();
  hDCA_SR_norm->Scale(1/hDCA_SR_norm->Integral());
  hDCA_SR_norm->GetYaxis()->SetTitle("Normalized");

  outFile->cd();
  hDCA_SR->Write();
  hDCA_SR_norm->Write();

  outFile->Close();
  delete outFile;

  return 0;
}

int main(int argc, char *argv[])
{
  CommandLine CL(argc, argv);
  string sampleInput     = CL.Get      ("sampleInput",    "output.root"); // Input data file
  string massFitResult   = CL.Get      ("massFitResult",  "fit.root");    // Input data file
  string output          = CL.Get      ("Output", "genMatchedTemplate.root");            // Output file

  int retVal = genMatchedTemplate( sampleInput, massFitResult,
                                    output);

  return retVal;
}