#include "TROOT.h"
#include <vector>
#include <string>
#include <filesystem>

using namespace std;

TString finPath = "/data00/jdlang/UPCD0LowPtAnalysis/SkimsMC/20250924_Skim_2023MC_D0_Pthat0_BeamA.root";
//TString finPath = "/data00/jdlang/UPCD0LowPtAnalysis/SkimsData/20250926_Skim_2023Data_Feb2025ReReco_HIForward0.root";
const bool isMC = true;
const bool isGammaN = true;

const float DtrkPtMin = 0.2;
const float DyMin = -1.;
const float DyMax = 1.;
long maxEntries = 50000;
const float KMatchDtrkPMax = 0.9;
const float KMatchSigmaMax = 3.0;

const int purityMinBin = 11;
const int purityMaxBin = 30;

bool ApplyDCut(
  float Dtrk1Pt,
  float Dtrk1PtErr,
  int Dtrk1PixelHit,
  int Dtrk1StripHit,
  float Dtrk2Pt,
  float Dtrk2PtErr,
  int Dtrk2PixelHit,
  int Dtrk2StripHit,
  float DtrkPtRelErrMax = 0.1,
  int DtrkHitMin = 11
) {
  if ((Dtrk1PtErr / Dtrk1Pt < DtrkPtRelErrMax) &&
      (Dtrk2PtErr / Dtrk2Pt < DtrkPtRelErrMax)
//      &&
//      (Dtrk1PixelHit + Dtrk1StripHit >= DtrkHitMin) &&
//      (Dtrk2PixelHit + Dtrk2StripHit >= DtrkHitMin)
    ) return true;
  else return false;
}

bool ApplyPASCut(
  float Dchi2cl,
  float Dalpha,
  float Ddtheta,
  float DsvpvDistance,
  float DsvpvDisErr,
  float Dtrk1Pt,
  float Dtrk2Pt,
  float Dpt,
  float DtrkPtMin   = 0.2,
  float Dchi2clMin  = 0.1,
  float DalphaMax   = 0.4,
  float DdthetaMax  = 0.5,
  float DsvpvSigMin = 2.5
) {
//  if (Dchi2cl > Dchi2clMin &&
//      Dalpha < DalphaMax &&
//      Ddtheta < DdthetaMax &&
//      (DsvpvDistance / DsvpvDisErr) > DsvpvSigMin &&
//      Dtrk1Pt > DtrkPtMin &&
//      Dtrk2Pt > DtrkPtMin
//    ) return true;
  if (
      Dchi2cl > Dchi2clMin &&
      Dpt > 0. && Dpt < 1. &&
      Dalpha < 1.2 &&
      Ddtheta < 1.4 &&
      (DsvpvDistance / DsvpvDisErr) > DsvpvSigMin &&
      Dtrk1Pt > DtrkPtMin &&
      Dtrk2Pt > DtrkPtMin
    ) return true;
  else if (
      Dchi2cl > Dchi2clMin &&
      Dpt > 1. && Dpt < 2. &&
      Dalpha < 1.0 &&
      Ddtheta < 1.0 &&
      (DsvpvDistance / DsvpvDisErr) > DsvpvSigMin &&
      Dtrk1Pt > DtrkPtMin &&
      Dtrk2Pt > DtrkPtMin
    ) return true;
  else if (
      Dchi2cl > Dchi2clMin &&
      Dpt > 2. && Dpt < 5. &&
      Dalpha < 0.5 &&
      Ddtheta < DdthetaMax &&
      (DsvpvDistance / DsvpvDisErr) > DsvpvSigMin &&
      Dtrk1Pt > DtrkPtMin &&
      Dtrk2Pt > DtrkPtMin
    ) return true;
  else return false;
}

bool ApplyPIDCut(
  float Dtrk1P,
  float Dtrk1MassHypo,
  float Dtrk1PionScore,
  float Dtrk1KaonScore,
  float Dtrk1ProtScore,
  float Dtrk2P,
  float Dtrk2MassHypo,
  float Dtrk2PionScore,
  float Dtrk2KaonScore,
  float Dtrk2ProtScore,
  bool matchKaonOnly = true,
  bool doProtonReject = false,
  float pionScoreMin = -3,
  float pionScoreMax = 3,
  float kaonScoreMin = -KMatchSigmaMax,
  float kaonScoreMax = KMatchSigmaMax,
  float protScoreMin = -1.,
  float protScoreMax = 999.,
  float PionMaxP = 0.85,
  float KaonMaxP = 1.0,
  float ProtMinP = 0.3,
  float ProtMaxP = 1.1
) {
  bool matchPion = false;
  bool matchKaon = false;
  bool matchProton = false;
  // match the kaon only
  if (matchKaonOnly) {
    if (Dtrk1MassHypo > 0.3) {
      matchKaon = (
        Dtrk1KaonScore > kaonScoreMin &&
        Dtrk1KaonScore < kaonScoreMax &&
        Dtrk1P < KaonMaxP) ||
        Dtrk1P > KaonMaxP;
    } else {
      matchKaon = (
        Dtrk2KaonScore > kaonScoreMin &&
        Dtrk2KaonScore < kaonScoreMax &&
        Dtrk2P < KaonMaxP) ||
        Dtrk2P > KaonMaxP;
    }
    matchPion = true;
  }
  // match kaon and pion
  else {
    if (Dtrk1MassHypo < 0.3) {
      matchPion = (
        Dtrk1PionScore > pionScoreMin &&
        Dtrk1PionScore < pionScoreMax &&
        Dtrk1P < PionMaxP); //||
//        Dtrk1P > PionMaxP;
      matchKaon = (
        Dtrk2KaonScore > kaonScoreMin &&
        Dtrk2KaonScore < kaonScoreMax &&
        Dtrk2P < KaonMaxP); //||
//        Dtrk2P > KaonMaxP;
    } else {
      matchPion = (
        Dtrk2PionScore > pionScoreMin &&
        Dtrk2PionScore < pionScoreMax &&
        Dtrk2P < PionMaxP); //||
//        Dtrk2P > PionMaxP;
      matchKaon = (
        Dtrk1KaonScore > kaonScoreMin &&
        Dtrk1KaonScore < kaonScoreMax &&
        Dtrk1P < KaonMaxP); //||
//        Dtrk1P > KaonMaxP;
    }
  }
  // proton rejection
  if (doProtonReject &&
      ((Dtrk1P > ProtMinP &&
        Dtrk1P < ProtMaxP &&
        Dtrk1ProtScore > protScoreMin &&
        Dtrk1ProtScore < protScoreMax) ||
       (Dtrk2P > ProtMinP &&
        Dtrk2P < ProtMaxP &&
        Dtrk2ProtScore > protScoreMin &&
        Dtrk2ProtScore < protScoreMax))
    ) matchProton = true;
  
  return (matchPion && matchKaon && !matchProton);
}

void DrawParamHist(
  TH1D* hist_0_1,
  TH1D* hist_1_2,
  TH1D* hist_2_5,
  TH1D* hist_swap_0_1,
  TH1D* hist_swap_1_2,
  TH1D* hist_swap_2_5,
  TH1D* hist_back_0_1,
  TH1D* hist_back_1_2,
  TH1D* hist_back_2_5,
  TString filename,
  bool doLogy = false
) {
  TCanvas* c3 = new TCanvas("c3", "", 1800, 600);
  gStyle->SetOptStat(0);
  c3->Divide(3, 1, 0.0001, 0.0001);
  
  float min_0_1 = 1 / (10 * hist_0_1->GetMaximum());
  float min_1_2 = 1 / (10 * hist_1_2->GetMaximum());
  float min_2_5 = 1 / (10 * hist_2_5->GetMaximum());
  
  hist_0_1->Scale(1./hist_0_1->Integral());
  hist_1_2->Scale(1./hist_1_2->Integral());
  hist_2_5->Scale(1./hist_2_5->Integral());
  hist_swap_0_1->Scale(hist_0_1->Integral()/hist_swap_0_1->Integral());
  hist_swap_1_2->Scale(hist_1_2->Integral()/hist_swap_1_2->Integral());
  hist_swap_2_5->Scale(hist_2_5->Integral()/hist_swap_2_5->Integral());
  hist_back_0_1->Scale(hist_0_1->Integral()/hist_back_0_1->Integral());
  hist_back_1_2->Scale(hist_1_2->Integral()/hist_back_1_2->Integral());
  hist_back_2_5->Scale(hist_2_5->Integral()/hist_back_2_5->Integral());
  
  TH1D* temp_0_1 = (TH1D*) hist_0_1->Clone("temp_0_1");
  TH1D* temp_1_2 = (TH1D*) hist_1_2->Clone("temp_1_2");
  TH1D* temp_2_5 = (TH1D*) hist_2_5->Clone("temp_2_5");
  temp_0_1->Reset();
  temp_1_2->Reset();
  temp_2_5->Reset();
  temp_0_1->GetYaxis()->SetTitle("Normalized entries");
  temp_1_2->GetYaxis()->SetTitle("Normalized entries");
  temp_2_5->GetYaxis()->SetTitle("Normalized entries");
  
  temp_0_1->SetMaximum(
    1.2 * fmax(
      hist_0_1->GetMaximum(),
      fmax(hist_swap_0_1->GetMaximum(), hist_back_0_1->GetMaximum())
    )
  );
  temp_1_2->SetMaximum(
    1.2 * fmax(
      hist_1_2->GetMaximum(),
      fmax(hist_swap_1_2->GetMaximum(), hist_back_1_2->GetMaximum())
    )
  );
  temp_2_5->SetMaximum(
    1.2 * fmax(
      hist_2_5->GetMaximum(),
      fmax(hist_swap_2_5->GetMaximum(), hist_back_2_5->GetMaximum())
    )
  );
  if (doLogy) {
    temp_0_1->SetMinimum(min_0_1);
    temp_1_2->SetMinimum(min_1_2);
    temp_2_5->SetMinimum(min_2_5);
  } else {
    temp_0_1->SetMinimum(1/hist_0_1->GetEntries());
    temp_1_2->SetMinimum(1/hist_1_2->GetEntries());
    temp_2_5->SetMinimum(1/hist_2_5->GetEntries());
  }
  
  hist_0_1->SetLineColor(kPink);
  hist_1_2->SetLineColor(kPink);
  hist_2_5->SetLineColor(kPink);
  
  hist_swap_0_1->SetLineColor(kAzure);
  hist_swap_1_2->SetLineColor(kAzure);
  hist_swap_2_5->SetLineColor(kAzure);
  
  hist_back_0_1->SetLineColor(kGray+1);
  hist_back_1_2->SetLineColor(kGray+1);
  hist_back_2_5->SetLineColor(kGray+1);
  
  c3->cd(1);
  if (doLogy) gPad->SetLogy(1);
  gPad->SetMargin(0.15, 0.05, 0.1, 0.1);
  temp_0_1->Draw();
  hist_back_0_1->Draw("same hist");
  hist_swap_0_1->Draw("same hist");
  hist_0_1->Draw("same hist");
  
  c3->cd(2);
  if (doLogy) gPad->SetLogy(1);
  gPad->SetMargin(0.15, 0.05, 0.1, 0.1);
  temp_1_2->Draw();
  hist_back_1_2->Draw("same hist");
  hist_swap_1_2->Draw("same hist");
  hist_1_2->Draw("same hist");
  
  c3->cd(3);
  if (doLogy) gPad->SetLogy(1);
  gPad->SetMargin(0.15, 0.05, 0.1, 0.1);
  temp_2_5->Draw();
  hist_back_2_5->Draw("same hist");
  hist_swap_2_5->Draw("same hist");
  hist_2_5->Draw("same hist");
  
  c3->cd();
  c3->Update();
  c3->SaveAs(filename);
  
  if (doLogy) gPad->SetLogy(0);
  
  delete c3;
}

void PlotDzeroPID() {
  TFile* fin = TFile::Open(finPath, "READ");
  TTree* tree = (TTree*) fin->Get("Tree");
  
  int Dsize;
  int nVtx;
  bool selectedBkgFilter;
  bool selectedVtxFilter;
  bool isL1ZDCOr;
  bool gapgammaN;
  bool gapNgamma;
  bool ZDCgammaN;
  bool ZDCNgamma;
  vector<bool>* gammaN = nullptr;
  vector<bool>* Ngamma = nullptr;
  vector<bool>* DpassCut23PAS = nullptr;
  vector<bool>* DpassCutNominal = nullptr;
  tree->SetBranchAddress("nVtx", &nVtx);
  tree->SetBranchAddress("Dsize", &Dsize);
  tree->SetBranchAddress("selectedBkgFilter", &selectedBkgFilter);
  tree->SetBranchAddress("selectedVtxFilter", &selectedVtxFilter);
  tree->SetBranchAddress("isL1ZDCOr", &isL1ZDCOr);
  tree->SetBranchAddress("gapgammaN", &gapgammaN);
  tree->SetBranchAddress("gapNgamma", &gapNgamma);
  tree->SetBranchAddress("ZDCgammaN", &ZDCgammaN);
  tree->SetBranchAddress("ZDCNgamma", &ZDCNgamma);
  tree->SetBranchAddress("gammaN", &gammaN);
  tree->SetBranchAddress("Ngamma", &Ngamma);
  tree->SetBranchAddress("DpassCut23PAS", &DpassCut23PAS);
  tree->SetBranchAddress("DpassCutNominal", &DpassCutNominal);
  
  vector<float>* Dpt = nullptr;
  vector<float>* Dy = nullptr;
  vector<float>* Dmass = nullptr;
  vector<float>* Dalpha = nullptr;
  vector<float>* Ddtheta = nullptr;
  vector<float>* Dchi2cl = nullptr;
  vector<float>* DsvpvDistance = nullptr;
  vector<float>* DsvpvDisErr = nullptr;
  vector<bool>* DisSignalCalc = nullptr;
  vector<int>* Dgen = nullptr;
  tree->SetBranchAddress("Dpt", &Dpt);
  tree->SetBranchAddress("Dy", &Dy);
  tree->SetBranchAddress("Dmass", &Dmass);
  tree->SetBranchAddress("Dalpha", &Dalpha);
  tree->SetBranchAddress("Ddtheta", &Ddtheta);
  tree->SetBranchAddress("Dchi2cl", &Dchi2cl);
  tree->SetBranchAddress("DsvpvDistance", &DsvpvDistance);
  tree->SetBranchAddress("DsvpvDisErr", &DsvpvDisErr);
  if (isMC) tree->SetBranchAddress("DisSignalCalc", &DisSignalCalc);
  if (isMC) tree->SetBranchAddress("Dgen", &Dgen);
  
  vector<float>* Dtrk1P = nullptr;
  vector<float>* Dtrk1Pt = nullptr;
  vector<float>* Dtrk1PtErr = nullptr;
  vector<float>* Dtrk1Eta = nullptr;
  vector<float>* Dtrk1dedx = nullptr;
  vector<float>* Dtrk1MassHypo = nullptr;
  vector<float>* Dtrk1PionScore = nullptr;
  vector<float>* Dtrk1KaonScore = nullptr;
  vector<float>* Dtrk1ProtScore = nullptr;
  vector<int>* Dtrk1PixelHit = nullptr;
  vector<int>* Dtrk1StripHit = nullptr;
  tree->SetBranchAddress("Dtrk1P", &Dtrk1P);
  tree->SetBranchAddress("Dtrk1Pt", &Dtrk1Pt);
  tree->SetBranchAddress("Dtrk1PtErr", &Dtrk1PtErr);
  tree->SetBranchAddress("Dtrk1Eta", &Dtrk1Eta);
  tree->SetBranchAddress("Dtrk1dedx", &Dtrk1dedx);
  tree->SetBranchAddress("Dtrk1MassHypo", &Dtrk1MassHypo);
  tree->SetBranchAddress("Dtrk1PionScore", &Dtrk1PionScore);
  tree->SetBranchAddress("Dtrk1KaonScore", &Dtrk1KaonScore);
  tree->SetBranchAddress("Dtrk1ProtScore", &Dtrk1ProtScore);
  tree->SetBranchAddress("Dtrk1PixelHit", &Dtrk1PixelHit);
  tree->SetBranchAddress("Dtrk1StripHit", &Dtrk1StripHit);
  
  vector<float>* Dtrk2P = nullptr;
  vector<float>* Dtrk2Pt = nullptr;
  vector<float>* Dtrk2PtErr = nullptr;
  vector<float>* Dtrk2Eta = nullptr;
  vector<float>* Dtrk2dedx = nullptr;
  vector<float>* Dtrk2MassHypo = nullptr;
  vector<float>* Dtrk2PionScore = nullptr;
  vector<float>* Dtrk2KaonScore = nullptr;
  vector<float>* Dtrk2ProtScore = nullptr;
  vector<int>* Dtrk2PixelHit = nullptr;
  vector<int>* Dtrk2StripHit = nullptr;
  tree->SetBranchAddress("Dtrk2P", &Dtrk2P);
  tree->SetBranchAddress("Dtrk2Pt", &Dtrk2Pt);
  tree->SetBranchAddress("Dtrk2PtErr", &Dtrk2PtErr);
  tree->SetBranchAddress("Dtrk2Eta", &Dtrk2Eta);
  tree->SetBranchAddress("Dtrk2dedx", &Dtrk2dedx);
  tree->SetBranchAddress("Dtrk2MassHypo", &Dtrk2MassHypo);
  tree->SetBranchAddress("Dtrk2PionScore", &Dtrk2PionScore);
  tree->SetBranchAddress("Dtrk2KaonScore", &Dtrk2KaonScore);
  tree->SetBranchAddress("Dtrk2ProtScore", &Dtrk2ProtScore);
  tree->SetBranchAddress("Dtrk2PixelHit", &Dtrk2PixelHit);
  tree->SetBranchAddress("Dtrk2StripHit", &Dtrk2StripHit);
  
  // Parameter spectra
  TString sDyBin = Form("%.f < Dy < %.f", DyMin, DyMax);
  
  TH1D* hDchi2cl = new TH1D(
    "hDchi2cl",
    "; Dchi2cl; Entries",
    50, 0., 1.
  );
  TH1D* hDchi2cl_Dpt0_1 = (TH1D*) hDchi2cl->Clone("hDchi2cl_Dpt0_1");
  hDchi2cl_Dpt0_1->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin);
  TH1D* hDchi2cl_Dpt1_2 = (TH1D*) hDchi2cl->Clone("hDchi2cl_Dpt1_2");
  hDchi2cl_Dpt1_2->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin);
  TH1D* hDchi2cl_Dpt2_5 = (TH1D*) hDchi2cl->Clone("hDchi2cl_Dpt2_5");
  hDchi2cl_Dpt2_5->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin);
  
  TH1D* hDchi2cl_swap_Dpt0_1 = (TH1D*) hDchi2cl->Clone("hDchi2cl_swap_Dpt0_1");
  hDchi2cl_swap_Dpt0_1->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin);
  TH1D* hDchi2cl_swap_Dpt1_2 = (TH1D*) hDchi2cl->Clone("hDchi2cl_swap_Dpt1_2");
  hDchi2cl_swap_Dpt1_2->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin);
  TH1D* hDchi2cl_swap_Dpt2_5 = (TH1D*) hDchi2cl->Clone("hDchi2cl_swap_Dpt2_5");
  hDchi2cl_swap_Dpt2_5->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin);
  
  TH1D* hDchi2cl_back_Dpt0_1 = (TH1D*) hDchi2cl->Clone("hDchi2cl_back_Dpt0_1");
  hDchi2cl_back_Dpt0_1->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin);
  TH1D* hDchi2cl_back_Dpt1_2 = (TH1D*) hDchi2cl->Clone("hDchi2cl_back_Dpt1_2");
  hDchi2cl_back_Dpt1_2->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin);
  TH1D* hDchi2cl_back_Dpt2_5 = (TH1D*) hDchi2cl->Clone("hDchi2cl_back_Dpt2_5");
  hDchi2cl_back_Dpt2_5->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin);
  
  TH1D* hDalpha = new TH1D(
    "hDalpha",
    "; Dalpha; Entries",
    32, 0., 3.2
  );
  TH1D* hDalpha_Dpt0_1 = (TH1D*) hDalpha->Clone("hDalpha_Dpt0_1");
  hDalpha_Dpt0_1->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin);
  TH1D* hDalpha_Dpt1_2 = (TH1D*) hDalpha->Clone("hDalpha_Dpt1_2");
  hDalpha_Dpt1_2->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin);
  TH1D* hDalpha_Dpt2_5 = (TH1D*) hDalpha->Clone("hDalpha_Dpt2_5");
  hDalpha_Dpt2_5->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin);
  
  TH1D* hDalpha_swap_Dpt0_1 = (TH1D*) hDalpha->Clone("hDalpha_swap_Dpt0_1");
  hDalpha_swap_Dpt0_1->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin);
  TH1D* hDalpha_swap_Dpt1_2 = (TH1D*) hDalpha->Clone("hDalpha_swap_Dpt1_2");
  hDalpha_swap_Dpt1_2->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin);
  TH1D* hDalpha_swap_Dpt2_5 = (TH1D*) hDalpha->Clone("hDalpha_swap_Dpt2_5");
  hDalpha_swap_Dpt2_5->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin);
  
  TH1D* hDalpha_back_Dpt0_1 = (TH1D*) hDalpha->Clone("hDalpha_back_Dpt0_1");
  hDalpha_back_Dpt0_1->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin);
  TH1D* hDalpha_back_Dpt1_2 = (TH1D*) hDalpha->Clone("hDalpha_back_Dpt1_2");
  hDalpha_back_Dpt1_2->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin);
  TH1D* hDalpha_back_Dpt2_5 = (TH1D*) hDalpha->Clone("hDalpha_back_Dpt2_5");
  hDalpha_back_Dpt2_5->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin);
  
  TH1D* hDdtheta = new TH1D(
    "hDdtheta",
    "; Ddtheta; Entries",
    32, 0., 3.2
  );
  TH1D* hDdtheta_Dpt0_1 = (TH1D*) hDdtheta->Clone("hDdtheta_Dpt0_1");
  hDdtheta_Dpt0_1->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin);
  TH1D* hDdtheta_Dpt1_2 = (TH1D*) hDdtheta->Clone("hDdtheta_Dpt1_2");
  hDdtheta_Dpt1_2->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin);
  TH1D* hDdtheta_Dpt2_5 = (TH1D*) hDdtheta->Clone("hDdtheta_Dpt2_5");
  hDdtheta_Dpt2_5->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin);
  
  TH1D* hDdtheta_swap_Dpt0_1 = (TH1D*) hDdtheta->Clone("hDdtheta_swap_Dpt0_1");
  hDdtheta_swap_Dpt0_1->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin);
  TH1D* hDdtheta_swap_Dpt1_2 = (TH1D*) hDdtheta->Clone("hDdtheta_swap_Dpt1_2");
  hDdtheta_swap_Dpt1_2->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin);
  TH1D* hDdtheta_swap_Dpt2_5 = (TH1D*) hDdtheta->Clone("hDdtheta_swap_Dpt2_5");
  hDdtheta_swap_Dpt2_5->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin);
  
  TH1D* hDdtheta_back_Dpt0_1 = (TH1D*) hDdtheta->Clone("hDdtheta_back_Dpt0_1");
  hDdtheta_back_Dpt0_1->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin);
  TH1D* hDdtheta_back_Dpt1_2 = (TH1D*) hDdtheta->Clone("hDdtheta_back_Dpt1_2");
  hDdtheta_back_Dpt1_2->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin);
  TH1D* hDdtheta_back_Dpt2_5 = (TH1D*) hDdtheta->Clone("hDdtheta_back_Dpt2_5");
  hDdtheta_back_Dpt2_5->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin);
  
  TH1D* hDsvpvSig = new TH1D(
    "hDsvpvSig",
    "; DsvpvDistance / DsvpvDisErr; Entries",
    40, 0., 20.
  );
  TH1D* hDsvpvSig_Dpt0_1 = (TH1D*) hDsvpvSig->Clone("hDsvpvSig_Dpt0_1");
  hDsvpvSig_Dpt0_1->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin);
  TH1D* hDsvpvSig_Dpt1_2 = (TH1D*) hDsvpvSig->Clone("hDsvpvSig_Dpt1_2");
  hDsvpvSig_Dpt1_2->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin);
  TH1D* hDsvpvSig_Dpt2_5 = (TH1D*) hDsvpvSig->Clone("hDsvpvSig_Dpt2_5");
  hDsvpvSig_Dpt2_5->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin);
  
  TH1D* hDsvpvSig_swap_Dpt0_1 = (TH1D*) hDsvpvSig->Clone("hDsvpvSig_swap_Dpt0_1");
  hDsvpvSig_swap_Dpt0_1->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin);
  TH1D* hDsvpvSig_swap_Dpt1_2 = (TH1D*) hDsvpvSig->Clone("hDsvpvSig_swap_Dpt1_2");
  hDsvpvSig_swap_Dpt1_2->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin);
  TH1D* hDsvpvSig_swap_Dpt2_5 = (TH1D*) hDsvpvSig->Clone("hDsvpvSig_swap_Dpt2_5");
  hDsvpvSig_swap_Dpt2_5->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin);
  
  TH1D* hDsvpvSig_back_Dpt0_1 = (TH1D*) hDsvpvSig->Clone("hDsvpvSig_back_Dpt0_1");
  hDsvpvSig_back_Dpt0_1->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin);
  TH1D* hDsvpvSig_back_Dpt1_2 = (TH1D*) hDsvpvSig->Clone("hDsvpvSig_back_Dpt1_2");
  hDsvpvSig_back_Dpt1_2->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin);
  TH1D* hDsvpvSig_back_Dpt2_5 = (TH1D*) hDsvpvSig->Clone("hDsvpvSig_back_Dpt2_5");
  hDsvpvSig_back_Dpt2_5->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin);
  
  TH1D* hDtrkP = new TH1D(
    "hDtrkP",
    "; Dtrk p [GeV]; Entries",
    40, 0., 8.
  );
  TH1D* hDtrkP_Dpt0_1 = (TH1D*) hDtrkP->Clone("hDtrkP_Dpt0_1");
  hDtrkP_Dpt0_1->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin);
  TH1D* hDtrkP_Dpt1_2 = (TH1D*) hDtrkP->Clone("hDtrkP_Dpt1_2");
  hDtrkP_Dpt1_2->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin);
  TH1D* hDtrkP_Dpt2_5 = (TH1D*) hDtrkP->Clone("hDtrkP_Dpt2_5");
  hDtrkP_Dpt2_5->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin);
  
  TH1D* hDtrkP_swap_Dpt0_1 = (TH1D*) hDtrkP->Clone("hDtrkP_swap_Dpt0_1");
  hDtrkP_swap_Dpt0_1->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin);
  TH1D* hDtrkP_swap_Dpt1_2 = (TH1D*) hDtrkP->Clone("hDtrkP_swap_Dpt1_2");
  hDtrkP_swap_Dpt1_2->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin);
  TH1D* hDtrkP_swap_Dpt2_5 = (TH1D*) hDtrkP->Clone("hDtrkP_swap_Dpt2_5");
  hDtrkP_swap_Dpt2_5->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin);
  
  TH1D* hDtrkP_back_Dpt0_1 = (TH1D*) hDtrkP->Clone("hDtrkP_back_Dpt0_1");
  hDtrkP_back_Dpt0_1->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin);
  TH1D* hDtrkP_back_Dpt1_2 = (TH1D*) hDtrkP->Clone("hDtrkP_back_Dpt1_2");
  hDtrkP_back_Dpt1_2->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin);
  TH1D* hDtrkP_back_Dpt2_5 = (TH1D*) hDtrkP->Clone("hDtrkP_back_Dpt2_5");
  hDtrkP_back_Dpt2_5->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin);
  
  TH1D* hDtrkPt = new TH1D(
    "hDtrkPt",
    "; Dtrk p_{T} [GeV]; Entries",
    50, 0., 5.
  );
  TH1D* hDtrkPt_Dpt0_1 = (TH1D*) hDtrkPt->Clone("hDtrkPt_Dpt0_1");
  hDtrkPt_Dpt0_1->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin);
  TH1D* hDtrkPt_Dpt1_2 = (TH1D*) hDtrkPt->Clone("hDtrkPt_Dpt1_2");
  hDtrkPt_Dpt1_2->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin);
  TH1D* hDtrkPt_Dpt2_5 = (TH1D*) hDtrkPt->Clone("hDtrkPt_Dpt2_5");
  hDtrkPt_Dpt2_5->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin);
  
  TH1D* hDtrkPt_swap_Dpt0_1 = (TH1D*) hDtrkPt->Clone("hDtrkPt_swap_Dpt0_1");
  hDtrkPt_swap_Dpt0_1->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin);
  TH1D* hDtrkPt_swap_Dpt1_2 = (TH1D*) hDtrkPt->Clone("hDtrkPt_swap_Dpt1_2");
  hDtrkPt_swap_Dpt1_2->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin);
  TH1D* hDtrkPt_swap_Dpt2_5 = (TH1D*) hDtrkPt->Clone("hDtrkPt_swap_Dpt2_5");
  hDtrkPt_swap_Dpt2_5->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin);
  
  TH1D* hDtrkPt_back_Dpt0_1 = (TH1D*) hDtrkPt->Clone("hDtrkPt_back_Dpt0_1");
  hDtrkPt_back_Dpt0_1->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin);
  TH1D* hDtrkPt_back_Dpt1_2 = (TH1D*) hDtrkPt->Clone("hDtrkPt_back_Dpt1_2");
  hDtrkPt_back_Dpt1_2->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin);
  TH1D* hDtrkPt_back_Dpt2_5 = (TH1D*) hDtrkPt->Clone("hDtrkPt_back_Dpt2_5");
  hDtrkPt_back_Dpt2_5->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin);
  
  TH1D* hDtrkPtQual = new TH1D(
    "hDtrkPtQual",
    "; DtrkPtErr / DtrkPt; Entries",
    25, 0., 0.25
  );
  TH1D* hDtrkPtQual_Dpt0_1 = (TH1D*) hDtrkPtQual->Clone("hDtrkPtQual_Dpt0_1");
  hDtrkPtQual_Dpt0_1->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin);
  TH1D* hDtrkPtQual_Dpt1_2 = (TH1D*) hDtrkPtQual->Clone("hDtrkPtQual_Dpt1_2");
  hDtrkPtQual_Dpt1_2->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin);
  TH1D* hDtrkPtQual_Dpt2_5 = (TH1D*) hDtrkPtQual->Clone("hDtrkPtQual_Dpt2_5");
  hDtrkPtQual_Dpt2_5->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin);
  
  TH1D* hDtrkPtQual_swap_Dpt0_1 = (TH1D*) hDtrkPtQual->Clone("hDtrkPtQual_swap_Dpt0_1");
  hDtrkPtQual_swap_Dpt0_1->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin);
  TH1D* hDtrkPtQual_swap_Dpt1_2 = (TH1D*) hDtrkPtQual->Clone("hDtrkPtQual_swap_Dpt1_2");
  hDtrkPtQual_swap_Dpt1_2->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin);
  TH1D* hDtrkPtQual_swap_Dpt2_5 = (TH1D*) hDtrkPtQual->Clone("hDtrkPtQual_swap_Dpt2_5");
  hDtrkPtQual_swap_Dpt2_5->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin);
  
  TH1D* hDtrkPtQual_back_Dpt0_1 = (TH1D*) hDtrkPtQual->Clone("hDtrkPtQual_back_Dpt0_1");
  hDtrkPtQual_back_Dpt0_1->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin);
  TH1D* hDtrkPtQual_back_Dpt1_2 = (TH1D*) hDtrkPtQual->Clone("hDtrkPtQual_back_Dpt1_2");
  hDtrkPtQual_back_Dpt1_2->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin);
  TH1D* hDtrkPtQual_back_Dpt2_5 = (TH1D*) hDtrkPtQual->Clone("hDtrkPtQual_back_Dpt2_5");
  hDtrkPtQual_back_Dpt2_5->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin);
  
  TH2D* hDpVsDedx_SiglBack = new TH2D(
    "hDpVsDedx_SiglBack",
    "; Dtrack1(2) p [GeV]; log(Dtrack1(2) dE/dx)",
    50, 0., 5.,
    50, 0., 5.
  );
  TH2D* hDpVsDedx_sigl_Dpt0_1 = (TH2D*) hDpVsDedx_SiglBack->Clone("hDpVsDedx_sigl_Dpt0_1");
  hDpVsDedx_sigl_Dpt0_1->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin);
  TH2D* hDpVsDedx_sigl_Dpt1_2 = (TH2D*) hDpVsDedx_SiglBack->Clone("hDpVsDedx_sigl_Dpt1_2");
  hDpVsDedx_sigl_Dpt1_2->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin);
  TH2D* hDpVsDedx_sigl_Dpt2_5 = (TH2D*) hDpVsDedx_SiglBack->Clone("hDpVsDedx_sigl_Dpt2_5");
  hDpVsDedx_sigl_Dpt2_5->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin);
  
  TH2D* hDpVsDedx_back_Dpt0_1 = (TH2D*) hDpVsDedx_SiglBack->Clone("hDpVsDedx_back_Dpt0_1");
  hDpVsDedx_back_Dpt0_1->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin);
  TH2D* hDpVsDedx_back_Dpt1_2 = (TH2D*) hDpVsDedx_SiglBack->Clone("hDpVsDedx_back_Dpt1_2");
  hDpVsDedx_back_Dpt1_2->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin);
  TH2D* hDpVsDedx_back_Dpt2_5 = (TH2D*) hDpVsDedx_SiglBack->Clone("hDpVsDedx_back_Dpt2_5");
  hDpVsDedx_back_Dpt2_5->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin);
  
  // Mass spectrum
  TH1D* hDmass = new TH1D(
    "hDmass",
    "Dmass; mass [GeV]; Entries",
    40, 1.465, 2.265
  );
  sDyBin = Form("%.f < Dy < %.f", DyMin, DyMax);
  TString axisTitleY = "Normalized Entries";
  
  TH1D* hDmass_Dpt0_1 = (TH1D*) hDmass->Clone("hDmass_Dpt0_1");
  hDmass_Dpt0_1->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin +"; mass [GeV]; "+ axisTitleY);
  TH1D* hDmass_Dpt1_2 = (TH1D*) hDmass->Clone("hDmass_Dpt1_2");
  hDmass_Dpt1_2->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin +"; mass [GeV]; "+ axisTitleY);
  TH1D* hDmass_Dpt2_5 = (TH1D*) hDmass->Clone("hDmass_Dpt2_5");
  hDmass_Dpt2_5->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin +"; mass [GeV]; "+ axisTitleY);
  
  TH1D* hDmass_Dpt0_1_PAS = (TH1D*) hDmass->Clone("hDmass_Dpt0_1_PAS");
  hDmass_Dpt0_1_PAS->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin +" (PAS only); mass [GeV]; "+ axisTitleY);
  TH1D* hDmass_Dpt1_2_PAS = (TH1D*) hDmass->Clone("hDmass_Dpt1_2_PAS");
  hDmass_Dpt1_2_PAS->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin +" (PAS only); mass [GeV]; "+ axisTitleY);
  TH1D* hDmass_Dpt2_5_PAS = (TH1D*) hDmass->Clone("hDmass_Dpt2_5_PAS");
  hDmass_Dpt2_5_PAS->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin +" (PAS only); mass [GeV]; "+ axisTitleY);
  
  TH1D* hDmass_Dpt0_1_PID = (TH1D*) hDmass->Clone("hDmass_Dpt0_1_PID");
  hDmass_Dpt0_1_PID->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin +" (PID only); mass [GeV]; "+ axisTitleY);
  TH1D* hDmass_Dpt1_2_PID = (TH1D*) hDmass->Clone("hDmass_Dpt1_2_PID");
  hDmass_Dpt1_2_PID->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin +" (PID only); mass [GeV]; "+ axisTitleY);
  TH1D* hDmass_Dpt2_5_PID = (TH1D*) hDmass->Clone("hDmass_Dpt2_5_PID");
  hDmass_Dpt2_5_PID->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin +" (PID only); mass [GeV]; "+ axisTitleY);
  
  TH1D* hDmass_Dpt0_1_PASPID = (TH1D*) hDmass->Clone("hDmass_Dpt0_1_PASPID");
  hDmass_Dpt0_1_PASPID->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin +" (PAS+PID); mass [GeV]; "+ axisTitleY);
  TH1D* hDmass_Dpt1_2_PASPID = (TH1D*) hDmass->Clone("hDmass_Dpt1_2_PASPID");
  hDmass_Dpt1_2_PASPID->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin +" (PAS+PID); mass [GeV]; "+ axisTitleY);
  TH1D* hDmass_Dpt2_5_PASPID = (TH1D*) hDmass->Clone("hDmass_Dpt2_5_PASPID");
  hDmass_Dpt2_5_PASPID->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin +" (PAS+PID); mass [GeV]; "+ axisTitleY);
  
  // Gen-matched signal
  TH1D* hDmass_Dpt0_1_PAS_sigl = (TH1D*) hDmass->Clone("hDmass_Dpt0_1_PAS_sigl");
  hDmass_Dpt0_1_PAS_sigl->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin +" (PAS signal); mass [GeV]; "+ axisTitleY);
  TH1D* hDmass_Dpt1_2_PAS_sigl = (TH1D*) hDmass->Clone("hDmass_Dpt1_2_PAS_sigl");
  hDmass_Dpt1_2_PAS_sigl->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin +" (PAS signal); mass [GeV]; "+ axisTitleY);
  TH1D* hDmass_Dpt2_5_PAS_sigl = (TH1D*) hDmass->Clone("hDmass_Dpt2_5_PAS_sigl");
  hDmass_Dpt2_5_PAS_sigl->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin +" (PAS signal); mass [GeV]; "+ axisTitleY);
  
  TH1D* hDmass_Dpt0_1_PID_sigl = (TH1D*) hDmass->Clone("hDmass_Dpt0_1_PID_sigl");
  hDmass_Dpt0_1_PID_sigl->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin +" (PID signal); mass [GeV]; "+ axisTitleY);
  TH1D* hDmass_Dpt1_2_PID_sigl = (TH1D*) hDmass->Clone("hDmass_Dpt1_2_PID_sigl");
  hDmass_Dpt1_2_PID_sigl->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin +" (PID signal); mass [GeV]; "+ axisTitleY);
  TH1D* hDmass_Dpt2_5_PID_sigl = (TH1D*) hDmass->Clone("hDmass_Dpt2_5_PID_sigl");
  hDmass_Dpt2_5_PID_sigl->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin +" (PID signal); mass [GeV]; "+ axisTitleY);
  
  TH1D* hDmass_Dpt0_1_PASPID_sigl = (TH1D*) hDmass->Clone("hDmass_Dpt0_1_PASPID_sigl");
  hDmass_Dpt0_1_PASPID_sigl->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin +" (PAS+PID signal); mass [GeV]; "+ axisTitleY);
  TH1D* hDmass_Dpt1_2_PASPID_sigl = (TH1D*) hDmass->Clone("hDmass_Dpt1_2_PASPID_sigl");
  hDmass_Dpt1_2_PASPID_sigl->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin +" (PAS+PID signal); mass [GeV]; "+ axisTitleY);
  TH1D* hDmass_Dpt2_5_PASPID_sigl = (TH1D*) hDmass->Clone("hDmass_Dpt2_5_PASPID_sigl");
  hDmass_Dpt2_5_PASPID_sigl->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin +" (PAS+PID signal); mass [GeV]; "+ axisTitleY);
  
  // Gen-matched swap
  TH1D* hDmass_Dpt0_1_PAS_swap = (TH1D*) hDmass->Clone("hDmass_Dpt0_1_PAS_swap");
  hDmass_Dpt0_1_PAS_swap->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin +" (PID swap); mass [GeV]; "+ axisTitleY);
  TH1D* hDmass_Dpt1_2_PAS_swap = (TH1D*) hDmass->Clone("hDmass_Dpt1_2_PAS_swap");
  hDmass_Dpt1_2_PAS_swap->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin +" (PID swap); mass [GeV]; "+ axisTitleY);
  TH1D* hDmass_Dpt2_5_PAS_swap = (TH1D*) hDmass->Clone("hDmass_Dpt2_5_PAS_swap");
  hDmass_Dpt2_5_PAS_swap->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin +" (PID swap); mass [GeV]; "+ axisTitleY);
  
  TH1D* hDmass_Dpt0_1_PID_swap = (TH1D*) hDmass->Clone("hDmass_Dpt0_1_PID_swap");
  hDmass_Dpt0_1_PID_swap->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin +" (PID swap); mass [GeV]; "+ axisTitleY);
  TH1D* hDmass_Dpt1_2_PID_swap = (TH1D*) hDmass->Clone("hDmass_Dpt1_2_PID_swap");
  hDmass_Dpt1_2_PID_swap->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin +" (PID swap); mass [GeV]; "+ axisTitleY);
  TH1D* hDmass_Dpt2_5_PID_swap = (TH1D*) hDmass->Clone("hDmass_Dpt2_5_PID_swap");
  hDmass_Dpt2_5_PID_swap->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin +" (PID swap); mass [GeV]; "+ axisTitleY);
  
  TH1D* hDmass_Dpt0_1_PASPID_swap = (TH1D*) hDmass->Clone("hDmass_Dpt0_1_PASPID_swap");
  hDmass_Dpt0_1_PASPID_swap->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin +" (PAS+PID swap); mass [GeV]; "+ axisTitleY);
  TH1D* hDmass_Dpt1_2_PASPID_swap = (TH1D*) hDmass->Clone("hDmass_Dpt1_2_PASPID_swap");
  hDmass_Dpt1_2_PASPID_swap->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin +" (PAS+PID swap); mass [GeV]; "+ axisTitleY);
  TH1D* hDmass_Dpt2_5_PASPID_swap = (TH1D*) hDmass->Clone("hDmass_Dpt2_5_PASPID_swap");
  hDmass_Dpt2_5_PASPID_swap->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin +" (PAS+PID swap); mass [GeV]; "+ axisTitleY);
  
  // Background (i.e. not gen-matched)
  TH1D* hDmass_Dpt0_1_PAS_back = (TH1D*) hDmass->Clone("hDmass_Dpt0_1_PAS_back");
  hDmass_Dpt0_1_PAS_back->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin +" (PAS background); mass [GeV]; "+ axisTitleY);
  TH1D* hDmass_Dpt1_2_PAS_back = (TH1D*) hDmass->Clone("hDmass_Dpt1_2_PAS_back");
  hDmass_Dpt1_2_PAS_back->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin +" (PAS background); mass [GeV]; "+ axisTitleY);
  TH1D* hDmass_Dpt2_5_PAS_back = (TH1D*) hDmass->Clone("hDmass_Dpt2_5_PAS_back");
  hDmass_Dpt2_5_PAS_back->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin +" (PAS background); mass [GeV]; "+ axisTitleY);
  
  TH1D* hDmass_Dpt0_1_PID_back = (TH1D*) hDmass->Clone("hDmass_Dpt0_1_PID_back");
  hDmass_Dpt0_1_PID_back->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin +" (PID background); mass [GeV]; "+ axisTitleY);
  TH1D* hDmass_Dpt1_2_PID_back = (TH1D*) hDmass->Clone("hDmass_Dpt1_2_PID_back");
  hDmass_Dpt1_2_PID_back->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin +" (PID background); mass [GeV]; "+ axisTitleY);
  TH1D* hDmass_Dpt2_5_PID_back = (TH1D*) hDmass->Clone("hDmass_Dpt2_5_PID_back");
  hDmass_Dpt2_5_PID_back->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin +" (PID background); mass [GeV]; "+ axisTitleY);
  
  TH1D* hDmass_Dpt0_1_PASPID_back = (TH1D*) hDmass->Clone("hDmass_Dpt0_1_PASPID_back");
  hDmass_Dpt0_1_PASPID_back->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin +" (PAS+PID background); mass [GeV]; "+ axisTitleY);
  TH1D* hDmass_Dpt1_2_PASPID_back = (TH1D*) hDmass->Clone("hDmass_Dpt1_2_PASPID_back");
  hDmass_Dpt1_2_PASPID_back->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin +" (PAS+PID background); mass [GeV]; "+ axisTitleY);
  TH1D* hDmass_Dpt2_5_PASPID_back = (TH1D*) hDmass->Clone("hDmass_Dpt2_5_PASPID_back");
  hDmass_Dpt2_5_PASPID_back->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin +" (PAS+PID background); mass [GeV]; "+ axisTitleY);
  
  // dEdx vs DtrkP
  TH2D* hDpVsDedx = new TH2D(
    "hDpVsDedx",
    "; Dtrack1(2) P [GeV]; log(Dtrack1(2) dE/dx)",
    60, 0., 3.,
    60, 0., 3.
  );
  
  TH2D* hDpVsDedx_Dpt0_1_PAS = (TH2D*) hDpVsDedx->Clone("hDpVsDedx_Dpt0_1_PAS");
  hDpVsDedx_Dpt0_1_PAS->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin +" (PAS only); Dtrack1(2) P [GeV]; Dtrack1(2) dE/dx");
  TH2D* hDpVsDedx_Dpt1_2_PAS = (TH2D*) hDpVsDedx->Clone("hDpVsDedx_Dpt1_2_PAS");
  hDpVsDedx_Dpt1_2_PAS->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin +" (PAS only); Dtrack1(2) P [GeV]; Dtrack1(2) dE/dx");
  TH2D* hDpVsDedx_Dpt2_5_PAS = (TH2D*) hDpVsDedx->Clone("hDpVsDedx_Dpt2_5_PAS");
  hDpVsDedx_Dpt2_5_PAS->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin +" (PAS only); Dtrack1(2) P [GeV]; Dtrack1(2) dE/dx");
  
  TH2D* hDpVsDedx_Dpt0_1_PID = (TH2D*) hDpVsDedx->Clone("hDpVsDedx_Dpt0_1_PID");
  hDpVsDedx_Dpt0_1_PID->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin +" (PID only); Dtrack1(2) P [GeV]; Dtrack1(2) dE/dx");
  TH2D* hDpVsDedx_Dpt1_2_PID = (TH2D*) hDpVsDedx->Clone("hDpVsDedx_Dpt1_2_PID");
  hDpVsDedx_Dpt1_2_PID->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin +" (PID only); Dtrack1(2) P [GeV]; Dtrack1(2) dE/dx");
  TH2D* hDpVsDedx_Dpt2_5_PID = (TH2D*) hDpVsDedx->Clone("hDpVsDedx_Dpt2_5_PID");
  hDpVsDedx_Dpt2_5_PID->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin +" (PID only); Dtrack1(2) P [GeV]; Dtrack1(2) dE/dx");
  
  TH2D* hDpVsDedx_Dpt0_1_PASPID = (TH2D*) hDpVsDedx->Clone("hDpVsDedx_Dpt0_1_PASPID");
  hDpVsDedx_Dpt0_1_PASPID->SetTitle("0 < Dp_{T} < 1 GeV, "+ sDyBin +" (PAS+PID); Dtrack1(2) P [GeV]; Dtrack1(2) dE/dx");
  TH2D* hDpVsDedx_Dpt1_2_PASPID = (TH2D*) hDpVsDedx->Clone("hDpVsDedx_Dpt1_2_PASPID");
  hDpVsDedx_Dpt1_2_PASPID->SetTitle("1 < Dp_{T} < 2 GeV, "+ sDyBin +" (PAS+PID); Dtrack1(2) P [GeV]; Dtrack1(2) dE/dx");
  TH2D* hDpVsDedx_Dpt2_5_PASPID = (TH2D*) hDpVsDedx->Clone("hDpVsDedx_Dpt2_5_PASPID");
  hDpVsDedx_Dpt2_5_PASPID->SetTitle("2 < Dp_{T} < 5 GeV, "+ sDyBin +" (PAS+PID); Dtrack1(2) P [GeV]; Dtrack1(2) dE/dx");
  
  hDmass_Dpt0_1_PAS->Sumw2();
  hDmass_Dpt0_1_PID->Sumw2();
  hDmass_Dpt0_1_PASPID->Sumw2();
  hDmass_Dpt0_1_PAS_sigl->Sumw2();
  hDmass_Dpt0_1_PID_sigl->Sumw2();
  hDmass_Dpt0_1_PASPID_sigl->Sumw2();
  hDmass_Dpt0_1_PAS_swap->Sumw2();
  hDmass_Dpt0_1_PID_swap->Sumw2();
  hDmass_Dpt0_1_PASPID_swap->Sumw2();
  hDmass_Dpt0_1_PAS_back->Sumw2();
  hDmass_Dpt0_1_PID_back->Sumw2();
  hDmass_Dpt0_1_PASPID_back->Sumw2();
  
  hDmass_Dpt1_2_PAS->Sumw2();
  hDmass_Dpt1_2_PID->Sumw2();
  hDmass_Dpt1_2_PASPID->Sumw2();
  hDmass_Dpt1_2_PAS_sigl->Sumw2();
  hDmass_Dpt1_2_PID_sigl->Sumw2();
  hDmass_Dpt1_2_PASPID_sigl->Sumw2();
  hDmass_Dpt1_2_PAS_swap->Sumw2();
  hDmass_Dpt1_2_PID_swap->Sumw2();
  hDmass_Dpt1_2_PASPID_swap->Sumw2();
  hDmass_Dpt1_2_PAS_back->Sumw2();
  hDmass_Dpt1_2_PID_back->Sumw2();
  hDmass_Dpt1_2_PASPID_back->Sumw2();
  
  hDmass_Dpt2_5_PAS->Sumw2();
  hDmass_Dpt2_5_PID->Sumw2();
  hDmass_Dpt2_5_PASPID->Sumw2();
  hDmass_Dpt2_5_PAS_sigl->Sumw2();
  hDmass_Dpt2_5_PID_sigl->Sumw2();
  hDmass_Dpt2_5_PASPID_sigl->Sumw2();
  hDmass_Dpt2_5_PAS_swap->Sumw2();
  hDmass_Dpt2_5_PID_swap->Sumw2();
  hDmass_Dpt2_5_PASPID_swap->Sumw2();
  hDmass_Dpt2_5_PAS_back->Sumw2();
  hDmass_Dpt2_5_PID_back->Sumw2();
  hDmass_Dpt2_5_PASPID_back->Sumw2();
  
  int signalCount_all_Dpt0_1 = 0;
  int signalCount_all_Dpt1_2 = 0;
  int signalCount_all_Dpt2_5 = 0;
  int signalCount_evSel_Dpt0_1 = 0;
  int signalCount_evSel_Dpt1_2 = 0;
  int signalCount_evSel_Dpt2_5 = 0;
  int signalCount_DSel_Dpt0_1 = 0;
  int signalCount_DSel_Dpt1_2 = 0;
  int signalCount_DSel_Dpt2_5 = 0;
  int signalCount_PASSel_Dpt0_1 = 0;
  int signalCount_PASSel_Dpt1_2 = 0;
  int signalCount_PASSel_Dpt2_5 = 0;
  int signalCount_PIDSel_Dpt0_1 = 0;
  int signalCount_PIDSel_Dpt1_2 = 0;
  int signalCount_PIDSel_Dpt2_5 = 0;
  int signalCount_PASPIDSel_Dpt0_1 = 0;
  int signalCount_PASPIDSel_Dpt1_2 = 0;
  int signalCount_PASPIDSel_Dpt2_5 = 0;
  // Differentiated Kaon band region only:
  int signalCount_evSelKband_Dpt0_1 = 0;
  int signalCount_evSelKband_Dpt1_2 = 0;
  int signalCount_evSelKband_Dpt2_5 = 0;
  int signalCount_PASSelKband_Dpt0_1 = 0;
  int signalCount_PASSelKband_Dpt1_2 = 0;
  int signalCount_PASSelKband_Dpt2_5 = 0;
  int signalCount_PIDSelKband_Dpt0_1 = 0;
  int signalCount_PIDSelKband_Dpt1_2 = 0;
  int signalCount_PIDSelKband_Dpt2_5 = 0;
  int signalCount_PASPIDSelKband_Dpt0_1 = 0;
  int signalCount_PASPIDSelKband_Dpt1_2 = 0;
  int signalCount_PASPIDSelKband_Dpt2_5 = 0;
  
  if (maxEntries < 0) maxEntries = tree->GetEntries();
  for (long i = 0; i < maxEntries; i++) {
    tree->GetEntry(i);
    vector<float> DgenMasses = {};
    // Fill initial event counts
    for (int j = 0; j < Dsize; j++) {
      bool isSigl = false;
      bool isSwap = false;
      if (isMC) {
//        isSigl = DisSignalCalc->at(j);
        isSigl = Dgen->at(j) == 23333 || Dgen->at(j) == 41022 || Dgen->at(j) == 41044;
        isSwap = Dgen->at(j) == 23344 || Dgen->at(j) == 41122 || Dgen->at(j) == 41144;
      }
      if (Dy->at(j) < DyMin || Dy->at(j) > DyMax) continue;
      if (Dpt->at(j) < 0. || Dpt->at(j) > 5.) continue;
      if (isSigl) {
        if (Dpt->at(j) > 0. && Dpt->at(j) < 1.) signalCount_all_Dpt0_1++;
        if (Dpt->at(j) > 1. && Dpt->at(j) < 2.) signalCount_all_Dpt1_2++;
        if (Dpt->at(j) > 2. && Dpt->at(j) < 5.) signalCount_all_Dpt2_5++;
        DgenMasses.push_back(Dmass->at(j));
        
        // Fill params for signal
        if (Dpt->at(j) > 0. && Dpt->at(j) < 1.) {
          hDchi2cl_Dpt0_1->Fill(Dchi2cl->at(j));
          hDalpha_Dpt0_1->Fill(fabs(Dalpha->at(j)));
          hDdtheta_Dpt0_1->Fill(fabs(Ddtheta->at(j)));
          hDsvpvSig_Dpt0_1->Fill(DsvpvDistance->at(j) / DsvpvDisErr->at(j));
          hDtrkP_Dpt0_1->Fill(Dtrk1P->at(j));
          hDtrkP_Dpt0_1->Fill(Dtrk2P->at(j));
          hDtrkPt_Dpt0_1->Fill(Dtrk1Pt->at(j));
          hDtrkPt_Dpt0_1->Fill(Dtrk2Pt->at(j));
          hDtrkPtQual_Dpt0_1->Fill(Dtrk1PtErr->at(j) / Dtrk1Pt->at(j));
          hDtrkPtQual_Dpt0_1->Fill(Dtrk2PtErr->at(j) / Dtrk2Pt->at(j));
          hDpVsDedx_sigl_Dpt0_1->Fill(Dtrk1P->at(j), Dtrk1dedx->at(j));
          hDpVsDedx_sigl_Dpt0_1->Fill(Dtrk2P->at(j), Dtrk2dedx->at(j));
        }
        if (Dpt->at(j) > 1. && Dpt->at(j) < 2.) {
          hDchi2cl_Dpt1_2->Fill(Dchi2cl->at(j));
          hDalpha_Dpt1_2->Fill(fabs(Dalpha->at(j)));
          hDdtheta_Dpt1_2->Fill(fabs(Ddtheta->at(j)));
          hDsvpvSig_Dpt1_2->Fill(DsvpvDistance->at(j) / DsvpvDisErr->at(j));
          hDtrkP_Dpt1_2->Fill(Dtrk1P->at(j));
          hDtrkP_Dpt1_2->Fill(Dtrk2P->at(j));
          hDtrkPt_Dpt1_2->Fill(Dtrk1Pt->at(j));
          hDtrkPt_Dpt1_2->Fill(Dtrk2Pt->at(j));
          hDtrkPtQual_Dpt1_2->Fill(Dtrk1PtErr->at(j) / Dtrk1Pt->at(j));
          hDtrkPtQual_Dpt1_2->Fill(Dtrk2PtErr->at(j) / Dtrk2Pt->at(j));
          hDpVsDedx_sigl_Dpt1_2->Fill(Dtrk1P->at(j), Dtrk1dedx->at(j));
          hDpVsDedx_sigl_Dpt1_2->Fill(Dtrk2P->at(j), Dtrk2dedx->at(j));
        }
        if (Dpt->at(j) > 2. && Dpt->at(j) < 5.) {
          hDchi2cl_Dpt2_5->Fill(Dchi2cl->at(j));
          hDalpha_Dpt2_5->Fill(fabs(Dalpha->at(j)));
          hDdtheta_Dpt2_5->Fill(fabs(Ddtheta->at(j)));
          hDsvpvSig_Dpt2_5->Fill(DsvpvDistance->at(j) / DsvpvDisErr->at(j));
          hDtrkP_Dpt2_5->Fill(Dtrk1P->at(j));
          hDtrkP_Dpt2_5->Fill(Dtrk2P->at(j));
          hDtrkPt_Dpt2_5->Fill(Dtrk1Pt->at(j));
          hDtrkPt_Dpt2_5->Fill(Dtrk2Pt->at(j));
          hDtrkPtQual_Dpt2_5->Fill(Dtrk1PtErr->at(j) / Dtrk1Pt->at(j));
          hDtrkPtQual_Dpt2_5->Fill(Dtrk2PtErr->at(j) / Dtrk2Pt->at(j));
          hDpVsDedx_sigl_Dpt2_5->Fill(Dtrk1P->at(j), Dtrk1dedx->at(j));
          hDpVsDedx_sigl_Dpt2_5->Fill(Dtrk2P->at(j), Dtrk2dedx->at(j));
        }
      } else if (isSwap) {
        // Fill swap
        if (Dpt->at(j) > 0. && Dpt->at(j) < 1.) {
          hDchi2cl_swap_Dpt0_1->Fill(Dchi2cl->at(j));
          hDalpha_swap_Dpt0_1->Fill(fabs(Dalpha->at(j)));
          hDdtheta_swap_Dpt0_1->Fill(fabs(Ddtheta->at(j)));
          hDsvpvSig_swap_Dpt0_1->Fill(DsvpvDistance->at(j) / DsvpvDisErr->at(j));
          hDtrkP_swap_Dpt0_1->Fill(Dtrk1P->at(j));
          hDtrkP_swap_Dpt0_1->Fill(Dtrk2P->at(j));
          hDtrkPt_swap_Dpt0_1->Fill(Dtrk1Pt->at(j));
          hDtrkPt_swap_Dpt0_1->Fill(Dtrk2Pt->at(j));
          hDtrkPtQual_swap_Dpt0_1->Fill(Dtrk1PtErr->at(j) / Dtrk1Pt->at(j));
          hDtrkPtQual_swap_Dpt0_1->Fill(Dtrk2PtErr->at(j) / Dtrk2Pt->at(j));
        }
        if (Dpt->at(j) > 1. && Dpt->at(j) < 2.) {
          hDchi2cl_swap_Dpt1_2->Fill(Dchi2cl->at(j));
          hDalpha_swap_Dpt1_2->Fill(fabs(Dalpha->at(j)));
          hDdtheta_swap_Dpt1_2->Fill(fabs(Ddtheta->at(j)));
          hDsvpvSig_swap_Dpt1_2->Fill(DsvpvDistance->at(j) / DsvpvDisErr->at(j));
          hDtrkP_swap_Dpt1_2->Fill(Dtrk1P->at(j));
          hDtrkP_swap_Dpt1_2->Fill(Dtrk2P->at(j));
          hDtrkPt_swap_Dpt1_2->Fill(Dtrk1Pt->at(j));
          hDtrkPt_swap_Dpt1_2->Fill(Dtrk2Pt->at(j));
          hDtrkPtQual_swap_Dpt1_2->Fill(Dtrk1PtErr->at(j) / Dtrk1Pt->at(j));
          hDtrkPtQual_swap_Dpt1_2->Fill(Dtrk2PtErr->at(j) / Dtrk2Pt->at(j));
        }
        if (Dpt->at(j) > 2. && Dpt->at(j) < 5.) {
          hDchi2cl_swap_Dpt2_5->Fill(Dchi2cl->at(j));
          hDalpha_swap_Dpt2_5->Fill(fabs(Dalpha->at(j)));
          hDdtheta_swap_Dpt2_5->Fill(fabs(Ddtheta->at(j)));
          hDsvpvSig_swap_Dpt2_5->Fill(DsvpvDistance->at(j) / DsvpvDisErr->at(j));
          hDtrkP_swap_Dpt2_5->Fill(Dtrk1P->at(j));
          hDtrkP_swap_Dpt2_5->Fill(Dtrk2P->at(j));
          hDtrkPt_swap_Dpt2_5->Fill(Dtrk1Pt->at(j));
          hDtrkPt_swap_Dpt2_5->Fill(Dtrk2Pt->at(j));
          hDtrkPtQual_swap_Dpt2_5->Fill(Dtrk1PtErr->at(j) / Dtrk1Pt->at(j));
          hDtrkPtQual_swap_Dpt2_5->Fill(Dtrk2PtErr->at(j) / Dtrk2Pt->at(j));
        }
      } else if (isMC) {
        // Fill background
        if (Dpt->at(j) > 0. && Dpt->at(j) < 1.) {
          hDchi2cl_back_Dpt0_1->Fill(Dchi2cl->at(j));
          hDalpha_back_Dpt0_1->Fill(fabs(Dalpha->at(j)));
          hDdtheta_back_Dpt0_1->Fill(fabs(Ddtheta->at(j)));
          hDsvpvSig_back_Dpt0_1->Fill(DsvpvDistance->at(j) / DsvpvDisErr->at(j));
          hDtrkP_back_Dpt0_1->Fill(Dtrk1P->at(j));
          hDtrkP_back_Dpt0_1->Fill(Dtrk2P->at(j));
          hDtrkPt_back_Dpt0_1->Fill(Dtrk1Pt->at(j));
          hDtrkPt_back_Dpt0_1->Fill(Dtrk2Pt->at(j));
          hDtrkPtQual_back_Dpt0_1->Fill(Dtrk1PtErr->at(j) / Dtrk1Pt->at(j));
          hDtrkPtQual_back_Dpt0_1->Fill(Dtrk2PtErr->at(j) / Dtrk2Pt->at(j));
          hDpVsDedx_back_Dpt0_1->Fill(Dtrk1P->at(j), Dtrk1dedx->at(j));
          hDpVsDedx_back_Dpt0_1->Fill(Dtrk2P->at(j), Dtrk2dedx->at(j));
        }
        if (Dpt->at(j) > 1. && Dpt->at(j) < 2.) {
          hDchi2cl_back_Dpt1_2->Fill(Dchi2cl->at(j));
          hDalpha_back_Dpt1_2->Fill(fabs(Dalpha->at(j)));
          hDdtheta_back_Dpt1_2->Fill(fabs(Ddtheta->at(j)));
          hDsvpvSig_back_Dpt1_2->Fill(DsvpvDistance->at(j) / DsvpvDisErr->at(j));
          hDtrkP_back_Dpt1_2->Fill(Dtrk1P->at(j));
          hDtrkP_back_Dpt1_2->Fill(Dtrk2P->at(j));
          hDtrkPt_back_Dpt1_2->Fill(Dtrk1Pt->at(j));
          hDtrkPt_back_Dpt1_2->Fill(Dtrk2Pt->at(j));
          hDtrkPtQual_back_Dpt1_2->Fill(Dtrk1PtErr->at(j) / Dtrk1Pt->at(j));
          hDtrkPtQual_back_Dpt1_2->Fill(Dtrk2PtErr->at(j) / Dtrk2Pt->at(j));
          hDpVsDedx_back_Dpt1_2->Fill(Dtrk1P->at(j), Dtrk1dedx->at(j));
          hDpVsDedx_back_Dpt1_2->Fill(Dtrk2P->at(j), Dtrk2dedx->at(j));
        }
        if (Dpt->at(j) > 2. && Dpt->at(j) < 5.) {
          hDchi2cl_back_Dpt2_5->Fill(Dchi2cl->at(j));
          hDalpha_back_Dpt2_5->Fill(fabs(Dalpha->at(j)));
          hDdtheta_back_Dpt2_5->Fill(fabs(Ddtheta->at(j)));
          hDsvpvSig_back_Dpt2_5->Fill(DsvpvDistance->at(j) / DsvpvDisErr->at(j));
          hDtrkP_back_Dpt2_5->Fill(Dtrk1P->at(j));
          hDtrkP_back_Dpt2_5->Fill(Dtrk2P->at(j));
          hDtrkPt_back_Dpt2_5->Fill(Dtrk1Pt->at(j));
          hDtrkPt_back_Dpt2_5->Fill(Dtrk2Pt->at(j));
          hDtrkPtQual_back_Dpt2_5->Fill(Dtrk1PtErr->at(j) / Dtrk1Pt->at(j));
          hDtrkPtQual_back_Dpt2_5->Fill(Dtrk2PtErr->at(j) / Dtrk2Pt->at(j));
          hDpVsDedx_back_Dpt2_5->Fill(Dtrk1P->at(j), Dtrk1dedx->at(j));
          hDpVsDedx_back_Dpt2_5->Fill(Dtrk2P->at(j), Dtrk2dedx->at(j));
        }
      }
    }
    // Apply event selection
    if (!selectedBkgFilter || !selectedVtxFilter) continue;
    if (isGammaN && !(gapgammaN && ZDCgammaN)) continue;
    if (!isGammaN && !(gapNgamma && ZDCNgamma)) continue;
    if (nVtx >= 3) continue;
    
    
    
    // Iterate over D candidates
    for (int j = 0; j < Dsize; j++) {
      // Select only |Dy| < 1 and Dpt 0-5 GeV
      if (Dy->at(j) < DyMin || Dy->at(j) > DyMax) continue;
      if (Dpt->at(j) < 0. || Dpt->at(j) > 5.) continue;
      bool isSigl = false;
      bool isSwap = false;
      if (isMC) {
//        isSigl = DisSignalCalc->at(j);
        isSigl = Dgen->at(j) == 23333 || Dgen->at(j) == 41022 || Dgen->at(j) == 41044;
//          || Dgen->at(j) == 233 || Dgen->at(j) == 3333 || Dgen->at(j) == 33;
        isSwap = Dgen->at(j) == 23344 || Dgen->at(j) == 41122 || Dgen->at(j) == 41144;
//          || Dgen->at(j) == 3344;
      }
      if (isSigl) {
        if (Dpt->at(j) > 0. && Dpt->at(j) < 1.) signalCount_evSel_Dpt0_1++;
        if (Dpt->at(j) > 1. && Dpt->at(j) < 2.) signalCount_evSel_Dpt1_2++;
        if (Dpt->at(j) > 2. && Dpt->at(j) < 5.) signalCount_evSel_Dpt2_5++;
        if ((Dtrk1P->at(j) < KMatchDtrkPMax && Dtrk1MassHypo->at(j) > 0.3) ||
            (Dtrk2P->at(j) < KMatchDtrkPMax && Dtrk2MassHypo->at(j) > 0.3)) {
          if (Dpt->at(j) > 0. && Dpt->at(j) < 1.) signalCount_evSelKband_Dpt0_1++;
          if (Dpt->at(j) > 1. && Dpt->at(j) < 2.) signalCount_evSelKband_Dpt1_2++;
          if (Dpt->at(j) > 2. && Dpt->at(j) < 5.) signalCount_evSelKband_Dpt2_5++;
        }
      }
      
      // Apply cuts
      bool passDCut = ApplyDCut(
        Dtrk1Pt->at(j), Dtrk1PtErr->at(j),
        Dtrk1PixelHit->at(j), Dtrk1StripHit->at(j),
        Dtrk2Pt->at(j), Dtrk2PtErr->at(j),
        Dtrk2PixelHit->at(j), Dtrk2StripHit->at(j)
      );
      bool passPASCut = ApplyPASCut(
        Dchi2cl->at(j), Dalpha->at(j), Ddtheta->at(j), DsvpvDistance->at(j),
        DsvpvDisErr->at(j), Dtrk1Pt->at(j), Dtrk2Pt->at(j),
        Dpt->at(j),
        DtrkPtMin
      );
      bool passPIDCut = ApplyPIDCut(
        Dtrk1P->at(j), Dtrk1MassHypo->at(j), Dtrk1PionScore->at(j),
        Dtrk1KaonScore->at(j), Dtrk1ProtScore->at(j),
        Dtrk2P->at(j), Dtrk2MassHypo->at(j), Dtrk2PionScore->at(j),
        Dtrk2KaonScore->at(j), Dtrk2ProtScore->at(j)
      );
      
      // Apply D selection
      if (!passDCut) continue;
      if (Dtrk1Pt->at(j) < DtrkPtMin || Dtrk2Pt->at(j) < DtrkPtMin) continue;
      if (isSigl) {
        if (Dpt->at(j) > 0. && Dpt->at(j) < 1.) signalCount_DSel_Dpt0_1++;
        if (Dpt->at(j) > 1. && Dpt->at(j) < 2.) signalCount_DSel_Dpt1_2++;
        if (Dpt->at(j) > 2. && Dpt->at(j) < 5.) signalCount_DSel_Dpt2_5++;
      }
      // Apply cut variations
      if (Dpt->at(j) > 0. && Dpt->at(j) < 1.) {
        if (passPASCut) {
          hDmass_Dpt0_1_PAS->Fill(Dmass->at(j));
          hDpVsDedx_Dpt0_1_PAS->Fill(Dtrk1P->at(j), log(Dtrk1dedx->at(j)));
          hDpVsDedx_Dpt0_1_PAS->Fill(Dtrk2P->at(j), log(Dtrk2dedx->at(j)));
          if (isSigl) {
            hDmass_Dpt0_1_PAS_sigl->Fill(Dmass->at(j));
            signalCount_PASSel_Dpt0_1++;
            if ((Dtrk1P->at(j) < KMatchDtrkPMax && Dtrk1MassHypo->at(j) > 0.3) ||
                (Dtrk2P->at(j) < KMatchDtrkPMax && Dtrk2MassHypo->at(j) > 0.3)
              ) signalCount_PASSelKband_Dpt0_1++;
          } else if (isSwap) {
            hDmass_Dpt0_1_PAS_swap->Fill(Dmass->at(j));
          } else if (isMC) {
            hDmass_Dpt0_1_PAS_back->Fill(Dmass->at(j));
          }
        }
        if (passPIDCut) {
          hDmass_Dpt0_1_PID->Fill(Dmass->at(j));
          hDpVsDedx_Dpt0_1_PID->Fill(Dtrk1P->at(j), log(Dtrk1dedx->at(j)));
          hDpVsDedx_Dpt0_1_PID->Fill(Dtrk2P->at(j), log(Dtrk2dedx->at(j)));
          if (isSigl) {
            hDmass_Dpt0_1_PID_sigl->Fill(Dmass->at(j));
            signalCount_PIDSel_Dpt0_1++;
            if ((Dtrk1P->at(j) < KMatchDtrkPMax && Dtrk1MassHypo->at(j) > 0.3) ||
                (Dtrk2P->at(j) < KMatchDtrkPMax && Dtrk2MassHypo->at(j) > 0.3)
              ) signalCount_PIDSelKband_Dpt0_1++;
          } else if (isSwap) {
            hDmass_Dpt0_1_PID_swap->Fill(Dmass->at(j));
          } else if (isMC) {
            hDmass_Dpt0_1_PID_back->Fill(Dmass->at(j));
          }
        }
        if (passPASCut && passPIDCut) {
          hDmass_Dpt0_1_PASPID->Fill(Dmass->at(j));
          hDpVsDedx_Dpt0_1_PASPID->Fill(Dtrk1P->at(j), log(Dtrk1dedx->at(j)));
          hDpVsDedx_Dpt0_1_PASPID->Fill(Dtrk2P->at(j), log(Dtrk2dedx->at(j)));
          if (isSigl) {
            hDmass_Dpt0_1_PASPID_sigl->Fill(Dmass->at(j));
            signalCount_PASPIDSel_Dpt0_1++;
            if ((Dtrk1P->at(j) < KMatchDtrkPMax && Dtrk1MassHypo->at(j) > 0.3) ||
                (Dtrk2P->at(j) < KMatchDtrkPMax && Dtrk2MassHypo->at(j) > 0.3)
              ) signalCount_PASPIDSelKband_Dpt0_1++;
          } else if (isSwap) {
            hDmass_Dpt0_1_PASPID_swap->Fill(Dmass->at(j));
          } else if (isMC) {
            hDmass_Dpt0_1_PASPID_back->Fill(Dmass->at(j));
            for (int k = 0; k < DgenMasses.size(); k++) {
              if (Dmass->at(j) == DgenMasses[k]) {
                cout << i << ", " << j << ": Dmass = " << Dmass->at(j) << ", Dgen = " << Dgen->at(j) << endl;
              }
            }
          }
        }
      }
      if (Dpt->at(j) > 1. && Dpt->at(j) < 2.) {
        if (passPASCut) {
          hDmass_Dpt1_2_PAS->Fill(Dmass->at(j));
          hDpVsDedx_Dpt1_2_PAS->Fill(Dtrk1P->at(j), log(Dtrk1dedx->at(j)));
          hDpVsDedx_Dpt1_2_PAS->Fill(Dtrk2P->at(j), log(Dtrk2dedx->at(j)));
          if (isSigl) {
            hDmass_Dpt1_2_PAS_sigl->Fill(Dmass->at(j));
            signalCount_PASSel_Dpt1_2++;
            if ((Dtrk1P->at(j) < KMatchDtrkPMax && Dtrk1MassHypo->at(j) > 0.3) ||
                (Dtrk2P->at(j) < KMatchDtrkPMax && Dtrk2MassHypo->at(j) > 0.3)
              ) signalCount_PASSelKband_Dpt1_2++;
          } else if (isSwap) {
            hDmass_Dpt1_2_PAS_swap->Fill(Dmass->at(j));
          } else if (isMC) {
            hDmass_Dpt1_2_PAS_back->Fill(Dmass->at(j));
//            if (Dmass->at(j) > 1.85 && Dmass->at(j) < 1.89)
//              cout << i << ", " << j << ": Dmass = " << Dmass->at(j) << ", Dgen = " << Dgen->at(j) << endl;
          }
        }
        if (passPIDCut) {
          hDmass_Dpt1_2_PID->Fill(Dmass->at(j));
          hDpVsDedx_Dpt1_2_PID->Fill(Dtrk1P->at(j), log(Dtrk1dedx->at(j)));
          hDpVsDedx_Dpt1_2_PID->Fill(Dtrk2P->at(j), log(Dtrk2dedx->at(j)));
          if (isSigl) {
            hDmass_Dpt1_2_PID_sigl->Fill(Dmass->at(j));
            signalCount_PIDSel_Dpt1_2++;
            if ((Dtrk1P->at(j) < KMatchDtrkPMax && Dtrk1MassHypo->at(j) > 0.3) ||
                (Dtrk2P->at(j) < KMatchDtrkPMax && Dtrk2MassHypo->at(j) > 0.3)
              ) signalCount_PIDSelKband_Dpt1_2++;
          } else if (isSwap) {
            hDmass_Dpt1_2_PID_swap->Fill(Dmass->at(j));
          } else if (isMC) {
            hDmass_Dpt1_2_PID_back->Fill(Dmass->at(j));
          }
        }
        if (passPASCut && passPIDCut) {
          hDmass_Dpt1_2_PASPID->Fill(Dmass->at(j));
          hDpVsDedx_Dpt1_2_PASPID->Fill(Dtrk1P->at(j), log(Dtrk1dedx->at(j)));
          hDpVsDedx_Dpt1_2_PASPID->Fill(Dtrk2P->at(j), log(Dtrk2dedx->at(j)));
          if (isSigl) {
            hDmass_Dpt1_2_PASPID_sigl->Fill(Dmass->at(j));
            signalCount_PASPIDSel_Dpt1_2++;
            if ((Dtrk1P->at(j) < KMatchDtrkPMax && Dtrk1MassHypo->at(j) > 0.3) ||
                (Dtrk2P->at(j) < KMatchDtrkPMax && Dtrk2MassHypo->at(j) > 0.3)
              ) signalCount_PASPIDSelKband_Dpt1_2++;
          } else if (isSwap) {
            hDmass_Dpt1_2_PASPID_swap->Fill(Dmass->at(j));
          } else if (isMC) {
            hDmass_Dpt1_2_PASPID_back->Fill(Dmass->at(j));
//            if (Dmass->at(j) > 1.85 && Dmass->at(j) < 1.89)
//              cout << i << ", " << j << ": Dmass = " << Dmass->at(j) << ", Dgen = " << Dgen->at(j) << endl;
            for (int k = 0; k < DgenMasses.size(); k++) {
              if (Dmass->at(j) == DgenMasses[k]) {
                cout << i << ", " << j << ": Dmass = " << Dmass->at(j) << ", Dgen = " << Dgen->at(j) << endl;
              }
            }
          }
        }
      }
      if (Dpt->at(j) > 2. && Dpt->at(j) < 5.) {
        if (passPASCut) {
          hDmass_Dpt2_5_PAS->Fill(Dmass->at(j));
          hDpVsDedx_Dpt2_5_PAS->Fill(Dtrk1P->at(j), log(Dtrk1dedx->at(j)));
          hDpVsDedx_Dpt2_5_PAS->Fill(Dtrk2P->at(j), log(Dtrk2dedx->at(j)));
          if (isSigl) {
            hDmass_Dpt2_5_PAS_sigl->Fill(Dmass->at(j));
            signalCount_PASSel_Dpt2_5++;
            if ((Dtrk1P->at(j) < KMatchDtrkPMax && Dtrk1MassHypo->at(j) > 0.3) ||
                (Dtrk2P->at(j) < KMatchDtrkPMax && Dtrk2MassHypo->at(j) > 0.3)
              ) signalCount_PASSelKband_Dpt2_5++;
          } else if (isSwap) {
            hDmass_Dpt2_5_PAS_swap->Fill(Dmass->at(j));
          } else if (isMC) {
            hDmass_Dpt2_5_PAS_back->Fill(Dmass->at(j));
          }
        }
        if (passPIDCut) {
          hDmass_Dpt2_5_PID->Fill(Dmass->at(j));
          hDpVsDedx_Dpt2_5_PID->Fill(Dtrk1P->at(j), log(Dtrk1dedx->at(j)));
          hDpVsDedx_Dpt2_5_PID->Fill(Dtrk2P->at(j), log(Dtrk2dedx->at(j)));
          if (isSigl) {
            hDmass_Dpt2_5_PID_sigl->Fill(Dmass->at(j));
            signalCount_PIDSel_Dpt2_5++;
            if ((Dtrk1P->at(j) < KMatchDtrkPMax && Dtrk1MassHypo->at(j) > 0.3) ||
                (Dtrk2P->at(j) < KMatchDtrkPMax && Dtrk2MassHypo->at(j) > 0.3)
              ) signalCount_PIDSelKband_Dpt2_5++;
          } else if (isSwap) {
            hDmass_Dpt2_5_PID_swap->Fill(Dmass->at(j));
          } else if (isMC) {
            hDmass_Dpt2_5_PID_back->Fill(Dmass->at(j));
          }
        }
        if (passPASCut && passPIDCut) {
          hDmass_Dpt2_5_PASPID->Fill(Dmass->at(j));
          hDpVsDedx_Dpt2_5_PASPID->Fill(Dtrk1P->at(j), log(Dtrk1dedx->at(j)));
          hDpVsDedx_Dpt2_5_PASPID->Fill(Dtrk2P->at(j), log(Dtrk2dedx->at(j)));
          if (isSigl) {
            hDmass_Dpt2_5_PASPID_sigl->Fill(Dmass->at(j));
            signalCount_PASPIDSel_Dpt2_5++;
            if ((Dtrk1P->at(j) < KMatchDtrkPMax && Dtrk1MassHypo->at(j) > 0.3) ||
                (Dtrk2P->at(j) < KMatchDtrkPMax && Dtrk2MassHypo->at(j) > 0.3)
              ) signalCount_PASPIDSelKband_Dpt2_5++;
          } else if (isSwap) {
            hDmass_Dpt2_5_PASPID_swap->Fill(Dmass->at(j));
          } else if (isMC) {
            hDmass_Dpt2_5_PASPID_back->Fill(Dmass->at(j));
            for (int k = 0; k < DgenMasses.size(); k++) {
              if (Dmass->at(j) == DgenMasses[k]) {
                cout << i << ", " << j << ": Dmass = " << Dmass->at(j) << ", Dgen = " << Dgen->at(j) << endl;
              }
            }
          }
        }
      }
    }
  }
  
  if (isMC) {
    cout << "Selection Counts ============================" << endl;
    cout << "  Total (pt 0-1):     " << signalCount_all_Dpt0_1 << endl;
    cout << "  Total (pt 1-2):     " << signalCount_all_Dpt1_2 << endl;
    cout << "  Total (pt 2-5):     " << signalCount_all_Dpt2_5 << endl;
    cout << "  Event Sel (pt 0-1): " << signalCount_evSel_Dpt0_1 << endl;
    cout << "  Event Sel (pt 1-2): " << signalCount_evSel_Dpt1_2 << endl;
    cout << "  Event Sel (pt 2-5): " << signalCount_evSel_Dpt2_5 << endl;
    cout << "  D Sel (pt 0-1):     " << signalCount_DSel_Dpt0_1 << endl;
    cout << "  D Sel (pt 1-2):     " << signalCount_DSel_Dpt1_2 << endl;
    cout << "  D Sel (pt 2-5):     " << signalCount_DSel_Dpt2_5 << endl;
    cout << "  PAS Only (pt 0-1):  " << signalCount_PASSel_Dpt0_1 << endl;
    cout << "  PAS Only (pt 1-2):  " << signalCount_PASSel_Dpt1_2 << endl;
    cout << "  PAS Only (pt 2-5):  " << signalCount_PASSel_Dpt2_5 << endl;
    cout << "  PID Only (pt 0-1):  " << signalCount_PIDSel_Dpt0_1 << endl;
    cout << "  PID Only (pt 1-2):  " << signalCount_PIDSel_Dpt1_2 << endl;
    cout << "  PID Only (pt 2-5):  " << signalCount_PIDSel_Dpt2_5 << endl;
    cout << "  PAS + PID (pt 0-1): " << signalCount_PASPIDSel_Dpt0_1 << endl;
    cout << "  PAS + PID (pt 1-2): " << signalCount_PASPIDSel_Dpt1_2 << endl;
    cout << "  PAS + PID (pt 2-5): " << signalCount_PASPIDSel_Dpt2_5 << endl;
    cout << "Event Sel Efficiency ========================" << endl;
    cout << "  Event Eff (pt 0-1): " << Form(
        "%.3f",
        float(signalCount_evSel_Dpt0_1) / float(signalCount_all_Dpt0_1)
      ) << endl;
    cout << "  Event Eff (pt 1-2): " << Form(
        "%.3f",
        float(signalCount_evSel_Dpt1_2) / float(signalCount_all_Dpt1_2)
      ) << endl;
    cout << "  Event Eff (pt 2-5): " << Form(
        "%.3f",
        float(signalCount_evSel_Dpt2_5) / float(signalCount_all_Dpt2_5)
      ) << endl;
    cout << "PAS-Only Efficiency (PAS Sel / Ev Sel) ======" << endl;
    cout << "  PAS Eff (pt 0-1):   " << Form(
        "%.3f",
        float(signalCount_PASSel_Dpt0_1) / float(signalCount_evSel_Dpt0_1)
      ) << endl;
    cout << "  PAS Eff (pt 1-2):   " << Form(
        "%.3f",
        float(signalCount_PASSel_Dpt1_2) / float(signalCount_evSel_Dpt1_2)
      ) << endl;
    cout << "  PAS Eff (pt 2-5):   " << Form(
        "%.3f",
        float(signalCount_PASSel_Dpt2_5) / float(signalCount_evSel_Dpt2_5)
      ) << endl;
    cout << "PID-Only Efficiency (PID Sel / Ev Sel) ======" << endl;
    cout << "  PID Eff (pt 0-1):   " << Form(
        "%.3f",
        float(signalCount_PIDSel_Dpt0_1) / float(signalCount_evSel_Dpt0_1)
      ) << endl;
    cout << "  PID Eff (pt 1-2):   " << Form(
        "%.3f",
        float(signalCount_PIDSel_Dpt1_2) / float(signalCount_evSel_Dpt1_2)
      ) << endl;
    cout << "  PID Eff (pt 2-5):   " << Form(
        "%.3f",
        float(signalCount_PIDSel_Dpt2_5) / float(signalCount_evSel_Dpt2_5)
      ) << endl;
    cout << "PAS + PID Efficiency (PID Sel / Ev Sel) =====" << endl;
    cout << "  PAS+PID Eff (pt 0-1): " << Form(
        "%.3f",
        float(signalCount_PASPIDSel_Dpt0_1) / float(signalCount_evSel_Dpt0_1)
      ) << endl;
    cout << "  PAS+PID Eff (pt 1-2): " << Form(
        "%.3f",
        float(signalCount_PASPIDSel_Dpt1_2) / float(signalCount_evSel_Dpt1_2)
      ) << endl;
    cout << "  PAS+PID Eff (pt 2-5): " << Form(
        "%.3f",
        float(signalCount_PASPIDSel_Dpt2_5) / float(signalCount_evSel_Dpt2_5)
      ) << endl;
  }
  
  TCanvas* c3 = new TCanvas("c3", "", 1800, 900);
  gStyle->SetOptStat(0);
  c3->Divide(3, 1, 0.0001, 0.0001);
  
  float rowHt = 0.03;
  float rowText = 0.025;
  
  TLatex* latex = new TLatex();
  latex->SetTextSize(rowText);
  latex->SetTextFont(42);
  latex->SetTextAlign(12);
  
//  hDmass_Dpt0_1_PAS_sigl->Scale(1/hDmass_Dpt0_1_PAS->Integral());
//  hDmass_Dpt0_1_PAS->Scale(1/hDmass_Dpt0_1_PAS->Integral());
//  hDmass_Dpt0_1_PID_sigl->Scale(1/hDmass_Dpt0_1_PID->Integral());
//  hDmass_Dpt0_1_PID->Scale(1/hDmass_Dpt0_1_PID->Integral());
//  hDmass_Dpt0_1_PASPID_sigl->Scale(1/hDmass_Dpt0_1_PASPID->Integral());
//  hDmass_Dpt0_1_PASPID->Scale(1/hDmass_Dpt0_1_PASPID->Integral());
//  
//  hDmass_Dpt1_2_PAS_sigl->Scale(1/hDmass_Dpt1_2_PAS->Integral());
//  hDmass_Dpt1_2_PAS->Scale(1/hDmass_Dpt1_2_PAS->Integral());
//  hDmass_Dpt1_2_PID_sigl->Scale(1/hDmass_Dpt1_2_PID->Integral());
//  hDmass_Dpt1_2_PID->Scale(1/hDmass_Dpt1_2_PID->Integral());
//  hDmass_Dpt1_2_PASPID_sigl->Scale(1/hDmass_Dpt1_2_PASPID->Integral());
//  hDmass_Dpt1_2_PASPID->Scale(1/hDmass_Dpt1_2_PASPID->Integral());
//  
//  hDmass_Dpt2_5_PAS_sigl->Scale(1/hDmass_Dpt2_5_PAS->Integral());
//  hDmass_Dpt2_5_PAS->Scale(1/hDmass_Dpt2_5_PAS->Integral());
//  hDmass_Dpt2_5_PID_sigl->Scale(1/hDmass_Dpt2_5_PID->Integral());
//  hDmass_Dpt2_5_PID->Scale(1/hDmass_Dpt2_5_PID->Integral());
//  hDmass_Dpt2_5_PASPID_sigl->Scale(1/hDmass_Dpt2_5_PASPID->Integral());
//  hDmass_Dpt2_5_PASPID->Scale(1/hDmass_Dpt2_5_PASPID->Integral());
  
  c3->cd(1);
  gPad->SetMargin(0.15, 0.05, 0.1, 0.1);
  hDmass_Dpt0_1->SetMaximum(1.2 * //hDmass_Dpt0_1_PAS->GetMaximum() );
    TMath::Max(hDmass_Dpt0_1_PASPID->GetMaximum(), TMath::Max(
      hDmass_Dpt0_1_PAS->GetMaximum(), hDmass_Dpt0_1_PID->GetMaximum()
  )));
  hDmass_Dpt0_1->SetMinimum(0);
  hDmass_Dpt0_1->Draw();
  hDmass_Dpt0_1_PAS->SetLineColor(kAzure-4);
  hDmass_Dpt0_1_PAS->SetLineWidth(1);
  hDmass_Dpt0_1_PAS->SetMinimum(0);
  hDmass_Dpt0_1_PAS->Draw("same hist");
  hDmass_Dpt0_1_PID->SetLineColor(kViolet-4);
  hDmass_Dpt0_1_PID->SetLineWidth(1);
  hDmass_Dpt0_1_PID->SetMinimum(0);
  hDmass_Dpt0_1_PID->Draw("same hist");
  hDmass_Dpt0_1_PASPID->SetLineColor(kPink-4);
  hDmass_Dpt0_1_PASPID->SetLineWidth(1);
  hDmass_Dpt0_1_PASPID->Draw("same hist");
  if (isMC) {
    hDmass_Dpt0_1_PAS_sigl->SetLineColor(kAzure);
    hDmass_Dpt0_1_PAS_sigl->SetLineWidth(1);
    hDmass_Dpt0_1_PAS_sigl->Draw("same hist");
    hDmass_Dpt0_1_PID_sigl->SetLineColor(kViolet);
    hDmass_Dpt0_1_PID_sigl->SetLineWidth(1);
    hDmass_Dpt0_1_PID_sigl->Draw("same hist");
    hDmass_Dpt0_1_PASPID_sigl->SetLineColor(kPink);
    hDmass_Dpt0_1_PASPID_sigl->SetLineWidth(1);
    hDmass_Dpt0_1_PASPID_sigl->Draw("same hist");
  }
  c3->cd(2);
  gPad->SetMargin(0.15, 0.05, 0.1, 0.1);
  hDmass_Dpt1_2->SetMaximum(1.2 * //hDmass_Dpt1_2_PAS->GetMaximum() );
    TMath::Max(hDmass_Dpt1_2_PASPID->GetMaximum(), TMath::Max(
      hDmass_Dpt1_2_PAS->GetMaximum(), hDmass_Dpt1_2_PID->GetMaximum()
  )));
  hDmass_Dpt1_2->SetMinimum(0);
  hDmass_Dpt1_2->Draw();
  hDmass_Dpt1_2_PAS->SetLineColor(kAzure-4);
  hDmass_Dpt1_2_PAS->SetLineWidth(1);
  hDmass_Dpt1_2_PAS->SetMinimum(0);
  hDmass_Dpt1_2_PAS->Draw("same hist");
  hDmass_Dpt1_2_PID->SetLineColor(kViolet-4);
  hDmass_Dpt1_2_PID->SetLineWidth(1);
  hDmass_Dpt1_2_PID->SetMinimum(0);
  hDmass_Dpt1_2_PID->Draw("same hist");
  hDmass_Dpt1_2_PASPID->SetLineColor(kPink-4);
  hDmass_Dpt1_2_PASPID->SetLineWidth(1);
  hDmass_Dpt1_2_PASPID->Draw("same hist");
  if (isMC) {
    hDmass_Dpt1_2_PAS_sigl->SetLineColor(kAzure);
    hDmass_Dpt1_2_PAS_sigl->SetLineWidth(1);
    hDmass_Dpt1_2_PAS_sigl->Draw("same hist");
    hDmass_Dpt1_2_PID_sigl->SetLineColor(kViolet);
    hDmass_Dpt1_2_PID_sigl->SetLineWidth(1);
    hDmass_Dpt1_2_PID_sigl->Draw("same hist");
    hDmass_Dpt1_2_PASPID_sigl->SetLineColor(kPink);
    hDmass_Dpt1_2_PASPID_sigl->SetLineWidth(1);
    hDmass_Dpt1_2_PASPID_sigl->Draw("same hist");
  }
  c3->cd(3);
  gPad->SetMargin(0.15, 0.05, 0.1, 0.1);
  hDmass_Dpt2_5->SetMaximum(1.2 * //hDmass_Dpt2_5_PAS->GetMaximum() );
    TMath::Max(hDmass_Dpt2_5_PASPID->GetMaximum(), TMath::Max(
      hDmass_Dpt2_5_PAS->GetMaximum(), hDmass_Dpt2_5_PID->GetMaximum()
  )));
  hDmass_Dpt2_5->SetMinimum(0);
  hDmass_Dpt2_5->Draw();
  hDmass_Dpt2_5_PAS->SetLineColor(kAzure-4);
  hDmass_Dpt2_5_PAS->SetLineWidth(1);
  hDmass_Dpt2_5_PAS->SetMinimum(0);
  hDmass_Dpt2_5_PAS->Draw("same hist");
  hDmass_Dpt2_5_PID->SetLineColor(kViolet-4);
  hDmass_Dpt2_5_PID->SetLineWidth(1);
  hDmass_Dpt2_5_PID->SetMinimum(0);
  hDmass_Dpt2_5_PID->Draw("same hist");
  hDmass_Dpt2_5_PASPID->SetLineColor(kPink-4);
  hDmass_Dpt2_5_PASPID->SetLineWidth(1);
  hDmass_Dpt2_5_PASPID->Draw("same hist");
  if (isMC) {
    hDmass_Dpt2_5_PAS_sigl->SetLineColor(kAzure);
    hDmass_Dpt2_5_PAS_sigl->SetLineWidth(1);
    hDmass_Dpt2_5_PAS_sigl->Draw("same hist");
    hDmass_Dpt2_5_PID_sigl->SetLineColor(kViolet);
    hDmass_Dpt2_5_PID_sigl->SetLineWidth(1);
    hDmass_Dpt2_5_PID_sigl->Draw("same hist");
    hDmass_Dpt2_5_PASPID_sigl->SetLineColor(kPink);
    hDmass_Dpt2_5_PASPID_sigl->SetLineWidth(1);
    hDmass_Dpt2_5_PASPID_sigl->Draw("same hist");
  }
  c3->cd();
  if (isMC) {
    latex->DrawLatex(0.065, 0.9-(1*rowHt), Form("Event Sel: %d", signalCount_evSel_Dpt0_1));
    latex->DrawLatex(0.065, 0.9-(2*rowHt), Form("PAS Sel: %d", signalCount_PASSel_Dpt0_1));
    latex->DrawLatex(0.065, 0.9-(3*rowHt), Form("PID Sel: %d", signalCount_PIDSel_Dpt0_1));
    latex->DrawLatex(0.065, 0.9-(4*rowHt), Form("PAS+PID Sel: %d", signalCount_PASPIDSel_Dpt0_1));
    latex->DrawLatex(0.065, 0.9-(6*rowHt), Form("Event Sel Eff: %.3f", float(signalCount_evSel_Dpt0_1) / float(signalCount_all_Dpt0_1)));
    latex->DrawLatex(0.065, 0.9-(7*rowHt), Form("PAS Eff: %.3f", float(signalCount_PASSel_Dpt0_1) / float(signalCount_evSel_Dpt0_1)));
    latex->DrawLatex(0.065, 0.9-(8*rowHt), Form("PID Eff: %.3f", float(signalCount_PIDSel_Dpt0_1) / float(signalCount_evSel_Dpt0_1)));
    latex->DrawLatex(0.065, 0.9-(9*rowHt), Form("PAS+PID Eff: %.3f", float(signalCount_PASPIDSel_Dpt0_1) / float(signalCount_evSel_Dpt0_1)));
    latex->DrawLatex(0.065, 0.9-(11*rowHt), Form("K-match & DtrkP<%.1f:", KMatchDtrkPMax));
    latex->DrawLatex(0.065, 0.9-(12*rowHt), Form("PAS Eff: %.3f", float(signalCount_PASSelKband_Dpt0_1) / float(signalCount_evSelKband_Dpt0_1)));
    latex->DrawLatex(0.065, 0.9-(13*rowHt), Form("PID Eff: %.3f", float(signalCount_PIDSelKband_Dpt0_1) / float(signalCount_evSelKband_Dpt0_1)));
    latex->DrawLatex(0.065, 0.9-(14*rowHt), Form("PAS+PID Eff: %.3f", float(signalCount_PASPIDSelKband_Dpt0_1) / float(signalCount_evSelKband_Dpt0_1)));
    
    latex->DrawLatex(0.400, 0.9-(1*rowHt), Form("Event Sel: %d", signalCount_evSel_Dpt1_2));
    latex->DrawLatex(0.400, 0.9-(2*rowHt), Form("PAS Sel: %d", signalCount_PASSel_Dpt1_2));
    latex->DrawLatex(0.400, 0.9-(3*rowHt), Form("PID Sel: %d", signalCount_PIDSel_Dpt1_2));
    latex->DrawLatex(0.400, 0.9-(4*rowHt), Form("PAS+PID Sel: %d", signalCount_PASPIDSel_Dpt1_2));
    latex->DrawLatex(0.400, 0.9-(6*rowHt), Form("Event Sel Eff: %.3f", float(signalCount_evSel_Dpt1_2) / float(signalCount_all_Dpt1_2)));
    latex->DrawLatex(0.400, 0.9-(7*rowHt), Form("PAS Eff: %.3f", float(signalCount_PASSel_Dpt1_2) / float(signalCount_evSel_Dpt1_2)));
    latex->DrawLatex(0.400, 0.9-(8*rowHt), Form("PID Eff: %.3f", float(signalCount_PIDSel_Dpt1_2) / float(signalCount_evSel_Dpt1_2)));
    latex->DrawLatex(0.400, 0.9-(9*rowHt), Form("PAS+PID Eff: %.3f", float(signalCount_PASPIDSel_Dpt1_2) / float(signalCount_evSel_Dpt1_2)));
    latex->DrawLatex(0.400, 0.9-(11*rowHt), Form("K-match & DtrkP<%.1f:", KMatchDtrkPMax));
    latex->DrawLatex(0.400, 0.9-(12*rowHt), Form("PAS Eff: %.3f", float(signalCount_PASSelKband_Dpt1_2) / float(signalCount_evSelKband_Dpt1_2)));
    latex->DrawLatex(0.400, 0.9-(13*rowHt), Form("PID Eff: %.3f", float(signalCount_PIDSelKband_Dpt1_2) / float(signalCount_evSelKband_Dpt1_2)));
    latex->DrawLatex(0.400, 0.9-(14*rowHt), Form("PAS+PID Eff: %.3f", float(signalCount_PASPIDSelKband_Dpt1_2) / float(signalCount_evSelKband_Dpt1_2)));
    
    latex->DrawLatex(0.733, 0.9-(1*rowHt), Form("Event Sel: %d", signalCount_evSel_Dpt2_5));
    latex->DrawLatex(0.733, 0.9-(2*rowHt), Form("PAS Sel: %d", signalCount_PASSel_Dpt2_5));
    latex->DrawLatex(0.733, 0.9-(3*rowHt), Form("PID Sel: %d", signalCount_PIDSel_Dpt2_5));
    latex->DrawLatex(0.733, 0.9-(4*rowHt), Form("PAS+PID Sel: %d", signalCount_PASPIDSel_Dpt2_5));
    latex->DrawLatex(0.733, 0.9-(6*rowHt), Form("Event Sel Eff: %.3f", float(signalCount_evSel_Dpt2_5) / float(signalCount_all_Dpt2_5)));
    latex->DrawLatex(0.733, 0.9-(7*rowHt), Form("PAS Eff: %.3f", float(signalCount_PASSel_Dpt2_5) / float(signalCount_evSel_Dpt2_5)));
    latex->DrawLatex(0.733, 0.9-(8*rowHt), Form("PID Eff: %.3f", float(signalCount_PIDSel_Dpt2_5) / float(signalCount_evSel_Dpt2_5)));
    latex->DrawLatex(0.733, 0.9-(9*rowHt), Form("PAS+PID Eff: %.3f", float(signalCount_PASPIDSel_Dpt2_5) / float(signalCount_evSel_Dpt2_5)));
    latex->DrawLatex(0.733, 0.9-(11*rowHt), Form("K-match & DtrkP<%.1f:", KMatchDtrkPMax));
    latex->DrawLatex(0.733, 0.9-(12*rowHt), Form("PAS Eff: %.3f", float(signalCount_PASSelKband_Dpt2_5) / float(signalCount_evSelKband_Dpt2_5)));
    latex->DrawLatex(0.733, 0.9-(13*rowHt), Form("PID Eff: %.3f", float(signalCount_PIDSelKband_Dpt2_5) / float(signalCount_evSelKband_Dpt2_5)));
    latex->DrawLatex(0.733, 0.9-(14*rowHt), Form("PAS+PID Eff: %.3f", float(signalCount_PASPIDSelKband_Dpt2_5) / float(signalCount_evSelKband_Dpt2_5)));
  }
  TLegend* legend;
  if (isMC) legend = new TLegend(0.87, 0.9-(6.5*rowHt), 0.96, 0.9-(0.5*rowHt));
  else legend = new TLegend(0.87, 0.9-(3.5*rowHt), .96, 0.9-(0.5*rowHt));
  legend->SetTextSize(rowText);
  legend->SetFillStyle(0);
  legend->SetBorderSize(0);
  legend->AddEntry(hDmass_Dpt2_5_PAS, "PAS only", "l");
  if (isMC) legend->AddEntry(hDmass_Dpt2_5_PAS_sigl, "PAS Gen", "l");
  legend->AddEntry(hDmass_Dpt2_5_PID, "PID only", "l");
  if (isMC) legend->AddEntry(hDmass_Dpt2_5_PID_sigl, "PID Gen", "l");
  legend->AddEntry(hDmass_Dpt2_5_PASPID, "PAS+PID", "l");
  if (isMC) legend->AddEntry(hDmass_Dpt2_5_PASPID_sigl, "PAS+PID Gen", "l");
  legend->Draw();
  c3->SaveAs(Form("plots/hDmass_DtrkPt%dMeV_Dy%.f-%.f.pdf", int(DtrkPtMin * 1000), DyMin, DyMax));
  
  TLine* purityMin_0_1;
  TLine* purityMax_0_1;
  TLine* purityMin_1_2;
  TLine* purityMax_1_2;
  TLine* purityMin_2_5;
  TLine* purityMax_2_5;
  
  // Draw PAS-only stack
  c3 = new TCanvas("c3", "", 1800, 600);
  rowHt = 0.04;
  rowText = 0.035;
  latex->SetTextSize(rowText);
  gStyle->SetOptStat(0);
  c3->Divide(3, 1, 0.0001, 0.0001);
  c3->cd(1);
  gPad->SetMargin(0.15, 0.05, 0.1, 0.1);
  hDmass_Dpt0_1->SetMaximum(1.25 * hDmass_Dpt0_1_PAS->GetMaximum());
  hDmass_Dpt0_1->Draw();
  hDmass_Dpt0_1->SetMinimum(0);
  THStack* sDmass_Dpt0_1 = new THStack("sDmass_Dpt0_1", "");
  if (isMC) {
    hDmass_Dpt0_1_PAS_sigl->SetFillColor(kAzure-7);
    hDmass_Dpt0_1_PAS_sigl->SetLineWidth(0);
    hDmass_Dpt0_1_PAS_swap->SetFillColor(kAzure-8);
    hDmass_Dpt0_1_PAS_swap->SetLineWidth(0);
    hDmass_Dpt0_1_PAS_back->SetFillColor(kAzure-9);
    hDmass_Dpt0_1_PAS_back->SetLineWidth(0);
    sDmass_Dpt0_1->Add(hDmass_Dpt0_1_PAS_back);
    sDmass_Dpt0_1->Add(hDmass_Dpt0_1_PAS_swap);
    sDmass_Dpt0_1->Add(hDmass_Dpt0_1_PAS_sigl);
    sDmass_Dpt0_1->Draw("same hist");
    purityMin_0_1 = new TLine(
      hDmass_Dpt0_1->GetBinLowEdge(purityMinBin),
      hDmass_Dpt0_1->GetMinimum(),
      hDmass_Dpt0_1->GetBinLowEdge(purityMinBin),
      hDmass_Dpt0_1->GetMaximum()
    );
    purityMax_0_1 = new TLine(
      hDmass_Dpt0_1->GetBinLowEdge(purityMaxBin+1),
      hDmass_Dpt0_1->GetMinimum(),
      hDmass_Dpt0_1->GetBinLowEdge(purityMaxBin+1),
      hDmass_Dpt0_1->GetMaximum()
    );
    purityMin_0_1->SetLineColorAlpha(kGray+1, 0.5);
    purityMax_0_1->SetLineColorAlpha(kGray+1, 0.5);
    purityMin_0_1->Draw();
    purityMax_0_1->Draw();
  }
  hDmass_Dpt0_1_PAS->SetLineColor(kAzure);
  hDmass_Dpt0_1_PAS->SetLineWidth(1);
  hDmass_Dpt0_1_PAS->Draw("same hist");
  gPad->RedrawAxis();
  c3->cd(2);
  gPad->SetMargin(0.15, 0.05, 0.1, 0.1);
  hDmass_Dpt1_2->SetMaximum(1.25 * hDmass_Dpt1_2_PAS->GetMaximum());
  hDmass_Dpt1_2->Draw();
  hDmass_Dpt1_2->SetMinimum(0);
  THStack* sDmass_Dpt1_2 = new THStack("sDmass_Dpt1_2", "");
  if (isMC) {
    hDmass_Dpt1_2_PAS_sigl->SetFillColor(kAzure-7);
    hDmass_Dpt1_2_PAS_sigl->SetLineWidth(0);
    hDmass_Dpt1_2_PAS_swap->SetFillColor(kAzure-8);
    hDmass_Dpt1_2_PAS_swap->SetLineWidth(0);
    hDmass_Dpt1_2_PAS_back->SetFillColor(kAzure-9);
    hDmass_Dpt1_2_PAS_back->SetLineWidth(0);
    sDmass_Dpt1_2->Add(hDmass_Dpt1_2_PAS_back);
    sDmass_Dpt1_2->Add(hDmass_Dpt1_2_PAS_swap);
    sDmass_Dpt1_2->Add(hDmass_Dpt1_2_PAS_sigl);
    sDmass_Dpt1_2->Draw("same hist");
    purityMin_1_2 = new TLine(
      hDmass_Dpt1_2->GetBinLowEdge(purityMinBin),
      hDmass_Dpt1_2->GetMinimum(),
      hDmass_Dpt1_2->GetBinLowEdge(purityMinBin),
      hDmass_Dpt1_2->GetMaximum()
    );
    purityMax_1_2 = new TLine(
      hDmass_Dpt1_2->GetBinLowEdge(purityMaxBin+1),
      hDmass_Dpt1_2->GetMinimum(),
      hDmass_Dpt1_2->GetBinLowEdge(purityMaxBin+1),
      hDmass_Dpt1_2->GetMaximum()
    );
    purityMin_1_2->SetLineColorAlpha(kGray+1, 0.5);
    purityMax_1_2->SetLineColorAlpha(kGray+1, 0.5);
    purityMin_1_2->Draw();
    purityMax_1_2->Draw();
  }
  hDmass_Dpt1_2_PAS->SetLineColor(kAzure);
  hDmass_Dpt1_2_PAS->SetLineWidth(1);
  hDmass_Dpt1_2_PAS->Draw("same hist");
  gPad->RedrawAxis();
  c3->cd(3);
  gPad->SetMargin(0.15, 0.05, 0.1, 0.1);
  hDmass_Dpt2_5->SetMaximum(1.25 * hDmass_Dpt2_5_PAS->GetMaximum());
  hDmass_Dpt2_5->Draw();
  hDmass_Dpt2_5->SetMinimum(0);
  THStack* sDmass_Dpt2_5 = new THStack("sDmass_Dpt2_5", "");
  if (isMC) {
    hDmass_Dpt2_5_PAS_sigl->SetFillColor(kAzure-7);
    hDmass_Dpt2_5_PAS_sigl->SetLineWidth(0);
    hDmass_Dpt2_5_PAS_swap->SetFillColor(kAzure-8);
    hDmass_Dpt2_5_PAS_swap->SetLineWidth(0);
    hDmass_Dpt2_5_PAS_back->SetFillColor(kAzure-9);
    hDmass_Dpt2_5_PAS_back->SetLineWidth(0);
    sDmass_Dpt2_5->Add(hDmass_Dpt2_5_PAS_back);
    sDmass_Dpt2_5->Add(hDmass_Dpt2_5_PAS_swap);
    sDmass_Dpt2_5->Add(hDmass_Dpt2_5_PAS_sigl);
    sDmass_Dpt2_5->Draw("same hist");
    purityMin_2_5 = new TLine(
      hDmass_Dpt2_5->GetBinLowEdge(purityMinBin),
      hDmass_Dpt2_5->GetMinimum(),
      hDmass_Dpt2_5->GetBinLowEdge(purityMinBin),
      hDmass_Dpt2_5->GetMaximum()
    );
    purityMax_2_5 = new TLine(
      hDmass_Dpt2_5->GetBinLowEdge(purityMaxBin+1),
      hDmass_Dpt2_5->GetMinimum(),
      hDmass_Dpt2_5->GetBinLowEdge(purityMaxBin+1),
      hDmass_Dpt2_5->GetMaximum()
    );
    purityMin_2_5->SetLineColorAlpha(kGray+1, 0.5);
    purityMax_2_5->SetLineColorAlpha(kGray+1, 0.5);
    purityMin_2_5->Draw();
    purityMax_2_5->Draw();
  }
  hDmass_Dpt2_5_PAS->SetLineColor(kAzure);
  hDmass_Dpt2_5_PAS->SetLineWidth(1);
  hDmass_Dpt2_5_PAS->Draw("same hist");
  gPad->RedrawAxis();
  c3->cd();
  TLegend* legend_PAS;
  if (isMC) {
    legend_PAS = new TLegend(0.87, 0.9-(4.5*rowHt), 0.96, 0.9-(0.5*rowHt));
    legend_PAS->SetTextSize(rowText);
    legend_PAS->SetFillStyle(0);
    legend_PAS->SetBorderSize(0);
    legend_PAS->AddEntry(hDmass_Dpt2_5_PAS, "Spectrum (PAS)", "l");
    legend_PAS->AddEntry(hDmass_Dpt2_5_PAS_sigl, "Signal", "f");
    legend_PAS->AddEntry(hDmass_Dpt2_5_PAS_swap, "Swapped Mass", "f");
    legend_PAS->AddEntry(hDmass_Dpt2_5_PAS_back, "Background", "f");
    legend_PAS->Draw();
    
    float siglSum_0_1 = hDmass_Dpt0_1_PAS_sigl->Integral();
    float swapSum_0_1 = hDmass_Dpt0_1_PAS_swap->Integral();
    float siglSum_1_2 = hDmass_Dpt1_2_PAS_sigl->Integral();
    float swapSum_1_2 = hDmass_Dpt1_2_PAS_swap->Integral();
    float siglSum_2_5 = hDmass_Dpt2_5_PAS_sigl->Integral();
    float swapSum_2_5 = hDmass_Dpt2_5_PAS_swap->Integral();
    float siglSumPart_0_1 = hDmass_Dpt0_1_PAS_sigl->Integral(purityMinBin, purityMaxBin);
    float swapSumPart_0_1 = hDmass_Dpt0_1_PAS_swap->Integral(purityMinBin, purityMaxBin);
    float backSumPart_0_1 = hDmass_Dpt0_1_PAS_back->Integral(purityMinBin, purityMaxBin);
    float siglSumPart_1_2 = hDmass_Dpt1_2_PAS_sigl->Integral(purityMinBin, purityMaxBin);
    float swapSumPart_1_2 = hDmass_Dpt1_2_PAS_swap->Integral(purityMinBin, purityMaxBin);
    float backSumPart_1_2 = hDmass_Dpt1_2_PAS_back->Integral(purityMinBin, purityMaxBin);
    float siglSumPart_2_5 = hDmass_Dpt2_5_PAS_sigl->Integral(purityMinBin, purityMaxBin);
    float swapSumPart_2_5 = hDmass_Dpt2_5_PAS_swap->Integral(purityMinBin, purityMaxBin);
    float backSumPart_2_5 = hDmass_Dpt2_5_PAS_back->Integral(purityMinBin, purityMaxBin);
    
    latex->DrawLatex(0.065, 0.9-(1*rowHt), Form("Event Sel: %d", signalCount_evSel_Dpt0_1));
    latex->DrawLatex(0.065, 0.9-(2*rowHt), Form("PAS Sel: %d", signalCount_PASSel_Dpt0_1));
    latex->DrawLatex(0.065, 0.9-(3*rowHt), Form("PAS Eff: %.3f", float(signalCount_PASSel_Dpt0_1) / float(signalCount_evSel_Dpt0_1)));
    latex->DrawLatex(0.065, 0.9-(5*rowHt), Form("Sigl/(Sigl+Back): %.3f", siglSumPart_0_1 / (siglSumPart_0_1 + backSumPart_0_1)));
    latex->DrawLatex(0.065, 0.9-(6*rowHt), Form("Swap/Sigl: %.3f", swapSum_0_1 / siglSum_0_1));
    
    latex->DrawLatex(0.400, 0.9-(1*rowHt), Form("Event Sel: %d", signalCount_evSel_Dpt1_2));
    latex->DrawLatex(0.400, 0.9-(2*rowHt), Form("PAS Sel: %d", signalCount_PASSel_Dpt1_2));
    latex->DrawLatex(0.400, 0.9-(3*rowHt), Form("PAS Eff: %.3f", float(signalCount_PASSel_Dpt1_2) / float(signalCount_evSel_Dpt1_2)));
    latex->DrawLatex(0.400, 0.9-(5*rowHt), Form("Sigl/(Sigl+Back): %.3f", siglSumPart_1_2 / (siglSumPart_1_2 + backSumPart_1_2)));
    latex->DrawLatex(0.400, 0.9-(6*rowHt), Form("Swap/Sigl: %.3f", swapSum_1_2 / siglSum_1_2));
    
    latex->DrawLatex(0.733, 0.9-(1*rowHt), Form("Event Sel: %d", signalCount_evSel_Dpt2_5));
    latex->DrawLatex(0.733, 0.9-(2*rowHt), Form("PAS Sel: %d", signalCount_PASSel_Dpt2_5));
    latex->DrawLatex(0.733, 0.9-(3*rowHt), Form("PAS Eff: %.3f", float(signalCount_PASSel_Dpt2_5) / float(signalCount_evSel_Dpt2_5)));
    latex->DrawLatex(0.733, 0.9-(5*rowHt), Form("Sigl/(Sigl+Back): %.3f", siglSumPart_2_5 / (siglSumPart_2_5 + backSumPart_2_5)));
    latex->DrawLatex(0.733, 0.9-(6*rowHt), Form("Swap/Sigl: %.3f", swapSum_2_5 / siglSum_2_5));
  }
  c3->SaveAs(Form("plots/hDmass_DtrkPt%dMeV_Dy%.f-%.f_PASOnly.pdf", int(DtrkPtMin * 1000), DyMin, DyMax));
  
  // Draw PID-only stack
  c3 = new TCanvas("c3", "", 1800, 600);
  gStyle->SetOptStat(0);
  c3->Divide(3, 1, 0.0001, 0.0001);
  c3->cd(1);
  gPad->SetMargin(0.15, 0.05, 0.1, 0.1);
  hDmass_Dpt0_1->SetMaximum(1.25 * hDmass_Dpt0_1_PID->GetMaximum());
  hDmass_Dpt0_1->Draw();
  hDmass_Dpt0_1->SetMinimum(0);
  sDmass_Dpt0_1 = new THStack("sDmass_Dpt0_1", "");
  if (isMC) {
    hDmass_Dpt0_1_PID_sigl->SetFillColor(kViolet-7);
    hDmass_Dpt0_1_PID_sigl->SetLineWidth(0);
    hDmass_Dpt0_1_PID_swap->SetFillColor(kViolet-8);
    hDmass_Dpt0_1_PID_swap->SetLineWidth(0);
    hDmass_Dpt0_1_PID_back->SetFillColor(kViolet-9);
    hDmass_Dpt0_1_PID_back->SetLineWidth(0);
    sDmass_Dpt0_1->Add(hDmass_Dpt0_1_PID_back);
    sDmass_Dpt0_1->Add(hDmass_Dpt0_1_PID_swap);
    sDmass_Dpt0_1->Add(hDmass_Dpt0_1_PID_sigl);
    sDmass_Dpt0_1->Draw("same hist");
    purityMin_0_1 = new TLine(
      hDmass_Dpt0_1->GetBinLowEdge(purityMinBin),
      hDmass_Dpt0_1->GetMinimum(),
      hDmass_Dpt0_1->GetBinLowEdge(purityMinBin),
      hDmass_Dpt0_1->GetMaximum()
    );
    purityMax_0_1 = new TLine(
      hDmass_Dpt0_1->GetBinLowEdge(purityMaxBin+1),
      hDmass_Dpt0_1->GetMinimum(),
      hDmass_Dpt0_1->GetBinLowEdge(purityMaxBin+1),
      hDmass_Dpt0_1->GetMaximum()
    );
    purityMin_0_1->SetLineColorAlpha(kGray+1, 0.5);
    purityMax_0_1->SetLineColorAlpha(kGray+1, 0.5);
    purityMin_0_1->Draw();
    purityMax_0_1->Draw();
  }
  hDmass_Dpt0_1_PID->SetLineColor(kViolet);
  hDmass_Dpt0_1_PID->SetLineWidth(1);
  hDmass_Dpt0_1_PID->Draw("same hist");
  c3->cd(2);
  gPad->SetMargin(0.15, 0.05, 0.1, 0.1);
  hDmass_Dpt1_2->SetMaximum(1.25 * hDmass_Dpt1_2_PID->GetMaximum());
  hDmass_Dpt1_2->Draw();
  hDmass_Dpt1_2->SetMinimum(0);
  sDmass_Dpt1_2 = new THStack("sDmass_Dpt1_2", "");
  gPad->RedrawAxis();
  if (isMC) {
    hDmass_Dpt1_2_PID_sigl->SetFillColor(kViolet-7);
    hDmass_Dpt1_2_PID_sigl->SetLineWidth(0);
    hDmass_Dpt1_2_PID_swap->SetFillColor(kViolet-8);
    hDmass_Dpt1_2_PID_swap->SetLineWidth(0);
    hDmass_Dpt1_2_PID_back->SetFillColor(kViolet-9);
    hDmass_Dpt1_2_PID_back->SetLineWidth(0);
    sDmass_Dpt1_2->Add(hDmass_Dpt1_2_PID_back);
    sDmass_Dpt1_2->Add(hDmass_Dpt1_2_PID_swap);
    sDmass_Dpt1_2->Add(hDmass_Dpt1_2_PID_sigl);
    sDmass_Dpt1_2->Draw("same hist");
    purityMin_1_2 = new TLine(
      hDmass_Dpt1_2->GetBinLowEdge(purityMinBin),
      hDmass_Dpt1_2->GetMinimum(),
      hDmass_Dpt1_2->GetBinLowEdge(purityMinBin),
      hDmass_Dpt1_2->GetMaximum()
    );
    purityMax_1_2 = new TLine(
      hDmass_Dpt1_2->GetBinLowEdge(purityMaxBin+1),
      hDmass_Dpt1_2->GetMinimum(),
      hDmass_Dpt1_2->GetBinLowEdge(purityMaxBin+1),
      hDmass_Dpt1_2->GetMaximum()
    );
    purityMin_1_2->SetLineColorAlpha(kGray+1, 0.5);
    purityMax_1_2->SetLineColorAlpha(kGray+1, 0.5);
    purityMin_1_2->Draw();
    purityMax_1_2->Draw();
  }
  hDmass_Dpt1_2_PID->SetLineColor(kViolet);
  hDmass_Dpt1_2_PID->SetLineWidth(1);
  hDmass_Dpt1_2_PID->Draw("same hist");
  c3->cd(3);
  gPad->SetMargin(0.15, 0.05, 0.1, 0.1);
  hDmass_Dpt2_5->SetMaximum(1.25 * hDmass_Dpt2_5_PID->GetMaximum());
  hDmass_Dpt2_5->Draw();
  hDmass_Dpt2_5->SetMinimum(0);
  sDmass_Dpt2_5 = new THStack("sDmass_Dpt2_5", "");
  gPad->RedrawAxis();
  if (isMC) {
    hDmass_Dpt2_5_PID_sigl->SetFillColor(kViolet-7);
    hDmass_Dpt2_5_PID_sigl->SetLineWidth(0);
    hDmass_Dpt2_5_PID_swap->SetFillColor(kViolet-8);
    hDmass_Dpt2_5_PID_swap->SetLineWidth(0);
    hDmass_Dpt2_5_PID_back->SetFillColor(kViolet-9);
    hDmass_Dpt2_5_PID_back->SetLineWidth(0);
    sDmass_Dpt2_5->Add(hDmass_Dpt2_5_PID_back);
    sDmass_Dpt2_5->Add(hDmass_Dpt2_5_PID_swap);
    sDmass_Dpt2_5->Add(hDmass_Dpt2_5_PID_sigl);
    sDmass_Dpt2_5->Draw("same hist");
    purityMin_2_5 = new TLine(
      hDmass_Dpt2_5->GetBinLowEdge(purityMinBin),
      hDmass_Dpt2_5->GetMinimum(),
      hDmass_Dpt2_5->GetBinLowEdge(purityMinBin),
      hDmass_Dpt2_5->GetMaximum()
    );
    purityMax_2_5 = new TLine(
      hDmass_Dpt2_5->GetBinLowEdge(purityMaxBin+1),
      hDmass_Dpt2_5->GetMinimum(),
      hDmass_Dpt2_5->GetBinLowEdge(purityMaxBin+1),
      hDmass_Dpt2_5->GetMaximum()
    );
    purityMin_2_5->SetLineColorAlpha(kGray+1, 0.5);
    purityMax_2_5->SetLineColorAlpha(kGray+1, 0.5);
    purityMin_2_5->Draw();
    purityMax_2_5->Draw();
  }
  hDmass_Dpt2_5_PID->SetLineColor(kViolet);
  hDmass_Dpt2_5_PID->SetLineWidth(1);
  hDmass_Dpt2_5_PID->Draw("same hist");
  gPad->RedrawAxis();
  c3->cd();
  TLegend* legend_PID;
  if (isMC) {
    legend_PID = new TLegend(0.87, 0.9-(4.5*rowHt), 0.96, 0.9-(0.5*rowHt));
    legend_PID->SetTextSize(rowText);
    legend_PID->SetFillStyle(0);
    legend_PID->SetBorderSize(0);
    legend_PID->AddEntry(hDmass_Dpt2_5_PID, "Spectrum (PID)", "l");
    legend_PID->AddEntry(hDmass_Dpt2_5_PID_sigl, "Signal", "f");
    legend_PID->AddEntry(hDmass_Dpt2_5_PID_swap, "Swapped Mass", "f");
    legend_PID->AddEntry(hDmass_Dpt2_5_PID_back, "Background", "f");
    legend_PID->Draw();
    
    float siglSum_0_1 = hDmass_Dpt0_1_PID_sigl->Integral();
    float swapSum_0_1 = hDmass_Dpt0_1_PID_swap->Integral();
    float siglSum_1_2 = hDmass_Dpt1_2_PID_sigl->Integral();
    float swapSum_1_2 = hDmass_Dpt1_2_PID_swap->Integral();
    float siglSum_2_5 = hDmass_Dpt2_5_PID_sigl->Integral();
    float swapSum_2_5 = hDmass_Dpt2_5_PID_swap->Integral();
    float siglSumPart_0_1 = hDmass_Dpt0_1_PID_sigl->Integral(purityMinBin, purityMaxBin);
    float swapSumPart_0_1 = hDmass_Dpt0_1_PID_swap->Integral(purityMinBin, purityMaxBin);
    float backSumPart_0_1 = hDmass_Dpt0_1_PID_back->Integral(purityMinBin, purityMaxBin);
    float siglSumPart_1_2 = hDmass_Dpt1_2_PID_sigl->Integral(purityMinBin, purityMaxBin);
    float swapSumPart_1_2 = hDmass_Dpt1_2_PID_swap->Integral(purityMinBin, purityMaxBin);
    float backSumPart_1_2 = hDmass_Dpt1_2_PID_back->Integral(purityMinBin, purityMaxBin);
    float siglSumPart_2_5 = hDmass_Dpt2_5_PID_sigl->Integral(purityMinBin, purityMaxBin);
    float swapSumPart_2_5 = hDmass_Dpt2_5_PID_swap->Integral(purityMinBin, purityMaxBin);
    float backSumPart_2_5 = hDmass_Dpt2_5_PID_back->Integral(purityMinBin, purityMaxBin);
    
    latex->DrawLatex(0.065, 0.9-(1*rowHt), Form("Event Sel: %d", signalCount_evSel_Dpt0_1));
    latex->DrawLatex(0.065, 0.9-(2*rowHt), Form("PID Sel: %d", signalCount_PIDSel_Dpt0_1));
    latex->DrawLatex(0.065, 0.9-(3*rowHt), Form("PID Eff: %.3f", float(signalCount_PIDSel_Dpt0_1) / float(signalCount_evSel_Dpt0_1)));
    latex->DrawLatex(0.065, 0.9-(5*rowHt), Form("Sigl/(Sigl+Back): %.3f", siglSumPart_0_1 / (siglSumPart_0_1 + backSumPart_0_1)));
    latex->DrawLatex(0.065, 0.9-(6*rowHt), Form("Swap/Sigl: %.3f", swapSum_0_1 / siglSum_0_1));
    
    latex->DrawLatex(0.400, 0.9-(1*rowHt), Form("Event Sel: %d", signalCount_evSel_Dpt1_2));
    latex->DrawLatex(0.400, 0.9-(2*rowHt), Form("PID Sel: %d", signalCount_PIDSel_Dpt1_2));
    latex->DrawLatex(0.400, 0.9-(3*rowHt), Form("PID Eff: %.3f", float(signalCount_PIDSel_Dpt1_2) / float(signalCount_evSel_Dpt1_2)));
    latex->DrawLatex(0.400, 0.9-(5*rowHt), Form("Sigl/(Sigl+Back): %.3f", siglSumPart_1_2 / (siglSumPart_1_2 + backSumPart_1_2)));
    latex->DrawLatex(0.400, 0.9-(6*rowHt), Form("Swap/Sigl: %.3f", swapSum_1_2 / siglSum_1_2));
    
    latex->DrawLatex(0.733, 0.9-(1*rowHt), Form("Event Sel: %d", signalCount_evSel_Dpt2_5));
    latex->DrawLatex(0.733, 0.9-(2*rowHt), Form("PID Sel: %d", signalCount_PIDSel_Dpt2_5));
    latex->DrawLatex(0.733, 0.9-(3*rowHt), Form("PID Eff: %.3f", float(signalCount_PIDSel_Dpt2_5) / float(signalCount_evSel_Dpt2_5)));
    latex->DrawLatex(0.733, 0.9-(5*rowHt), Form("Sigl/(Sigl+Back): %.3f", siglSumPart_2_5 / (siglSumPart_2_5 + backSumPart_2_5)));
    latex->DrawLatex(0.733, 0.9-(6*rowHt), Form("Swap/Sigl: %.3f", swapSum_2_5 / siglSum_2_5));
  }
  c3->SaveAs(Form("plots/hDmass_DtrkPt%dMeV_Dy%.f-%.f_PIDOnly.pdf", int(DtrkPtMin * 1000), DyMin, DyMax));
  
  // Draw PAS+PID stack
  c3 = new TCanvas("c3", "", 1800, 600);
  gStyle->SetOptStat(0);
  c3->Divide(3, 1, 0.0001, 0.0001);
  c3->cd(1);
  gPad->SetMargin(0.15, 0.05, 0.1, 0.1);
  hDmass_Dpt0_1->SetMaximum(1.25 * hDmass_Dpt0_1_PASPID->GetMaximum());
  hDmass_Dpt0_1->Draw();
  hDmass_Dpt0_1->SetMinimum(0);
  sDmass_Dpt0_1 = new THStack("sDmass_Dpt0_1", "");
  if (isMC) {
    hDmass_Dpt0_1_PASPID_sigl->SetFillColor(kPink+3);
    hDmass_Dpt0_1_PASPID_sigl->SetLineWidth(0);
    hDmass_Dpt0_1_PASPID_swap->SetFillColor(kPink+2);
    hDmass_Dpt0_1_PASPID_swap->SetLineWidth(0);
    hDmass_Dpt0_1_PASPID_back->SetFillColor(kPink+1);
    hDmass_Dpt0_1_PASPID_back->SetLineWidth(0);
    sDmass_Dpt0_1->Add(hDmass_Dpt0_1_PASPID_back);
    sDmass_Dpt0_1->Add(hDmass_Dpt0_1_PASPID_swap);
    sDmass_Dpt0_1->Add(hDmass_Dpt0_1_PASPID_sigl);
    sDmass_Dpt0_1->Draw("same hist");
    purityMin_0_1 = new TLine(
      hDmass_Dpt0_1->GetBinLowEdge(purityMinBin),
      hDmass_Dpt0_1->GetMinimum(),
      hDmass_Dpt0_1->GetBinLowEdge(purityMinBin),
      hDmass_Dpt0_1->GetMaximum()
    );
    purityMax_0_1 = new TLine(
      hDmass_Dpt0_1->GetBinLowEdge(purityMaxBin+1),
      hDmass_Dpt0_1->GetMinimum(),
      hDmass_Dpt0_1->GetBinLowEdge(purityMaxBin+1),
      hDmass_Dpt0_1->GetMaximum()
    );
    purityMin_0_1->SetLineColorAlpha(kGray+1, 0.5);
    purityMax_0_1->SetLineColorAlpha(kGray+1, 0.5);
    purityMin_0_1->Draw();
    purityMax_0_1->Draw();
  }
  hDmass_Dpt0_1_PASPID->SetLineColor(kPink);
  hDmass_Dpt0_1_PASPID->SetLineWidth(1);
  hDmass_Dpt0_1_PASPID->Draw("same hist");
  gPad->RedrawAxis();
  c3->cd(2);
  gPad->SetMargin(0.15, 0.05, 0.1, 0.1);
  hDmass_Dpt1_2->SetMaximum(1.25 * hDmass_Dpt1_2_PASPID->GetMaximum());
  hDmass_Dpt1_2->Draw();
  hDmass_Dpt1_2->SetMinimum(0);
  sDmass_Dpt1_2 = new THStack("sDmass_Dpt1_2", "");
  if (isMC) {
    hDmass_Dpt1_2_PASPID_sigl->SetFillColor(kPink+3);
    hDmass_Dpt1_2_PASPID_sigl->SetLineWidth(0);
    hDmass_Dpt1_2_PASPID_swap->SetFillColor(kPink+2);
    hDmass_Dpt1_2_PASPID_swap->SetLineWidth(0);
    hDmass_Dpt1_2_PASPID_back->SetFillColor(kPink+1);
    hDmass_Dpt1_2_PASPID_back->SetLineWidth(0);
    sDmass_Dpt1_2->Add(hDmass_Dpt1_2_PASPID_back);
    sDmass_Dpt1_2->Add(hDmass_Dpt1_2_PASPID_swap);
    sDmass_Dpt1_2->Add(hDmass_Dpt1_2_PASPID_sigl);
    sDmass_Dpt1_2->Draw("same hist");
    purityMin_1_2 = new TLine(
      hDmass_Dpt1_2->GetBinLowEdge(purityMinBin),
      hDmass_Dpt1_2->GetMinimum(),
      hDmass_Dpt1_2->GetBinLowEdge(purityMinBin),
      hDmass_Dpt1_2->GetMaximum()
    );
    purityMax_1_2 = new TLine(
      hDmass_Dpt1_2->GetBinLowEdge(purityMaxBin+1),
      hDmass_Dpt1_2->GetMinimum(),
      hDmass_Dpt1_2->GetBinLowEdge(purityMaxBin+1),
      hDmass_Dpt1_2->GetMaximum()
    );
    purityMin_1_2->SetLineColorAlpha(kGray+1, 0.5);
    purityMax_1_2->SetLineColorAlpha(kGray+1, 0.5);
    purityMin_1_2->Draw();
    purityMax_1_2->Draw();
  }
  hDmass_Dpt1_2_PASPID->SetLineColor(kPink);
  hDmass_Dpt1_2_PASPID->SetLineWidth(1);
  hDmass_Dpt1_2_PASPID->Draw("same hist");
  gPad->RedrawAxis();
  c3->cd(3);
  gPad->SetMargin(0.15, 0.05, 0.1, 0.1);
  hDmass_Dpt2_5->SetMaximum(1.25 * hDmass_Dpt2_5_PASPID->GetMaximum());
  hDmass_Dpt2_5->Draw();
  hDmass_Dpt2_5->SetMinimum(0);
  sDmass_Dpt2_5 = new THStack("sDmass_Dpt2_5", "");
  if (isMC) {
    hDmass_Dpt2_5_PASPID_sigl->SetFillColor(kPink+3);
    hDmass_Dpt2_5_PASPID_sigl->SetLineWidth(0);
    hDmass_Dpt2_5_PASPID_swap->SetFillColor(kPink+2);
    hDmass_Dpt2_5_PASPID_swap->SetLineWidth(0);
    hDmass_Dpt2_5_PASPID_back->SetFillColor(kPink+1);
    hDmass_Dpt2_5_PASPID_back->SetLineWidth(0);
    sDmass_Dpt2_5->Add(hDmass_Dpt2_5_PASPID_back);
    sDmass_Dpt2_5->Add(hDmass_Dpt2_5_PASPID_swap);
    sDmass_Dpt2_5->Add(hDmass_Dpt2_5_PASPID_sigl);
    sDmass_Dpt2_5->Draw("same hist");
    purityMin_2_5 = new TLine(
      hDmass_Dpt2_5->GetBinLowEdge(purityMinBin),
      hDmass_Dpt2_5->GetMinimum(),
      hDmass_Dpt2_5->GetBinLowEdge(purityMinBin),
      hDmass_Dpt2_5->GetMaximum()
    );
    purityMax_2_5 = new TLine(
      hDmass_Dpt2_5->GetBinLowEdge(purityMaxBin+1),
      hDmass_Dpt2_5->GetMinimum(),
      hDmass_Dpt2_5->GetBinLowEdge(purityMaxBin+1),
      hDmass_Dpt2_5->GetMaximum()
    );
    purityMin_2_5->SetLineColorAlpha(kGray+1, 0.5);
    purityMax_2_5->SetLineColorAlpha(kGray+1, 0.5);
    purityMin_2_5->Draw();
    purityMax_2_5->Draw();
  }
  hDmass_Dpt2_5_PASPID->SetLineColor(kPink);
  hDmass_Dpt2_5_PASPID->SetLineWidth(1);
  hDmass_Dpt2_5_PASPID->Draw("same hist");
  gPad->RedrawAxis();
  c3->cd();
  TLegend* legend_PASPID;
  if (isMC) {
    legend_PASPID = new TLegend(0.87, 0.9-(4.5*rowHt), 0.96, 0.9-(0.5*rowHt));
    legend_PASPID->SetTextSize(rowText);
    legend_PASPID->SetFillStyle(0);
    legend_PASPID->SetBorderSize(0);
    legend_PASPID->AddEntry(hDmass_Dpt2_5_PASPID, "Spec. (PAS+PID)", "l");
    legend_PASPID->AddEntry(hDmass_Dpt2_5_PASPID_sigl, "Signal", "f");
    legend_PASPID->AddEntry(hDmass_Dpt2_5_PASPID_swap, "Swapped Mass", "f");
    legend_PASPID->AddEntry(hDmass_Dpt2_5_PASPID_back, "Background", "f");
    legend_PASPID->Draw();
    
    float siglSum_0_1 = hDmass_Dpt0_1_PASPID_sigl->Integral();
    float swapSum_0_1 = hDmass_Dpt0_1_PASPID_swap->Integral();
    float siglSum_1_2 = hDmass_Dpt1_2_PASPID_sigl->Integral();
    float swapSum_1_2 = hDmass_Dpt1_2_PASPID_swap->Integral();
    float siglSum_2_5 = hDmass_Dpt2_5_PASPID_sigl->Integral();
    float swapSum_2_5 = hDmass_Dpt2_5_PASPID_swap->Integral();
    float siglSumPart_0_1 = hDmass_Dpt0_1_PASPID_sigl->Integral(purityMinBin, purityMaxBin);
    float swapSumPart_0_1 = hDmass_Dpt0_1_PASPID_swap->Integral(purityMinBin, purityMaxBin);
    float backSumPart_0_1 = hDmass_Dpt0_1_PASPID_back->Integral(purityMinBin, purityMaxBin);
    float siglSumPart_1_2 = hDmass_Dpt1_2_PASPID_sigl->Integral(purityMinBin, purityMaxBin);
    float swapSumPart_1_2 = hDmass_Dpt1_2_PASPID_swap->Integral(purityMinBin, purityMaxBin);
    float backSumPart_1_2 = hDmass_Dpt1_2_PASPID_back->Integral(purityMinBin, purityMaxBin);
    float siglSumPart_2_5 = hDmass_Dpt2_5_PASPID_sigl->Integral(purityMinBin, purityMaxBin);
    float swapSumPart_2_5 = hDmass_Dpt2_5_PASPID_swap->Integral(purityMinBin, purityMaxBin);
    float backSumPart_2_5 = hDmass_Dpt2_5_PASPID_back->Integral(purityMinBin, purityMaxBin);
    
    latex->DrawLatex(0.065, 0.9-(1*rowHt), Form("Event Sel: %d", signalCount_evSel_Dpt0_1));
    latex->DrawLatex(0.065, 0.9-(2*rowHt), Form("PAS+PID Sel: %d", signalCount_PASPIDSel_Dpt0_1));
    latex->DrawLatex(0.065, 0.9-(3*rowHt), Form("PAS+PID Eff: %.3f", float(signalCount_PASPIDSel_Dpt0_1) / float(signalCount_evSel_Dpt0_1)));
    latex->DrawLatex(0.065, 0.9-(5*rowHt), Form("Sigl/(Sigl+Back): %.3f", siglSumPart_0_1 / (siglSumPart_0_1 + backSumPart_0_1)));
    latex->DrawLatex(0.065, 0.9-(6*rowHt), Form("Swap/Sigl: %.3f", swapSum_0_1 / siglSum_0_1));
    
    latex->DrawLatex(0.400, 0.9-(1*rowHt), Form("Event Sel: %d", signalCount_evSel_Dpt1_2));
    latex->DrawLatex(0.400, 0.9-(2*rowHt), Form("PAS+PID Sel: %d", signalCount_PASPIDSel_Dpt1_2));
    latex->DrawLatex(0.400, 0.9-(3*rowHt), Form("PAS+PID Eff: %.3f", float(signalCount_PASPIDSel_Dpt1_2) / float(signalCount_evSel_Dpt1_2)));
    latex->DrawLatex(0.400, 0.9-(5*rowHt), Form("Sigl/(Sigl+Back): %.3f", siglSumPart_1_2 / (siglSumPart_1_2 + backSumPart_1_2)));
    latex->DrawLatex(0.400, 0.9-(6*rowHt), Form("Swap/Sigl: %.3f", swapSum_1_2 / siglSum_1_2));
    
    latex->DrawLatex(0.733, 0.9-(1*rowHt), Form("Event Sel: %d", signalCount_evSel_Dpt2_5));
    latex->DrawLatex(0.733, 0.9-(2*rowHt), Form("PAS+PID Sel: %d", signalCount_PASPIDSel_Dpt2_5));
    latex->DrawLatex(0.733, 0.9-(3*rowHt), Form("PAS+PID Eff: %.3f", float(signalCount_PASPIDSel_Dpt2_5) / float(signalCount_evSel_Dpt2_5)));
    latex->DrawLatex(0.733, 0.9-(5*rowHt), Form("Sigl/(Sigl+Back): %.3f", siglSumPart_2_5 / (siglSumPart_2_5 + backSumPart_2_5)));
    latex->DrawLatex(0.733, 0.9-(6*rowHt), Form("Swap/Sigl: %.3f", swapSum_2_5 / siglSum_2_5));
  }
  c3->SaveAs(Form("plots/hDmass_DtrkPt%dMeV_Dy%.f-%.f_PASPID.pdf", int(DtrkPtMin * 1000), DyMin, DyMax));
  delete c3;
  
  // dE/dx vs track P
  TCanvas* c6 = new TCanvas("c6", "", 1800, 900);
  gStyle->SetOptStat(0);
  c6->Divide(3, 2, 0.0001, 0.0001);
  
  c6->cd(1);
  gPad->SetMargin(0.15, 0.12, 0.1, 0.1);
  hDpVsDedx_Dpt0_1_PAS->Draw("colz");
  c6->cd(4);
  gPad->SetMargin(0.15, 0.12, 0.1, 0.1);
  hDpVsDedx_Dpt0_1_PID->Draw("colz");
  c6->cd(2);
  gPad->SetMargin(0.15, 0.12, 0.1, 0.1);
  hDpVsDedx_Dpt1_2_PAS->Draw("colz");
  c6->cd(5);
  gPad->SetMargin(0.15, 0.12, 0.1, 0.1);
  hDpVsDedx_Dpt1_2_PID->Draw("colz");
  c6->cd(3);
  gPad->SetMargin(0.15, 0.12, 0.1, 0.1);
  hDpVsDedx_Dpt2_5_PAS->Draw("colz");
  c6->cd(6);
  gPad->SetMargin(0.15, 0.12, 0.1, 0.1);
  hDpVsDedx_Dpt2_5_PID->Draw("colz");
  c6->cd();
  c6->SaveAs(Form("plots/hDpVsDedx_DtrkPt%dMeV_Dy%.f-%.f_3x2.pdf", int(DtrkPtMin * 1000), DyMin, DyMax));
  delete c6;
  
  TCanvas* c9 = new TCanvas("c9", "", 1800, 1350);
  gStyle->SetOptStat(0);
  c9->Divide(3, 3, 0.0001, 0.0001);
  
  c9->cd(1);
  gPad->SetMargin(0.15, 0.12, 0.1, 0.1);
  hDpVsDedx_Dpt0_1_PAS->Draw("colz");
  c9->cd(4);
  gPad->SetMargin(0.15, 0.12, 0.1, 0.1);
  hDpVsDedx_Dpt0_1_PID->Draw("colz");
  c9->cd(7);
  gPad->SetMargin(0.15, 0.12, 0.1, 0.1);
  hDpVsDedx_Dpt0_1_PASPID->Draw("colz");
  c9->cd(2);
  gPad->SetMargin(0.15, 0.12, 0.1, 0.1);
  hDpVsDedx_Dpt1_2_PAS->Draw("colz");
  c9->cd(5);
  gPad->SetMargin(0.15, 0.12, 0.1, 0.1);
  hDpVsDedx_Dpt1_2_PID->Draw("colz");
  c9->cd(8);
  gPad->SetMargin(0.15, 0.12, 0.1, 0.1);
  hDpVsDedx_Dpt1_2_PASPID->Draw("colz");
  c9->cd(3);
  gPad->SetMargin(0.15, 0.12, 0.1, 0.1);
  hDpVsDedx_Dpt2_5_PAS->Draw("colz");
  c9->cd(6);
  gPad->SetMargin(0.15, 0.12, 0.1, 0.1);
  hDpVsDedx_Dpt2_5_PID->Draw("colz");
  c9->cd(9);
  gPad->SetMargin(0.15, 0.12, 0.1, 0.1);
  hDpVsDedx_Dpt2_5_PASPID->Draw("colz");
  c9->cd();
  c9->SaveAs(Form("plots/hDpVsDedx_DtrkPt%dMeV_Dy%.f-%.f_3x3.pdf", int(DtrkPtMin * 1000), DyMin, DyMax));
  delete c9;
  
  DrawParamHist(
    hDchi2cl_Dpt0_1,
    hDchi2cl_Dpt1_2,
    hDchi2cl_Dpt2_5,
    hDchi2cl_swap_Dpt0_1,
    hDchi2cl_swap_Dpt1_2,
    hDchi2cl_swap_Dpt2_5,
    hDchi2cl_back_Dpt0_1,
    hDchi2cl_back_Dpt1_2,
    hDchi2cl_back_Dpt2_5,
    Form("plots/cutParam_Dchi2cl_Dy%.f-%.f.pdf", DyMin, DyMax)
  );
  DrawParamHist(
    hDalpha_Dpt0_1,
    hDalpha_Dpt1_2,
    hDalpha_Dpt2_5,
    hDalpha_swap_Dpt0_1,
    hDalpha_swap_Dpt1_2,
    hDalpha_swap_Dpt2_5,
    hDalpha_back_Dpt0_1,
    hDalpha_back_Dpt1_2,
    hDalpha_back_Dpt2_5,
    Form("plots/cutParam_Dalpha_Dy%.f-%.f.pdf", DyMin, DyMax)
  );
  DrawParamHist(
    hDdtheta_Dpt0_1,
    hDdtheta_Dpt1_2,
    hDdtheta_Dpt2_5,
    hDdtheta_swap_Dpt0_1,
    hDdtheta_swap_Dpt1_2,
    hDdtheta_swap_Dpt2_5,
    hDdtheta_back_Dpt0_1,
    hDdtheta_back_Dpt1_2,
    hDdtheta_back_Dpt2_5,
    Form("plots/cutParam_Ddtheta_Dy%.f-%.f.pdf", DyMin, DyMax)
  );
  DrawParamHist(
    hDsvpvSig_Dpt0_1,
    hDsvpvSig_Dpt1_2,
    hDsvpvSig_Dpt2_5,
    hDsvpvSig_swap_Dpt0_1,
    hDsvpvSig_swap_Dpt1_2,
    hDsvpvSig_swap_Dpt2_5,
    hDsvpvSig_back_Dpt0_1,
    hDsvpvSig_back_Dpt1_2,
    hDsvpvSig_back_Dpt2_5,
    Form("plots/cutParam_DsvpvSig_Dy%.f-%.f.pdf", DyMin, DyMax),
    true
  );
  DrawParamHist(
    hDtrkP_Dpt0_1,
    hDtrkP_Dpt1_2,
    hDtrkP_Dpt2_5,
    hDtrkP_swap_Dpt0_1,
    hDtrkP_swap_Dpt1_2,
    hDtrkP_swap_Dpt2_5,
    hDtrkP_back_Dpt0_1,
    hDtrkP_back_Dpt1_2,
    hDtrkP_back_Dpt2_5,
    Form("plots/cutParam_DtrkP_Dy%.f-%.f.pdf", DyMin, DyMax),
    true
  );
  DrawParamHist(
    hDtrkPt_Dpt0_1,
    hDtrkPt_Dpt1_2,
    hDtrkPt_Dpt2_5,
    hDtrkPt_swap_Dpt0_1,
    hDtrkPt_swap_Dpt1_2,
    hDtrkPt_swap_Dpt2_5,
    hDtrkPt_back_Dpt0_1,
    hDtrkPt_back_Dpt1_2,
    hDtrkPt_back_Dpt2_5,
    Form("plots/cutParam_DtrkPt_Dy%.f-%.f.pdf", DyMin, DyMax),
    true
  );
  DrawParamHist(
    hDtrkPtQual_Dpt0_1,
    hDtrkPtQual_Dpt1_2,
    hDtrkPtQual_Dpt2_5,
    hDtrkPtQual_swap_Dpt0_1,
    hDtrkPtQual_swap_Dpt1_2,
    hDtrkPtQual_swap_Dpt2_5,
    hDtrkPtQual_back_Dpt0_1,
    hDtrkPtQual_back_Dpt1_2,
    hDtrkPtQual_back_Dpt2_5,
    Form("plots/cutParam_DtrkPtQual_Dy%.f-%.f.pdf", DyMin, DyMax),
    true
  );
  
  c3 = new TCanvas("c3", "", 1800, 600);
  gStyle->SetOptStat(0);
  c3->Divide(3, 1, 0.0001, 0.0001);
  
  c3->cd(1);
  gPad->SetMargin(0.15, 0.12, 0.1, 0.1);
  hDpVsDedx_sigl_Dpt0_1->Draw("colz");
  c3->cd(2);
  gPad->SetMargin(0.15, 0.12, 0.1, 0.1);
  hDpVsDedx_sigl_Dpt1_2->Draw("colz");
  c3->cd(3);
  gPad->SetMargin(0.15, 0.12, 0.1, 0.1);
  hDpVsDedx_sigl_Dpt2_5->Draw("colz");
  c3->cd();
  c3->Update();
  c3->SaveAs(Form("plots/cutParam_DtrkdedxVsDtrkP_sigl_Dy%.f-%.f.pdf", DyMin, DyMax));
  
  c3->cd(1);
  gPad->SetMargin(0.15, 0.12, 0.1, 0.1);
  hDpVsDedx_back_Dpt0_1->Draw("colz");
  c3->cd(2);
  gPad->SetMargin(0.15, 0.12, 0.1, 0.1);
  hDpVsDedx_back_Dpt1_2->Draw("colz");
  c3->cd(3);
  gPad->SetMargin(0.15, 0.12, 0.1, 0.1);
  hDpVsDedx_back_Dpt2_5->Draw("colz");
  c3->cd();
  c3->Update();
  c3->SaveAs(Form("plots/cutParam_DtrkdedxVsDtrkP_back_Dy%.f-%.f.pdf", DyMin, DyMax));
  delete c3;
  
  fin->Close();
}


// bins ->Integral(11, 30);
