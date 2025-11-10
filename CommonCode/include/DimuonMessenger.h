#ifndef DIMUONMESSENGER_H_GUARD
#define DIMUONMESSENGER_H_GUARD

#include <iostream>
#include <vector>

#include "TFile.h"
#include "TTree.h"

#define JETCOUNTMAX 500
#define GENCOUNTMAX 250
#define VERTEXCOUNTMAX 200
#define DZEROCOUNTMAX 20000 //FIXME: to be fined tuned
#define DZEROGENCOUNTMAX 300 //FIXME: to be fined tuned
#define SVTXCOUNTMAX 50
#define TRACKCOUNTMAX 20000
#define PLANEMAX 200
#define MUMAX 50

class DimuonJetMessenger
{
public:
   TTree *Tree;

   // EVENT PROPERTIES
   int Run;
   long long Event;
   int Lumi;
   int hiBin;
   float hiHF;
   int NVertex;
   float VX, VY, VZ, VXError, VYError, VZError;
   float NCollWeight;
   float EventWeight;
   float PTHat;
   int NPU;
   int nsvtx;
   int ntrk;

   // JET PROPERTIES
   float JetPT;
   float JetEta;
   float JetPhi;
   bool IsMuMuTagged;
   bool GenIsMuMuTagged;

   // SINGLE MU INFO
   float muPt1;
   float muPt2;
   float muEta1;
   float muEta2;
   float muPhi1;
   float muPhi2;
   int muCharge1;
   int muCharge2;
   float muDiDxy1;
   float muDiDxy1Err;
   float muDiDxy2;
   float muDiDxy2Err;
   float muDiDz1;
   float muDiDz1Err;
   float muDiDz2;
   float muDiDz2Err;
   float muDiDxy1Dxy2;
   float muDiDxy1Dxy2Err;

   // DIMUON INFO
   float mumuMass;
   float mumuEta;
   float mumuY;
   float mumuPhi;
   float mumuPt;
   bool mumuIsGenMatched;
   //std::vector<int> *mumuisOnia;
   float DRJetmu1;
   float DRJetmu2;
   float muDeta;
   float muDphi;
   float muDR;
   std::vector<float> *ExtraMuWeight; // size 12
   float MuMuWeight;

   //GEN MUON INFO
   float GenMuPt1;
   float GenMuPt2;
   float GenMuEta1;
   float GenMuEta2;
   float GenMuPhi1;
   float GenMuPhi2;
   float GenMuMuMass;
   float GenMuMuEta;
   float GenMuMuY;
   float GenMuMuPhi;
   float GenMuMuPt;
   float GenMuDeta;
   float GenMuDphi;
   float GenMuDR;

   // FLAVOR
   int PartonFlavor;
   int HadronFlavor;
   int NcHad;
   int NbHad;
   int NbPar;
   int NcPar;
   bool HasGSPB;
   bool HasGSPC;


   // TAGGING INFO
   float PN_pu;
   float PN_bb;
   float PN_b;
   float PN_cc;
   float PN_c;
   float PN_undef;
   float PN_uds;
   float PN_g;

   // MISC
   int jtNsvtx;
   int jtNtrk;
   float jtptCh;

   // SVTX INFO
   std::vector<int> *svtxJetId;
   std::vector<int> *svtxNtrk;
   std::vector<float> *svtxdl;
   std::vector<float> *svtxdls;
   std::vector<float> *svtxdl2d;
   std::vector<float> *svtxdls2d;
   std::vector<float> *svtxm;
   std::vector<float> *svtxmcorr;
   std::vector<float> *svtxpt;
   std::vector<float> *svtxnormchi2;
   std::vector<float> *svtxchi2;

   int svtxIdx_mu1;
   int svtxIdx_mu2;

   // TRACK INFO
   std::vector<int> *trkJetId;
   std::vector<int> *trkSvtxId;
   std::vector<float> *trkPt;
   std::vector<float> *trkEta;
   std::vector<float> *trkPhi;
   std::vector<float> *trkIp3d;
   std::vector<float> *trkIp3dSig;
   std::vector<float> *trkIp2d;
   std::vector<float> *trkIp2dSig;
   std::vector<float> *trkDistToAxis;
   std::vector<float> *trkDistToAxisSig;
   std::vector<float> *trkIpProb3d;
   std::vector<float> *trkIpProb2d;
   std::vector<float> *trkDz;
   std::vector<int> *trkPdgId;
   std::vector<int> *trkMatchSta;

   int trkIdx_mu1;
   int trkIdx_mu2;

private:
   bool WriteMode;
   bool Initialized;
public:
   DimuonJetMessenger(TFile &File, std::string TreeName = "tree");
   DimuonJetMessenger(TFile *File, std::string TreeName = "tree");
   DimuonJetMessenger(TTree *MuMuJetTree = nullptr);
   ~DimuonJetMessenger();
   bool Initialize(TTree *MuMuJetTree);
   bool Initialize();
   int GetEntries();
   bool GetEntry(int iEntry);
   bool SetBranch(TTree *T);
   void Clear();
   void Clear_Jet();
   void CopyNonTrack(DimuonJetMessenger &M);
   bool FillEntry();
};

class GenDimuonJetMessenger
{
public:
    TTree *Tree;
    
    // EVENT PROPERTIES
    int Run;
    long long Event;
    int Lumi;
    int hiBin;
    float hiHF;
    int NVertex;
    float VX, VY, VZ, VXError, VYError, VZError;
    float NCollWeight;
    float EventWeight;
    float PTHat;
    int NPU;
    
    // JET PROPERTIES
   float GenJetPT;
   float GenJetEta;
   float GenJetPhi;
   int GenJetMatchIdx;
   bool GenIsMuMuTagged;



private:
   bool WriteMode;
   bool Initialized;
public:
   GenDimuonJetMessenger(TFile &File, std::string TreeName = "tree");
   GenDimuonJetMessenger(TFile *File, std::string TreeName = "tree");
   GenDimuonJetMessenger(TTree *MuMuJetTree = nullptr);
   ~GenDimuonJetMessenger();
   bool Initialize(TTree *MuMuJetTree);
   bool Initialize();
   int GetEntries();
   bool GetEntry(int iEntry);
   bool SetBranch(TTree *T);
   void Clear();
   void Clear_Jet();
   void CopyNonTrack(GenDimuonJetMessenger &M);
   bool FillEntry();
};

#endif
