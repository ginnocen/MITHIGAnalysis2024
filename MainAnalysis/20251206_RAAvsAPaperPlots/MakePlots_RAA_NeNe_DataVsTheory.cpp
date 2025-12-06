#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TColor.h"
#include "TLine.h"
#include "TGraphMultiErrors.h"
#include "TBox.h"

#define NORM_UNCERT_OO 0.05
#define NORM_UNCERT_NENE 0.0566

void PlotRAA_DataVsTheory(
  TString plotName,
  TGraphAsymmErrors* gOO,
  TGraphAsymmErrors* gOOSyst,
  TGraphAsymmErrors* gOOTheory,
  TString OOLegend,
  TGraphAsymmErrors* gNeNe,
  TGraphAsymmErrors* gNeNeSyst,
  TGraphAsymmErrors* gNeNeTheory,
  TString NeNeLegend,
  int theoryLineStyle = 1,
  float yMin = 0.,
  float yMax = 1.6,
  float xMin = 0.,
  float xMax = 150,
  int colorOOTheory = kAzure+8,
  int colorOOTheorySyst = kAzure+8,
  float alphaOOTheorySyst = 0.4,
  int colorNeNeTheory = kPink+9,
  int colorNeNeTheorySyst = kPink+9,
  float alphaNeNeTheorySyst = 0.4
) {
  TCanvas* canvas = new TCanvas("canvas","",800,800);
  canvas->SetBorderSize(0);
  canvas->SetLineWidth(0);
  canvas->SetLeftMargin(0.15);
  canvas->SetTopMargin(0.07);
  canvas->SetBorderSize(0);
  canvas->SetBottomMargin(0.15);
  canvas->SetLogx();
  
  TLegend* specLeg;
//  specLeg = new TLegend(0.39,0.17,0.78,0.36);
//  specLeg = new TLegend(0.17,0.17,0.78,0.38);
  specLeg = new TLegend(0.17,0.17,0.78,0.35);
  specLeg->SetTextFont(42);
  specLeg->SetTextSize(0.045);
  specLeg->SetFillStyle(0);
  specLeg->SetBorderSize(0);
 
  specLeg->AddEntry(gOO, "OO data (HIN-25-008)", "p f" );
  specLeg->AddEntry(gNeNeSyst, "NeNe data", "p f");
    
  TLatex *eta = new TLatex(0.26,0.7,"|#eta| < 1");
  eta->SetTextFont(42);
  eta->SetTextSize(0.05);
  eta->SetNDC(true);
  
  setTDRStyle();
  gStyle->SetOptStat(0);
  gStyle->SetLegendBorderSize(0);
  gStyle->SetStatBorderSize(0);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetPadTickY(1);
  gStyle->SetPadTickX(1);
  
  TH1D * hRAAFrame = new TH1D(
    "hRAAFrame",
    "Charged particle R_{AA}; p_{T} (GeV)",
    3, 2, 150
  );
  hRAAFrame->GetYaxis()->SetTitleOffset(1.4);
  hRAAFrame->GetYaxis()->SetTitleSize(0.045);
  hRAAFrame->GetYaxis()->SetLabelSize(0.04);
  hRAAFrame->GetYaxis()->CenterTitle();
  hRAAFrame->GetYaxis()->SetLabelOffset(0.004);
  hRAAFrame->GetXaxis()->SetTitleFont(42);
  hRAAFrame->GetXaxis()->SetTitleSize(0.045);
  hRAAFrame->GetXaxis()->SetLabelSize(0.04);
  hRAAFrame->GetXaxis()->SetTitleOffset(1.2);
  hRAAFrame->GetXaxis()->CenterTitle();
  hRAAFrame->GetYaxis()->SetRangeUser(0.3,1.1);
  hRAAFrame->GetXaxis()->SetRangeUser(0.5,150);
  hRAAFrame->Draw();
  
  TBox* bNormOO = new TBox(110, 1 - NORM_UNCERT_OO, 120, 1 + NORM_UNCERT_OO);
  bNormOO->SetFillStyle(1001);
  bNormOO->SetFillColorAlpha(colorOOSyst, alphaOOSyst);
  bNormOO->Draw("same");
  
  TBox* bNormNeNe = new TBox(120, 1 - NORM_UNCERT_NENE, 130, 1 + NORM_UNCERT_NENE);
  bNormNeNe->SetFillStyle(1001);
  bNormNeNe->SetFillColorAlpha(colorNeNeSyst, alphaNeNeSyst);
  bNormNeNe->Draw("same");
  
  TLine* l = new TLine(2, 1, 150, 1);
  l->SetLineStyle(2);
  l->Draw("same");
  
  gOO->SetMarkerStyle(20);
  gOO->SetMarkerSize(1.3);
  gOO->SetMarkerColor(kBlack);
  gOO->SetLineColor(kBlack);
  gOO->Print("All");
  gOO->GetAttLine(0)->SetLineWidth(2);
  gOO->GetAttLine(1)->SetLineColor(TColor::GetColor("#5790fc"));
  gOO->GetAttFill(1)->SetFillColor(TColor::GetColor("#5790fc"));
  gOO->GetAttFill(1)->SetFillStyle(1001);
  
  gNeNe->SetMarkerStyle(21);
  gNeNe->SetMarkerSize(1.3);
  gNeNe->SetLineWidth(2);
  gNeNe->SetLineColor(kViolet+2);
  gNeNe->SetMarkerColor(kViolet+2);
  gNeNe->GetXaxis()->SetRangeUser(3,100);

  gNeNeSyst->SetFillColorAlpha(kViolet-9, 0.75);
  gNeNeSyst->SetLineColor(kBlack);
  gNeNeSyst->SetMarkerSize(1.3);
  gNeNeSyst->SetMarkerStyle(21);
  gNeNeSyst->SetMarkerColor(kViolet+2);
  gNeNeSyst->SetMarkerColor(kViolet+2);
  gNeNeSyst->GetXaxis()->SetRangeUser(3,100);
  
  gOOCUJET->SetLineColor(colorOOTheory);
  gOOCUJET->SetLineWidth(4);
  gOOCUJET->SetLineStyle(theoryLineStyle);
  gOOCUJET->SetFillColorAlpha(colorOOTheorySyst, alphaOOTheorySyst);
  
  gNeNeCUJET->SetLineColor(colorNeNeTheory); // purple/red
  gNeNeCUJET->SetLineWidth(4);
  gNeNeCUJET->SetLineStyle(theoryLineStyle);
  gNeNeCUJET->SetFillColorAlpha(colorNeNeTheorySyst, alphaNeNeTheorySyst);
  
  gOO->Draw("PZs s=0.01 same;2 s=1");
  gOO->Draw("PZs s=0.01 same;X");
  gNeNe->Draw("ep same");
  gNeNeSyst->Draw("e2 same");
  
  raaOOSyst->Draw("FE2 same");
  raaNeNeSyst->Draw("FE2 same");
  
  raaOOTheory->Draw("LE3 same");
  raaNeNeTheory->Draw("LE3 same");
  
  raaOO->Draw("PE5 same");
  raaNeNeSyst->Draw("PE5 same");
  
  specLeg->Draw("same");
  eta->Draw("same");
  
  CMS_lumi(canvas, 0,11);
  canvas->RedrawAxis();
}

vector<TGraphAsymmErrors*> BuildDataRAAGraphs(
  TString filepath,
  TString raaName,
  TString raaSystName,
  TString species
) {
  TFile* f = TFile::Open(filepath,"read");
  TH1D*  hRAA = (TH1D*) f->Get(raaName);
  TH1D*  hRAASyst = (TH1D*) f->Get(raaSystName);
  
  const int N = hRAA->GetNbinsX();
  TGraphAsymmErrors *gRAA = new TGraphAsymmErrors("g"+species, "Converted from TH1D", N);
  TGraphAsymmErrors *gRAASyst = new TGraphAsymmErrors("g"+species+"Syst", "Converted from TH1D", N);
  
  for (int i = 1; i <= N; ++i) {
    double x = hRAA->GetBinCenter(i);
    double y = hRAA->GetBinContent(i);
    double ex = hRAASyst->GetBinWidth(i) / 2.0;
    double ey = hRAA->GetBinError(i);
    double eySyst = hRAASyst->GetBinContent(i) / 100.0 * y;
    //luminosity scaling
    if ( x < 10.0) {
      y = y / lowPtLumi;
      ey = ey / lowPtLumi;
    } else {
      y = y / highPtLumi;
      ey = ey / highPtLumi;
    }
    gRAA->SetPoint(i-1, x, y);
    gRAA->SetPointError(i-1, 0, ey);
    gRAASyst->SetPoint(i-1, x, y);
    gRAASyst->SetPointError(i-1, ex, ex, eySyst, eySyst);
  }
  
  f->Close();
  delete hRAA;
  delete hRAASyst;
  delete f;
  
  vector<TGraphAsymmErrors*> vRAA = {gRAA, gRAASyst};
  
  return vRAA;
}

void MakePlots_RAA_NeNe_DataVsTheory(
) {
  vector<TGraphAsymmErrors*> vRAAOO = BuildDataRAAGraphs(
    "Results/pp_OO_raa_20250904.root",
    "normalized_RAA",
    "Raa_Total_uncertainty",
    "OO"
  );
  vector<TGraphAsymmErrors*> vRAANeNe = BuildDataRAAGraphs(
    "Results/NeNeOverPPRatio.root",
    "normalized_RAA_NucleusNucleus",
    "Raa_Total_uncertainty_NucleusNucleus",
    "NeNe"
  );
  
  TFile* f1 = TFile::Open("../../Theory/20250819_OONeNePredictions/20250821_CUJET/MinBias_CUJET.root");
  TFile* f2 = TFile::Open("../../Theory/20250819_OONeNePredictions/20250820_Zakharov/MinBias_Zakharov.root");
  TFile* f3 = TFile::Open("../../Theory/20250819_OONeNePredictions/20250819_FaradayHorowitz/MinBias.root");
  TFile* f4 = TFile::Open("../../Theory/20250828_OONeNePredictions/MinBias_SimpleModel.root");
  TFile* f5 = TFile::Open("../../Theory/20250904_OONeNePredictions/MinBias_TrajectumJEWELhadRAA.root");
  TFile* f6 = TFile::Open("../../Theory/20250904_OONeNePredictions/MinBias_TrajectumPathLength.root");
    
  // === Fetch graphs from MinBias.root ===
  TGraphAsymmErrors* gNeNe_CUJET = (TGraphAsymmErrors*) f1->Get("NeNeMinBias");
  TGraphAsymmErrors* gOO_CUJET   = (TGraphAsymmErrors*) f1->Get("OOMinBias");

  // === Fetch graphs from MinBias_Zakharov.root ===
  TGraphAsymmErrors* gOO_Zakharov   = (TGraphAsymmErrors*) f2->Get("OOMinBias");
  TGraphAsymmErrors* gNeNe_Zakharov = (TGraphAsymmErrors*) f2->Get("NeNeMinBias");
  TGraphAsymmErrors* gOO_mQGP   = (TGraphAsymmErrors*) f2->Get("OOMinBias_mQGP");
  TGraphAsymmErrors* gNeNe_mQGP = (TGraphAsymmErrors*) f2->Get("NeNeMinBias_mQGP");
  TGraphAsymmErrors* gOO_Faraday   = (TGraphAsymmErrors*) f3->Get("OOMinBias");
  gOOFaraday->SetName("OOMinBias_Faraday");
  TGraphAsymmErrors* gNeNe_Faraday = (TGraphAsymmErrors*) f3->Get("NeNeMinBias");
  gNeNeFaraday->SetName("NeNeMinBias_Faraday");

  // === Fetch graphs from MinBias_SimpleModel.root ===
  TGraphAsymmErrors* gOO_PGCM  = (TGraphAsymmErrors*) f4->Get("OO_PGCM");
  TGraphAsymmErrors* gNeNe_PGCM = (TGraphAsymmErrors*) f4->Get("NeNe_PGCM");
  TGraphAsymmErrors* gOO_NLEFT = (TGraphAsymmErrors*) f4->Get("OO_NLEFT");
  TGraphAsymmErrors* gNeNe_NLEFT = (TGraphAsymmErrors*) f4->Get("NeNe_NLEFT");

  // === Fetch graphs from MinBias_TrajectumJEWELhadRAA.root ===
  TGraphAsymmErrors* gOO_PGCM_JEWEL  = (TGraphAsymmErrors*) f5->Get("OO_PGCM");
  gOO_PGCM_JEWEL->SetName("gOO_PGCM_JEWEL");
  TGraphAsymmErrors* gNeNe_PGCM_JEWEL = (TGraphAsymmErrors*) f5->Get("NeNe_PGCM");
  gNeNe_PGCM_JEWEL->SetName("gNeNe_PGCM_JEWEL");
  TGraphAsymmErrors* gOO_NLEFT_JEWEL = (TGraphAsymmErrors*) f5->Get("OO_NLEFT");
  gOO_NLEFT_JEWEL->SetName("gOO_NLEFT_JEWEL");
  TGraphAsymmErrors* gNeNe_NLEFT_JEWEL = (TGraphAsymmErrors*) f5->Get("NeNe_NLEFT");
  gNeNe_NLEFT_JEWEL->SetName("gNeNe_NLEFT_JEWEL");
    
  // === Fetch graphs from MinBias_TrajectumPathLength.root ===
  TGraphAsymmErrors* gOO_PGCM_PATH  = (TGraphAsymmErrors*) f6->Get("OO_PGCM");
  gOO_PGCM_PATH->SetName("gOO_PGCM_PATH");
  TGraphAsymmErrors* gNeNe_PGCM_PATH = (TGraphAsymmErrors*) f6->Get("NeNe_PGCM");
  gNeNe_PGCM_PATH->SetName("gNeNe_PGCM_PATH");
  TGraphAsymmErrors* gOO_NLEFT_PATH = (TGraphAsymmErrors*) f6->Get("OO_NLEFT");
  gOO_NLEFT_PATH->SetName("gOO_NLEFT_PATH");
  TGraphAsymmErrors* gNeNe_NLEFT_PATH = (TGraphAsymmErrors*) f6->Get("NeNe_NLEFT");
  gNeNe_NLEFT_PATH->SetName("gNeNe_NLEFT_PATH");
  
  PlotRAA_DataVsTheory(
    "plots_DataVsTheory/Figure_003_theory_Faraday.pdf",
    vRAAOO[0], vRAAOO[1],
    gOO_Faraday,
    "Faraday & Horowitz, OO",
    vRAANeNe[0], vRAANeNe[1],
    gNeNe_Faraday,
    "Faraday & Horowitz, NeNe",
    1, // theory line style
    0., 1.6, // ymin, ymax
  );
  PlotRAA_DataVsTheory(
    "plots_DataVsTheory/Figure_003_theory_CUJET.pdf",
    vRAAOO[0], vRAAOO[1],
    gOO_CUJET,
    "CUJET/CIBJET, OO",
    vRAANeNe[0], vRAANeNe[1],
    gNeNe_CUJET,
    "CUJET/CIBJET, NeNe",
    2, // theory line style
    0., 1.6, // ymin, ymax
  );
  PlotRAA_DataVsTheory(
    "plots_DataVsTheory/Figure_003_theory_Zakharov.pdf",
    vRAAOO[0], vRAAOO[1],
    gOO_Zakharov,
    "LCPI + mQGP, OO (Zakharov)",
    vRAANeNe[0], vRAANeNe[1],
    gNeNe_Zakharov,
    "LCPI + mQGP, NeNe (Zakharov)",
    3, // theory line style
    0., 1.6, // ymin, ymax
  );
  PlotRAA_DataVsTheory(
    "plots_DataVsTheory/Figure_003_theory_Zakharov_nomQGP.pdf",
    vRAAOO[0], vRAAOO[1],
    gOO_mQGP,
    "LCPI + no mQGP, OO (Zakharov)",
    vRAANeNe[0], vRAANeNe[1],
    gNeNe_mQGP,
    "LCPI + no mQGP, NeNe (Zakharov)",
    4, // theory line style
    0., 1.6, // ymin, ymax
  );
  PlotRAA_DataVsTheory(
    "plots_DataVsTheory/Figure_004_theory_PGCM.pdf",
    vRAAOO[0], vRAAOO[1],
    gOO_PGCM,
    "Simple model PGCM, OO",
    vRAANeNe[0], vRAANeNe[1],
    gNeNe_PGCM,
    "Simple model PGCM, NeNe",
    5, // theory line style
    0., 1.6, // ymin, ymax
  );
  PlotRAA_DataVsTheory(
    "plots_DataVsTheory/Figure_004_theory_NLEFT.pdf",
    vRAAOO[0], vRAAOO[1],
    gOO_NLEFT,
    "Simple model NLEFT, OO",
    vRAANeNe[0], vRAANeNe[1],
    gNeNe_NLEFT,
    "Simple model NLEFT, NeNe",
    6, // theory line style
    0., 1.6, // ymin, ymax
  );
  PlotRAA_DataVsTheory(
    ("plots_DataVsTheory/Figure_005_theory_PGCM_JEWEL.pdf"),
    vRAAOO[0], vRAAOO[1],
    gOO_PGCM_JEWEL,
    "Trajectum JEWEL PGCM, OO",
    vRAANeNe[0], vRAANeNe[1],
    gNeNe_PGCM_JEWEL,
    "Trajectum JEWEL PGCM, NeNe",
    7, // theory line style
    0., 1.6, // ymin, ymax
  );
  PlotRAA_DataVsTheory(
    "plots_DataVsTheory/Figure_005_theory_NLEFT_JEWEL.pdf",
    vRAAOO[0], vRAAOO[1],
    gOO_NLEFT_JEWEL,
    "Trajectum JEWEL NLEFT, OO",
    vRAANeNe[0], vRAANeNe[1],
    gNeNe_NLEFT_JEWEL,
    "Trajectum JEWEL NLEFT, NeNe",
    8, // theory line style
    0., 1.6, // ymin, ymax
  );
  PlotRAA_DataVsTheory(
    "plots_DataVsTheory/Figure_005_theory_PGCM_PATH.pdf",
    vRAAOO[0], vRAAOO[1],
    gOO_PGCM_PATH,
    "Path length PGCM, OO",
    vRAANeNe[0], vRAANeNe[1],
    gNeNe_PGCM_PATH,
    "Path length PGCM, NeNe",
    9, // theory line style
    0., 1.6, // ymin, ymax
  );
  PlotRAA_DataVsTheory(
    "plots_DataVsTheory/Figure_005_theory_NLEFT_PATH.pdf",
    vRAAOO[0], vRAAOO[1],
    gOO_NLEFT_PATH,
    "Path length NLEFT, OO",
    vRAANeNe[0], vRAANeNe[1],
    gNeNe_NLEFT_PATH,
    "Path length NLEFT, NeNe",
    10, // theory line style
    0., 1.6, // ymin, ymax
  );
}
