// run with
// root -l -b -q PlottingExample.C
#include <filesystem>
#include <iostream>
#include "TCanvas.h"
#include "TPad.h"
#include "TH1.h"
#include "TH2.h"
#include "TRandom.h"
#include "include/plotting.h"

void plotEventSelection() {
  // Make output directory and simple hists for examples
  system("mkdir -p ./plots");

  // ppref Data
  TFile* fin = TFile::Open("output/output_20250723_Skim_ppref2024_Data_noEvtSel.root");
  const char* output = "plots/20250723_Skim_ppref2024_Data_noEvtSel";

  // ppref MC
  //TFile* fin = TFile::Open("output/20250723_Skim_ppref2024_Official_noEvtSel.root");
  //const char* output = "plots/20250723_Skim_ppref2024_Official_noEvtSel";

  // ===========================================================================
  TCanvas* ex2Canvas = new TCanvas("ex2Canvas", "", 800, 600);

  TH1D* hNEvtPassCuts = (TH1D*)fin->Get("hNEvtPassCuts");
  hNEvtPassCuts->Scale(1./hNEvtPassCuts->GetBinContent(1));

  TPad* pad_Nevt = (TPad*) plotCMSSimple(
    ex2Canvas, {hNEvtPassCuts}, "", {"PPRefZeroBiasPlusForward4"},
    {cmsRed, cmsRed}, {0, 0}, {cmsBlack, cmsBlack}, {0, 0},
    "", -1, -1,
    "Events passed", 0, 1.5,
    false, false, true
  );

  AddCMSHeader(
    pad_Nevt,      // Provide the TPad
    "Internal", // (optional) Add a subheader to the CMS header
    true
  );
  AddUPCHeader(pad_Nevt, "5.36 TeV", "pp ref");
  pad_Nevt->Update();

  // That's it!
  ex2Canvas->SaveAs(Form("%s_Nevt.pdf", output));

  // ===========================================================================
  TCanvas* c_eventCor_vsLeadingPt = new TCanvas("ex3Canvas", "", 800, 600);

  TH1D* hLeadingTrkPt_noSel = (TH1D*)fin->Get("hLeadingTrkPt_noSel");
  TH1D* hLeadingTrkPt = (TH1D*)fin->Get("hLeadingTrkPt");

  TPad* pad_selVsPt = (TPad*) plotCMSRatio(
    {hLeadingTrkPt_noSel, hLeadingTrkPt}, "", {"no event selection", "PV Filter & |vz|<15"},
    {cmsBlue, cmsRed}, {1, 1}, {cmsBlack, cmsBlack}, {0, 0},
    "Leading Track p_{T} [GeV/c]", 0.5, 80,
    "Number of Events", 1e1, 1e8,
    "Selection Efficiency", 0.85, 1.15,
    0,
    false, true, true
  );

  AddCMSHeader(
    pad_selVsPt,      // Provide the TPad
    "Internal", // (optional) Add a subheader to the CMS header
    true
  );

  AddUPCHeader(pad_selVsPt, "5.36 TeV", "pp ref");
  pad_selVsPt->Update();

  c_eventCor_vsLeadingPt->SaveAs(Form("%s_LeadingPt.pdf", output));

  // ===========================================================================
  TCanvas* c_mult = new TCanvas("c_mult", "", 800, 600);

  TH1D* hMult_noSel_oneVtx = (TH1D*)fin->Get("hMult_noSel_oneVtx");
  TH1D* hMult_oneVtx = (TH1D*)fin->Get("hMult_oneVtx");
  TH1D* hMult_noSel_oneVtx_Eta1p5 = (TH1D*)fin->Get("hMult_noSel_oneVtx_Eta1p5");
  TH1D* hMult_oneVtx_Eta1p5 = (TH1D*)fin->Get("hMult_oneVtx_Eta1p5");

  TPad* pad_mult = (TPad*) plotCMSRatio(
    {hMult_oneVtx, hMult_noSel_oneVtx}, "", {"nVtx=1, PV Filter & |vz|<15", "nVtx=1, no event selection"},
    {cmsRed, cmsBlue}, {1, 2}, {cmsBlack, cmsBlack}, {0, 0},
    "Multiplicity", 0, 20,
    "Number of Events", 1e5, 2e7,
    "Fraction of Events Selected  ", 0.9, 1.1,
    1,
    false, true, true
  );

  AddCMSHeader(
    pad_mult,      // Provide the TPad
    "Internal", // (optional) Add a subheader to the CMS header
    true
  );

  AddUPCHeader(pad_mult, "5.36 TeV", "pp ref");
  pad_mult->Update();

  c_mult->SaveAs(Form("%s_Mult.pdf", output));

  // ===========================================================================
  TCanvas* c_mult_ratio = new TCanvas("c_mult_ratio", "", 800, 600);

  TH1D* hMultEff = (TH1D*)hMult_oneVtx->Clone("hEff");
  hMultEff->Divide(hMult_noSel_oneVtx);
  TH1D* hMultEff_Eta1p5 = (TH1D*)hMult_oneVtx_Eta1p5->Clone("hMultEff_Eta1p5");
  hMultEff_Eta1p5->Divide(hMult_noSel_oneVtx_Eta1p5);

  TPad* pad_mult_ratio = (TPad*) plotCMSSimple(
    c_mult_ratio, {hMultEff_Eta1p5, hMultEff}, "", {"selected / zero bias; |eta|<1.5", "selected / zero bias; |eta|<2.4"},
    {cmsRed, cmsBlue}, {1, 1}, {cmsRed, cmsBlue}, {24, 24},
    "Multiplicity", 1, 110,
    "Fraction of Events Selected", 0.9, 1.1,
    true, false, false
  );

  AddCMSHeader(
    pad_mult_ratio,      // Provide the TPad
    "Internal", // (optional) Add a subheader to the CMS header
    true
  );

  AddPlotLabels(
    pad_mult_ratio, 
    {"selected: nVtx=1 & real vertex & |vz|<15 & PV Filter", "zero bias: nVtx=1 & real vertex"},
    0.04, 0.015, 0.25, 0.65
  );

  AddUPCHeader(pad_mult_ratio, "5.36 TeV", "pp ref");
  pad_mult_ratio->Update();

  pad_mult_ratio->SaveAs(Form("%s_MultRatio.pdf", output));

  // print value of the correction for all bins 10<mult<70
  /*
  for (int i = 1; i <= hMultEff->GetNbinsX(); ++i) {
    double mult = hMultEff->GetBinCenter(i);
    double eff = hMultEff->GetBinContent(i);
    if (mult > 10 && mult < 100) {
      double eff_err = hMultEff->GetBinError(i);
      cout << "Mult: " << mult << ", Efficiency: " << eff << " +/- " << eff_err << endl;
    }
  }
  */

  // save mult ratio as Fraction of Events Selected as a function of multiplicity
  // correction factor should be 1/bin content
  TFile* fout = TFile::Open(Form("%s_EventCorrection.root", output), "RECREATE");
  fout->cd();
  hMultEff->Write();
  fout->Close();

}
