#include <iostream>
using namespace std;

#include "TFile.h"
#include "TLorentzVector.h"
#include "TTree.h"

#include "CommandLine.h"
#include "CommonFunctions.h"
#include "Messenger.h"
#include "ProgressBar.h"
#include "DimuonMessenger.h"

#include "tnp_weight_lowptPbPb.h"


int main(int argc, char *argv[]);
double GetHFSum(PFTreeMessenger *M);
double GetGenHFSum(GenParticleTreeMessenger *M, int SubEvent = -1);
bool isMuonSelected(SingleMuTreeMessenger *M, int i);
bool isGenMuonSelected(SingleMuTreeMessenger *M, int i);
bool isDimuonGenMatched(SingleMuTreeMessenger *M, int gen1, int gen2, int mu1, int mu2);
//int isOnia(float mass);
std::vector<int> mu_trackmatch(JetTreeMessenger *MJet, int jetno, float pt, float eta, float phi);

int main(int argc, char *argv[]) {
  string VersionString = "V8";

  CommandLine CL(argc, argv);

  vector<string> InputFileNames = CL.GetStringVector("Input");
  string OutputFileName = CL.Get("Output");

  bool IsData = CL.GetBool("IsData", false);
  bool IsPP = CL.GetBool("IsPP", false);
  bool svtx = CL.GetBool("svtx", false);
  int Year = CL.GetInt("Year", 2018);
  double Fraction = CL.GetDouble("Fraction", 1.00);
  double MinJetPT = CL.GetDouble("MinJetPT", 30);
  string PFJetCollection = CL.Get("PFJetCollection", "akCs3PFJetAnalyzer/t");
  string PFTreeName = IsPP ? "pfcandAnalyzer/pfTree" : "particleFlowAnalyser/pftree";
  PFTreeName = CL.Get("PFTree", PFTreeName);

  TFile OutputFile(OutputFileName.c_str(), "RECREATE");
  TTree Tree("Tree", Form("Tree for MuMu tagged jet analysis (%s)", VersionString.c_str()));
  TTree InfoTree("InfoTree", "Information");
  TTree GenTree("GenTree", Form("Gen Tree for MuMu tagged jet analysis (%s)", VersionString.c_str()));

  DimuonJetMessenger MMuMuJet;
  GenDimuonJetMessenger MGenMuMuJet;
  MMuMuJet.SetBranch(&Tree);
  MGenMuMuJet.SetBranch(&GenTree);

  std::vector<int> mt1;
  std::vector<int> mt2;

  for (string InputFileName : InputFileNames) {

    std::cout << "Processing file: " << InputFileName << std::endl;
    TFile InputFile(InputFileName.c_str());

    HiEventTreeMessenger MEvent(InputFile);
    PPTrackTreeMessenger MTrackPP(InputFile);
    PbPbTrackTreeMessenger MTrack(InputFile);
    GenParticleTreeMessenger MGen(InputFile);
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
      MMu.GetEntry(iE);
      MSingleMu.GetEntry(iE);
      MSkim.GetEntry(iE);
      MTrigger.GetEntry(iE);
      MJet.GetEntry(iE);
      MMuMuJet.Clear();
      MGenMuMuJet.Clear();
      ////////////////////////////////////////
      ////////// Global event stuff //////////
      ////////////////////////////////////////

      MMuMuJet.Run = MEvent.Run;
      MGenMuMuJet.Run = MEvent.Run;
      MMuMuJet.Lumi = MEvent.Lumi;
      MGenMuMuJet.Lumi = MEvent.Lumi;
      MMuMuJet.Event = MEvent.Event;
      MGenMuMuJet.Event = MEvent.Event;
      MMuMuJet.hiBin = MEvent.hiBin;
      MGenMuMuJet.hiBin = MEvent.hiBin;
      MMuMuJet.hiHF = MEvent.hiHF;
      MGenMuMuJet.hiHF = MEvent.hiHF;
      MMuMuJet.NPU = 0;
      MGenMuMuJet.NPU = 0;
      if (MEvent.npus->size() == 9){
        MMuMuJet.NPU = MEvent.npus->at(5);
        MGenMuMuJet.NPU = MEvent.npus->at(5);
      } else if (MEvent.npus->size() > 1) {
        MMuMuJet.NPU = MEvent.npus->at(0);
        MGenMuMuJet.NPU = MEvent.npus->at(0);
      } else {
        MMuMuJet.NPU = 0;
        MGenMuMuJet.NPU = 0;
      }

      MMuMuJet.nsvtx = MJet.nsvtx;
      MMuMuJet.ntrk = MJet.ntrk;

      MMuMuJet.PTHat = MEvent.pthat;
      MGenMuMuJet.PTHat = MEvent.pthat;
      MMuMuJet.EventWeight = MEvent.weight;
      MGenMuMuJet.EventWeight = MEvent.weight;
      MMuMuJet.NCollWeight = FindNColl(MMuMuJet.hiBin);
      MGenMuMuJet.NCollWeight = FindNColl(MGenMuMuJet.hiBin);
      ////////////////////////////
      ////////// Vertex //////////
      ////////////////////////////
      MMuMuJet.NVertex = 0;
      MGenMuMuJet.NVertex = 0;
      int BestVertex = -1;
      for (int i = 0; i < (IsPP ? MTrackPP.nVtx : MTrack.VX->size()); i++) {
        if (IsPP == true && (BestVertex < 0 || MTrackPP.ptSumVtx->at(i) > MTrackPP.ptSumVtx->at(BestVertex)))
          BestVertex = i;
        if (IsPP == false && (BestVertex < 0 || MTrack.VPTSum->at(i) > MTrack.VPTSum->at(BestVertex)))
          BestVertex = i;

        MMuMuJet.NVertex = MMuMuJet.NVertex + 1;
        MGenMuMuJet.NVertex = MGenMuMuJet.NVertex + 1;
      }

      if (BestVertex >= 0) {
        MMuMuJet.VX = IsPP ? MTrackPP.xVtx->at(BestVertex) : MTrack.VX->at(BestVertex);
        MMuMuJet.VY = IsPP ? MTrackPP.yVtx->at(BestVertex) : MTrack.VY->at(BestVertex);
        MMuMuJet.VZ = IsPP ? MTrackPP.zVtx->at(BestVertex) : MTrack.VZ->at(BestVertex);
        MMuMuJet.VXError = IsPP ? MTrackPP.xErrVtx->at(BestVertex) : MTrack.VXError->at(BestVertex);
        MMuMuJet.VYError = IsPP ? MTrackPP.yErrVtx->at(BestVertex) : MTrack.VYError->at(BestVertex);
        MMuMuJet.VZError = IsPP ? MTrackPP.zErrVtx->at(BestVertex) : MTrack.VZError->at(BestVertex);

        MGenMuMuJet.VX = MMuMuJet.VX;
        MGenMuMuJet.VY = MMuMuJet.VY;
        MGenMuMuJet.VZ = MMuMuJet.VZ;
        MGenMuMuJet.VXError = MMuMuJet.VXError;
        MGenMuMuJet.VYError = MMuMuJet.VYError;
        MGenMuMuJet.VZError = MMuMuJet.VZError;
      }
      /////////////////////////////////////
      ////////// Event selection //////////
      /////////////////////////////////////

      if (IsPP == true) {
        if (IsData == true) {
          int pprimaryVertexFilter = MSkim.PVFilter;
          int beamScrapingFilter = MSkim.BeamScrapingFilter; 
          if (pprimaryVertexFilter == 0 || beamScrapingFilter == 0)
            continue;
          int minbiastrigger = MTrigger.CheckTriggerStartWith("HLT_HIAK4PFJet30_v*");
          if (minbiastrigger == 0)
            continue;
        } // end if pp data
        else {
          // pp MC event selection if any
        }
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
          int minbiastrigger = MTrigger.CheckTriggerStartWith("HLT_HIMinimumBias_v*");
          if (minbiastrigger == 0)
            continue;
        }
        else {
          // PbPb MC event selection if any
        }
      }

      // JET SELECTION

      for (int ijet = 0; ijet < MJet.JetCount; ijet++) {
        if (MJet.JetPT[ijet] < MinJetPT)
          continue;
        if (fabs(MJet.JetEta[ijet]) > 2)
          continue;
        bool passPurity = MJet.JetPFNHF[ijet] < 0.90 && MJet.JetPFNEF[ijet] < 0.90 && MJet.JetPFMUF[ijet] < 0.80 &&
                          MJet.JetPFCHF[ijet] > 0. && MJet.JetPFCHM[ijet] > 0. && MJet.JetPFCEF[ijet] < 0.80;
        if (!passPurity)
          continue;

        MMuMuJet.JetPT = MJet.JetPT[ijet];
        MMuMuJet.JetEta = MJet.JetEta[ijet];
        MMuMuJet.JetPhi = MJet.JetPhi[ijet];

        // ADD FLAVOR INFO -- note that only a few of these branches are implemented for PbPb
        MMuMuJet.HadronFlavor = IsPP ? MJet.HadronFlavor[ijet] : MJet.MJTHadronFlavor[ijet];
        MMuMuJet.PartonFlavor = MJet.PartonFlavor[ijet];
        MMuMuJet.NcHad = IsPP ? MJet.NcHad[ijet] : MJet.MJTNcHad[ijet];
        MMuMuJet.NbHad = IsPP ? MJet.NbHad[ijet] : MJet.MJTNbHad[ijet];
        MMuMuJet.NbPar = MJet.NbPar[ijet];
        MMuMuJet.NcPar = MJet.NcPar[ijet];
        MMuMuJet.HasGSPB = MJet.HasGSPB[ijet];
        MMuMuJet.HasGSPC = MJet.HasGSPC[ijet];

        // ADD TAGGING INFO
        MMuMuJet.PN_bb = MJet.PN_bb[ijet];
        MMuMuJet.PN_b = MJet.PN_b[ijet];
        MMuMuJet.PN_cc = MJet.PN_cc[ijet];
        MMuMuJet.PN_c = MJet.PN_c[ijet];
        MMuMuJet.PN_undef = MJet.PN_undef[ijet];
        MMuMuJet.PN_uds = MJet.PN_uds[ijet];
        MMuMuJet.PN_g = MJet.PN_g[ijet];
        MMuMuJet.PN_pu = MJet.PN_pu[ijet];

        if (svtx) {

          // ADD SVTX INFORMATION

          MMuMuJet.jtNsvtx = MJet.jtNsvtx[ijet];
          MMuMuJet.jtNtrk = MJet.jtNtrk[ijet];
          MMuMuJet.jtptCh = MJet.jtptCh[ijet];

          for (int isvtx = 0; isvtx < MJet.nsvtx; isvtx++) {

            if (MJet.svtxJetId[isvtx] == ijet) {

              MMuMuJet.svtxJetId->push_back(MJet.svtxJetId[isvtx]);
              MMuMuJet.svtxNtrk->push_back(MJet.svtxNtrk[isvtx]);
              MMuMuJet.svtxdl->push_back(MJet.svtxdl[isvtx]);
              MMuMuJet.svtxdls->push_back(MJet.svtxdls[isvtx]);
              MMuMuJet.svtxdl2d->push_back(MJet.svtxdl2d[isvtx]);
              MMuMuJet.svtxdls2d->push_back(MJet.svtxdls2d[isvtx]);
              MMuMuJet.svtxm->push_back(MJet.svtxm[isvtx]);
              MMuMuJet.svtxmcorr->push_back(MJet.svtxmcorr[isvtx]);
              MMuMuJet.svtxpt->push_back(MJet.svtxpt[isvtx]);
              MMuMuJet.svtxnormchi2->push_back(MJet.svtxnormchi2[isvtx]);
              MMuMuJet.svtxchi2->push_back(MJet.svtxchi2[isvtx]);


            }
          }

          for (int itrk = 0; itrk < MJet.ntrk; itrk++) {
            if (MJet.trkJetId[itrk] == ijet) {

              // ADD TRACK INFORMATION

              MMuMuJet.trkJetId->push_back(MJet.trkJetId[itrk]);
              MMuMuJet.trkSvtxId->push_back(MJet.trkSvtxId[itrk]);
              MMuMuJet.trkPt->push_back(MJet.trkPt[itrk]);
              MMuMuJet.trkEta->push_back(MJet.trkEta[itrk]);
              MMuMuJet.trkPhi->push_back(MJet.trkPhi[itrk]);
              MMuMuJet.trkIp3d->push_back(MJet.trkIp3d[itrk]);
              MMuMuJet.trkIp3dSig->push_back(MJet.trkIp3dSig[itrk]);
              MMuMuJet.trkIp2d->push_back(MJet.trkIp2d[itrk]);
              MMuMuJet.trkIp2dSig->push_back(MJet.trkIp2dSig[itrk]);
              MMuMuJet.trkDistToAxis->push_back(MJet.trkDistToAxis[itrk]);
              MMuMuJet.trkDistToAxisSig->push_back(MJet.trkDistToAxisSig[itrk]);
              MMuMuJet.trkIpProb3d->push_back(MJet.trkIpProb3d[itrk]);
              MMuMuJet.trkIpProb2d->push_back(MJet.trkIpProb2d[itrk]);
              MMuMuJet.trkDz->push_back(MJet.trkDz[itrk]);
              MMuMuJet.trkPdgId->push_back(MJet.trkPdgId[itrk]);
              MMuMuJet.trkMatchSta->push_back(MJet.trkMatchSta[itrk]);

              
            }
          }

        }

        bool isJetMuonTagged = false;
        float muPt1 = -999.;
        float muPt2 = -999.;
        float muEta1 = -999.;
        float muEta2 = -999.;
        float muPhi1 = -999.;
        float muPhi2 = -999.;
        int muCharge1 = -999;
        int muCharge2 = -999;
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
        //int mumuisOnia = -999;
        float dRJetMu1 = -999.;
        float dRJetMu2 = -999.;
        float muDeta = -999.;
        float muDphi = -999.;
        float muDR = -999.;
        // variable to identify the highest pt dimuon pair
        float maxmumuPt = -999;
        int maxMu1Index = -1;
        int maxMu2Index = -1;

        int nSingleMu = MSingleMu.SingleMuPT->size();
        for (int isinglemu1 = 0; isinglemu1 < nSingleMu; isinglemu1++) {
          if (isMuonSelected(&MSingleMu, isinglemu1) == false) continue;
          for (int isinglemu2 = isinglemu1 + 1; isinglemu2 < nSingleMu; isinglemu2++) {
            if (isMuonSelected(&MSingleMu, isinglemu2) == false) continue;
            // if (charge1 == charge2)
            // continue;
            float jetEta = MJet.JetEta[ijet];
            float jetPhi = MJet.JetPhi[ijet];
            float muEta1 = MSingleMu.SingleMuEta->at(isinglemu1);
            float muPhi1 = MSingleMu.SingleMuPhi->at(isinglemu1);
            float muEta2 = MSingleMu.SingleMuEta->at(isinglemu2);
            float muPhi2 = MSingleMu.SingleMuPhi->at(isinglemu2);
            int muCharge1 = MSingleMu.SingleMuCharge->at(isinglemu1);
            int muCharge2 = MSingleMu.SingleMuCharge->at(isinglemu2);
            float dPhiMu1Jet_ = DeltaPhi(muPhi1, jetPhi);
            float dEtaMu1Jet_ = muEta1 - jetEta;
            float dPhiMu2Jet_ = DeltaPhi(muPhi2, jetPhi);
            float dEtaMu2Jet_ = muEta2 - jetEta;
            float dRmu1Jet = sqrt(dPhiMu1Jet_ * dPhiMu1Jet_ + dEtaMu1Jet_ * dEtaMu1Jet_);
            float dRmu2Jet = sqrt(dPhiMu2Jet_ * dPhiMu2Jet_ + dEtaMu2Jet_ * dEtaMu2Jet_);
            if (dRmu1Jet > 0.3)
              continue;
            if (dRmu2Jet > 0.3)
              continue;
            TLorentzVector Mu1, Mu2;
            Mu1.SetPtEtaPhiM(MSingleMu.SingleMuPT->at(isinglemu1), muEta1, muPhi1, M_MU);
            Mu2.SetPtEtaPhiM(MSingleMu.SingleMuPT->at(isinglemu2), muEta2, muPhi2, M_MU);
            TLorentzVector MuMu = Mu1 + Mu2;
            //if (fabs(MuMu.Eta()) > 2.4)
            // continue;
            if (MuMu.Pt() > maxmumuPt) {
              maxmumuPt = MuMu.Pt();
              maxMu1Index = isinglemu1;
              maxMu2Index = isinglemu2;
            } // end if dimuon pT larger than current max
          } // end loop over single muon 2
        } // end loop over single muon 1

        if (maxmumuPt > 0. && maxMu1Index >= 0 && maxMu2Index >= 0) {
          //cout << " reco pair at entry " << iE << endl;
          isJetMuonTagged = true;
  
          muPt1 = MSingleMu.SingleMuPT->at(maxMu1Index);
          muPt2 = MSingleMu.SingleMuPT->at(maxMu2Index);
          muEta1 = MSingleMu.SingleMuEta->at(maxMu1Index);
          muEta2 = MSingleMu.SingleMuEta->at(maxMu2Index);
          muPhi1 = MSingleMu.SingleMuPhi->at(maxMu1Index);
          muPhi2 = MSingleMu.SingleMuPhi->at(maxMu2Index);
          muCharge1 = MSingleMu.SingleMuCharge->at(maxMu1Index);
          muCharge2 = MSingleMu.SingleMuCharge->at(maxMu2Index);
          muDiDxy1 = MSingleMu.SingleMuDxy->at(maxMu1Index);
          muDiDxy1Err = MSingleMu.SingleMuDxyError->at(maxMu1Index);
          muDiDxy2 = MSingleMu.SingleMuDxy->at(maxMu2Index);
          muDiDxy2Err = MSingleMu.SingleMuDxyError->at(maxMu2Index);
          muDiDz1 = MSingleMu.SingleMuDz->at(maxMu1Index);
          muDiDz1Err = MSingleMu.SingleMuDzError->at(maxMu1Index);
          muDiDz2 = MSingleMu.SingleMuDz->at(maxMu2Index);
          muDiDz2Err = MSingleMu.SingleMuDzError->at(maxMu2Index);
          muDiDxy1Dxy2 = muDiDxy1 * muDiDxy2;
          muDiDxy1Dxy2Err =
              sqrt(muDiDxy1Err / muDiDxy1 * muDiDxy1Err / muDiDxy1 + muDiDxy2Err / muDiDxy2 * muDiDxy2Err / muDiDxy2) *
              muDiDxy1Dxy2;
          TLorentzVector Mu1, Mu2;
          Mu1.SetPtEtaPhiM(muPt1, muEta1, muPhi1, M_MU);
          Mu2.SetPtEtaPhiM(muPt2, muEta2, muPhi2, M_MU);
          TLorentzVector MuMu = Mu1 + Mu2;
          mumuMass = MuMu.M();
          mumuEta = MuMu.Eta();
          mumuY = MuMu.Rapidity();
          mumuPhi = MuMu.Phi();
          mumuPt = MuMu.Pt();
          //mumuisOnia = isOnia(mumuMass);
          float jetEta = MJet.JetEta[ijet];
          float jetPhi = MJet.JetPhi[ijet];
          float dPhiMu1Jet_ = DeltaPhi(muPhi1, jetPhi);
          float dEtaMu1Jet_ = muEta1 - jetEta;
          float dPhiMu2Jet_ = DeltaPhi(muPhi2, jetPhi);
          float dEtaMu2Jet_ = muEta2 - jetEta;
          dRJetMu1 = sqrt(dPhiMu1Jet_ * dPhiMu1Jet_ + dEtaMu1Jet_ * dEtaMu1Jet_);
          dRJetMu2 = sqrt(dPhiMu2Jet_ * dPhiMu2Jet_ + dEtaMu2Jet_ * dEtaMu2Jet_);
          muDeta = muEta1 - muEta2;
          muDphi = DeltaPhi(muPhi1, muPhi2);
          muDR = sqrt(muDeta * muDeta + muDphi * muDphi);
        } // end if dimuon pair found

        MMuMuJet.IsMuMuTagged = isJetMuonTagged;
        MMuMuJet.muPt1= muPt1;
        MMuMuJet.muPt2= muPt2;
        MMuMuJet.muEta1= muEta1;
        MMuMuJet.muEta2= muEta2;
        MMuMuJet.muPhi1= muPhi1;
        MMuMuJet.muPhi2= muPhi2;
        MMuMuJet.muCharge1= muCharge1;
        MMuMuJet.muCharge2= muCharge2;
        MMuMuJet.muDiDxy1= muDiDxy1;
        MMuMuJet.muDiDxy1Err= muDiDxy1Err;
        MMuMuJet.muDiDxy2= muDiDxy2;
        MMuMuJet.muDiDxy2Err= muDiDxy2Err;
        MMuMuJet.muDiDz1= muDiDz1;
        MMuMuJet.muDiDz1Err= muDiDz1Err;
        MMuMuJet.muDiDz2= muDiDz2;
        MMuMuJet.muDiDz2Err= muDiDz2Err;
        MMuMuJet.muDiDxy1Dxy2= muDiDxy1Dxy2;
        MMuMuJet.muDiDxy1Dxy2Err= muDiDxy1Dxy2Err;
        MMuMuJet.mumuMass= mumuMass;
        MMuMuJet.mumuEta= mumuEta;
        MMuMuJet.mumuY= mumuY;
        MMuMuJet.mumuPhi= mumuPhi;
        MMuMuJet.mumuPt= mumuPt;
        //MMuMuJet.mumuisOnia= mumuisOnia;
        MMuMuJet.DRJetmu1= dRJetMu1;
        MMuMuJet.DRJetmu2= dRJetMu2;
        MMuMuJet.muDeta= muDeta;
        MMuMuJet.muDphi= muDphi;
        MMuMuJet.muDR= muDR;

        mt1 = mu_trackmatch(&MJet, ijet, muPt1, muEta1, muPhi1);
        mt2 = mu_trackmatch(&MJet, ijet, muPt2, muEta2, muPhi2);

        MMuMuJet.trkIdx_mu1 = mt1[0];
        MMuMuJet.trkIdx_mu2 = mt2[0];
        MMuMuJet.svtxIdx_mu1 = mt1[1];
        MMuMuJet.svtxIdx_mu2 = mt2[1];

        mt1.clear();
        mt2.clear();

        // Gen muon info  

	      bool GenIsJetMuonTagged = false;
        float GenMuPt1 = -999;
        float GenMuPt2 = -999;
	      float GenMuEta1 = -999; 
        float GenMuEta2 = -999; 
        float GenMuPhi1 = -999;
        float GenMuPhi2 = -999; 

        float GenMuMuMass = -999;
        float GenMuMuEta = -999;
        float GenMuMuY = -999;
        float GenMuMuPhi = -999;
        float GenMuMuPt = -999;

        float GenMuDeta = -999 ;
        float GenMuDphi = -999 ;
        float GenMuDR = -999;

        float maxGenmumuPt = -999;
        int maxGenMu1Index = -1;
        int maxGenMu2Index = -1;

        int nGenSingleMu = MSingleMu.GenSingleMuPT->size(); 
        
        for(int igen1 = 0; igen1 < nGenSingleMu; igen1++){ 
          if(isGenMuonSelected(&MSingleMu, igen1) == false) continue;
            for(int igen2 = igen1 + 1; igen2 < nGenSingleMu; igen2++){
              if(isGenMuonSelected(&MSingleMu, igen2) == false) continue;

              //TLorentzVector VGenMu1, VGenMu2;
              //VGenMu1.SetPtEtaPhiM(MSingleMu.GenSingleMuPT->at(igen1),MSingleMu.GenSingleMuEta->at(igen1),MSingleMu.GenSingleMuPhi->at(igen1),M_MU);
              //VGenMu2.SetPtEtaPhiM(MSingleMu.GenSingleMuPT->at(igen2),MSingleMu.GenSingleMuEta->at(igen2),MSingleMu.GenSingleMuPhi->at(igen2),M_MU);

              float jetEta = MJet.JetEta[ijet];
              float jetPhi = MJet.JetPhi[ijet];
              float dPhiMu1Jet_ = DeltaPhi(MSingleMu.GenSingleMuPhi->at(igen1), jetPhi);
              float dEtaMu1Jet_ = MSingleMu.GenSingleMuEta->at(igen1) - jetEta;
              float dPhiMu2Jet_ = DeltaPhi(MSingleMu.GenSingleMuPhi->at(igen2), jetPhi);
              float dEtaMu2Jet_ = MSingleMu.GenSingleMuEta->at(igen2) - jetEta;
              float dRMu1Jet = sqrt(dPhiMu1Jet_*dPhiMu1Jet_ + dEtaMu1Jet_*dEtaMu1Jet_);
              float dRMu2Jet = sqrt(dPhiMu2Jet_*dPhiMu2Jet_ + dEtaMu2Jet_*dEtaMu2Jet_);
              if (dRMu1Jet > 0.3) continue;
              if (dRMu2Jet > 0.3) continue;

              TLorentzVector GenMu1, GenMu2;
              GenMu1.SetPtEtaPhiM(MSingleMu.GenSingleMuPT->at(igen1), MSingleMu.GenSingleMuEta->at(igen1), MSingleMu.GenSingleMuPhi->at(igen1), M_MU);
              GenMu2.SetPtEtaPhiM(MSingleMu.GenSingleMuPT->at(igen2), MSingleMu.GenSingleMuEta->at(igen2), MSingleMu.GenSingleMuPhi->at(igen2), M_MU);

              TLorentzVector GenMuMu = GenMu1 + GenMu2;
               //cout << dRMu1Jet << " " << dRMu2Jet << " " << GenMuMu.Pt() << endl;

              if (GenMuMu.Pt() > maxGenmumuPt) {
                maxGenmumuPt = GenMuMu.Pt();
                maxGenMu1Index = igen1;
                maxGenMu2Index = igen2;
              } // end if dimuon pT larger than current max
            
            }
          }

        if(maxGenmumuPt > 0. && maxGenMu1Index >= 0 && maxGenMu2Index >= 0){
          //cout << "gen pair at entry " << iE << endl;
          GenIsJetMuonTagged = true;
          
          GenMuPt1 = MSingleMu.GenSingleMuPT->at(maxGenMu1Index);
          GenMuPt2 = MSingleMu.GenSingleMuPT->at(maxGenMu2Index);
          GenMuEta1 = MSingleMu.GenSingleMuEta->at(maxGenMu1Index); 
          GenMuEta2 = MSingleMu.GenSingleMuEta->at(maxGenMu2Index); 
          GenMuPhi1 = MSingleMu.GenSingleMuPhi->at(maxGenMu1Index);
          GenMuPhi2 = MSingleMu.GenSingleMuPhi->at(maxGenMu2Index); 

          TLorentzVector Mu1, Mu2;
          Mu1.SetPtEtaPhiM(GenMuPt1, GenMuEta1, GenMuPhi1, M_MU);
          Mu2.SetPtEtaPhiM(GenMuPt2, GenMuEta2, GenMuPhi2, M_MU);

          TLorentzVector MuMu = Mu1 + Mu2;
          GenMuMuMass = MuMu.M();
          GenMuMuEta = MuMu.Eta();
          GenMuMuY = MuMu.Rapidity();
          GenMuMuPhi = MuMu.Phi();
          GenMuMuPt = MuMu.Pt();

          GenMuDeta = GenMuEta1 - GenMuEta2;
          GenMuDphi = DeltaPhi(GenMuPhi1, GenMuPhi2);
          GenMuDR = sqrt(GenMuDeta * GenMuDeta + GenMuDphi * GenMuDphi);
          
        } // end if dimuon pair found ll 

        if(GenIsJetMuonTagged && isJetMuonTagged){
          MMuMuJet.mumuIsGenMatched = isDimuonGenMatched(&MSingleMu, maxGenMu1Index, maxGenMu2Index, maxMu1Index, maxMu2Index);
        }
        else{
          MMuMuJet.mumuIsGenMatched = false;
        }
        

        MMuMuJet.GenIsMuMuTagged = GenIsJetMuonTagged;
        MMuMuJet.GenMuMuMass = GenMuMuMass;
        MMuMuJet.GenMuMuPt = GenMuMuPt;
        MMuMuJet.GenMuMuPhi = GenMuMuPhi;
        MMuMuJet.GenMuMuEta = GenMuMuEta;
        MMuMuJet.GenMuMuY = GenMuMuY;

        MMuMuJet.GenMuPt1 = GenMuPt1;
        MMuMuJet.GenMuPt2 = GenMuPt2;
        MMuMuJet.GenMuEta1 = GenMuEta1;
        MMuMuJet.GenMuEta2 = GenMuEta2;
        MMuMuJet.GenMuPhi1 = GenMuPhi1;
        MMuMuJet.GenMuPhi2 = GenMuPhi2;

        MMuMuJet.GenMuDeta = GenMuDeta;
        MMuMuJet.GenMuDphi = GenMuDphi;
        //MMuMuJet.GenMuDR->push_back(sqrt(GenMuDeta * GenMuDeta + GenMuDphi * GenMuDphi));
        MMuMuJet.GenMuDR = GenMuDR;

        ////// EXTRA MUON WEIGHT //////)
        MMuMuJet.MuMuWeight = 1.0;
        MMuMuJet.ExtraMuWeight->assign(12, 1.0); // 12 sources of systematics
        if(isJetMuonTagged){
          MMuMuJet.MuMuWeight = tnp_weight_trk_pbpb(muEta1, 0) * tnp_weight_trk_pbpb(muEta2, 0) *
                                tnp_weight_muid_pbpb(muPt1, muEta1, 0) * tnp_weight_muid_pbpb(muPt2, muEta2, 0);
          MMuMuJet.ExtraMuWeight->at(0) = tnp_weight_trk_pbpb(muEta1, -1) / tnp_weight_trk_pbpb(muEta1, 0) *
                                      tnp_weight_trk_pbpb(muEta2, -1) / tnp_weight_trk_pbpb(muEta2, 0);
          MMuMuJet.ExtraMuWeight->at(1) = tnp_weight_trk_pbpb(muEta1, -2) / tnp_weight_trk_pbpb(muEta1, 0) *
                                      tnp_weight_trk_pbpb(muEta2, -2) / tnp_weight_trk_pbpb(muEta2, 0);      
          MMuMuJet.ExtraMuWeight->at(2) = tnp_weight_muid_pbpb(muPt1, muEta1, -1) / tnp_weight_muid_pbpb(muPt1, muEta1, 0) *
                                      tnp_weight_muid_pbpb(muPt2, muEta2, -1) / tnp_weight_muid_pbpb(muPt2, muEta2, 0);
          MMuMuJet.ExtraMuWeight->at(3) = tnp_weight_muid_pbpb(muPt1, muEta1, -2) / tnp_weight_muid_pbpb(muPt1, muEta1, 0) *
                                      tnp_weight_muid_pbpb(muPt2, muEta2, -2) / tnp_weight_muid_pbpb(muPt2, muEta2, 0);
          MMuMuJet.ExtraMuWeight->at(4) = tnp_weight_trg_pbpb(muPt1, muEta1,0, -1) / tnp_weight_trg_pbpb(muPt1, muEta1,0, 0) *
                                      tnp_weight_trg_pbpb(muPt2, muEta2,0, -1) / tnp_weight_trg_pbpb(muPt2, muEta2,0, 0);
          MMuMuJet.ExtraMuWeight->at(5) = tnp_weight_trg_pbpb(muPt1, muEta1,0, -2) / tnp_weight_trg_pbpb(muPt1,0, muEta1, 0) *
                                      tnp_weight_trg_pbpb(muPt2, muEta2,0, -2) / tnp_weight_trg_pbpb(muPt2,0, muEta2, 0);
          MMuMuJet.ExtraMuWeight->at(6) = tnp_weight_trk_pbpb(muEta1, 1) / tnp_weight_trk_pbpb(muEta1, 0) *
                                      tnp_weight_trk_pbpb(muEta2, 1) / tnp_weight_trk_pbpb(muEta2, 0);
          MMuMuJet.ExtraMuWeight->at(7) = tnp_weight_trk_pbpb(muEta1, 2) / tnp_weight_trk_pbpb(muEta1, 0) *
                                      tnp_weight_trk_pbpb(muEta2, 2) / tnp_weight_trk_pbpb(muEta2, 0);
          MMuMuJet.ExtraMuWeight->at(8) = tnp_weight_muid_pbpb(muPt1, muEta1, 1) / tnp_weight_muid_pbpb(muPt1, muEta1, 0) *
                                      tnp_weight_muid_pbpb(muPt2, muEta2, 1) / tnp_weight_muid_pbpb(muPt2, muEta2, 0);
          MMuMuJet.ExtraMuWeight->at(9) = tnp_weight_muid_pbpb(muPt1, muEta1, 2) / tnp_weight_muid_pbpb(muPt1, muEta1, 0) *
                                      tnp_weight_muid_pbpb(muPt2, muEta2, 2) / tnp_weight_muid_pbpb(muPt2, muEta2, 0);
          MMuMuJet.ExtraMuWeight->at(10) = tnp_weight_trg_pbpb(muPt1, muEta1,0, 1) / tnp_weight_trg_pbpb(muPt1, muEta1,0, 0) *
                                      tnp_weight_trg_pbpb(muPt2, muEta2,0, 1) / tnp_weight_trg_pbpb(muPt2, muEta2,0, 0);
          MMuMuJet.ExtraMuWeight->at(11) = tnp_weight_trg_pbpb(muPt1, muEta1,0, 2) / tnp_weight_trg_pbpb(muPt1, muEta1,0, 0) *
                                      tnp_weight_trg_pbpb(muPt2, muEta2,0, 2) / tnp_weight_trg_pbpb(muPt2, muEta2,0, 0);
          }

         MMuMuJet.FillEntry();
      } // end loop over jets

      for(int i = 0; i< MJet.GenCount; i++){
        MGenMuMuJet.GenJetPT = MJet.GenPT[i];
        MGenMuMuJet.GenJetEta = MJet.GenEta[i];
        MGenMuMuJet.GenJetPhi = MJet.GenPhi[i];
        MGenMuMuJet.GenJetMatchIdx = MJet.GenMatchIndex[i];
        MGenMuMuJet.FillEntry();
      }

      
      
    } // end loop over events

    Bar.Update(EntryCount);
    Bar.Print();
    Bar.PrintLine();

    InputFile.Close();
  } // end loop over input files

  OutputFile.cd();
  Tree.Write();
  GenTree.Write();
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
  if (M->SingleMuPT->at(i) < 3.5)
    return false;
  if (fabs(M->SingleMuEta->at(i)) > 2.3)
    return false;
  if (M->SingleMuIsTracker->at(i) == 0 || M->SingleMuIsGlobal->at(i) == 0 || M->SingleMuHybridSoft->at(i) == 0 ||
      M->SingleMuIsGood->at(i) == 0)
    return false;

  return true;
}

bool isGenMuonSelected(SingleMuTreeMessenger *M, int i) {
  if (M == nullptr)
    return false;
  if (M->Tree == nullptr)
    return false;
  if (M->GenSingleMuPT->at(i) < 3.5)
    return false;
  if (fabs(M->GenSingleMuEta->at(i)) > 2.3)
    return false;
  //if (M->SingleMuIsTracker->at(i) == 0 || M->SingleMuIsGlobal->at(i) == 0 || M->SingleMuHybridSoft->at(i) == 0 ||
  //  M->SingleMuIsGood->at(i) == 0)
  //  return false;

  return true;
}

bool isDimuonGenMatched(SingleMuTreeMessenger *M, int gen1, int gen2, int reco1, int reco2) {
  if (M == nullptr)
    return false;
  if (M->Tree == nullptr)
    return false;

  float deta11 = M->GenSingleMuEta->at(gen1) - M->SingleMuEta->at(reco1);  
  float dphi11 = DeltaPhi(M->GenSingleMuPhi->at(gen1), M->SingleMuPhi->at(reco1));
  float dr11 = sqrt(dphi11 * dphi11 + deta11 * deta11);
  bool chargematch11 = (M->GenSingleMuPID->at(gen1) * M->SingleMuCharge->at(reco1) < 0);

  float deta22 = M->GenSingleMuEta->at(gen2) - M->SingleMuEta->at(reco2);  
  float dphi22 = DeltaPhi(M->GenSingleMuPhi->at(gen2), M->SingleMuPhi->at(reco2));
  float dr22 = sqrt(dphi22 * dphi22 + deta22 * deta22);
  bool chargematch22 = (M->GenSingleMuPID->at(gen2) * M->SingleMuCharge->at(reco2) < 0);

  float deta12 = M->GenSingleMuEta->at(gen1) - M->SingleMuEta->at(reco2);  
  float dphi12 = DeltaPhi(M->GenSingleMuPhi->at(gen1), M->SingleMuPhi->at(reco2));
  float dr12 = sqrt(dphi12 * dphi12 + deta12 * deta12);
  bool chargematch12 = (M->GenSingleMuPID->at(gen1) * M->SingleMuCharge->at(reco2) < 0);

  float deta21 = M->GenSingleMuEta->at(gen2) - M->SingleMuEta->at(reco1);  
  float dphi21 = DeltaPhi(M->GenSingleMuPhi->at(gen2), M->SingleMuPhi->at(reco1));
  float dr21 = sqrt(dphi21 * dphi21 + deta21 * deta21);
  bool chargematch21 = (M->GenSingleMuPID->at(gen2) * M->SingleMuCharge->at(reco1) < 0);

  if((dr11 < 0.03) && (dr22 < 0.03) && !chargematch11 && !chargematch22){
    return true;
  }
  if((dr12 < 0.03) && (dr21 < 0.03) && !chargematch12 && !chargematch21){
    return true;
  }


  return false;
}

/*int isOnia(float mass){


    // J/psi mass window
    float mjpsi = 3.0969;
    float mjpsi_width = .0002778;
    if(fabs(mass - mjpsi) < mjpsi_width){
        return 1; // J/psi
    }

    // Psi(2S) mass window
    float mpsi2s = 3.686097; 
    float mpsi2s_width = 0.000879; // 0.67 MeV
    if(fabs(mass - mpsi2s) < mpsi2s_width){
        return 2; // Psi(2S)
    }

    // Upsilon
    float mupsilon = 9.46040;
    float mupsilon_width = 0.00016206; // 260 keV
    if(fabs(mass - mupsilon) < mupsilon_width){
        return 3; // Upsilon
    }
    
    return 0;

}*/

std::vector<int> mu_trackmatch(JetTreeMessenger *MJet, int jetno, float pt, float eta, float phi){

  float dr_cut = 0.0004; // SUBJECT TO TUNING
  std::vector<int> idx = {-1, -1};
  std::vector<int> bad = {-1, -1};

  if (MJet == nullptr) {
    return bad;
  }
  if (MJet->Tree == nullptr) {
    return bad;
  }

  int c = 0;
  for (int i = 0; i < MJet->ntrk; i++) {
    // if(MJet->trkJetId[i] != jetno){continue;}
    //if (fabs(MJet->trkPt[i] - pt) > 5) {
    //  continue;
    //}
    if (fabs(MJet->trkPdgId[i]) != 13) {
      continue;
    }
    float dEta = MJet->trkEta[i] - eta;
    float dPhi = DeltaPhi(MJet->trkPhi[i], phi);

    float dR = sqrt(dEta * dEta + dPhi * dPhi);
    if(dR > dr_cut){continue;}

    c += 1;
    idx[0] = i;
    idx[1] = MJet->trkSvtxId[i];
  }

  if (c != 1) {
    return bad;
  } else {
    return idx;
  }
}
