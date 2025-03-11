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

int sidebandSubtraction(string sampleInput,
                        string massFitResult,
                        string sideBand,
                        float sideBandEdge,
                        string output,
                        string plotPrefix)
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

  //////// Step 1: Determine the normalization constants (NSBM, NSBP, NSR)
  RooRealVar* nbkgVar 			  = dynamic_cast<RooRealVar*>(floatParams.find("nbkg"));
  RooRealVar* lambdaVar       = dynamic_cast<RooRealVar*>(floatParams.find("lambda"));
  RooRealVar m("Dmass", "Mass [GeV]", DMASSMIN, DMASSMAX);
  RooExponential combPDF_exp("combinatorics_exp", "combinatorics model", m, *lambdaVar);

  // Define the variable range
  m.setRange("fullRange", m.getMin(), m.getMax());
  m.setRange("leftSideBand" , m.getMin(), centerVal-sideBandEdge*sigmaVal);
  m.setRange("rightSideBand", centerVal+sideBandEdge*sigmaVal, m.getMax());
  m.setRange("signalRegion" , centerVal-2*sigmaVal, centerVal+2*sigmaVal);

  // Compute the integral of the exponential over the full range
  double NINC = nbkgVar->getVal() * combPDF_exp.createIntegral(m, RooFit::NormSet(m), RooFit::Range("fullRange"))->getVal();
  double NSBM = nbkgVar->getVal() * combPDF_exp.createIntegral(m, RooFit::NormSet(m), RooFit::Range("leftSideBand"))->getVal();
  double NSBP = nbkgVar->getVal() * combPDF_exp.createIntegral(m, RooFit::NormSet(m), RooFit::Range("rightSideBand"))->getVal();
  double NSR  = nbkgVar->getVal() * combPDF_exp.createIntegral(m, RooFit::NormSet(m), RooFit::Range("signalRegion"))->getVal();
  printf("(NINC, NSBM, NSBP, NSR) = (%.3f, %.3f, %.3f, %.3f)\n", NINC, NSBM, NSBP, NSR);
  
  //////// Step 2: Get DCA distributions in different mass windows
  TFile* infile = TFile::Open(sampleInput.c_str());
  TTree *nt = (TTree*) infile->Get("nt");

  // DCA with variable histogram bins
  // double _xBins[] = { 0.0, 0.0025, 0.005, 0.0075, 0.01, 0.015, 0.02, // 0.028, 
  //                     0.036, // 0.045, 
  //                     0.06 , // 0.08, 
  //                     0.10};
  // double _xBins[] = { 0.0, 0.004, 0.006, 0.0085, 0.0115, 0.016, 0.05 };
  // double _xBins[] = { 0.0, 0.004, 0.006, 0.0085, 0.016, 0.05 };
  double _xBins[] = { 0.0, 0.004, 0.006, 0.0090, 0.016, 0.036, 0.06, 0.10 };
  const int _nXBins = sizeof(_xBins)/sizeof(double) - 1;
  
  TH1D* hDCA_INC = new TH1D("hDCA_INC", ";DCA (cm);Entries", _nXBins, _xBins);
  TH1D* hDCA_SBM = new TH1D("hDCA_SBM", ";DCA (cm);Entries", _nXBins, _xBins);
  TH1D* hDCA_SBP = new TH1D("hDCA_SBP", ";DCA (cm);Entries", _nXBins, _xBins);
  TH1D* hDCA_SR  = new TH1D("hDCA_SR" , ";DCA (cm);Entries", _nXBins, _xBins);
  hDCA_INC->Sumw2();
  hDCA_SBM->Sumw2();
  hDCA_SBP->Sumw2();
  hDCA_SR ->Sumw2();

  float DmassVal, DCAVal;
  nt->SetBranchAddress("Dmass", &DmassVal);
  nt->SetBranchAddress("DCA", &DCAVal);

  for (Long64_t iEntry = 0; iEntry < nt->GetEntries(); iEntry++)
  {
    nt->GetEntry(iEntry);

    if (DmassVal >= m.getMin() &&
        DmassVal <= m.getMax())
    {
      hDCA_INC->Fill(DCAVal);
    }
    if (DmassVal >= m.getMin() &&
        DmassVal <= centerVal-sideBandEdge*sigmaVal)
    {
      hDCA_SBM->Fill(DCAVal);
    }
    if (DmassVal >= centerVal+sideBandEdge*sigmaVal &&
        DmassVal <= m.getMax())
    {
      hDCA_SBP->Fill(DCAVal);
    }
    if (DmassVal >= centerVal-2*sigmaVal &&
        DmassVal <= centerVal+2*sigmaVal)
    {
      hDCA_SR ->Fill(DCAVal);
    }
  }

  //////// Step 3: Sideband subtraction with the scaledInSR background component from the S.R. histogram
  TH1D* hDCA_SB_scaledInSR = (sideBand=="SBP")? (TH1D*) hDCA_SBP->Clone(Form("%s_scaledInSR", hDCA_SBP->GetName())):
                                            (TH1D*) hDCA_SBM->Clone(Form("%s_scaledInSR", hDCA_SBM->GetName()));
  hDCA_SB_scaledInSR->Sumw2();
  hDCA_SB_scaledInSR->Scale( (sideBand=="SBP")? NSR/NSBP:
                                            NSR/NSBM);
  TH1D* hDCA_SR_sub = (TH1D*) hDCA_SR->Clone(Form("%s_sub", hDCA_SR->GetName()));
  hDCA_SR_sub->Sumw2();
  hDCA_SR_sub->Add(hDCA_SB_scaledInSR, -1);

  //////// Step 4: Saving things to output file
  TFile* outFile = new TFile(output.c_str(), "recreate");

  normalizeHistoBinWidth(hDCA_INC);
  normalizeHistoBinWidth(hDCA_SBM);
  normalizeHistoBinWidth(hDCA_SBP);
  normalizeHistoBinWidth(hDCA_SR);
  normalizeHistoBinWidth(hDCA_SB_scaledInSR);
  normalizeHistoBinWidth(hDCA_SR_sub);
  hDCA_INC->GetYaxis()->SetTitle("Yield per unit");
  hDCA_SBM->GetYaxis()->SetTitle("Yield per unit");
  hDCA_SBP->GetYaxis()->SetTitle("Yield per unit");
  hDCA_SR->GetYaxis()->SetTitle("Yield per unit");
  hDCA_SB_scaledInSR->GetYaxis()->SetTitle("Yield per unit");
  hDCA_SR_sub->GetYaxis()->SetTitle("Yield per unit");

  TH1D* hDCA_SBP_norm   = (TH1D*) hDCA_SBP->Clone(Form("%s_norm",hDCA_SBP->GetName()));
  TH1D* hDCA_SBM_norm   = (TH1D*) hDCA_SBM->Clone(Form("%s_norm",hDCA_SBM->GetName()));
  TH1D* hDCA_INC_norm   = (TH1D*) hDCA_INC->Clone(Form("%s_norm",hDCA_INC->GetName()));
  TH1D* hDCA_SR_norm    = (TH1D*) hDCA_SR->Clone(Form("%s_norm",hDCA_SR->GetName()));
  TH1D* hDCA_SB_scaledInSR_norm = (TH1D*) hDCA_SB_scaledInSR->Clone(Form("%s_norm",hDCA_SB_scaledInSR->GetName()));
  TH1D* hDCA_SR_sub_norm = (TH1D*) hDCA_SR_sub->Clone(Form("%s_norm",hDCA_SR_sub->GetName()));
  hDCA_SBP_norm->Sumw2();
  hDCA_SBM_norm->Sumw2();
  hDCA_INC_norm->Sumw2();
  hDCA_SR_norm->Sumw2();
  hDCA_SB_scaledInSR_norm->Sumw2();
  hDCA_SR_sub_norm->Sumw2();

  hDCA_INC_norm->Scale(1/hDCA_INC_norm->Integral());
  hDCA_SBM_norm->Scale(1/hDCA_SBM_norm->Integral());
  hDCA_SBP_norm->Scale(1/hDCA_SBP_norm->Integral());
  hDCA_SR_norm->Scale(1/hDCA_SR_norm->Integral());
  hDCA_SB_scaledInSR_norm->Scale(1/hDCA_SB_scaledInSR_norm->Integral());
  hDCA_SR_sub_norm->Scale(1/hDCA_SR_sub_norm->Integral());

  hDCA_INC_norm->GetYaxis()->SetTitle("Normalized");
  hDCA_SBM_norm->GetYaxis()->SetTitle("Normalized");
  hDCA_SBP_norm->GetYaxis()->SetTitle("Normalized");
  hDCA_SR_norm->GetYaxis()->SetTitle("Normalized");
  hDCA_SB_scaledInSR_norm->GetYaxis()->SetTitle("Normalized");
  hDCA_SR_sub_norm->GetYaxis()->SetTitle("Normalized");

  outFile->cd();
  hDCA_SR_sub->Write();

  TDirectoryFile* subDir = (TDirectoryFile*) outFile->mkdir("details");
  subDir->cd();
  hDCA_INC->Write();
  hDCA_SBM->Write();
  hDCA_SBP->Write();
  hDCA_SR->Write();
  hDCA_SB_scaledInSR->Write();
  hDCA_SR_sub->Write();

  hDCA_INC_norm->Write();
  hDCA_SBM_norm->Write();
  hDCA_SBP_norm->Write();
  hDCA_SR_norm->Write();
  hDCA_SB_scaledInSR_norm->Write();
  hDCA_SR_sub_norm->Write();

  //////// Step 5: Plotting
  TCanvas *cAll = new TCanvas("cAll", "cAll", 1200, 1200);
  cAll->Divide(2, 2, 0.001, 0.001);
  // gPad->SetLeftMargin(0.13);
  // gPad->SetRightMargin(0.05);

  hDCA_SBP->SetLineWidth(2);
  hDCA_SBM->SetLineWidth(2);
  hDCA_INC->SetLineWidth(2);
  hDCA_SR->SetLineWidth(2);
  hDCA_SB_scaledInSR->SetLineWidth(2);
  hDCA_SR_sub->SetLineWidth(2);

  hDCA_INC->SetLineColorAlpha(kBlue, 0.2);
  hDCA_INC->SetFillColorAlpha(kBlue, 0.2);
  hDCA_SBM->SetLineColor(kViolet-3);
  hDCA_SBP->SetLineColor(kTeal-7);
  hDCA_SR->SetLineColor(kRed-9);
  hDCA_SB_scaledInSR->SetLineColorAlpha(kGray, 0.5);
  hDCA_SB_scaledInSR->SetFillColorAlpha(kGray, 0.5);
  hDCA_SR_sub->SetLineColor(kRed);

  hDCA_SBP_norm->SetLineWidth(2);
  hDCA_SBM_norm->SetLineWidth(2);
  hDCA_INC_norm->SetLineWidth(2);
  hDCA_SR_norm->SetLineWidth(2);
  hDCA_SB_scaledInSR_norm->SetLineWidth(2);
  hDCA_SR_sub_norm->SetLineWidth(2);

  hDCA_INC_norm->SetLineColorAlpha(kBlue, 0.2);
  hDCA_INC_norm->SetFillColorAlpha(kBlue, 0.2);
  hDCA_SBM_norm->SetLineColor(kViolet-3);
  hDCA_SBP_norm->SetLineColor(kTeal-7);
  hDCA_SR_norm->SetLineColor(kRed-9);
  hDCA_SB_scaledInSR_norm->SetLineColorAlpha(kGray, 0.5);
  hDCA_SB_scaledInSR_norm->SetFillColorAlpha(kGray, 0.5);
  hDCA_SR_sub_norm->SetLineColor(kRed);

  cAll->cd(1);
  gPad->SetLeftMargin(0.13);
  gPad->SetRightMargin(0.05);
  gStyle->SetOptStat(0);
  gPad->SetLogy();
  hDCA_INC->DrawClone("hist");
  hDCA_SBP->DrawClone("same e");
  hDCA_SBM->DrawClone("same e");
  hDCA_SR->DrawClone("same e");

  TLegend *legend1 = new TLegend(0.4, 0.7, 0.9, 0.9);
  legend1->AddEntry(hDCA_INC, Form("Whole region [%.2f,%.2f]",   m.getMin(), m.getMax()));
  legend1->AddEntry(hDCA_SBM, Form("Sideband minus [%.2f,%.2f]", m.getMin(), centerVal-sideBandEdge*sigmaVal));
  legend1->AddEntry(hDCA_SBP, Form("Sideband plus [%.2f,%.2f]",  centerVal+sideBandEdge*sigmaVal, m.getMax()));
  legend1->AddEntry(hDCA_SR,  Form("Signal region [%.2f,%.2f]",  centerVal-2*sigmaVal, centerVal+2*sigmaVal));
  formatLegend(legend1,18);
  legend1->Draw();

  cAll->cd(2);
  gPad->SetLeftMargin(0.13);
  gPad->SetRightMargin(0.05);
  gPad->SetLogy();
  hDCA_SR->DrawClone("e");
  hDCA_SB_scaledInSR->DrawClone("same hist");
  hDCA_SR_sub->DrawClone("same e");

  TLegend *legend2 = new TLegend(0.4, 0.7, 0.9, 0.9);
  legend2->AddEntry(hDCA_SR,  Form("Signal region [%.2f,%.2f]",  centerVal-2*sigmaVal, centerVal+2*sigmaVal), "l");
  legend2->AddEntry(hDCA_SR_sub, "S.R. w/ bkg sub.", "l");
  formatLegend(legend2,18);
  legend2->Draw();

  cAll->cd(3);
  gPad->SetLeftMargin(0.13);
  gPad->SetRightMargin(0.05);
  gPad->SetLogy();
  hDCA_SBP_norm->DrawClone("e");
  hDCA_SBM_norm->DrawClone("same e");
  hDCA_INC_norm->DrawClone("same hist");
  hDCA_SR_norm->DrawClone("same e");
  legend1->Draw();

  cAll->cd(4);
  gPad->SetLeftMargin(0.13);
  gPad->SetRightMargin(0.05);
  gPad->SetLogy();
  hDCA_SR_norm->DrawClone("e");
  hDCA_SR_sub_norm->DrawClone("same e");
  legend2->Draw();

  cAll->SaveAs(Form("%ssidebandSubtraction.pdf",plotPrefix.c_str()));

  delete legend1;
  delete legend2;
  delete cAll;

  outFile->Close();
  delete outFile;

  return 0;
}

int main(int argc, char *argv[])
{
  CommandLine CL(argc, argv);
  string sampleInput     = CL.Get      ("sampleInput",    "output.root"); // Input micro tree file, which contains the DCA distribution
  string massFitResult   = CL.Get      ("massFitResult",  "fit.root");    // Input file for the invariant mass fit result
  string sideBand        = CL.Get      ("sideBand", "SBP");               // Whether subtraction is from plus or minus side: SBP, SBM
  double sideBandEdge    = CL.GetDouble("sideBandEdge", 3);               // Number of standard deviations from mass peak to the start of the side band region
  string output          = CL.Get      ("Output", "DCA_sidebandSubtracted.root");// Output file
  string plotPrefix      = CL.Get      ("plotPrefix", "./");              // Plot prefix

  int retVal = sidebandSubtraction( sampleInput, massFitResult,
                                    sideBand, sideBandEdge,
                                    output, plotPrefix );

  return retVal;
}