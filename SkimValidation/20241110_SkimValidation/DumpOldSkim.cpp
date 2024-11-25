/* + Descriptions:
 *		Macro to dump the old skim format into .txt and histograms in .root, for the validations btw two skims
 * + Output:
 * 		+ .txt:  printing of event-, reco-D- and gen-D-level quantities
 * 		+ .root: contains histograms defined in ValidateHist.h
 */
#include <cstdio>
#include <iostream>
#include <string>
using namespace std;

#include "TFile.h"
#include "TH2D.h"
#include "TTree.h"

#include "CommandLine.h"
#include "CommonFunctions.h"
#include "Messenger.h"
#include "ProgressBar.h"

#include "ValidateHist.h"

// copy from main analysis
void calcRapidityGapsInput(std::vector<float> *pfE, std::vector<float> *pfPt, std::vector<float> *pfEta,
                           std::vector<int> *pfId, int &nPFHFMinus_, int &nPFHFPlus_);

bool tightsel(int j, DzeroTreeMessenger &MDzero, std::string varySel = "");

int findBestVertex(PbPbUPCTrackTreeMessenger &MTrackPbPbUPC) {
  int BestVertex = -1;

  for (int i = 0; i < MTrackPbPbUPC.nVtx; i++) {
    if (BestVertex < 0 || MTrackPbPbUPC.ptSumVtx->at(i) > MTrackPbPbUPC.ptSumVtx->at(BestVertex))
      BestVertex = i;
  }

  return BestVertex;
}
int main(int argc, char *argv[]) {
  CommandLine CL(argc, argv);

  string InputFileName = CL.Get("Input");
  string OutputName = CL.Get("Output");
  bool IsData = CL.GetBool("IsData");

  double Fraction = CL.GetDouble("Fraction", 1.00);
  bool ApplyEventSelection = CL.GetBool("ApplyEventSelection", true);
  bool SkimDzeroPresence = CL.GetBool("SkimDzeroPresence", false);
  double SkimDzeroPresenceAbovePTMin = CL.GetDouble("SkimDzeroPresenceAbovePTMin", 2.);
  bool ApplyDSelection = CL.GetBool("ApplyDSelection", false);
  double MinDzeroPT = CL.GetDouble("MinDzeroPT", 2.);
  double MaxDzeroPT = CL.GetDouble("MaxDzeroPT", 12.);
  double MinDzeroY = CL.GetDouble("MinDzeroY", -2.);
  double MaxDzeroY = CL.GetDouble("MaxDzeroY", 2.);
  string PFTreeName = CL.Get("PFTree", "particleFlowAnalyser/pftree");
  string DGenTreeName = CL.Get("DGenTree", "Dfinder/ntGen");

  TFile InputFile(InputFileName.c_str());
  HiEventTreeMessenger MEvent(InputFile);
  PbPbUPCTrackTreeMessenger MTrackPbPbUPC(InputFile);
  GenParticleTreeMessenger MGen(InputFile);
  PFTreeMessenger MPF(InputFile, PFTreeName);
  SkimTreeMessenger MSkim(InputFile);
  TriggerTreeMessenger MTrigger(InputFile);
  DzeroTreeMessenger MDzero(InputFile);
  DzeroGenTreeMessenger MDzeroGen(InputFile, DGenTreeName);
  ZDCTreeMessenger MZDC(InputFile);
  METFilterTreeMessenger MMETFilter(InputFile);

  TTree *tt = (TTree *)InputFile.Get("ppTracks/trackTree");
  int Run;
  int Event;
  int Lumi;
  tt->SetBranchAddress("nRun", &Run);
  tt->SetBranchAddress("nLumi", &Lumi);
  tt->SetBranchAddress("nEv", &Event);

  TTree *ht = (TTree *)InputFile.Get("hltanalysis/HltTree");
  int HLT_HIUPC_ZDC1nOR_SinglePixelTrackLowPt_MaxPixelCluster400_v8;
  int HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000_v8;
  // int HLT_HIUPC_SingleJet8_ZDC1nXOR_MaxPixelCluster50000;
  int HLT_HIUPC_SingleJet8_ZDC1nAsymXOR_MaxPixelCluster50000_v1;
  ht->SetBranchAddress("HLT_HIUPC_ZDC1nOR_SinglePixelTrackLowPt_MaxPixelCluster400_v8",
                       &HLT_HIUPC_ZDC1nOR_SinglePixelTrackLowPt_MaxPixelCluster400_v8);
  ht->SetBranchAddress("HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000_v8",
                       &HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000_v8);
  // ht->SetBranchAddress("HLT_HIUPC_SingleJet8_ZDC1nXOR_MaxPixelCluster50000",
  // &HLT_HIUPC_SingleJet8_ZDC1nXOR_MaxPixelCluster50000);
  ht->SetBranchAddress("HLT_HIUPC_SingleJet8_ZDC1nAsymXOR_MaxPixelCluster50000_v1",
                       &HLT_HIUPC_SingleJet8_ZDC1nAsymXOR_MaxPixelCluster50000_v1);

  FILE *outfile = fopen((OutputName + ".txt").c_str(), "w");
  if (outfile == nullptr) {
    perror("Error opening outfile");
    exit(EXIT_FAILURE);
  }

  ValidateHist v((OutputName + ".root").c_str(), "recreate");

  int EntryCount = MEvent.GetEntries() * Fraction;
  ProgressBar Bar(cout, EntryCount);
  Bar.SetStyle(-1);

  for (int iE = 0; iE < EntryCount; iE++) {
    if (EntryCount < 300 || (iE % (EntryCount / 250)) == 0) {
      Bar.Update(iE);
      Bar.Print();
    }

    MEvent.GetEntry(iE);
    MGen.GetEntry(iE);
    MTrackPbPbUPC.GetEntry(iE);
    MPF.GetEntry(iE);
    MSkim.GetEntry(iE);
    MTrigger.GetEntry(iE);
    MDzero.GetEntry(iE);
    if (!IsData)
      MDzeroGen.GetEntry(iE);
    MZDC.GetEntry(iE);
    MMETFilter.GetEntry(iE);
    tt->GetEntry(iE);
    ht->GetEntry(iE);

    ////////// event selection //////////
    bool isL1ZDCOr = HLT_HIUPC_ZDC1nOR_SinglePixelTrackLowPt_MaxPixelCluster400_v8 ||
                     HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000_v8;
    bool isL1ZDCXORJet8 = HLT_HIUPC_SingleJet8_ZDC1nAsymXOR_MaxPixelCluster50000_v1;

    bool selectedBkgFilter = MSkim.ClusterCompatibilityFilter == 1 && MMETFilter.cscTightHalo2015Filter;
    bool selectedVtxFilter = MSkim.PVFilter == 1 && fabs(MTrackPbPbUPC.zVtx->at(0)) < 15. && MTrackPbPbUPC.nVtx <= 3;

    bool ZDCgammaN = (MZDC.sumMinus > 1000. && MZDC.sumPlus <= 1100.);
    bool ZDCNgamma = (MZDC.sumMinus <= 1000. && MZDC.sumPlus > 1100.);

    int _nPFHFMinus, _nPFHFPlus;
    calcRapidityGapsInput(MPF.E, MPF.PT, MPF.Eta, MPF.ID, _nPFHFMinus, _nPFHFPlus);
    bool gapgammaN = _nPFHFPlus == 0;
    bool gapNgamma = _nPFHFMinus == 0;

    bool gammaN = ZDCgammaN && gapgammaN;
    bool Ngamma = ZDCNgamma && gapNgamma;

    bool DRequirement;
    if (!SkimDzeroPresence) {
      DRequirement = true;
    } else {
      DRequirement = false;
      for (int iD = 0; iD < MDzero.Dsize; ++iD) {
        if (MDzero.Dpt[iD] > SkimDzeroPresenceAbovePTMin) {
          DRequirement = true;
          break;
        }
      }
    }

    bool passEventSelection = (IsData) ? (isL1ZDCOr || isL1ZDCXORJet8) && selectedBkgFilter && selectedVtxFilter &&
                                             (gammaN || Ngamma) && DRequirement
                                       : true; // don't skim on MC
    if (ApplyEventSelection && !passEventSelection)
      continue;
    ////////// event selection //////////

    int BestVertex = findBestVertex(MTrackPbPbUPC);

    if (IsData) {
      fprintf(outfile,
              // "[Entry %d] "
              "Run %d, Lumi %d, Event %d\n"
              "isL1ZDCOr %o, isL1ZDCXORJet8 %o\n"
              "selectedBkgFilter %d, selectedVtxFilter %d\n"
              "gammaN %d, Ngamma %d\n"
              "VX %f VY %f VZ %f\n"
              "VXError %f VYError %f VZError %f\n"
              "Dsize %d\n",
              // iE,
              Run, Lumi, Event, isL1ZDCOr, isL1ZDCXORJet8, selectedBkgFilter, selectedVtxFilter, gammaN, Ngamma,
              MTrackPbPbUPC.xVtx->at(BestVertex), MTrackPbPbUPC.yVtx->at(BestVertex),
              MTrackPbPbUPC.zVtx->at(BestVertex), MTrackPbPbUPC.xErrVtx->at(BestVertex),
              MTrackPbPbUPC.yErrVtx->at(BestVertex), MTrackPbPbUPC.zErrVtx->at(BestVertex), MDzero.Dsize);

      v.FillEventInfo(Run, Lumi, Event, MTrackPbPbUPC.xVtx->at(BestVertex), MTrackPbPbUPC.yVtx->at(BestVertex),
                      MTrackPbPbUPC.zVtx->at(BestVertex), MTrackPbPbUPC.xErrVtx->at(BestVertex),
                      MTrackPbPbUPC.yErrVtx->at(BestVertex), MTrackPbPbUPC.zErrVtx->at(BestVertex), MDzero.Dsize);
    } else {
      fprintf(outfile,
              // "[Entry %d] "
              "Run %d, Lumi %d, Event %d\n"
              "VX %f VY %f VZ %f\n"
              "VXError %f VYError %f VZError %f\n"
              "Dsize %d Gsize %d\n",
              // iE,
              Run, Lumi, Event, MTrackPbPbUPC.xVtx->at(BestVertex), MTrackPbPbUPC.yVtx->at(BestVertex),
              MTrackPbPbUPC.zVtx->at(BestVertex), MTrackPbPbUPC.xErrVtx->at(BestVertex),
              MTrackPbPbUPC.yErrVtx->at(BestVertex), MTrackPbPbUPC.zErrVtx->at(BestVertex), MDzero.Dsize,
              MDzeroGen.Gsize);

      v.FillEventInfo(Run, Lumi, Event, MTrackPbPbUPC.xVtx->at(BestVertex), MTrackPbPbUPC.yVtx->at(BestVertex),
                      MTrackPbPbUPC.zVtx->at(BestVertex), MTrackPbPbUPC.xErrVtx->at(BestVertex),
                      MTrackPbPbUPC.yErrVtx->at(BestVertex), MTrackPbPbUPC.zErrVtx->at(BestVertex), MDzero.Dsize,
                      MDzeroGen.Gsize);
    }

    v.FillEventSelectionInfo(isL1ZDCOr, isL1ZDCXORJet8, selectedBkgFilter, selectedVtxFilter, gammaN, Ngamma);

    for (int iD = 0; iD < MDzero.Dsize; ++iD) {

      bool passDSelection = tightsel(iD, MDzero) && MDzero.Dpt[iD] >= MinDzeroPT && MDzero.Dpt[iD] <= MaxDzeroPT &&
                            MDzero.Dy[iD] >= MinDzeroY && MDzero.Dy[iD] <= MaxDzeroY;
      if (ApplyDSelection && !passDSelection)
        continue;

      fprintf(outfile,
              "[D %d] "
              "Dpt %f Dy %f Dmass %f\n"
              "Dtrk1Pt %f Dtrk2Pt %f\n"
              "Dchi2cl %f DsvpvDistance %f DsvpvDisErr %f\n"
              "Dalpha %f DsvpvDistance_2D %f DsvpvDisErr_2D %f\n"
              "Ddtheta %f\n",
              iD, MDzero.Dpt[iD], MDzero.Dy[iD], MDzero.Dmass[iD], MDzero.Dtrk1Pt[iD], MDzero.Dtrk2Pt[iD],
              MDzero.Dchi2cl[iD], MDzero.DsvpvDistance[iD], MDzero.DsvpvDisErr[iD], MDzero.Dalpha[iD],
              MDzero.DsvpvDistance_2D[iD], MDzero.DsvpvDisErr_2D[iD], MDzero.Ddtheta[iD]);

      v.FillRecoDInfo(MDzero.Dpt[iD], MDzero.Dy[iD], MDzero.Dmass[iD], MDzero.Dtrk1Pt[iD], MDzero.Dtrk2Pt[iD],
                      MDzero.Dchi2cl[iD], MDzero.DsvpvDistance[iD], MDzero.DsvpvDisErr[iD], MDzero.Dalpha[iD],
                      MDzero.DsvpvDistance_2D[iD], MDzero.DsvpvDisErr_2D[iD], MDzero.Ddtheta[iD]);

      if (!IsData) {
        bool isSignalGenMatched = MDzero.Dgen[iD] == 23333 && MDzero.Dgenpt[iD] > 0.;
        bool isPromptGenMatched = isSignalGenMatched && (MDzero.DgenBAncestorpdgId[iD] == 0);
        bool isFeeddownGenMatched =
            isSignalGenMatched && ((MDzero.DgenBAncestorpdgId[iD] >= 500 && MDzero.DgenBAncestorpdgId[iD] < 600) ||
                                   (MDzero.DgenBAncestorpdgId[iD] > -600 && MDzero.DgenBAncestorpdgId[iD] <= -500));

        fprintf(outfile, "Dgen %d DisSignalCalc %o DisSignalCalcPrompt %o DisSignalCalcFeeddown %o\n", MDzero.Dgen[iD],
                isSignalGenMatched, isPromptGenMatched, isFeeddownGenMatched);

        v.FillRecoDGenMatchedInfo(MDzero.Dgen[iD], (int)isSignalGenMatched, (int)isPromptGenMatched,
                                  (int)isFeeddownGenMatched);
      }
    }

    if (!IsData) {
      for (int iG = 0; iG < MDzeroGen.Gsize; ++iG) {
        bool isSignalGen = (MDzeroGen.GisSignal[iG] == 1 || MDzeroGen.GisSignal[iG] == 2) && MDzeroGen.Gpt[iG] > 0.;
        bool isPromptGen = isSignalGen && MDzeroGen.GBAncestorpdgId[iG] == 0;
        bool isFeeddownGen =
            isSignalGen && ((MDzeroGen.GBAncestorpdgId[iG] >= 500 && MDzeroGen.GBAncestorpdgId[iG] < 600) ||
                            (MDzeroGen.GBAncestorpdgId[iG] > -600 && MDzeroGen.GBAncestorpdgId[iG] <= -500));

        fprintf(outfile,
                "[G %d] "
                "Gpt %f Gy %f\n"
                "GisSignalCalc %d GisSignalCalcPrompt %d GisSignalCalcFeeddown %d\n",
                iG, MDzeroGen.Gpt[iG], MDzeroGen.Gy[iG], isSignalGen, isPromptGen, isFeeddownGen);

        v.FillGenDInfo(MDzeroGen.Gpt[iG], MDzeroGen.Gy[iG], isSignalGen, isPromptGen, isFeeddownGen);
      }
    }
  }
  v.Write();

  fclose(outfile);

  return 0;
}

// copy from main analysis
void calcRapidityGapsInput(std::vector<float> *pfE, std::vector<float> *pfPt, std::vector<float> *pfEta,
                           std::vector<int> *pfId, int &nPFHFMinus_, int &nPFHFPlus_) {
  // initialize counters
  nPFHFMinus_ = 0;
  nPFHFPlus_ = 0;

  // defining the constant
  const static float pfEMinMinus_ = 8.6;
  const static float pfEMinPlus_ = 9.2;
  const static float pfAEtaMin_ = 3.0;
  const static float pfAEtaMax_ = 5.2;
  const static std::vector<Int_t> pfValidId_ = {6, 7};
  // float minPfPt monitoring for protection
  float minPfPtFound_ = 999999.0;
  float minPfAEtaFound_ = -1.0;
  float maxPfAEtaFound_ = -1.0;

  for (unsigned int pI = 0; pI < pfE->size(); ++pI) {
    // For protection check/record min
    if (pfPt->at(pI) < minPfPtFound_)
      minPfPtFound_ = pfPt->at(pI);

    float aEta = TMath::Abs(pfEta->at(pI));

    // For protection check/record max
    if (aEta > maxPfAEtaFound_)
      maxPfAEtaFound_ = aEta;
    if (aEta < minPfAEtaFound_)
      minPfAEtaFound_ = aEta;

    // Apply kin. cuts
    // Eta cuts
    if (aEta < pfAEtaMin_)
      continue;
    if (aEta >= pfAEtaMax_)
      continue;
    // Eta gated pfEMin cuts
    if (pfEta->at(pI) < 0.0) { // first minus
      if (pfE->at(pI) < pfEMinMinus_)
        continue;
    } else { // Now plus
      if (pfE->at(pI) < pfEMinPlus_)
        continue;
    }

    // Apply ID cut
    bool idIsGood = false;
    for (auto const &goodId : pfValidId_) {
      if (pfId->at(pI) == goodId) {
        idIsGood = true;
        break;
      }
    }
    if (!idIsGood)
      continue;

    // After all cuts passed, increment counter
    if (pfEta->at(pI) < 0.0)
      ++nPFHFMinus_;
    else
      ++nPFHFPlus_;
  }

  return;
}

bool tightsel(int j, DzeroTreeMessenger &MDzero, std::string varySel) {
  using floatVector2D = std::vector<std::vector<float>>;

  // Nominal cut values
  const static floatVector2D DsvpvSigCutValue = {                                // {3.5, 3.5, 3.5, 3.5, 3.5, 3.5},
                                                 {2.5, 2.5, 2.5, 2.5, 2.5, 2.5}, // check with gm
                                                 {3.5, 3.5, 3.5, 3.5, 3.5, 3.5},
                                                 {3.5, 3.5, 3.5, 3.5, 3.5, 3.5}};
  const static floatVector2D Dchi2clCutValue = {
      {0.1, 0.1, 0.1, 0.1, 0.1, 0.1}, {0.1, 0.1, 0.1, 0.1, 0.1, 0.1}, {0.1, 0.1, 0.1, 0.1, 0.1, 0.1}};
  const static floatVector2D DalphaCutValue = {
      {0.2, 0.2, 0.4, 0.4, 0.2, 0.2}, {0.25, 0.25, 0.35, 0.35, 0.25, 0.25}, {0.25, 0.25, 0.4, 0.4, 0.25, 0.25}};
  const static floatVector2D DdthetaCutValue = {
      {0.3, 0.3, 0.5, 0.5, 0.3, 0.3}, {0.3, 0.3, 0.3, 0.3, 0.3, 0.3}, {0.3, 0.3, 0.3, 0.3, 0.3, 0.3}};
  const static floatVector2D Dtrk1PtCutValue = {
      {1.0, 1.0, 1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0, 1.0, 1.0}};
  const static floatVector2D Dtrk2PtCutValue = {
      {1.0, 1.0, 1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0, 1.0, 1.0}};
  static const int nPt = 3;
  static const int nY = 6;
  float ptBins[nPt + 1] = {2.0, 5.0, 8.0, 999.0};
  float yBins[nY + 1] = {-2.4, -2.0, -1.0, 0.0, 1.0, 2.0, 2.4};
  TH2D Dtrk1PtCut("Dtrk1PtCut", "Dtrk1PtCut", nPt, ptBins, nY, yBins);
  TH2D Dtrk2PtCut("Dtrk2PtCut", "Dtrk2PtCut", nPt, ptBins, nY, yBins);
  TH2D DsvpvSigCut("DsvpvSigCut", "DsvpvSigCut", nPt, ptBins, nY, yBins);
  TH2D DalphaCut("DalphaCut", "DalphaCut", nPt, ptBins, nY, yBins);
  TH2D DdthetaCut("DdthetaCut", "DdthetaCut", nPt, ptBins, nY, yBins);
  TH2D Dchi2clCut("Dchi2clCut", "Dchi2clCut", nPt, ptBins, nY, yBins);
  TH2D Dtrk1PtVariedCut("Dtrk1PtVariedCut", "Dtrk1PtVariedCut", nPt, ptBins, nY, yBins);
  TH2D Dtrk2PtVariedCut("Dtrk2PtVariedCut", "Dtrk2PtVariedCut", nPt, ptBins, nY, yBins);
  TH2D DsvpvSigVariedCut("DsvpvSigVariedCut", "DsvpvSigVariedCut", nPt, ptBins, nY, yBins);
  TH2D DalphaVariedCut("DalphaVariedCut", "DalphaVariedCut", nPt, ptBins, nY, yBins);
  TH2D DdthetaVariedCut("DdthetaVariedCut", "DdthetaVariedCut", nPt, ptBins, nY, yBins);
  TH2D Dchi2clVariedCut("Dchi2clVariedCut", "Dchi2clVariedCut", nPt, ptBins, nY, yBins);
  for (int iPt = 0; iPt < nPt; ++iPt) {
    for (int iY = 0; iY < nY; ++iY) {
      DsvpvSigCut.SetBinContent(iPt + 1, iY + 1, DsvpvSigCutValue[iPt][iY]);
      DalphaCut.SetBinContent(iPt + 1, iY + 1, DalphaCutValue[iPt][iY]);
      DdthetaCut.SetBinContent(iPt + 1, iY + 1, DdthetaCutValue[iPt][iY]);
      Dchi2clCut.SetBinContent(iPt + 1, iY + 1, Dchi2clCutValue[iPt][iY]);
      Dtrk1PtCut.SetBinContent(iPt + 1, iY + 1, Dtrk1PtCutValue[iPt][iY]);
      Dtrk2PtCut.SetBinContent(iPt + 1, iY + 1, Dtrk2PtCutValue[iPt][iY]);
    }
  }

  // static bool init = false;
  // if (!init)
  // {
  //   std::cout << __PRETTY_FUNCTION__ << ": Cuts updated as follows " << std::endl;
  //   std::cout << " DsvpvSigCut Print" << std::endl;
  //   DsvpvSigCut.Print("ALL");
  //   std::cout << " DalphaCut Print" << std::endl;
  //   DalphaCut.Print("ALL");
  //   std::cout << " DdthetaCut Print" << std::endl;
  //   DdthetaCut.Print("ALL");
  //   std::cout << " Dchi2clCut Print" << std::endl;
  //   Dchi2clCut.Print("ALL");
  //   std::cout << " Dtrk1PtCut Print" << std::endl;
  //   Dtrk1PtCut.Print("ALL");
  //   std::cout << " Dtrk2PtCut Print" << std::endl;
  //   Dtrk2PtCut.Print("ALL");

  //   init = true;
  // }

  // IF YOU CHANGE VARIABLES USED BY THIS FUNCTION,
  // PLEASE EDIT tightselVarHI_, tightselVarPP_ APPROPRIATELY!
  // Protections for cut
  //  if(!tightselVarsFound_) return cutProtectionMsg(__PRETTY_FUNCTION__);

  // Valid variations
  std::vector<std::string> validVaryVect = {"SVPV", "alpha", "chi2cl"};
  // Test if a variation requested is valid
  if (varySel.size() != 0) {
    bool validVaryBool = false;
    for (unsigned int vI = 0; vI < validVaryVect.size(); ++vI) {
      if (validVaryVect[vI] == varySel) {
        validVaryBool = true;
        break;
      }
    }

    if (!validVaryBool) {
      std::cout << __PRETTY_FUNCTION__ << " ERROR: Requested variation '" << varySel
                << "' is not valid. Check, returning false always" << std::endl;
      return false;
    }
  }

  float Dpt = MDzero.Dpt[j];
  float Dy = MDzero.Dy[j];
  int bin = DalphaCut.FindBin(Dpt, Dy);

  Double_t DsvpvSigCutLocal = DsvpvSigCut[bin];
  Double_t DalphaCutLocal = DalphaCut[bin];
  Double_t DdthetaCutLocal = DdthetaCut[bin];
  Double_t Dchi2clCutLocal = Dchi2clCut[bin];
  // if(varySel == "SVPV") {
  //   DsvpvSigCutLocal = DsvpvSigVariedCut[bin];
  // }
  // if(varySel == "alpha") {
  //   DalphaCutLocal = DalphaVariedCut[bin];
  //   DdthetaCutLocal = DdthetaVariedCut[bin];
  // }
  // if(varySel == "chi2cl") {
  //   Dchi2clCutLocal = Dchi2clVariedCut[bin];
  // }

  bool cut = MDzero.DsvpvDistance[j] / MDzero.DsvpvDisErr[j] > DsvpvSigCutLocal;
  cut = cut && MDzero.Dalpha[j] < DalphaCutLocal;
  cut = cut && MDzero.Ddtheta[j] < DdthetaCutLocal;
  cut = cut && MDzero.Dchi2cl[j] > Dchi2clCutLocal;
  // if(varySel == "alpha") {
  //   std::cout << bvf_["Dalpha"][j] << " <? " << DalphaCutLocal << std::endl;
  // }

  // if(ishi_) cut = true; // wtf is this
  return cut;
}
