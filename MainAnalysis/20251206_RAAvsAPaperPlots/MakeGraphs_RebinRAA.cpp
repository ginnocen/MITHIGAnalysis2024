#include <TROOT.h>
#include <vector>
#include <filesystem>
#include "MITHIG_CMSStyle.h"
#define A_POINTS 5
#define N_POINTS 6

bool plotBinMergeChecks = false;
bool printMergeTable = false;

// OO, 5.36 TeV, 0-100% (https://arxiv.org/pdf/2507.05853, p20)
float Npart_OO = 10.8;
float Ncoll_OO = 12.8;
// NeNe, 5.36 TeV, 0-100% (https://arxiv.org/pdf/2507.05853, p20)
float Npart_NeNe = 12.7;
float Ncoll_NeNe = 15.7;
// XeXe 5.44 TeV, 0-100% (https://arxiv.org/pdf/1710.07098, p22)
float Npart_XeXe = 71.26;
float Ncoll_XeXe = 199.6;
// PbPb 5.02 TeV, 0-100% (https://arxiv.org/pdf/1710.07098, p18)
float Npart_PbPb = 113.8;
float Ncoll_PbPb = 382.2;
// pPb 5.02 TeV, 0-100% (https://arxiv.org/pdf/1710.07098, p20)
float Npart_pPb = 7.723;
float Ncoll_pPb = 6.723;

using namespace std;

void FillBinsBySpecies(
  TGraphErrors* gRAA_All,
  TGraphErrors* gRAA_All_Stat,
  TGraphErrors* gRAA_All_Syst,
  TGraphErrors* gRAA_All_Norm,
  TGraphErrors* gRAA_All_TotUncert,
  TH1F* hRAA_AA,
  TH1F* hRAA_AA_Stat,
  TH1F* hRAA_AA_Syst,
  TH1F* hRAA_AA_Norm,
  TH1F* hRAA_AA_TotUncert,
  float ptMin,
  float ptMax,
  int iPoint,
  float pointX
) {
  for (int i = 1; i <= hRAA_AA->GetNbinsX(); i++) {
    if (hRAA_AA->GetBinLowEdge(i) < ptMin - 0.001 ||
        hRAA_AA->GetBinLowEdge(i+1) > ptMax + 0.001) continue;
    else {
      gRAA_All->SetPoint(iPoint, pointX, hRAA_AA->GetBinContent(i));
      gRAA_All->SetPointError(iPoint, 0, 0);
      
      gRAA_All_Stat->SetPoint(iPoint, pointX, hRAA_AA->GetBinContent(i));
      gRAA_All_Stat->SetPointError(iPoint, 0, hRAA_AA_Stat->GetBinContent(i));
      
      gRAA_All_Syst->SetPoint(iPoint, pointX, hRAA_AA->GetBinContent(i));
      gRAA_All_Syst->SetPointError(iPoint, 0, hRAA_AA_Syst->GetBinContent(i));
      
      gRAA_All_Norm->SetPoint(iPoint, pointX, hRAA_AA->GetBinContent(i));
      gRAA_All_Norm->SetPointError(iPoint, 0, hRAA_AA_Norm->GetBinContent(i));
      
      gRAA_All_TotUncert->SetPoint(iPoint, pointX, hRAA_AA->GetBinContent(i));
      gRAA_All_TotUncert->SetPointError(iPoint, 0, hRAA_AA_TotUncert->GetBinContent(i));
    }
    break;
  }
}

void MakePtBinnedGraphs(
  TH1F* hRAA_OO,
  TH1F* hRAA_OO_Stat,
  TH1F* hRAA_OO_Syst,
  TH1F* hRAA_OO_Norm,
  TH1F* hRAA_OO_TotUncert,
  TH1F* hRAA_NeNe,
  TH1F* hRAA_NeNe_Stat,
  TH1F* hRAA_NeNe_Syst,
  TH1F* hRAA_NeNe_Norm,
  TH1F* hRAA_NeNe_TotUncert,
  TH1F* hRAA_XeXe,
  TH1F* hRAA_XeXe_Stat,
  TH1F* hRAA_XeXe_Syst,
  TH1F* hRAA_XeXe_Norm,
  TH1F* hRAA_XeXe_TotUncert,
  TH1F* hRAA_PbPb,
  TH1F* hRAA_PbPb_Stat,
  TH1F* hRAA_PbPb_Syst,
  TH1F* hRAA_PbPb_Norm,
  TH1F* hRAA_PbPb_TotUncert,
  TH1F* hRAA_pPb,
  TH1F* hRAA_pPb_Stat,
  TH1F* hRAA_pPb_Syst,
  TH1F* hRAA_pPb_Norm,
  TH1F* hRAA_pPb_TotUncert,
  vector<float> ptBins,
  bool doLogx = true
) {
  int nPtBins = ptBins.size();
  // OO RAAvsPt
  TGraphErrors* gOO_RAAVsPt = new TGraphErrors(nPtBins);
    gOO_RAAVsPt->SetName("gOO_RAAVsPt");
    gOO_RAAVsPt->SetTitle("OO R_{AA}, Central value only");
  TGraphErrors* gOO_RAAVsPt_Stat = new TGraphErrors(nPtBins);
    gOO_RAAVsPt_Stat->SetName("gOO_RAAVsPt_Stat");
    gOO_RAAVsPt_Stat->SetTitle("OO R_{AA} with statistical uncertainty");
  TGraphErrors* gOO_RAAVsPt_Syst = new TGraphErrors(nPtBins);
    gOO_RAAVsPt_Syst->SetName("gOO_RAAVsPt_Syst");
    gOO_RAAVsPt_Syst->SetTitle("OO R_{AA} with systematic uncertainty");
  TGraphErrors* gOO_RAAVsPt_Norm = new TGraphErrors(nPtBins);
    gOO_RAAVsPt_Norm->SetName("gOO_RAAVsPt_Norm");
    gOO_RAAVsPt_Norm->SetTitle("OO R_{AA} with normalization uncertainty");
  TGraphErrors* gOO_RAAVsPt_TotUncert = new TGraphErrors(nPtBins);
    gOO_RAAVsPt_TotUncert->SetName("gOO_RAAVsPt_TotUncert");
    gOO_RAAVsPt_TotUncert->SetTitle("OO R_{AA} with total combined uncertainty");
  // NeNe RAAvsPt
  TGraphErrors* gNeNe_RAAVsPt = new TGraphErrors(nPtBins);
    gNeNe_RAAVsPt->SetName("gNeNe_RAAVsPt");
    gNeNe_RAAVsPt->SetTitle("NeNe R_{AA}, Central value only");
  TGraphErrors* gNeNe_RAAVsPt_Stat = new TGraphErrors(nPtBins);
    gNeNe_RAAVsPt_Stat->SetName("gNeNe_RAAVsPt_Stat");
    gNeNe_RAAVsPt_Stat->SetTitle("NeNe R_{AA} with statistical uncertainty");
  TGraphErrors* gNeNe_RAAVsPt_Syst = new TGraphErrors(nPtBins);
    gNeNe_RAAVsPt_Syst->SetName("gNeNe_RAAVsPt_Syst");
    gNeNe_RAAVsPt_Syst->SetTitle("NeNe R_{AA} with systematic uncertainty");
  TGraphErrors* gNeNe_RAAVsPt_Norm = new TGraphErrors(nPtBins);
    gNeNe_RAAVsPt_Norm->SetName("gNeNe_RAAVsPt_Norm");
    gNeNe_RAAVsPt_Norm->SetTitle("NeNe R_{AA} with normalization uncertainty");
  TGraphErrors* gNeNe_RAAVsPt_TotUncert = new TGraphErrors(nPtBins);
    gNeNe_RAAVsPt_TotUncert->SetName("gNeNe_RAAVsPt_TotUncert");
    gNeNe_RAAVsPt_TotUncert->SetTitle("NeNe R_{AA} with total combined uncertainty");
  // XeXe RAAvsPt
  TGraphErrors* gXeXe_RAAVsPt = new TGraphErrors(nPtBins);
    gXeXe_RAAVsPt->SetName("gXeXe_RAAVsPt");
    gXeXe_RAAVsPt->SetTitle("XeXe R_{AA}, Central value only");
  TGraphErrors* gXeXe_RAAVsPt_Stat = new TGraphErrors(nPtBins);
    gXeXe_RAAVsPt_Stat->SetName("gXeXe_RAAVsPt_Stat");
    gXeXe_RAAVsPt_Stat->SetTitle("XeXe R_{AA} with statistical uncertainty");
  TGraphErrors* gXeXe_RAAVsPt_Syst = new TGraphErrors(nPtBins);
    gXeXe_RAAVsPt_Syst->SetName("gXeXe_RAAVsPt_Syst");
    gXeXe_RAAVsPt_Syst->SetTitle("XeXe R_{AA} with systematic uncertainty");
  TGraphErrors* gXeXe_RAAVsPt_Norm = new TGraphErrors(nPtBins);
    gXeXe_RAAVsPt_Norm->SetName("gXeXe_RAAVsPt_Norm");
    gXeXe_RAAVsPt_Norm->SetTitle("XeXe R_{AA} with normalization uncertainty");
  TGraphErrors* gXeXe_RAAVsPt_TotUncert = new TGraphErrors(nPtBins);
    gXeXe_RAAVsPt_TotUncert->SetName("gXeXe_RAAVsPt_TotUncert");
    gXeXe_RAAVsPt_TotUncert->SetTitle("XeXe R_{AA} with total combined uncertainty");
  // PbPb RAAvsPt
  TGraphErrors* gPbPb_RAAVsPt = new TGraphErrors(nPtBins);
    gPbPb_RAAVsPt->SetName("gPbPb_RAAVsPt");
    gPbPb_RAAVsPt->SetTitle("PbPb R_{AA}, Central value only");
  TGraphErrors* gPbPb_RAAVsPt_Stat = new TGraphErrors(nPtBins);
    gPbPb_RAAVsPt_Stat->SetName("gPbPb_RAAVsPt_Stat");
    gPbPb_RAAVsPt_Stat->SetTitle("PbPb R_{AA} with statistical uncertainty");
  TGraphErrors* gPbPb_RAAVsPt_Syst = new TGraphErrors(nPtBins);
    gPbPb_RAAVsPt_Syst->SetName("gPbPb_RAAVsPt_Syst");
    gPbPb_RAAVsPt_Syst->SetTitle("PbPb R_{AA} with systematic uncertainty");
  TGraphErrors* gPbPb_RAAVsPt_Norm = new TGraphErrors(nPtBins);
    gPbPb_RAAVsPt_Norm->SetName("gPbPb_RAAVsPt_Norm");
    gPbPb_RAAVsPt_Norm->SetTitle("PbPb R_{AA} with normalization uncertainty");
  TGraphErrors* gPbPb_RAAVsPt_TotUncert = new TGraphErrors(nPtBins);
    gPbPb_RAAVsPt_TotUncert->SetName("gPbPb_RAAVsPt_TotUncert");
    gPbPb_RAAVsPt_TotUncert->SetTitle("PbPb R_{AA} with total combined uncertainty");
  // pPb RAAvsPt
  TGraphErrors* gpPb_RAAVsPt = new TGraphErrors(nPtBins);
    gpPb_RAAVsPt->SetName("gpPb_RAAVsPt");
    gpPb_RAAVsPt->SetTitle("pPb R_{AA}, Central value only");
  TGraphErrors* gpPb_RAAVsPt_Stat = new TGraphErrors(nPtBins);
    gpPb_RAAVsPt_Stat->SetName("gpPb_RAAVsPt_Stat");
    gpPb_RAAVsPt_Stat->SetTitle("pPb R_{AA} with statistical uncertainty");
  TGraphErrors* gpPb_RAAVsPt_Syst = new TGraphErrors(nPtBins);
    gpPb_RAAVsPt_Syst->SetName("gpPb_RAAVsPt_Syst");
    gpPb_RAAVsPt_Syst->SetTitle("pPb R_{AA} with systematic uncertainty");
  TGraphErrors* gpPb_RAAVsPt_Norm = new TGraphErrors(nPtBins);
    gpPb_RAAVsPt_Norm->SetName("gpPb_RAAVsPt_Norm");
    gpPb_RAAVsPt_Norm->SetTitle("pPb R_{AA} with normalization uncertainty");
  TGraphErrors* gpPb_RAAVsPt_TotUncert = new TGraphErrors(nPtBins);
    gpPb_RAAVsPt_TotUncert->SetName("gpPb_RAAVsPt_TotUncert");
    gpPb_RAAVsPt_TotUncert->SetTitle("pPb R_{AA} with total combined uncertainty");
  for (int i = 0; i < nPtBins-1; i++) {
    float x = 0.5 * (ptBins[i] + ptBins[i+1]);
    float xErr = 0.5 * (ptBins[i+1] - ptBins[i]);
    for (int j = 1; j <= hRAA_OO->GetNbinsX(); j++) {
      if (hRAA_OO->GetBinLowEdge(j) < ptBins[i] - 0.001 ||
          hRAA_OO->GetBinLowEdge(j+1) > ptBins[i+1] + 0.001) continue;
      float yOO = hRAA_OO->GetBinContent(j);
      gOO_RAAVsPt->SetPoint(i, x, yOO);
      gOO_RAAVsPt_Stat->SetPoint(i, x, yOO);
      gOO_RAAVsPt_Syst->SetPoint(i, x, yOO);
      gOO_RAAVsPt_Norm->SetPoint(i, x, yOO);
      gOO_RAAVsPt_TotUncert->SetPoint(i, x, yOO);
      gOO_RAAVsPt->SetPointError(i, 0., 0.);
      gOO_RAAVsPt_Stat->SetPointError(i, 0., hRAA_OO_Stat->GetBinContent(j));
      gOO_RAAVsPt_Syst->SetPointError(i, xErr, hRAA_OO_Syst->GetBinContent(j));
      gOO_RAAVsPt_Norm->SetPointError(i, xErr, hRAA_OO_Norm->GetBinContent(j));
      gOO_RAAVsPt_TotUncert->SetPointError(i, xErr, hRAA_OO_TotUncert->GetBinContent(j));
    }
    for (int j = 1; j <= hRAA_NeNe->GetNbinsX(); j++) {
      if (hRAA_NeNe->GetBinLowEdge(j) < ptBins[i] - 0.001 ||
          hRAA_NeNe->GetBinLowEdge(j+1) > ptBins[i+1] + 0.001) continue;
      float yNeNe = hRAA_NeNe->GetBinContent(j);
      gNeNe_RAAVsPt->SetPoint(i, x, yNeNe);
      gNeNe_RAAVsPt_Stat->SetPoint(i, x, yNeNe);
      gNeNe_RAAVsPt_Syst->SetPoint(i, x, yNeNe);
      gNeNe_RAAVsPt_Norm->SetPoint(i, x, yNeNe);
      gNeNe_RAAVsPt_TotUncert->SetPoint(i, x, yNeNe);
      gNeNe_RAAVsPt->SetPointError(i, 0., 0.);
      gNeNe_RAAVsPt_Stat->SetPointError(i, 0., hRAA_NeNe_Stat->GetBinContent(j));
      gNeNe_RAAVsPt_Syst->SetPointError(i, xErr, hRAA_NeNe_Syst->GetBinContent(j));
      gNeNe_RAAVsPt_Norm->SetPointError(i, xErr, hRAA_NeNe_Norm->GetBinContent(j));
      gNeNe_RAAVsPt_TotUncert->SetPointError(i, xErr, hRAA_NeNe_TotUncert->GetBinContent(j));
    }
    for (int j = 1; j <= hRAA_XeXe->GetNbinsX(); j++) {
      if (hRAA_XeXe->GetBinLowEdge(j) < ptBins[i] - 0.001 ||
          hRAA_XeXe->GetBinLowEdge(j+1) > ptBins[i+1] + 0.001) continue;
      float yXeXe = hRAA_XeXe->GetBinContent(j);
      gXeXe_RAAVsPt->SetPoint(i, x, yXeXe);
      gXeXe_RAAVsPt_Stat->SetPoint(i, x, yXeXe);
      gXeXe_RAAVsPt_Syst->SetPoint(i, x, yXeXe);
      gXeXe_RAAVsPt_Norm->SetPoint(i, x, yXeXe);
      gXeXe_RAAVsPt_TotUncert->SetPoint(i, x, yXeXe);
      gXeXe_RAAVsPt->SetPointError(i, 0., 0.);
      gXeXe_RAAVsPt_Stat->SetPointError(i, 0., hRAA_XeXe_Stat->GetBinContent(j));
      gXeXe_RAAVsPt_Syst->SetPointError(i, xErr, hRAA_XeXe_Syst->GetBinContent(j));
      gXeXe_RAAVsPt_Norm->SetPointError(i, xErr, hRAA_XeXe_Norm->GetBinContent(j));
      gXeXe_RAAVsPt_TotUncert->SetPointError(i, xErr, hRAA_XeXe_TotUncert->GetBinContent(j));
    }
    for (int j = 1; j <= hRAA_PbPb->GetNbinsX(); j++) {
      if (hRAA_PbPb->GetBinLowEdge(j) < ptBins[i] - 0.001 ||
          hRAA_PbPb->GetBinLowEdge(j+1) > ptBins[i+1] + 0.001) continue;
      float yPbPb = hRAA_PbPb->GetBinContent(j);
      gPbPb_RAAVsPt->SetPoint(i, x, yPbPb);
      gPbPb_RAAVsPt_Stat->SetPoint(i, x, yPbPb);
      gPbPb_RAAVsPt_Syst->SetPoint(i, x, yPbPb);
      gPbPb_RAAVsPt_Norm->SetPoint(i, x, yPbPb);
      gPbPb_RAAVsPt_TotUncert->SetPoint(i, x, yPbPb);
      gPbPb_RAAVsPt->SetPointError(i, 0., 0.);
      gPbPb_RAAVsPt_Stat->SetPointError(i, 0., hRAA_PbPb_Stat->GetBinContent(j));
      gPbPb_RAAVsPt_Syst->SetPointError(i, xErr, hRAA_PbPb_Syst->GetBinContent(j));
      gPbPb_RAAVsPt_Norm->SetPointError(i, xErr, hRAA_PbPb_Norm->GetBinContent(j));
      gPbPb_RAAVsPt_TotUncert->SetPointError(i, xErr, hRAA_PbPb_TotUncert->GetBinContent(j));
    }
    for (int j = 1; j <= hRAA_pPb->GetNbinsX(); j++) {
      if (hRAA_pPb->GetBinLowEdge(j) < ptBins[i] - 0.001 ||
          hRAA_pPb->GetBinLowEdge(j+1) > ptBins[i+1] + 0.001) continue;
      float ypPb = hRAA_pPb->GetBinContent(j);
      gpPb_RAAVsPt->SetPoint(i, x, ypPb);
      gpPb_RAAVsPt_Stat->SetPoint(i, x, ypPb);
      gpPb_RAAVsPt_Syst->SetPoint(i, x, ypPb);
      gpPb_RAAVsPt_Norm->SetPoint(i, x, ypPb);
      gpPb_RAAVsPt_TotUncert->SetPoint(i, x, ypPb);
      gpPb_RAAVsPt->SetPointError(i, 0., 0.);
      gpPb_RAAVsPt_Stat->SetPointError(i, 0., hRAA_pPb_Stat->GetBinContent(j));
      gpPb_RAAVsPt_Syst->SetPointError(i, xErr, hRAA_pPb_Syst->GetBinContent(j));
      gpPb_RAAVsPt_Norm->SetPointError(i, xErr, hRAA_pPb_Norm->GetBinContent(j));
      gpPb_RAAVsPt_TotUncert->SetPointError(i, xErr, hRAA_pPb_TotUncert->GetBinContent(j));
    }
  }
  // Save TGraphs to file
  TFile* fout_RAAvsPt = new TFile("speciesBinned_RAAVsPt.root", "RECREATE");
  fout_RAAvsPt->cd();
  gOO_RAAVsPt->Write();
  gOO_RAAVsPt_Stat->Write();
  gOO_RAAVsPt_Syst->Write();
  gOO_RAAVsPt_Norm->Write();
  gOO_RAAVsPt_TotUncert->Write();
  gNeNe_RAAVsPt->Write();
  gNeNe_RAAVsPt_Stat->Write();
  gNeNe_RAAVsPt_Syst->Write();
  gNeNe_RAAVsPt_Norm->Write();
  gNeNe_RAAVsPt_TotUncert->Write();
  gXeXe_RAAVsPt->Write();
  gXeXe_RAAVsPt_Stat->Write();
  gXeXe_RAAVsPt_Syst->Write();
  gXeXe_RAAVsPt_Norm->Write();
  gXeXe_RAAVsPt_TotUncert->Write();
  gPbPb_RAAVsPt->Write();
  gPbPb_RAAVsPt_Stat->Write();
  gPbPb_RAAVsPt_Syst->Write();
  gPbPb_RAAVsPt_Norm->Write();
  gPbPb_RAAVsPt_TotUncert->Write();
  gpPb_RAAVsPt->Write();
  gpPb_RAAVsPt_Stat->Write();
  gpPb_RAAVsPt_Syst->Write();
  gpPb_RAAVsPt_Norm->Write();
  gpPb_RAAVsPt_TotUncert->Write();
  cout << "Made speciesBinned_RAAVsPt.root" << endl;
  fout_RAAvsPt->Close();
  
  vector<TGraphErrors*> vRAA_PtVsA;
  vector<TGraphErrors*> vRAA_PtVsA_Stat;
  vector<TGraphErrors*> vRAA_PtVsA_Syst;
  vector<TGraphErrors*> vRAA_PtVsA_Norm;
  vector<TGraphErrors*> vRAA_PtVsA_TotUncert;
  for (int i = 0; i < ptBins.size()-1; i++) {
    TString ptBinTitle = Form("%.1f < p_{T} < %.1f GeV", ptBins[i], ptBins[i+1]);
    
    TGraphErrors* gRAA_PtVsA = new TGraphErrors(A_POINTS);
    gRAA_PtVsA->SetName(Form("gRAA_PtVsA_ptBin%d", i));
    gRAA_PtVsA->SetTitle(ptBinTitle);
    vRAA_PtVsA.push_back(gRAA_PtVsA);
    
    TGraphErrors* gRAA_PtVsA_Stat = new TGraphErrors(A_POINTS);
    gRAA_PtVsA_Stat->SetName(Form("gRAA_PtVsA_Stat_ptBin%d", i));
    gRAA_PtVsA_Stat->SetTitle(ptBinTitle);
    vRAA_PtVsA_Stat.push_back(gRAA_PtVsA_Stat);
    
    TGraphErrors* gRAA_PtVsA_Syst = new TGraphErrors(A_POINTS);
    gRAA_PtVsA_Syst->SetName(Form("gRAA_PtVsA_Syst_ptBin%d", i));
    gRAA_PtVsA_Syst->SetTitle(ptBinTitle);
    vRAA_PtVsA_Syst.push_back(gRAA_PtVsA_Syst);
    
    TGraphErrors* gRAA_PtVsA_Norm = new TGraphErrors(A_POINTS);
    gRAA_PtVsA_Norm->SetName(Form("gRAA_PtVsA_Norm_ptBin%d", i));
    gRAA_PtVsA_Norm->SetTitle(ptBinTitle);
    vRAA_PtVsA_Norm.push_back(gRAA_PtVsA_Norm);
    
    TGraphErrors* gRAA_PtVsA_TotUncert = new TGraphErrors(A_POINTS);
    gRAA_PtVsA_TotUncert->SetName(Form("gRAA_PtVsA_TotUncert_ptBin%d", i));
    gRAA_PtVsA_TotUncert->SetTitle(ptBinTitle);
    vRAA_PtVsA_TotUncert.push_back(gRAA_PtVsA_TotUncert);
  }
  for (int i = 0; i < ptBins.size()-1; i++) {
    vRAA_PtVsA[i]->SetPoint(0, 0, -1);
    vRAA_PtVsA_Stat[i]->SetPoint(0, 0, -1);
    vRAA_PtVsA_Syst[i]->SetPoint(0, 0, -1);
    vRAA_PtVsA_Norm[i]->SetPoint(0, 0, -1);
    vRAA_PtVsA_TotUncert[i]->SetPoint(0, 0, -1);
    FillBinsBySpecies( // OO
      vRAA_PtVsA[i],
      vRAA_PtVsA_Stat[i],
      vRAA_PtVsA_Syst[i],
      vRAA_PtVsA_Norm[i],
      vRAA_PtVsA_TotUncert[i],
      hRAA_OO,
      hRAA_OO_Stat,
      hRAA_OO_Syst,
      hRAA_OO_Norm,
      hRAA_OO_TotUncert,
      ptBins[i],
      ptBins[i+1],
      1, 16
    );
    FillBinsBySpecies( // NeNe
      vRAA_PtVsA[i],
      vRAA_PtVsA_Stat[i],
      vRAA_PtVsA_Syst[i],
      vRAA_PtVsA_Norm[i],
      vRAA_PtVsA_TotUncert[i],
      hRAA_NeNe,
      hRAA_NeNe_Stat,
      hRAA_NeNe_Syst,
      hRAA_NeNe_Norm,
      hRAA_NeNe_TotUncert,
      ptBins[i],
      ptBins[i+1],
      2, 20
    );
    FillBinsBySpecies( // XeXe
      vRAA_PtVsA[i],
      vRAA_PtVsA_Stat[i],
      vRAA_PtVsA_Syst[i],
      vRAA_PtVsA_Norm[i],
      vRAA_PtVsA_TotUncert[i],
      hRAA_XeXe,
      hRAA_XeXe_Stat,
      hRAA_XeXe_Syst,
      hRAA_XeXe_Norm,
      hRAA_XeXe_TotUncert,
      ptBins[i],
      ptBins[i+1],
      3, 129
    );
    FillBinsBySpecies( // PbPb
      vRAA_PtVsA[i],
      vRAA_PtVsA_Stat[i],
      vRAA_PtVsA_Syst[i],
      vRAA_PtVsA_Norm[i],
      vRAA_PtVsA_TotUncert[i],
      hRAA_PbPb,
      hRAA_PbPb_Stat,
      hRAA_PbPb_Syst,
      hRAA_PbPb_Norm,
      hRAA_PbPb_TotUncert,
      ptBins[i],
      ptBins[i+1],
      4, 208
    );
  }
  // Save TGraphs to file
  TFile* fout_A = new TFile("ptBinned_RAAVsA.root", "RECREATE");
  fout_A->cd();
  for (int i = 0; i < vRAA_PtVsA.size(); i++) {
    vRAA_PtVsA[i]->Write();
    vRAA_PtVsA_Stat[i]->Write();
    vRAA_PtVsA_Syst[i]->Write();
    vRAA_PtVsA_Norm[i]->Write();
    vRAA_PtVsA_TotUncert[i]->Write();
  }
  cout << "Made ptBinned_RAAVsA.root" << endl;
  fout_A->Close();
  
  vector<TGraphErrors*> vRAA_PtVsNcoll;
  vector<TGraphErrors*> vRAA_PtVsNcoll_Stat;
  vector<TGraphErrors*> vRAA_PtVsNcoll_Syst;
  vector<TGraphErrors*> vRAA_PtVsNcoll_Norm;
  vector<TGraphErrors*> vRAA_PtVsNcoll_TotUncert;
  vector<TGraphErrors*> vRAA_PtVsNpart;
  vector<TGraphErrors*> vRAA_PtVsNpart_Stat;
  vector<TGraphErrors*> vRAA_PtVsNpart_Syst;
  vector<TGraphErrors*> vRAA_PtVsNpart_Norm;
  vector<TGraphErrors*> vRAA_PtVsNpart_TotUncert;
  for (int i = 0; i < ptBins.size()-1; i++) {
    TString ptBinTitle = Form("%.1f < p_{T} < %.1f GeV", ptBins[i], ptBins[i+1]);
    
    // N_coll
    TGraphErrors* gRAA_PtVsNcoll = new TGraphErrors(N_POINTS);
    gRAA_PtVsNcoll->SetName(Form("gRAA_PtVsNcoll_ptBin%d", i));
    gRAA_PtVsNcoll->SetTitle(ptBinTitle);
    vRAA_PtVsNcoll.push_back(gRAA_PtVsNcoll);
    
    TGraphErrors* gRAA_PtVsNcoll_Stat = new TGraphErrors(N_POINTS);
    gRAA_PtVsNcoll_Stat->SetName(Form("gRAA_PtVsNcoll_Stat_ptBin%d", i));
    gRAA_PtVsNcoll_Stat->SetTitle(ptBinTitle);
    vRAA_PtVsNcoll_Stat.push_back(gRAA_PtVsNcoll_Stat);
    
    TGraphErrors* gRAA_PtVsNcoll_Syst = new TGraphErrors(N_POINTS);
    gRAA_PtVsNcoll_Syst->SetName(Form("gRAA_PtVsNcoll_Syst_ptBin%d", i));
    gRAA_PtVsNcoll_Syst->SetTitle(ptBinTitle);
    vRAA_PtVsNcoll_Syst.push_back(gRAA_PtVsNcoll_Syst);
    
    TGraphErrors* gRAA_PtVsNcoll_Norm = new TGraphErrors(N_POINTS);
    gRAA_PtVsNcoll_Norm->SetName(Form("gRAA_PtVsNcoll_Norm_ptBin%d", i));
    gRAA_PtVsNcoll_Norm->SetTitle(ptBinTitle);
    vRAA_PtVsNcoll_Norm.push_back(gRAA_PtVsNcoll_Norm);
    
    TGraphErrors* gRAA_PtVsNcoll_TotUncert = new TGraphErrors(N_POINTS);
    gRAA_PtVsNcoll_TotUncert->SetName(Form("gRAA_PtVsNcoll_TotUncert_ptBin%d", i));
    gRAA_PtVsNcoll_TotUncert->SetTitle(ptBinTitle);
    vRAA_PtVsNcoll_TotUncert.push_back(gRAA_PtVsNcoll_TotUncert);
    
    // N_part
    TGraphErrors* gRAA_PtVsNpart = new TGraphErrors(N_POINTS);
    gRAA_PtVsNpart->SetName(Form("gRAA_PtVsNpart_ptBin%d", i));
    gRAA_PtVsNpart->SetTitle(ptBinTitle);
    vRAA_PtVsNpart.push_back(gRAA_PtVsNpart);
    
    TGraphErrors* gRAA_PtVsNpart_Stat = new TGraphErrors(N_POINTS);
    gRAA_PtVsNpart_Stat->SetName(Form("gRAA_PtVsNpart_Stat_ptBin%d", i));
    gRAA_PtVsNpart_Stat->SetTitle(ptBinTitle);
    vRAA_PtVsNpart_Stat.push_back(gRAA_PtVsNpart_Stat);
    
    TGraphErrors* gRAA_PtVsNpart_Syst = new TGraphErrors(N_POINTS);
    gRAA_PtVsNpart_Syst->SetName(Form("gRAA_PtVsNpart_Syst_ptBin%d", i));
    gRAA_PtVsNpart_Syst->SetTitle(ptBinTitle);
    vRAA_PtVsNpart_Syst.push_back(gRAA_PtVsNpart_Syst);
    
    TGraphErrors* gRAA_PtVsNpart_Norm = new TGraphErrors(N_POINTS);
    gRAA_PtVsNpart_Norm->SetName(Form("gRAA_PtVsNpart_Norm_ptBin%d", i));
    gRAA_PtVsNpart_Norm->SetTitle(ptBinTitle);
    vRAA_PtVsNpart_Norm.push_back(gRAA_PtVsNpart_Norm);
    
    TGraphErrors* gRAA_PtVsNpart_TotUncert = new TGraphErrors(N_POINTS);
    gRAA_PtVsNpart_TotUncert->SetName(Form("gRAA_PtVsNpart_TotUncert_ptBin%d", i));
    gRAA_PtVsNpart_TotUncert->SetTitle(ptBinTitle);
    vRAA_PtVsNpart_TotUncert.push_back(gRAA_PtVsNpart_TotUncert);
  }
  for (int i = 0; i < ptBins.size()-1; i++) {
    vRAA_PtVsNcoll[i]->SetPoint(0, 0, -1);
    vRAA_PtVsNcoll_Stat[i]->SetPoint(0, 0, -1);
    vRAA_PtVsNcoll_Syst[i]->SetPoint(0, 0, -1);
    vRAA_PtVsNcoll_Norm[i]->SetPoint(0, 0, -1);
    vRAA_PtVsNcoll_TotUncert[i]->SetPoint(0, 0, -1);
    FillBinsBySpecies( // OO
      vRAA_PtVsNcoll[i],
      vRAA_PtVsNcoll_Stat[i],
      vRAA_PtVsNcoll_Syst[i],
      vRAA_PtVsNcoll_Norm[i],
      vRAA_PtVsNcoll_TotUncert[i],
      hRAA_OO,
      hRAA_OO_Stat,
      hRAA_OO_Syst,
      hRAA_OO_Norm,
      hRAA_OO_TotUncert,
      ptBins[i],
      ptBins[i+1],
      1, Ncoll_OO
    );
    FillBinsBySpecies( // NeNe
      vRAA_PtVsNcoll[i],
      vRAA_PtVsNcoll_Stat[i],
      vRAA_PtVsNcoll_Syst[i],
      vRAA_PtVsNcoll_Norm[i],
      vRAA_PtVsNcoll_TotUncert[i],
      hRAA_NeNe,
      hRAA_NeNe_Stat,
      hRAA_NeNe_Syst,
      hRAA_NeNe_Norm,
      hRAA_NeNe_TotUncert,
      ptBins[i],
      ptBins[i+1],
      2, Ncoll_NeNe
    );
    FillBinsBySpecies( // XeXe
      vRAA_PtVsNcoll[i],
      vRAA_PtVsNcoll_Stat[i],
      vRAA_PtVsNcoll_Syst[i],
      vRAA_PtVsNcoll_Norm[i],
      vRAA_PtVsNcoll_TotUncert[i],
      hRAA_XeXe,
      hRAA_XeXe_Stat,
      hRAA_XeXe_Syst,
      hRAA_XeXe_Norm,
      hRAA_XeXe_TotUncert,
      ptBins[i],
      ptBins[i+1],
      3, Ncoll_XeXe
    );
    FillBinsBySpecies( // PbPb
      vRAA_PtVsNcoll[i],
      vRAA_PtVsNcoll_Stat[i],
      vRAA_PtVsNcoll_Syst[i],
      vRAA_PtVsNcoll_Norm[i],
      vRAA_PtVsNcoll_TotUncert[i],
      hRAA_PbPb,
      hRAA_PbPb_Stat,
      hRAA_PbPb_Syst,
      hRAA_PbPb_Norm,
      hRAA_PbPb_TotUncert,
      ptBins[i],
      ptBins[i+1],
      4, Ncoll_PbPb
    );
    FillBinsBySpecies( // pPb
      vRAA_PtVsNcoll[i],
      vRAA_PtVsNcoll_Stat[i],
      vRAA_PtVsNcoll_Syst[i],
      vRAA_PtVsNcoll_Norm[i],
      vRAA_PtVsNcoll_TotUncert[i],
      hRAA_pPb,
      hRAA_pPb_Stat,
      hRAA_pPb_Syst,
      hRAA_pPb_Norm,
      hRAA_pPb_TotUncert,
      ptBins[i],
      ptBins[i+1],
      5, Ncoll_pPb
    );
  }
  // Save TGraphs to file
  TFile* fout_Ncoll = new TFile("ptBinned_RAAVsNcoll.root", "RECREATE");
  fout_Ncoll->cd();
  for (int i = 0; i < vRAA_PtVsNcoll.size(); i++) {
    vRAA_PtVsNcoll[i]->Write();
    vRAA_PtVsNcoll_Stat[i]->Write();
    vRAA_PtVsNcoll_Syst[i]->Write();
    vRAA_PtVsNcoll_Norm[i]->Write();
    vRAA_PtVsNcoll_TotUncert[i]->Write();
  }
  cout << "Made ptBinned_RAAVsNcoll.root" << endl;
  fout_Ncoll->Close();
  
  for (int i = 0; i < ptBins.size()-1; i++) {
    vRAA_PtVsNpart[i]->SetPoint(0, 0, -1);
    vRAA_PtVsNpart_Stat[i]->SetPoint(0, 0, -1);
    vRAA_PtVsNpart_Syst[i]->SetPoint(0, 0, -1);
    vRAA_PtVsNpart_Norm[i]->SetPoint(0, 0, -1);
    vRAA_PtVsNpart_TotUncert[i]->SetPoint(0, 0, -1);
    FillBinsBySpecies( // OO
      vRAA_PtVsNpart[i],
      vRAA_PtVsNpart_Stat[i],
      vRAA_PtVsNpart_Syst[i],
      vRAA_PtVsNpart_Norm[i],
      vRAA_PtVsNpart_TotUncert[i],
      hRAA_OO,
      hRAA_OO_Stat,
      hRAA_OO_Syst,
      hRAA_OO_Norm,
      hRAA_OO_TotUncert,
      ptBins[i],
      ptBins[i+1],
      1, Npart_OO
    );
    FillBinsBySpecies( // NeNe
      vRAA_PtVsNpart[i],
      vRAA_PtVsNpart_Stat[i],
      vRAA_PtVsNpart_Syst[i],
      vRAA_PtVsNpart_Norm[i],
      vRAA_PtVsNpart_TotUncert[i],
      hRAA_NeNe,
      hRAA_NeNe_Stat,
      hRAA_NeNe_Syst,
      hRAA_NeNe_Norm,
      hRAA_NeNe_TotUncert,
      ptBins[i],
      ptBins[i+1],
      2, Npart_NeNe
    );
    FillBinsBySpecies( // XeXe
      vRAA_PtVsNpart[i],
      vRAA_PtVsNpart_Stat[i],
      vRAA_PtVsNpart_Syst[i],
      vRAA_PtVsNpart_Norm[i],
      vRAA_PtVsNpart_TotUncert[i],
      hRAA_XeXe,
      hRAA_XeXe_Stat,
      hRAA_XeXe_Syst,
      hRAA_XeXe_Norm,
      hRAA_XeXe_TotUncert,
      ptBins[i],
      ptBins[i+1],
      3, Npart_XeXe
    );
    FillBinsBySpecies( // PbPb
      vRAA_PtVsNpart[i],
      vRAA_PtVsNpart_Stat[i],
      vRAA_PtVsNpart_Syst[i],
      vRAA_PtVsNpart_Norm[i],
      vRAA_PtVsNpart_TotUncert[i],
      hRAA_PbPb,
      hRAA_PbPb_Stat,
      hRAA_PbPb_Syst,
      hRAA_PbPb_Norm,
      hRAA_PbPb_TotUncert,
      ptBins[i],
      ptBins[i+1],
      4, Npart_PbPb
    );
    FillBinsBySpecies( // pPb
      vRAA_PtVsNpart[i],
      vRAA_PtVsNpart_Stat[i],
      vRAA_PtVsNpart_Syst[i],
      vRAA_PtVsNpart_Norm[i],
      vRAA_PtVsNpart_TotUncert[i],
      hRAA_pPb,
      hRAA_pPb_Stat,
      hRAA_pPb_Syst,
      hRAA_pPb_Norm,
      hRAA_pPb_TotUncert,
      ptBins[i],
      ptBins[i+1],
      5, Npart_pPb
    );
  }
  // Save TGraphs to file
  TFile* fout_Npart = new TFile("ptBinned_RAAVsNpart.root", "RECREATE");
  fout_Npart->cd();
  for (int i = 0; i < vRAA_PtVsNpart.size(); i++) {
    vRAA_PtVsNpart[i]->Write();
    vRAA_PtVsNpart_Stat[i]->Write();
    vRAA_PtVsNpart_Syst[i]->Write();
    vRAA_PtVsNpart_Norm[i]->Write();
    vRAA_PtVsNpart_TotUncert[i]->Write();
  }
  cout << "Made ptBinned_RAAVsNpart.root" << endl;
  fout_Npart->Close();
}

void PlotMergeChecks(
  TH1F* hOriginal,
  TH1F* hRebinned,
  TString title,
  TString output,
  bool doLogx = true,
  bool doLogy = false
) {
  hRebinned->SetTitle(title);
  
  TCanvas* canvas = new TCanvas("canvas", "", 600, 600);
  TPad* pad = (TPad*) canvas->GetPad(0);
  TLegend* legend = new TLegend(0.19, 0.18, 0.5, 0.26);
  legend->SetTextSize(0.028);
  legend->SetFillStyle(0);
  legend->SetBorderSize(0);
  pad->cd();
  SetTDRStyle();
  pad->SetMargin(0.15, 0.05, 0.13, 0.07);
  
  float yMin = 0;
  float yMax = 1.25 * hRebinned->GetMaximum();
  if (doLogx) gPad->SetLogx(1);
  if (doLogy) {
    gPad->SetLogy(1);
    yMin = 0.001;
    yMax = yMax / 1.25 * 10.;
  }
  
  hRebinned->SetLineColor(kBlack);
  hRebinned->SetLineWidth(2);
  hRebinned->Draw("hist");
  if (hOriginal != nullptr) {
    hOriginal->SetLineColor(kRed);
    hOriginal->SetLineStyle(2);
    hOriginal->SetLineWidth(2);
    hOriginal->Draw("same hist");
  }
  if (!doLogy) hRebinned->SetMinimum(yMin);
  hRebinned->SetMaximum(yMax);
  pad->Update();
  canvas->SaveAs(output);
  gPad->SetLogx(0);
  gPad->SetLogy(0);
  delete canvas;
  delete legend;
}


void PlotMergeChecksRel(
  TH1F* hOriginal,
  TH1F* hOriginalErr,
  TH1F* hRebinned,
  TH1F* hRebinnedErr,
  TString title,
  TString output,
  bool doLogx = true,
  bool doLogy = false,
  bool normByPt = false
) {
  TH1F* hRebinnedRel = (TH1F*) hRebinnedErr->Clone("hRebinnedRel");
  hRebinnedRel->Divide(hRebinned);
  hRebinnedRel->Scale(100.);
  hRebinnedRel->SetTitle(title);
  
  TCanvas* canvas = new TCanvas("canvas", "", 600, 600);
  TPad* pad = (TPad*) canvas->GetPad(0);
  TLegend* legend = new TLegend(0.19, 0.18, 0.5, 0.26);
  legend->SetTextSize(0.028);
  legend->SetFillStyle(0);
  legend->SetBorderSize(0);
  pad->cd();
  SetTDRStyle();
  pad->SetMargin(0.15, 0.05, 0.13, 0.07);
  
  float yMin = 0;
  float yMax = 1.25 * hRebinnedRel->GetMaximum();
  if (doLogx) gPad->SetLogx(1);
  if (doLogy) {
    gPad->SetLogy(1);
    yMin = 0.001;
    yMax = yMax / 1.25 * 10.;
  }
  if (normByPt) {
    for (int i = 1; i <= hRebinnedRel->GetNbinsX(); i++) {
      hRebinnedRel->SetBinContent(i, 
        hRebinnedRel->GetBinContent(i) /
        pow(hRebinnedRel->GetBinCenter(i), 2)
      );
    }
  }
  TH1F* hOriginalRel;
  if (hOriginal != nullptr && hOriginalErr != nullptr) {
    hOriginalRel = (TH1F*) hOriginalErr->Clone("hOriginalRel");
    hOriginalRel->Divide(hOriginal);
    if (normByPt) {
      for (int i = 1; i <= hOriginalRel->GetNbinsX(); i++) {
        hOriginalRel->SetBinContent(i,
          hOriginalRel->GetBinContent(i) /
          pow(hOriginalRel->GetBinCenter(i), 2)
        );
      }
    }
  }
  
  hRebinnedRel->SetLineColor(kBlack);
  hRebinnedRel->SetLineWidth(2);
  hRebinnedRel->Draw("hist");
  if (hOriginal != nullptr && hOriginalErr != nullptr) {
    hOriginalRel->Scale(100.);
    hOriginalRel->SetLineColor(kRed);
    hOriginalRel->SetLineStyle(2);
    hOriginalRel->SetLineWidth(2);
    hOriginalRel->Draw("same hist");
  }
  if (!doLogy) hRebinnedRel->SetMinimum(yMin);
  hRebinnedRel->SetMaximum(yMax);
  pad->Update();
  canvas->SaveAs(output);
  gPad->SetLogx(0);
  gPad->SetLogy(0);
  delete canvas;
  delete legend;
}

vector<TH1F*> MergeRAABins(
  TH1F* hAA,
  TH1F* hAA_Stat,
  TH1F* hAA_Syst,
  TH1F* hRAA_TAA,
  TH1F* hRAA_Lumi,
  TH1F* hpp,
  TH1F* hpp_Stat,
  TH1F* hpp_Syst,
  vector<float> merged_bins,
  TString species = "AA",
  TH1F* hAARAA = nullptr,
  TH1F* hAARAA_Stat = nullptr,
  TH1F* hAARAA_Syst = nullptr,
  float tolerance = 0.001
) {
  float* merged_bins_arr = &merged_bins[0];
  // AA rebin
  TH1F* hAA_Rebin = new TH1F(
    "hAA_Rebin",
    "hAA_Rebin; PT [GEV]; YIELD",
    merged_bins.size() - 1,
    merged_bins_arr
  );
  TH1F* hAA_Rebin_Stat = new TH1F(
    "hAA_Rebin_Stat",
    "hAA_Rebin_Stat; PT [GEV]; stat",
    merged_bins.size() - 1,
    merged_bins_arr
  );
  TH1F* hAA_Rebin_Syst = new TH1F(
    "hAA_Rebin_Syst",
    "hAA_Rebin_Syst; PT [GEV]; sys",
    merged_bins.size() - 1,
    merged_bins_arr
  );
  // pp rebin
  TH1F* hpp_Rebin = new TH1F(
    "hpp_Rebin",
    "hpp_Rebin; PT [GEV]; YIELD",
    merged_bins.size() - 1,
    merged_bins_arr
  );
  TH1F* hpp_Rebin_Stat = new TH1F(
    "hpp_Rebin_Stat",
    "hpp_Rebin_Stat; PT [GEV]; stat",
    merged_bins.size() - 1,
    merged_bins_arr
  );
  TH1F* hpp_Rebin_Syst = new TH1F(
    "hpp_Rebin_Syst",
    "hpp_Rebin_Syst; PT [GEV]; sys",
    merged_bins.size() - 1,
    merged_bins_arr
  );
  // AA RAA rebin
  TH1F* hAARAA_Rebin = new TH1F(
    "h" + species + "RAA_Rebin",
    "h" + species + "RAA_Rebin; PT [GEV]; YIELD",
    merged_bins.size() - 1,
    merged_bins_arr
  );
  TH1F* hAARAA_Rebin_Stat = new TH1F(
    "h" + species + "RAA_Rebin_Stat",
    "h" + species + "RAA_Rebin_Stat; PT [GEV]; STAT",
    merged_bins.size() - 1,
    merged_bins_arr
  );
  TH1F* hAARAA_Rebin_Syst = new TH1F(
    "h" + species + "RAA_Rebin_Syst",
    "h" + species + "RAA_Rebin_Syst; PT [GEV]; SYST",
    merged_bins.size() - 1,
    merged_bins_arr
  );
  TH1F* hAARAA_Rebin_Norm = new TH1F(
    "h" + species + "RAA_Rebin_Norm",
    "h" + species + "RAA_Rebin_Norm; PT [GEV]; NORM",
    merged_bins.size() - 1,
    merged_bins_arr
  );
  // Rebin AA yield
  for (int i=1; i <= hAA_Rebin->GetNbinsX(); i++) {
    float binValSum     = 0;
    float binStatSum    = 0;
    float binSystSum    = 0;
    float binNormSum    = 0;
    float binWidthSum   = 0;
    for (int j=1; j <= hAA->GetNbinsX(); j++) {
      if (hAA->GetBinLowEdge(j) < hAA_Rebin->GetBinLowEdge(i) - tolerance ||
          hAA->GetBinLowEdge(j+1) > hAA_Rebin->GetBinLowEdge(i+1) + tolerance
        ) continue;
      float binCenter = hAA->GetBinCenter(j);
      float binWidth = hAA->GetBinWidth(j);
      binValSum += hAA->GetBinContent(j) * binCenter * binWidth;
      binStatSum += pow(hAA_Stat->GetBinContent(j) * binCenter * binWidth, 2);
      binSystSum += hAA_Syst->GetBinContent(j) * binCenter * binWidth;
      binWidthSum += hAA->GetBinWidth(j);
      binNormSum += sqrt(pow(hRAA_TAA->GetBinContent(j), 2) +
        pow(hRAA_Lumi->GetBinContent(j), 2)) * binCenter * binWidth;
    }
    hAA_Rebin->SetBinContent(i, binValSum / binWidthSum / hAA_Rebin->GetBinCenter(i) );
    hAA_Rebin_Stat->SetBinContent(i, sqrt(binStatSum) / binWidthSum / hAA_Rebin->GetBinCenter(i) );
    hAA_Rebin_Syst->SetBinContent(i, binSystSum / binWidthSum / hAA_Rebin->GetBinCenter(i) );
    hAARAA_Rebin_Norm->SetBinContent(i, binNormSum / binWidthSum / hAA_Rebin->GetBinCenter(i) );
  }
  // Rebin pp yield
  for (int i=1; i <= hpp_Rebin->GetNbinsX(); i++) {
    float binValSum     = 0;
    float binStatSum    = 0;
    float binSystSum    = 0;
    float binWidthSum   = 0;
    for (int j=1; j <= hpp->GetNbinsX(); j++) {
      if (hpp->GetBinLowEdge(j) < hpp_Rebin->GetBinLowEdge(i) - tolerance ||
          hpp->GetBinLowEdge(j+1) > hpp_Rebin->GetBinLowEdge(i+1) + tolerance
        ) continue;
      float binCenter = hpp->GetBinCenter(j);
      float binWidth = hpp->GetBinWidth(j);
      binValSum += hpp->GetBinContent(j) * binCenter * binWidth;
      binStatSum += pow(hpp_Stat->GetBinContent(j) * binCenter * binWidth, 2);
      binSystSum += hpp_Syst->GetBinContent(j) * binCenter * binWidth;
      binWidthSum += hpp->GetBinWidth(j);
    }
    hpp_Rebin->SetBinContent(i, binValSum / binWidthSum / hpp_Rebin->GetBinCenter(i) );
    hpp_Rebin_Stat->SetBinContent(i, sqrt(binStatSum) / binWidthSum / hpp_Rebin->GetBinCenter(i) );
    hpp_Rebin_Syst->SetBinContent(i, binSystSum / binWidthSum / hpp_Rebin->GetBinCenter(i) );
  }
  
  // Recalculate AA RAA
  for (int i=1; i <= hAARAA_Rebin->GetNbinsX(); i++) {
    hAARAA_Rebin->SetBinContent(i,
      hAA_Rebin->GetBinContent(i) / hpp_Rebin->GetBinContent(i)
    );
    hAARAA_Rebin_Stat->SetBinContent(i,
      sqrt(pow(hAA_Rebin_Stat->GetBinContent(i), 2) + pow(hpp_Rebin_Stat->GetBinContent(i), 2))
    );
    hAARAA_Rebin_Syst->SetBinContent(i,
      (hAA_Rebin_Syst->GetBinContent(i) + hpp_Rebin_Syst->GetBinContent(i))
    );
  }
  if (printMergeTable) {
    cout << "\\multicolumn{3}{|c|}{ PbPb $R_{AA}$ } & \\multicolumn{3}{|c|}{ Original } & \\multicolumn{3}{|c|}{ Rebinned } \\\\" << endl;
    cout << "\\multicolumn{3}{|c|}{ $p_{T}$ (GeV) } & $R_{AA}$ & Stat & Syst & $R_{AA}$ & Stat & Syst \\\\" << endl;
  }
  for (int i=1; i <= hAA_Rebin->GetNbinsX(); i++) {
    for (int j=1; j <= hAA->GetNbinsX(); j++) {
      if (hAARAA->GetBinLowEdge(j) < hAARAA_Rebin->GetBinLowEdge(i) - tolerance ||
          hAARAA->GetBinLowEdge(j+1) > hAARAA_Rebin->GetBinLowEdge(i+1) + tolerance
        ) continue;
      if (printMergeTable) {
        cout << Form("%.1f & $< p_{T} <$ & %.1f & %.3f & $\\pm %.3f$ & $\\pm %.3f$ & %.3f & $\\pm %.3f$ & $\\pm %.3f$ \\\\",
          hAARAA->GetBinLowEdge(j),
          hAARAA->GetBinLowEdge(j+1),
          hAARAA->GetBinContent(j),
          hAARAA_Stat->GetBinContent(j),
          hAARAA_Syst->GetBinContent(j),
          hAARAA_Rebin->GetBinContent(i),
          hAARAA_Rebin_Stat->GetBinContent(i),
          hAARAA_Rebin_Syst->GetBinContent(i)
        ) << endl;
      }
    }
  }
  for (int i=1; i <= hpp_Rebin->GetNbinsX(); i++) {
    for (int j=1; j <= hpp->GetNbinsX(); j++) {
      if (hpp->GetBinLowEdge(j) < hpp_Rebin->GetBinLowEdge(i) - tolerance ||
          hpp->GetBinLowEdge(j+1) > hpp_Rebin->GetBinLowEdge(i+1) + tolerance
        ) continue;
      if (printMergeTable) {
        cout << Form("%.1f & $< p_{T} <$ & %.1f & %.3f & %.3f & %.3f & %.3f & %.3f & %.3f",
          hpp->GetBinLowEdge(j),
          hpp->GetBinLowEdge(j+1),
          hpp->GetBinContent(j),
          hpp_Stat->GetBinContent(j),
          hpp_Syst->GetBinContent(j),
          hpp_Rebin->GetBinContent(i),
          hpp_Rebin_Stat->GetBinContent(i),
          hpp_Rebin_Syst->GetBinContent(i)
        ) << endl;
      }
    }
  }
  if (plotBinMergeChecks) {
    // Plot checks
    system("mkdir -p checks");
    // Check RAA
    PlotMergeChecks(
      hAARAA, hAARAA_Rebin,
      species +" R_{AA}; p_{T} [GeV]; R_{AA}",
      "checks/hRAARebin_"+ species +".pdf",
      true, false
    );
    PlotMergeChecks(
      hAARAA_Stat, hAARAA_Rebin_Stat,
      species +" R_{AA}, stat. uncertainty; p_{T} [GeV]; #deltaR_{AA}, stat.",
      "checks/hRAARebin_Stat_"+ species +".pdf",
      true, false
    );
    PlotMergeChecks(
      hAARAA_Syst, hAARAA_Rebin_Syst,
      species +" R_{AA}, syst. uncertainty; p_{T} [GeV]; #deltaR_{AA}, syst.",
      "checks/hRAARebin_Syst_"+ species +".pdf",
      true, false
    );
    PlotMergeChecksRel(
      hAARAA, hAARAA_Stat, hAARAA_Rebin, hAARAA_Rebin_Stat,
      species +" R_{AA}, relative stat. uncert.; p_{T} [GeV]; #deltaR_{AA}, stat.",
      "checks/hRAARebin_Stat_"+ species +"_Rel.pdf",
      true, false
    );
    PlotMergeChecksRel(
      hAARAA, hAARAA_Syst, hAARAA_Rebin, hAARAA_Rebin_Syst,
      species +" R_{AA}, relative syst. uncert.; p_{T} [GeV]; #deltaR_{AA}, syst.",
      "checks/hRAARebin_Syst_"+ species +"_Rel.pdf",
      true, false
    );
    // Check AA cross-section
    PlotMergeChecks(
      hAA, hAA_Rebin,
      species +" #sigma_{AA}; p_{T} [GeV]; #sigma_{AA}",
      "checks/hAARebin_"+ species +".pdf",
      true, true
    );
    PlotMergeChecks(
      hAA_Stat, hAA_Rebin_Stat,
      species +" #sigma_{AA}, stat. uncertainty; p_{T} [GeV]; #delta#sigma_{AA}, stat.",
      "checks/hAARebin_Stat_"+ species +".pdf",
      true, true
    );
    PlotMergeChecks(
      hAA_Syst, hAA_Rebin_Syst,
      species +" #sigma_{AA}, syst. uncertainty; p_{T} [GeV]; #delta#sigma_{AA}, syst.",
      "checks/hAARebin_Syst_"+ species +".pdf",
      true, true
    );
    PlotMergeChecksRel(
      hAA, hAA_Stat, hAA_Rebin, hAA_Rebin_Stat,
      species +" #sigma_{AA}, relative stat. uncert.; p_{T} [GeV]; #delta#sigma_{AA}, stat.",
      "checks/hAARebin_Stat_"+ species +"_Rel.pdf",
      true, false
    );
    PlotMergeChecksRel(
      hAA, hAA_Syst, hAA_Rebin, hAA_Rebin_Syst,
      species +" #sigma_{AA}, relative syst. uncert.; p_{T} [GeV]; #delta#sigma_{AA}, syst.",
      "checks/hAARebin_Syst_"+ species +"_Rel.pdf",
      true, false
    );
    // Check pp cross-section
    PlotMergeChecks(
      hpp, hpp_Rebin,
      species +" #sigma_{pp}; p_{T} [GeV]; #sigma_{pp}",
      "checks/hppRebin_"+ species +".pdf",
      true, true
    );
    PlotMergeChecks(
      hpp_Stat, hpp_Rebin_Stat,
      species +" #sigma_{pp}, stat. uncertainty; p_{T} [GeV]; #delta#sigma_{pp}, stat.",
      "checks/hppRebin_Stat_"+ species +".pdf",
      true, true
    );
    PlotMergeChecks(
      hpp_Syst, hpp_Rebin_Syst,
      species +" #sigma_{pp}, syst. uncertainty; p_{T} [GeV]; #delta#sigma_{pp}, syst.",
      "checks/hppRebin_Syst_"+ species +".pdf",
      true, true
    );
    PlotMergeChecksRel(
      hpp, hpp_Stat, hpp_Rebin, hpp_Rebin_Stat,
      species +" #sigma_{pp}, relative stat. uncert.; p_{T} [GeV]; #delta#sigma_{pp}, stat.",
      "checks/hppRebin_Stat_"+ species +"_Rel.pdf",
      true, false
    );
    PlotMergeChecksRel(
      hpp, hpp_Syst, hpp_Rebin, hpp_Rebin_Syst,
      species +" #sigma_{pp}, relative syst. uncert.; p_{T} [GeV]; #delta#sigma_{pp}, syst.",
      "checks/hppRebin_Syst_"+ species +"_Rel.pdf",
      true, false
    );
  }

  delete hAA_Rebin;
  delete hAA_Rebin_Stat;
  delete hAA_Rebin_Syst;
  delete hpp_Rebin;
  delete hpp_Rebin_Stat;
  delete hpp_Rebin_Syst;
  
  vector<TH1F*> vAARAA_Rebin = {
    hAARAA_Rebin, hAARAA_Rebin_Stat, hAARAA_Rebin_Syst, hAARAA_Rebin_Norm
  };
  return vAARAA_Rebin;
}

vector<TH1F*> MergePbPbBins(
  TFile* fin_PbPb,
  vector<float> merged_bins
) {
  // PbPb RAA
  TDirectoryFile* tab15 = (TDirectoryFile*) fin_PbPb->Get("Table 15");
  TH1F* hRAA_PbPb      = (TH1F*) tab15->Get("Hist1D_y1");
  TH1F* hRAA_PbPb_Stat = (TH1F*) tab15->Get("Hist1D_y1_e1");
  TH1F* hRAA_PbPb_Syst = (TH1F*) tab15->Get("Hist1D_y1_e2");
  TH1F* hRAA_PbPb_TAA  = (TH1F*) tab15->Get("Hist1D_y1_e3minus");
  TH1F* hRAA_PbPb_Lumi = (TH1F*) tab15->Get("Hist1D_y1_e4");
  // pp yield
  TDirectoryFile* tab7 = (TDirectoryFile*) fin_PbPb->Get("Table 7");
  TH1F* hpp      = (TH1F*) tab7->Get("Hist1D_y1");
  TH1F* hpp_Stat = (TH1F*) tab7->Get("Hist1D_y1_e1");
  TH1F* hpp_Syst = (TH1F*) tab7->Get("Hist1D_y1_e2plus");
  // Build PbPb yield by multiplying RAA and pp
  TH1F* hPbPb      = (TH1F*) hRAA_PbPb->Clone("hPbPb");
  TH1F* hPbPb_Stat = (TH1F*) hRAA_PbPb_Stat->Clone("hPbPb_Stat");
  TH1F* hPbPb_Syst = (TH1F*) hRAA_PbPb_Syst->Clone("hPbPb_Syst");
  hPbPb->Clear();
  hPbPb_Stat->Clear();
  hPbPb_Syst->Clear();
  for (int i=1; i <= hPbPb->GetNbinsX(); i++) {
    for (int j=1; j <= hpp->GetNbinsX(); j++) {
      if (hPbPb->GetBinLowEdge(i) != hpp->GetBinLowEdge(j) ||
          hPbPb->GetBinLowEdge(i+1) != hpp->GetBinLowEdge(j+1)
        ) continue;
      // Multiply central values
      hPbPb->SetBinContent(i,
        hRAA_PbPb->GetBinContent(i) * hpp->GetBinContent(j)
      );
      // Stat=Quadrature: Subtract pp stat^2 from RAA stat^2
      hPbPb_Stat->SetBinContent(i,
        sqrt(pow(hRAA_PbPb_Stat->GetBinContent(i),2) - pow(hpp_Stat->GetBinContent(j),2))
      );
      // Syst=Linear: Subtract pp syst from RAA syst
      hPbPb_Syst->SetBinContent(i,
        hRAA_PbPb_Syst->GetBinContent(i) - hpp_Syst->GetBinContent(j)
      );
    }
  }
  // Merge bins to match XeXe binning
  vector<TH1F*> vPbPbRAA_Rebin = MergeRAABins(
    hPbPb,
    hPbPb_Stat,
    hPbPb_Syst,
    hRAA_PbPb_TAA,
    hRAA_PbPb_Lumi,
    hpp,
    hpp_Stat,
    hpp_Syst,
    merged_bins,
    "PbPb",
    hRAA_PbPb,
    hRAA_PbPb_Stat,
    hRAA_PbPb_Syst,
    0.001
  );
  
  
  
  // Cleanup
  tab15->Close();
  delete tab15;
  tab7->Close();
  delete tab7;
  return vPbPbRAA_Rebin;
}

vector<TH1F*> MergepPbBins(
  TFile* fin_pPb,
  vector<float> merged_bins
) {
  // pPb RAA
  TDirectoryFile* tab16 = (TDirectoryFile*) fin_pPb->Get("Table 16");
  TH1F* hRAA_pPb      = (TH1F*) tab16->Get("Hist1D_y1");
  TH1F* hRAA_pPb_Stat = (TH1F*) tab16->Get("Hist1D_y1_e1");
  TH1F* hRAA_pPb_Syst = (TH1F*) tab16->Get("Hist1D_y1_e2plus");
  TH1F* hRAA_pPb_TpA  = (TH1F*) tab16->Get("Hist1D_y1_e3plus");
  TH1F* hRAA_pPb_Lumi = (TH1F*) tab16->Get("Hist1D_y1_e4");
  // pp yield
  TDirectoryFile* tab7 = (TDirectoryFile*) fin_pPb->Get("Table 7");
  TH1F* hpp      = (TH1F*) tab7->Get("Hist1D_y1");
  TH1F* hpp_Stat = (TH1F*) tab7->Get("Hist1D_y1_e1");
  TH1F* hpp_Syst = (TH1F*) tab7->Get("Hist1D_y1_e2plus");
  // Build pPb yield by multiplying RAA and pp
  TH1F* hpPb      = (TH1F*) hRAA_pPb->Clone("hpPb");
  TH1F* hpPb_Stat = (TH1F*) hRAA_pPb_Stat->Clone("hpPb_Stat");
  TH1F* hpPb_Syst = (TH1F*) hRAA_pPb_Syst->Clone("hpPb_Syst");
  hpPb->Clear();
  hpPb_Stat->Clear();
  hpPb_Syst->Clear();
  for (int i=1; i <= hpPb->GetNbinsX(); i++) {
    for (int j=1; j <= hpp->GetNbinsX(); j++) {
      if (hpPb->GetBinLowEdge(i) != hpp->GetBinLowEdge(j) ||
          hpPb->GetBinLowEdge(i+1) != hpp->GetBinLowEdge(j+1)
        ) continue;
      // Multiply central values
      hpPb->SetBinContent(i,
        hRAA_pPb->GetBinContent(i) * hpp->GetBinContent(j)
      );
      // Stat=Quadrature: Subtract pp stat^2 from RAA stat^2
      hpPb_Stat->SetBinContent(i,
        sqrt(pow(hRAA_pPb_Stat->GetBinContent(i),2) - pow(hpp_Stat->GetBinContent(j),2))
      );
      // Syst=Linear: Subtract pp syst from RAA syst
      hpPb_Syst->SetBinContent(i,
        hRAA_pPb_Syst->GetBinContent(i) - hpp_Syst->GetBinContent(j)
      );
    }
  }
  // Merge bins to match XeXe binning
  vector<TH1F*> vpPbRAA_Rebin = MergeRAABins(
    hpPb,
    hpPb_Stat,
    hpPb_Syst,
    hRAA_pPb_TpA,
    hRAA_pPb_Lumi,
    hpp,
    hpp_Stat,
    hpp_Syst,
    merged_bins,
    "pPb",
    hRAA_pPb,
    hRAA_pPb_Stat,
    hRAA_pPb_Syst,
    0.001
  );
  
  // Cleanup
  tab16->Close();
  delete tab16;
  tab7->Close();
  delete tab7;
  return vpPbRAA_Rebin;
}

void MakeGraphs_RebinRAA(
  TString finpathOO = "Results/pp_OO_raa_XeXe_binning.root",
  TString finpathNeNe = "Results/NeNeOverPPRatioCoarserBinning.root",
  TString finpathXeXe = "Results/HEPData-ins1692558-v1-root.root",
  TString finpathPbPb = "Results/HEPData-ins1496050-v2-root.root",
  TString finpathpPb = "Results/HEPData-ins1496050-v2-root.root"
) {
  // Define Limited XeXe Binning
  vector<float> XeXeBins = {
     3.2,  4.0,  4.8,  5.6,  6.4,
     7.2,  9.6, 12.0, 14.4, 19.2,
    24.0, 28.8, 35.2, 48.0, 73.6,
    103.6
  };
  
  // Get OO RAA
  TFile* fin_OO = new TFile(finpathOO,   "READ");
  TH1F* hRAA_OO           = (TH1F*) fin_OO->Get("RAA_XeXe_binning");
  TH1F* hRAA_OO_Stat_Raw  = (TH1F*) fin_OO->Get("RAA_XeXe_binning");
  TH1F* hRAA_OO_Syst_Raw  = (TH1F*) fin_OO->Get("Systematics_total_raa");
  TH1F* hRAA_OO_Stat      = (TH1F*) hRAA_OO->Clone("hRAA_OO_Stat");
  hRAA_OO_Stat->Clear();
  TH1F* hRAA_OO_Syst      = (TH1F*) hRAA_OO->Clone("hRAA_OO_Syst");
  hRAA_OO_Syst->Clear();
  TH1F* hRAA_OO_Norm      = (TH1F*) hRAA_OO->Clone("hRAA_OO_Norm");
  hRAA_OO_Norm->Clear();
  TH1F* hRAA_OO_TotUncert = (TH1F*) hRAA_OO->Clone("hRAA_OO_TotUncert");
  hRAA_OO_TotUncert->Clear();
  for (int i=1; i <= hRAA_OO->GetNbinsX(); i++) {
    hRAA_OO_Stat->SetBinContent(i,
      hRAA_OO_Stat_Raw->GetBinError(i)
    );
    hRAA_OO_Syst->SetBinContent(i,
      hRAA_OO_Syst_Raw->GetBinContent(i) * hRAA_OO->GetBinContent(i) / 100.
    );
    hRAA_OO_Norm->SetBinContent(i,
      hRAA_OO->GetBinContent(i) * 0.05
    );
    hRAA_OO_TotUncert->SetBinContent(i, sqrt(
      pow(hRAA_OO_Stat->GetBinContent(i), 2) +
      pow(hRAA_OO_Syst->GetBinContent(i), 2) +
      pow(hRAA_OO_Norm->GetBinContent(i), 2)
    ));
  }
  
  // Get NeNe RAA
  TFile* fin_NeNe = new TFile(finpathNeNe, "READ");
  TH1F* hRAA_NeNe           = (TH1F*) fin_NeNe->Get("normalized_RAA_NucleusNucleus");
  TH1F* hRAA_NeNe_Stat_Raw  = (TH1F*) fin_NeNe->Get("normalized_RAA_NucleusNucleus");
  TH1F* hRAA_NeNe_Syst_Raw  = (TH1F*) fin_NeNe->Get("Raa_Total_uncertainty_NucleusNucleus");
  TH1F* hRAA_NeNe_Stat      = (TH1F*) hRAA_NeNe->Clone("hRAA_NeNe_Stat");
  hRAA_NeNe_Stat->Clear();
  TH1F* hRAA_NeNe_Syst      = (TH1F*) hRAA_NeNe->Clone("hRAA_NeNe_Syst");
  hRAA_NeNe_Syst->Clear();
  TH1F* hRAA_NeNe_Norm      = (TH1F*) hRAA_NeNe->Clone("hRAA_NeNe_Norm");
  hRAA_NeNe_Norm->Clear();
  TH1F* hRAA_NeNe_TotUncert = (TH1F*) hRAA_NeNe->Clone("hRAA_NeNe_TotUncert");
  hRAA_NeNe_TotUncert->Clear();
  for (int i=1; i <= hRAA_NeNe->GetNbinsX(); i++) {
    hRAA_NeNe_Stat->SetBinContent(i,
      hRAA_NeNe_Stat_Raw->GetBinError(i)
    );
    hRAA_NeNe_Syst->SetBinContent(i,
      hRAA_NeNe_Syst_Raw->GetBinError(i)
    );
    hRAA_NeNe_Norm->SetBinContent(i,
      hRAA_NeNe->GetBinContent(i) * 0.0566
    );
    hRAA_NeNe_TotUncert->SetBinContent(i, sqrt(
      pow(hRAA_NeNe_Stat->GetBinContent(i), 2) +
      pow(hRAA_NeNe_Syst->GetBinContent(i), 2) +
      pow(hRAA_NeNe_Norm->GetBinContent(i), 2)
    ));
  }
  
  // Get XeXe RAA
  TFile* fin_XeXe = new TFile(finpathXeXe, "READ");
  TH1F* hRAA_XeXe           = (TH1F*) fin_XeXe->Get("Table 5/Hist1D_y1");
  TH1F* hRAA_XeXe_Stat      = (TH1F*) fin_XeXe->Get("Table 5/Hist1D_y1_e1");
  TH1F* hRAA_XeXe_Syst      = (TH1F*) fin_XeXe->Get("Table 5/Hist1D_y1_e2");
  TH1F* hRAA_XeXe_Norm      = (TH1F*) fin_XeXe->Get("Table 5/Hist1D_y1_e3");
  TH1F* hRAA_XeXe_TotUncert = (TH1F*) hRAA_XeXe->Clone("hRAA_XeXe_TotUncert");
  hRAA_XeXe_TotUncert->Clear();
  for (int i=1; i <= hRAA_XeXe_TotUncert->GetNbinsX(); i++) {
    hRAA_XeXe_TotUncert->SetBinContent(i, sqrt(
      pow(hRAA_XeXe_Stat->GetBinContent(i), 2) +
      pow(hRAA_XeXe_Syst->GetBinContent(i), 2) +
      pow(hRAA_XeXe_Norm->GetBinContent(i), 2)
    ));
  }
  
  // Get PbPb RAA
  TFile* fin_PbPb = new TFile(finpathPbPb, "READ");
  vector<TH1F*> vRAARebin_PbPb = MergePbPbBins(fin_PbPb, XeXeBins);
  TH1F* hRAARebin_PbPb      = vRAARebin_PbPb[0];
  TH1F* hRAARebin_PbPb_Stat = vRAARebin_PbPb[1];
  TH1F* hRAARebin_PbPb_Syst = vRAARebin_PbPb[2];
  TH1F* hRAARebin_PbPb_Norm = vRAARebin_PbPb[3];
  TH1F* hRAARebin_PbPb_TotUncert = (TH1F*) hRAARebin_PbPb->Clone("hRAARebin_PbPb_TotUncert");
  hRAARebin_PbPb_TotUncert->Clear();
  for (int i=1; i <= hRAARebin_PbPb_TotUncert->GetNbinsX(); i++) {
    hRAARebin_PbPb_TotUncert->SetBinContent(i, sqrt(
      pow(hRAARebin_PbPb_Stat->GetBinContent(i), 2) +
      pow(hRAARebin_PbPb_Syst->GetBinContent(i), 2) +
      pow(hRAARebin_PbPb_Norm->GetBinContent(i), 2)
    ));
  }
  
  // Get pPb RAA
  TFile* fin_pPb = new TFile(finpathpPb, "READ");
  vector<TH1F*> vRAARebin_pPb = MergepPbBins(fin_pPb, XeXeBins);
  TH1F* hRAARebin_pPb      = vRAARebin_pPb[0];
  TH1F* hRAARebin_pPb_Stat = vRAARebin_pPb[1];
  TH1F* hRAARebin_pPb_Syst = vRAARebin_pPb[2];
  TH1F* hRAARebin_pPb_Norm = vRAARebin_pPb[3];
  TH1F* hRAARebin_pPb_TotUncert = (TH1F*) hRAARebin_pPb->Clone("hRAARebin_pPb_TotUncert");
  hRAARebin_pPb_TotUncert->Clear();
  for (int i=1; i <= hRAARebin_pPb_TotUncert->GetNbinsX(); i++) {
    hRAARebin_pPb_TotUncert->SetBinContent(i, sqrt(
      pow(hRAARebin_pPb_Stat->GetBinContent(i), 2) +
      pow(hRAARebin_pPb_Syst->GetBinContent(i), 2) +
      pow(hRAARebin_pPb_Norm->GetBinContent(i), 2)
    ));
  }
  
  // Combine above into RAA pT vs A plot:
  MakePtBinnedGraphs(
    hRAA_OO,
    hRAA_OO_Stat,
    hRAA_OO_Syst,
    hRAA_OO_Norm,
    hRAA_OO_TotUncert,
    hRAA_NeNe,
    hRAA_NeNe_Stat,
    hRAA_NeNe_Syst,
    hRAA_NeNe_Norm,
    hRAA_NeNe_TotUncert,
    hRAA_XeXe,
    hRAA_XeXe_Stat,
    hRAA_XeXe_Syst,
    hRAA_XeXe_Norm,
    hRAA_XeXe_TotUncert,
    hRAARebin_PbPb,
    hRAARebin_PbPb_Stat,
    hRAARebin_PbPb_Syst,
    hRAARebin_PbPb_Norm,
    hRAARebin_PbPb_TotUncert,
    hRAARebin_pPb,
    hRAARebin_pPb_Stat,
    hRAARebin_pPb_Syst,
    hRAARebin_pPb_Norm,
    hRAARebin_pPb_TotUncert,
    XeXeBins
  );
  
  // Cleanup
  fin_OO->Close();
  fin_NeNe->Close();
  fin_XeXe->Close();
  fin_PbPb->Close();
  fin_pPb->Close();
  delete fin_OO;
  delete fin_NeNe;
  delete fin_XeXe;
  delete fin_PbPb;
  delete fin_pPb;
}
