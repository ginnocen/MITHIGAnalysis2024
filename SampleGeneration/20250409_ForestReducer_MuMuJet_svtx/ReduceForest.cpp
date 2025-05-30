#include <iostream>
using namespace std;

#include "TFile.h"
#include "TLorentzVector.h"
#include "TTree.h"

#include "CommandLine.h"
#include "CommonFunctions.h"
#include "Messenger.h"
#include "ProgressBar.h"

#include "tnp_weight_lowptPbPb.h"


int main(int argc, char *argv[]);
double GetHFSum(PFTreeMessenger *M);
double GetGenHFSum(GenParticleTreeMessenger *M, int SubEvent = -1);
bool isMuonSelected(SingleMuTreeMessenger *M, int i);
//int isOnia(float mass);
std::vector<int> mu_trackmatch(float dr_cut, JetTreeMessenger *MJet, int jetno, float pt, float eta, float phi);

int main(int argc, char *argv[]) {
  string VersionString = "V8";

  CommandLine CL(argc, argv);

  vector<string> InputFileNames = CL.GetStringVector("Input");
  string OutputFileName = CL.Get("Output");

  bool IsData = CL.GetBool("IsData", false);
  bool IsPP = CL.GetBool("IsPP", false);
  bool svtx = CL.GetBool("svtx", false);
  float dr_cut = CL.GetDouble("dr_cut", 0.0004);
  //int cent_high  = CL.GetInt("cent_high", 100);
  //int cent_low = CL.GetInt("cent_low", 0);

  int Year = CL.GetInt("Year", 2018);
  double Fraction = CL.GetDouble("Fraction", 1.00);
  double MinJetPT = CL.GetDouble("MinJetPT", 40);
  string PFJetCollection = CL.Get("PFJetCollection", "akCs3PFJetAnalyzer/t");
  string PFTreeName = IsPP ? "pfcandAnalyzer/pfTree" : "particleFlowAnalyser/pftree";
  PFTreeName = CL.Get("PFTree", PFTreeName);

  TFile OutputFile(OutputFileName.c_str(), "RECREATE");
  TTree Tree("Tree", Form("Tree for MuMu tagged jet analysis (%s)", VersionString.c_str()));
  TTree InfoTree("InfoTree", "Information");

  MuMuJetMessenger MMuMuJet;
  MMuMuJet.SetBranch(&Tree);

  std::vector<int> svtxJetId_v;
  std::vector<int> svtxNtrk_v;
  std::vector<float> svtxdl_v;
  std::vector<float> svtxdls_v;
  std::vector<float> svtxdl2d_v;
  std::vector<float> svtxdls2d_v;
  std::vector<float> svtxm_v;
  std::vector<float> svtxmcorr_v;
  std::vector<float> svtxpt_v;
  std::vector<float> svtxnormchi2_v;
  std::vector<float> svtxchi2_v;

  std::vector<int> trkJetId_v;
  std::vector<int> trkSvtxId_v;
  std::vector<float> trkPt_v;
  std::vector<float> trkEta_v;
  std::vector<float> trkPhi_v;
  std::vector<float> trkIp3d_v;
  std::vector<float> trkIp3dSig_v;
  std::vector<float> trkIp2d_v;
  std::vector<float> trkIp2dSig_v;
  std::vector<float> trkDistToAxis_v;
  std::vector<float> trkDistToAxisSig_v;
  std::vector<float> trkIpProb3d_v;
  std::vector<float> trkIpProb2d_v;
  std::vector<float> trkDz_v;
  std::vector<int> trkPdgId_v;
  std::vector<int> trkMatchSta_v;

  std::vector<int> mt2;
  std::vector<int> mt1;

  for (string InputFileName : InputFileNames) {

    std::cout << "Processing file: " << InputFileName << std::endl;
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
      // std::cout << "Event " << iE << endl;

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
      //if(hiBin/2.0 > cent_high || hiBin/2.0 < cent_low) {continue;}
      MMuMuJet.hiHF = MEvent.hiHF;
      MMuMuJet.NPU = 0;
      if (MEvent.npus->size() == 9)
        MMuMuJet.NPU = MEvent.npus->at(5);
      else if (MEvent.npus->size() > 1)
        MMuMuJet.NPU = MEvent.npus->at(0);
      else
        MMuMuJet.NPU = 0;

      MMuMuJet.nsvtx = MJet.nsvtx;
      MMuMuJet.ntrk = MJet.ntrk;

      MMuMuJet.PTHat = MEvent.pthat;
      MMuMuJet.EventWeight = MEvent.weight;
      MMuMuJet.NCollWeight = FindNColl(MMuMuJet.hiBin);
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
          if (pprimaryVertexFilter == 0 || beamScrapingFilter == 0)
            continue;
          int minbiastrigger = MTrigger.CheckTriggerStartWith("HLT_HIMinimumBias_v*");
          if (minbiastrigger == 0)
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
          int minbiastrigger = MTrigger.CheckTriggerStartWith("HLT_HIMinimumBias_v*");
          if (minbiastrigger == 0)
            continue;
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
        
        MMuMuJet.MJTHadronFlavor->push_back(MJet.MJTHadronFlavor[ijet]);
        MMuMuJet.MJTNcHad->push_back(MJet.MJTNcHad[ijet]);
        MMuMuJet.MJTNbHad->push_back(MJet.MJTNbHad[ijet]);
        MMuMuJet.JetPT->push_back(MJet.JetPT[ijet]);
        MMuMuJet.JetEta->push_back(MJet.JetEta[ijet]);
        MMuMuJet.JetPhi->push_back(MJet.JetPhi[ijet]);

        if (svtx) {

          // ADD SVTX INFORMATION

          MMuMuJet.jtNsvtx->push_back(MJet.jtNsvtx[ijet]);
          MMuMuJet.jtNtrk->push_back(MJet.jtNtrk[ijet]);
          MMuMuJet.jtptCh->push_back(MJet.jtptCh[ijet]);

          for (int isvtx = 0; isvtx < MJet.nsvtx; isvtx++) {

            if (MJet.svtxJetId[isvtx] == ijet) {

              // cout << MJet.svtxJetId[isvtx] << endl;
              svtxJetId_v.push_back(MJet.svtxJetId[isvtx]);
              svtxNtrk_v.push_back(MJet.svtxNtrk[isvtx]);
              svtxdl_v.push_back(MJet.svtxdl[isvtx]);
              svtxdls_v.push_back(MJet.svtxdls[isvtx]);
              svtxdl2d_v.push_back(MJet.svtxdl2d[isvtx]);
              svtxdls2d_v.push_back(MJet.svtxdls2d[isvtx]);
              svtxm_v.push_back(MJet.svtxm[isvtx]);
              svtxmcorr_v.push_back(MJet.svtxmcorr[isvtx]);
              svtxpt_v.push_back(MJet.svtxpt[isvtx]);
              svtxnormchi2_v.push_back(MJet.svtxnormchi2[isvtx]);
              svtxchi2_v.push_back(MJet.svtxchi2[isvtx]);
            }
          }

          MMuMuJet.svtxJetId->push_back(svtxJetId_v);
          MMuMuJet.svtxNtrk->push_back(svtxNtrk_v);
          MMuMuJet.svtxdl->push_back(svtxdl_v);
          MMuMuJet.svtxdls->push_back(svtxdls_v);
          MMuMuJet.svtxdl2d->push_back(svtxdl2d_v);
          MMuMuJet.svtxdls2d->push_back(svtxdls2d_v);
          MMuMuJet.svtxm->push_back(svtxm_v);
          MMuMuJet.svtxmcorr->push_back(svtxmcorr_v);
          MMuMuJet.svtxpt->push_back(svtxpt_v);
          MMuMuJet.svtxnormchi2->push_back(svtxnormchi2_v);
          MMuMuJet.svtxchi2->push_back(svtxchi2_v);

          for (int itrk = 0; itrk < MJet.ntrk; itrk++) {
            if (MJet.trkJetId[itrk] == ijet) {

              // ADD TRACK INFORMATION

              // cout << "ping " << MJet.trkJetId[itrk] << " " << MJet.trkSvtxId[itrk] << endl;
              trkJetId_v.push_back(MJet.trkJetId[itrk]);
              trkSvtxId_v.push_back(MJet.trkSvtxId[itrk]);
              trkPt_v.push_back(MJet.trkPt[itrk]);
              trkEta_v.push_back(MJet.trkEta[itrk]);
              trkPhi_v.push_back(MJet.trkPhi[itrk]);
              trkIp3d_v.push_back(MJet.trkIp3d[itrk]);
              trkIp3dSig_v.push_back(MJet.trkIp3dSig[itrk]);
              trkIp2d_v.push_back(MJet.trkIp2d[itrk]);
              trkIp2dSig_v.push_back(MJet.trkIp2dSig[itrk]);
              trkDistToAxis_v.push_back(MJet.trkDistToAxis[itrk]);
              trkDistToAxisSig_v.push_back(MJet.trkDistToAxisSig[itrk]);
              trkIpProb3d_v.push_back(MJet.trkIpProb3d[itrk]);
              trkIpProb2d_v.push_back(MJet.trkIpProb2d[itrk]);
              trkDz_v.push_back(MJet.trkDz[itrk]);
              trkPdgId_v.push_back(MJet.trkPdgId[itrk]);
              trkMatchSta_v.push_back(MJet.trkMatchSta[itrk]);
            }
          }

          MMuMuJet.trkJetId->push_back(trkJetId_v);
          MMuMuJet.trkSvtxId->push_back(trkSvtxId_v);
          MMuMuJet.trkPt->push_back(trkPt_v);
          MMuMuJet.trkEta->push_back(trkEta_v);
          MMuMuJet.trkPhi->push_back(trkPhi_v);
          MMuMuJet.trkIp3d->push_back(trkIp3d_v);
          MMuMuJet.trkIp3dSig->push_back(trkIp3dSig_v);
          MMuMuJet.trkIp2d->push_back(trkIp2d_v);
          MMuMuJet.trkIp2dSig->push_back(trkIp2dSig_v);
          MMuMuJet.trkDistToAxis->push_back(trkDistToAxis_v);
          MMuMuJet.trkDistToAxisSig->push_back(trkDistToAxisSig_v);
          MMuMuJet.trkIpProb3d->push_back(trkIpProb3d_v);
          MMuMuJet.trkIpProb2d->push_back(trkIpProb2d_v);
          MMuMuJet.trkDz->push_back(trkDz_v);
          MMuMuJet.trkPdgId->push_back(trkPdgId_v);
          MMuMuJet.trkMatchSta->push_back(trkMatchSta_v);

          svtxJetId_v.clear();
          svtxNtrk_v.clear();
          svtxdl_v.clear();
          svtxdls_v.clear();
          svtxdl2d_v.clear();
          svtxdls2d_v.clear();
          svtxm_v.clear();
          svtxmcorr_v.clear();
          svtxpt_v.clear();
          svtxnormchi2_v.clear();
          svtxchi2_v.clear();

          trkJetId_v.clear();
          trkSvtxId_v.clear();
          trkPt_v.clear();
          trkEta_v.clear();
          trkPhi_v.clear();
          trkIp3d_v.clear();
          trkIp3dSig_v.clear();
          trkIp2d_v.clear();
          trkIp2dSig_v.clear();
          trkDistToAxis_v.clear();
          trkDistToAxisSig_v.clear();
          trkIpProb3d_v.clear();
          trkIpProb2d_v.clear();
          trkDz_v.clear();
          trkPdgId_v.clear();
          trkMatchSta_v.clear();
        }

        bool isJetTagged = false;
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
        float maxmumuPt = 0.;
        int maxMu1Index = -1;
        int maxMu2Index = -1;

        int nSingleMu = MSingleMu.SingleMuPT->size();
        for (int isinglemu1 = 0; isinglemu1 < nSingleMu; isinglemu1++) {
          if (isMuonSelected(&MSingleMu, isinglemu1) == false)
            continue;
          for (int isinglemu2 = isinglemu1 + 1; isinglemu2 < nSingleMu; isinglemu2++) {
            if (isMuonSelected(&MSingleMu, isinglemu2) == false)
              continue;
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
          isJetTagged = true;
  
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

        MMuMuJet.IsMuMuTagged->push_back(isJetTagged);
        MMuMuJet.muPt1->push_back(muPt1);
        MMuMuJet.muPt2->push_back(muPt2);
        MMuMuJet.muEta1->push_back(muEta1);
        MMuMuJet.muEta2->push_back(muEta2);
        MMuMuJet.muPhi1->push_back(muPhi1);
        MMuMuJet.muPhi2->push_back(muPhi2);
        MMuMuJet.muCharge1->push_back(muCharge1);
        MMuMuJet.muCharge2->push_back(muCharge2);
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
        //MMuMuJet.mumuisOnia->push_back(mumuisOnia);
        MMuMuJet.DRJetmu1->push_back(dRJetMu1);
        MMuMuJet.DRJetmu2->push_back(dRJetMu2);
        MMuMuJet.muDeta->push_back(muDeta);
        MMuMuJet.muDphi->push_back(muDphi);
        MMuMuJet.muDR->push_back(muDR);

        mt1 = mu_trackmatch(dr_cut, &MJet, ijet, muPt1, muEta1, muPhi1);
        mt2 = mu_trackmatch(dr_cut, &MJet, ijet, muPt2, muEta2, muPhi2);

        MMuMuJet.trkIdx_mu1->push_back(mt1[0]);
        MMuMuJet.trkIdx_mu2->push_back(mt2[0]);
        MMuMuJet.svtxIdx_mu1->push_back(mt1[1]);
        MMuMuJet.svtxIdx_mu2->push_back(mt2[1]);

        mt1.clear();
        mt2.clear();

        MMuMuJet.MuMuWeight = tnp_weight_trk_pbpb(muEta1, 0) * tnp_weight_trk_pbpb(muEta2, 0) *
                              tnp_weight_muid_pbpb(muPt1, muEta1, 0) * tnp_weight_muid_pbpb(muPt2, muEta2, 0);

        MMuMuJet.ExtraMuWeight[0] = tnp_weight_trk_pbpb(muEta1, -1) / tnp_weight_trk_pbpb(muEta1, 0) *
                                    tnp_weight_trk_pbpb(muEta2, -1) / tnp_weight_trk_pbpb(muEta2, 0);
        MMuMuJet.ExtraMuWeight[1] = tnp_weight_trk_pbpb(muEta1, -2) / tnp_weight_trk_pbpb(muEta1, 0) *
                                    tnp_weight_trk_pbpb(muEta2, -2) / tnp_weight_trk_pbpb(muEta2, 0);
        MMuMuJet.ExtraMuWeight[2] = tnp_weight_muid_pbpb(muPt1, muEta1, -1) / tnp_weight_muid_pbpb(muPt1, muEta1, 0) *
                                    tnp_weight_muid_pbpb(muPt2, muEta2, -1) / tnp_weight_muid_pbpb(muPt2, muEta2, 0);
        MMuMuJet.ExtraMuWeight[3] = tnp_weight_muid_pbpb(muPt1, muEta1, -2) / tnp_weight_muid_pbpb(muPt1, muEta1, 0) *
                                    tnp_weight_muid_pbpb(muPt2, muEta2, -2) / tnp_weight_muid_pbpb(muPt2, muEta2, 0);
        MMuMuJet.ExtraMuWeight[4] = tnp_weight_trg_pbpb(muPt1, muEta1,0, -1) / tnp_weight_trg_pbpb(muPt1, muEta1,0, 0) *
                                    tnp_weight_trg_pbpb(muPt2, muEta2,0, -1) / tnp_weight_trg_pbpb(muPt2, muEta2,0, 0);

        MMuMuJet.ExtraMuWeight[5] = tnp_weight_trg_pbpb(muPt1, muEta1,0, -2) / tnp_weight_trg_pbpb(muPt1,0, muEta1, 0) *
                                    tnp_weight_trg_pbpb(muPt2, muEta2,0, -2) / tnp_weight_trg_pbpb(muPt2,0, muEta2, 0);

        MMuMuJet.ExtraMuWeight[6] = tnp_weight_trk_pbpb(muEta1, 1) / tnp_weight_trk_pbpb(muEta1, 0) *
                                    tnp_weight_trk_pbpb(muEta2, 1) / tnp_weight_trk_pbpb(muEta2, 0);

        MMuMuJet.ExtraMuWeight[7] = tnp_weight_trk_pbpb(muEta1, 2) / tnp_weight_trk_pbpb(muEta1, 0) *
                                    tnp_weight_trk_pbpb(muEta2, 2) / tnp_weight_trk_pbpb(muEta2, 0);
        MMuMuJet.ExtraMuWeight[8] = tnp_weight_muid_pbpb(muPt1, muEta1, 1) / tnp_weight_muid_pbpb(muPt1, muEta1, 0) *
                                    tnp_weight_muid_pbpb(muPt2, muEta2, 1) / tnp_weight_muid_pbpb(muPt2, muEta2, 0);
        MMuMuJet.ExtraMuWeight[9] = tnp_weight_muid_pbpb(muPt1, muEta1, 2) / tnp_weight_muid_pbpb(muPt1, muEta1, 0) *
                                    tnp_weight_muid_pbpb(muPt2, muEta2, 2) / tnp_weight_muid_pbpb(muPt2, muEta2, 0);

        MMuMuJet.ExtraMuWeight[10] = tnp_weight_trg_pbpb(muPt1, muEta1,0, 1) / tnp_weight_trg_pbpb(muPt1, muEta1,0, 0) *
                                     tnp_weight_trg_pbpb(muPt2, muEta2,0, 1) / tnp_weight_trg_pbpb(muPt2, muEta2,0, 0);

        MMuMuJet.ExtraMuWeight[11] = tnp_weight_trg_pbpb(muPt1, muEta1,0, 2) / tnp_weight_trg_pbpb(muPt1, muEta1,0, 0) *
                                     tnp_weight_trg_pbpb(muPt2, muEta2,0, 2) / tnp_weight_trg_pbpb(muPt2, muEta2,0, 0);

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
  if (M->SingleMuPT->at(i) < 3.5)
    return false;
  if (fabs(M->SingleMuEta->at(i)) > 2.3)
    return false;
  if (M->SingleMuIsTracker->at(i) == 0 || M->SingleMuIsGlobal->at(i) == 0 || M->SingleMuHybridSoft->at(i) == 0 ||
      M->SingleMuIsGood->at(i) == 0)
    return false;

  return true;
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

std::vector<int> mu_trackmatch(float dr_cut, JetTreeMessenger *MJet, int jetno, float pt, float eta, float phi){

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
