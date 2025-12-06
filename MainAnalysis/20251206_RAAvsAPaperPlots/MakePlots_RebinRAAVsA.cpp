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

void DrawRAAvsA(
  TString output,
  TH1F* hRAA_PtVsA,
  vector<TGraphErrors*> vRAA_PtVsA,
  vector<TGraphErrors*> vRAA_PtVsA_Stat,
  vector<TGraphErrors*> vRAA_PtVsA_Syst,
  vector<TGraphErrors*> vRAA_PtVsA_Norm,
  vector<int> ptColors,
  vector<int> ptMarkers,
  vector<float> ptMarkerScales,
  vector<int> binsToSkip,
  bool doLogx
) {
  int nPtBins = vRAA_PtVsA.size() - binsToSkip.size();
  float scaleLegend = 1.;
  if (nPtBins > 6) scaleLegend = 6. / nPtBins;
  if (nPtBins > 8) scaleLegend = 8. / nPtBins;
  if (nPtBins > 10) scaleLegend = 12. / (nPtBins + (nPtBins%2));
  
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
  if (nPtBins <= 10) {
    legend = new TLegend(0.19, 0.18, 0.5, 0.18 + (0.045 * nPtBins * scaleLegend));
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
  
  // Add norm systematics
  float O_Norm = 0;
  float Ne_Norm = 0;
  float Xe_Norm = 0;
  float Pb_Norm = 0;
  int O_minBin = 0;
  int Ne_minBin = 0;
  int Xe_minBin = 0;
  int Pb_minBin = 0;
  for (int i = 0; i < vRAA_PtVsA_Syst.size(); i++) {
    if (std::find(binsToSkip.begin(), binsToSkip.end(), i) != binsToSkip.end()) continue;
    if (vRAA_PtVsA_Syst[i]->GetPointY(1) - vRAA_PtVsA_Syst[i]->GetErrorY(1) <
        vRAA_PtVsA_Syst[O_minBin]->GetPointY(1) - vRAA_PtVsA_Syst[O_minBin]->GetErrorY(1)
      ) O_minBin = i;
    if (vRAA_PtVsA_Syst[i]->GetPointY(2) - vRAA_PtVsA_Syst[i]->GetErrorY(2) <
        vRAA_PtVsA_Syst[Ne_minBin]->GetPointY(2) - vRAA_PtVsA_Syst[Ne_minBin]->GetErrorY(2)
      ) Ne_minBin = i;
    if (vRAA_PtVsA_Syst[i]->GetPointY(3) - vRAA_PtVsA_Syst[i]->GetErrorY(3) <
        vRAA_PtVsA_Syst[Xe_minBin]->GetPointY(3) - vRAA_PtVsA_Syst[Xe_minBin]->GetErrorY(3)
      ) Xe_minBin = i;
    if (vRAA_PtVsA_Syst[i]->GetPointY(4) - vRAA_PtVsA_Syst[i]->GetErrorY(4) <
        vRAA_PtVsA_Syst[Pb_minBin]->GetPointY(4) - vRAA_PtVsA_Syst[Pb_minBin]->GetErrorY(4)
      ) Pb_minBin = i;
    O_Norm = TMath::Max(
      float(vRAA_PtVsA_Norm[i]->GetErrorY(1) / vRAA_PtVsA[i]->GetPointY(1)),
      O_Norm
    );
    Ne_Norm = TMath::Max(
      float(vRAA_PtVsA_Norm[i]->GetErrorY(2) / vRAA_PtVsA[i]->GetPointY(2)),
      Ne_Norm
    );
    Xe_Norm = TMath::Max(
      float(vRAA_PtVsA_Norm[i]->GetErrorY(3) / vRAA_PtVsA[i]->GetPointY(3)),
      Xe_Norm
    );
    Pb_Norm = TMath::Max(
      float(vRAA_PtVsA_Norm[i]->GetErrorY(4) / vRAA_PtVsA[i]->GetPointY(4)),
      Pb_Norm
    );
  }
  if (DEBUG) {
    cout << ">>> Summary of Norm Uncertainties:" << endl;
    cout << "  OO:   " << O_Norm << endl;
    cout << "  NeNe: " << Ne_Norm << endl;
    cout << "  XeXe: " << Xe_Norm << endl;
    cout << "  PbPb: " << Pb_Norm << endl;
  }
  TGraphErrors* gNormSyst = new TGraphErrors(5);
  gNormSyst->SetPoint(1, vRAA_PtVsA[1]->GetPointX(1), 1.);
  gNormSyst->SetPointError(1, vRAA_PtVsA_Syst[1]->GetErrorX(1), O_Norm);
  gNormSyst->SetPoint(2, vRAA_PtVsA[1]->GetPointX(2), 1.);
  gNormSyst->SetPointError(2, vRAA_PtVsA_Syst[1]->GetErrorX(2), Ne_Norm);
  gNormSyst->SetPoint(3, vRAA_PtVsA[1]->GetPointX(3), 1.);
  gNormSyst->SetPointError(3, vRAA_PtVsA_Syst[1]->GetErrorX(3), Xe_Norm);
  gNormSyst->SetPoint(4, vRAA_PtVsA[1]->GetPointX(4), 1.);
  gNormSyst->SetPointError(4, vRAA_PtVsA_Syst[1]->GetErrorX(4), Pb_Norm);
  gNormSyst->SetFillColorAlpha(kGray, 0.45);
  gNormSyst->SetLineWidth(0);
  gNormSyst->Draw("E2 same");
  
  // Add point for proton
  TGraph* proton = new TGraph(1);
  proton->SetPoint(0, 1, 1);
  proton->SetMarkerStyle(20);
  proton->SetMarkerColor(kBlack);
  proton->Draw("P same");
  
  // Add species labels
  TLatex Alatex;
  Alatex.SetTextSize(0.032);
  Alatex.SetTextAlign(23);  // Center align to top
  Alatex.DrawLatex(
    1,
    1 - 0.03,
    "pp");
  Alatex.SetTextAlign(33);
  Alatex.DrawLatex(
    vRAA_PtVsA[O_minBin]->GetPointX(1) + vRAA_PtVsA_Syst[O_minBin]->GetErrorX(1),
    vRAA_PtVsA[O_minBin]->GetPointY(1) - vRAA_PtVsA_Syst[O_minBin]->GetErrorY(1) - 0.03,
    "OO");
  Alatex.SetTextAlign(13);
  Alatex.DrawLatex(
    vRAA_PtVsA[Ne_minBin]->GetPointX(2) - vRAA_PtVsA_Syst[Ne_minBin]->GetErrorX(2),
    vRAA_PtVsA[Ne_minBin]->GetPointY(2) - vRAA_PtVsA_Syst[Ne_minBin]->GetErrorY(2) - 0.03,
    "NeNe");
  Alatex.SetTextAlign(23);
  Alatex.DrawLatex(
    vRAA_PtVsA[Xe_minBin]->GetPointX(3),
    vRAA_PtVsA[Xe_minBin]->GetPointY(3) - vRAA_PtVsA_Syst[Xe_minBin]->GetErrorY(3) - 0.03,
    "XeXe");
  Alatex.DrawLatex(
    vRAA_PtVsA[Pb_minBin]->GetPointX(4),
    vRAA_PtVsA[Pb_minBin]->GetPointY(4) - vRAA_PtVsA_Syst[Pb_minBin]->GetErrorY(4) - 0.05,
    "PbPb");
  
  // Draw Systematic Uncertainties
  int iStyle = 0;
  for (int i = 0; i < vRAA_PtVsA_Syst.size(); i++) {
    if(std::find(binsToSkip.begin(), binsToSkip.end(), i) != binsToSkip.end()) continue;
    vRAA_PtVsA_Syst[i]->SetFillColorAlpha(ptColors[iStyle % ptColors.size()], 0.45);
    vRAA_PtVsA_Syst[i]->SetLineWidth(0);
    vRAA_PtVsA_Syst[i]->Draw("E2 same");
    iStyle++;
  }
  // Draw Statistical Uncertainties
  iStyle = 0;
  for (int i = 0; i < vRAA_PtVsA_Stat.size(); i++) {
    if(std::find(binsToSkip.begin(), binsToSkip.end(), i) != binsToSkip.end()) continue;
    vRAA_PtVsA_Stat[i]->SetLineColor(ptColors[iStyle % ptColors.size()]);
    vRAA_PtVsA_Stat[i]->SetLineWidth(2);
    vRAA_PtVsA_Stat[i]->Draw("E5 same");
    iStyle++;
  }
  // Draw Points
  vector<TF1*> vFits;
  iStyle = 0;
  for (int i = 0; i < vRAA_PtVsA.size(); i++) {
    if(std::find(binsToSkip.begin(), binsToSkip.end(), i) != binsToSkip.end()) continue;
    vRAA_PtVsA[i]->SetMarkerColor(ptColors[iStyle % ptColors.size()]);
    vRAA_PtVsA[i]->SetLineColor(ptColors[iStyle % ptColors.size()]);
    vRAA_PtVsA[i]->SetFillColorAlpha(ptColors[iStyle % ptColors.size()], 0.45);
    vRAA_PtVsA[i]->SetLineWidth(1);
    vRAA_PtVsA[i]->SetMarkerStyle(ptMarkers[iStyle % ptMarkers.size()]);
    vRAA_PtVsA[i]->SetMarkerSize(0.8 * ptMarkerScales[iStyle % ptMarkers.size()]);
    vRAA_PtVsA[i]->Draw("P same");
    legend->AddEntry(
      vRAA_PtVsA[i],
      vRAA_PtVsA[i]->GetTitle(),
      "pf"
    );
    iStyle++;
  }
  legend->Draw();
  // Add header and labels
  pad->cd();
  AddCMSHeader(pad, "Preliminary", false);
//  AddCMSHeader2(pad, "Preliminary", true);
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
  float etaLabelY = (0.045 * nPtBins * scaleLegend) + 0.23;
  if (nPtBins > 10) etaLabelY = (0.045 * 6) + 0.23;
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

void PlotRAAVsA_Log(
  vector<TGraphErrors*> vRAA_PtVsA,
  vector<TGraphErrors*> vRAA_PtVsA_Stat,
  vector<TGraphErrors*> vRAA_PtVsA_Syst,
  vector<TGraphErrors*> vRAA_PtVsA_Norm,
  vector<int> ptColors,
  vector<int> ptMarkers,
  vector<float> ptMarkerScales,
  vector<int> binsToSkip = {},
  TString output = "ptBinned_RAAVsA",
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
  int iStyle = 0;
  for (int i = 0; i < vRAA_PtVsA.size(); i++) {
    vRAA_PtVsA_Stat[i]->SetPointError(1, 0, vRAA_PtVsA_Stat[i]->GetErrorY(1));
    vRAA_PtVsA_Stat[i]->SetPointError(2, 0, vRAA_PtVsA_Stat[i]->GetErrorY(2));
    vRAA_PtVsA_Stat[i]->SetPointError(3, 0, vRAA_PtVsA_Stat[i]->GetErrorY(3));
    vRAA_PtVsA_Stat[i]->SetPointError(4, 0, vRAA_PtVsA_Stat[i]->GetErrorY(4));
    vRAA_PtVsA_Syst[i]->SetPointError(1, xErrors[1], vRAA_PtVsA_Syst[i]->GetErrorY(1));
    vRAA_PtVsA_Syst[i]->SetPointError(2, xErrors[2], vRAA_PtVsA_Syst[i]->GetErrorY(2));
    vRAA_PtVsA_Syst[i]->SetPointError(3, xErrors[3], vRAA_PtVsA_Syst[i]->GetErrorY(3));
    vRAA_PtVsA_Syst[i]->SetPointError(4, xErrors[4], vRAA_PtVsA_Syst[i]->GetErrorY(4));
  }
  
  // Style base histogram
  TH1F* hRAA_PtVsA = new TH1F(
    "hRAA_PtVsA",
    "R_{AA}(A, p_{T}); A; Charged particle R_{AA}",
    1, xMin, xMax
  );
  hRAA_PtVsA->SetMinimum(yMin);
  hRAA_PtVsA->SetMaximum(yMax);
  
  DrawRAAvsA(
    output,
    hRAA_PtVsA,
    vRAA_PtVsA,
    vRAA_PtVsA_Stat,
    vRAA_PtVsA_Syst,
    vRAA_PtVsA_Norm,
    ptColors,
    ptMarkers,
    ptMarkerScales,
    binsToSkip,
    true
  );
  
  delete hRAA_PtVsA;
}

vector<vector<TGraphErrors*>> RescaleXByPower(
  vector<TGraphErrors*> vRAA_PtVsA,
  vector<TGraphErrors*> vRAA_PtVsA_Stat,
  vector<TGraphErrors*> vRAA_PtVsA_Syst,
  vector<TGraphErrors*> vRAA_PtVsA_Norm,
  vector<TGraphErrors*> vRAA_PtVsA_TotUncert,
  float xPowNum,
  float xPowDen,
  float xWidth
) {
  vector<TGraphErrors*> vRAA_PtVsA_Pow;
  vector<TGraphErrors*> vRAA_PtVsA_Stat_Pow;
  vector<TGraphErrors*> vRAA_PtVsA_Syst_Pow;
  vector<TGraphErrors*> vRAA_PtVsA_Norm_Pow;
  vector<TGraphErrors*> vRAA_PtVsA_TotUncert_Pow;
  
  vector<float> powCenters = {
    1.,
    float(pow( 16., xPowNum / xPowDen)),
    float(pow( 20., xPowNum / xPowDen)),
    float(pow(129., xPowNum / xPowDen)),
    float(pow(208., xPowNum / xPowDen))
  };
  
  for (int i = 0; i < vRAA_PtVsA.size(); i++) {
    TGraphErrors* gRAA_PtVsA_Pow = (TGraphErrors*) vRAA_PtVsA[i]->Clone(Form("gRAA_PtVsA_Pow_ptBin%d", i));
    TGraphErrors* gRAA_PtVsA_Stat_Pow = (TGraphErrors*) vRAA_PtVsA_Stat[i]->Clone(Form("gRAA_PtVsA_Stat_Pow_ptBin%d", i));
    TGraphErrors* gRAA_PtVsA_Syst_Pow = (TGraphErrors*) vRAA_PtVsA_Syst[i]->Clone(Form("gRAA_PtVsA_Syst_Pow_ptBin%d", i));
    TGraphErrors* gRAA_PtVsA_Norm_Pow = (TGraphErrors*) vRAA_PtVsA_Norm[i]->Clone(Form("gRAA_PtVsA_Norm_Pow_ptBin%d", i));
    TGraphErrors* gRAA_PtVsA_TotUncert_Pow = (TGraphErrors*) vRAA_PtVsA_TotUncert[i]->Clone(Form("gRAA_PtVsA_TotUncert_Pow_ptBin%d", i));
    for (int j = 1; j < A_POINTS; j++) {
      gRAA_PtVsA_Pow->SetPointX(j, powCenters[j]);
      gRAA_PtVsA_Pow->SetPointError(j, 0, 0);
      gRAA_PtVsA_Stat_Pow->SetPointX(j, powCenters[j]);
      gRAA_PtVsA_Stat_Pow->SetPointError(j, 0, gRAA_PtVsA_Stat_Pow->GetErrorY(j));
      gRAA_PtVsA_Syst_Pow->SetPointX(j, powCenters[j]);
      gRAA_PtVsA_Syst_Pow->SetPointError(j, xWidth, gRAA_PtVsA_Syst_Pow->GetErrorY(j));
      gRAA_PtVsA_Norm_Pow->SetPointX(j, powCenters[j]);
      gRAA_PtVsA_Norm_Pow->SetPointError(j, xWidth, gRAA_PtVsA_Norm_Pow->GetErrorY(j));
      gRAA_PtVsA_TotUncert_Pow->SetPointX(j, powCenters[j]);
      gRAA_PtVsA_TotUncert_Pow->SetPointError(j, xWidth, gRAA_PtVsA_TotUncert_Pow->GetErrorY(j));
    }
    vRAA_PtVsA_Pow.push_back(gRAA_PtVsA_Pow);
    vRAA_PtVsA_Stat_Pow.push_back(gRAA_PtVsA_Stat_Pow);
    vRAA_PtVsA_Syst_Pow.push_back(gRAA_PtVsA_Syst_Pow);
    vRAA_PtVsA_Norm_Pow.push_back(gRAA_PtVsA_Norm_Pow);
    vRAA_PtVsA_TotUncert_Pow.push_back(gRAA_PtVsA_TotUncert_Pow);
  }
  
  TFile* fout = new TFile(
    Form("ptBinned_RAAVsA_Pow%.f-%.f.root", xPowNum, xPowDen),
    "RECREATE"
  );
  fout->cd();
  for (int i = 0; i < vRAA_PtVsA_Pow.size(); i++) {
    vRAA_PtVsA_Pow[i]->Write();
    vRAA_PtVsA_Stat_Pow[i]->Write();
    vRAA_PtVsA_Syst_Pow[i]->Write();
    vRAA_PtVsA_Norm_Pow[i]->Write();
    vRAA_PtVsA_TotUncert_Pow[i]->Write();
  }
  fout->Close();
  
  vector<vector<TGraphErrors*>> vRAA_PowRescaled = {
    vRAA_PtVsA_Pow,
    vRAA_PtVsA_Stat_Pow,
    vRAA_PtVsA_Syst_Pow,
    vRAA_PtVsA_Norm_Pow,
    vRAA_PtVsA_TotUncert_Pow
  };
  return vRAA_PowRescaled;
}

void PlotRAAVsA_Pow(
  vector<TGraphErrors*> vRAA_PtVsA,
  vector<TGraphErrors*> vRAA_PtVsA_Stat,
  vector<TGraphErrors*> vRAA_PtVsA_Syst,
  vector<TGraphErrors*> vRAA_PtVsA_Norm,
  vector<TGraphErrors*> vRAA_PtVsA_TotUncert,
  vector<int> ptColors,
  vector<int> ptMarkers,
  vector<float> ptMarkerScales,
  vector<int> binsToSkip,
  TString output,
  float xPowNum,
  float xPowDen,
  float xWidth,
  float xMin,
  float xMax,
  float yMin = 0.0,
  float yMax = 1.4
) {
  vector<vector<TGraphErrors*>> vRAA_PowRescaled = RescaleXByPower(
    vRAA_PtVsA,
    vRAA_PtVsA_Stat,
    vRAA_PtVsA_Syst,
    vRAA_PtVsA_Norm,
    vRAA_PtVsA_TotUncert,
    xPowNum,
    xPowDen,
    xWidth
  );
  vector<TGraphErrors*> vRAA_PtVsA_Pow = vRAA_PowRescaled[0];
  vector<TGraphErrors*> vRAA_PtVsA_Stat_Pow = vRAA_PowRescaled[1];
  vector<TGraphErrors*> vRAA_PtVsA_Syst_Pow = vRAA_PowRescaled[2];
  vector<TGraphErrors*> vRAA_PtVsA_Norm_Pow = vRAA_PowRescaled[3];
  vector<TGraphErrors*> vRAA_PtVsA_TotUncert_Pow = vRAA_PowRescaled[4];
  
  // Style base histogram
  TH1F* hRAA_PtVsA = new TH1F(
    "hRAA_PtVsA",
    Form("R_{AA}(A, p_{T}); A^{%.f/%.f}; Charged particle R_{AA}", xPowNum, xPowDen),
    1, xMin, xMax
  );
  hRAA_PtVsA->SetMinimum(yMin);
  hRAA_PtVsA->SetMaximum(yMax);
  
  DrawRAAvsA(
    output,
    hRAA_PtVsA,
    vRAA_PtVsA_Pow,
    vRAA_PtVsA_Stat_Pow,
    vRAA_PtVsA_Syst_Pow,
    vRAA_PtVsA_Norm_Pow,
    ptColors,
    ptMarkers,
    ptMarkerScales,
    binsToSkip,
    false
  );
  
  delete hRAA_PtVsA;
}


void MakePlots_RebinRAAVsA(
) {
  // Styling
//  vector<int> ptColorsRainbow = {
//    kPink-8,
//    kViolet+2,
//    kAzure+2,
//    kTeal+2,
//    kSpring-8,
//    kOrange+2
//  };
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
  vector<TGraphErrors*> vRAA_PtVsA;
  vector<TGraphErrors*> vRAA_PtVsA_Stat;
  vector<TGraphErrors*> vRAA_PtVsA_Syst;
  vector<TGraphErrors*> vRAA_PtVsA_Norm;
  vector<TGraphErrors*> vRAA_PtVsA_TotUncert;
  for (int i=0; i<20; i++) {
    TGraphErrors* tempRAA = (TGraphErrors*) fin_graphs->Get(Form("gRAA_PtVsA_ptBin%d", i));
    TGraphErrors* tempRAA_Stat = (TGraphErrors*) fin_graphs->Get(Form("gRAA_PtVsA_Stat_ptBin%d", i));
    TGraphErrors* tempRAA_Syst = (TGraphErrors*) fin_graphs->Get(Form("gRAA_PtVsA_Syst_ptBin%d", i));
    TGraphErrors* tempRAA_Norm = (TGraphErrors*) fin_graphs->Get(Form("gRAA_PtVsA_Norm_ptBin%d", i));
    TGraphErrors* tempRAA_TotUncert = (TGraphErrors*) fin_graphs->Get(Form("gRAA_PtVsA_TotUncert_ptBin%d", i));
    if (tempRAA != nullptr && tempRAA_Stat != nullptr &&
        tempRAA_Syst != nullptr && tempRAA_Norm != nullptr &&
        tempRAA_TotUncert != nullptr) {
      vRAA_PtVsA.push_back(tempRAA);
      vRAA_PtVsA_Stat.push_back(tempRAA_Stat);
      vRAA_PtVsA_Syst.push_back(tempRAA_Syst);
      vRAA_PtVsA_Norm.push_back(tempRAA_Norm);
      vRAA_PtVsA_TotUncert.push_back(tempRAA_TotUncert);
    }
    else {
      delete tempRAA;
      delete tempRAA_Stat;
      delete tempRAA_Syst;
      delete tempRAA_Norm;
      delete tempRAA_TotUncert;
    }
  }
  
  // Combine above into RAA pT vs A plot:
  vector<int> binsToSkip;
  
  binsToSkip = {};
  PlotRAAVsA_Log(
    vRAA_PtVsA,
    vRAA_PtVsA_Stat,
    vRAA_PtVsA_Syst,
    vRAA_PtVsA_Norm,
    ptColorsRainbow,
    ptMarkersOpen,
    ptMarkerScales,
    binsToSkip,
    "ptBinned_RAAVsA_AllBins",
    0.4, 400 // xMin, xMax
  );
  PlotRAAVsA_Log(
    vRAA_PtVsA,
    vRAA_PtVsA_Stat,
    vRAA_PtVsA_Syst,
    vRAA_PtVsA_Norm,
    ptColorsRainbow,
    ptMarkersOpen,
    ptMarkerScales,
    binsToSkip,
    "ptBinned_RAAVsA_AllBins",
    0.4, 400 // xMin, xMax
  );
  PlotRAAVsA_Pow(
    vRAA_PtVsA,
    vRAA_PtVsA_Stat,
    vRAA_PtVsA_Syst,
    vRAA_PtVsA_Norm,
    vRAA_PtVsA_TotUncert,
    ptColorsRainbow,
    ptMarkersOpen,
    ptMarkerScales,
    binsToSkip,
    "ptBinned_RAAVsA_Pow1-3_AllBins",
    1, 3, // Pow_Num, Pow_Den
    0.06, // xWidth
    -2., 6.4 // xMin, xMax
  );
  
  // RAA vs A select pt bins
  binsToSkip = {0, 1, 2, 3, 4, 5, 7, 9, 11, 14};
  PlotRAAVsA_Log(
    vRAA_PtVsA,
    vRAA_PtVsA_Stat,
    vRAA_PtVsA_Syst,
    vRAA_PtVsA_Norm,
    ptColorsRainbow,
    ptMarkersOpen,
    ptMarkerScales,
    binsToSkip,
    "ptBinned_RAAVsA_SelectBins"
  );
  PlotRAAVsA_Pow(
    vRAA_PtVsA,
    vRAA_PtVsA_Stat,
    vRAA_PtVsA_Syst,
    vRAA_PtVsA_Norm,
    vRAA_PtVsA_TotUncert,
    ptColorsRainbow,
    ptMarkersOpen,
    ptMarkerScales,
    binsToSkip,
    "ptBinned_RAAVsA_Pow1-3_SelectBins",
    1, 3, // Pow_Num, Pow_Den
    0.06, // xWidth
    0.6, 6.4 // xMin, xMax
  );
  
  // Cleanup
  fin_graphs->Close();
  delete fin_graphs;
}
