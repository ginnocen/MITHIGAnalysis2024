#include <TROOT.h>
#include <vector>
#include "MITHIG_CMSStyle.h"

#define DEBUG 0
#define A_POINTS 5

using namespace std;

TF1* FitToLine(
  TGraphErrors* gRAA_PtVsNcoll_Uncert,
  TString funcName,
  float xMin,
  float xMax
) {
  TF1* fRAAvsA = new TF1(funcName, "[0]*(x-1) + [1]", xMin, xMax);
  fRAAvsA->SetParameters(-0.5 / (xMax - xMin), 1.);
  gRAA_PtVsNcoll_Uncert->Fit(fRAAvsA, "N", "" , xMin, xMax);
  return fRAAvsA;
}

void DrawRAAvsN(
  TString output,
  TH1F* hRAA_PtVsN,
  vector<TGraphErrors*> vRAA_PtVsN,
  vector<TGraphErrors*> vRAA_PtVsN_Stat,
  vector<TGraphErrors*> vRAA_PtVsN_Syst,
  vector<TGraphErrors*> vRAA_PtVsN_Norm,
  vector<TGraphErrors*> vRAA_pPb_Stat,
  vector<TGraphErrors*> vRAA_pPb_Syst,
  float protonX,
  vector<int> ptColors,
  vector<int> ptMarkers,
  vector<float> ptMarkerScales,
  vector<int> binsToSkip,
  bool doLogx,
  bool showpPb = true
) {
  int nPtBins = vRAA_PtVsN.size() - binsToSkip.size();
  float scaleLegend = 1.;
  if (nPtBins > 8) scaleLegend = 8. / nPtBins;
  else if (nPtBins > 6) scaleLegend = 6. / nPtBins;
  int firstBin = -1; // used for ion species labels
  
  // Make canvas
  TCanvas* canvas = new TCanvas("canvas", "", 600, 600);
  TPad* pad = (TPad*) canvas->GetPad(0);
  pad->cd();
  SetTDRStyle();
  pad->SetMargin(0.16, 0.04, 0.13, 0.07);
  
  // Style and draw base histogram
  hRAA_PtVsN->Draw();
  hRAA_PtVsN->SetTitleOffset(1.2, "x");
  hRAA_PtVsN->SetTitleOffset(1.5, "y");
  hRAA_PtVsN->GetXaxis()->SetTickLength(0.02);
  hRAA_PtVsN->GetYaxis()->SetTickLength(0.02);
  hRAA_PtVsN->GetXaxis()->CenterTitle(true);
  hRAA_PtVsN->GetYaxis()->CenterTitle(true);
  gStyle->SetOptStat(0);
  gPad->SetTicks(1, 1);
  pad->Update();
  
  // Make legend
  TLegend* legend;
  if (showpPb) legend = new TLegend(0.19, 0.16, 0.5, 0.16 + (0.045 * nPtBins * scaleLegend));
  else legend = new TLegend(0.19, 0.18, 0.5, 0.18 + (0.045 * nPtBins * scaleLegend));
  legend->SetTextSize(0.035 * scaleLegend);
  legend->SetFillStyle(0);
  legend->SetBorderSize(0);
  
  // Add line at RAA = 1
  TLine* unity = new TLine(
    hRAA_PtVsN->GetBinLowEdge(1), 1,
    hRAA_PtVsN->GetBinLowEdge(hRAA_PtVsN->GetNbinsX()+1), 1
  );
  unity->SetLineColor(kGray);
  unity->SetLineStyle(2);
  unity->Draw();
  
  // Add norm systematics
  float O_Norm = 0;
  float Ne_Norm = 0;
  float Xe_Norm = 0;
  float Pb_Norm = 0;
  float pPb_Norm = 0;
  for (int i = 0; i < vRAA_PtVsN_Norm.size(); i++) {
    if (std::find(binsToSkip.begin(), binsToSkip.end(), i) != binsToSkip.end()) continue;
    if (firstBin < 0) firstBin = i;
    O_Norm = TMath::Max(
      float(vRAA_PtVsN_Norm[i]->GetErrorY(1) / vRAA_PtVsN[i]->GetPointY(1)),
      O_Norm
    );
    Ne_Norm = TMath::Max(
      float(vRAA_PtVsN_Norm[i]->GetErrorY(2) / vRAA_PtVsN[i]->GetPointY(2)),
      Ne_Norm
    );
    Xe_Norm = TMath::Max(
      float(vRAA_PtVsN_Norm[i]->GetErrorY(3) / vRAA_PtVsN[i]->GetPointY(3)),
      Xe_Norm
    );
    Pb_Norm = TMath::Max(
      float(vRAA_PtVsN_Norm[i]->GetErrorY(4) / vRAA_PtVsN[i]->GetPointY(4)),
      Pb_Norm
    );
    pPb_Norm = TMath::Max(
      float(vRAA_PtVsN_Norm[i]->GetErrorY(5) / vRAA_PtVsN[i]->GetPointY(5)),
      pPb_Norm
    );
  }
  if (DEBUG) {
    cout << ">>> Summary of Norm Uncertainties:" << endl;
    cout << "  OO:   " << O_Norm << endl;
    cout << "  NeNe: " << Ne_Norm << endl;
    cout << "  XeXe: " << Xe_Norm << endl;
    cout << "  PbPb: " << Pb_Norm << endl;
    cout << "  pPb:  " << pPb_Norm << endl;
  }
  TGraphErrors* gNormSyst = new TGraphErrors(5);
  gNormSyst->SetPoint(1, vRAA_PtVsN[1]->GetPointX(1), 1.);
  gNormSyst->SetPointError(1, vRAA_PtVsN_Syst[1]->GetErrorX(1), O_Norm);
  gNormSyst->SetPoint(2, vRAA_PtVsN[1]->GetPointX(2), 1.);
  gNormSyst->SetPointError(2, vRAA_PtVsN_Syst[1]->GetErrorX(2), Ne_Norm);
  gNormSyst->SetPoint(3, vRAA_PtVsN[1]->GetPointX(3), 1.);
  gNormSyst->SetPointError(3, vRAA_PtVsN_Syst[1]->GetErrorX(3), Xe_Norm);
  gNormSyst->SetPoint(4, vRAA_PtVsN[1]->GetPointX(4), 1.);
  gNormSyst->SetPointError(4, vRAA_PtVsN_Syst[1]->GetErrorX(4), Pb_Norm);
  if (showpPb) {
    gNormSyst->SetPoint(5, vRAA_PtVsN[1]->GetPointX(5), 1.);
    gNormSyst->SetPointError(5, vRAA_pPb_Syst[1]->GetErrorX(0), pPb_Norm);
  }
  gNormSyst->SetFillColorAlpha(kGray, 0.45);
  gNormSyst->SetLineWidth(0);
  gNormSyst->Draw("E2 same");
  
  // Add point for proton
  TGraph* proton = new TGraph(1);
  proton->SetPoint(0, protonX, 1);
  proton->SetMarkerStyle(20);
  proton->SetMarkerColor(kBlack);
  proton->Draw("P same");
  
  // Add species labels
  TLatex Alatex;
  Alatex.SetTextSize(0.032);
  Alatex.SetTextAlign(23);  // Center align to top
  Alatex.DrawLatex(
    protonX,
    1 - 0.03,
    "pp");
  Alatex.SetTextAlign(33);
  Alatex.DrawLatex(
    vRAA_PtVsN[firstBin]->GetPointX(1) + vRAA_PtVsN_Syst[firstBin]->GetErrorX(1),
    vRAA_PtVsN[firstBin]->GetPointY(1) - vRAA_PtVsN_Syst[firstBin]->GetErrorY(1) - 0.03,
    "OO");
  Alatex.SetTextAlign(13);
  Alatex.DrawLatex(
    vRAA_PtVsN[firstBin]->GetPointX(2) - vRAA_PtVsN_Syst[firstBin]->GetErrorX(2),
    vRAA_PtVsN[firstBin]->GetPointY(2) - vRAA_PtVsN_Syst[firstBin]->GetErrorY(2) - 0.03,
    "NeNe");
  Alatex.SetTextAlign(23);
  Alatex.DrawLatex(
    vRAA_PtVsN[firstBin]->GetPointX(3),
    vRAA_PtVsN[firstBin]->GetPointY(3) - vRAA_PtVsN_Syst[firstBin]->GetErrorY(3) - 0.03,
    "XeXe");
  Alatex.DrawLatex(
    vRAA_PtVsN[firstBin]->GetPointX(4),
    vRAA_PtVsN[firstBin]->GetPointY(4) - vRAA_PtVsN_Syst[firstBin]->GetErrorY(4) - 0.05,
    "PbPb");
  if (showpPb) {
    Alatex.DrawLatex(
      vRAA_PtVsN[firstBin]->GetPointX(5),
      vRAA_PtVsN[firstBin]->GetPointY(5) - vRAA_pPb_Syst[firstBin]->GetErrorY(0) - 0.03,
      "pPb");
  }
  
  // Draw Systematic Uncertainties
  int iStyle = 0;
  for (int i = 0; i < vRAA_PtVsN_Syst.size(); i++) {
    if(std::find(binsToSkip.begin(), binsToSkip.end(), i) != binsToSkip.end()) continue;
    vRAA_PtVsN_Syst[i]->SetFillColorAlpha(ptColors[iStyle % ptColors.size()], 0.45);
    vRAA_PtVsN_Syst[i]->SetLineWidth(0);
    vRAA_PtVsN_Syst[i]->Draw("E2 same");
    if (showpPb) {
      vRAA_pPb_Syst[i]->SetLineColor(ptColors[iStyle % ptColors.size()]);
      vRAA_pPb_Syst[i]->SetFillColorAlpha(ptColors[iStyle % ptColors.size()], 0.);
      vRAA_pPb_Syst[i]->SetLineWidth(1);
      vRAA_pPb_Syst[i]->Draw("same s=0.01;5");
    }
    iStyle++;
  }
  // Draw Statistical Uncertainties
  iStyle = 0;
  for (int i = 0; i < vRAA_PtVsN_Stat.size(); i++) {
    if(std::find(binsToSkip.begin(), binsToSkip.end(), i) != binsToSkip.end()) continue;
    vRAA_PtVsN_Stat[i]->SetLineColor(ptColors[iStyle % ptColors.size()]);
    vRAA_PtVsN_Stat[i]->SetLineWidth(2);
    vRAA_PtVsN_Stat[i]->Draw("E5 same");
    if (showpPb) {
      vRAA_pPb_Stat[i]->SetLineColor(ptColors[iStyle % ptColors.size()]);
      vRAA_pPb_Stat[i]->SetLineWidth(2);
      vRAA_pPb_Stat[i]->Draw("E5 same");
    }
    iStyle++;
  }
  // Draw Points
  vector<TF1*> vFits;
  iStyle = 0;
  for (int i = 0; i < vRAA_PtVsN.size(); i++) {
    if(std::find(binsToSkip.begin(), binsToSkip.end(), i) != binsToSkip.end()) continue;
    vRAA_PtVsN[i]->SetMarkerColor(ptColors[iStyle % ptColors.size()]);
    vRAA_PtVsN[i]->SetLineColor(ptColors[iStyle % ptColors.size()]);
    vRAA_PtVsN[i]->SetFillColorAlpha(ptColors[iStyle % ptColors.size()], 0.45);
    vRAA_PtVsN[i]->SetLineWidth(1);
    vRAA_PtVsN[i]->SetMarkerStyle(ptMarkers[iStyle % ptMarkers.size()]);
    vRAA_PtVsN[i]->SetMarkerSize(0.8 * ptMarkerScales[iStyle % ptMarkers.size()]);
    vRAA_PtVsN[i]->Draw("P same");
    legend->AddEntry(
      vRAA_PtVsN[i],
      vRAA_PtVsN[i]->GetTitle(),
      "pf"
    );
    iStyle++;
  }
  if (doLogx || nPtBins < 7) legend->Draw();
  // Add header and labels
  pad->cd();
  AddCMSHeader(pad, "Preliminary", false);
  float labelsY = 0.88;
  if (showpPb) labelsY = 0.90;
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
    labelsY
  );
  vector<TString> plotLabels2;
  if (showpPb) {
    plotLabels2 = {
      "XeXe (5.44 TeV, 0-80%)",
      "PbPb (5.02 TeV, 0-100%)",
      "pPb (5.02 TeV, 0-100%)"
    };
  }
  else {
    plotLabels2 = {
      "XeXe (5.44 TeV, 0-80%)",
      "PbPb (5.02 TeV, 0-100%)",
      " "
    };
  }
  AddPlotLabels(
    pad,
    plotLabels2,
    0.034,
    plotTextOffset,
    0.57,
    labelsY
  );
  vector<TString> plotLabels3 = {
    "|#eta| < 1",
  };
  float etaLabelY = 0.22;
  if (doLogx || nPtBins < 7) {
    etaLabelY = (0.045 * nPtBins * scaleLegend) + 0.21;
    if (!showpPb) etaLabelY = (0.045 * nPtBins * scaleLegend) + 0.23;
  }
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
  system("mkdir -p plots_RAAvsN;");
  canvas->SaveAs("plots_RAAvsN/"+ output +".pdf");
  delete canvas;
  delete legend;
}

void PlotRAAVsNcoll_Log(
  vector<TGraphErrors*> vRAA_PtVsN,
  vector<TGraphErrors*> vRAA_PtVsN_Stat,
  vector<TGraphErrors*> vRAA_PtVsN_Syst,
  vector<TGraphErrors*> vRAA_PtVsN_Norm,
  vector<int> ptColors,
  vector<int> ptMarkers,
  vector<float> ptMarkerScales,
  vector<int> binsToSkip = {},
  TString output = "ptBinned_RAAVsNcoll",
  bool showpPb = true,
  float yMin = 0.0,
  float yMax = 1.6
) {
  vector<TGraphErrors*> vRAA_PtVsNcoll;
  vector<TGraphErrors*> vRAA_PtVsNcoll_Stat;
  vector<TGraphErrors*> vRAA_PtVsNcoll_Syst;
  vector<TGraphErrors*> vRAA_PtVsNcoll_Norm;
  for (int i = 0; i < vRAA_PtVsN.size(); i++) {
    TGraphErrors* gRAA_PtVsN = (TGraphErrors*) vRAA_PtVsN[i]->Clone();
    TGraphErrors* gRAA_PtVsN_Stat = (TGraphErrors*) vRAA_PtVsN_Stat[i]->Clone();
    TGraphErrors* gRAA_PtVsN_Syst = (TGraphErrors*) vRAA_PtVsN_Syst[i]->Clone();
    TGraphErrors* gRAA_PtVsN_Norm = (TGraphErrors*) vRAA_PtVsN_Norm[i]->Clone();
    vRAA_PtVsNcoll.push_back(gRAA_PtVsN);
    vRAA_PtVsNcoll_Stat.push_back(gRAA_PtVsN_Stat);
    vRAA_PtVsNcoll_Syst.push_back(gRAA_PtVsN_Syst);
    vRAA_PtVsNcoll_Norm.push_back(gRAA_PtVsN_Norm);
  }

  vector<float> xErrors;
  float logWidth = 0.07;
  float O_Ncoll = vRAA_PtVsNcoll[0]->GetPointX(1);
  float Ne_Ncoll = vRAA_PtVsNcoll[0]->GetPointX(2);
  float Xe_Ncoll = vRAA_PtVsNcoll[0]->GetPointX(3);
  float Pb_Ncoll = vRAA_PtVsNcoll[0]->GetPointX(4);
  float pPb_Ncoll = vRAA_PtVsNcoll[0]->GetPointX(5);
  xErrors = {
    (float) exp(logWidth) - 1,
    (float) exp(log(O_Ncoll)   + logWidth) - O_Ncoll,
    (float) exp(log(Ne_Ncoll)  + logWidth) - Ne_Ncoll,
    (float) exp(log(Xe_Ncoll)  + logWidth) - Xe_Ncoll,
    (float) exp(log(Pb_Ncoll)  + logWidth) - Pb_Ncoll,
    (float) exp(log(pPb_Ncoll) + logWidth) - pPb_Ncoll
  };
  
  // Update Stat and Syst error bars
  vector<TGraphErrors*> vRAA_pPb_Stat;
  vector<TGraphErrors*> vRAA_pPb_Syst;
  int iStyle = 0;
  for (int i = 0; i < vRAA_PtVsNcoll.size(); i++) {
    vRAA_PtVsNcoll_Stat[i]->SetPointError(1, 0, vRAA_PtVsNcoll_Stat[i]->GetErrorY(1));
    vRAA_PtVsNcoll_Stat[i]->SetPointError(2, 0, vRAA_PtVsNcoll_Stat[i]->GetErrorY(2));
    vRAA_PtVsNcoll_Stat[i]->SetPointError(3, 0, vRAA_PtVsNcoll_Stat[i]->GetErrorY(3));
    vRAA_PtVsNcoll_Stat[i]->SetPointError(4, 0, vRAA_PtVsNcoll_Stat[i]->GetErrorY(4));
    vRAA_PtVsNcoll_Syst[i]->SetPointError(1, xErrors[1], vRAA_PtVsNcoll_Syst[i]->GetErrorY(1));
    vRAA_PtVsNcoll_Syst[i]->SetPointError(2, xErrors[2], vRAA_PtVsNcoll_Syst[i]->GetErrorY(2));
    vRAA_PtVsNcoll_Syst[i]->SetPointError(3, xErrors[3], vRAA_PtVsNcoll_Syst[i]->GetErrorY(3));
    vRAA_PtVsNcoll_Syst[i]->SetPointError(4, xErrors[4], vRAA_PtVsNcoll_Syst[i]->GetErrorY(4));
    if (showpPb) {
      TGraphErrors* gRAA_pPb_Stat = new TGraphErrors(1);
      TGraphErrors* gRAA_pPb_Syst = new TGraphErrors(1);
      gRAA_pPb_Stat->SetPoint(0, vRAA_PtVsNcoll_Stat[i]->GetPointX(5), vRAA_PtVsNcoll_Stat[i]->GetPointY(5));
      gRAA_pPb_Stat->SetPointError(0, 0, vRAA_PtVsNcoll_Stat[i]->GetErrorY(5));
      gRAA_pPb_Syst->SetPoint(0, vRAA_PtVsNcoll_Syst[i]->GetPointX(5), vRAA_PtVsNcoll_Syst[i]->GetPointY(5));
      gRAA_pPb_Syst->SetPointError(0, xErrors[5], vRAA_PtVsNcoll_Syst[i]->GetErrorY(5));
      vRAA_pPb_Stat.push_back(gRAA_pPb_Stat);
      vRAA_pPb_Syst.push_back(gRAA_pPb_Syst);
    }
    else vRAA_PtVsNcoll[i]->RemovePoint(5);
    vRAA_PtVsNcoll_Stat[i]->RemovePoint(5);
    vRAA_PtVsNcoll_Syst[i]->RemovePoint(5);
  }
  
  // Style base histogram
  TH1F* hRAA_PtVsNcoll = new TH1F(
    "hRAA_PtVsNcoll",
    "R_{AA}(N_{coll}, p_{T}); Binary collisions N_{coll}; Charged particle R_{AA}",
    1, 0.6, 700
  );
  hRAA_PtVsNcoll->SetMinimum(yMin);
  hRAA_PtVsNcoll->SetMaximum(yMax);
  
  DrawRAAvsN(
    output,
    hRAA_PtVsNcoll,
    vRAA_PtVsNcoll,
    vRAA_PtVsNcoll_Stat,
    vRAA_PtVsNcoll_Syst,
    vRAA_PtVsNcoll_Norm,
    vRAA_pPb_Stat,
    vRAA_pPb_Syst,
    1,
    ptColors,
    ptMarkers,
    ptMarkerScales,
    binsToSkip,
    true,
    showpPb
  );
  
  delete hRAA_PtVsNcoll;
}

void PlotRAAVsNpart_Log(
  vector<TGraphErrors*> vRAA_PtVsN,
  vector<TGraphErrors*> vRAA_PtVsN_Stat,
  vector<TGraphErrors*> vRAA_PtVsN_Syst,
  vector<TGraphErrors*> vRAA_PtVsN_Norm,
  vector<int> ptColors,
  vector<int> ptMarkers,
  vector<float> ptMarkerScales,
  vector<int> binsToSkip = {},
  TString output = "ptBinned_RAAVsNpart",
  bool showpPb = true,
  float yMin = 0.0,
  float yMax = 1.6
) {
  vector<TGraphErrors*> vRAA_PtVsNpart;
  vector<TGraphErrors*> vRAA_PtVsNpart_Stat;
  vector<TGraphErrors*> vRAA_PtVsNpart_Syst;
  vector<TGraphErrors*> vRAA_PtVsNpart_Norm;
  for (int i = 0; i < vRAA_PtVsN.size(); i++) {
    TGraphErrors* gRAA_PtVsN = (TGraphErrors*) vRAA_PtVsN[i]->Clone();
    TGraphErrors* gRAA_PtVsN_Stat = (TGraphErrors*) vRAA_PtVsN_Stat[i]->Clone();
    TGraphErrors* gRAA_PtVsN_Syst = (TGraphErrors*) vRAA_PtVsN_Syst[i]->Clone();
    TGraphErrors* gRAA_PtVsN_Norm = (TGraphErrors*) vRAA_PtVsN_Norm[i]->Clone();
    vRAA_PtVsNpart.push_back(gRAA_PtVsN);
    vRAA_PtVsNpart_Stat.push_back(gRAA_PtVsN_Stat);
    vRAA_PtVsNpart_Syst.push_back(gRAA_PtVsN_Syst);
    vRAA_PtVsNpart_Norm.push_back(gRAA_PtVsN_Norm);
  }
  vector<float> xErrors;
  float logWidth = 0.05;
  float O_Npart = vRAA_PtVsNpart[0]->GetPointX(1);
  float Ne_Npart = vRAA_PtVsNpart[0]->GetPointX(2);
  float Xe_Npart = vRAA_PtVsNpart[0]->GetPointX(3);
  float Pb_Npart = vRAA_PtVsNpart[0]->GetPointX(4);
  float pPb_Npart = vRAA_PtVsNpart[0]->GetPointX(5);
  xErrors = {
    (float) exp(logWidth) - 1,
    (float) exp(log(O_Npart)   + logWidth) - O_Npart,
    (float) exp(log(Ne_Npart)  + logWidth) - Ne_Npart,
    (float) exp(log(Xe_Npart)  + logWidth) - Xe_Npart,
    (float) exp(log(Pb_Npart)  + logWidth) - Pb_Npart,
    (float) exp(log(pPb_Npart) + logWidth) - pPb_Npart
  };
  
  // Update Stat and Syst error bars
  vector<TGraphErrors*> vRAA_pPb_Stat;
  vector<TGraphErrors*> vRAA_pPb_Syst;
  int iStyle = 0;
  for (int i = 0; i < vRAA_PtVsNpart.size(); i++) {
    vRAA_PtVsNpart_Stat[i]->SetPointError(1, 0, vRAA_PtVsNpart_Stat[i]->GetErrorY(1));
    vRAA_PtVsNpart_Stat[i]->SetPointError(2, 0, vRAA_PtVsNpart_Stat[i]->GetErrorY(2));
    vRAA_PtVsNpart_Stat[i]->SetPointError(3, 0, vRAA_PtVsNpart_Stat[i]->GetErrorY(3));
    vRAA_PtVsNpart_Stat[i]->SetPointError(4, 0, vRAA_PtVsNpart_Stat[i]->GetErrorY(4));
    vRAA_PtVsNpart_Stat[i]->SetPointError(5, 0, vRAA_PtVsNpart_Stat[i]->GetErrorY(5));
    vRAA_PtVsNpart_Syst[i]->SetPointError(1, xErrors[1], vRAA_PtVsNpart_Syst[i]->GetErrorY(1));
    vRAA_PtVsNpart_Syst[i]->SetPointError(2, xErrors[2], vRAA_PtVsNpart_Syst[i]->GetErrorY(2));
    vRAA_PtVsNpart_Syst[i]->SetPointError(3, xErrors[3], vRAA_PtVsNpart_Syst[i]->GetErrorY(3));
    vRAA_PtVsNpart_Syst[i]->SetPointError(4, xErrors[4], vRAA_PtVsNpart_Syst[i]->GetErrorY(4));
    vRAA_PtVsNpart_Syst[i]->SetPointError(5, xErrors[5], vRAA_PtVsNpart_Syst[i]->GetErrorY(5));
    if (showpPb) {
      TGraphErrors* gRAA_pPb_Stat = new TGraphErrors(1);
      TGraphErrors* gRAA_pPb_Syst = new TGraphErrors(1);
      gRAA_pPb_Stat->SetPoint(0, vRAA_PtVsNpart_Stat[i]->GetPointX(5), vRAA_PtVsNpart_Stat[i]->GetPointY(5));
      gRAA_pPb_Stat->SetPointError(0, 0, vRAA_PtVsNpart_Stat[i]->GetErrorY(5));
      gRAA_pPb_Syst->SetPoint(0, vRAA_PtVsNpart_Syst[i]->GetPointX(5), vRAA_PtVsNpart_Syst[i]->GetPointY(5));
      gRAA_pPb_Syst->SetPointError(0, xErrors[5], vRAA_PtVsNpart_Syst[i]->GetErrorY(5));
      vRAA_pPb_Stat.push_back(gRAA_pPb_Stat);
      vRAA_pPb_Syst.push_back(gRAA_pPb_Syst);
    }
    else vRAA_PtVsNpart[i]->RemovePoint(5);
    vRAA_PtVsNpart_Stat[i]->RemovePoint(5);
    vRAA_PtVsNpart_Syst[i]->RemovePoint(5);
  }
  
  // Style base histogram
  TH1F* hRAA_PtVsNpart = new TH1F(
    "hRAA_PtVsNpart",
    "R_{AA}(N_{part}, p_{T}); Participant nucleons N_{part}; Charged particle R_{AA}",
    1, 1., 250
  );
  hRAA_PtVsNpart->SetMinimum(yMin);
  hRAA_PtVsNpart->SetMaximum(yMax);
  
  DrawRAAvsN(
    output,
    hRAA_PtVsNpart,
    vRAA_PtVsNpart,
    vRAA_PtVsNpart_Stat,
    vRAA_PtVsNpart_Syst,
    vRAA_PtVsNpart_Norm,
    vRAA_pPb_Stat,
    vRAA_pPb_Syst,
    2,
    ptColors,
    ptMarkers,
    ptMarkerScales,
    binsToSkip,
    true,
    showpPb
  );
  
  delete hRAA_PtVsNpart;
}

void MakePlots_RebinRAAVsN(
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
  TString finpathNcoll = "ptBinned_RAAVsNcoll.root";
  TFile* fin_Ncoll = new TFile(finpathNcoll, "READ");
  vector<TGraphErrors*> vRAA_PtVsNcoll;
  vector<TGraphErrors*> vRAA_PtVsNcoll_Stat;
  vector<TGraphErrors*> vRAA_PtVsNcoll_Syst;
  vector<TGraphErrors*> vRAA_PtVsNcoll_Norm;
  vector<TGraphErrors*> vRAA_PtVsNcoll_TotUncert;
  for (int i=0; i<20; i++) {
    TGraphErrors* tempRAA = (TGraphErrors*) fin_Ncoll->Get(Form("gRAA_PtVsNcoll_ptBin%d", i));
    TGraphErrors* tempRAA_Stat = (TGraphErrors*) fin_Ncoll->Get(Form("gRAA_PtVsNcoll_Stat_ptBin%d", i));
    TGraphErrors* tempRAA_Syst = (TGraphErrors*) fin_Ncoll->Get(Form("gRAA_PtVsNcoll_Syst_ptBin%d", i));
    TGraphErrors* tempRAA_Norm = (TGraphErrors*) fin_Ncoll->Get(Form("gRAA_PtVsNcoll_Norm_ptBin%d", i));
    TGraphErrors* tempRAA_TotUncert = (TGraphErrors*) fin_Ncoll->Get(Form("gRAA_PtVsNcoll_TotUncert_ptBin%d", i));
    if (tempRAA != nullptr && tempRAA_Stat != nullptr &&
        tempRAA_Syst != nullptr && tempRAA_Norm != nullptr &&
        tempRAA_TotUncert != nullptr) {
      vRAA_PtVsNcoll.push_back(tempRAA);
      vRAA_PtVsNcoll_Stat.push_back(tempRAA_Stat);
      vRAA_PtVsNcoll_Syst.push_back(tempRAA_Syst);
      vRAA_PtVsNcoll_Norm.push_back(tempRAA_Norm);
      vRAA_PtVsNcoll_TotUncert.push_back(tempRAA_TotUncert);
    }
    else {
      delete tempRAA;
      delete tempRAA_Stat;
      delete tempRAA_Syst;
      delete tempRAA_Norm;
      delete tempRAA_TotUncert;
    }
  }
  
  // Get TGraphs from root file
  TString finpathNpart = "ptBinned_RAAVsNpart.root";
  TFile* fin_Npart = new TFile(finpathNpart, "READ");
  vector<TGraphErrors*> vRAA_PtVsNpart;
  vector<TGraphErrors*> vRAA_PtVsNpart_Stat;
  vector<TGraphErrors*> vRAA_PtVsNpart_Syst;
  vector<TGraphErrors*> vRAA_PtVsNpart_Norm;
  vector<TGraphErrors*> vRAA_PtVsNpart_TotUncert;
  for (int i=0; i<20; i++) {
    TGraphErrors* tempRAA = (TGraphErrors*) fin_Npart->Get(Form("gRAA_PtVsNpart_ptBin%d", i));
    TGraphErrors* tempRAA_Stat = (TGraphErrors*) fin_Npart->Get(Form("gRAA_PtVsNpart_Stat_ptBin%d", i));
    TGraphErrors* tempRAA_Syst = (TGraphErrors*) fin_Npart->Get(Form("gRAA_PtVsNpart_Syst_ptBin%d", i));
    TGraphErrors* tempRAA_Norm = (TGraphErrors*) fin_Npart->Get(Form("gRAA_PtVsNpart_Norm_ptBin%d", i));
    TGraphErrors* tempRAA_TotUncert = (TGraphErrors*) fin_Npart->Get(Form("gRAA_PtVsNpart_TotUncert_ptBin%d", i));
    if (tempRAA != nullptr && tempRAA_Stat != nullptr &&
        tempRAA_Syst != nullptr && tempRAA_Norm != nullptr &&
        tempRAA_TotUncert != nullptr) {
      vRAA_PtVsNpart.push_back(tempRAA);
      vRAA_PtVsNpart_Stat.push_back(tempRAA_Stat);
      vRAA_PtVsNpart_Syst.push_back(tempRAA_Syst);
      vRAA_PtVsNpart_Norm.push_back(tempRAA_Norm);
      vRAA_PtVsNpart_TotUncert.push_back(tempRAA_TotUncert);
    }
    else {
      delete tempRAA;
      delete tempRAA_Stat;
      delete tempRAA_Syst;
      delete tempRAA_Norm;
      delete tempRAA_TotUncert;
    }
  }
  
  // Combine above into RAA pT vs N_coll plot:
  vector<int> binsToSkip;
  
  binsToSkip = {};
  PlotRAAVsNcoll_Log(
    vRAA_PtVsNcoll,
    vRAA_PtVsNcoll_Stat,
    vRAA_PtVsNcoll_Syst,
    vRAA_PtVsNcoll_Norm,
    ptColorsRainbow,
    ptMarkersOpen,
    ptMarkerScales,
    binsToSkip,
    "ptBinned_RAAVsNcoll_AllBins"
  );
  PlotRAAVsNcoll_Log(
    vRAA_PtVsNcoll,
    vRAA_PtVsNcoll_Stat,
    vRAA_PtVsNcoll_Syst,
    vRAA_PtVsNcoll_Norm,
    ptColorsRainbow,
    ptMarkersOpen,
    ptMarkerScales,
    binsToSkip,
    "ptBinned_RAAVsNcoll_AllBins"
  );
  PlotRAAVsNpart_Log(
    vRAA_PtVsNpart,
    vRAA_PtVsNpart_Stat,
    vRAA_PtVsNpart_Syst,
    vRAA_PtVsNpart_Norm,
    ptColorsRainbow,
    ptMarkersOpen,
    ptMarkerScales,
    binsToSkip,
    "ptBinned_RAAVsNpart_AllBins"
  );
  
  // RAA vs A select pt bins
  binsToSkip = {0, 1, 2, 3, 4, 5, 7, 9, 11, 14};
  PlotRAAVsNcoll_Log(
    vRAA_PtVsNcoll,
    vRAA_PtVsNcoll_Stat,
    vRAA_PtVsNcoll_Syst,
    vRAA_PtVsNcoll_Norm,
    ptColorsRainbow,
    ptMarkersOpen,
    ptMarkerScales,
    binsToSkip,
    "ptBinned_RAAVsNcoll_SelectBins_pPb"
  );
  PlotRAAVsNcoll_Log(
    vRAA_PtVsNcoll,
    vRAA_PtVsNcoll_Stat,
    vRAA_PtVsNcoll_Syst,
    vRAA_PtVsNcoll_Norm,
    ptColorsRainbow,
    ptMarkersOpen,
    ptMarkerScales,
    binsToSkip,
    "ptBinned_RAAVsNcoll_SelectBins",
    false,
    0., 1.4
  );
  PlotRAAVsNpart_Log(
    vRAA_PtVsNpart,
    vRAA_PtVsNpart_Stat,
    vRAA_PtVsNpart_Syst,
    vRAA_PtVsNpart_Norm,
    ptColorsRainbow,
    ptMarkersOpen,
    ptMarkerScales,
    binsToSkip,
    "ptBinned_RAAVsNpart_SelectBins_pPb"
  );
  PlotRAAVsNpart_Log(
    vRAA_PtVsNpart,
    vRAA_PtVsNpart_Stat,
    vRAA_PtVsNpart_Syst,
    vRAA_PtVsNpart_Norm,
    ptColorsRainbow,
    ptMarkersOpen,
    ptMarkerScales,
    binsToSkip,
    "ptBinned_RAAVsNpart_SelectBins",
    false,
    0., 1.4
  );
  
  // Cleanup
  fin_Ncoll->Close();
  fin_Npart->Close();
  delete fin_Ncoll;
  delete fin_Npart;
}
