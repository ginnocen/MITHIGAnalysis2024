#include <TROOT.h>
#include <vector>
#include "MITHIG_CMSStyle.h"

#define DEBUG 0
#define A_POINTS 5

using namespace std;

void PlotRAAVsPt(
  TGraphErrors* gRAAVsPt_OO,
  TGraphErrors* gRAAVsPt_OO_Stat,
  TGraphErrors* gRAAVsPt_OO_Syst,
  TGraphErrors* gRAAVsPt_OO_Norm,
  TGraphErrors* gRAAVsPt_NeNe,
  TGraphErrors* gRAAVsPt_NeNe_Stat,
  TGraphErrors* gRAAVsPt_NeNe_Syst,
  TGraphErrors* gRAAVsPt_NeNe_Norm,
  TGraphErrors* gRAAVsPt_XeXe,
  TGraphErrors* gRAAVsPt_XeXe_Stat,
  TGraphErrors* gRAAVsPt_XeXe_Syst,
  TGraphErrors* gRAAVsPt_XeXe_Norm,
  TGraphErrors* gRAAVsPt_PbPb,
  TGraphErrors* gRAAVsPt_PbPb_Stat,
  TGraphErrors* gRAAVsPt_PbPb_Syst,
  TGraphErrors* gRAAVsPt_PbPb_Norm,
  TGraphErrors* gRAAVsPt_pPb,
  TGraphErrors* gRAAVsPt_pPb_Stat,
  TGraphErrors* gRAAVsPt_pPb_Syst,
  TGraphErrors* gRAAVsPt_pPb_Norm,
  vector<int> ptColors,
  TString output,
  bool showpPb = false
) {
  vector<float> ptBins = {
     3.2,  4.0,  4.8,  5.6,  6.4,
     7.2,  9.6, 12.0, 14.4, 19.2,
    24.0, 28.8, 35.2, 48.0, 73.6,
    103.6
  };
  int nPtBins = ptBins.size();
  
  // Add norm systematics
  float OO_Norm   = 0.050;
  float NeNe_Norm = 0.0566;
  float XeXe_Norm = 0.084;
  float PbPb_Norm = 0.041;
  float pPb_Norm  = 0.050;
  float normRight = 3.0;
  float normWidth = 0.08;
  vector<float> normEdges = {
    exp(log(normRight) - normWidth * 5),
    exp(log(normRight) - normWidth * 4),
    exp(log(normRight) - normWidth * 3),
    exp(log(normRight) - normWidth * 2),
    exp(log(normRight) - normWidth),
    exp(log(normRight)),
  };
  
  // OO Marker
  gRAAVsPt_OO->SetMarkerColor(kBlack);
  gRAAVsPt_OO->SetLineColor(kBlack);
  gRAAVsPt_OO->SetFillColorAlpha(kAzure-4, 0.7);
  gRAAVsPt_OO->SetMarkerStyle(24);
  gRAAVsPt_OO->SetMarkerSize(0.83);
  gRAAVsPt_OO->SetLineWidth(1);
  gRAAVsPt_OO_Stat->SetLineColor(kBlack);
  gRAAVsPt_OO_Stat->SetLineWidth(2);
  gRAAVsPt_OO_Syst->SetLineWidth(0);
  gRAAVsPt_OO_Syst->SetFillColorAlpha(kAzure-4, 0.7);
  gRAAVsPt_OO_Syst->SetFillStyle(1001);
  TBox* gOO_Norm = new TBox(normEdges[1], 1-OO_Norm, normEdges[2], 1+OO_Norm);
  gOO_Norm->SetFillColorAlpha(kAzure-4, 0.7);
  gOO_Norm->SetLineWidth(0);
  
  gRAAVsPt_NeNe->SetMarkerColor(kViolet+2);
  gRAAVsPt_NeNe->SetLineColor(kViolet+2);
  gRAAVsPt_NeNe->SetFillColorAlpha(kViolet-4, 0.7);
  gRAAVsPt_NeNe->SetMarkerStyle(25);
  gRAAVsPt_NeNe->SetMarkerSize(0.83);
  gRAAVsPt_NeNe->SetLineWidth(1);
  gRAAVsPt_NeNe_Stat->SetLineColor(kViolet+2);
  gRAAVsPt_NeNe_Stat->SetLineWidth(2);
  gRAAVsPt_NeNe_Syst->SetLineWidth(0);
  gRAAVsPt_NeNe_Syst->SetFillColorAlpha(kViolet-4, 0.7);
  gRAAVsPt_NeNe_Syst->SetFillStyle(1001);
  TBox* gNeNe_Norm = new TBox(normEdges[2], 1-NeNe_Norm, normEdges[3], 1+NeNe_Norm);
  gNeNe_Norm->SetFillColorAlpha(kViolet-4, 0.7);
  gNeNe_Norm->SetLineWidth(0);
  
  gRAAVsPt_XeXe->SetMarkerColor(TColor::GetColor("#e42536"));
  gRAAVsPt_XeXe->SetLineColor(TColor::GetColor("#e42536"));
  gRAAVsPt_XeXe->SetFillColorAlpha(kPink-3, 0.4);
  gRAAVsPt_XeXe->SetMarkerStyle(28);
  gRAAVsPt_XeXe->SetMarkerSize(0.98);
  gRAAVsPt_XeXe->SetLineWidth(1);
  
  gRAAVsPt_XeXe_Stat->SetLineColor(TColor::GetColor("#e42536"));
  gRAAVsPt_XeXe_Stat->SetLineWidth(2);
  
  gRAAVsPt_XeXe_Syst->SetLineWidth(0);
  gRAAVsPt_XeXe_Syst->SetFillColorAlpha(kPink-3, 0.4);
  gRAAVsPt_XeXe_Syst->SetFillStyle(1001);
  TBox* gXeXe_Norm = new TBox(normEdges[3], 1-XeXe_Norm, normEdges[4], 1+XeXe_Norm);
  gXeXe_Norm->SetFillColorAlpha(kPink-3, 0.4);
  gXeXe_Norm->SetLineWidth(0);
  
  gRAAVsPt_PbPb->SetMarkerColor(kBlack); // (kOrange+1)
  gRAAVsPt_PbPb->SetLineColor(kBlack); // (kOrange+1)
  gRAAVsPt_PbPb->SetFillColorAlpha(kOrange, 0.8);
  gRAAVsPt_PbPb->SetMarkerStyle(24);
  gRAAVsPt_PbPb->SetMarkerSize(0.83);
  gRAAVsPt_PbPb->SetLineWidth(1);
  
  gRAAVsPt_PbPb_Stat->SetLineColor(kBlack); // (kOrange+1)
  gRAAVsPt_PbPb_Stat->SetLineWidth(2);
  
  gRAAVsPt_PbPb_Syst->SetLineWidth(0);
  gRAAVsPt_PbPb_Syst->SetFillColorAlpha(kOrange, 0.8);
  gRAAVsPt_PbPb_Syst->SetFillStyle(1001);
  TBox* gPbPb_Norm = new TBox(normEdges[4], 1-PbPb_Norm, normEdges[5], 1+PbPb_Norm);
  gPbPb_Norm->SetFillColorAlpha(kOrange, 0.8);
  gPbPb_Norm->SetLineWidth(0);
  
  TGraphAsymmErrors* gpPb_Norm;
  if (showpPb) {
    gRAAVsPt_pPb->SetMarkerColor(kGreen-2);
    gRAAVsPt_pPb->SetLineColor(kGreen-2);
    gRAAVsPt_pPb->SetFillColorAlpha(kGreen+2, 0.0);
    gRAAVsPt_pPb->SetMarkerStyle(24);
    gRAAVsPt_pPb->SetMarkerSize(0.83);
    gRAAVsPt_pPb->SetLineWidth(1);
    
    gRAAVsPt_pPb_Stat->SetLineColor(kGreen-2);
    gRAAVsPt_pPb_Stat->SetLineWidth(2);
    
    gRAAVsPt_pPb_Syst->SetLineWidth(1);
    gRAAVsPt_pPb_Syst->SetLineColor(kGreen-2);
    gRAAVsPt_pPb_Syst->SetFillColorAlpha(kGreen+2, 0.0);
    gRAAVsPt_pPb_Syst->SetFillStyle(1001);
    gpPb_Norm = new TGraphAsymmErrors(1);
    gpPb_Norm->SetPoint(0, 0.5 * (normEdges[0] + normEdges[1]), 1);
    gpPb_Norm->SetPointError(0,
      0.5 * (normEdges[0] + normEdges[1]) - normEdges[0],
      normEdges[1] - 0.5 * (normEdges[0] + normEdges[1]),
      pPb_Norm, pPb_Norm
    );
    gpPb_Norm->SetFillColorAlpha(kGreen-2, 0.0);
    gpPb_Norm->SetLineColor(kGreen+2);
    gpPb_Norm->SetLineWidth(1);
  }
  
  // Make canvas
  TCanvas* canvas = new TCanvas("canvas", "", 600, 600);
  TPad* pad = (TPad*) canvas->GetPad(0);
  pad->cd();
  SetTDRStyle();
  pad->SetMargin(0.16, 0.04, 0.13, 0.07);
  
  // Style and draw base histogram
  TH1F* hRAAVsPt;
  TString hRAAVsPt_labels = "R_{AA}(p_{T}); p_{T} (GeV); Charged particle R_{AA}";
  if (showpPb) hRAAVsPt = new TH1F("hRAAVsPt", hRAAVsPt_labels, 1, 1.8, 130);
  else hRAAVsPt = new TH1F("hRAAVsPt", hRAAVsPt_labels, 1, 2, 130);
  hRAAVsPt->SetMinimum(0.0);
  hRAAVsPt->SetMaximum(1.4);
  if (showpPb) hRAAVsPt->SetMaximum(1.6);
  hRAAVsPt->Draw();
  hRAAVsPt->SetTitleOffset(1.15, "x");
  hRAAVsPt->SetTitleOffset(1.5, "y");
  gStyle->SetOptStat(0);
  gPad->SetTicks(1, 1);
  hRAAVsPt->GetXaxis()->SetTickLength(0.02);
  hRAAVsPt->GetYaxis()->SetTickLength(0.02);
  hRAAVsPt->GetXaxis()->CenterTitle(true);
  hRAAVsPt->GetYaxis()->CenterTitle(true);
  
  // Make legend
//  TLegend* legend = new TLegend(0.34, 0.89 - (0.045 * 4), 0.72, 0.89);
  TLegend* legend;
  if (showpPb) legend = new TLegend(0.38, 0.77, 0.95, 0.91);
  else legend = new TLegend(0.38, 0.80, 0.95, 0.91);
  legend->SetTextFont(42);
  legend->SetTextSize(0.026);
  legend->SetFillStyle(0);
  legend->SetBorderSize(0);
  legend->SetNColumns(2);
  legend->AddEntry(gRAAVsPt_OO, "OO (5.36 TeV)", "fp");
  legend->AddEntry(gRAAVsPt_NeNe, "NeNe (5.36 TeV)", "fp");
  legend->AddEntry(gRAAVsPt_PbPb, "PbPb (5.02 TeV)", "fp");
  legend->AddEntry(gRAAVsPt_XeXe, "XeXe (5.44 TeV, 0-80%)", "fp");
  if (showpPb) legend->AddEntry(gRAAVsPt_pPb, "pPb (5.02 TeV)", "fp");
  
  // Add line at RAA = 1
  TLine* unity = new TLine(
    hRAAVsPt->GetBinLowEdge(1), 1,
    hRAAVsPt->GetBinLowEdge(hRAAVsPt->GetNbinsX()+1), 1
  );
  unity->SetLineColor(kGray);
  unity->SetLineStyle(2);
  unity->Draw();
  
  // Add norms
  gOO_Norm->Draw("same");
  gNeNe_Norm->Draw("same");
  gXeXe_Norm->Draw("same");
  gPbPb_Norm->Draw("same");
  
  // Draw species RAA
  gRAAVsPt_PbPb_Syst->Draw("E2 same");
  gRAAVsPt_PbPb_Stat->Draw("E5 same");
  gRAAVsPt_PbPb->Draw("P same");
  
  gRAAVsPt_XeXe_Syst->Draw("E2 same");
  gRAAVsPt_XeXe_Stat->Draw("E5 same");
  gRAAVsPt_XeXe->Draw("P same");
  
  gRAAVsPt_OO_Syst->Draw("E2 same");
  gRAAVsPt_OO_Stat->Draw("E5 same");
  gRAAVsPt_OO->Draw("P same");
  
  gRAAVsPt_NeNe_Syst->Draw("E2 same");
  gRAAVsPt_NeNe_Stat->Draw("E5 same");
  gRAAVsPt_NeNe->Draw("P same");
  
  if (showpPb) {
    gpPb_Norm->Draw("same s=0.01;5");
    gRAAVsPt_pPb_Syst->Draw("same s=0.01;5");
    gRAAVsPt_pPb_Stat->Draw("E5 same");
    gRAAVsPt_pPb->Draw("P same");
  }
  
  // Add header and labels
  legend->Draw();
  pad->cd();
  
  // Custom CMS header and labels
  TLatex texCmsHeader;
  float textSize = 0.042;
  texCmsHeader.SetNDC();
  texCmsHeader.SetTextAngle(0);
  texCmsHeader.SetTextColor(kBlack);
  texCmsHeader.SetTextFont(42);
  texCmsHeader.SetTextSize(textSize);
  texCmsHeader.SetTextAlign(13);
  float l = pad->GetLeftMargin();
  float t = pad->GetTopMargin();
  float r = pad->GetRightMargin();
  float b = pad->GetBottomMargin();
  texCmsHeader.DrawLatex(
    l + (1 - l - r) * 0.03,
    1 - (t + (1 - t - b) * 0.04),
    "#font[61]{#scale[1.25]{CMS}}"
  );
  texCmsHeader.DrawLatex(
    l + (1 - l - r) * 0.03,
    1 - (t + (1 - t - b) * 0.04) - 1.25 * textSize,
    "#font[52]{Preliminary}"
  );
  
  TLatex latex;
  latex.SetNDC();
  latex.SetTextAngle(0);
  latex.SetTextColor(kBlack);
  latex.SetTextFont(42);
  latex.SetTextSize(0.040);
  latex.SetTextAlign(33);
  latex.DrawLatex(
    0.9,
    0.30,
    "#font[62]{|#eta| < 1}"
  );
  latex.DrawLatex(
    0.9,
    0.30 - 0.05,
    "Data binned"
  );
  latex.DrawLatex(
    0.9,
    0.30 - 0.09,
    "for comparison"
  );
  gPad->SetLogx(1);
  pad->Update();

  // Make pdf and png
  system("mkdir -p plots_RAAvsPt;");
  canvas->SaveAs("plots_RAAvsPt/"+ output +".pdf");
  gPad->SetLogx(0);
  delete canvas;
  delete legend;
  delete hRAAVsPt;
}


void MakePlots_RebinRAAVsPt(
) {
  TFile* fRAAVsPt = TFile::Open("speciesBinned_RAAVsPt.root", "read");
  TGraphErrors* gRAAVsPt_OO         = (TGraphErrors*) fRAAVsPt->Get("gOO_RAAVsPt");
  TGraphErrors* gRAAVsPt_OO_Stat    = (TGraphErrors*) fRAAVsPt->Get("gOO_RAAVsPt_Stat");
  TGraphErrors* gRAAVsPt_OO_Syst    = (TGraphErrors*) fRAAVsPt->Get("gOO_RAAVsPt_Syst");
  TGraphErrors* gRAAVsPt_OO_Norm    = (TGraphErrors*) fRAAVsPt->Get("gOO_RAAVsPt_Norm");
  TGraphErrors* gRAAVsPt_NeNe       = (TGraphErrors*) fRAAVsPt->Get("gNeNe_RAAVsPt");
  TGraphErrors* gRAAVsPt_NeNe_Stat  = (TGraphErrors*) fRAAVsPt->Get("gNeNe_RAAVsPt_Stat");
  TGraphErrors* gRAAVsPt_NeNe_Syst  = (TGraphErrors*) fRAAVsPt->Get("gNeNe_RAAVsPt_Syst");
  TGraphErrors* gRAAVsPt_NeNe_Norm  = (TGraphErrors*) fRAAVsPt->Get("gNeNe_RAAVsPt_Norm");
  TGraphErrors* gRAAVsPt_XeXe       = (TGraphErrors*) fRAAVsPt->Get("gXeXe_RAAVsPt");
  TGraphErrors* gRAAVsPt_XeXe_Stat  = (TGraphErrors*) fRAAVsPt->Get("gXeXe_RAAVsPt_Stat");
  TGraphErrors* gRAAVsPt_XeXe_Syst  = (TGraphErrors*) fRAAVsPt->Get("gXeXe_RAAVsPt_Syst");
  TGraphErrors* gRAAVsPt_XeXe_Norm  = (TGraphErrors*) fRAAVsPt->Get("gXeXe_RAAVsPt_Norm");
  TGraphErrors* gRAAVsPt_PbPb       = (TGraphErrors*) fRAAVsPt->Get("gPbPb_RAAVsPt");
  TGraphErrors* gRAAVsPt_PbPb_Stat  = (TGraphErrors*) fRAAVsPt->Get("gPbPb_RAAVsPt_Stat");
  TGraphErrors* gRAAVsPt_PbPb_Syst  = (TGraphErrors*) fRAAVsPt->Get("gPbPb_RAAVsPt_Syst");
  TGraphErrors* gRAAVsPt_PbPb_Norm  = (TGraphErrors*) fRAAVsPt->Get("gPbPb_RAAVsPt_Norm");
  TGraphErrors* gRAAVsPt_pPb        = (TGraphErrors*) fRAAVsPt->Get("gpPb_RAAVsPt");
  TGraphErrors* gRAAVsPt_pPb_Stat   = (TGraphErrors*) fRAAVsPt->Get("gpPb_RAAVsPt_Stat");
  TGraphErrors* gRAAVsPt_pPb_Syst   = (TGraphErrors*) fRAAVsPt->Get("gpPb_RAAVsPt_Syst");
  TGraphErrors* gRAAVsPt_pPb_Norm   = (TGraphErrors*) fRAAVsPt->Get("gpPb_RAAVsPt_Norm");
  
  vector<int> SpeciesColors = {
    kP10Blue, kP10Violet, kP10Red, kP10Yellow
  };
  PlotRAAVsPt(
    gRAAVsPt_OO,   gRAAVsPt_OO_Stat,   gRAAVsPt_OO_Syst,   gRAAVsPt_OO_Norm,
    gRAAVsPt_NeNe, gRAAVsPt_NeNe_Stat, gRAAVsPt_NeNe_Syst, gRAAVsPt_NeNe_Norm,
    gRAAVsPt_XeXe, gRAAVsPt_XeXe_Stat, gRAAVsPt_XeXe_Syst, gRAAVsPt_XeXe_Norm,
    gRAAVsPt_PbPb, gRAAVsPt_PbPb_Stat, gRAAVsPt_PbPb_Syst, gRAAVsPt_PbPb_Norm,
    gRAAVsPt_pPb,  gRAAVsPt_pPb_Stat,  gRAAVsPt_pPb_Syst,  gRAAVsPt_pPb_Norm,
    SpeciesColors,
    "speciesBinned_RAAVsPt"
  );
  PlotRAAVsPt(
    gRAAVsPt_OO,   gRAAVsPt_OO_Stat,   gRAAVsPt_OO_Syst,   gRAAVsPt_OO_Norm,
    gRAAVsPt_NeNe, gRAAVsPt_NeNe_Stat, gRAAVsPt_NeNe_Syst, gRAAVsPt_NeNe_Norm,
    gRAAVsPt_XeXe, gRAAVsPt_XeXe_Stat, gRAAVsPt_XeXe_Syst, gRAAVsPt_XeXe_Norm,
    gRAAVsPt_PbPb, gRAAVsPt_PbPb_Stat, gRAAVsPt_PbPb_Syst, gRAAVsPt_PbPb_Norm,
    gRAAVsPt_pPb,  gRAAVsPt_pPb_Stat,  gRAAVsPt_pPb_Syst,  gRAAVsPt_pPb_Norm,
    SpeciesColors,
    "speciesBinned_RAAVsPt_pPb",
    true
  );
}
