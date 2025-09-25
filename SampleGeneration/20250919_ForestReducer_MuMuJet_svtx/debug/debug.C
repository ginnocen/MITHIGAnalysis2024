#include <vector>
#include "TInterpreter.h"
#include "../VectorDict.h"

using namespace std;

// Generate dictionary before the function
#ifdef __CLING__
#pragma cling load("libTree")
#pragma cling load("libTreePlayer")
#endif

bool notbothnan(float a, float b){
    if (std::isnan(a) && std::isnan(b)) {
        return false;
    }
    return true;
}

void debug(){
    // Force dictionary generation explicitly at runtime
    gROOT->ProcessLine("#include <vector>");
    gInterpreter->GenerateDictionary("vector<vector<float>>", "vector");
    gInterpreter->GenerateDictionary("vector<vector<int>>", "vector");
    
    cout << "Dictionary generation attempted" << endl;




    //TFile* forest = TFile::Open("/data00/g2ccbar/mc2018/forest_lowerJetPt_may232025");
    TFile* skim1 = TFile::Open("/data00/g2ccbar/mc2018/skim_09252025/mergedfile.root");
    TFile* skim2 = TFile::Open("/data00/g2ccbar/mc2018/skim_09222025/mergedfile.root");

    TTree* T1 = (TTree*)skim1->Get("Tree");
    TTree* T2 = (TTree*)skim2->Get("Tree");
    TTree* T2G = (TTree*)skim2->Get("GenTree");

    Long64_t event1 = 0;
    int run1 = 0;
    int lumi1 = 0;
    int hiBin1 = 0;
    float hiHF1 = 0;
    int NVertex1 = 0;
    float VX1 = 0, VY1 = 0, VZ1 = 0;
    float VXError1 = 0, VYError1 = 0, VZError1 = 0;
    float NCollWeight1 = 0;
    float EventWeight1 = 0;
    float PTHat1 = 0;
    int NPU1 = 0;
    vector<float> *JetPT_1 = nullptr;
    vector<float> *JetEta_1 = nullptr;
    vector<float> *JetPhi_1 = nullptr;
    vector<bool> *isMuMuTagged_1 = nullptr;
    vector<float> *muPt1_1 = nullptr;
    vector<float> *muPt2_1 = nullptr;
    vector<float> *muEta1_1 = nullptr;
    vector<float> *muEta2_1 = nullptr;
    vector<float> *muPhi1_1 = nullptr;
    vector<float> *muPhi2_1 = nullptr;
    vector<int> *muCharge1_1 = nullptr;
    vector<int> *muCharge2_1 = nullptr;
    vector<float> *muDiDxy1_1 = nullptr;
    vector<float> *muDiDxy1Err_1 = nullptr;
    vector<float> *muDiDxy2_1 = nullptr;
    vector<float> *muDiDxy2Err_1 = nullptr;
    vector<float> *muDiDz1_1 = nullptr;
    vector<float> *muDiDz1Err_1 = nullptr;
    vector<float> *muDiDz2_1 = nullptr;
    vector<float> *muDiDz2Err_1 = nullptr;
    vector<float> *muDiDxy1Dxy2_1 = nullptr;
    vector<float> *muDiDxy1Dxy2Err_1 = nullptr;
    vector<float> *mumuMass_1 = nullptr;
    vector<float> *mumuEta_1 = nullptr;
    vector<float> *mumuY_1 = nullptr;
    vector<float> *mumuPhi_1 = nullptr;
    vector<float> *mumuPt_1 = nullptr;
    vector<bool> *mumuIsGenMatched_1 = nullptr;
    vector<float> *DRJetmu1_1 = nullptr;
    vector<float> *DRJetmu2_1 = nullptr;
    vector<float> *muDeta_1 = nullptr;
    vector<float> *muDphi_1 = nullptr;
    vector<float> *muDR_1 = nullptr;
    vector<vector<float>> *ExtraMuWeight_1 = nullptr;
    vector<float> *MuMuWeight_1 = nullptr;


    vector<int> *MJTHadronFlavor_1 = nullptr;
    vector<int> *MJTNcHad_1 = nullptr;
    vector<int> *MJTNbHad_1 = nullptr;
    vector<int> *jtNsvtx_1 = nullptr;
    vector<int> *jtNtrk_1 = nullptr;
    vector<float> *jtptCh_1 = nullptr;
    int nsvtx_1 = 0;
    vector<vector<int>> *svtxJetId_1 = nullptr;
    vector<vector<int>> *svtxNtrk_1 = nullptr;
    vector<vector<float>> *svtxdl_1 = nullptr;
    vector<vector<float>> *svtxdls_1 = nullptr;
    vector<vector<float>> *svtxdl2d_1 = nullptr;
    vector<vector<float>> *svtxdls2d_1 = nullptr;
    vector<vector<float>> *svtxm_1 = nullptr;
    vector<vector<float>> *svtxmcorr_1 = nullptr;
    vector<vector<float>> *svtxnormchi2_1 = nullptr;
    vector<vector<float>> *svtxchi2_1 = nullptr;

    vector<int> *svtxIdx_mu1_1 = nullptr;
    vector<int> *svtxIdx_mu2_1 = nullptr;
    vector<int> *trkIdx_mu1_1 = nullptr;
    vector<int> *trkIdx_mu2_1 = nullptr;

    int ntrk_1 = 0;
    vector<vector<int>> *trkJetId_1 = nullptr;
    vector<vector<int>> *trkSvtxId_1 = nullptr;
    vector<vector<float>> *trkPt_1 = nullptr;
    vector<vector<float>> *trkEta_1 = nullptr;
    vector<vector<float>> *trkPhi_1 = nullptr;
    vector<vector<float>> *trkIp3d_1 = nullptr;
    vector<vector<float>> *trkIp3dSig_1 = nullptr;
    vector<vector<float>> *trkIp2d_1 = nullptr;
    vector<vector<float>> *trkIp2dSig_1 = nullptr;
    vector<vector<float>> *trkDistToAxis_1 = nullptr;
    vector<vector<float>> *trkDistToAxisSig_1 = nullptr;
    vector<vector<float>> *trkIpProb2d_1 = nullptr;
    vector<vector<float>> *trkIpProb3d_1 = nullptr;
    vector<vector<float>> *trkDz_1 = nullptr;
    vector<vector<int>> *trkPdgId_1 = nullptr;
    vector<vector<int>> *trkMatchSta_1 = nullptr;



    T1->SetBranchAddress("Event", &event1);
    T1->SetBranchAddress("Run", &run1);
    T1->SetBranchAddress("Lumi", &lumi1);
    T1->SetBranchAddress("hiBin", &hiBin1);
    T1->SetBranchAddress("hiHF", &hiHF1);
    T1->SetBranchAddress("NVertex", &NVertex1);
    T1->SetBranchAddress("VX", &VX1);
    T1->SetBranchAddress("VY", &VY1);
    T1->SetBranchAddress("VZ", &VZ1);
    T1->SetBranchAddress("VXError", &VXError1);
    T1->SetBranchAddress("VYError", &VYError1);
    T1->SetBranchAddress("VZError", &VZError1);
    T1->SetBranchAddress("NCollWeight", &NCollWeight1);
    T1->SetBranchAddress("EventWeight", &EventWeight1);
    T1->SetBranchAddress("PTHat", &PTHat1);
    T1->SetBranchAddress("NPU", &NPU1);
    T1->SetBranchAddress("JetPT", &JetPT_1);
    T1->SetBranchAddress("JetEta", &JetEta_1);
    T1->SetBranchAddress("JetPhi", &JetPhi_1);
    T1->SetBranchAddress("IsMuMuTagged", &isMuMuTagged_1);
    T1->SetBranchAddress("muPt1", &muPt1_1);
    T1->SetBranchAddress("muPt2", &muPt2_1);
    T1->SetBranchAddress("muEta1", &muEta1_1);
    T1->SetBranchAddress("muEta2", &muEta2_1);
    T1->SetBranchAddress("muPhi1", &muPhi1_1);
    T1->SetBranchAddress("muPhi2", &muPhi2_1);
    T1->SetBranchAddress("muCharge1", &muCharge1_1);
    T1->SetBranchAddress("muCharge2", &muCharge2_1);
    T1->SetBranchAddress("muDiDxy1", &muDiDxy1_1);
    T1->SetBranchAddress("muDiDxy1Err", &muDiDxy1Err_1);
    T1->SetBranchAddress("muDiDxy2", &muDiDxy2_1);
    T1->SetBranchAddress("muDiDxy2Err", &muDiDxy2Err_1);
    T1->SetBranchAddress("muDiDz1", &muDiDz1_1);
    T1->SetBranchAddress("muDiDz1Err", &muDiDz1Err_1);
    T1->SetBranchAddress("muDiDz2", &muDiDz2_1);
    T1->SetBranchAddress("muDiDz2Err", &muDiDz2Err_1);
    T1->SetBranchAddress("muDiDxy1Dxy2", &muDiDxy1Dxy2_1);
    T1->SetBranchAddress("muDiDxy1Dxy2Err", &muDiDxy1Dxy2Err_1);
    T1->SetBranchAddress("mumuMass", &mumuMass_1);
    T1->SetBranchAddress("mumuEta", &mumuEta_1);
    T1->SetBranchAddress("mumuY", &mumuY_1);
    T1->SetBranchAddress("mumuPhi", &mumuPhi_1);
    T1->SetBranchAddress("mumuPt", &mumuPt_1);
    T1->SetBranchAddress("mumuIsGenMatched", &mumuIsGenMatched_1);
    T1->SetBranchAddress("DRJetmu1", &DRJetmu1_1);
    T1->SetBranchAddress("DRJetmu2", &DRJetmu2_1);
    T1->SetBranchAddress("muDeta", &muDeta_1);
    T1->SetBranchAddress("muDphi", &muDphi_1);
    T1->SetBranchAddress("muDR", &muDR_1);
    T1->SetBranchAddress("ExtraMuWeight", &ExtraMuWeight_1);
    T1->SetBranchAddress("MuMuWeight", &MuMuWeight_1);

    T1->SetBranchAddress("MJTHadronFlavor", &MJTHadronFlavor_1);
    T1->SetBranchAddress("MJTNcHad", &MJTNcHad_1);
    T1->SetBranchAddress("MJTNbHad", &MJTNbHad_1);
    T1->SetBranchAddress("jtNsvtx", &jtNsvtx_1);
    T1->SetBranchAddress("jtNtrk", &jtNtrk_1);
    T1->SetBranchAddress("jtptCh", &jtptCh_1);
    //T1->SetBranchAddress("nsvtx", &nsvtx_1);
    T1->SetBranchAddress("svtxJetId", &svtxJetId_1);
    T1->SetBranchAddress("svtxNtrk", &svtxNtrk_1);
    T1->SetBranchAddress("svtxdl", &svtxdl_1);
    T1->SetBranchAddress("svtxdls", &svtxdls_1);
    T1->SetBranchAddress("svtxdl2d", &svtxdl2d_1);
    T1->SetBranchAddress("svtxdls2d", &svtxdls2d_1);
    T1->SetBranchAddress("svtxm", &svtxm_1);
    T1->SetBranchAddress("svtxmcorr", &svtxmcorr_1);
    T1->SetBranchAddress("svtxnormchi2", &svtxnormchi2_1);
    T1->SetBranchAddress("svtxchi2", &svtxchi2_1);
    T1->SetBranchAddress("svtxIdx_mu1", &svtxIdx_mu1_1);
    T1->SetBranchAddress("svtxIdx_mu2", &svtxIdx_mu2_1);
    T1->SetBranchAddress("trkIdx_mu1", &trkIdx_mu1_1);
    T1->SetBranchAddress("trkIdx_mu2", &trkIdx_mu2_1);
    //T1->SetBranchAddress("ntrk", &ntrk_1);  
    T1->SetBranchAddress("trkJetId", &trkJetId_1);
    T1->SetBranchAddress("trkSvtxId", &trkSvtxId_1);
    T1->SetBranchAddress("trkPt", &trkPt_1);
    T1->SetBranchAddress("trkEta", &trkEta_1);
    T1->SetBranchAddress("trkPhi", &trkPhi_1);
    T1->SetBranchAddress("trkIp3d", &trkIp3d_1);
    T1->SetBranchAddress("trkIp3dSig", &trkIp3dSig_1);
    T1->SetBranchAddress("trkIp2d", &trkIp2d_1);
    T1->SetBranchAddress("trkIp2dSig", &trkIp2dSig_1);
    T1->SetBranchAddress("trkDistToAxis", &trkDistToAxis_1);
    T1->SetBranchAddress("trkDistToAxisSig", &trkDistToAxisSig_1);
    T1->SetBranchAddress("trkIpProb2d", &trkIpProb2d_1);
    T1->SetBranchAddress("trkIpProb3d", &trkIpProb3d_1);
    T1->SetBranchAddress("trkDz", &trkDz_1);
    T1->SetBranchAddress("trkPdgId", &trkPdgId_1);
    T1->SetBranchAddress("trkMatchSta", &trkMatchSta_1);




    Long64_t event2 = 0;
    int run2 = 0;
    int lumi2 = 0;
    int hiBin2 = 0;
    float hiHF2 = 0;
    int NVertex2 = 0;
    float VX2 = 0, VY2 = 0, VZ2 = 0;
    float VXError2 = 0, VYError2 = 0, VZError2 = 0;
    float NCollWeight2 = 0;
    float EventWeight2 = 0;
    float PTHat2 = 0;
    int NPU2 = 0;
    float JetPT_2 = 0;
    float JetEta_2 = 0;
    float JetPhi_2 = 0;
    bool isMuMuTagged_2 = false;
    float muPt1_2 = 0;
    float muPt2_2 = 0;
    float muEta1_2 = 0;
    float muEta2_2 = 0;
    float muPhi1_2 = 0;
    float muPhi2_2 = 0;
    int muCharge1_2 = 0;
    int muCharge2_2 = 0;
    float muDiDxy1_2 = 0;
    float muDiDxy1Err_2 = 0;
    float muDiDxy2_2 = 0;
    float muDiDxy2Err_2 = 0;
    float muDiDz1_2 = 0;
    float muDiDz1Err_2 = 0;
    float muDiDz2_2 = 0;
    float muDiDz2Err_2 = 0;
    float muDiDxy1Dxy2_2 = 0;
    float muDiDxy1Dxy2Err_2 = 0;
    float mumuMass_2 = 0;
    float mumuEta_2 = 0;
    float mumuY_2 = 0;
    float mumuPhi_2 = 0;
    float mumuPt_2 = 0;
    bool mumuIsGenMatched_2 = false;
    float DRJetmu1_2 = 0;
    float DRJetmu2_2 = 0;
    float muDeta_2 = 0;
    float muDphi_2 = 0;
    float muDR_2 = 0;
    vector<float> *ExtraMuWeight2 = nullptr;
    float MuMuWeight2 = 0;

    int MJTHadronFlavor_2 = 0;
    int MJTNcHad_2 = 0;
    int MJTNbHad_2 = 0;
    int jtNsvtx_2 = 0;
    int jtNtrk_2 = 0;
    float jtptCh_2 = 0;

    int nsvtx_2 = 0;
    vector<int> *svtxJetId_2 = nullptr;
    vector<int> *svtxNtrk_2 = nullptr;
    vector<float> *svtxdl_2 = nullptr;
    vector<float> *svtxdls_2 = nullptr;
    vector<float> *svtxdl2d_2 = nullptr;
    vector<float> *svtxdls2d_2 = nullptr;
    vector<float> *svtxm_2 = nullptr;
    vector<float> *svtxmcorr_2 = nullptr;
    vector<float> *svtxnormchi2_2 = nullptr;
    vector<float> *svtxchi2_2 = nullptr;
    int svtxIdx_mu1_2 = 0;
    int svtxIdx_mu2_2 = 0;
    int trkIdx_mu1_2 = 0;
    int trkIdx_mu2_2 = 0;
    int ntrk_2 = 0;
    vector<int> *trkJetId_2 = nullptr;
    vector<int> *trkSvtxId_2 = nullptr;
    vector<float> *trkPt_2 = nullptr;
    vector<float> *trkEta_2 = nullptr;
    vector<float> *trkPhi_2 = nullptr;
    vector<float> *trkIp3d_2 = nullptr;
    vector<float> *trkIp3dSig_2 = nullptr;
    vector<float> *trkIp2d_2 = nullptr;
    vector<float> *trkIp2dSig_2 = nullptr;
    vector<float> *trkDistToAxis_2 = nullptr;
    vector<float> *trkDistToAxisSig_2 = nullptr;
    vector<float> *trkIpProb2d_2 = nullptr;
    vector<float> *trkIpProb3d_2 = nullptr;
    vector<float> *trkDz_2 = nullptr;
    vector<int> *trkPdgId_2 = nullptr;
    vector<int> *trkMatchSta_2 = nullptr;

    T2->SetBranchAddress("Event", &event2);
    T2->SetBranchAddress("Run", &run2);
    T2->SetBranchAddress("Lumi", &lumi2);
    T2->SetBranchAddress("hiBin", &hiBin2);
    T2->SetBranchAddress("hiHF", &hiHF2);       
    T2->SetBranchAddress("NVertex", &NVertex2);
    T2->SetBranchAddress("VX", &VX2);
    T2->SetBranchAddress("VY", &VY2);
    T2->SetBranchAddress("VZ", &VZ2);
    T2->SetBranchAddress("VXError", &VXError2);
    T2->SetBranchAddress("VYError", &VYError2);     
    T2->SetBranchAddress("VZError", &VZError2);
    T2->SetBranchAddress("NCollWeight", &NCollWeight2);
    T2->SetBranchAddress("EventWeight", &EventWeight2);
    T2->SetBranchAddress("PTHat", &PTHat2);
    T2->SetBranchAddress("NPU", &NPU2);
    T2->SetBranchAddress("JetPT", &JetPT_2);
    T2->SetBranchAddress("JetEta", &JetEta_2);
    T2->SetBranchAddress("JetPhi", &JetPhi_2);
    T2->SetBranchAddress("IsMuMuTagged", &isMuMuTagged_2);
    T2->SetBranchAddress("muPt1", &muPt1_2);
    T2->SetBranchAddress("muPt2", &muPt2_2);
    T2->SetBranchAddress("muEta1", &muEta1_2);
    T2->SetBranchAddress("muEta2", &muEta2_2);
    T2->SetBranchAddress("muPhi1", &muPhi1_2);
    T2->SetBranchAddress("muPhi2", &muPhi2_2);
    T2->SetBranchAddress("muCharge1", &muCharge1_2);
    T2->SetBranchAddress("muCharge2", &muCharge2_2);
    T2->SetBranchAddress("muDiDxy1", &muDiDxy1_2);
    T2->SetBranchAddress("muDiDxy1Err", &muDiDxy1Err_2);
    T2->SetBranchAddress("muDiDxy2", &muDiDxy2_2);
    T2->SetBranchAddress("muDiDxy2Err", &muDiDxy2Err_2);
    T2->SetBranchAddress("muDiDz1", &muDiDz1_2);
    T2->SetBranchAddress("muDiDz1Err", &muDiDz1Err_2);
    T2->SetBranchAddress("muDiDz2", &muDiDz2_2);
    T2->SetBranchAddress("muDiDz2Err", &muDiDz2Err_2);
    T2->SetBranchAddress("muDiDxy1Dxy2", &muDiDxy1Dxy2_2);
    T2->SetBranchAddress("muDiDxy1Dxy2Err", &muDiDxy1Dxy2Err_2);
    T2->SetBranchAddress("mumuMass", &mumuMass_2);
    T2->SetBranchAddress("mumuEta", &mumuEta_2);
    T2->SetBranchAddress("mumuY", &mumuY_2);
    T2->SetBranchAddress("mumuPhi", &mumuPhi_2);
    T2->SetBranchAddress("mumuPt", &mumuPt_2);
    T2->SetBranchAddress("mumuIsGenMatched", &mumuIsGenMatched_2);
    T2->SetBranchAddress("DRJetmu1", &DRJetmu1_2);
    T2->SetBranchAddress("DRJetmu2", &DRJetmu2_2);
    T2->SetBranchAddress("muDeta", &muDeta_2);
    T2->SetBranchAddress("muDphi", &muDphi_2);
    T2->SetBranchAddress("muDR", &muDR_2);
    T2->SetBranchAddress("ExtraMuWeight", &ExtraMuWeight2);
    T2->SetBranchAddress("MuMuWeight", &MuMuWeight2);

    T2->SetBranchAddress("MJTHadronFlavor", &MJTHadronFlavor_2);
    T2->SetBranchAddress("MJTNcHad", &MJTNcHad_2);
    T2->SetBranchAddress("MJTNbHad", &MJTNbHad_2);
    T2->SetBranchAddress("jtNsvtx", &jtNsvtx_2);
    T2->SetBranchAddress("jtNtrk", &jtNtrk_2);
    T2->SetBranchAddress("jtptCh", &jtptCh_2);
    //T2->SetBranchAddress("nsvtx", &nsvtx_2);
    T2->SetBranchAddress("svtxJetId", &svtxJetId_2);
    T2->SetBranchAddress("svtxNtrk", &svtxNtrk_2);
    T2->SetBranchAddress("svtxdl", &svtxdl_2);
    T2->SetBranchAddress("svtxdls", &svtxdls_2);
    T2->SetBranchAddress("svtxdl2d", &svtxdl2d_2);
    T2->SetBranchAddress("svtxdls2d", &svtxdls2d_2);
    T2->SetBranchAddress("svtxm", &svtxm_2);
    T2->SetBranchAddress("svtxmcorr", &svtxmcorr_2);
    T2->SetBranchAddress("svtxnormchi2", &svtxnormchi2_2);
    T2->SetBranchAddress("svtxchi2", &svtxchi2_2);
    T2->SetBranchAddress("svtxIdx_mu1", &svtxIdx_mu1_2);
    T2->SetBranchAddress("svtxIdx_mu2", &svtxIdx_mu2_2);
    T2->SetBranchAddress("trkIdx_mu1", &trkIdx_mu1_2);
    T2->SetBranchAddress("trkIdx_mu2", &trkIdx_mu2_2);
    //T2->SetBranchAddress("ntrk", &ntrk_2);  
    T2->SetBranchAddress("trkJetId", &trkJetId_2);
    T2->SetBranchAddress("trkSvtxId", &trkSvtxId_2);
    T2->SetBranchAddress("trkPt", &trkPt_2);
    T2->SetBranchAddress("trkEta", &trkEta_2);
    T2->SetBranchAddress("trkPhi", &trkPhi_2);
    T2->SetBranchAddress("trkIp3d", &trkIp3d_2);
    T2->SetBranchAddress("trkIp3dSig", &trkIp3dSig_2);
    T2->SetBranchAddress("trkIp2d", &trkIp2d_2);
    T2->SetBranchAddress("trkIp2dSig", &trkIp2dSig_2);
    T2->SetBranchAddress("trkDistToAxis", &trkDistToAxis_2);
    T2->SetBranchAddress("trkDistToAxisSig", &trkDistToAxisSig_2);
    T2->SetBranchAddress("trkIpProb2d", &trkIpProb2d_2);
    T2->SetBranchAddress("trkIpProb3d", &trkIpProb3d_2);
    T2->SetBranchAddress("trkDz", &trkDz_2);
    T2->SetBranchAddress("trkPdgId", &trkPdgId_2);
    T2->SetBranchAddress("trkMatchSta", &trkMatchSta_2);
    

    int jetindex = 0;
    for(int i = 0; i < T1->GetEntries(); i++){
        if(i % 10000 == 0) cout << "Processing entry " << i *1.0/ T1->GetEntries() << endl;


        T1->GetEntry(i);

        // Jet loop
        if(JetPT_1->size() > 0){

            for(int j = 0; j < JetPT_1->size(); j++){
                
                T2->GetEntry(jetindex);

                // EVENT PROPERTIES
                if(event1 != event2) {cout << "EVENT MISMATCH: " << event1 << " " << event2 << endl;}
                if(run1 != run2) {cout << "RUN MISMATCH: " << run1 << " " << run2 << endl;}
                if(lumi1 != lumi2) {cout << "LUMI MISMATCH: " << lumi1 << " " << lumi2 << endl;}
                if(hiBin1 != hiBin2) {cout << "HIBIN MISMATCH: " << hiBin1 << " " << hiBin2 << endl;}
                if(hiHF1 != hiHF2) {cout << "HIHF MISMATCH: " << hiHF1 << " " << hiHF2 << endl;}
                if(NVertex1 != NVertex2) {cout << "NVERTEX MISMATCH: " << NVertex1 << " " << NVertex2 << endl;}
                if(VX1 != VX2) {cout << "VX MISMATCH: " << VX1 << " " << VX2 << endl;}
                if(VY1 != VY2) {cout << "VY MISMATCH: " << VY1 << " " << VY2 << endl;}
                if(VZ1 != VZ2) {cout << "VZ MISMATCH: " << VZ1 << " " << VZ2 << endl;}
                if(VXError1 != VXError2) {cout << "VXERROR MISMATCH: " << VXError1 << " " << VXError2 << endl;}
                if(VYError1 != VYError2) {cout << "VYERROR MISMATCH: " << VYError1 << " " << VYError2 << endl;}
                if(VZError1 != VZError2) {cout << "VZERROR MISMATCH: " << VZError1 << " " << VZError2 << endl;}
                if(NCollWeight1 != NCollWeight2) {cout << "NCOLLWEIGHT MISMATCH: " << NCollWeight1 << " " << NCollWeight2 << endl;}
                if(EventWeight1 != EventWeight2) {cout << "EVENTWEIGHT MISMATCH: " << EventWeight1 << " " << EventWeight2 << endl;} 
                if(PTHat1 != PTHat2) {cout << "PTHAT MISMATCH: " << PTHat1 << " " << PTHat2 << endl;}

                if(NPU1 != NPU2) {cout << "NPU MISMATCH: " << NPU1 << " " << NPU2 << endl;}



                // JET PROPERTIES
                if(JetPT_1->at(j) != JetPT_2) {cout << "JET PT MISMATCH: " << JetPT_1->at(j) << " " << JetPT_2 << endl;}
                if(JetEta_1->at(j) != JetEta_2) {cout << "JET ETA MISMATCH: " << JetEta_1->at(j) << " " << JetEta_2 << endl;}
                if(JetPhi_1->at(j) != JetPhi_2) {cout << "JET PHI MISMATCH: " << JetPhi_1->at(j) << " " << JetPhi_2 << endl;}
                if(isMuMuTagged_1->at(j) != isMuMuTagged_2) {cout << "ISMUMUTAGGED MISMATCH: " << isMuMuTagged_1->at(j) << " " << isMuMuTagged_2 << endl;}
                
                if(MJTHadronFlavor_1->at(j) != MJTHadronFlavor_2) {cout << "MJTHADRONFLAVOR MISMATCH: " << MJTHadronFlavor_1->at(j) << " " << MJTHadronFlavor_2 << endl;}
                if(MJTNcHad_1->at(j) != MJTNcHad_2) {cout << "MJTNCHAD MISMATCH: " << MJTNcHad_1->at(j) << " " << MJTNcHad_2 << endl;}
                if(MJTNbHad_1->at(j) != MJTNbHad_2) {cout << "MJTNBHAD MISMATCH: " << MJTNbHad_1->at(j) << " " << MJTNbHad_2 << endl;}
                if(jtNsvtx_1->at(j) != jtNsvtx_2) {cout << "JTNVTX MISMATCH: " << jtNsvtx_1->at(j) << " " << jtNsvtx_2 << endl;}
                if(jtNtrk_1->at(j) != jtNtrk_2) {cout << "JTNTRK MISMATCH: " << jtNtrk_1->at(j) << " " << jtNtrk_2 << endl;}
                if(jtptCh_1->at(j) != jtptCh_2) {cout << "JPTCH MISMATCH: " << jtptCh_1->at(j) << " " << jtptCh_2 << endl;}


                for(int k = 0; k < jtNsvtx_1->at(j); k++){
                    if(svtxJetId_1->at(j).at(k) != svtxJetId_2->at(k)) {cout << "SVTXJETID MISMATCH index " << k << ": " << svtxJetId_1->at(j).at(k) << " " << svtxJetId_2->at(k) << endl;}
                    if(svtxNtrk_1->at(j).at(k) != svtxNtrk_2->at(k)) {cout << "SVTXNTRK MISMATCH index " << k << ": " << svtxNtrk_1->at(j).at(k) << " " << svtxNtrk_2->at(k) << endl;}
                    if(svtxdl_1->at(j).at(k) != svtxdl_2->at(k) && notbothnan(svtxdl_1->at(j).at(k), svtxdl_2->at(k))) {cout << "SVTXDL MISMATCH index " << k << ": " << svtxdl_1->at(j).at(k) << " " << svtxdl_2->at(k) << endl;}
                    if(svtxdls_1->at(j).at(k) != svtxdls_2->at(k) && notbothnan(svtxdls_1->at(j).at(k), svtxdls_2->at(k))) {cout << "SVTXDLS MISMATCH index " << k << ": " << svtxdls_1->at(j).at(k) << " " << svtxdls_2->at(k) << endl;}
                    if(svtxdl2d_1->at(j).at(k) != svtxdl2d_2->at(k) && notbothnan(svtxdl2d_1->at(j).at(k), svtxdl2d_2->at(k))) {cout << "SVTXDL2D MISMATCH index " << k << ": " << svtxdl2d_1->at(j).at(k) << " " << svtxdl2d_2->at(k) << endl;}
                    if(svtxdls2d_1->at(j).at(k) != svtxdls2d_2->at(k) && notbothnan(svtxdls2d_1->at(j).at(k), svtxdls2d_2->at(k))) {cout << "SVTXDLS2D MISMATCH index " << k << ": " << svtxdls2d_1->at(j).at(k) << " " << svtxdls2d_2->at(k) << endl;}
                    if(svtxm_1->at(j).at(k) != svtxm_2->at(k) && notbothnan(svtxm_1->at(j).at(k), svtxm_2->at(k))) {cout << "SVTXM MISMATCH index " << k << ": " << svtxm_1->at(j).at(k) << " " << svtxm_2->at(k) << endl;}
                    if(svtxmcorr_1->at(j).at(k) != svtxmcorr_2->at(k) && notbothnan(svtxmcorr_1->at(j).at(k), svtxmcorr_2->at(k))) {cout << "SVTXMCORR MISMATCH index " << k << ": " << svtxmcorr_1->at(j).at(k) << " " << svtxmcorr_2->at(k) << endl;}
                    if(svtxnormchi2_1->at(j).at(k) != svtxnormchi2_2->at(k) && notbothnan(svtxnormchi2_1->at(j).at(k), svtxnormchi2_2->at(k))) {cout << "SVTXNORMCHI2 MISMATCH index " << k << ": " << svtxnormchi2_1->at(j).at(k) << " " << svtxnormchi2_2->at(k) << endl;}
                    if(svtxchi2_1->at(j).at(k) != svtxchi2_2->at(k) && notbothnan(svtxchi2_1->at(j).at(k), svtxchi2_2->at(k))) {cout << "SVTXCHI2 MISMATCH index " << k << ": " << svtxchi2_1->at(j).at(k) << " " << svtxchi2_2->at(k) << endl;}
                }
                if(svtxIdx_mu1_1->at(j) != svtxIdx_mu1_2) {cout << "SVTXIDX_MU1 MISMATCH: " << svtxIdx_mu1_1->at(j) << " " << svtxIdx_mu1_2 << endl;}
                if(svtxIdx_mu2_1->at(j) != svtxIdx_mu2_2) {cout << "SVTXIDX_MU2 MISMATCH: " << svtxIdx_mu2_1->at(j) << " " << svtxIdx_mu2_2 << endl;}
                if(trkIdx_mu1_1->at(j) != trkIdx_mu1_2) {cout << "TRKIDX_MU1 MISMATCH: " << trkIdx_mu1_1->at(j) << " " << trkIdx_mu1_2 << endl;}
                if(trkIdx_mu2_1->at(j) != trkIdx_mu2_2) {cout << "TRKIDX_MU2 MISMATCH: " << trkIdx_mu2_1->at(j) << " " << trkIdx_mu2_2 << endl;}    
                for(int k = 0; k < jtNtrk_1->at(j); k++){
                    if(trkJetId_1->at(j).at(k) != trkJetId_2->at(k)) {cout << "TRKJETID MISMATCH index " << k << ": " << trkJetId_1->at(j).at(k) << " " << trkJetId_2->at(k) << endl;}
                    if(trkSvtxId_1->at(j).at(k) != trkSvtxId_2->at(k)) {cout << "TRKSVTXID MISMATCH index " << k << ": " << trkSvtxId_1->at(j).at(k) << " " << trkSvtxId_2->at(k) << endl;}
                    if(trkPt_1->at(j).at(k) != trkPt_2->at(k) && notbothnan(trkPt_1->at(j).at(k), trkPt_2->at(k))) {cout << "TRKPT MISMATCH index " << k << ": " << trkPt_1->at(j).at(k) << " " << trkPt_2->at(k) << endl;}
                    if(trkEta_1->at(j).at(k) != trkEta_2->at(k) && notbothnan(trkEta_1->at(j).at(k), trkEta_2->at(k))) {cout << "TRKETA MISMATCH index " << k << ": " << trkEta_1->at(j).at(k) << " " << trkEta_2->at(k) << endl;}
                    if(trkPhi_1->at(j).at(k) != trkPhi_2->at(k) && notbothnan(trkPhi_1->at(j).at(k), trkPhi_2->at(k))) {cout << "TRKPHI MISMATCH index " << k << ": " << trkPhi_1->at(j).at(k) << " " << trkPhi_2->at(k) << endl;}
                    if(trkIp3d_1->at(j).at(k) != trkIp3d_2->at(k) && notbothnan(trkIp3d_1->at(j).at(k), trkIp3d_2->at(k))) {cout << "TRKIP3D MISMATCH index " << k << ": " << trkIp3d_1->at(j).at(k) << " " << trkIp3d_2->at(k) << endl;}
                    if(trkIp3dSig_1->at(j).at(k) != trkIp3dSig_2->at(k) && notbothnan(trkIp3dSig_1->at(j).at(k), trkIp3dSig_2->at(k))) {cout << "TRKIP3DSIG MISMATCH index " << k << ": " << trkIp3dSig_1->at(j).at(k) << " " << trkIp3dSig_2->at(k) << endl;}
                    if(trkIp2d_1->at(j).at(k) != trkIp2d_2->at(k) && notbothnan(trkIp2d_1->at(j).at(k), trkIp2d_2->at(k))) {cout << "TRKIP2D MISMATCH index " << k << ": " << trkIp2d_1->at(j).at(k) << " " << trkIp2d_2->at(k) << endl;}
                    if(trkIp2dSig_1->at(j).at(k) != trkIp2dSig_2->at(k) && notbothnan(trkIp2dSig_1->at(j).at(k), trkIp2dSig_2->at(k))) {cout << "TRKIP2DSIG MISMATCH index " << k << ": " << trkIp2dSig_1->at(j).at(k) << " " << trkIp2dSig_2->at(k) << endl;}
                    if(trkDistToAxis_1->at(j).at(k) != trkDistToAxis_2->at(k) && notbothnan(trkDistToAxis_1->at(j).at(k), trkDistToAxis_2->at(k))) {cout << "TRKDISTTOAXIS MISMATCH index " << k << ": " << trkDistToAxis_1->at(j).at(k) << " " << trkDistToAxis_2->at(k) << endl;}
                    if(trkDistToAxisSig_1->at(j).at(k) != trkDistToAxisSig_2->at(k) && notbothnan(trkDistToAxisSig_1->at(j).at(k), trkDistToAxisSig_2->at(k))) {cout << "TRKDISTTOAXISSIG MISMATCH index " << k << ": " << trkDistToAxisSig_1->at(j).at(k) << " " << trkDistToAxisSig_2->at(k) << endl;}
                    if(trkIpProb2d_1->at(j).at(k) != trkIpProb2d_2->at(k) && notbothnan(trkIpProb2d_1->at(j).at(k), trkIpProb2d_2->at(k))) {cout << "TRKIPPROB2D MISMATCH index " << k << ": " << trkIpProb2d_1->at(j).at(k) << " " << trkIpProb2d_2->at(k) << endl;}
                    if(trkIpProb3d_1->at(j).at(k) != trkIpProb3d_2->at(k) && notbothnan(trkIpProb3d_1->at(j).at(k), trkIpProb3d_2->at(k))) {cout << "TRKIPPROB3D MISMATCH index " << k << ": " << trkIpProb3d_1->at(j).at(k) << " " << trkIpProb3d_2->at(k) << endl;}
                    if(trkDz_1->at(j).at(k) != trkDz_2->at(k) && notbothnan(trkDz_1->at(j).at(k), trkDz_2->at(k))) {cout << "TRKDZ MISMATCH index " << k << ": " << trkDz_1->at(j).at(k) << " " << trkDz_2->at(k) << endl;}
                    if(trkPdgId_1->at(j).at(k) != trkPdgId_2->at(k) && notbothnan(trkPdgId_1->at(j).at(k), trkPdgId_2->at(k))) {cout << "TRKPDGID MISMATCH index " << k << ": " << trkPdgId_1->at(j).at(k) << " " << trkPdgId_2->at(k) << endl;}
                    if(trkMatchSta_1->at(j).at(k) != trkMatchSta_2->at(k) && notbothnan(trkMatchSta_1->at(j).at(k), trkMatchSta_2->at(k))) {cout << "TRKMATCHSTA MISMATCH index " << k << ": " << trkMatchSta_1->at(j).at(k) << " " << trkMatchSta_2->at(k) << endl;}
                }



                if(isMuMuTagged_1->at(j) == true){
                // MUON PROPERTIES
                for(int k = 0; k < 12; k++){
                    if(ExtraMuWeight_1->at(j).at(k) != ExtraMuWeight2->at(k)) {cout << "EXTRAMUWEIGHT MISMATCH index " << k << ": " << ExtraMuWeight_1->at(j).at(k) << " " << ExtraMuWeight2->at(k) << endl;}
                }
                if(muPt1_1->at(j) != muPt1_2) {cout << "MUPT1 MISMATCH: " << muPt1_1->at(j) << " " << muPt1_2 << endl;}
                if(muPt2_1->at(j) != muPt2_2) {cout << "MUPT2 MISMATCH: " << muPt2_1->at(j) << " " << muPt2_2 << endl;}
                if(muEta1_1->at(j) != muEta1_2) {cout << "MUETA1 MISMATCH: " << muEta1_1->at(j) << " " << muEta1_2 << endl;}
                if(muEta2_1->at(j) != muEta2_2) {cout << "MUETA2 MISMATCH: " << muEta2_1->at(j) << " " << muEta2_2 << endl;}
                if(muPhi1_1->at(j) != muPhi1_2) {cout << "MUPHI1 MISMATCH: " << muPhi1_1->at(j) << " " << muPhi1_2 << endl;}
                if(muPhi2_1->at(j) != muPhi2_2) {cout << "MUPHI2 MISMATCH: " << muPhi2_1->at(j) << " " << muPhi2_2 << endl;}
                if(muCharge1_1->at(j) != muCharge1_2) {cout << "MUCHARGE1 MISMATCH: " << muCharge1_1->at(j) << " " << muCharge1_2 << endl;}
                if(muCharge2_1->at(j) != muCharge2_2) {cout << "MUCHARGE2 MISMATCH: " << muCharge2_1->at(j) << " " << muCharge2_2 << endl;}
                if(muDiDxy1_1->at(j) != muDiDxy1_2) {cout << "MUDIDXY1 MISMATCH: " << muDiDxy1_1->at(j) << " " << muDiDxy1_2 << endl;}
                if(muDiDxy1Err_1->at(j) != muDiDxy1Err_2) {cout << "MUDIDXY1ERR MISMATCH: " << muDiDxy1Err_1->at(j) << " " << muDiDxy1Err_2 << endl;}
                if(muDiDxy2_1->at(j) != muDiDxy2_2) {cout << "MUDIDXY2 MISMATCH: " << muDiDxy2_1->at(j) << " " << muDiDxy2_2 << endl;}
                if(muDiDxy2Err_1->at(j) != muDiDxy2Err_2) {cout << "MUDIDXY2ERR MISMATCH: " << muDiDxy2Err_1->at(j) << " " << muDiDxy2Err_2 << endl;}
                if(muDiDz1_1->at(j) != muDiDz1_2) {cout << "MUDIDZ1 MISMATCH: " << muDiDz1_1->at(j) << " " << muDiDz1_2 << endl;}
                if(muDiDz1Err_1->at(j) != muDiDz1Err_2) {cout << "MUDIDZ1ERR MISMATCH: " << muDiDz1Err_1->at(j) << " " << muDiDz1Err_2 << endl;}
                if(muDiDz2_1->at(j) != muDiDz2_2) {cout << "MUDIDZ2 MISMATCH: " << muDiDz2_1->at(j) << " " << muDiDz2_2 << endl;}
                if(muDiDz2Err_1->at(j) != muDiDz2Err_2) {cout << "MUDIDZ2ERR MISMATCH: " << muDiDz2Err_1->at(j) << " " << muDiDz2Err_2 << endl;}
                if(muDiDxy1Dxy2_1->at(j) != muDiDxy1Dxy2_2) {cout << "MUDIDXY1DXY2 MISMATCH: " << muDiDxy1Dxy2_1->at(j) << " " << muDiDxy1Dxy2_2 << endl;}
                if(muDiDxy1Dxy2Err_1->at(j) != muDiDxy1Dxy2Err_2) {cout << "MUDIDXY1DXY2ERR MISMATCH: " << muDiDxy1Dxy2Err_1->at(j) << " " << muDiDxy1Dxy2Err_2 << endl;}
                if(mumuMass_1->at(j) != mumuMass_2) {cout << "MUMUMASS MISMATCH: " << mumuMass_1->at(j) << " " << mumuMass_2 << endl;}
                if(mumuEta_1->at(j) != mumuEta_2) {cout << "MUMUETA MISMATCH: " << mumuEta_1->at(j) << " " << mumuEta_2 << endl;}
                if(mumuY_1->at(j) != mumuY_2) {cout << "MUMUY MISMATCH: " << mumuY_1->at(j) << " " << mumuY_2 << endl;}
                if(mumuPhi_1->at(j) != mumuPhi_2) {cout << "MUMUPHI MISMATCH: " << mumuPhi_1->at(j) << " " << mumuPhi_2 << endl;}
                if(mumuPt_1->at(j) != mumuPt_2) {cout << "MUMUPT MISMATCH: " << mumuPt_1->at(j) << " " << mumuPt_2 << endl;}
                if(mumuIsGenMatched_1->at(j) != mumuIsGenMatched_2) {cout << "MUMUISGENMATCHED MISMATCH: " << mumuIsGenMatched_1->at(j) << " " << mumuIsGenMatched_2 << endl;}
                if(DRJetmu1_1->at(j) != DRJetmu1_2) {cout << "DRJETMU1 MISMATCH: " << DRJetmu1_1->at(j) << " " << DRJetmu1_2 << endl;}
                if(DRJetmu2_1->at(j) != DRJetmu2_2) {cout << "DRJETMU2 MISMATCH: " << DRJetmu2_1->at(j) << " " << DRJetmu2_2 << endl;}
                if(muDeta_1->at(j) != muDeta_2) {cout << "MUDETA MISMATCH: " << muDeta_1->at(j) << " " << muDeta_2 << endl;}
                if(muDphi_1->at(j) != muDphi_2) {cout << "MUDPHI MISMATCH: " << muDphi_1->at(j) << " " << muDphi_2 << endl;}
                if(muDR_1->at(j) != muDR_2) {cout << "MUDR MISMATCH: " << muDR_1->at(j) << " " << muDR_2 << endl;}
                }



                jetindex+=1;



            }


        }

    }

    // Clean up files
    if(skim1) {
        skim1->Close();
        delete skim1;
    }
    if(skim2) {
        skim2->Close();
        delete skim2;
    }
    
    cout << "Analysis completed successfully" << endl;
}