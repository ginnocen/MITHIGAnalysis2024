#include <TROOT.h>
#include <vector>
#include "MITHIG_CMSStyle.h"

#define DEBUG 1
#define A_POINTS 5

using namespace std;

TF1* FitToLine(
  TGraphErrors* gRAA_PtVsA_Uncert,
  TString funcName,
  float xMin,
  float xMax
) {
  TF1* fRAAvsA = new TF1(funcName, "[0]*(x-1) + [1]", xMin, xMax);
  fRAAvsA->SetParameters(-0.5 / (xMax - xMin), 1.);
  gRAA_PtVsA_Uncert->Fit(fRAAvsA, "N", "" , xMin, xMax);
  return fRAAvsA;
}

void DrawRAAvsA_DataVsTheory(
  TString output,
  TH1F* hRAA_PtVsA,
  TGraphErrors* gRAA_Data_PtVsA,
  TGraphErrors* gRAA_Data_PtVsA_TotUncert,
  int ptColor_Data,
  int ptMarker_Data,
  float ptMarkerScale_Data,
  vector<TGraphErrors*> vRAA_Theory_PtVsA,
  vector<int> ptColors_Theory,
  vector<TString> TheoryLabels,
  bool doLogx
) {
  int nLegends = 1 + vRAA_Theory_PtVsA.size();
  float scaleLegend = 1.;
  if (nLegends > 6)  scaleLegend =  6. / nLegends;
  if (nLegends > 8)  scaleLegend =  8. / nLegends;
  if (nLegends > 10) scaleLegend = 12. / nLegends;
  
  // Make canvas
  TCanvas* canvas = new TCanvas("canvas", "", 600, 600);
  TPad* pad = (TPad*) canvas->GetPad(0);
  
  // Style and draw base histogram
  hRAA_PtVsA->Draw();
  hRAA_PtVsA->SetTitleOffset(1.2, "x");
  hRAA_PtVsA->SetTitleOffset(1.5, "y");
  hRAA_PtVsA->GetXaxis()->SetTickLength(0.02);
  hRAA_PtVsA->GetYaxis()->SetTickLength(0.02);
  hRAA_PtVsA->GetXaxis()->CenterTitle(true);
  hRAA_PtVsA->GetYaxis()->CenterTitle(true);
  gStyle->SetOptStat(0);
  gPad->SetTicks(1, 1);
  pad->Update();
  
  // Make legend
  TLegend* legend;
  if (nLegends <= 10) {
    legend = new TLegend(0.19, 0.18, 0.5, 0.18 + (0.045 * nLegends * scaleLegend));
  } else {
    legend = new TLegend(0.19, 0.18, 0.78, 0.18 + (0.045 * 6));
    legend->SetNColumns(2);
  }
  legend->SetTextSize(0.035 * scaleLegend);
  legend->SetFillStyle(0);
  legend->SetBorderSize(0);
  pad->cd();
  SetTDRStyle();
  pad->SetMargin(0.16, 0.04, 0.13, 0.07);
  
  // Add line at RAA = 1
  TLine* unity = new TLine(
    hRAA_PtVsA->GetBinLowEdge(1), 1,
    hRAA_PtVsA->GetBinLowEdge(hRAA_PtVsA->GetNbinsX()+1), 1
  );
  unity->SetLineColor(kGray);
  unity->SetLineStyle(2);
  unity->Draw();
  
  // Add point for proton
  TGraph* proton = new TGraph(1);
  proton->SetPoint(0, 1, 1);
  proton->SetMarkerStyle(24);
  proton->SetMarkerColor(kBlack);
  proton->Draw("P same");
  
  // Draw Data
  // Total Uncertainty
  gRAA_Data_PtVsA_TotUncert->SetLineColor(ptColor_Data);
  gRAA_Data_PtVsA_TotUncert->SetLineWidth(1);
  gRAA_Data_PtVsA_TotUncert->Draw("E5 same");
  // Data points
  gRAA_Data_PtVsA->SetMarkerColor(ptColor_Data);
  gRAA_Data_PtVsA->SetLineColor(ptColor_Data);
  gRAA_Data_PtVsA->SetFillColorAlpha(ptColor_Data, 0.45);
  gRAA_Data_PtVsA->SetLineWidth(1);
  gRAA_Data_PtVsA->SetMarkerStyle(ptMarker_Data);
  gRAA_Data_PtVsA->SetMarkerSize(0.8 * ptMarkerScale_Data);
  gRAA_Data_PtVsA->Draw("P same");
  legend->AddEntry(
    gRAA_Data_PtVsA,
    "Data",
    "pf"
  );
  
  // Draw Theory
  for (int i = vRAA_Theory_PtVsA.size()-1; i >= 0; i--) {
    // Theory points
    vRAA_Theory_PtVsA[i]->SetMarkerColorAlpha(kBlack, 0.0);
    vRAA_Theory_PtVsA[i]->SetLineColor(ptColors_Theory[i % ptColors_Theory.size()]);
    vRAA_Theory_PtVsA[i]->SetLineWidth(1);
    vRAA_Theory_PtVsA[i]->SetMarkerStyle(0);
    vRAA_Theory_PtVsA[i]->SetFillColorAlpha(ptColors_Theory[i % ptColors_Theory.size()], 0.45);
//    vRAA_Theory_PtVsA[i]->SetFillStyle(3150 + 3 * i);
    vRAA_Theory_PtVsA[i]->Draw("E5 same");
    legend->AddEntry(
      vRAA_Theory_PtVsA[i],
      TheoryLabels[i],
      "pf"
    );
  }
  
  // Add species labels
  TLatex Alatex;
  Alatex.SetTextSize(0.032);
  Alatex.SetTextAlign(23);  // Center align to top
  Alatex.DrawLatex(
    1,
    1 - 0.03,
    "pp");
  Alatex.SetTextAlign(31);
  Alatex.DrawLatex(
    gRAA_Data_PtVsA_TotUncert->GetPointX(1) + gRAA_Data_PtVsA_TotUncert->GetErrorX(1),
    gRAA_Data_PtVsA_TotUncert->GetPointY(1) + gRAA_Data_PtVsA_TotUncert->GetErrorY(1) + 0.03,
    "OO");
  Alatex.SetTextAlign(11);
  Alatex.DrawLatex(
    gRAA_Data_PtVsA_TotUncert->GetPointX(2) - gRAA_Data_PtVsA_TotUncert->GetErrorX(2),
    gRAA_Data_PtVsA_TotUncert->GetPointY(2) + gRAA_Data_PtVsA_TotUncert->GetErrorY(2) + 0.03,
    "NeNe");
  Alatex.SetTextAlign(21);
  Alatex.DrawLatex(
    gRAA_Data_PtVsA_TotUncert->GetPointX(3),
    gRAA_Data_PtVsA_TotUncert->GetPointY(3) + gRAA_Data_PtVsA_TotUncert->GetErrorY(3) + 0.03,
    "XeXe");
  Alatex.DrawLatex(
    gRAA_Data_PtVsA_TotUncert->GetPointX(4),
    gRAA_Data_PtVsA_TotUncert->GetPointY(4) + gRAA_Data_PtVsA_TotUncert->GetErrorY(4) + 0.05,
    "PbPb");
  
  legend->Draw();
  // Add header and labels
  pad->cd();
  AddCMSHeader(pad, "Preliminary", false);
  //AddCMSHeader2(pad, "Preliminary", true);
  vector<TString> plotLabels1 = {
    "OO (5.36 TeV, 0-100%)",
    "NeNe (5.36 TeV, 0-100%)"
  };
  AddPlotLabels(
    pad,
    plotLabels1,
    0.034,
    plotTextOffset,
    0.20,
    0.88
  );
  vector<TString> plotLabels2 = {
    "XeXe (5.44 TeV, 0-80%)",
    "PbPb (5.02 TeV, 0-100%)"
  };
  AddPlotLabels(
    pad,
    plotLabels2,
    0.034,
    plotTextOffset,
    0.57,
    0.88
  );
  vector<TString> plotLabels3 = {
    "|#eta| < 1",
  };
  float etaLabelY = (0.045 * nLegends * scaleLegend) + 0.23;
  if (nLegends > 10) etaLabelY = (0.045 * 6) + 0.23;
  AddPlotLabels(
    pad,
    plotLabels3,
    0.040,
    plotTextOffset,
    0.202,
    etaLabelY
  );
  if (doLogx) gPad->SetLogx(1);
  pad->Update();

  // Make pdf and png
  system("mkdir -p plots_RAAvsA;");
  canvas->SaveAs("plots_RAAvsA/"+ output +".pdf");
  delete canvas;
  delete legend;
}

void PlotRAAVsA_DataVsTheory_Log(
  TGraphErrors* gRAA_Data_PtVsA,
  TGraphErrors* gRAA_Data_PtVsA_TotUncert,
  int ptColor_Data,
  int ptMarker_Data,
  float ptMarkerScale_Data,
  vector<TGraphErrors*> vRAA_Theory_PtVsA,
  vector<int> ptColors_Theory,
  vector<TString> TheoryLabels,
  float ptMin,
  float ptMax,
  float xMin = 0.6,
  float xMax = 400,
  float yMin = 0.0,
  float yMax = 1.4
) {
  vector<float> xErrors;
  float logWidth = 0.07;
  xErrors = {
    (float) exp(logWidth) - 1,
    (float) exp(log(16)  + logWidth) - 16,
    (float) exp(log(20)  + logWidth) - 20,
    (float) exp(log(129) + logWidth) - 129,
    (float) exp(log(208) + logWidth) - 208
  };
  
  // Update Stat and Syst error bars
  gRAA_Data_PtVsA_TotUncert->SetPointError(1, xErrors[1], gRAA_Data_PtVsA_TotUncert->GetErrorY(1));
  gRAA_Data_PtVsA_TotUncert->SetPointError(2, xErrors[2], gRAA_Data_PtVsA_TotUncert->GetErrorY(2));
  gRAA_Data_PtVsA_TotUncert->SetPointError(3, xErrors[3], gRAA_Data_PtVsA_TotUncert->GetErrorY(3));
  gRAA_Data_PtVsA_TotUncert->SetPointError(4, xErrors[4], gRAA_Data_PtVsA_TotUncert->GetErrorY(4));
  for (int i = 0; i < vRAA_Theory_PtVsA.size(); i++) {
    for (int j = 0; j < vRAA_Theory_PtVsA[i]->GetN(); j++) {
      if (vRAA_Theory_PtVsA[i]->GetPointX(j) == 16) vRAA_Theory_PtVsA[i]->SetPointError(j, xErrors[1], vRAA_Theory_PtVsA[i]->GetErrorY(j));
      else if (vRAA_Theory_PtVsA[i]->GetPointX(j) == 20) vRAA_Theory_PtVsA[i]->SetPointError(j, xErrors[2], vRAA_Theory_PtVsA[i]->GetErrorY(j));
      else if (vRAA_Theory_PtVsA[i]->GetPointX(j) == 129) vRAA_Theory_PtVsA[i]->SetPointError(j, xErrors[3], vRAA_Theory_PtVsA[i]->GetErrorY(j));
      else if (vRAA_Theory_PtVsA[i]->GetPointX(j) == 208) vRAA_Theory_PtVsA[i]->SetPointError(j, xErrors[4], vRAA_Theory_PtVsA[i]->GetErrorY(j));
    }
  }
  // Style base histogram
  TH1F* hRAA_PtVsA = new TH1F(
    "hRAA_PtVsA",
    Form("R_{AA}(A, p_{T}), %.1f < p_{T} < %.1f GeV; A; Charged particle R_{AA}", ptMin, ptMax),
    1, xMin, xMax
  );
  hRAA_PtVsA->SetMinimum(yMin);
  hRAA_PtVsA->SetMaximum(yMax);
  
  TString output = Form("ptBinned_RAAVsA_DataVsTheory_pt%.1f-%.1f", ptMin, ptMax);
  DrawRAAvsA_DataVsTheory(
    output,
    hRAA_PtVsA,
    gRAA_Data_PtVsA,
    gRAA_Data_PtVsA_TotUncert,
    ptColor_Data,
    ptMarker_Data,
    ptMarkerScale_Data,
    vRAA_Theory_PtVsA,
    ptColors_Theory,
    TheoryLabels,
    true
  );
  
  delete hRAA_PtVsA;
}

vector<vector<TGraphErrors*>> RescaleXByPower(
  TGraphErrors* gRAA_Data_PtVsA,
  TGraphErrors* gRAA_Data_PtVsA_TotUncert,
  vector<TGraphErrors*> vRAA_Theory_PtVsA,
  float ptMin,
  float ptMax,
  float xPowNum,
  float xPowDen,
  float xWidth
) {
  vector<float> powCenters = {
    1.,
    float(pow( 16., xPowNum / xPowDen)),
    float(pow( 20., xPowNum / xPowDen)),
    float(pow(129., xPowNum / xPowDen)),
    float(pow(208., xPowNum / xPowDen))
  };
  
  TGraphErrors* gRAA_Data_PtVsA_Pow = (TGraphErrors*) gRAA_Data_PtVsA->Clone("gRAA_Data_PtVsA_Pow");
  TGraphErrors* gRAA_Data_PtVsA_TotUncert_Pow = (TGraphErrors*) gRAA_Data_PtVsA_TotUncert->Clone("gRAA_Data_PtVsA_TotUncert_Pow");
  for (int j = 1; j < A_POINTS; j++) {
    gRAA_Data_PtVsA_Pow->SetPointX(j, powCenters[j]);
    gRAA_Data_PtVsA_Pow->SetPointError(j, 0, 0);
    gRAA_Data_PtVsA_TotUncert_Pow->SetPointX(j, powCenters[j]);
    gRAA_Data_PtVsA_TotUncert_Pow->SetPointError(j, xWidth, gRAA_Data_PtVsA_TotUncert_Pow->GetErrorY(j));
  }
  
  vector<TGraphErrors*> vRAA_Theory_PtVsA_Pow;
  for (int i = 0; i < vRAA_Theory_PtVsA.size(); i++) {
    TGraphErrors* gRAA_Theory_PtVsA_Pow = (TGraphErrors*) vRAA_Theory_PtVsA[i]->Clone(Form("gRAA_Theory_PtVsA_Pow_%d", i));
    for (int j = 1; j < A_POINTS; j++) {
      gRAA_Theory_PtVsA_Pow->SetPointX(j, powCenters[j]);
      gRAA_Theory_PtVsA_Pow->SetPointError(j, xWidth, gRAA_Theory_PtVsA_Pow->GetErrorY(j));
    }
    vRAA_Theory_PtVsA_Pow.push_back(gRAA_Theory_PtVsA_Pow);
  }
  
  system("mkdir -p ptBinned_RAAVsA_DataVsTheory");
  TFile* fout = new TFile(
    Form("ptBinned_RAAVsA_DataVsTheory/ptBinned_RAAVsA_DataVsTheory_Pow%.f-%.f_pt%.1f-%.1f.root", xPowNum, xPowDen, ptMin, ptMax),
    "RECREATE"
  );
  fout->cd();
  gRAA_Data_PtVsA_Pow->Write();
  gRAA_Data_PtVsA_TotUncert_Pow->Write();
  for (int i = 0; i < vRAA_Theory_PtVsA_Pow.size(); i++) {
    vRAA_Theory_PtVsA_Pow[i]->Write();
  }
  fout->Close();
  
  vector<TGraphErrors*> vRAA_Data_PtVsA_Pow = {gRAA_Data_PtVsA_Pow};
  vector<TGraphErrors*> vRAA_Data_PtVsA_TotUncert_Pow = {gRAA_Data_PtVsA_TotUncert_Pow};
  
  vector<vector<TGraphErrors*>> vRAA_PowRescaled = {
    vRAA_Data_PtVsA_Pow,
    vRAA_Data_PtVsA_TotUncert_Pow,
    vRAA_Theory_PtVsA_Pow
  };
  return vRAA_PowRescaled;
}

void PlotRAAVsA_DataVsTheory_Pow(
  TGraphErrors* gRAA_Data_PtVsA,
  TGraphErrors* gRAA_Data_PtVsA_TotUncert,
  int ptColor_Data,
  int ptMarker_Data,
  float ptMarkerScale_Data,
  vector<TGraphErrors*> vRAA_Theory_PtVsA,
  vector<int> ptColors_Theory,
  vector<TString> TheoryLabels,
  float ptMin,
  float ptMax,
  float xPowNum,
  float xPowDen,
  float xWidth,
  float xMin,
  float xMax,
  float yMin = 0.0,
  float yMax = 1.4
) {
  vector<vector<TGraphErrors*>> vRAA_PowRescaled = RescaleXByPower(
    gRAA_Data_PtVsA,
    gRAA_Data_PtVsA_TotUncert,
    vRAA_Theory_PtVsA,
    ptMin,
    ptMax,
    xPowNum,
    xPowDen,
    xWidth
  );
  TGraphErrors* gRAA_Data_PtVsA_Pow = vRAA_PowRescaled[0][0];
  TGraphErrors* gRAA_Data_PtVsA_TotUncert_Pow = vRAA_PowRescaled[1][0];
  vector<TGraphErrors*> vRAA_Theory_PtVsA_Pow = vRAA_PowRescaled[2];
  
  // Style base histogram
  TH1F* hRAA_PtVsA = new TH1F(
    "hRAA_PtVsA",
    Form("R_{AA}(A, p_{T}), %.1f < p_{T} < %.1f GeV; A^{%.f/%.f}; Charged particle R_{AA}",  ptMin, ptMax, xPowNum, xPowDen),
    1, xMin, xMax
  );
  hRAA_PtVsA->SetMinimum(yMin);
  hRAA_PtVsA->SetMaximum(yMax);
  
  TString output = Form("ptBinned_RAAVsA_DataVsTheory_Pow%.f-%.f_pt%.1f-%.1f", xPowNum, xPowDen, ptMin, ptMax);
  DrawRAAvsA_DataVsTheory(
    output,
    hRAA_PtVsA,
    gRAA_Data_PtVsA,
    gRAA_Data_PtVsA_TotUncert,
    ptColor_Data,
    ptMarker_Data,
    ptMarkerScale_Data,
    vRAA_Theory_PtVsA,
    ptColors_Theory,
    TheoryLabels,
    false
  );
  
  delete hRAA_PtVsA;
}


void MakePlots_RebinRAAVsA_DataVsTheory(
) {
  // Styling
  vector<int> ptColorsRainbow = {
    kPink-2,
//    kPink+9,
    kViolet-2,
//    kViolet+8,
    kAzure-2,
    kAzure+10,
//    kTeal-2,
    kSpring-8,
    kOrange-2,
    kOrange+8
  };
  vector<int> ptColorsRootVis = {
    kP10Blue,   kP10Yellow, kP10Red,    kP10Gray,   kP10Violet,
    kP10Brown,  kP10Orange, kP10Green,  kP10Ash,    kP10Cyan
  };
  vector<int> ptMarkersFilled = {
     20,  21,  33,  43,  34,  47,  22,  23,  41,  45
  };
  vector<int> ptMarkersOpen = {
     24,  25,  27,  42,  28,  46,  26,  32,  40,  44
  };
  vector<float> ptMarkerScales = {
    1.0, 0.9, 1.5, 1.5, 1.0, 1.0, 1.1, 1.1, 1.0, 0.9
  };
  
  // Get TGraphs from root file
  TString finpath = "ptBinned_RAAVsA.root";
  TFile* fin_graphs = new TFile(finpath,   "READ");
  vector<TGraphErrors*> vRAA_Data_PtVsA;
  vector<TGraphErrors*> vRAA_Data_PtVsA_TotUncert;
  for (int i=0; i<15; i++) {
    TGraphErrors* tempRAA = (TGraphErrors*) fin_graphs->Get(Form("gRAA_PtVsA_ptBin%d", i));
    TGraphErrors* tempRAA_TotUncert = (TGraphErrors*) fin_graphs->Get(Form("gRAA_PtVsA_TotUncert_ptBin%d", i));
    if (tempRAA != nullptr && tempRAA_TotUncert != nullptr) {
      vRAA_Data_PtVsA.push_back(tempRAA);
      vRAA_Data_PtVsA_TotUncert.push_back(tempRAA_TotUncert);
    }
    else {
      delete tempRAA;
      delete tempRAA_TotUncert;
    }
  }
  
  TFile* fin_FaraHoro = TFile::Open("Theory_RAAvsA/theory_RAAvsA_Faraday-Horowitz.root", "READ");
  TFile* fin_Zakharov = TFile::Open("Theory_RAAvsA/theory_RAAvsA_Zakharov.root", "READ");
  TFile* fin_BayesQHt = TFile::Open("Theory_RAAvsA/theory_RAAvsA_Bayesian-qHat_fix.root", "READ");
  vector<TString> graphNames = {
    "gRAA_pT_7p2_9p6",
    "gRAA_pT_9p6_12",
    "gRAA_pT_12_14p4",
    "gRAA_pT_14p4_19p2",
    "gRAA_pT_19p2_24",
    "gRAA_pT_24_28p8",
    "gRAA_pT_28p8_35p2",
    "gRAA_pT_35p2_48",
    "gRAA_pT_48_73p6",
    "gRAA_pT_73p6_103p6"
  };
  vector<float> ptMinMax = {
     7.2,  9.6, 12.0, 14.4, 19.2,
    24.0, 28.8, 35.2, 48.0, 73.6,
    103.6
  };
  vector<int> ptColors_Theory = {
    kPink+9,    // Faraday-Horowitz
    kAzure+8,   // Zakharov
    kSpring-2   // Bayesian qHat
  };
  vector<TString> TheoryLabels = {
    "Faraday-Horowitz",
    "Zakharov",
    "Bayesian qHat",
  };
  
  for (int i = 0 ; i < graphNames.size() - 1 ; i++) {
    TGraphErrors* gRAA_PtVsA_FaraHoro = (TGraphErrors*) fin_FaraHoro->Get(graphNames[i]);
    TGraphErrors* gRAA_PtVsA_Zakharov = (TGraphErrors*) fin_Zakharov->Get(graphNames[i]);
    TGraphErrors* gRAA_PtVsA_BayesQHt = (TGraphErrors*) fin_BayesQHt->Get(graphNames[i]);
    
    vector<TGraphErrors*> vRAA_Theory_PtVsA = {
      gRAA_PtVsA_FaraHoro, gRAA_PtVsA_Zakharov, gRAA_PtVsA_BayesQHt
    };
    PlotRAAVsA_DataVsTheory_Log(
      vRAA_Data_PtVsA[i+5],
      vRAA_Data_PtVsA_TotUncert[i+5],
      kBlack,
      24,
      1.0,
      vRAA_Theory_PtVsA,
      ptColors_Theory,
      TheoryLabels,
      ptMinMax[i],
      ptMinMax[i+1],
      0.4,  // xMin
      400,  // xMax
      0.0,  // yMin
      1.4   // yMax
    );
    PlotRAAVsA_DataVsTheory_Pow(
      vRAA_Data_PtVsA[i+5],
      vRAA_Data_PtVsA_TotUncert[i+5],
      kBlack,
      24,
      1.0,
      vRAA_Theory_PtVsA,
      ptColors_Theory,
      TheoryLabels,
      ptMinMax[i],
      ptMinMax[i+1],
      1,    // pow numerator
      3,    // pow denominator
      0.06, // xWidth
      0.6,  // xMin
      6.4,  // xMax
      0.0,  // yMin
      1.4   // yMax
    );
  }
  
//  // Combine above into RAA pT vs A plot:
//  vector<int> binsToSkip;
//  
//  binsToSkip = {};
//  PlotRAAVsA_Log(
//    vRAA_PtVsA,
//    vRAA_PtVsA_Stat,
//    vRAA_PtVsA_Syst,
//    vRAA_PtVsA_Norm,
//    ptColorsRainbow,
//    ptMarkersOpen,
//    ptMarkerScales,
//    binsToSkip,
//    "ptBinned_RAAVsA_AllBins",
//    0.4, 400 // xMin, xMax
//  );
//  PlotRAAVsA_Log(
//    vRAA_PtVsA,
//    vRAA_PtVsA_Stat,
//    vRAA_PtVsA_Syst,
//    vRAA_PtVsA_Norm,
//    ptColorsRainbow,
//    ptMarkersOpen,
//    ptMarkerScales,
//    binsToSkip,
//    "ptBinned_RAAVsA_AllBins",
//    0.4, 400 // xMin, xMax
//  );
//  PlotRAAVsA_Pow(
//    vRAA_PtVsA,
//    vRAA_PtVsA_Stat,
//    vRAA_PtVsA_Syst,
//    vRAA_PtVsA_Norm,
//    vRAA_PtVsA_TotUncert,
//    ptColorsRainbow,
//    ptMarkersOpen,
//    ptMarkerScales,
//    binsToSkip,
//    "ptBinned_RAAVsA_Pow1-3_AllBins",
//    1, 3, // Pow_Num, Pow_Den
//    0.06, // xWidth
//    -2., 6.4 // xMin, xMax
//  );
//  
//  // RAA vs A select pt bins
//  binsToSkip = {0, 1, 2, 3, 4, 5, 7, 9, 11, 14};
//  PlotRAAVsA_Log(
//    vRAA_PtVsA,
//    vRAA_PtVsA_Stat,
//    vRAA_PtVsA_Syst,
//    vRAA_PtVsA_Norm,
//    ptColorsRainbow,
//    ptMarkersOpen,
//    ptMarkerScales,
//    binsToSkip,
//    "ptBinned_RAAVsA_SelectBins"
//  );
//  PlotRAAVsA_Pow(
//    vRAA_PtVsA,
//    vRAA_PtVsA_Stat,
//    vRAA_PtVsA_Syst,
//    vRAA_PtVsA_Norm,
//    vRAA_PtVsA_TotUncert,
//    ptColorsRainbow,
//    ptMarkersOpen,
//    ptMarkerScales,
//    binsToSkip,
//    "ptBinned_RAAVsA_Pow1-3_SelectBins",
//    1, 3, // Pow_Num, Pow_Den
//    0.06, // xWidth
//    0.6, 6.4 // xMin, xMax
//  );
//  
  // Cleanup
  fin_graphs->Close();
  fin_FaraHoro->Close();
  fin_Zakharov->Close();
  fin_BayesQHt->Close();
  delete fin_graphs;
  delete fin_FaraHoro;
  delete fin_Zakharov;
  delete fin_BayesQHt;
}
