#include <iostream>
using namespace std;

#include "TFile.h"
#include "TLorentzVector.h"
#include "TTree.h"

#include "CommandLine.h"
#include "CommonFunctions.h"
#include "Messenger.h"
#include "ProgressBar.h"

#include "TrackResidualCorrector.h"
#include "tnp_weight.h"
#include "trackingEfficiency2017pp.h"
#include "trackingEfficiency2018PbPb.h"
#include "trackingEfficiency2023PbPb.h"

int main(int argc, char *argv[]);
double GetHFSum(PFTreeMessenger *M);
double GetGenHFSum(GenParticleTreeMessenger *M, int SubEvent = -1);
bool isMuonSelected(SingleMuTreeMessenger *M, int i);

int main(int argc, char *argv[]) {
  string VersionString = "V8";

  CommandLine CL(argc, argv);

  vector<string> InputFileNames = CL.GetStringVector("Input");
  string OutputFileName = CL.Get("Output");

  bool IsData = CL.GetBool("IsData", false);
  bool IsPP = CL.GetBool("IsPP", false);
  int Year = CL.GetInt("Year", 2018);
  double Fraction = CL.GetDouble("Fraction", 1.00);
  double MinJetPT = CL.GetDouble("MinJetPT", 80);
  string PFJetCollection = CL.Get("PFJetCollection", "akCs3PFJetAnalyzer/t");
  string PFTreeName = IsPP ? "pfcandAnalyzer/pfTree" : "particleFlowAnalyser/pftree";
  PFTreeName = CL.Get("PFTree", PFTreeName);

  TFile OutputFile(OutputFileName.c_str(), "RECREATE");
  TTree Tree("Tree", Form("Tree for MuMu tagged jet analysis (%s)", VersionString.c_str()));
  TTree InfoTree("InfoTree", "Information");

  MuMuJetMessenger MMuMuJet;
  MMuMuJet.SetBranch(&Tree);

  for (string InputFileName : InputFileNames) {
    TFile InputFile(InputFileName.c_str());

    HiEventTreeMessenger MEvent(InputFile);
    TrackTreeMessenger MTrackPP(InputFile);
    PbPbTrackTreeMessenger MTrack(InputFile);
    GenParticleTreeMessenger MGen(InputFile);
    PFTreeMessenger MPF(InputFile, PFTreeName);
    MuTreeMessenger MMu(InputFile);
    SingleMuTreeMessenger MSingleMu(InputFile);
    SkimTreeMessenger MSkim(InputFile);
    TriggerTreeMessenger MTrigger(InputFile);
    JetTreeMessenger MJet(InputFile, PFJetCollection);

    int EntryCount = MEvent.GetEntries() * Fraction;
    ProgressBar Bar(cout, EntryCount);
    Bar.SetStyle(-1);

    /////////////////////////////////
    //////// Main Event Loop ////////
    /////////////////////////////////

    for (int iE = 0; iE < EntryCount; iE++) {
      if (EntryCount < 300 || (iE % (EntryCount / 250)) == 0) {
        Bar.Update(iE);
        Bar.Print();
      }
      MEvent.GetEntry(iE);
      MGen.GetEntry(iE);
      if (IsPP == true)
        MTrackPP.GetEntry(iE);
      else
        MTrack.GetEntry(iE);
      MPF.GetEntry(iE);
      MMu.GetEntry(iE);
      MSingleMu.GetEntry(iE);
      MSkim.GetEntry(iE);
      MTrigger.GetEntry(iE);
      MJet.GetEntry(iE);
      MMuMuJet.Clear();

      ////////////////////////////////////////
      ////////// Global event stuff //////////
      ////////////////////////////////////////

      MMuMuJet.Run = MEvent.Run;
      MMuMuJet.Lumi = MEvent.Lumi;
      MMuMuJet.Event = MEvent.Event;
      MMuMuJet.hiBin = MEvent.hiBin;
      MMuMuJet.hiHF = MEvent.hiHF;
      MMuMuJet.NPU = 0;
      if (MEvent.npus->size() == 9)
        MMuMuJet.NPU = MEvent.npus->at(5);
      else if (MEvent.npus->size() > 1)
        MMuMuJet.NPU = MEvent.npus->at(0);
      else
        MMuMuJet.NPU = 0;

      ////////////////////////////
      ////////// Vertex //////////
      ////////////////////////////

      MMuMuJet.NVertex = 0;
      int BestVertex = -1;
      for (int i = 0; i < (IsPP ? MTrackPP.nVtx : MTrack.VX->size()); i++) {
        if (IsPP == true && (BestVertex < 0 || MTrackPP.sumPtVtx[i] > MTrackPP.sumPtVtx[BestVertex]))
          BestVertex = i;
        if (IsPP == false && (BestVertex < 0 || MTrack.VPTSum->at(i) > MTrack.VPTSum->at(BestVertex)))
          BestVertex = i;

        MMuMuJet.NVertex = MMuMuJet.NVertex + 1;
      }

      if (BestVertex >= 0) {
        MMuMuJet.VX = IsPP ? MTrackPP.xVtx[BestVertex] : MTrack.VX->at(BestVertex);
        MMuMuJet.VY = IsPP ? MTrackPP.yVtx[BestVertex] : MTrack.VY->at(BestVertex);
        MMuMuJet.VZ = IsPP ? MTrackPP.zVtx[BestVertex] : MTrack.VZ->at(BestVertex);
        MMuMuJet.VXError = IsPP ? MTrackPP.xVtxErr[BestVertex] : MTrack.VXError->at(BestVertex);
        MMuMuJet.VYError = IsPP ? MTrackPP.yVtxErr[BestVertex] : MTrack.VYError->at(BestVertex);
        MMuMuJet.VZError = IsPP ? MTrackPP.zVtxErr[BestVertex] : MTrack.VZError->at(BestVertex);
      }

      /////////////////////////////////////
      ////////// Event selection //////////
      /////////////////////////////////////

      if (IsPP == true) {
        std::cout << "pp analysis is not yet implemented" << std::endl;
        if (IsData == true) {
          int pprimaryVertexFilter = MSkim.PVFilter;
          int beamScrapingFilter = MSkim.BeamScrapingFilter;
          if(pprimaryVertexFilter == 0 || beamScrapingFilter == 0)
            continue;
          int HLT_HIL3DoubleMuOpen_2018 = MTrigger.CheckTriggerStartWith("HLT_HIL3DoubleMu");
          if (HLT_HIL3DoubleMuOpen_2018 == 0)
            continue;
         } // end if pp data
      } else { // if PbPb
        if (IsData == true) {
          int pprimaryVertexFilter = MSkim.PVFilter;
          int phfCoincFilter2Th4 = MSkim.HFCoincidenceFilter2Th4;
          int pclusterCompatibilityFilter = MSkim.ClusterCompatibilityFilter;

          // Event selection criteria
          //    see https://twiki.cern.ch/twiki/bin/viewauth/CMS/HIPhotonJe5TeVpp2017PbPb2018
          if (pprimaryVertexFilter == 0 || phfCoincFilter2Th4 == 0 || pclusterCompatibilityFilter == 0)
            continue;

          // HLT trigger to select dimuon events, see Kaya's note: AN2019_143_v12, p.5
          //  FIXME: need to be replaced with the actual PbPb triggers
          int HLT_HIL3DoubleMuOpen_2018 = MTrigger.CheckTriggerStartWith("HLT_HIL3DoubleMu");
          if (HLT_HIL3DoubleMuOpen_2018 == 0)
            continue;
        }
      }

      for (int ijet = 0; ijet < MJet.JetCount; ijet++) {
        if (MJet.JetPT[ijet] < MinJetPT)
          continue;
        if (fabs(MJet.JetEta[ijet]) > 2)
          continue;
        bool passPurity = MJet.JetPFNHF[ijet] < 0.90 && MJet.JetPFNEF[ijet] < 0.90 && MJet.JetPFMUF[ijet] < 0.80 &&
                          MJet.JetPFCHF[ijet] > 0. && MJet.JetPFCHM[ijet] > 0. && MJet.JetPFCEF[ijet] < 0.80;
        if (!passPurity)
          continue;
        MMuMuJet.MJTHadronFlavor->push_back(MJet.MJTHadronFlavor[ijet]);
        MMuMuJet.MJTNcHad->push_back(MJet.MJTNcHad[ijet]);
        MMuMuJet.MJTNbHad->push_back(MJet.MJTNbHad[ijet]);
        MMuMuJet.JetPT->push_back(MJet.JetPT[ijet]);
        MMuMuJet.JetEta->push_back(MJet.JetEta[ijet]);
        MMuMuJet.JetPhi->push_back(MJet.JetPhi[ijet]);
        bool isJetTagged = false;
        float muPt1 = -999.;
        float muPt2 = -999.;
        float muEta1 = -999.;
        float muEta2 = -999.;
        float muPhi1 = -999.;
        float muPhi2 = -999.;
        float muDiDxy1 = -999.;
        float muDiDxy1Err = -999.;
        float muDiDxy2 = -999.;
        float muDiDxy2Err = -999.;
        float muDiDz1 = -999.;
        float muDiDz1Err = -999.;
        float muDiDz2 = -999.;
        float muDiDz2Err = -999.;
        float muDiDxy1Dxy2 = -999.;
        float muDiDxy1Dxy2Err = -999.;
        float mumuMass = -999.;
        float mumuEta = -999.;
        float mumuY = -999.;
        float mumuPhi = -999.;
        float mumuPt = -999.;
        float DRJetmu1 = -999.;
        float DRJetmu2 = -999.;
        float muDeta = -999.;
        float muDphi = -999.;
        float muDR = -999.;
        // variable to identify the highest pt dimuon pair
        float maxmumuPt = 0.;
        int maxMu1Index = -1;
        int maxMu2Index = -1;

        int nSingleMu = MSingleMu.SingleMuPT->size();
        for (int isinglemu1 = 0; isinglemu1 < nSingleMu; isinglemu1++){
          if (isMuonSelected(&MSingleMu, isinglemu1) == false)
	    continue;
            for (int isinglemu2 = isinglemu1 + 1; isinglemu2 < nSingleMu; isinglemu2++){
              if (isMuonSelected(&MSingleMu, isinglemu2) == false)
		continue;
              int charge1 = MSingleMu.SingleMuCharge->at(isinglemu1);
              int charge2 = MSingleMu.SingleMuCharge->at(isinglemu2);
              if (charge1 == charge2)
		continue;
              float jetEta = MJet.JetEta[ijet];
              float jetPhi = MJet.JetPhi[ijet];
              float muEta1 = MSingleMu.SingleMuEta->at(isinglemu1);
              float muPhi1 = MSingleMu.SingleMuPhi->at(isinglemu1);
              float muEta2 = MSingleMu.SingleMuEta->at(isinglemu2);
              float muPhi2 = MSingleMu.SingleMuPhi->at(isinglemu2);
              float dPhiMu1Jet_ = DeltaPhi(muPhi1, jetPhi);
              float dEtaMu1Jet_ = muEta1 - jetEta;
              float dPhiMu2Jet_ = DeltaPhi(muPhi2, jetPhi);
              float dEtaMu2Jet_ = muEta2 - jetEta;
              float dRMu1Jet = sqrt(dPhiMu1Jet_*dPhiMu1Jet_ + dEtaMu1Jet_*dEtaMu1Jet_);
              float dRMu2Jet = sqrt(dPhiMu2Jet_*dPhiMu2Jet_ + dEtaMu2Jet_*dEtaMu2Jet_);
              if (dRMu1Jet > 0.3) continue;
              if (dRMu2Jet > 0.3) continue;
              TLorentzVector Mu1, Mu2;
              Mu1.SetPtEtaPhiM(MSingleMu.SingleMuPT->at(isinglemu1), muEta1, muPhi1, M_MU);
              Mu2.SetPtEtaPhiM(MSingleMu.SingleMuPT->at(isinglemu2), muEta2, muPhi2, M_MU);
              TLorentzVector MuMu = Mu1 + Mu2;
              if (MuMu.M() > 130) continue;
              if (MuMu.Eta() > 2.4) continue;
              if (MuMu.Pt() > maxmumuPt) {
		maxmumuPt = MuMu.Pt();
		maxMu1Index = isinglemu1;
		maxMu2Index = isinglemu2;
	      } // end if dimuon pT larger than current max
            } // end loop over single muon 2
        } // end loop over single muon 1
        if (maxmumuPt > 0. && maxMu1Index >= 0 && maxMu2Index >= 0) {
          isJetTagged = true;
          muPt1 = MSingleMu.SingleMuPT->at(maxMu1Index);
          muPt2 = MSingleMu.SingleMuPT->at(maxMu2Index);
          muEta1 = MSingleMu.SingleMuEta->at(maxMu1Index);
          muEta2 = MSingleMu.SingleMuEta->at(maxMu2Index);
          muPhi1 = MSingleMu.SingleMuPhi->at(maxMu1Index);
          muPhi2 = MSingleMu.SingleMuPhi->at(maxMu2Index);
          muDiDxy1 = MSingleMu.SingleMuDxy->at(maxMu1Index);
          muDiDxy1Err = MSingleMu.SingleMuDxyError->at(maxMu1Index);
          muDiDxy2 = MSingleMu.SingleMuDxy->at(maxMu2Index);
          muDiDxy2Err = MSingleMu.SingleMuDxyError->at(maxMu2Index);
          muDiDz1 = MSingleMu.SingleMuDz->at(maxMu1Index);
          muDiDz1Err = MSingleMu.SingleMuDzError->at(maxMu1Index);
          muDiDz2 = MSingleMu.SingleMuDz->at(maxMu2Index);
          muDiDz2Err = MSingleMu.SingleMuDzError->at(maxMu2Index);
          muDiDxy1Dxy2 = muDiDxy1*muDiDxy2;
          muDiDxy1Dxy2Err = sqrt(muDiDxy1Err/muDiDxy1*muDiDxy1Err/muDiDxy1 + muDiDxy2Err/muDiDxy2*muDiDxy2Err/muDiDxy2)*muDiDxy1Dxy2;
          TLorentzVector Mu1, Mu2;
          Mu1.SetPtEtaPhiM(muPt1, muEta1, muPhi1, M_MU);
          Mu2.SetPtEtaPhiM(muPt2, muEta2, muPhi2, M_MU);
          TLorentzVector MuMu = Mu1 + Mu2;
          mumuMass = MuMu.M();
          mumuEta = MuMu.Eta();
          mumuY = MuMu.Rapidity();
          mumuPhi = MuMu.Phi();
          mumuPt = MuMu.Pt();
          float jetEta = MJet.JetEta[ijet];
          float jetPhi = MJet.JetPhi[ijet];
          float muEta1 = MSingleMu.SingleMuEta->at(maxMu1Index);
          float muPhi1 = MSingleMu.SingleMuPhi->at(maxMu1Index);
          float muEta2 = MSingleMu.SingleMuEta->at(maxMu2Index);
          float muPhi2 = MSingleMu.SingleMuPhi->at(maxMu2Index);
          float dPhiMu1Jet_ = DeltaPhi(muPhi1, jetPhi);
          float dEtaMu1Jet_ = muEta1 - jetEta;
          float dPhiMu2Jet_ = DeltaPhi(muPhi2, jetPhi);
          float dEtaMu2Jet_ = muEta2 - jetEta;
          float dRMu1Jet = sqrt(dPhiMu1Jet_*dPhiMu1Jet_ + dEtaMu1Jet_*dEtaMu1Jet_);
          float dRMu2Jet = sqrt(dPhiMu2Jet_*dPhiMu2Jet_ + dEtaMu2Jet_*dEtaMu2Jet_);
          muDeta = muEta1 - muEta2;
          muDphi = DeltaPhi(muPhi1, muPhi2);
          muDR = sqrt(muDeta * muDeta + muDphi * muDphi);
        } // end if dimuon pair found
        MMuMuJet.IsMuMuTagged->push_back(isJetTagged);
        MMuMuJet.muPt1->push_back(muPt1);
        MMuMuJet.muPt2->push_back(muPt2);
        MMuMuJet.muEta1->push_back(muEta1);
        MMuMuJet.muEta2->push_back(muEta2);
        MMuMuJet.muPhi1->push_back(muPhi1);
        MMuMuJet.muPhi2->push_back(muPhi2);
        MMuMuJet.muDiDxy1->push_back(muDiDxy1);
        MMuMuJet.muDiDxy1Err->push_back(muDiDxy1Err);
        MMuMuJet.muDiDxy2->push_back(muDiDxy2);
        MMuMuJet.muDiDxy2Err->push_back(muDiDxy2Err);
        MMuMuJet.muDiDz1->push_back(muDiDz1);
        MMuMuJet.muDiDz1Err->push_back(muDiDz1Err);
        MMuMuJet.muDiDz2->push_back(muDiDz2);
        MMuMuJet.muDiDz2Err->push_back(muDiDz2Err);
        MMuMuJet.muDiDxy1Dxy2->push_back(muDiDxy1Dxy2);
        MMuMuJet.muDiDxy1Dxy2Err->push_back(muDiDxy1Dxy2Err);
        MMuMuJet.mumuMass->push_back(mumuMass);
        MMuMuJet.mumuEta->push_back(mumuEta);
        MMuMuJet.mumuY->push_back(mumuY);
        MMuMuJet.mumuPhi->push_back(mumuPhi);
        MMuMuJet.mumuPt->push_back(mumuPt);
        MMuMuJet.DRJetmu1->push_back(DRJetmu1);
        MMuMuJet.DRJetmu2->push_back(DRJetmu2);
        MMuMuJet.muDeta->push_back(muDeta);
        MMuMuJet.muDphi->push_back(muDphi);
        MMuMuJet.muDR->push_back(muDR);
      } // end loop over jets
      MMuMuJet.FillEntry();
    } // end loop over events

    Bar.Update(EntryCount);
    Bar.Print();
    Bar.PrintLine();

    InputFile.Close();
  } // end loop over input files

  OutputFile.cd();
  Tree.Write();
  InfoTree.Write();

  OutputFile.Close();

  return 0;
}

double GetHFSum(PFTreeMessenger *M) {
  if (M == nullptr)
    return -1;
  if (M->Tree == nullptr)
    return -1;

  double Sum = 0;
  for (int iPF = 0; iPF < M->ID->size(); iPF++) {
    if (fabs(M->Eta->at(iPF)) < 3)
      continue;
    if (fabs(M->Eta->at(iPF)) > 5)
      continue;
    Sum = Sum + M->E->at(iPF);
  }

  // cout << Sum << endl;

  return Sum;
}

double GetGenHFSum(GenParticleTreeMessenger *M, int SubEvent) {
  if (M == nullptr)
    return -1;
  if (M->Tree == nullptr)
    return -1;

  double Sum = 0;
  for (int iGen = 0; iGen < M->Mult; iGen++) {
    if (fabs(M->Eta->at(iGen)) < 3)
      continue;
    if (fabs(M->Eta->at(iGen)) > 5)
      continue;
    if (M->DaughterCount->at(iGen) > 0)
      continue;
    if (M->PT->at(iGen) < 0.4) // for now...
      continue;

    if (SubEvent >= 0) // if SubEvent >= 0, check subevent
    {
      if (M->SubEvent->at(iGen) != SubEvent)
        continue;
    }

    Sum = Sum + M->PT->at(iGen) * cosh(M->Eta->at(iGen));
  }

  return Sum;
}

bool isMuonSelected(SingleMuTreeMessenger *M, int i) {
  if (M == nullptr)
    return false;
  if (M->Tree == nullptr)
    return false;
  if (M->SingleMuPT->at(i) < 3.)
    return false;
  if (fabs(M->SingleMuEta->at(i)) > 2.3)
    return false;
  if (M->SingleMuIsTracker->at(i)  == 0 ||
      M->SingleMuIsGlobal->at(i)   == 0 || 
      M->SingleMuHybridSoft->at(i) == 0 ||
      M->SingleMuIsGood->at(i)     == 0)
    return false;

  return true;
}
