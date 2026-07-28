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
using namespace std;

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
#include "HistoHelper.h"
#include "utilities.h"

using namespace RooFit;
#include <RooArgSet.h>
#include <RooRealVar.h>
#include <RooConstVar.h>
#include <RooFormulaVar.h>
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <sstream>
#include <algorithm>
#include <cctype>

#define DMASSMIN 1.66
#define DMASSMAX 2.16

// ---------- NEW helpers ----------
static std::vector<double> ParseCSVdoubles(const std::string &csv)
{
  std::vector<double> v;
  std::stringstream ss(csv);
  std::string item;
  while (std::getline(ss, item, ','))
  {
    item.erase(item.begin(),
               std::find_if(item.begin(), item.end(), [](unsigned char ch){ return !std::isspace(ch); }));
    item.erase(std::find_if(item.rbegin(), item.rend(), [](unsigned char ch){ return !std::isspace(ch); }).base(),
               item.end());
    if (!item.empty())
      v.push_back(std::stod(item));
  }
  return v;
}

static std::string SanitizeTag(std::string s)
{
  for (char &c : s)
  {
    if (!(std::isalnum((unsigned char)c) || c=='_')) c = '_';
  }
  return s;
}
// --------------------------------

int sidebandSubtraction(string sampleInput,
                        string massFitResult,
                        string sideBand,
                        float sideBandEdge,
                        string output,
                        string plotPrefix,
                        string varExpr,      // NEW
                        string xTitle,       // NEW
                        string binsCSV,      // NEW (optional)
                        int nbins,           // NEW
                        double xmin,         // NEW
                        double xmax,         // NEW
                        string plotNameTag)  // optional: PDF stem; empty -> sanitize(varExpr)
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
  RooRealVar* sig_meanVar     = dynamic_cast<RooRealVar*>(floatParams.find("sig_mean"));
  RooRealVar* sig_frac1Var    = dynamic_cast<RooRealVar*>(constParams.find("sig_frac1"));
  RooRealVar* sig_sigma1Var   = dynamic_cast<RooRealVar*>(constParams.find("sig_sigma1"));
  RooRealVar* sig_sigma2Var   = dynamic_cast<RooRealVar*>(constParams.find("sig_sigma2"));

  // double centerVal  =  sig_meanVar->getVal();
  // double sigmaVal   = (sig_frac1Var->getVal()>0.5)? sig_sigma1Var->getVal():
  //                                                   sig_sigma2Var->getVal();
  double centerVal = 1.86484;
  double sigmaVal  = 0.02/2.;

  printf("Mean of the double Gaus: %.3f\n", centerVal);
  printf("Width of the 1st Gaus: %.3f, amplitude: %.3f\n", sig_sigma1Var->getVal(),   sig_frac1Var->getVal());
  printf("Width of the 2nd Gaus: %.3f, amplitude: %.3f\n", sig_sigma2Var->getVal(), 1-sig_frac1Var->getVal());
  printf("2-sigma band: [%.3f, %.3f]\n", centerVal-2*sigmaVal, centerVal+2*sigmaVal);

  //////// Step 1: Determine the normalization constants (NSBM, NSBP, NSB, NSR)
  RooRealVar* nbkgVar       = dynamic_cast<RooRealVar*>(floatParams.find("nbkg"));
  RooRealVar* lambdaVar     = dynamic_cast<RooRealVar*>(floatParams.find("lambda"));
  RooRealVar m("Dmass", "Mass [GeV]", DMASSMIN, DMASSMAX);
  RooExponential combPDF_exp("combinatorics_exp", "combinatorics model", m, *lambdaVar);

  // Define the variable range
  m.setRange("fullRange", m.getMin(), m.getMax());
  m.setRange("leftSideBand" , m.getMin(), centerVal-sideBandEdge*sigmaVal);
  m.setRange("rightSideBand", centerVal+sideBandEdge*sigmaVal, m.getMax());
  m.setRange("signalRegion" , centerVal-2*sigmaVal, centerVal+2*sigmaVal);

  // Compute integrals
  double NINC = nbkgVar->getVal() * combPDF_exp.createIntegral(m, RooFit::NormSet(m), RooFit::Range("fullRange"))->getVal();
  double NSBM = nbkgVar->getVal() * combPDF_exp.createIntegral(m, RooFit::NormSet(m), RooFit::Range("leftSideBand"))->getVal();
  double NSBP = nbkgVar->getVal() * combPDF_exp.createIntegral(m, RooFit::NormSet(m), RooFit::Range("rightSideBand"))->getVal();
  double NSR  = nbkgVar->getVal() * combPDF_exp.createIntegral(m, RooFit::NormSet(m), RooFit::Range("signalRegion"))->getVal();
  double NSB  = NSBM + NSBP;
  printf("(NINC, NSBM, NSBP, NSB, NSR) = (%.3f, %.3f, %.3f, %.3f, %.3f)\n", NINC, NSBM, NSBP, NSB, NSR);

  //////// Step 2: Get distributions in different mass windows (GENERALIZED)
  TFile* infile = TFile::Open(sampleInput.c_str());
  if (!infile || infile->IsZombie()) {
    std::cerr << "Failed to open input file: " << sampleInput << std::endl;
    return 1;
  }
  TTree *nt = (TTree*) infile->Get("nt");
  if (!nt) {
    std::cerr << "TTree nt not found in input!" << std::endl;
    return 1;
  }

  const std::string tag =
      plotNameTag.empty() ? SanitizeTag(varExpr) : SanitizeTag(plotNameTag);

  // Decide binning: either variable edges from --bins, or uniform from --nbins/--xmin/--xmax
  std::vector<double> edges;
  bool useVarBins = false;
  if (!binsCSV.empty())
  {
    edges = ParseCSVdoubles(binsCSV);
    if (edges.size() >= 2) useVarBins = true;
    else std::cerr << "Warning: --bins parsed <2 edges; falling back to uniform bins.\n";
  }

  TH1D* hist_INC = nullptr; // keep original names to minimize downstream edits
  TH1D* hist_SBM = nullptr;
  TH1D* hist_SBP = nullptr;
  TH1D* hist_SB  = nullptr;
  TH1D* hist_SR  = nullptr;

  bool plotLogy = (varExpr.find("DsvpvDistance")!=string::npos);

  if (useVarBins)
  {
    hist_INC = new TH1D("hist_INC", Form(";%s;Entries", xTitle.c_str()), (int)edges.size()-1, edges.data());
    hist_SBM = new TH1D("hist_SBM", Form(";%s;Entries", xTitle.c_str()), (int)edges.size()-1, edges.data());
    hist_SBP = new TH1D("hist_SBP", Form(";%s;Entries", xTitle.c_str()), (int)edges.size()-1, edges.data());
    hist_SB  = new TH1D("hist_SB" , Form(";%s;Entries", xTitle.c_str()), (int)edges.size()-1, edges.data());
    hist_SR  = new TH1D("hist_SR" , Form(";%s;Entries", xTitle.c_str()), (int)edges.size()-1, edges.data());
  }
  else
  {
    if (nbins <= 0) nbins = 60;
    hist_INC = new TH1D("hist_INC", Form(";%s;Entries", xTitle.c_str()), nbins, xmin, xmax);
    hist_SBM = new TH1D("hist_SBM", Form(";%s;Entries", xTitle.c_str()), nbins, xmin, xmax);
    hist_SBP = new TH1D("hist_SBP", Form(";%s;Entries", xTitle.c_str()), nbins, xmin, xmax);
    hist_SB  = new TH1D("hist_SB" , Form(";%s;Entries", xTitle.c_str()), nbins, xmin, xmax);
    hist_SR  = new TH1D("hist_SR" , Form(";%s;Entries", xTitle.c_str()), nbins, xmin, xmax);
  }

  if (hist_INC->GetSumw2N()==0) hist_INC->Sumw2();
  if (hist_SBM->GetSumw2N()==0) hist_SBM->Sumw2();
  if (hist_SBP->GetSumw2N()==0) hist_SBP->Sumw2();
  if (hist_SB->GetSumw2N()==0) hist_SB ->Sumw2();
  if (hist_SR->GetSumw2N()==0) hist_SR ->Sumw2();

  // ---------- NEW: gen-matched histogram (no SR restriction) ----------
  bool doGen = false;
  TH1D* hist_GEN = (TH1D*)hist_INC->Clone("hist_GEN");
  hist_GEN->Reset("ICESM");
  hist_GEN->SetTitle(Form(";%s;Entries", xTitle.c_str()));
  if (hist_GEN->GetSumw2N()==0) hist_GEN->Sumw2();
  // -------------------------------------------------------------------

  // Mass-window cuts
  const std::string cutINC = Form("Dmass>=%.6f && Dmass<=%.6f", m.getMin(), m.getMax());
  const std::string cutSBM = Form("Dmass>=%.6f && Dmass<=%.6f", m.getMin(), centerVal-sideBandEdge*sigmaVal);
  const std::string cutSBP = Form("Dmass>=%.6f && Dmass<=%.6f", centerVal+sideBandEdge*sigmaVal, m.getMax());
  const std::string cutSB  = Form("(Dmass>=%.6f && Dmass<=%.6f) || (Dmass>=%.6f && Dmass<=%.6f)", 
                                  m.getMin(), centerVal-sideBandEdge*sigmaVal,
                                  centerVal+sideBandEdge*sigmaVal, m.getMax());
  const std::string cutSR  = Form("Dmass>=%.6f && Dmass<=%.6f", centerVal-2*sigmaVal, centerVal+2*sigmaVal);

  // Fill using TTree::Draw so varExpr can be any branch or expression (mass windows only; no ortho cuts)
  nt->Draw(Form("%s>>%s", varExpr.c_str(), hist_INC->GetName()), cutINC.c_str(), "goff");
  nt->Draw(Form("%s>>%s", varExpr.c_str(), hist_SBM->GetName()), cutSBM.c_str(), "goff");
  nt->Draw(Form("%s>>%s", varExpr.c_str(), hist_SBP->GetName()), cutSBP.c_str(), "goff");
  nt->Draw(Form("%s>>%s", varExpr.c_str(), hist_SB ->GetName()), cutSB.c_str(),  "goff");
  nt->Draw(Form("%s>>%s", varExpr.c_str(), hist_SR ->GetName()), cutSR.c_str(), "goff");

  // ---------- NEW Step 6: gen-matched projection (no SR cut) ----------
  if (nt->GetBranch("Dgen") || nt->FindLeaf("Dgen"))
  {
    Long64_t nNonZero = nt->Draw("Dgen", "Dgen!=0", "goff");
    if (nNonZero > 0) doGen = true;
  }

  if (doGen)
  {
    const std::string cutGEN = "(Dgen==23333 || Dgen==23344)";
    nt->Draw(Form("%s>>%s", varExpr.c_str(), hist_GEN->GetName()), cutGEN.c_str(), "goff");
    std::cout << "[INFO] Filled gen-matched histogram with cut: " << cutGEN << std::endl;
  }
  else
  {
    std::cout << "[INFO] Dgen not found or all-zero; skipping gen-matched histogram." << std::endl;
  }
  // -------------------------------------------------------------

  //////// Step 3: Sideband subtraction with the scaledInSR background component from the S.R. histogram
  TH1D* hist_SB_scaledInSR = (sideBand=="SBP")? (TH1D*) hist_SBP->Clone(Form("%s_scaledInSR", hist_SBP->GetName())):
                             (sideBand=="SBM")? (TH1D*) hist_SBM->Clone(Form("%s_scaledInSR", hist_SBM->GetName())):
                                                (TH1D*) hist_SB ->Clone(Form("%s_scaledInSR", hist_SB ->GetName()));
  if (hist_SB_scaledInSR->GetSumw2N()==0) hist_SB_scaledInSR->Sumw2();
  hist_SB_scaledInSR->Scale( (sideBand=="SBP")? NSR/NSBP : 
                             (sideBand=="SBM")? NSR/NSBM : NSR/NSB);

  TH1D* hist_SR_sub = (TH1D*) hist_SR->Clone(Form("%s_sub", hist_SR->GetName()));
  if (hist_SR_sub->GetSumw2N()==0) hist_SR_sub->Sumw2();
  hist_SR_sub->Add(hist_SB_scaledInSR, -1);

  //////// Step 4: Saving things to output file
  TFile* outFile = new TFile(output.c_str(), "recreate");

  normalizeHistoBinWidth(hist_INC);
  normalizeHistoBinWidth(hist_SBM);
  normalizeHistoBinWidth(hist_SBP);
  normalizeHistoBinWidth(hist_SB);
  normalizeHistoBinWidth(hist_SR);
  normalizeHistoBinWidth(hist_SB_scaledInSR);
  normalizeHistoBinWidth(hist_SR_sub);
  if (doGen) normalizeHistoBinWidth(hist_GEN);

  hist_INC->GetYaxis()->SetTitle("Yield per unit");
  hist_SBM->GetYaxis()->SetTitle("Yield per unit");
  hist_SBP->GetYaxis()->SetTitle("Yield per unit");
  hist_SB->GetYaxis()->SetTitle("Yield per unit");
  hist_SR->GetYaxis()->SetTitle("Yield per unit");
  hist_SB_scaledInSR->GetYaxis()->SetTitle("Yield per unit");
  hist_SR_sub->GetYaxis()->SetTitle("Yield per unit");
  if (doGen) hist_GEN->GetYaxis()->SetTitle("Yield per unit");

  TH1D* hist_SBP_norm   = (TH1D*) hist_SBP->Clone(Form("%s_norm",hist_SBP->GetName()));
  TH1D* hist_SBM_norm   = (TH1D*) hist_SBM->Clone(Form("%s_norm",hist_SBM->GetName()));
  TH1D* hist_INC_norm   = (TH1D*) hist_INC->Clone(Form("%s_norm",hist_INC->GetName()));
  TH1D* hist_SB_norm    = (TH1D*) hist_SB->Clone(Form("%s_norm",hist_SB->GetName()));
  TH1D* hist_SR_norm    = (TH1D*) hist_SR->Clone(Form("%s_norm",hist_SR->GetName()));
  TH1D* hist_SB_scaledInSR_norm = (TH1D*) hist_SB_scaledInSR->Clone(Form("%s_norm",hist_SB_scaledInSR->GetName()));
  TH1D* hist_SR_sub_norm = (TH1D*) hist_SR_sub->Clone(Form("%s_norm",hist_SR_sub->GetName()));

  if (hist_SBP_norm->GetSumw2N()==0) hist_SBP_norm->Sumw2();
  if (hist_SBM_norm->GetSumw2N()==0) hist_SBM_norm->Sumw2();
  if (hist_INC_norm->GetSumw2N()==0) hist_INC_norm->Sumw2();
  if (hist_SB_norm->GetSumw2N()==0) hist_SB_norm->Sumw2();
  if (hist_SR_norm->GetSumw2N()==0) hist_SR_norm->Sumw2();
  if (hist_SB_scaledInSR_norm->GetSumw2N()==0) hist_SB_scaledInSR_norm->Sumw2();
  if (hist_SR_sub_norm->GetSumw2N()==0) hist_SR_sub_norm->Sumw2();

  if (hist_INC_norm->Integral() != 0) hist_INC_norm->Scale(1/hist_INC_norm->Integral());
  if (hist_SBM_norm->Integral() != 0) hist_SBM_norm->Scale(1/hist_SBM_norm->Integral());
  if (hist_SBP_norm->Integral() != 0) hist_SBP_norm->Scale(1/hist_SBP_norm->Integral());
  if (hist_SB_norm->Integral()  != 0) hist_SB_norm->Scale(1/hist_SB_norm->Integral());
  if (hist_SR_norm->Integral()  != 0) hist_SR_norm->Scale(1/hist_SR_norm->Integral());
  if (hist_SB_scaledInSR_norm->Integral() != 0) hist_SB_scaledInSR_norm->Scale(1/hist_SB_scaledInSR_norm->Integral());
  if (hist_SR_sub_norm->Integral() != 0) hist_SR_sub_norm->Scale(1/hist_SR_sub_norm->Integral());

  hist_INC_norm->GetYaxis()->SetTitle("Normalized");
  hist_SBM_norm->GetYaxis()->SetTitle("Normalized");
  hist_SBP_norm->GetYaxis()->SetTitle("Normalized");
  hist_SB_norm->GetYaxis()->SetTitle("Normalized");
  hist_SR_norm->GetYaxis()->SetTitle("Normalized");
  hist_SB_scaledInSR_norm->GetYaxis()->SetTitle("Normalized");
  hist_SR_sub_norm->GetYaxis()->SetTitle("Normalized");

  TH1D* hist_GEN_norm = nullptr;
  if (doGen)
  {
    hist_GEN_norm = (TH1D*) hist_GEN->Clone(Form("%s_norm", hist_GEN->GetName()));
    if (hist_GEN_norm->GetSumw2N()==0) hist_GEN_norm->Sumw2();
    if (hist_GEN_norm->Integral() != 0) hist_GEN_norm->Scale(1.0 / hist_GEN_norm->Integral());
    hist_GEN_norm->GetYaxis()->SetTitle("Normalized");
  }

  outFile->cd();
  hist_SR_sub_norm->Write(); // keep same behavior (top-level writes the SR_sub)
  if (doGen) hist_GEN_norm->Write();

  TDirectoryFile* subDir = (TDirectoryFile*) outFile->mkdir("details");
  subDir->cd();
  hist_INC->Write();
  hist_SBM->Write();
  hist_SBP->Write();
  hist_SB->Write();
  hist_SR->Write();
  hist_SB_scaledInSR->Write();
  hist_SR_sub->Write();

  hist_INC_norm->Write();
  hist_SBM_norm->Write();
  hist_SBP_norm->Write();
  hist_SB_norm->Write();
  hist_SR_norm->Write();
  hist_SB_scaledInSR_norm->Write();
  hist_SR_sub_norm->Write();
  if (doGen)
  {
    hist_GEN->Write();
    hist_GEN_norm->Write();
  }

  //////// Step 5: Plotting
  TCanvas *cAll = new TCanvas("cAll", "cAll", 1200, 1200);
  cAll->Divide(2, 2, 0.001, 0.001);

  hist_SBP->SetLineWidth(2);
  hist_SBM->SetLineWidth(2);
  hist_INC->SetLineWidth(2);
  hist_SB->SetLineWidth(2);
  hist_SR->SetLineWidth(2);
  hist_SB_scaledInSR->SetLineWidth(2);
  hist_SR_sub->SetLineWidth(2);

  hist_INC->SetLineColorAlpha(kBlue, 0.2);
  hist_INC->SetFillColorAlpha(kBlue, 0.2);
  hist_SBM->SetLineColor(kViolet-3);
  hist_SBP->SetLineColor(kTeal-7);
  hist_SB->SetLineColor(kBlack);
  hist_SR->SetLineColor(kRed-9);
  hist_SB_scaledInSR->SetLineColorAlpha(kGray, 0.5);
  hist_SB_scaledInSR->SetFillColorAlpha(kGray, 0.5);
  hist_SR_sub->SetLineColor(kRed);

  hist_SBP_norm->SetLineWidth(2);
  hist_SBM_norm->SetLineWidth(2);
  hist_INC_norm->SetLineWidth(2);
  hist_SB_norm->SetLineWidth(2);
  hist_SR_norm->SetLineWidth(2);
  hist_SB_scaledInSR_norm->SetLineWidth(2);
  hist_SR_sub_norm->SetLineWidth(2);

  hist_INC_norm->SetLineColorAlpha(kBlue, 0.2);
  hist_INC_norm->SetFillColorAlpha(kBlue, 0.2);
  hist_SBM_norm->SetLineColor(kViolet-3);
  hist_SBP_norm->SetLineColor(kTeal-7);
  hist_SB_norm->SetLineColor(kBlack);
  hist_SR_norm->SetLineColor(kRed-9);
  hist_SB_scaledInSR_norm->SetLineColorAlpha(kGray, 0.5);
  hist_SB_scaledInSR_norm->SetFillColorAlpha(kGray, 0.5);
  hist_SR_sub_norm->SetLineColor(kRed);

  if (doGen)
  {
    hist_GEN->SetLineWidth(2);
    hist_GEN->SetLineColorAlpha(kMagenta+1, 0.5);
    hist_GEN->SetFillColorAlpha(kMagenta+1, 0.5);
    hist_GEN->SetFillStyle(3354);

    hist_GEN_norm->SetLineWidth(2);
    hist_GEN_norm->SetLineColorAlpha(kMagenta+1, 0.5);
    hist_GEN_norm->SetFillColorAlpha(kMagenta+1, 0.5);
    hist_GEN_norm->SetFillStyle(3354);
  }

  cAll->cd(1);
  gPad->SetLeftMargin(0.13);
  gPad->SetRightMargin(0.05);
  gStyle->SetOptStat(0);
  if (plotLogy) gPad->SetLogy();
  hist_INC->DrawClone("hist");
  hist_SBP->DrawClone("same e");
  hist_SBM->DrawClone("same e");
  hist_SB->DrawClone("same e");
  hist_SR->DrawClone("same e");

  TLegend *legend1 = new TLegend(0.4, 0.7, 0.9, 0.9);
  legend1->AddEntry(hist_INC, Form("Whole region [%.2f,%.2f]",   m.getMin(), m.getMax()));
  legend1->AddEntry(hist_SBM, Form("Sideband minus [%.2f,%.2f]", m.getMin(), centerVal-sideBandEdge*sigmaVal));
  legend1->AddEntry(hist_SBP, Form("Sideband plus [%.2f,%.2f]",  centerVal+sideBandEdge*sigmaVal, m.getMax()));
  legend1->AddEntry(hist_SB,  Form("Sideband region [%.2f,%.2f] || [%.2f,%.2f]", m.getMin(), centerVal-sideBandEdge*sigmaVal,
                                                                                 centerVal+sideBandEdge*sigmaVal, m.getMax()));
  legend1->AddEntry(hist_SR,  Form("Signal region [%.2f,%.2f]",  centerVal-2*sigmaVal, centerVal+2*sigmaVal));
  formatLegend(legend1,18);
  legend1->Draw();

  cAll->cd(2);
  gPad->SetLeftMargin(0.13);
  gPad->SetRightMargin(0.05);
  if (plotLogy) gPad->SetLogy();
  hist_SR->DrawClone("e");
  hist_SB_scaledInSR->DrawClone("same hist");
  hist_SR_sub->DrawClone("same e");
  // if (doGen) hist_GEN->DrawClone("same hist");

  cAll->cd(3);
  gPad->SetLeftMargin(0.13);
  gPad->SetRightMargin(0.05);
  if (plotLogy) gPad->SetLogy();
  hist_SBP_norm->DrawClone("e");
  hist_SBM_norm->DrawClone("same e");
  hist_INC_norm->DrawClone("same hist");
  hist_SB_norm->DrawClone("same e");
  hist_SR_norm->DrawClone("same e");
  legend1->Draw();

  cAll->cd(4);
  gPad->SetLeftMargin(0.13);
  gPad->SetRightMargin(0.05);
  if (plotLogy) gPad->SetLogy();
  hist_SR_norm->DrawClone("e");
  hist_SR_sub_norm->DrawClone("same e");
  if (doGen) hist_GEN_norm->DrawClone("same hist");

  TLegend *legend2 = new TLegend(0.4, 0.7, 0.9, 0.9);
  legend2->AddEntry(hist_SR,  Form("Signal region [%.2f,%.2f]",  centerVal-2*sigmaVal, centerVal+2*sigmaVal), "l");
  legend2->AddEntry(hist_SR_sub, "S.R. w/ bkg sub.", "l");
  if (doGen) legend2->AddEntry(hist_GEN, "Gen-matched (23333/23344)", "f");
  formatLegend(legend2,18);
  legend2->DrawClone();

  cAll->cd(2);
  legend2->AddEntry(hist_SB_scaledInSR, "S.B. subtraction", "f");
  legend2->DrawClone();

  // PDF name: --filesuffix if set (matches JSON / .root stem), else sanitize(--var)
  cAll->SaveAs(Form("%ssidebandSubtraction_%s.pdf", plotPrefix.c_str(), tag.c_str()));

  delete legend1;
  delete legend2;
  delete cAll;

  outFile->Close();
  delete outFile;

  return 0;
}

/** Project a branch from an event-level tree (e.g. nt_evt) without mass-fit / sideband subtraction.
 *  Writes hist_SR_sub_norm for compatibility with DataMCComp (no hist_GEN_norm). */
static int eventOnlyHistogram(
    const std::string &sampleInput,
    const std::string &output,
    const std::string &plotPrefix,
    const std::string &treeName,
    const std::string &varExpr,
    const std::string &xTitle,
    const std::string &binsCSV,
    int nbins, double xmin, double xmax,
    const std::string &plotNameTag)
{
  TFile *infile = TFile::Open(sampleInput.c_str());
  if (!infile || infile->IsZombie()) {
    std::cerr << "[eventOnlyHistogram] Failed to open: " << sampleInput << std::endl;
    return 1;
  }
  TTree *evtTree = (TTree *)infile->Get(treeName.c_str());
  if (!evtTree) {
    std::cerr << "[eventOnlyHistogram] Tree '" << treeName << "' not found in " << sampleInput << std::endl;
    infile->Close();
    delete infile;
    return 1;
  }
  if (!evtTree->GetBranch(varExpr.c_str()) && !evtTree->FindLeaf(varExpr.c_str())) {
    std::cerr << "[eventOnlyHistogram] Branch/leaf '" << varExpr << "' not found in " << treeName << std::endl;
    infile->Close();
    delete infile;
    return 1;
  }

  const std::string tag =
      plotNameTag.empty() ? SanitizeTag(varExpr) : SanitizeTag(plotNameTag);

  std::vector<double> edges;
  bool useVarBins = false;
  if (!binsCSV.empty()) {
    edges = ParseCSVdoubles(binsCSV);
    if (edges.size() >= 2)
      useVarBins = true;
    else
      std::cerr << "[eventOnlyHistogram] Warning: --bins parsed <2 edges; using uniform bins.\n";
  }

  TH1D *hist_SR = nullptr;
  if (useVarBins) {
    hist_SR = new TH1D("hist_SR", Form(";%s;Events", xTitle.c_str()), (int)edges.size() - 1, edges.data());
  } else {
    if (nbins <= 0)
      nbins = 60;
    hist_SR = new TH1D("hist_SR", Form(";%s;Events", xTitle.c_str()), nbins, xmin, xmax);
  }
  if (hist_SR->GetSumw2N() == 0)
    hist_SR->Sumw2();

  Long64_t nd = evtTree->Draw(Form("%s>>%s", varExpr.c_str(), hist_SR->GetName()), "", "goff");
  if (nd <= 0)
    std::cerr << "[eventOnlyHistogram] Warning: Draw selected 0 entries for " << varExpr << std::endl;

  normalizeHistoBinWidth(hist_SR);
  hist_SR->GetYaxis()->SetTitle("Yield per unit");

  TH1D *hist_SR_sub_norm = (TH1D *)hist_SR->Clone("hist_SR_sub_norm");
  if (hist_SR_sub_norm->GetSumw2N() == 0)
    hist_SR_sub_norm->Sumw2();
  if (hist_SR_sub_norm->Integral() != 0)
    hist_SR_sub_norm->Scale(1.0 / hist_SR_sub_norm->Integral());
  hist_SR_sub_norm->GetYaxis()->SetTitle("Normalized");

  TFile *outFile = new TFile(output.c_str(), "recreate");
  outFile->cd();
  hist_SR_sub_norm->Write();

  TDirectoryFile *subDir = (TDirectoryFile *)outFile->mkdir("details");
  subDir->cd();
  hist_SR->Write();

  bool plotLogy = false;
  TCanvas *cAll = new TCanvas("cAll", "cAll", 900, 700);
  cAll->cd();
  gPad->SetLeftMargin(0.13);
  gPad->SetRightMargin(0.05);
  gStyle->SetOptStat(0);
  hist_SR->SetLineWidth(2);
  hist_SR->SetLineColor(kBlue + 1);
  hist_SR->DrawClone("hist e");
  if (plotLogy)
    gPad->SetLogy();
  cAll->SaveAs(Form("%ssidebandSubtraction_%s.pdf", plotPrefix.c_str(), tag.c_str()));

  delete cAll;
  outFile->Close();
  delete outFile;
  delete hist_SR_sub_norm;
  delete hist_SR;

  infile->Close();
  delete infile;
  return 0;
}

int main(int argc, char *argv[])
{
  CommandLine CL(argc, argv);
  string sampleInput     = CL.Get      ("sampleInput",    "output.root"); // Input micro tree file
  bool eventOnly         = CL.GetBool  ("eventOnly",      false);          // true: project from --eventTree, no mass fit / SB sub
  string eventTreeName   = CL.Get      ("eventTree",      "nt_evt");       // Event-level tree (one row per selected event)
  string massFitResult   = CL.Get      ("massFitResult",  "fit.root");    // Input file for the invariant mass fit result
  string sideBand        = CL.Get      ("sideBand", "SB");                // SB, SBP, SBM
  double sideBandEdge    = CL.GetDouble("sideBandEdge", 3);               // #sigma away from peak for side band start
  string output          = CL.Get      ("Output", "sidebandSubtracted.root");
  string plotPrefix      = CL.Get      ("plotPrefix", "./");

  // -------- NEW CLI options for variable plotting --------
  // var can be a branch OR an expression understood by TTree::Draw, e.g. "abs(DsvpvDistance/DsvpvDisErr)"
  string varExpr         = CL.Get      ("var", "DCA");
  string xTitle          = CL.Get      ("xTitle", "DCA (cm)");

  // If bins is non-empty: variable bin edges comma-separated
  // Example: --bins "0,0.004,0.006,0.009,0.016,0.036,0.06,0.10"
  string binsCSV         = CL.Get      ("bins", "");

  // Otherwise use uniform binning
  int nbins              = CL.GetInt   ("nbins", 60);
  double xmin            = CL.GetDouble("xmin", 0.0);
  double xmax            = CL.GetDouble("xmax", 0.1);
  // If set, diagnostic PDF is sidebandSubtraction_<filesuffix>.pdf (matches JSON filesuffix / .root stem)
  string plotNameTag     = CL.Get      ("filesuffix", "");
  // ------------------------------------------------------

  if (eventOnly) {
    return eventOnlyHistogram(sampleInput, output, plotPrefix, eventTreeName,
                              varExpr, xTitle, binsCSV, nbins, xmin, xmax, plotNameTag);
  }

  int retVal = sidebandSubtraction(sampleInput, massFitResult,
                                   sideBand, sideBandEdge,
                                   output, plotPrefix,
                                   varExpr, xTitle,
                                   binsCSV, nbins, xmin, xmax,
                                   plotNameTag);

  return retVal;
}
