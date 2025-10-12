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

void plotEventCorrectionMCvsData() {
  // Make output directory and simple hists for examples
  system("mkdir -p ./plots");

  const char* output = "plots/20250723_Skim_ppref2024_Data_EvtCorrComparison";

  // ===========================================================================
  TCanvas* c_trackPt = new TCanvas("c_trackPt", "", 800, 600);

  // ppref data analysis output
  TFile* fin = TFile::Open("output/output_20250723_Skim_ppref2024_Data_noEvtSel.root");
  TH1D* hTrkPt = (TH1D*)fin->Get("hTrkPt");
  TH1D* hTrkPt_noSel = (TH1D*)fin->Get("hTrkPt_noSel");

  // data with data-driven correction output
  TFile* fin_DataDrivenCorrection = TFile::Open("output/output_20250723_Skim_ppref2024_Data_noEvtSel_DataDrivenCorrection.root");
  TH1D* hTrkPt_DataDrivenCorrection = (TH1D*)fin_DataDrivenCorrection->Get("hTrkPt");
  hTrkPt_DataDrivenCorrection->SetName("hTrkPt_DataDrivenCorrection");

  // data with MC-driven correction output
  TFile* fin_MCDrivenCorrection = TFile::Open("output/output_20250723_Skim_ppref2024_Data_noEvtSel_MCDrivenCorrection.root");
  TH1D* hTrkPt_MCDrivenCorrection = (TH1D*)fin_MCDrivenCorrection->Get("hTrkPt");
  hTrkPt_MCDrivenCorrection->SetName("hTrkPt_MCDrivenCorrection");

  divideByWidth(hTrkPt);
  divideByWidth(hTrkPt_noSel);
  divideByWidth(hTrkPt_DataDrivenCorrection);
  divideByWidth(hTrkPt_MCDrivenCorrection);

  TPad* pad_trackPt = (TPad*) plotCMSRatio(
    {hTrkPt, hTrkPt_DataDrivenCorrection, hTrkPt_MCDrivenCorrection, hTrkPt_noSel}, "", {"selected events", "Data-driven correction", "MC-driven correction", "no event selection"},
    {cmsRed, cmsYellow, cmsBlue, cmsBlack}, {1, 1, 1, -1}, {cmsBlack, cmsBlack, cmsBlack, cmsBlack}, {0, 0, 0, 20},
    "p_{T}", 0.5, 50,
    "dN/dp_{T}", 1, 1e12,
    "Fraction of Events Selected  ", 0.97, 1.03,
    3,
    true, true, true
  );

  AddCMSHeader(
    pad_trackPt,      // Provide the TPad
    "Internal", // (optional) Add a subheader to the CMS header
    true
  );

  AddUPCHeader(pad_trackPt, "5.36 TeV", "pp ref");
  pad_trackPt->Update();

  c_trackPt->SaveAs(Form("%s_TrackPt.pdf", output));


  // ===========================================================================
  TCanvas* c_eventWeight = new TCanvas("c_eventweight", "", 800, 600);

  TH1D* DataDrivenEventWeight = (TH1D*)fin_DataDrivenCorrection->Get("hTrkWeight");

  TPad *pad_eventWeight = (TPad*) plotCMSSimple(
    c_eventWeight, {DataDrivenEventWeight}, "", {"Data-driven event weight"},
    {cmsRed}, {1}, {cmsBlack}, {0},
    "weight", 0, 1.05,
    "counts", -1, -1,
    false, false, false
  );

  AddCMSHeader(
    pad_eventWeight,      // Provide the TPad
    "Internal", // (optional) Add a subheader to the CMS header
    true
  );

  AddUPCHeader(pad_eventWeight, "5.36 TeV", "pp ref");
  pad_eventWeight->Update();

  c_eventWeight->SaveAs(Form("%s_EventWeight.pdf", output));

}
