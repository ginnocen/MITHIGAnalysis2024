// Purpose: Read the skimmed tree and make a histogram of the D meson mass
// How to run: root -l standalone_cross.C

// ROOT
#include <TDirectoryFile.h>
// #include <TEnv.h>
#include <TFile.h>
#include <TMath.h>
// #include <TLorentzVector.h>
#include <TCanvas.h>
#include <TCut.h>
#include <TH1.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TTree.h>
// c and cpp
#include <algorithm> // for sort
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
// #include "uti.h"
// #include "parameters.h"
#include "TF1.h"
#include "TF2.h"
#include "TH2D.h"
#include <TError.h>

// Set true to make and fill efficiency comparison plots

int main(int argc, char *argv[]) {
  gErrorIgnoreLevel = kError;
  // input arguments
  // 1. input data file
  TString nameinputdata = argv[1];
  // 2. output file
  TString nameoutput = argv[2];
  TFile *finput = TFile::Open(nameinputdata.Data());
  TTree *nt = (TTree *)finput->Get("PbPbTracks/trackTree"); // Get the D meson tree
  nt->AddFriend("hltanalysis/HltTree");                     // Add the HLT tree as a friend
  nt->AddFriend("skimanalysis/HltTree");                    // Add the skim tree as a friend
  nt->AddFriend("hiEvtAnalyzer/HiTree");                    // Add the event tree as a friend
  nt->AddFriend("particleFlowAnalyser/pftree");             // Add the PF candidate tree as a friend

  // analysis param
  float pfEtaAbsMin = 3.;
  float pfEtaAbsMax = 5.2;

  int nVtx, nTrk;
  int nRun;
  int pclusterCompatibilityFilter, pprimaryVertexFilter;
  float BSz, BSx, BSy;
  float PVx, PVy, PVz;
  float pthat, pthatWeight, softQCDWeight;

  std::vector<float> *pfE = nullptr;
  std::vector<float> *xVtx = nullptr;
  std::vector<float> *yVtx = nullptr;
  std::vector<float> *zVtx = nullptr;
  std::vector<float> *nTracksVtx = nullptr;
  std::vector<float> *chi2Vtx = nullptr;
  std::vector<float> *ndofVtx = nullptr;
  std::vector<float> *pfEta = nullptr;
  std::vector<int> *pfId = nullptr;
  std::vector<float> *trkEta = nullptr;
  std::vector<float> *trkPt = nullptr;
  std::vector<bool> *highPurity = nullptr;

  // set branches of variables
  bool hasclusterCompatibilityFilter = false;

  nt->SetBranchAddress("pclusterCompatibilityFilter", &pclusterCompatibilityFilter);
  nt->SetBranchAddress("pprimaryVertexFilter", &pprimaryVertexFilter);
  nt->SetBranchAddress("nVtx", &nVtx);
  nt->SetBranchAddress("zVtx", &(zVtx));
  nt->SetBranchAddress("nTrk", &nTrk);
  nt->SetBranchAddress("pfE", &(pfE));
  nt->SetBranchAddress("pfEta", &pfEta);
  nt->SetBranchAddress("trkEta", &trkEta);
  nt->SetBranchAddress("trkPt", &trkPt);
  nt->SetBranchAddress("pfId", &pfId);
  nt->SetBranchAddress("pfEta", &pfEta);
  nt->SetBranchAddress("pfE", &pfE);

  Long64_t entries = nt->GetEntries();

  TH1F *hnVtx = new TH1F("hnVtx", "Number of vertices", 5, -0.5, 4.5);
  TH2F *hTrackPtEta = new TH2F("hTrackPtEta", ";#eta; p_{T}; Track pT vs eta", 100, -4, 4, 100, 0, 50);
  TH1F *hPFHFMaxEPlus = new TH1F("hPFHFMaxEPlus", "PFHFMaxEPlus", 200, 0, 200);
  TH1F *hPFHFMaxEMinus = new TH1F("hPFHFMaxEMinus", "PFHFMaxEMinus", 200, 0, 200);
  TH1F *hPFHFEPlus = new TH1F("hPFHFEPlus", "PFHFEPlus", 200, 0, 200);
  TH1F *hPFHFEMinus = new TH1F("hPFHFEMinus", "PFHFEMinus", 200, 0, 200);

  for (Long64_t i = 0; i < entries; i++) {
    nt->GetEntry(i);
    if (i % 1000 == 0)
      std::cout << "Processing event " << i << " of " << entries << std::endl;
    if ((nVtx < 1) || (pclusterCompatibilityFilter == 0) || (pprimaryVertexFilter == 0)) {
      continue; // skip events with no vertices
    }

    float PFHFTotPlus = 0.;
    float PFHFTotMinus = 0.;
    float PFHFMaxEPlus = 0.;
    float PFHFMaxEMinus = 0.;
    float PFHFSecMaxEPlus = 0.;
    float PFHFSecMaxEMinus = 0.;
    for (long unsigned int j = 0; j < pfE->size(); j++) {
      if ((*pfId)[j] == 6 || (*pfId)[j] == 7) {
        if ((*pfEta)[j] > pfEtaAbsMin & (*pfEta)[j] < pfEtaAbsMax) {
          hPFHFEPlus->Fill((*pfE)[j]);
          PFHFTotPlus += (*pfE)[j];
          if ((*pfE)[j] > PFHFMaxEPlus) {
            PFHFSecMaxEPlus = PFHFMaxEPlus;
            PFHFMaxEPlus = (*pfE)[j];
          } else if ((*pfE)[j] > PFHFSecMaxEPlus) {
            PFHFSecMaxEPlus = (*pfE)[j];
          }
        } // end of pfEtaAbsMin & pfEtaAbsMax plus region
        if ((*pfEta)[j]<-pfEtaAbsMin &(*pfEta)[j]> - pfEtaAbsMax) {
          hPFHFEMinus->Fill((*pfE)[j]);
          PFHFTotMinus += (*pfE)[j];
          if ((*pfE)[j] > PFHFMaxEMinus) {
            PFHFSecMaxEMinus = PFHFMaxEMinus;
            PFHFMaxEMinus = (*pfE)[j];
          } else if ((*pfE)[j] > PFHFSecMaxEMinus) {
            PFHFSecMaxEMinus = (*pfE)[j];
          }
        } // end of pfEtaAbsMin & pfEtaAbsMax minus region
      } // end of pfId == 6 || pfId == 7
    } // end of loop over pf candidates
    hPFHFMaxEPlus->Fill(PFHFMaxEPlus);
    hPFHFMaxEMinus->Fill(PFHFMaxEMinus);
    hnVtx->Fill(nVtx);
    // fill the trk pt histogram

    for (int itrk = 0; itrk < trkPt->size(); itrk++) {
      hTrackPtEta->Fill(trkEta->at(itrk), trkPt->at(itrk));
    }
  } // end of loop over events

  TFile *fout = new TFile(nameoutput.Data(), "recreate");
  fout->cd();
  hnVtx->Write();
  hTrackPtEta->Write();
  hPFHFEPlus->Write();
  hPFHFMaxEPlus->Write();
  hPFHFMaxEMinus->Write();
  delete fout;
}
