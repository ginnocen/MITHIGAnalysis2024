// aksdjk`
#include "../../CommonCode/include/CommonFunctions.h"
#include "../../CommonCode/include/ProgressBar.h"
#include <TF1.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <iostream>
using namespace std;

bool isMuonSelected(vector<float> *pT, vector<float> *eta, vector<bool> *isTracker, vector<bool> *isGlobal,
                    vector<bool> *hybridSoft, vector<bool> *isGood, int i) {
  if (pT->at(i) < 3.5) {
    return false;
  }
  if (fabs(eta->at(i)) > 2.3) {
    return false;
  }
  if (isTracker->at(i) == 0 || isGlobal->at(i) == 0 || hybridSoft->at(i) == 0 || isGood->at(i) == 0) {
    return false;
  }

  return true;
}

void validate(int Event, int Vertex, int Jet, int SVTX, int Track, int Muon) {

  // gSystem->Load("../20250409_ForestReducer_MuMuJet_svtx/libMyDict.so");
  gInterpreter->GenerateDictionary("vector<vector<int>>", "vector");
  gInterpreter->GenerateDictionary("vector<vector<float>>", "vector");

  cout << "start" << endl;

  // import files

  TFile *f = TFile::Open("/data00/g2ccbar/mc2018/skimmer_validation/gccbartestforest.root");
  TFile *fskim = TFile::Open("/data00/g2ccbar/mc2018/skimmer_validation/skim.root");

  // organize trees

  TTree *Tskim = (TTree *)fskim->Get("Tree");

  TTree *T_hltanalysis = (TTree *)f->Get("hltanalysis/HltTree");
  TTree *T_muonAnalyzer = (TTree *)f->Get("muonAnalyzer/MuonTree");
  TTree *T_skimanalysis = (TTree *)f->Get("skimanalysis/HltTree");
  TTree *T_HiForestInfo = (TTree *)f->Get("HiForestInfo/HiForest");
  TTree *T_PbPbTracks = (TTree *)f->Get("PbPbTracks/trackTree");
  TTree *T_hiEvtAnalyzer = (TTree *)f->Get("hiEvtAnalyzer/HiTree");
  TTree *T_hltMuTree = (TTree *)f->Get("hltMuTree/HLTMuTree");
  TTree *T_akCs3PFJetAnalyzer = (TTree *)f->Get("akCs3PFJetAnalyzer/t");
  TTree *T_bHadronAna = (TTree *)f->Get("bHadronAna/hi");
  TTree *T_cHadronAna = (TTree *)f->Get("cHadronAna/hi");

  // Global Event parameters

  int run_skim = 0;
  UInt_t run_forest = 0;
  Tskim->SetBranchAddress("Run", &run_skim);
  T_hiEvtAnalyzer->SetBranchAddress("run", &run_forest);

  int lumi_skim = 0;
  UInt_t lumi_forest = 0;
  Tskim->SetBranchAddress("Lumi", &lumi_skim);
  T_hiEvtAnalyzer->SetBranchAddress("lumi", &lumi_forest);

  Long64_t event_skim = 0;
  ULong64_t event_forest = 0;

  Tskim->SetBranchAddress("Event", &event_skim);
  T_hiEvtAnalyzer->SetBranchAddress("evt", &event_forest);

  int hiBin_skim = 0;
  int hiBin_forest = 0;

  Tskim->SetBranchAddress("hiBin", &hiBin_skim);
  T_hiEvtAnalyzer->SetBranchAddress("hiBin", &hiBin_forest);

  float ncollweight_skim = 0;
  float ncollweight_forest = 0;

  Tskim->SetBranchAddress("NCollWeight", &ncollweight_skim);

  float eventweight_skim = 0;
  float eventweight_forest = 0;

  Tskim->SetBranchAddress("EventWeight", &eventweight_skim);
  T_hiEvtAnalyzer->SetBranchAddress("weight", &eventweight_forest);

  float pthat_skim = 0;
  float pthat_forest = 0;

  Tskim->SetBranchAddress("PTHat", &pthat_skim);
  T_hiEvtAnalyzer->SetBranchAddress("pthat", &pthat_forest);

  float hiHF_skim = 0;
  float hiHF_forest = 0;

  Tskim->SetBranchAddress("hiHF", &hiHF_skim);
  T_hiEvtAnalyzer->SetBranchAddress("hiHF", &hiHF_forest);

  // Vertex parameters

  float vx_skim = 0;
  float vx_forest = 0;
  Tskim->SetBranchAddress("VX", &vx_skim);

  float vy_skim = 0;
  float vy_forest = 0;
  Tskim->SetBranchAddress("VY", &vy_skim);

  float vz_skim = 0;
  float vz_forest = 0;
  Tskim->SetBranchAddress("VZ", &vz_skim);

  float vxerror_skim = 0;
  float vxerror_forest = 0;
  Tskim->SetBranchAddress("VXError", &vxerror_skim);

  float vyerror_skim = 0;
  float vyerror_forest = 0;
  Tskim->SetBranchAddress("VYError", &vyerror_skim);

  float vzerror_skim = 0;
  float vzerror_forest = 0;
  Tskim->SetBranchAddress("VZError", &vzerror_skim);

  vector<float> *vx_forest_vec = nullptr;
  T_PbPbTracks->SetBranchAddress("xVtx", &vx_forest_vec);

  vector<float> *vy_forest_vec = nullptr;
  T_PbPbTracks->SetBranchAddress("yVtx", &vy_forest_vec);

  vector<float> *vz_forest_vec = nullptr;
  T_PbPbTracks->SetBranchAddress("zVtx", &vz_forest_vec);

  vector<float> *vxerror_forest_vec = nullptr;
  T_PbPbTracks->SetBranchAddress("xErrVtx", &vxerror_forest_vec);

  vector<float> *vyerror_forest_vec = nullptr;
  T_PbPbTracks->SetBranchAddress("yErrVtx", &vyerror_forest_vec);

  vector<float> *vzerror_forest_vec = nullptr;
  T_PbPbTracks->SetBranchAddress("zErrVtx", &vzerror_forest_vec);

  vector<float> *vptsum_forest_vec = nullptr;
  T_PbPbTracks->SetBranchAddress("ptSumVtx", &vptsum_forest_vec);

  // Jet parameters

  const int max = 200;
  int nref = 0;
  T_akCs3PFJetAnalyzer->SetBranchAddress("nref", &nref);

  vector<float> *JetPT_skim = nullptr;
  Tskim->SetBranchAddress("JetPT", &JetPT_skim);

  vector<float> *JetEta_skim = nullptr;
  Tskim->SetBranchAddress("JetEta", &JetEta_skim);

  vector<float> *JetPhi_skim = nullptr;
  Tskim->SetBranchAddress("JetPhi", &JetPhi_skim);

  Float_t JetPt_forest[max];
  T_akCs3PFJetAnalyzer->SetBranchAddress("jtpt", JetPt_forest);

  Float_t JetEta_forest[max];
  T_akCs3PFJetAnalyzer->SetBranchAddress("jteta", JetEta_forest);

  Float_t JetPhi_forest[max];
  T_akCs3PFJetAnalyzer->SetBranchAddress("jtphi", JetPhi_forest);

  Float_t JetPFMUF_forest[max];
  Float_t JetPFNEF_forest[max];
  Float_t JetPFNHF_forest[max];
  Float_t JetPFCEF_forest[max];
  Float_t JetPFCHF_forest[max];
  Int_t JetPFCHM_forest[max];
  T_akCs3PFJetAnalyzer->SetBranchAddress("jtPfNEF", JetPFNEF_forest);
  T_akCs3PFJetAnalyzer->SetBranchAddress("jtPfNHF", JetPFNHF_forest);
  T_akCs3PFJetAnalyzer->SetBranchAddress("jtPfCEF", JetPFCEF_forest);
  T_akCs3PFJetAnalyzer->SetBranchAddress("jtPfCHF", JetPFCHF_forest);
  T_akCs3PFJetAnalyzer->SetBranchAddress("jtPfCHM", JetPFCHM_forest);
  T_akCs3PFJetAnalyzer->SetBranchAddress("jtPfMUF", JetPFMUF_forest);

  vector<int> *nCh_skim = nullptr;
  Tskim->SetBranchAddress("MJTNcHad", &nCh_skim);
  Int_t nCh_forest[max];
  T_akCs3PFJetAnalyzer->SetBranchAddress("mjtNcHad", nCh_forest);

  vector<int> *nBh_skim = nullptr;
  Tskim->SetBranchAddress("MJTNbHad", &nBh_skim);
  Int_t nBh_forest[max];
  T_akCs3PFJetAnalyzer->SetBranchAddress("mjtNbHad", nBh_forest);

  vector<int> *nsvtx_skim = nullptr;
  Tskim->SetBranchAddress("jtNsvtx", &nsvtx_skim);
  Int_t nsvtx_forest[max];
  T_akCs3PFJetAnalyzer->SetBranchAddress("jtNsvtx", nsvtx_forest);

  vector<int> *ntrk_skim = nullptr;
  Tskim->SetBranchAddress("jtNtrk", &ntrk_skim);
  Int_t ntrk_forest[max];
  T_akCs3PFJetAnalyzer->SetBranchAddress("jtNtrk", ntrk_forest);

  // SVTX parameters

  Int_t svtxJetId_forest[max];
  Int_t svtxNtrk_forest[max];

  Float_t svtxm_forest[max];
  Float_t svtxmcorr_forest[max];
  Float_t svtxpt_forest[max];
  Float_t svtxdl_forest[max];
  Float_t svtxdls_forest[max];
  Float_t svtxdl2d_forest[max];
  Float_t svtxdls2d_forest[max];
  Float_t svtxchi2_forest[max];
  Float_t svtxnormchi2_forest[max];

  T_akCs3PFJetAnalyzer->SetBranchAddress("svtxJetId", svtxJetId_forest);
  T_akCs3PFJetAnalyzer->SetBranchAddress("svtxNtrk", svtxNtrk_forest);

  T_akCs3PFJetAnalyzer->SetBranchAddress("svtxm", svtxm_forest);
  T_akCs3PFJetAnalyzer->SetBranchAddress("svtxmcorr", svtxmcorr_forest);
  T_akCs3PFJetAnalyzer->SetBranchAddress("svtxpt", svtxpt_forest);
  T_akCs3PFJetAnalyzer->SetBranchAddress("svtxdl", svtxdl_forest);
  T_akCs3PFJetAnalyzer->SetBranchAddress("svtxdls", svtxdls_forest);
  T_akCs3PFJetAnalyzer->SetBranchAddress("svtxdl2d", svtxdl2d_forest);
  T_akCs3PFJetAnalyzer->SetBranchAddress("svtxdls2d", svtxdls2d_forest);
  T_akCs3PFJetAnalyzer->SetBranchAddress("svtxchi2", svtxchi2_forest);
  T_akCs3PFJetAnalyzer->SetBranchAddress("svtxnormchi2", svtxnormchi2_forest);

  vector<vector<int>> *svtxJetId_skim = nullptr;
  vector<vector<int>> *svtxNtrk_skim = nullptr;

  vector<vector<float>> *svtxm_skim = nullptr;
  vector<vector<float>> *svtxmcorr_skim = nullptr;
  vector<vector<float>> *svtxpt_skim = nullptr;
  vector<vector<float>> *svtxdl_skim = nullptr;
  vector<vector<float>> *svtxdls_skim = nullptr;
  vector<vector<float>> *svtxdl2d_skim = nullptr;
  vector<vector<float>> *svtxdls2d_skim = nullptr;
  vector<vector<float>> *svtxchi2_skim = nullptr;
  vector<vector<float>> *svtxnormchi2_skim = nullptr;

  Tskim->SetBranchAddress("svtxJetId", &svtxJetId_skim);
  Tskim->SetBranchAddress("svtxNtrk", &svtxNtrk_skim);

  Tskim->SetBranchAddress("svtxm", &svtxm_skim);
  Tskim->SetBranchAddress("svtxmcorr", &svtxmcorr_skim);
  Tskim->SetBranchAddress("svtxpt", &svtxpt_skim);
  Tskim->SetBranchAddress("svtxdl", &svtxdl_skim);
  Tskim->SetBranchAddress("svtxdls", &svtxdls_skim);
  Tskim->SetBranchAddress("svtxdl2d", &svtxdl2d_skim);
  Tskim->SetBranchAddress("svtxdls2d", &svtxdls2d_skim);
  Tskim->SetBranchAddress("svtxchi2", &svtxchi2_skim);
  Tskim->SetBranchAddress("svtxnormchi2", &svtxnormchi2_skim);

  // Track parameters

  Int_t trkJetId_forest[max];
  Int_t trkSvtxId_forest[max];

  Float_t trkPt_forest[max];
  Float_t trkEta_forest[max];
  Float_t trkPhi_forest[max];
  Float_t trkIp3d_forest[max];
  Float_t trkIp2d_forest[max];
  Float_t trkIp3dSig_forest[max];
  Float_t trkIp2dSig_forest[max];
  Float_t trkIpProb3d_forest[max];
  Float_t trkIpProb2d_forest[max];
  Float_t trkDistToAxis_forest[max];
  Float_t trkDistToAxisSig_forest[max];
  Float_t trkDz_forest[max];
  Int_t trkPdgId_forest[max];
  Int_t trkMatchSta_forest[max];

  T_akCs3PFJetAnalyzer->SetBranchAddress("trkJetId", trkJetId_forest);
  T_akCs3PFJetAnalyzer->SetBranchAddress("trkSvtxId", trkSvtxId_forest);
  T_akCs3PFJetAnalyzer->SetBranchAddress("trkPt", trkPt_forest);
  T_akCs3PFJetAnalyzer->SetBranchAddress("trkEta", trkEta_forest);
  T_akCs3PFJetAnalyzer->SetBranchAddress("trkPhi", trkPhi_forest);
  T_akCs3PFJetAnalyzer->SetBranchAddress("trkIp3d", trkIp3d_forest);
  T_akCs3PFJetAnalyzer->SetBranchAddress("trkIp2d", trkIp2d_forest);
  T_akCs3PFJetAnalyzer->SetBranchAddress("trkIp3dSig", trkIp3dSig_forest);
  T_akCs3PFJetAnalyzer->SetBranchAddress("trkIp2dSig", trkIp2dSig_forest);
  T_akCs3PFJetAnalyzer->SetBranchAddress("trkIpProb3d", trkIpProb3d_forest);
  T_akCs3PFJetAnalyzer->SetBranchAddress("trkIpProb2d", trkIpProb2d_forest);
  T_akCs3PFJetAnalyzer->SetBranchAddress("trkDistToAxis", trkDistToAxis_forest);
  T_akCs3PFJetAnalyzer->SetBranchAddress("trkDistToAxisSig", trkDistToAxisSig_forest);
  T_akCs3PFJetAnalyzer->SetBranchAddress("trkDz", trkDz_forest);
  T_akCs3PFJetAnalyzer->SetBranchAddress("trkPdgId", trkPdgId_forest);
  T_akCs3PFJetAnalyzer->SetBranchAddress("trkMatchSta", trkMatchSta_forest);

  vector<vector<int>> *trkSvtxId_skim = nullptr;
  vector<vector<int>> *trkJetId_skim = nullptr;
  vector<vector<float>> *trkPt_skim = nullptr;
  vector<vector<float>> *trkEta_skim = nullptr;
  vector<vector<float>> *trkPhi_skim = nullptr;
  vector<vector<float>> *trkIp3d_skim = nullptr;
  vector<vector<float>> *trkIp2d_skim = nullptr;
  vector<vector<float>> *trkIp3dSig_skim = nullptr;
  vector<vector<float>> *trkIp2dSig_skim = nullptr;
  vector<vector<float>> *trkIpProb3d_skim = nullptr;
  vector<vector<float>> *trkIpProb2d_skim = nullptr;
  vector<vector<float>> *trkDistToAxis_skim = nullptr;
  vector<vector<float>> *trkDistToAxisSig_skim = nullptr;
  vector<vector<float>> *trkDz_skim = nullptr;
  vector<vector<int>> *trkMatchSta_skim = nullptr;
  vector<vector<int>> *trkPdgId_skim = nullptr;

  Tskim->SetBranchAddress("trkSvtxId", &trkSvtxId_skim);
  Tskim->SetBranchAddress("trkJetId", &trkJetId_skim);

  Tskim->SetBranchAddress("trkPt", &trkPt_skim);
  Tskim->SetBranchAddress("trkEta", &trkEta_skim);
  Tskim->SetBranchAddress("trkPhi", &trkPhi_skim);
  Tskim->SetBranchAddress("trkIp3d", &trkIp3d_skim);
  Tskim->SetBranchAddress("trkIp2d", &trkIp2d_skim);
  Tskim->SetBranchAddress("trkIp3dSig", &trkIp3dSig_skim);
  Tskim->SetBranchAddress("trkIp2dSig", &trkIp2dSig_skim);
  Tskim->SetBranchAddress("trkIpProb3d", &trkIpProb3d_skim);
  Tskim->SetBranchAddress("trkIpProb2d", &trkIpProb2d_skim);
  Tskim->SetBranchAddress("trkDistToAxis", &trkDistToAxis_skim);
  Tskim->SetBranchAddress("trkDistToAxisSig", &trkDistToAxisSig_skim);
  Tskim->SetBranchAddress("trkDz", &trkDz_skim);
  Tskim->SetBranchAddress("trkMatchSta", &trkMatchSta_skim);
  Tskim->SetBranchAddress("trkPdgId", &trkPdgId_skim);

  // Muon entries

  vector<bool> *isMuMuTagged_skim = nullptr;
  Tskim->SetBranchAddress("IsMuMuTagged", &isMuMuTagged_skim);

  vector<float> *mupt_forest = nullptr;
  vector<float> *mueta_forest = nullptr;
  vector<float> *muphi_forest = nullptr;
  vector<int> *mucharge_forest = nullptr;
  vector<float> *mudiDxy_forest = nullptr;
  vector<float> *mudiDz_forest = nullptr;
  vector<float> *mudiDxyErr_forest = nullptr;
  vector<float> *mudiDzErr_forest = nullptr;

  T_muonAnalyzer->SetBranchAddress("recoPt", &mupt_forest);
  T_muonAnalyzer->SetBranchAddress("recoEta", &mueta_forest);
  T_muonAnalyzer->SetBranchAddress("recoPhi", &muphi_forest);
  T_muonAnalyzer->SetBranchAddress("recoCharge", &mucharge_forest);
  T_muonAnalyzer->SetBranchAddress("globalDxy", &mudiDxy_forest);
  T_muonAnalyzer->SetBranchAddress("globalDz", &mudiDz_forest);
  T_muonAnalyzer->SetBranchAddress("globalDxyErr", &mudiDxyErr_forest);
  T_muonAnalyzer->SetBranchAddress("globalDzErr", &mudiDzErr_forest);

  vector<bool> *IsTracker_forest = nullptr;
  vector<bool> *IsGlobal_forest = nullptr;
  vector<bool> *IsHybridSoft_forest = nullptr;
  vector<bool> *IsGood_forest = nullptr;

  T_muonAnalyzer->SetBranchAddress("recoIsTracker", &IsTracker_forest);
  T_muonAnalyzer->SetBranchAddress("recoIsGlobal", &IsGlobal_forest);
  T_muonAnalyzer->SetBranchAddress("recoIDHybridSoft", &IsHybridSoft_forest);
  T_muonAnalyzer->SetBranchAddress("recoIsGood", &IsGood_forest);

  vector<float> *muPt1_skim = nullptr;
  vector<float> *muPt2_skim = nullptr;
  vector<float> *muEta1_skim = nullptr;
  vector<float> *muEta2_skim = nullptr;
  vector<float> *muPhi1_skim = nullptr;
  vector<float> *muPhi2_skim = nullptr;
  vector<int> *muCharge1_skim = nullptr;
  vector<int> *muCharge2_skim = nullptr;
  vector<float> *muDiDxy1_skim = nullptr;
  vector<float> *muDiDxy2_skim = nullptr;
  vector<float> *muDiDxy1Err_skim = nullptr;
  vector<float> *muDiDxy2Err_skim = nullptr;
  vector<float> *muDiDz1_skim = nullptr;
  vector<float> *muDiDz2_skim = nullptr;
  vector<float> *muDiDz1Err_skim = nullptr;
  vector<float> *muDiDz2Err_skim = nullptr;
  vector<float> *dRJetmu1_skim = nullptr;
  vector<float> *dRJetmu2_skim = nullptr;

  Tskim->SetBranchAddress("muPt1", &muPt1_skim);
  Tskim->SetBranchAddress("muPt2", &muPt2_skim);
  Tskim->SetBranchAddress("muEta1", &muEta1_skim);
  Tskim->SetBranchAddress("muEta2", &muEta2_skim);
  Tskim->SetBranchAddress("muPhi1", &muPhi1_skim);
  Tskim->SetBranchAddress("muPhi2", &muPhi2_skim);
  Tskim->SetBranchAddress("muCharge1", &muCharge1_skim);
  Tskim->SetBranchAddress("muCharge2", &muCharge2_skim);
  Tskim->SetBranchAddress("muDiDxy1", &muDiDxy1_skim);
  Tskim->SetBranchAddress("muDiDxy2", &muDiDxy2_skim);
  Tskim->SetBranchAddress("muDiDxy1Err", &muDiDxy1Err_skim);
  Tskim->SetBranchAddress("muDiDxy2Err", &muDiDxy2Err_skim);
  Tskim->SetBranchAddress("muDiDz1", &muDiDz1_skim);
  Tskim->SetBranchAddress("muDiDz2", &muDiDz2_skim);
  Tskim->SetBranchAddress("muDiDz1Err", &muDiDz1Err_skim);
  Tskim->SetBranchAddress("muDiDz2Err", &muDiDz2Err_skim);
  Tskim->SetBranchAddress("DRJetmu1", &dRJetmu1_skim);
  Tskim->SetBranchAddress("DRJetmu2", &dRJetmu2_skim);

  vector<float> *mumumass_skim = nullptr;
  vector<float> *mumupt_skim = nullptr;
  vector<float> *mumueta_skim = nullptr;
  vector<float> *mumuphi_skim = nullptr;
  vector<float> *mumuY_skim = nullptr;
  vector<float> *mudR_skim = nullptr;

  Tskim->SetBranchAddress("mumuPt", &mumupt_skim);
  Tskim->SetBranchAddress("mumuMass", &mumumass_skim);
  Tskim->SetBranchAddress("mumuEta", &mumueta_skim);
  Tskim->SetBranchAddress("mumuPhi", &mumuphi_skim);
  Tskim->SetBranchAddress("mumuY", &mumuY_skim);
  Tskim->SetBranchAddress("muDR", &mudR_skim);

  float mumumass_forest = 0;
  float mumupt_forest = 0;
  float mumueta_forest = 0;
  float mumuphi_forest = 0;
  float mumuY_forest = 0;
  float mudR_forest = 0;

  // BEGIN

  cout << "skim entries: " << Tskim->GetEntries() << endl;
  cout << "hltanalysis entries: " << T_hltanalysis->GetEntries() << endl;
  cout << "muonAnalyzer entries: " << T_muonAnalyzer->GetEntries() << endl;
  cout << "skimanalysis entries: " << T_skimanalysis->GetEntries() << endl;
  cout << "HiForestInfo entries: " << T_HiForestInfo->GetEntries() << endl;
  cout << "PbPbTrackTree entries: " << T_PbPbTracks->GetEntries() << endl;
  cout << "hiEvtAnalyzer entries: " << T_hiEvtAnalyzer->GetEntries() << endl;
  cout << "hltMuTree entries: " << T_hltMuTree->GetEntries() << endl;
  cout << "akCs3PFJetAnalyzer entries: " << T_akCs3PFJetAnalyzer->GetEntries() << endl;
  cout << "bHadronAna entries: " << T_bHadronAna->GetEntries() << endl;
  cout << "cHadronAna entries: " << T_cHadronAna->GetEntries() << endl;

  int skim_idx = 0;
  int svtx_idx = 0;
  int trk_idx = 0;
  int nmuons = 0;
  float maxmumuPt = 0;
  int maxMu1Index = -1;
  int maxMu2Index = -1;
  bool ismumutagged = false;

  ProgressBar Bar(cout, Tskim->GetEntries());
  Bar.SetStyle(-1);

  for (int i = 0; i < Tskim->GetEntries(); i++) {

    if (i % 500 == 0) {
      Bar.Update(i);
      Bar.Print();
    }

    Tskim->GetEntry(i);
    T_hltanalysis->GetEntry(i);
    T_muonAnalyzer->GetEntry(i);
    T_skimanalysis->GetEntry(i);
    // T_HiForestInfo->GetEntry(i);
    T_PbPbTracks->GetEntry(i);
    T_hiEvtAnalyzer->GetEntry(i);
    T_hltMuTree->GetEntry(i);
    T_akCs3PFJetAnalyzer->GetEntry(i);
    T_bHadronAna->GetEntry(i);
    T_cHadronAna->GetEntry(i);

    // HiEventAnalyzer tree

    if (Event > 0) {
      if (run_skim != run_forest) {
        cout << "Run mismatch at entry " << i << ": Skim run = " << run_skim << ", Forest run = " << run_forest << endl;
      }

      if (lumi_skim != lumi_forest) {
        cout << "Lumi mismatch at entry " << i << ": Skim lumi = " << lumi_skim << ", Forest lumi = " << lumi_forest
             << endl;
      }

      if (event_skim != event_forest) {
        cout << "Event mismatch at entry " << i << ": Skim event = " << event_skim
             << ", Forest event = " << event_forest << endl;
      }

      if (hiBin_skim != hiBin_forest) {
        cout << "hiBin mismatch at entry " << i << ": Skim hiBin = " << hiBin_skim
             << ", Forest hiBin = " << hiBin_forest << endl;
      }

      if (hiHF_skim != hiHF_forest) {
        cout << "hiHF mismatch at entry " << i << ": Skim hiHF = " << hiHF_skim << ", Forest hiHF = " << hiHF_forest
             << endl;
      }

      ncollweight_forest = FindNColl(hiBin_forest);
      if (ncollweight_skim != ncollweight_forest) {
        cout << "ncollweight mismatch at entry " << i << ": Skim ncollweight = " << ncollweight_skim
             << ", Forest ncollweight = " << ncollweight_forest << endl;
      }

      if (eventweight_skim != eventweight_forest) {
        cout << "eventweight mismatch at entry " << i << ": Skim eventweight = " << eventweight_skim
             << ", Forest eventweight = " << eventweight_forest << endl;
      }

      if (pthat_skim != pthat_forest) {
        cout << "pthat mismatch at entry " << i << ": Skim pthat = " << pthat_skim
             << ", Forest pthat = " << pthat_forest << endl;
      }
    }

    // Check PbPbTracks tree

    if (Vertex > 0) {

      int BestVertex = -1;
      for (int m = 0; m < (vx_forest_vec->size()); m++) {
        if (BestVertex < 0 || vptsum_forest_vec->at(m) > vptsum_forest_vec->at(BestVertex)) {
          BestVertex = m;
        }
      }

      if (BestVertex >= 0) {
        vx_forest = vx_forest_vec->at(BestVertex);
        vy_forest = vy_forest_vec->at(BestVertex);
        vz_forest = vz_forest_vec->at(BestVertex);
        vxerror_forest = vxerror_forest_vec->at(BestVertex);
        vyerror_forest = vyerror_forest_vec->at(BestVertex);
        vzerror_forest = vzerror_forest_vec->at(BestVertex);
      }

      if (vx_skim != vx_forest) {
        cout << "vx mismatch at entry " << i << ": Skim vx = " << vx_skim << ", Forest vx = " << vx_forest << endl;
      }

      if (vy_skim != vy_forest) {
        cout << "vy mismatch at entry " << i << ": Skim vy = " << vy_skim << ", Forest vy = " << vy_forest << endl;
      }

      if (vz_skim != vz_forest) {
        cout << "vz mismatch at entry " << i << ": Skim vz = " << vz_skim << ", Forest vz = " << vz_forest << endl;
      }

      if (vxerror_skim != vxerror_forest) {
        cout << "vxerror mismatch at entry " << i << ": Skim vxerror = " << vxerror_skim
             << ", Forest vxerror = " << vxerror_forest << endl;
      }

      if (vyerror_skim != vyerror_forest) {
        cout << "vyerror mismatch at entry " << i << ": Skim vyerror = " << vyerror_skim
             << ", Forest vyerror = " << vyerror_forest << endl;
      }

      if (vzerror_skim != vzerror_forest) {
        cout << "vzerror mismatch at entry " << i << ": Skim vzerror = " << vzerror_skim
             << ", Forest vzerror = " << vzerror_forest << endl;
      }
    }

    // check Jet properties

    skim_idx = -1;
    if (nref > 0) {

      for (int j = 0; j < nref; j++) {

        // apply jet purity

        if (JetPt_forest[j] < 30) {
          continue;
        }
        if (fabs(JetEta_forest[j]) > 2) {
          continue;
        }
        bool passPurity = JetPFNHF_forest[j] < 0.90 && JetPFNEF_forest[j] < 0.90 && JetPFMUF_forest[j] < 0.80 &&
                          JetPFCHF_forest[j] > 0. && JetPFCHM_forest[j] > 0. && JetPFCEF_forest[j] < 0.80;
        if (!passPurity) {
          continue;
        }

        skim_idx += 1;

        // Jet properties

        if (Jet > 0) {
          if (skim_idx > JetPT_skim->size()) {
            cout << "Out of Bounds: wrong # of jets: skim: " << JetPT_skim->size() << " forest: " << skim_idx << endl;
            break;
          }

          if (JetPT_skim->at(skim_idx) != JetPt_forest[j]) {
            cout << "JetPT mismatch at entry " << i << ", jet " << j << ": Skim JetPT = " << JetPT_skim->at(skim_idx)
                 << ", Forest JetPt = " << JetPt_forest[j] << endl;
          }

          if (JetEta_skim->at(skim_idx) != JetEta_forest[j]) {
            cout << "JetEta mismatch at entry " << i << ", jet " << j << ": Skim JetEta = " << JetEta_skim->at(skim_idx)
                 << ", Forest JetEta = " << JetEta_forest[j] << endl;
          }

          if (JetPhi_skim->at(skim_idx) != JetPhi_forest[j]) {
            cout << "JetPhi mismatch at entry " << i << ", jet " << j << ": Skim JetPhi = " << JetPhi_skim->at(skim_idx)
                 << ", Forest JetPhi = " << JetPhi_forest[j] << endl;
          }

          if (nCh_skim->at(skim_idx) != nCh_forest[j]) {
            cout << "nCh mismatch at entry " << i << ", jet " << j << ": Skim nCh = " << nCh_skim->at(skim_idx)
                 << ", Forest nCh = " << nCh_forest[j] << endl;
          }

          if (nBh_skim->at(skim_idx) != nBh_forest[j]) {
            cout << "nBh mismatch at entry " << i << ", jet " << j << ": Skim nBh = " << nBh_skim->at(skim_idx)
                 << ", Forest nBh = " << nBh_forest[j] << endl;
          }

          if (nsvtx_skim->at(skim_idx) != nsvtx_forest[j]) {
            cout << "nsvtx mismatch at entry " << i << ", jet " << j << ": Skim nsvtx = " << nsvtx_skim->at(skim_idx)
                 << ", Forest nsvtx = " << nsvtx_forest[j] << endl;
          }

          if (ntrk_skim->at(skim_idx) != ntrk_forest[j]) {
            cout << "ntrk mismatch at entry " << i << ", jet " << j << ": Skim ntrk = " << ntrk_skim->at(skim_idx)
                 << ", Forest ntrk = " << ntrk_forest[j] << endl;
          }
        }

        // Check SVTX properties

        if (SVTX > 0) {
          svtx_idx = -1;
          for (int k = 0; k < nsvtx_forest[j]; k++) {

            if (svtxJetId_forest[k] != j) {
              continue;
            }

            svtx_idx += 1;
            // cout << svtx_idx << " " <<svtxm_forest[k] << " " << svtxm_skim->at(skim_idx).at(svtx_idx) << endl;

            if (svtxNtrk_forest[k] != svtxNtrk_skim->at(skim_idx).at(svtx_idx)) {
              cout << "svtxNtrk mismatch at entry " << i << ", jet " << j << ", svtx " << k
                   << ": Skim svtxNtrk = " << svtxNtrk_skim->at(skim_idx).at(svtx_idx)
                   << ", Forest svtxNtrk = " << svtxNtrk_forest[k] << endl;
            }

            if (svtxm_forest[k] != svtxm_skim->at(skim_idx).at(svtx_idx)) {
              cout << "svtxm mismatch at entry " << i << ", jet " << j << ", svtx " << k
                   << ": Skim svtxm = " << svtxm_skim->at(skim_idx).at(svtx_idx)
                   << ", Forest svtxm = " << svtxm_forest[k] << endl;
            }

            if (svtxmcorr_forest[k] != svtxmcorr_skim->at(skim_idx).at(svtx_idx)) {
              cout << "svtxmcorr mismatch at entry " << i << ", jet " << j << ", svtx " << k
                   << ": Skim svtxmcorr = " << svtxmcorr_skim->at(skim_idx).at(svtx_idx)
                   << ", Forest svtxmcorr = " << svtxmcorr_forest[k] << endl;
            }

            if (svtxpt_forest[k] != svtxpt_skim->at(skim_idx).at(svtx_idx)) {
              cout << "svtxpt mismatch at entry " << i << ", jet " << j << ", svtx " << k
                   << ": Skim svtxpt = " << svtxpt_skim->at(skim_idx).at(svtx_idx)
                   << ", Forest svtxpt = " << svtxpt_forest[k] << endl;
            }

            if (svtxdl_forest[k] != svtxdl_skim->at(skim_idx).at(svtx_idx)) {
              cout << "svtxdl mismatch at entry " << i << ", jet " << j << ", svtx " << k
                   << ": Skim svtxdl = " << svtxdl_skim->at(skim_idx).at(svtx_idx)
                   << ", Forest svtxdl = " << svtxdl_forest[k] << endl;
            }

            if (svtxdls_forest[k] != svtxdls_skim->at(skim_idx).at(svtx_idx)) {
              cout << "svtxdls mismatch at entry " << i << ", jet " << j << ", svtx " << k
                   << ": Skim svtxdls = " << svtxdls_skim->at(skim_idx).at(svtx_idx)
                   << ", Forest svtxdls = " << svtxdls_forest[k] << endl;
            }

            if (svtxdl2d_forest[k] != svtxdl2d_skim->at(skim_idx).at(svtx_idx)) {
              cout << "svtxdl2d mismatch at entry " << i << ", jet " << j << ", svtx " << k
                   << ": Skim svtxdl2d = " << svtxdl2d_skim->at(skim_idx).at(svtx_idx)
                   << ", Forest svtxdl2d = " << svtxdl2d_forest[k] << endl;
            }

            if (svtxdls2d_forest[k] != svtxdls2d_skim->at(skim_idx).at(svtx_idx)) {
              cout << "svtxdls2d mismatch at entry " << i << ", jet " << j << ", svtx " << k
                   << ": Skim svtxdls2d = " << svtxdls2d_skim->at(skim_idx).at(svtx_idx)
                   << ", Forest svtxdls2d = " << svtxdls2d_forest[k] << endl;
            }

            if (svtxchi2_forest[k] != svtxchi2_skim->at(skim_idx).at(svtx_idx)) {
              cout << "svtxchi2 mismatch at entry " << i << ", jet " << j << ", svtx " << k
                   << ": Skim svtxchi2 = " << svtxchi2_skim->at(skim_idx).at(svtx_idx)
                   << ", Forest svtxchi2 = " << svtxchi2_forest[k] << endl;
            }

            if (svtxnormchi2_forest[k] != svtxnormchi2_skim->at(skim_idx).at(svtx_idx)) {
              cout << "svtxnormchi2 mismatch at entry " << i << ", jet " << j << ", svtx " << k
                   << ": Skim svtxnormchi2 = " << svtxnormchi2_skim->at(skim_idx).at(svtx_idx)
                   << ", Forest svtxnormchi2 = " << svtxnormchi2_forest[k] << endl;
            }
          }
        }

        // Check Track properties

        if (Track > 0) {
          trk_idx = -1;
          for (int l = 0; l < ntrk_forest[j]; l++) {

            if (trkJetId_forest[l] != j) {
              continue;
            }
            trk_idx += 1;

            if (trkSvtxId_skim->at(skim_idx).at(trk_idx) != trkSvtxId_forest[l]) {
              cout << "trkSvtxId mismatch at entry " << i << ", jet " << j << ", trk " << l
                   << ": Skim trkSvtxId = " << trkSvtxId_skim->at(skim_idx).at(trk_idx)
                   << ", Forest trkSvtxId = " << trkSvtxId_forest[l] << endl;
            }
            if (trkJetId_skim->at(skim_idx).at(trk_idx) != trkJetId_forest[l]) {
              cout << "trkJetId mismatch at entry " << i << ", jet " << j << ", trk " << l
                   << ": Skim trkJetId = " << trkJetId_skim->at(skim_idx).at(trk_idx)
                   << ", Forest trkJetId = " << trkJetId_forest[l] << endl;
            }

            if (trkPt_skim->at(skim_idx).at(trk_idx) != trkPt_forest[l]) {
              cout << "trkPt mismatch at entry " << i << ", jet " << j << ", trk " << l
                   << ": Skim trkPt = " << trkPt_skim->at(skim_idx).at(trk_idx)
                   << ", Forest trkPt = " << trkPt_forest[l] << endl;
            }

            if (trkEta_skim->at(skim_idx).at(trk_idx) != trkEta_forest[l]) {
              cout << "trkEta mismatch at entry " << i << ", jet " << j << ", trk " << l
                   << ": Skim trkEta = " << trkEta_skim->at(skim_idx).at(trk_idx)
                   << ", Forest trkEta = " << trkEta_forest[l] << endl;
            }

            if (trkPhi_skim->at(skim_idx).at(trk_idx) != trkPhi_forest[l]) {
              cout << "trkPhi mismatch at entry " << i << ", jet " << j << ", trk " << l
                   << ": Skim trkPhi = " << trkPhi_skim->at(skim_idx).at(trk_idx)
                   << ", Forest trkPhi = " << trkPhi_forest[l] << endl;
            }

            if (trkIp3d_skim->at(skim_idx).at(trk_idx) != trkIp3d_forest[l]) {
              cout << "trkIp3d mismatch at entry " << i << ", jet " << j << ", trk " << l
                   << ": Skim trkIp3d = " << trkIp3d_skim->at(skim_idx).at(trk_idx)
                   << ", Forest trkIp3d = " << trkIp3d_forest[l] << endl;
            }

            if (trkIp2d_skim->at(skim_idx).at(trk_idx) != trkIp2d_forest[l]) {
              cout << "trkIp2d mismatch at entry " << i << ", jet " << j << ", trk " << l
                   << ": Skim trkIp2d = " << trkIp2d_skim->at(skim_idx).at(trk_idx)
                   << ", Forest trkIp2d = " << trkIp2d_forest[l] << endl;
            }

            if ((trkIp3dSig_skim->at(skim_idx).at(trk_idx) != trkIp3dSig_forest[l]) &&
                (trkIp3dSig_forest[l] == trkIp3dSig_forest[l])) {
              cout << "trkIp3dSig mismatch at entry " << i << ", jet " << j << ", trk " << l
                   << ": Skim trkIp3dSig = " << trkIp3dSig_skim->at(skim_idx).at(trk_idx)
                   << ", Forest trkIp3dSig = " << trkIp3dSig_forest[l] << endl;
            }

            if (trkIp2dSig_skim->at(skim_idx).at(trk_idx) != trkIp2dSig_forest[l]) {
              cout << "trkIp2dSig mismatch at entry " << i << ", jet " << j << ", trk " << l
                   << ": Skim trkIp2dSig = " << trkIp2dSig_skim->at(skim_idx).at(trk_idx)
                   << ", Forest trkIp2dSig = " << trkIp2dSig_forest[l] << endl;
            }

            if ((trkIpProb3d_skim->at(skim_idx).at(trk_idx) != trkIpProb3d_forest[l]) &&
                (trkIpProb3d_forest[l] == trkIpProb3d_forest[l])) {
              cout << "trkIpProb3d mismatch at entry " << i << ", jet " << j << ", trk " << l
                   << ": Skim trkIpProb3d = " << trkIpProb3d_skim->at(skim_idx).at(trk_idx)
                   << ", Forest trkIpProb3d = " << trkIpProb3d_forest[l] << endl;
            }

            if ((trkIpProb2d_skim->at(skim_idx).at(trk_idx) != trkIpProb2d_forest[l]) &&
                (trkIpProb2d_forest[l] == trkIpProb2d_forest[l])) {
              cout << "trkIpProb2d mismatch at entry " << i << ", jet " << j << ", trk " << l
                   << ": Skim trkIpProb2d = " << trkIpProb2d_skim->at(skim_idx).at(trk_idx)
                   << ", Forest trkIpProb2d = " << trkIpProb2d_forest[l] << endl;
            }

            if (trkDistToAxis_skim->at(skim_idx).at(trk_idx) != trkDistToAxis_forest[l]) {
              cout << "trkDistToAxis mismatch at entry " << i << ", jet " << j << ", trk " << l
                   << ": Skim trkDistToAxis = " << trkDistToAxis_skim->at(skim_idx).at(trk_idx)
                   << ", Forest trkDistToAxis = " << trkDistToAxis_forest[l] << endl;
            }

            if (trkDistToAxisSig_skim->at(skim_idx).at(trk_idx) != trkDistToAxisSig_forest[l]) {
              cout << "trkDistToAxisSig mismatch at entry " << i << ", jet " << j << ", trk " << l
                   << ": Skim trkDistToAxisSig = " << trkDistToAxisSig_skim->at(skim_idx).at(trk_idx)
                   << ", Forest trkDistToAxisSig = " << trkDistToAxisSig_forest[l] << endl;
            }

            if (trkDz_skim->at(skim_idx).at(trk_idx) != trkDz_forest[l]) {
              cout << "trkDz mismatch at entry " << i << ", jet " << j << ", trk " << l
                   << ": Skim trkDz = " << trkDz_skim->at(skim_idx).at(trk_idx)
                   << ", Forest trkDz = " << trkDz_forest[l] << endl;
            }

            if (trkMatchSta_skim->at(skim_idx).at(trk_idx) != trkMatchSta_forest[l]) {
              cout << "trkMatchSta mismatch at entry " << i << ", jet " << j << ", trk " << l
                   << ": Skim trkMatchSta = " << trkMatchSta_skim->at(skim_idx).at(trk_idx)
                   << ", Forest trkMatchSta = " << trkMatchSta_forest[l] << endl;
            }

            if (trkPdgId_skim->at(skim_idx).at(trk_idx) != trkPdgId_forest[l]) {
              cout << "trkPdgId mismatch at entry " << i << ", jet " << j << ", trk " << l
                   << ": Skim trkPdgId = " << trkPdgId_skim->at(skim_idx).at(trk_idx)
                   << ", Forest trkPdgId = " << trkPdgId_forest[l] << endl;
            }
          }
        }

        // Check muon properties

        if (Muon > 0) {
          maxmumuPt = 0;
          maxMu1Index = -1;
          maxMu2Index = -1;
          ismumutagged = false;
          nmuons = mupt_forest->size();
          for (int mu1idx = 0; mu1idx < nmuons; mu1idx++) {
            if (!isMuonSelected(mupt_forest, mueta_forest, IsTracker_forest, IsGlobal_forest, IsHybridSoft_forest,
                                IsGood_forest, mu1idx)) {
              continue;
            }
            for (int mu2idx = mu1idx + 1; mu2idx < nmuons; mu2idx++) {
              if (!isMuonSelected(mupt_forest, mueta_forest, IsTracker_forest, IsGlobal_forest, IsHybridSoft_forest,
                                  IsGood_forest, mu2idx)) {
                continue;
              }

              float mupt1 = mupt_forest->at(mu1idx);
              float mueta1 = mueta_forest->at(mu1idx);
              float muphi1 = muphi_forest->at(mu1idx);
              float mupt2 = mupt_forest->at(mu2idx);
              float mueta2 = mueta_forest->at(mu2idx);
              float muphi2 = muphi_forest->at(mu2idx);
              float drjetmu1 = sqrt(DeltaPhi(JetPhi_forest[j], muphi1) * DeltaPhi(JetPhi_forest[j], muphi1) +
                                    (mueta1 - JetEta_forest[j]) * (mueta1 - JetEta_forest[j]));
              float drjetmu2 = sqrt(DeltaPhi(JetPhi_forest[j], muphi2) * DeltaPhi(JetPhi_forest[j], muphi2) +
                                    (mueta2 - JetEta_forest[j]) * (mueta2 - JetEta_forest[j]));

              if (drjetmu1 > 0.3 || drjetmu2 > 0.3) {
                continue;
              }
              TLorentzVector Mu1, Mu2;
              Mu1.SetPtEtaPhiM(mupt1, mueta1, muphi1, M_MU);
              Mu2.SetPtEtaPhiM(mupt2, mueta2, muphi2, M_MU);
              TLorentzVector MuMu = Mu1 + Mu2;

              if (MuMu.Pt() > maxmumuPt) {
                maxmumuPt = MuMu.Pt();
                maxMu1Index = mu1idx;
                maxMu2Index = mu2idx;
              }
            }
          }

          if (maxmumuPt > 0. && maxMu1Index >= 0 && maxMu2Index >= 0) {
            ismumutagged = true;
          }

          if (isMuMuTagged_skim->at(skim_idx) != ismumutagged) {
            cout << "isMuMuTagged mismatch at entry " << i << ", jet " << j
                 << ": Skim isMuMuTagged = " << isMuMuTagged_skim->at(skim_idx)
                 << ", Forest isMuMuTagged = " << ismumutagged << endl;
          }

          if (ismumutagged) {

            if (mupt_forest->at(maxMu1Index) != muPt1_skim->at(skim_idx)) {
              cout << "muPt1 mismatch at entry " << i << ", jet " << j << ": Skim muPt1 = " << muPt1_skim->at(skim_idx)
                   << ", Forest muPt1 = " << mupt_forest->at(maxMu1Index) << endl;
            }
            if (mupt_forest->at(maxMu2Index) != muPt2_skim->at(skim_idx)) {
              cout << "muPt2 mismatch at entry " << i << ", jet " << j << ": Skim muPt2 = " << muPt2_skim->at(skim_idx)
                   << ", Forest muPt2 = " << mupt_forest->at(maxMu2Index) << endl;
            }

            if (mueta_forest->at(maxMu1Index) != muEta1_skim->at(skim_idx)) {
              cout << "muEta1 mismatch at entry " << i << ", jet " << j
                   << ": Skim muEta1 = " << muEta1_skim->at(skim_idx)
                   << ", Forest muEta1 = " << mueta_forest->at(maxMu1Index) << endl;
            }
            if (mueta_forest->at(maxMu2Index) != muEta2_skim->at(skim_idx)) {
              cout << "muEta2 mismatch at entry " << i << ", jet " << j
                   << ": Skim muEta2 = " << muEta2_skim->at(skim_idx)
                   << ", Forest muEta2 = " << mueta_forest->at(maxMu2Index) << endl;
            }
            if (muphi_forest->at(maxMu1Index) != muPhi1_skim->at(skim_idx)) {
              cout << "muPhi1 mismatch at entry " << i << ", jet " << j
                   << ": Skim muPhi1 = " << muPhi1_skim->at(skim_idx)
                   << ", Forest muPhi1 = " << muphi_forest->at(maxMu1Index) << endl;
            }
            if (muphi_forest->at(maxMu2Index) != muPhi2_skim->at(skim_idx)) {
              cout << "muPhi2 mismatch at entry " << i << ", jet " << j
                   << ": Skim muPhi2 = " << muPhi2_skim->at(skim_idx)
                   << ", Forest muPhi2 = " << muphi_forest->at(maxMu2Index) << endl;
            }

            if (mucharge_forest->at(maxMu1Index) != muCharge1_skim->at(skim_idx)) {
              cout << "muCharge1 mismatch at entry " << i << ", jet " << j
                   << ": Skim muCharge1 = " << muCharge1_skim->at(skim_idx)
                   << ", Forest muCharge1 = " << mucharge_forest->at(maxMu1Index) << endl;
            }

            if (mucharge_forest->at(maxMu2Index) != muCharge2_skim->at(skim_idx)) {
              cout << "muCharge2 mismatch at entry " << i << ", jet " << j
                   << ": Skim muCharge2 = " << muCharge2_skim->at(skim_idx)
                   << ", Forest muCharge2 = " << mucharge_forest->at(maxMu2Index) << endl;
            }

            if (mudiDxy_forest->at(maxMu1Index) != muDiDxy1_skim->at(skim_idx)) {
              cout << "muDiDxy1 mismatch at entry " << i << ", jet " << j
                   << ": Skim muDiDxy1 = " << muDiDxy1_skim->at(skim_idx)
                   << ", Forest muDiDxy1 = " << mudiDxy_forest->at(maxMu1Index) << endl;
            }

            if (mudiDxy_forest->at(maxMu2Index) != muDiDxy2_skim->at(skim_idx)) {
              cout << "muDiDxy2 mismatch at entry " << i << ", jet " << j
                   << ": Skim muDiDxy2 = " << muDiDxy2_skim->at(skim_idx)
                   << ", Forest muDiDxy2 = " << mudiDxy_forest->at(maxMu2Index) << endl;
            }

            if (mudiDz_forest->at(maxMu1Index) - muDiDz1_skim->at(skim_idx)) {
              cout << "muDiDz1 mismatch at entry " << i << ", jet " << j
                   << ": Skim muDiDz1 = " << muDiDz1_skim->at(skim_idx)
                   << ", Forest muDiDz1 = " << mudiDz_forest->at(maxMu1Index) << endl;
            }

            if (mudiDz_forest->at(maxMu2Index) != muDiDz2_skim->at(skim_idx)) {
              cout << "muDiDz2 mismatch at entry " << i << ", jet " << j
                   << ": Skim muDiDz2 = " << muDiDz2_skim->at(skim_idx)
                   << ", Forest muDiDz2 = " << mudiDz_forest->at(maxMu2Index) << endl;
            }

            if (mudiDxyErr_forest->at(maxMu1Index) != muDiDxy1Err_skim->at(skim_idx)) {
              cout << "muDiDxy1Err mismatch at entry " << i << ", jet " << j
                   << ": Skim muDiDxy1Err = " << muDiDxy1Err_skim->at(skim_idx)
                   << ", Forest muDiDxy1Err = " << mudiDxyErr_forest->at(maxMu1Index) << endl;
            }

            if (mudiDxyErr_forest->at(maxMu2Index) != muDiDxy2Err_skim->at(skim_idx)) {
              cout << "muDiDxy2Err mismatch at entry " << i << ", jet " << j
                   << ": Skim muDiDxy2Err = " << muDiDxy2Err_skim->at(skim_idx)
                   << ", Forest muDiDxy2Err = " << mudiDxyErr_forest->at(maxMu2Index) << endl;
            }

            if (mudiDzErr_forest->at(maxMu1Index) != muDiDz1Err_skim->at(skim_idx)) {
              cout << "muDiDz1Err mismatch at entry " << i << ", jet " << j
                   << ": Skim muDiDz1Err = " << muDiDz1Err_skim->at(skim_idx)
                   << ", Forest muDiDz1Err = " << mudiDzErr_forest->at(maxMu1Index) << endl;
            }

            if (mudiDzErr_forest->at(maxMu2Index) != muDiDz2Err_skim->at(skim_idx)) {
              cout << "muDiDz2Err mismatch at entry " << i << ", jet " << j
                   << ": Skim muDiDz2Err = " << muDiDz2Err_skim->at(skim_idx)
                   << ", Forest muDiDz2Err = " << mudiDzErr_forest->at(maxMu2Index) << endl;
            }

            float drjetmu1_forest = sqrt(DeltaPhi(JetPhi_forest[j], muphi_forest->at(maxMu1Index)) *
                                             DeltaPhi(JetPhi_forest[j], muphi_forest->at(maxMu1Index)) +
                                         (mueta_forest->at(maxMu1Index) - JetEta_forest[j]) *
                                             (mueta_forest->at(maxMu1Index) - JetEta_forest[j]));
            float drjetmu2_forest = sqrt(DeltaPhi(JetPhi_forest[j], muphi_forest->at(maxMu2Index)) *
                                             DeltaPhi(JetPhi_forest[j], muphi_forest->at(maxMu2Index)) +
                                         (mueta_forest->at(maxMu2Index) - JetEta_forest[j]) *
                                             (mueta_forest->at(maxMu2Index) - JetEta_forest[j]));

            if (fabs(drjetmu1_forest - dRJetmu1_skim->at(skim_idx)) > 1.0e-6) {
              cout << "drjetmu1 mismatch at entry " << i << ", jet " << j
                   << ": Skim drjetmu1 = " << dRJetmu1_skim->at(skim_idx) << ", Forest drjetmu1 = " << drjetmu1_forest
                   << endl;
            }

            if (fabs(drjetmu2_forest - dRJetmu2_skim->at(skim_idx)) > 1.0e-6) {
              cout << "drjetmu2 mismatch at entry " << i << ", jet " << j
                   << ": Skim drjetmu2 = " << dRJetmu2_skim->at(skim_idx) << ", Forest drjetmu2 = " << drjetmu2_forest
                   << endl;
            }

            TLorentzVector gMu1, gMu2;
            gMu1.SetPtEtaPhiM(mupt_forest->at(maxMu1Index), mueta_forest->at(maxMu1Index),
                              muphi_forest->at(maxMu1Index), M_MU);
            gMu2.SetPtEtaPhiM(mupt_forest->at(maxMu2Index), mueta_forest->at(maxMu2Index),
                              muphi_forest->at(maxMu2Index), M_MU);
            TLorentzVector gMuMu = gMu1 + gMu2;
            mumumass_forest = gMuMu.M();
            mumupt_forest = gMuMu.Pt();
            mumueta_forest = gMuMu.Eta();
            mumuphi_forest = gMuMu.Phi();
            mumuY_forest = gMuMu.Rapidity();
            mudR_forest = sqrt(DeltaPhi(muphi_forest->at(maxMu2Index), muphi_forest->at(maxMu1Index)) *
                                   DeltaPhi(muphi_forest->at(maxMu2Index), muphi_forest->at(maxMu1Index)) +
                               (mueta_forest->at(maxMu1Index) - mueta_forest->at(maxMu2Index)) *
                                   (mueta_forest->at(maxMu1Index) - mueta_forest->at(maxMu2Index)));

            if (mumumass_forest != mumumass_skim->at(skim_idx)) {
              cout << "mumuMass mismatch at entry " << i << ", jet " << j
                   << ": Skim mumuMass = " << mumumass_skim->at(skim_idx) << ", Forest mumuMass = " << mumumass_forest
                   << endl;
            }

            if (mumupt_forest != mumupt_skim->at(skim_idx)) {
              cout << "mumuPt mismatch at entry " << i << ", jet " << j
                   << ": Skim mumuPt = " << mumupt_skim->at(skim_idx) << ", Forest mumuPt = " << mumupt_forest << endl;
            }

            if (mumueta_forest != mumueta_skim->at(skim_idx)) {
              cout << "mumuEta mismatch at entry " << i << ", jet " << j
                   << ": Skim mumuEta = " << mumueta_skim->at(skim_idx) << ", Forest mumuEta = " << mumueta_forest
                   << endl;
            }

            if (mumuphi_forest != mumuphi_skim->at(skim_idx)) {
              cout << "mumuPhi mismatch at entry " << i << ", jet " << j
                   << ": Skim mumuPhi = " << mumuphi_skim->at(skim_idx) << ", Forest mumuPhi = " << mumuphi_forest
                   << endl;
            }

            if (mumuY_forest != mumuY_skim->at(skim_idx)) {
              cout << "mumuY mismatch at entry " << i << ", jet " << j << ": Skim mumuY = " << mumuY_skim->at(skim_idx)
                   << ", Forest mumuY = " << mumuY_forest << endl;
            }

            if (fabs(mudR_forest - mudR_skim->at(skim_idx)) > 1.0e-6) {
              cout << "mudR mismatch at entry " << i << ", jet " << j << ": Skim mudR = " << mudR_skim->at(skim_idx)
                   << ", Forest mudR = " << mudR_forest << endl;

              cout << fabs(mudR_forest - mudR_skim->at(skim_idx)) << endl;
            }
          }
        }
      }
    }
  }
}

void skimvalidator() {

  validate(1, // Event properties (HiEvtAnalyzer)
           1, // Vertex properties (PbPbTracks)
           1, // Jet properties (akCs3PFJetAnalyzer)
           1, // Secondary vertex properties (akCs3PFJetAnalyzer)
           1, // Track properties (akCs3PFJetAnalyzer)
           1  // Muon properties (muonAnalyzer)
  );
}
