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
double deltaR(double eta1, double phi1, double eta2, double phi2);

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
        int maxMuMuIndex = -1;

        for (int ipair = 0; ipair < MMu.NDi; ipair++) {
          if (MMu.DiCharge1[ipair] == MMu.DiCharge2[ipair])
            continue;
          if (fabs(MMu.DiEta1[ipair]) > 2.3)
            continue;
          if (fabs(MMu.DiEta2[ipair]) > 2.3)
            continue;
          if (fabs(MMu.DiPT1[ipair]) < 3)
            continue;
          if (fabs(MMu.DiPT2[ipair]) < 3)
            continue;
          if (MMu.DiMass[ipair] > 130)
            continue;
          if (deltaR(MJet.JetEta[ijet], MJet.JetPhi[ijet], MMu.DiEta1[ipair], MMu.DiPhi1[ipair]) > 0.3)
            continue;
          if (deltaR(MJet.JetEta[ijet], MJet.JetPhi[ijet], MMu.DiEta2[ipair], MMu.DiPhi2[ipair]) > 0.3)
            continue;
          if (fabs(MMu.DiDxy1[ipair]) < 0.01)
            continue;
          if (fabs(MMu.DiDxy2[ipair]) < 0.01)
            continue;
          // build dimuon TLorentzVector
          TLorentzVector Mu1, Mu2;
          Mu1.SetPtEtaPhiM(MMu.DiPT1[ipair], MMu.DiEta1[ipair], MMu.DiPhi1[ipair], M_MU);
          Mu2.SetPtEtaPhiM(MMu.DiPT2[ipair], MMu.DiEta2[ipair], MMu.DiPhi2[ipair], M_MU);
          TLorentzVector MuMu = Mu1 + Mu2;
          if (MuMu.Pt() > maxmumuPt) {
            maxmumuPt = MuMu.Pt();
            maxMuMuIndex = ipair;
          } // end if dimuon pT larger than current max
        }   // end loop over dimuon pairs
        if (maxmumuPt > 0. && maxMuMuIndex >= 0) {
          isJetTagged = true;
          muPt1 = MMu.DiPT1[maxMuMuIndex];
          muPt2 = MMu.DiPT2[maxMuMuIndex];
          muEta1 = MMu.DiEta1[maxMuMuIndex];
          muEta2 = MMu.DiEta2[maxMuMuIndex];
          muPhi1 = MMu.DiPhi1[maxMuMuIndex];
          muPhi2 = MMu.DiPhi2[maxMuMuIndex];
          mumuMass = MMu.DiMass[maxMuMuIndex];
          mumuEta = MMu.DiEta[maxMuMuIndex];
          mumuY = MMu.DiRapidity[maxMuMuIndex];
          mumuPhi = MMu.DiPhi[maxMuMuIndex];
          mumuPt = MMu.DiPT[maxMuMuIndex];
          DRJetmu1 = deltaR(MJet.JetEta[ijet], MJet.JetPhi[ijet], MMu.DiEta1[maxMuMuIndex], MMu.DiPhi1[maxMuMuIndex]);
          DRJetmu2 = deltaR(MJet.JetEta[ijet], MJet.JetPhi[ijet], MMu.DiEta2[maxMuMuIndex], MMu.DiPhi2[maxMuMuIndex]);
          muDeta = MMu.DiEta1[maxMuMuIndex] - MMu.DiEta2[maxMuMuIndex];
          muDphi = PhiRangePositive(DeltaPhi(MMu.DiPhi1[maxMuMuIndex], MMu.DiPhi2[maxMuMuIndex]));
          muDR = sqrt(muDeta * muDeta + muDphi * muDphi);
        } // end if dimuon pair found
        MMuMuJet.IsMuMuTagged->push_back(isJetTagged);
        MMuMuJet.muPt1->push_back(muPt1);
        MMuMuJet.muPt2->push_back(muPt2);
        MMuMuJet.muEta1->push_back(muEta1);
        MMuMuJet.muEta2->push_back(muEta2);
        MMuMuJet.muPhi1->push_back(muPhi1);
        MMuMuJet.muPhi2->push_back(muPhi2);
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

double deltaR(double eta1, double phi1, double eta2, double phi2) {
  double dEta = eta1 - eta2;
  double dPhi = std::fabs(phi1 - phi2);
  // Correct for the periodicity of the azimuthal angle
  if (dPhi > M_PI) {
    dPhi = 2 * M_PI - dPhi;
  }
  return std::sqrt(dEta * dEta + dPhi * dPhi);
}
