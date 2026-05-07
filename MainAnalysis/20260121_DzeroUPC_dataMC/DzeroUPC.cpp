#include <TCanvas.h>
#include <TCut.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TLegend.h>
#include <TTree.h>

#include <cmath>
#include <iostream>

using namespace std;
#include "CommandLine.h" // Yi's Commandline bundle
#include "Messenger.h"   // Yi's Messengers for reading data files
#include "ProgressBar.h" // Yi's fish progress bar
#include "helpMessage.h" // Print out help message
#include "parameter.h"   // The parameters used in the analysis
#include "utilities.h"   // Yen-Jie's random utility functions

#include "WeightHandler2D.h"
#include "WeightHandler1D.h"

#define DMASS 1.86484

// Mass ranges and bins for Dmass monitoring histograms,
// intended for cross-checking datapoints from the massfit.C routine.
// Current analysis binning:
#define DMASS_HISTMIN 1.66
#define DMASS_HISTMAX 2.26
#define DMASS_HISTNBINS 48
// Bins matched to HIN-24-003 Dmass fit bins:
#define DMASS_HISTMIN_24003 1.68
#define DMASS_HISTMAX_24003 2.05
#define DMASS_HISTNBINS_24003 74
// Bins matched to HIN-25-002 Dmass fit bins:
#define DMASS_HISTMIN_25002 1.66
#define DMASS_HISTMAX_25002 2.16
#define DMASS_HISTNBINS_25002 48

//============================================================//
// Function to check for configuration errors
//============================================================//
bool checkError(const Parameters &par) { return false; }

//======= trackSelection =====================================//
// Check if the track pass selection criteria
//============================================================//
bool dzeroSelection(DzeroUPCTreeMessenger *b, Parameters par, int j) { return true; }

//======= eventSelection =====================================//
// Check if the event pass eventSelection criteria
//============================================================//
bool eventSelection(DzeroUPCTreeMessenger *b, const Parameters &par) {
  if (par.IsData)
  {
    if (par.TriggerChoice == 1 && b->isL1ZDCOr == false)
      return false;
    if (par.TriggerChoice == 2 && b->isL1ZDCXORJet8 == false)
      return false;
    if (par.TriggerChoice == 3 && b->isL1ZDCXORJet12 == false)
      return false;
    if (par.TriggerChoice == 4 && b->isL1ZDCXORJet16 == false)
      return false;
  }
  
  if (par.BkgFilterChoice == 1 &&
      (b->selectedBkgFilter == false || b->selectedVtxFilter == false)
    ) return false;
  if (par.BkgFilterChoice == 2 &&
      (b->cscTightHalo2015Filter == false || b->selectedVtxFilter == false)
    ) return false;

  // Set HF rapidity definition
  float HFEMaxPlus;
  float HFEMaxMinus;
  if (par.HFMaxRapDefn == 5) {
    HFEMaxPlus = b->HFEMaxPlus_eta5;
    HFEMaxMinus = b->HFEMaxMinus_eta5;
  } else if (par.HFMaxRapDefn == 6) {
    HFEMaxPlus = b->HFEMaxPlus_forest;
    HFEMaxMinus = b->HFEMaxMinus_forest;
  } else {
    HFEMaxPlus = b->HFEMaxPlus;
    HFEMaxMinus = b->HFEMaxMinus;
  }
  if (par.DoSystRapGap==-1)
  {
    // alternative (loose) rapidity gap selection
    if (par.IsGammaN && (b->ZDCgammaN && HFEMaxPlus < 15) == false)
      return false;
    if (!par.IsGammaN && (b->ZDCNgamma && HFEMaxMinus < 15) == false)
      return false;
  }
  else if (par.DoSystRapGap==1)
  {
    // alternative (tight) rapidity gap selection
    if (par.IsGammaN && (b->ZDCgammaN && HFEMaxPlus < 5.5) == false)
      return false;
    if (!par.IsGammaN && (b->ZDCNgamma && HFEMaxMinus < 5.5) == false)
      return false;
  } 
  else if (par.DoSystRapGap > 9)
  {
    // Custom rapidity gap threshold decision
    if (par.IsGammaN && (b->ZDCgammaN && HFEMaxPlus < (((float)par.DoSystRapGap)/10.)) == false)
      return false;
    if (!par.IsGammaN && (b->ZDCNgamma && HFEMaxMinus < (((float)par.DoSystRapGap)/10.)) == false)
      return false;
  } 
  else
  {
    // nominal rapidity gap selection
    // if (par.IsGammaN && (b->ZDCgammaN && HFEMaxPlus < 9.2) == false)
    if (par.IsGammaN && (b->ZDCgammaN && HFEMaxPlus < 16) == false)
      return false;
    // if (!par.IsGammaN && (b->ZDCNgamma && HFEMaxMinus < 8.6) == fa/lse)
    if (!par.IsGammaN && (b->ZDCNgamma && HFEMaxMinus < 16) == false)
      return false;
  }

  if (b->nVtx >= 3) return false;
  return true;
}

/*** To align data and MC skim samples, we need to apply the same base cut criteria to both data and MC. ***/
// Base cut (event): selectedVtxFilter && cscTightHalo2015Filter && ZDC && HF (< 16), using the same
// HF branch choice as eventSelection (HFMaxRapDefn: 5 = eta5, 6 = forest, else = HFEMaxPlus/Minus).
// Trigger is handled by eventSelection via TriggerChoice (isL1ZDCOr is not duplicated here).
static bool passesBaseCutEvent(DzeroUPCTreeMessenger *b, const Parameters &par) {
  if (b->selectedVtxFilter == false)
    return false;
  if (b->cscTightHalo2015Filter == false)
    return false;
  float HFEMaxPlus;
  float HFEMaxMinus;
  if (par.HFMaxRapDefn == 5) {
    HFEMaxPlus = b->HFEMaxPlus_eta5;
    HFEMaxMinus = b->HFEMaxMinus_eta5;
  } else if (par.HFMaxRapDefn == 6) {
    HFEMaxPlus = b->HFEMaxPlus_forest;
    HFEMaxMinus = b->HFEMaxMinus_forest;
  } else {
    HFEMaxPlus = b->HFEMaxPlus;
    HFEMaxMinus = b->HFEMaxMinus;
  }
  if (par.IsGammaN) {
    if ((b->ZDCgammaN && HFEMaxPlus < 16.f) == false)
      return false;
  } else {
    if ((b->ZDCNgamma && HFEMaxMinus < 16.f) == false)
      return false;
  }
  return true;
}

// Base cut (candidate): track kinematics/quality, |y|<2.4, D quality, decay length significance.
static bool passesBaseCutCandidate(DzeroUPCTreeMessenger *b, int j) {
  if (b->Dtrk1Pt == nullptr || b->Dtrk2Pt == nullptr || b->Dtrk1Eta == nullptr ||
      b->Dtrk2Eta == nullptr || b->Dtrk1PtErr == nullptr || b->Dtrk2PtErr == nullptr)
    return false;
  if (j < 0 || j >= (int)b->Dsize)
    return false;
  float t1Pt = b->Dtrk1Pt->at(j);
  float t2Pt = b->Dtrk2Pt->at(j);
  if (t1Pt <= 0.5f || t2Pt <= 0.5f)
    return false;
  if (std::fabs(b->Dtrk1Eta->at(j)) >= 2.4f || std::fabs(b->Dtrk2Eta->at(j)) >= 2.4f)
    return false;
  if (std::fabs(b->Dtrk1PtErr->at(j) / t1Pt) >= 0.1f)
    return false;
  if (std::fabs(b->Dtrk2PtErr->at(j) / t2Pt) >= 0.1f)
    return false;
  if (b->Dchi2cl->at(j) <= 0.05f)
    return false;
  float ds = b->DsvpvDistance->at(j);
  float de = b->DsvpvDisErr->at(j);
  if (de <= 1.e-8f)
    return false;
  if (ds / de <= 1.f)
    return false;
  if (b->DsvpvDisErr_2D == nullptr || b->DsvpvDisErr_2D->at(j) <= 1.e-8f)
    return false;
  return true;
}

// Optional extra D selection (bitwise OR, after passesBaseCutCandidate). 0 = disabled.
// Bit 1: Dchi2cl > 0.1
// Bit 2: open angle — |Dy|<1: Dalpha<0.4 and Ddtheta<0.5; 1<=|Dy|<2: Dalpha<0.2 and Ddtheta<0.3
// Bit 4: decay length significance |DsvpvDistance|/DsvpvDisErr > 2.5
// Bit 8: Dtrk1Pt > 1 and Dtrk2Pt > 1 (GeV)
static bool passesDextraSelectionMask(DzeroUPCTreeMessenger *b, int j, int mask) {
  if (mask == 0)
    return true;
  if (j < 0 || j >= (int)b->Dsize)
    return false;
  if (mask & 1) {
    if (b->Dchi2cl->at(j) <= 0.1f)
      return false;
  }
  if (mask & 2) {
    if (b->Dalpha == nullptr || b->Dy == nullptr || b->Ddtheta == nullptr)
      return false;
    const float ady = std::fabs(b->Dy->at(j));
    const float alpha = b->Dalpha->at(j);
    const float dth = b->Ddtheta->at(j);
    if (ady < 1.f) {
      if (alpha >= 0.4f || dth >= 0.5f)
        return false;
    } else if (ady < 2.f) {
      if (alpha >= 0.2f || dth >= 0.3f)
        return false;
    }
  }
  if (mask & 4) {
    float de = b->DsvpvDisErr->at(j);
    if (de <= 1.e-8f)
      return false;
    if (std::fabs(b->DsvpvDistance->at(j)) / de <= 2.5f)
      return false;
  }
  if (mask & 8) {
    if (b->Dtrk1Pt == nullptr || b->Dtrk2Pt == nullptr)
      return false;
    if (b->Dtrk1Pt->at(j) <= 1.f || b->Dtrk2Pt->at(j) <= 1.f)
      return false;
  }
  return true;
}
/**********************************************************************************************************************/

// Micro-tree row: TTree branches (TNtuple::Fill is limited to ~15 float args in many ROOT builds).
struct MicroTreeNtRow {
  float Dmass, Dgen, Dpt, Dy, Dalpha, Dchi2cl, DsvpvDistance, DsvpvDisErr;
  float Dmva_BDT, Ddtheta;
  float DsvpvDistance_2D, DsvpvDisErr_2D;
  float Dtrk1Pt, Dtrk1PtErr, Dtrk2Pt, Dtrk2PtErr;
};

class DataAnalyzer {
public:
  TFile *inf, *outf;
  TH1D *hDmass;
  TH1D *hDmass24003Bins;
  TH1D *hDmass25002Bins;
  DzeroUPCTreeMessenger *MDzeroUPC;
  TTree *nt;
  /** Event-level observables: one entry per selected event (filled on first D candidate that passes all cuts). */
  TTree *ntEvt;
  Int_t evt_nTrackInAcceptanceHP;
  MicroTreeNtRow ntRow;
  string title;
  TH2D *hHFEmaxPlus_vs_EvtMult;
  TH2D *hHFEmaxMinus_vs_EvtMult;
  TH1D *hDenEvtEff;
  TH1D *hNumEvtEff;
  TH1D *hRatioEvtEff;
  TH1D *hDenDEff;
  TH1D *hNumDEff;
  TH1D *hRatioDEff;

  DataAnalyzer(const char *filename, const char *outFilename, const char *mytitle = "")
      : inf(new TFile(filename)), MDzeroUPC(new DzeroUPCTreeMessenger(*inf, string("Tree"))), title(mytitle),
        outf(new TFile(outFilename, "recreate")) {
    outf->cd();
    nt = new TTree("nt", "D0 mass tree");
    nt->Branch("Dmass", &ntRow.Dmass, "Dmass/F");
    nt->Branch("Dgen", &ntRow.Dgen, "Dgen/F");
    nt->Branch("Dpt", &ntRow.Dpt, "Dpt/F");
    nt->Branch("Dy", &ntRow.Dy, "Dy/F");
    nt->Branch("Dalpha", &ntRow.Dalpha, "Dalpha/F");
    nt->Branch("Dchi2cl", &ntRow.Dchi2cl, "Dchi2cl/F");
    nt->Branch("DsvpvDistance", &ntRow.DsvpvDistance, "DsvpvDistance/F");
    nt->Branch("DsvpvDisErr", &ntRow.DsvpvDisErr, "DsvpvDisErr/F");
    nt->Branch("Dmva_BDT", &ntRow.Dmva_BDT, "Dmva_BDT/F");
    nt->Branch("Ddtheta", &ntRow.Ddtheta, "Ddtheta/F");
    nt->Branch("DsvpvDistance_2D", &ntRow.DsvpvDistance_2D, "DsvpvDistance_2D/F");
    nt->Branch("DsvpvDisErr_2D", &ntRow.DsvpvDisErr_2D, "DsvpvDisErr_2D/F");
    nt->Branch("Dtrk1Pt", &ntRow.Dtrk1Pt, "Dtrk1Pt/F");
    nt->Branch("Dtrk1PtErr", &ntRow.Dtrk1PtErr, "Dtrk1PtErr/F");
    nt->Branch("Dtrk2Pt", &ntRow.Dtrk2Pt, "Dtrk2Pt/F");
    nt->Branch("Dtrk2PtErr", &ntRow.Dtrk2PtErr, "Dtrk2PtErr/F");
    nt->SetAutoFlush(200000);
    nt->SetAutoSave(200000000);
    nt->SetDirectory(outf);

    ntEvt = new TTree("nt_evt", "Event-level observables (one entry if >=1 D passes cuts)");
    ntEvt->Branch("nTrackInAcceptanceHP", &evt_nTrackInAcceptanceHP, "nTrackInAcceptanceHP/I");
    ntEvt->SetAutoFlush(200000);
    ntEvt->SetAutoSave(200000000);
    ntEvt->SetDirectory(outf);
  }

  ~DataAnalyzer() {
    deleteHistograms();
    delete nt;
    delete ntEvt;
    inf->Close();
    outf->Close();
    delete MDzeroUPC;
  }

  void analyze(Parameters &par) {
    outf->cd();
    hDmass = new TH1D(Form("hDmass%s", title.c_str()), "", DMASS_HISTNBINS, DMASS_HISTMIN, DMASS_HISTMAX);
    hDmass24003Bins = new TH1D(Form("hDmass%s_24003Bins", title.c_str()), "", DMASS_HISTNBINS_24003, DMASS_HISTMIN_24003, DMASS_HISTMAX_24003);
    hDmass25002Bins = new TH1D(Form("hDmass%s_25002Bins", title.c_str()), "", DMASS_HISTNBINS_25002, DMASS_HISTMIN_25002, DMASS_HISTMAX_25002);
    hDenEvtEff = new TH1D(Form("hDenEvtEff%s", title.c_str()), "", 1, 0.5, 1.5);
    hNumEvtEff = new TH1D(Form("hNumEvtEff%s", title.c_str()), "", 1, 0.5, 1.5);
    hRatioEvtEff = (TH1D*) hNumEvtEff->Clone("hRatioEvtEff");
    hDenDEff = new TH1D(Form("hDenDEff%s", title.c_str()), "", 1, 0.5, 1.5);
    hNumDEff = new TH1D(Form("hNumDEff%s", title.c_str()), "", 1, 0.5, 1.5);
    hRatioDEff = (TH1D*) hNumDEff->Clone("hRatioDEff");
    hHFEmaxMinus_vs_EvtMult = nullptr;
    hHFEmaxPlus_vs_EvtMult = nullptr;

    bool doHFEmaxDistributions=(par.DoSystRapGap > 9);
    if (doHFEmaxDistributions) {
      hHFEmaxMinus_vs_EvtMult = new TH2D(Form("hHFEmaxMinus_vs_EvtMult%s", title.c_str()), "", 80, 0, 20, 200, 0, 1000);
      hHFEmaxPlus_vs_EvtMult = new TH2D(Form("hHFEmaxPlus_vs_EvtMult%s", title.c_str()), "", 80, 0, 20, 200, 0, 1000);

      hHFEmaxMinus_vs_EvtMult->Sumw2();
      hHFEmaxPlus_vs_EvtMult->Sumw2();
    }

    hDmass->Sumw2();
    hDmass24003Bins->Sumw2();
    hDmass25002Bins->Sumw2();
    hDenEvtEff->Sumw2();
    hNumEvtEff->Sumw2();
    hDenDEff->Sumw2();
    hNumDEff->Sumw2();

    WeightHandler2D GptGyWH;
    if (par.DoGptGyReweighting)
    {
      GptGyWH.LoadFromFile(par.GptGyWeightFileName);
    }

    WeightHandler1D MultWH;
    if (par.DoMultReweighting)
    {
      MultWH.LoadFromFile(par.MultWeightFileName);
    }

    par.printParameters();
    unsigned long nEntry = MDzeroUPC->GetEntries() * par.scaleFactor;
    ProgressBar Bar(cout, nEntry);
    Bar.SetStyle(1);

    for (unsigned long i = 0; i < nEntry; i++) {
      MDzeroUPC->GetEntry(i);
      if (i % 1000 == 0) {
        Bar.Update(i);
        Bar.Print();
      }

      // Check if the event is a signal MC event, a.k.a., having at least one gen-level D candidate that falls into the (pt,y) bin of interest
      bool isSigMCEvt = false;
      double leadingGpt = -999.;
      double leadingGy  = -999.;
      if (!par.IsData){
        for (unsigned long j = 0; j < MDzeroUPC->Gpt->size(); j++) {
          if (MDzeroUPC->GisSignalCalc->at(j) == false)
            continue;
          if (MDzeroUPC->Gpt->at(j) > leadingGpt)
          {
            leadingGpt = MDzeroUPC->Gpt->at(j);
            leadingGy  = MDzeroUPC->Gy ->at(j);
          }

          if (MDzeroUPC->Gpt->at(j) < par.MinDzeroPT)
            continue;
          if (MDzeroUPC->Gpt->at(j) > par.MaxDzeroPT)
            continue;
          if (MDzeroUPC->Gy->at(j) < par.MinDzeroY)
            continue;
          if (MDzeroUPC->Gy->at(j) > par.MaxDzeroY)
            continue;
          isSigMCEvt = true;
        }
      }

      double GptGyWeight = 1.0;
      double MultWeight  = 1.0;
      if (!par.IsData)
      {
        if (par.DoGptGyReweighting) GptGyWeight = GptGyWH.GetWeight(leadingGpt, leadingGy, (!par.IsGammaN));
        if (par.DoMultReweighting ) MultWeight  = MultWH.GetWeight(MDzeroUPC->nTrackInAcceptanceHP);
      }

      if (!par.IsData && isSigMCEvt) hDenEvtEff->Fill(1, GptGyWeight*MultWeight);

      const bool mcInclusiveSkipBase =
          par.MCInclusiveOnlyBaseCuts && !par.IsData;
      // MC_inclusive: eventSelection only (no passesBaseCutEvent); data/MC otherwise: + passesBaseCutEvent
      const bool eventPasses =
          mcInclusiveSkipBase
              ? eventSelection(MDzeroUPC, par)
              : (eventSelection(MDzeroUPC, par) &&
                 passesBaseCutEvent(MDzeroUPC, par));

      if (eventPasses) {
        if (!par.IsData && isSigMCEvt) hNumEvtEff->Fill(1, GptGyWeight*MultWeight);
        bool doTrkPtErrFilter = false;
        bool doTrkHitFilter = false;
        if (par.DoTrkPtErrFilter &&
            MDzeroUPC->Dtrk1PtErr != nullptr &&
            MDzeroUPC->Dtrk2PtErr != nullptr) doTrkPtErrFilter = true;
        if (par.DoTrkHitFilter &&
            MDzeroUPC->Dtrk1PixelHit != nullptr &&
            MDzeroUPC->Dtrk1StripHit != nullptr &&
            MDzeroUPC->Dtrk2PixelHit != nullptr &&
            MDzeroUPC->Dtrk2StripHit != nullptr) doTrkHitFilter = true;
        bool filledEvtTreeThisEvent = false;
        for (unsigned long j = 0; j < MDzeroUPC->Dsize; j++) {
          if (MDzeroUPC->Dpt->at(j) < par.MinDzeroPT)
            continue;
          if (MDzeroUPC->Dpt->at(j) > par.MaxDzeroPT)
            continue;
          if (MDzeroUPC->Dy->at(j) < par.MinDzeroY)
            continue;
          if (MDzeroUPC->Dy->at(j) > par.MaxDzeroY)
            continue;
          // MC_inclusive: skip passesBaseCutCandidate; data & other MC: base reco-quality cut
          if (!mcInclusiveSkipBase && !passesBaseCutCandidate(MDzeroUPC, (int)j))
            continue;
          if (par.DextraSelectionMask != 0 &&
              !passesDextraSelectionMask(MDzeroUPC, (int)j, par.DextraSelectionMask))
            continue;
          if (par.DoSystD==0 && MDzeroUPC->DpassCut23PAS->at(j) == false) continue;
          if (par.DoSystD==1 && MDzeroUPC->DpassCut23PASSystDsvpvSig->at(j) == false) continue;
          if (par.DoSystD==2 && MDzeroUPC->DpassCut23PASSystDtrkPt->at(j) == false) continue;
          if (par.DoSystD==3 && MDzeroUPC->DpassCut23PASSystDalpha->at(j) == false) continue;
          if (par.DoSystD==4 && MDzeroUPC->DpassCut23PASSystDchi2cl->at(j) == false) continue;
          // Keep candidates with Dmva_BDT > BDTCutValue (strict)
          if (par.DoBDTD && MDzeroUPC->Dmva_BDT != nullptr &&
              MDzeroUPC->Dmva_BDT->at(j) <= par.BDTCutValue)
            continue;
//          if (par.DoSystD==0 && MDzeroUPC->DpassCutDefault->at(j) == false) continue;
//          if (par.DoSystD==1 && MDzeroUPC->DpassCutSystDsvpvSig->at(j) == false) continue;
//          if (par.DoSystD==2 && MDzeroUPC->DpassCutSystDtrkPt->at(j) == false) continue;
//          if (par.DoSystD==3 && MDzeroUPC->DpassCutSystDalpha->at(j) == false) continue;
//          if (par.DoSystD==4 && MDzeroUPC->DpassCutSystDchi2cl->at(j) == false) continue;
          if (doTrkPtErrFilter && (
              (MDzeroUPC->Dtrk1PtErr->at(j) / MDzeroUPC->Dtrk1Pt->at(j)) > 0.1 ||
              (MDzeroUPC->Dtrk2PtErr->at(j) / MDzeroUPC->Dtrk2Pt->at(j)) > 0.1
            )) continue;
          if (doTrkHitFilter && (
              (MDzeroUPC->Dtrk1PixelHit->at(j) + MDzeroUPC->Dtrk1StripHit->at(j)) < 11 ||
              (MDzeroUPC->Dtrk2PixelHit->at(j) + MDzeroUPC->Dtrk2StripHit->at(j)) < 11
            )) continue;

          if (!filledEvtTreeThisEvent) {
            evt_nTrackInAcceptanceHP = MDzeroUPC->nTrackInAcceptanceHP;
            ntEvt->Fill();
            filledEvtTreeThisEvent = true;
          }

          hDmass->Fill((*MDzeroUPC->Dmass)[j]);
          hDmass24003Bins->Fill((*MDzeroUPC->Dmass)[j]);
          hDmass25002Bins->Fill((*MDzeroUPC->Dmass)[j]);
          float dmvaBdt =
              (MDzeroUPC->Dmva_BDT != nullptr) ? MDzeroUPC->Dmva_BDT->at(j) : -999.f;
          float ddtheta =
              (MDzeroUPC->Ddtheta != nullptr) ? MDzeroUPC->Ddtheta->at(j) : -999.f;
          float dsv2 =
              (MDzeroUPC->DsvpvDistance_2D != nullptr) ? MDzeroUPC->DsvpvDistance_2D->at(j) : -999.f;
          float dse2 =
              (MDzeroUPC->DsvpvDisErr_2D != nullptr) ? MDzeroUPC->DsvpvDisErr_2D->at(j) : -999.f;
          float t1pt = (MDzeroUPC->Dtrk1Pt != nullptr) ? MDzeroUPC->Dtrk1Pt->at(j) : -999.f;
          float t1pe = (MDzeroUPC->Dtrk1PtErr != nullptr) ? MDzeroUPC->Dtrk1PtErr->at(j) : -999.f;
          float t2pt = (MDzeroUPC->Dtrk2Pt != nullptr) ? MDzeroUPC->Dtrk2Pt->at(j) : -999.f;
          float t2pe = (MDzeroUPC->Dtrk2PtErr != nullptr) ? MDzeroUPC->Dtrk2PtErr->at(j) : -999.f;

          ntRow.Dmass = (*MDzeroUPC->Dmass)[j];
          ntRow.Dgen = par.IsData ? 0.f : (*MDzeroUPC->Dgen)[j];
          ntRow.Dpt = (*MDzeroUPC->Dpt)[j];
          ntRow.Dy = (*MDzeroUPC->Dy)[j];
          ntRow.Dalpha = (*MDzeroUPC->Dalpha)[j];
          ntRow.Dchi2cl = (*MDzeroUPC->Dchi2cl)[j];
          ntRow.DsvpvDistance = (*MDzeroUPC->DsvpvDistance)[j];
          ntRow.DsvpvDisErr = (*MDzeroUPC->DsvpvDisErr)[j];
          ntRow.Dmva_BDT = dmvaBdt;
          ntRow.Ddtheta = ddtheta;
          ntRow.DsvpvDistance_2D = dsv2;
          ntRow.DsvpvDisErr_2D = dse2;
          ntRow.Dtrk1Pt = t1pt;
          ntRow.Dtrk1PtErr = t1pe;
          ntRow.Dtrk2Pt = t2pt;
          ntRow.Dtrk2PtErr = t2pe;
          nt->Fill();

          if (!par.IsData && MDzeroUPC->Dgen->at(j) == 23333) {
            hNumDEff->Fill(1, GptGyWeight*MultWeight);
          }

          // Fill HF E_max distributions for data
          if(doHFEmaxDistributions && par.IsData) {
            hHFEmaxMinus_vs_EvtMult->Fill(MDzeroUPC->HFEMaxMinus, MDzeroUPC->nTrackInAcceptanceHP);
            hHFEmaxPlus_vs_EvtMult->Fill(MDzeroUPC->HFEMaxPlus, MDzeroUPC->nTrackInAcceptanceHP);
          }
        } // end of reco-level Dzero loop

        if (!par.IsData && isSigMCEvt) {
          for (unsigned long j = 0; j < MDzeroUPC->Gpt->size(); j++) {
            if (MDzeroUPC->Gpt->at(j) < par.MinDzeroPT)
              continue;
            if (MDzeroUPC->Gpt->at(j) > par.MaxDzeroPT)
              continue;
            if (MDzeroUPC->Gy->at(j) < par.MinDzeroY)
              continue;
            if (MDzeroUPC->Gy->at(j) > par.MaxDzeroY)
              continue;
            if (MDzeroUPC->GisSignalCalc->at(j) == false)
              continue;
            hDenDEff->Fill(1, GptGyWeight*MultWeight);
            // Fill HF E_max distributions for MC
            if(doHFEmaxDistributions) {
              hHFEmaxMinus_vs_EvtMult->Fill(MDzeroUPC->HFEMaxMinus, MDzeroUPC->nTrackInAcceptanceHP);
              hHFEmaxPlus_vs_EvtMult->Fill(MDzeroUPC->HFEMaxPlus, MDzeroUPC->nTrackInAcceptanceHP);
            }
          } // end of gen-level Dzero loop
        }   // end of gen-level Dzero loop
      }   // end of event selection
    }     // end of event loop
  }       // end of analyze

  void writeHistograms(TFile *outf) {
    outf->cd();
    smartWrite(hDmass);
    smartWrite(hDmass24003Bins);
    smartWrite(hDmass25002Bins);
    hRatioEvtEff->Divide(hNumEvtEff, hDenEvtEff, 1, 1, "B");
    hRatioDEff->Divide(hNumDEff, hDenDEff, 1, 1, "B");
    hDenEvtEff->Write();
    hNumEvtEff->Write();
    hRatioEvtEff->Write();
    hDenDEff->Write();
    hNumDEff->Write();
    hRatioDEff->Write();
    smartWrite(nt);
    smartWrite(ntEvt);
    smartWrite(hHFEmaxMinus_vs_EvtMult);
    smartWrite(hHFEmaxPlus_vs_EvtMult);
  }

private:
  void deleteHistograms() {
    delete hDmass;
    delete hDmass24003Bins;
    delete hDmass25002Bins;
    delete hDenEvtEff;
    delete hNumEvtEff;
    delete hRatioEvtEff;
    delete hDenDEff;
    delete hNumDEff;
    delete hRatioDEff;
    if (hHFEmaxMinus_vs_EvtMult != nullptr) {
      delete hHFEmaxMinus_vs_EvtMult;
    }
    if (hHFEmaxPlus_vs_EvtMult != nullptr) {
      delete hHFEmaxPlus_vs_EvtMult;
    }
  }
};

//============================================================//
// Main analysis
//============================================================//
int main(int argc, char *argv[]) {
  if (printHelpMessage(argc, argv))
    return 0;
  CommandLine CL(argc, argv);
  int Year = CL.GetDouble("Year", 0); // Data year that sample is associated with (optional for now)
  float MinDzeroPT = CL.GetDouble("MinDzeroPT", 2);  // Minimum Dzero transverse momentum threshold for Dzero selection.
  float MaxDzeroPT = CL.GetDouble("MaxDzeroPT", 5);  // Maximum Dzero transverse momentum threshold for Dzero selection.
  float MinDzeroY = CL.GetDouble("MinDzeroY", -2);   // Minimum Dzero rapidity threshold for Dzero selection.
  float MaxDzeroY = CL.GetDouble("MaxDzeroY", +2);   // Maximum Dzero rapidity threshold for Dzero selection.
  bool IsGammaN = CL.GetBool("IsGammaN", true);      // GammaN analysis (or NGamma)
  int TriggerChoice = CL.GetInt("TriggerChoice", 2); // 0 = no trigger sel, 1 = isL1ZDCOr, 2 = isL1ZDCXORJet8, 3 = isL1ZDCXORJet12, 4 = isL1ZDCXORJet16,
  int BkgFilterChoice = CL.GetInt("BkgFilterChoice", 1); // 1 = CCF + halo; 2 = halo only
  float scaleFactor = CL.GetDouble("scaleFactor", 1); // Scale factor for the number of events to be processed.
  int DoSystRapGap = CL.GetInt("DoSystRapGap", 0);   // Systematic study: apply the alternative event selections
                                                     // 0 = nominal, 1 = tight, -1: loose
                                                     // 9 < DoSystRapGap: use custom HF energy threshold, the threshold value will be DoSystRapGap/10.
  bool DoBDTD = CL.GetBool("DoBDTD", false);         // Apply Dmva_BDT cut at BDTCutValue; forces DoSystD = -1
  float BDTCutValue = CL.GetDouble("BDTCutValue", 0.f); // BDT classifier working point when DoBDTD is true
  int DoSystD = CL.GetInt("DoSystD", 0);             // Systematic study: apply the alternative D selections
                                                     // 0 = nominal, 1 = Dsvpv variation, 2: DtrkPt variation
                                                     // 3 = Dalpha variation, 4: Dchi2cl variation
                                                     // -1 = BDT mode (set automatically when DoBDTD is true)
  if (DoBDTD)
    DoSystD = -1;
  bool DoGptGyReweighting   = CL.GetBool  ("DoGptGyReweighting", false);
  string GptGyWeightFileName= CL.Get      ("GptGyWeightFileName", "../../WeightHandler/20250305_DzeroUPC_GptGyWeight/Weights/testWeight.root");
  bool DoMultReweighting   = CL.GetBool  ("DoMultReweighting", false);
  string MultWeightFileName= CL.Get      ("MultWeightFileName", "../../WeightHandler/20250312_DzeroUPC_multiplicityWeight/Weights/testWeight.root");
  float HFMaxRapDefn = CL.GetDouble("HFMaxRapDefn", -1);  // HF max rapidity definition: 5 for HFEmaxPlus(Minus)_eta5, 6 for HFEmaxPlus(Minus)_forest, 5.2 for HFEmaxPlus(Minus), -1 for samples with only HFEmaxPlus(Minus) branch
  bool DoTrkPtErrFilter = CL.GetBool("DoTrkPtErrFilter", true);
  bool DoTrkHitFilter = CL.GetBool("DoTrkHitFilter", true);

  bool IsData = CL.GetBool("IsData", 0);              // Data or MC
  bool MCInclusiveOnlyBaseCuts = CL.GetBool("MCInclusiveOnlyBaseCuts", false);
  int DextraSelectionMask = CL.GetInt("DextraSelectionMask", 0);
  Parameters par(MinDzeroPT, MaxDzeroPT, MinDzeroY, MaxDzeroY, IsGammaN,
                 TriggerChoice, BkgFilterChoice, IsData, Year, scaleFactor,
                 DoSystRapGap, DoSystD,
                 DoGptGyReweighting, GptGyWeightFileName,
                 DoMultReweighting, MultWeightFileName,
                 HFMaxRapDefn, DoTrkPtErrFilter, DoTrkHitFilter,
                 DoBDTD, BDTCutValue, MCInclusiveOnlyBaseCuts, DextraSelectionMask);
  par.input = CL.Get("Input", "mergedSample.root"); // Input file
  par.output = CL.Get("Output", "output.root");     // Output file
  par.nThread = CL.GetInt("nThread", 1);            // The number of threads to be used for parallel processing.
  par.nChunk =
      CL.GetInt("nChunk", 1); // Specifies which chunk (segment) of the data to process, used in parallel processing.
  if (checkError(par))
    return -1;
  std::cout << "Parameters are set" << std::endl;
  // Analyze Data
  DataAnalyzer analyzer(par.input.c_str(), par.output.c_str(), "");
  analyzer.analyze(par);
  analyzer.writeHistograms(analyzer.outf);
  saveParametersToHistograms(par, analyzer.outf);
  cout << "done!" << analyzer.outf->GetName() << endl;

  return 0;
}
