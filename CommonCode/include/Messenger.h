#ifndef MESSENGER_H_ASDFASDF
#define MESSENGER_H_ASDFASDF

#include <iostream>
#include <vector>

#include "TFile.h"
#include "TTree.h"

#define JETCOUNTMAX 500
#define GENCOUNTMAX 250
#define VERTEXCOUNTMAX 200
#define DFINDERCOUNTMAX 20000
#define DFINDERGENCOUNTMAX 300 //FIXME: to be fined tuned for Dfinder
#define SVTXCOUNTMAX 50
#define TRACKCOUNTMAX 20000
#define PLANEMAX 200
#define MUMAX 50
#define PPSMAXN 56
#define FSCMAXN 50

// Input/forest messengers
class HiEventTreeMessenger;
class METFilterTreeMessenger;
class GGTreeMessenger;
class RhoTreeMessenger;
class SkimTreeMessenger;
class JetTreeMessenger;
class GenParticleTreeMessenger;
class PFTreeMessenger;
class TriggerTreeMessenger;
class TriggerObjectTreeMessenger;
class TrackTreeMessenger;
class MuTreeMessenger;
class SingleMuTreeMessenger;
class PbPbTrackTreeMessenger;
class PbPbUPCTrackTreeMessenger;
class ZDCTreeMessenger;
class PPSTreeMessenger;
class FSCTreeMessenger;
class HFAdcMessenger;
class DfinderMasterMessenger;
class DzeroTreeMessenger;
class LambdaCpkpiTreeMessenger;
class LambdaCpksTreeMessenger;
class DfinderGenTreeMessenger;
class DzeroGenTreeMessenger;

// Output/skim messengers
class ZHadronMessenger;
class DzeroUPCTreeMessenger;
class LambdaCpksUPCTreeMessenger;
class LambdaCpkpiUPCTreeMessenger;
class ChargedHadronRAATreeMessenger;
class UPCEECTreeMessenger;
class MuMuJetMessenger;


class HiEventTreeMessenger
{
public:
   TTree *Tree;
   float hiHF;
   int hiBin;
   unsigned int Run;
   unsigned long long Event;
   unsigned int Lumi;
   float pthat;
   float vx;
   float vy;
   float vz;
   float weight;
   std::vector<int> *npus;
   std::vector<float> *tnpus;
   float hiHFplus;
   float hiHFminus;
   float hiHFplusEta4;
   float hiHFminusEta4;
   int hiNevtPlane;
   float hiEvtPlanes[PLANEMAX];
   float hiHF_pf;
   float hiHFPlus_pf;
   float hiHFMinus_pf;
   float hiHFPlus_pfle1;
   float hiHFPlus_pfle2;
   float hiHFPlus_pfle3;
   float hiHFMinus_pfle1;
   float hiHFMinus_pfle2;
   float hiHFMinus_pfle3;
   float Ncoll;
   float Npart;
public:
   HiEventTreeMessenger(TFile &File);
   HiEventTreeMessenger(TFile *File);
   HiEventTreeMessenger(TTree *HiEventTree);
   bool Initialize(TTree *HiEventTree);
   bool Initialize();
   bool GetEntry(int iEntry);
   int GetEntries();
};

class METFilterTreeMessenger
{
public:
  TTree *Tree;
  bool cscTightHalo2015Filter;
public:
  METFilterTreeMessenger(TFile &File);
  METFilterTreeMessenger(TFile *File);
  METFilterTreeMessenger(TTree *METFilterTree);
  bool Initialize(TTree *METFilterTree);
  bool Initialize();
  bool GetEntry(int iEntry);
};

class GGTreeMessenger
{
public:
   TTree *Tree;
   int NPUInfo;
   std::vector<int> *PUCount;
   std::vector<int> *PUBX;
   std::vector<float> *PUTrue;
   int PFJetCount;
   std::vector<float> *PFJetPT;
   std::vector<float> *PFJetEta;
   std::vector<float> *PFJetPhi;
   int CaloJetCount;
   std::vector<float> *CaloJetPT;
   std::vector<float> *CaloJetEta;
   std::vector<float> *CaloJetPhi;
   int GenJetCount;
   std::vector<float> *GenJetPT;
   std::vector<float> *GenJetEta;
   std::vector<float> *GenJetPhi;
public:
   GGTreeMessenger(TFile &File, std::string TreeName = "ggHiNtuplizer/EventTree");
   GGTreeMessenger(TFile *File, std::string TreeName = "ggHiNtuplizer/EventTree");
   GGTreeMessenger(TTree *EventTree);
   bool Initialize(TTree *EventTree);
   bool Initialize();
   bool GetEntry(int iEntry);
};

class RhoTreeMessenger
{
public:
   TTree *Tree;
   std::vector<double> *EtaMin;
   std::vector<double> *EtaMax;
   std::vector<double> *Rho;
   std::vector<double> *RhoM;
public:
   RhoTreeMessenger(TFile &File, std::string TreeName = "hiFJRhoAnalyzer/t");
   RhoTreeMessenger(TFile *File, std::string TreeName = "hiFJRhoAnalyzer/t");
   RhoTreeMessenger(TTree *RhoTree);
   bool Initialize(TTree *RhoTree);
   bool Initialize();
   bool GetEntry(int iEntry);
};

class SkimTreeMessenger
{
public:
   TTree *Tree;
   int HBHENoise;
   int HBHENoiseRun2Loose;
   int PVFilter;
   int ClusterCompatibilityFilter;
   int BeamScrapingFilter;
   int HFCoincidenceFilter;
   int HFCoincidenceFilter2Th4;
   int CollisionEventSelection;
public:
   SkimTreeMessenger(TFile &File);
   SkimTreeMessenger(TFile *File);
   SkimTreeMessenger(TTree *SkimTree);
   bool Initialize(TTree *SkimTree);
   bool Initialize();
   bool GetEntry(int iEntry);
   bool PassBasicFilter();
   bool PassBasicFilterLoose();
};

class JetTreeMessenger
{
public:
   TTree *Tree;
   float PTHat;
   int JetCount;
   float JetRawPT[JETCOUNTMAX];
   float JetPT[JETCOUNTMAX];
   float JetEta[JETCOUNTMAX];
   float JetY[JETCOUNTMAX];
   float JetPhi[JETCOUNTMAX];
   float JetPU[JETCOUNTMAX];
   float JetM[JETCOUNTMAX];
   float JetTau1[JETCOUNTMAX];
   float JetTau2[JETCOUNTMAX];
   float JetTau3[JETCOUNTMAX];
   float JetArea[JETCOUNTMAX];
   float JetCSVV1[JETCOUNTMAX];
   float JetCSVV2[JETCOUNTMAX];
   float JetCSVV1N[JETCOUNTMAX];
   float JetCSVV2N[JETCOUNTMAX];
   float JetCSVV1P[JETCOUNTMAX];
   float JetCSVV2P[JETCOUNTMAX];
   std::vector<std::vector<float> > *JetSubJetPT;
   std::vector<std::vector<float> > *JetSubJetEta;
   std::vector<std::vector<float> > *JetSubJetPhi;
   std::vector<std::vector<float> > *JetSubJetM;
   float JetSym[JETCOUNTMAX];
   int JetDroppedBranches[JETCOUNTMAX];
   float RefPT[JETCOUNTMAX];
   float RefEta[JETCOUNTMAX];
   float RefY[JETCOUNTMAX];
   float RefPhi[JETCOUNTMAX];
   float RefM[JETCOUNTMAX];
   float RefArea[JETCOUNTMAX];
   float RefGPT[JETCOUNTMAX];
   float RefGEta[JETCOUNTMAX];
   float RefGPhi[JETCOUNTMAX];
   float RefGM[JETCOUNTMAX];
   float RefPartonPT[JETCOUNTMAX];
   int RefPartonFlavor[JETCOUNTMAX];
   int RefPartonFlavorForB[JETCOUNTMAX];
   int MJTHadronFlavor[JETCOUNTMAX]; // MJT hadron tagging is from newer CMSSW for g2ccbar PbPb analysis
   int MJTNcHad[JETCOUNTMAX];
   int MJTNbHad[JETCOUNTMAX];
   int HadronFlavor[JETCOUNTMAX];
   int PartonFlavor[JETCOUNTMAX];
   int NcHad[JETCOUNTMAX];
   int NbHad[JETCOUNTMAX];
   int NbPar[JETCOUNTMAX];
   int NcPar[JETCOUNTMAX];
   bool HasGSPB[JETCOUNTMAX];
   bool HasGSPC[JETCOUNTMAX];
   float PN_bb[JETCOUNTMAX];
   float PN_b[JETCOUNTMAX];
   float PN_cc[JETCOUNTMAX];
   float PN_c[JETCOUNTMAX];
   float PN_uds[JETCOUNTMAX];
   float PN_g[JETCOUNTMAX];
   float PN_pu[JETCOUNTMAX];
   float PN_undef[JETCOUNTMAX];
   std::vector<std::vector<float> > *RefGSubJetPT;
   std::vector<std::vector<float> > *RefGSubJetEta;
   std::vector<std::vector<float> > *RefGSubJetPhi;
   std::vector<std::vector<float> > *RefGSubJetM;
   int GenCount;
   int GenMatchIndex[JETCOUNTMAX];
   float GenPT[JETCOUNTMAX];
   float GenEta[JETCOUNTMAX];
   float GenY[JETCOUNTMAX];
   float GenPhi[JETCOUNTMAX];
   float GenM[JETCOUNTMAX];
   float HcalSum[JETCOUNTMAX];
   float EcalSum[JETCOUNTMAX];
   float JetPFCHF[JETCOUNTMAX];
   float JetPFNHF[JETCOUNTMAX];
   float JetPFCEF[JETCOUNTMAX];
   float JetPFNEF[JETCOUNTMAX];
   float JetPFMUF[JETCOUNTMAX];
   int JetPFCHM[JETCOUNTMAX];
   int JetPFNHM[JETCOUNTMAX];
   int JetPFCEM[JETCOUNTMAX];
   int JetPFNEM[JETCOUNTMAX];
   int JetPFMUM[JETCOUNTMAX];

   // Jet info 

   int jtNsvtx[JETCOUNTMAX];
   int jtNtrk[JETCOUNTMAX];
   float jtptCh[JETCOUNTMAX];

   // SVTX

   int nsvtx;
   int svtxJetId[SVTXCOUNTMAX];
   int svtxNtrk[SVTXCOUNTMAX];
   float svtxdl[SVTXCOUNTMAX];
   float svtxdls[SVTXCOUNTMAX];
   float svtxdl2d[SVTXCOUNTMAX];
   float svtxdls2d[SVTXCOUNTMAX];
   float svtxm[SVTXCOUNTMAX];
   float svtxmcorr[SVTXCOUNTMAX];
   float svtxpt[SVTXCOUNTMAX];
   float svtxnormchi2[SVTXCOUNTMAX];
   float svtxchi2[SVTXCOUNTMAX];

   // Track

   int ntrk;
   int trkJetId[TRACKCOUNTMAX];
   int trkSvtxId[TRACKCOUNTMAX];
   float trkPt[TRACKCOUNTMAX];
   float trkEta[TRACKCOUNTMAX];
   float trkPhi[TRACKCOUNTMAX];
   float trkIp3d[TRACKCOUNTMAX];
   float trkIp3dSig[TRACKCOUNTMAX];
   float trkIp2d[TRACKCOUNTMAX];
   float trkIp2dSig[TRACKCOUNTMAX];
   float trkDistToAxis[TRACKCOUNTMAX];
   float trkDistToAxisSig[TRACKCOUNTMAX];
   float trkIpProb3d[TRACKCOUNTMAX];
   float trkIpProb2d[TRACKCOUNTMAX];
   float trkDz[TRACKCOUNTMAX];
   int trkPdgId[TRACKCOUNTMAX];
   int trkMatchSta[TRACKCOUNTMAX];
 

public:
   JetTreeMessenger(TFile &File, std::string TreeName = "akCs4PFJetAnalyzer/t");
   JetTreeMessenger(TFile *File, std::string TreeName = "akCs4PFJetAnalyzer/t");
   JetTreeMessenger(TTree *JetTree);
   bool Initialize(TTree *JetTree);
   bool Initialize();
   bool GetEntry(int iEntry);
};

class GenParticleTreeMessenger
{
public:
   TTree *Tree;
   float NPart;
   float NColl;
   float NHard;
   float Phi0;
   int Mult;
   std::vector<float> *PT;
   std::vector<float> *Eta;
   std::vector<float> *Phi;
   std::vector<int> *ID;
   std::vector<int> *Charge;
   std::vector<int> *DaughterCount;
   std::vector<int> *SubEvent;
public:
   GenParticleTreeMessenger(TFile &File);
   GenParticleTreeMessenger(TFile *File);
   GenParticleTreeMessenger(TTree *GenParticleTree);
   bool Initialize(TTree *GenParticleTree);
   bool Initialize();
   bool GetEntry(int iEntry);
};

class PFTreeMessenger
{
public:
   TTree *Tree;
   std::vector<int> *ID;
   std::vector<float> *PT;
   std::vector<float> *E;
   std::vector<float> *Eta;
   std::vector<float> *Phi;
   std::vector<float> *M;
public:
   PFTreeMessenger(TFile &File, std::string TreeName = "pfcandAnalyzer/pfTree");
   PFTreeMessenger(TFile *File, std::string TreeName = "pfcandAnalyzer/pfTree");
   bool Initialize(TFile &File, std::string TreeName = "pfcandAnalyzer/pfTree");
   bool Initialize(TFile *File, std::string TreeName = "pfcandAnalyzer/pfTree");
   PFTreeMessenger(TTree *PFTree);
   bool Initialize(TTree *PFTree);
   bool Initialize();
   bool GetEntry(int iEntry);
};

class TriggerTreeMessenger
{
public:
   TTree *Tree;
   int Run;
   unsigned long long Event;
   int Lumi;
   std::vector<std::string> Name;
   std::vector<int> Decision;
   std::vector<int> Prescale;
   std::vector<bool> Exist;
   std::vector<bool> PrescaleExist;
public:
   TriggerTreeMessenger(TFile &File, std::string TreeName = "hltanalysis/HltTree");
   TriggerTreeMessenger(TFile *File, std::string TreeName = "hltanalysis/HltTree");
   TriggerTreeMessenger(TTree *TriggerTree);
   bool Initialize(TTree *TriggerTree);
   bool Initialize();
   bool GetEntry(int iEntry);
   void FillTriggerNames();
   int FindIndex(std::string Trigger);
   int CheckTrigger(std::string Trigger);
   int CheckTriggerStartWith(std::string Trigger);
   int CheckTrigger(int Index);
   int GetPrescale(std::string Trigger);
   int GetPrescaleStartWith(std::string Trigger);
   int GetPrescale(int Index);
};

class TriggerObjectTreeMessenger
{
public:
   TTree *Tree;
   std::vector<double> *ID;
   std::vector<double> *PT;
   std::vector<double> *Eta;
   std::vector<double> *Phi;
   std::vector<double> *Mass;
public:
   TriggerObjectTreeMessenger(TFile &File, std::string TreeName = "hltobjects/HLT_HIPuAK4CaloJet60_Eta5p1_v");
   TriggerObjectTreeMessenger(TFile *File, std::string TreeName = "hltobjects/HLT_HIPuAK4CaloJet60_Eta5p1_v");
   TriggerObjectTreeMessenger(TTree *TriggerTree);
   bool Initialize(TTree *TriggerTree);
   bool Initialize();
   bool GetEntry(int iEntry);
};

class TrackTreeMessenger
{
public:
   TTree *Tree;
   int nVtx;
   int nTrkVtx[VERTEXCOUNTMAX];
   float normChi2Vtx[VERTEXCOUNTMAX];
   float sumPtVtx[VERTEXCOUNTMAX];
   float xVtx[VERTEXCOUNTMAX];
   float yVtx[VERTEXCOUNTMAX];
   float zVtx[VERTEXCOUNTMAX];
   float xVtxErr[VERTEXCOUNTMAX];
   float yVtxErr[VERTEXCOUNTMAX];
   float zVtxErr[VERTEXCOUNTMAX];
   float vtxDist2D[VERTEXCOUNTMAX];
   float vtxDist2DErr[VERTEXCOUNTMAX];
   float vtxDist2DSig[VERTEXCOUNTMAX];
   float vtxDist3D[VERTEXCOUNTMAX];
   float vtxDist3DErr[VERTEXCOUNTMAX];
   float vtxDist3DSig[VERTEXCOUNTMAX];
   int nTrk;
   float trkPt[TRACKCOUNTMAX];
   float trkPtError[TRACKCOUNTMAX];
   short trkNHit[TRACKCOUNTMAX];
   short trkNlayer[TRACKCOUNTMAX];
   float trkEta[TRACKCOUNTMAX];
   float trkPhi[TRACKCOUNTMAX];
   int trkCharge[TRACKCOUNTMAX];
   short trkNVtx[TRACKCOUNTMAX];
   int nTrkTimesnVtx;
   bool trkAssocVtx[TRACKCOUNTMAX];
   float trkDxyOverDxyError[TRACKCOUNTMAX];
   float trkDzOverDzError[TRACKCOUNTMAX];
   bool highPurity[TRACKCOUNTMAX];
   bool tight[TRACKCOUNTMAX];
   bool loose[TRACKCOUNTMAX];
   float trkChi2[TRACKCOUNTMAX];
   short trkNdof[TRACKCOUNTMAX];
   float trkDxy1[TRACKCOUNTMAX];
   float trkDxyError1[TRACKCOUNTMAX];
   float trkDz1[TRACKCOUNTMAX];
   float trkDzError1[TRACKCOUNTMAX];
   bool trkFake[TRACKCOUNTMAX];
   short trkAlgo[TRACKCOUNTMAX];
   short trkOriginalAlgo[TRACKCOUNTMAX];
public:
   TrackTreeMessenger(TFile &File, std::string TreeName = "ppTrack/trackTree");
   TrackTreeMessenger(TFile *File, std::string TreeName = "ppTrack/trackTree");
   TrackTreeMessenger(TTree *TrackTree);
   bool Initialize(TTree *TrackTree);
   bool Initialize();
   bool GetEntry(int iEntry);
   int GetBestVertexIndex();
   bool PassZHadron2022Cut(int index);
   bool PassZHadron2022CutLoose(int index);
   bool PassZHadron2022CutTight(int index);
};

class DfinderMasterMessenger
{
public:
    TTree *Tree;

    // Candidate info
    int   Dsize;
    float Dpt[DFINDERCOUNTMAX];
    float Dphi[DFINDERCOUNTMAX];
    float Dy[DFINDERCOUNTMAX];
    float Dmass[DFINDERCOUNTMAX];
    float Dchi2cl[DFINDERCOUNTMAX];
    float DsvpvDistance[DFINDERCOUNTMAX];
    float DsvpvDisErr[DFINDERCOUNTMAX];
    float DsvpvDistance_2D[DFINDERCOUNTMAX];
    float DsvpvDisErr_2D[DFINDERCOUNTMAX];
    float Dalpha[DFINDERCOUNTMAX];
    float Ddtheta[DFINDERCOUNTMAX];
    // Candidate gen info
    int   Dgen[DFINDERCOUNTMAX];
    float Dgenpt[DFINDERCOUNTMAX];
    int   DgenBAncestorpdgId[DFINDERCOUNTMAX];

    // Candidate daughter track 1
    float Dtrk1P[DFINDERCOUNTMAX];
    float Dtrk1Pt[DFINDERCOUNTMAX];
    float Dtrk1PtErr[DFINDERCOUNTMAX];
    float Dtrk1Eta[DFINDERCOUNTMAX];
    float Dtrk1dedx[DFINDERCOUNTMAX];
    float Dtrk1MassHypo[DFINDERCOUNTMAX];
    float Dtrk1PixelHit[DFINDERCOUNTMAX];
    float Dtrk1StripHit[DFINDERCOUNTMAX];
    float Dtrk1highPurity[DFINDERCOUNTMAX];
    // Candidate daughter track 2
    float Dtrk2P[DFINDERCOUNTMAX];
    float Dtrk2Pt[DFINDERCOUNTMAX];
    float Dtrk2PtErr[DFINDERCOUNTMAX];
    float Dtrk2Eta[DFINDERCOUNTMAX];
    float Dtrk2dedx[DFINDERCOUNTMAX];
    float Dtrk2MassHypo[DFINDERCOUNTMAX];
    float Dtrk2PixelHit[DFINDERCOUNTMAX];
    float Dtrk2StripHit[DFINDERCOUNTMAX];
    float Dtrk2highPurity[DFINDERCOUNTMAX];
    // Candidate daughter track 3
    float Dtrk3P[DFINDERCOUNTMAX];
    float Dtrk3Pt[DFINDERCOUNTMAX];
    float Dtrk3PtErr[DFINDERCOUNTMAX];
    float Dtrk3Eta[DFINDERCOUNTMAX];
    float Dtrk3dedx[DFINDERCOUNTMAX];
    float Dtrk3MassHypo[DFINDERCOUNTMAX];
    float Dtrk3PixelHit[DFINDERCOUNTMAX];
    float Dtrk3StripHit[DFINDERCOUNTMAX];
    float Dtrk3highPurity[DFINDERCOUNTMAX];
    // Candidate daughter track 4
    float Dtrk4P[DFINDERCOUNTMAX];
    float Dtrk4Pt[DFINDERCOUNTMAX];
    float Dtrk4PtErr[DFINDERCOUNTMAX];
    float Dtrk4Eta[DFINDERCOUNTMAX];
    float Dtrk4dedx[DFINDERCOUNTMAX];
    float Dtrk4MassHypo[DFINDERCOUNTMAX];
    float Dtrk4PixelHit[DFINDERCOUNTMAX];
    float Dtrk4StripHit[DFINDERCOUNTMAX];
    float Dtrk4highPurity[DFINDERCOUNTMAX];
    
    // Candidate resonance info
    float DtktkResmass[DFINDERCOUNTMAX];
    float DtktkRespt[DFINDERCOUNTMAX];
    float DtktkReseta[DFINDERCOUNTMAX];
    float DtktkRes_chi2cl[DFINDERCOUNTMAX];
    // Resonance daughter track 1
    float DRestrk1P[DFINDERCOUNTMAX];
    float DRestrk1Pt[DFINDERCOUNTMAX];
    float DRestrk1PtErr[DFINDERCOUNTMAX];
    float DRestrk1Eta[DFINDERCOUNTMAX];
    float DRestrk1dedx[DFINDERCOUNTMAX];
    float DRestrk1MassHypo[DFINDERCOUNTMAX];
    float DRestrk1highPurity[DFINDERCOUNTMAX];
    // Resonance daughter track 2
    float DRestrk2P[DFINDERCOUNTMAX];
    float DRestrk2Pt[DFINDERCOUNTMAX];
    float DRestrk2PtErr[DFINDERCOUNTMAX];
    float DRestrk2Eta[DFINDERCOUNTMAX];
    float DRestrk2dedx[DFINDERCOUNTMAX];
    float DRestrk2MassHypo[DFINDERCOUNTMAX];
    float DRestrk2highPurity[DFINDERCOUNTMAX];
    
public:
    DfinderMasterMessenger(TFile &File, std::string TreeName);
    DfinderMasterMessenger(TFile *File, std::string TreeName);
    DfinderMasterMessenger(TTree *DfinderTree);
    bool Initialize(TTree *DfinderTree);
    bool Initialize();
    bool GetEntry(int iEntry);
};

class DzeroTreeMessenger: public DfinderMasterMessenger {
  public:
    DzeroTreeMessenger(TFile &File, std::string TreeName = "Dfinder/ntDkpi")
      : DfinderMasterMessenger{File, TreeName}
    {
    }
    DzeroTreeMessenger(TFile *File, std::string TreeName = "Dfinder/ntDkpi")
      : DfinderMasterMessenger{File, TreeName}
    {
    }
};

class LambdaCpkpiTreeMessenger: public DfinderMasterMessenger {
  public:
    LambdaCpkpiTreeMessenger(TFile &File, std::string TreeName = "Dfinder/ntLctopkpi")
      : DfinderMasterMessenger{File, TreeName}
    {
    }
    LambdaCpkpiTreeMessenger(TFile *File, std::string TreeName = "Dfinder/ntLctopkpi")
      : DfinderMasterMessenger{File, TreeName}
    {
    }
};

class LambdaCpksTreeMessenger: public DfinderMasterMessenger {
  public:
    LambdaCpksTreeMessenger(TFile &File, std::string TreeName = "Dfinder/ntLctopkstoppipi")
      : DfinderMasterMessenger{File, TreeName}
    {
    }
    LambdaCpksTreeMessenger(TFile *File, std::string TreeName = "Dfinder/ntLctopkstoppipi")
      : DfinderMasterMessenger{File, TreeName}
    {
    }
};

class DfinderGenTreeMessenger
{
public:
    TTree *Tree;
    int Gsize;
    float Gpt[DFINDERGENCOUNTMAX];
    float Gy[DFINDERGENCOUNTMAX];
    float Gphi[DFINDERGENCOUNTMAX];
    int GpdgId[DFINDERGENCOUNTMAX];
    int GisSignal[DFINDERGENCOUNTMAX];
    int GcollisionId[DFINDERGENCOUNTMAX];
    int GSignalType[DFINDERGENCOUNTMAX];
    int GBAncestorpdgId[DFINDERGENCOUNTMAX];

public:
    DfinderGenTreeMessenger(TFile &File, std::string TreeName = "Dfinder/ntGen");
    DfinderGenTreeMessenger(TFile *File, std::string TreeName = "Dfinder/ntGen");
    DfinderGenTreeMessenger(TTree *DfinderGenTree);
    bool Initialize(TTree *DfinderGenTree);
    bool Initialize();
    bool GetEntry(int iEntry);
};

class DzeroGenTreeMessenger: public DfinderGenTreeMessenger {
    DzeroGenTreeMessenger(TFile &File, std::string TreeName = "Dfinder/ntGen")
      : DfinderGenTreeMessenger{File, TreeName}
    {
    }
    DzeroGenTreeMessenger(TFile *File, std::string TreeName = "Dfinder/ntGen")
      : DfinderGenTreeMessenger{File, TreeName}
    {
    }
};

class MuTreeMessenger
{
public:
   TTree *Tree;
   int Run;
   int Event;
   int Lumi;
   float Vx;
   float Vy;
   float Vz;
   int NGen;
   int GenPID[GENCOUNTMAX];
   int GenMom[GENCOUNTMAX];
   int GenStatus[GENCOUNTMAX];
   float GenP[GENCOUNTMAX];
   float GenPT[GENCOUNTMAX];
   float GenEta[GENCOUNTMAX];
   float GenPhi[GENCOUNTMAX];
   int NGlb;
   int GlbCharge[MUMAX];
   float GlbP[MUMAX];
   float GlbPT[MUMAX];
   float GlbEta[MUMAX];
   float GlbPhi[MUMAX];
   float GlbDxy[MUMAX];
   float GlbDz[MUMAX];
   int GlbNValMuHits[MUMAX];
   int GlbNValTrkHits[MUMAX];
   int GlbNValPixHits[MUMAX];
   int GlbTrkLayerWMeas[MUMAX];
   int GlbNMatchedStations[MUMAX];
   int GlbNTrkFound[MUMAX];
   float GlbGlbChi2NDof[MUMAX];
   float GlbTrkChi2NDof[MUMAX];
   int GlbPixLayerWMeas[MUMAX];
   float GlbTrkDxy[MUMAX];
   float GlbTrkDz[MUMAX];
   int NSta;
   int StaCharge[MUMAX];
   float StaP[MUMAX];
   float StaPT[MUMAX];
   float StaEta[MUMAX];
   float StaPhi[MUMAX];
   float StaDxy[MUMAX];
   float StaDz[MUMAX];
   int GlbIsArbitrated[MUMAX];
   int NDi;
   float DiVProb[MUMAX];
   float DiMass[MUMAX];
   float DiE[MUMAX];
   float DiPT[MUMAX];
   float DiPT1[MUMAX];
   float DiPT2[MUMAX];
   float DiEta[MUMAX];
   float DiEta1[MUMAX];
   float DiEta2[MUMAX];
   float DiRapidity[MUMAX];
   float DiPhi[MUMAX];
   float DiPhi1[MUMAX];
   float DiPhi2[MUMAX];
   int DiCharge[MUMAX];
   int DiCharge1[MUMAX];
   int DiCharge2[MUMAX];
   int DiIsArb1[MUMAX];
   int DiIsArb2[MUMAX];
   int DiNTrkHit1[MUMAX];
   int DiNTrkHit2[MUMAX];
   int DiNMuHit1[MUMAX];
   int DiNMuHit2[MUMAX];
   int DiNTrkLayers1[MUMAX];
   int DiNTrkLayers2[MUMAX];
   int DiNPixHit1[MUMAX];
   int DiNPixHit2[MUMAX];
   int DiNMatchedStations1[MUMAX];
   int DiNMatchedStations2[MUMAX];
   float DiTrkChi21[MUMAX];
   float DiTrkChi22[MUMAX];
   float DiGlbChi21[MUMAX];
   float DiGlbChi22[MUMAX];
   float DiDxy1[MUMAX];
   float DiDxy2[MUMAX];
   float DiDz1[MUMAX];
   float DiDz2[MUMAX];
public:
   MuTreeMessenger(TFile &File, std::string TreeName = "hltMuTree/HLTMuTree");
   MuTreeMessenger(TFile *File, std::string TreeName = "hltMuTree/HLTMuTree");
   MuTreeMessenger(TTree *MuTree);
   bool Initialize(TTree *MuTree);
   bool Initialize();
   bool GetEntry(int iEntry);
   bool DimuonPassTightCut(int index);
};

class SingleMuTreeMessenger
{
public:
   TTree *Tree;
   std::vector<float> *SingleMuPT;
   std::vector<float> *SingleMuEta;
   std::vector<float> *SingleMuPhi;
   std::vector<float> *SingleMuDxy;
   std::vector<float> *SingleMuDxyError;
   std::vector<float> *SingleMuDz;
   std::vector<float> *SingleMuDzError;
   std::vector<int> *SingleMuCharge;
   std::vector<bool> *SingleMuIsGood;
   std::vector<bool> *SingleMuIsGlobal;
   std::vector<bool> *SingleMuIsTracker;
   std::vector<bool> *SingleMuHybridSoft;
   std::vector<bool> *SingleMuSoft;

   std::vector<float> *GenSingleMuPT;
   std::vector<float> *GenSingleMuEta;
   std::vector<float> *GenSingleMuPhi;
   std::vector<int> *GenSingleMuPID;

public:
   SingleMuTreeMessenger(TFile &File, std::string TreeName = "muonAnalyzer/MuonTree");
   SingleMuTreeMessenger(TFile *File, std::string TreeName = "muonAnalyzer/MuonTree");
   SingleMuTreeMessenger(TTree *SingleMuTree);
   bool Initialize(TTree *SingleMuTree);
   bool Initialize();
   bool GetEntry(int iEntry);
};

class PbPbTrackTreeMessenger
{
public:
   TTree *Tree;
   int Run;
   int Event;
   int Lumi;
   std::vector<float> *VX;
   std::vector<float> *VY;
   std::vector<float> *VZ;
   std::vector<float> *VXError;
   std::vector<float> *VYError;
   std::vector<float> *VZError;
   std::vector<float> *VChi2;
   std::vector<float> *VNDof;
   std::vector<bool>  *VIsFake;
   std::vector<int>   *VNTracks;
   std::vector<float> *VPTSum;
   std::vector<float> *TrackPT;
   std::vector<float> *TrackPTError;
   std::vector<float> *TrackEta;
   std::vector<float> *TrackPhi;
   std::vector<char>  *TrackCharge;
   std::vector<int>   *TrackPDGId;
   std::vector<char>  *TrackNHits;
   std::vector<char>  *TrackNPixHits;
   std::vector<char>  *TrackNLayers;
   std::vector<float> *TrackNormChi2;
   std::vector<bool>  *TrackHighPurity;
   std::vector<float> *PFEnergy;
   std::vector<float> *PFEcal;
   std::vector<float> *PFHcal;
   std::vector<int>   *TrackAssociatedVertexIndex;
   std::vector<int>   *TrackAssociatedVertexQuality;
   std::vector<float> *TrackAssociatedVertexDz;
   std::vector<float> *TrackAssociatedVertexDzError;
   std::vector<float> *TrackAssociatedVertexDxy;
   std::vector<float> *TrackAssociatedVertexDxyError;
   std::vector<int>   *TrackFirstVertexQuality;
   std::vector<float> *TrackFirstVertexDz;
   std::vector<float> *TrackFirstVertexDzError;
   std::vector<float> *TrackFirstVertexDxy;
   std::vector<float> *TrackFirstVertexDxyError;
public:
   PbPbTrackTreeMessenger(TFile &File, std::string TreeName = "PbPbTracks/trackTree");
   PbPbTrackTreeMessenger(TFile *File, std::string TreeName = "PbPbTracks/trackTree");
   PbPbTrackTreeMessenger(TTree *PbPbTrackTree);
   bool Initialize(TTree *PbPbTrackTree);
   bool Initialize();
   bool GetEntry(int iEntry);
   bool PassZHadron2022Cut(int index);
   bool PassZHadron2022CutNoVZ(int index);
   bool PassZHadron2022CutLoose(int index);
   bool PassZHadron2022CutTight(int index);
};

class PbPbUPCTrackTreeMessenger
{
public:
   TTree *Tree;
   int nVtx;
   int nTrk;
   std::vector<float> *ptSumVtx;
   std::vector<float> *xVtx;
   std::vector<float> *yVtx;
   std::vector<float> *zVtx;
   std::vector<float> *xErrVtx;
   std::vector<float> *yErrVtx;
   std::vector<float> *zErrVtx;
   std::vector<float> *trkPt;
   std::vector<float> *trkEta;
   std::vector<float> *trkPhi;
   std::vector<float> *highPurity;
   std::vector<float> *trkNormChi2;
   std::vector<char>  *trkNLayers;
   std::vector<float> *trkDzFirstVtx;
   std::vector<float> *trkDzErrFirstVtx; 
   std::vector<float> *trkDxyFirstVtx;
   std::vector<float> *trkDxyErrFirstVtx;
   std::vector<char>  *trkNHits;
   std::vector<float> *trkPtError;
   std::vector<float> *PFEnergy;


public:
   PbPbUPCTrackTreeMessenger(TFile &File, std::string TreeName = "ppTracks/trackTree");
   PbPbUPCTrackTreeMessenger(TFile *File, std::string TreeName = "ppTracks/trackTree");
   PbPbUPCTrackTreeMessenger(TTree *PbPbUPCTrackTree);
   bool Initialize(TTree *PbPbUPCTrackTree);
   bool Initialize();
   bool GetEntry(int iEntry);
};

class PPTrackTreeMessenger
{
public:
   TTree *Tree;
   int nVtx;
   int nTrk;
   std::vector<float> *ptSumVtx;
   std::vector<float> *xVtx;
   std::vector<float> *yVtx;
   std::vector<float> *zVtx;
   std::vector<float> *xErrVtx;
   std::vector<float> *yErrVtx;
   std::vector<float> *zErrVtx;
   std::vector<bool> *isFakeVtx;
   std::vector<int> *nTracksVtx;
   std::vector<float> *chi2Vtx;
   std::vector<float> *ndofVtx;
   std::vector<float> *trkPt;
   std::vector<float> *trkPhi;
   std::vector<float> *trkPtError;
   std::vector<float> *trkEta;
   std::vector<bool> *highPurity;
   std::vector<float> *trkDxyAssociatedVtx;
   std::vector<float> *trkDzAssociatedVtx;
   std::vector<float> *trkDxyErrAssociatedVtx;
   std::vector<float> *trkDzErrAssociatedVtx;
   std::vector<int> *trkAssociatedVtxIndx;
   std::vector<char> *trkCharge;
   std::vector<char> *trkNHits;
   std::vector<char> *trkNPixHits;
   std::vector<char> *trkNLayers;
   std::vector<float> *trkNormChi2;
   std::vector<float> *pfEnergy;


public:
   PPTrackTreeMessenger(TFile &File, std::string TreeName = "ppTracks/trackTree");
   PPTrackTreeMessenger(TFile *File, std::string TreeName = "ppTracks/trackTree");
   PPTrackTreeMessenger(TTree *PPTrackTree);
   bool Initialize(TTree *PPTrackTree);
   bool Initialize();
   bool GetEntry(int iEntry);
   bool PassChargedHadronPPStandardCuts(int index);
   bool PassChargedHadronPPOONeNe2025StandardCuts(int index);
   bool PassChargedHadronPPOONeNe2025LooseCuts(int index);
   bool PassChargedHadronPPOONeNe2025TightCuts(int index);
};

class ZDCTreeMessenger
{
public:
   TTree *Tree;
   float sumPlus, sumMinus;

public:
   ZDCTreeMessenger(TFile &File, std::string TreeName = "zdcanalyzer/zdcdigi");
   ZDCTreeMessenger(TFile *File, std::string TreeName = "zdcanalyzer/zdcdigi");
   ZDCTreeMessenger(TTree *ZDCTree);
   bool Initialize(TTree *ZDCTree);
   bool Initialize();
   bool GetEntry(int iEntry);
};

class PPSTreeMessenger
{
public:
   TTree *Tree;
   int n;
   int zside[PPSMAXN];
   int station[PPSMAXN];
   float x[PPSMAXN];
   float y[PPSMAXN];

public:
   PPSTreeMessenger(TFile &File, std::string TreeName = "ppsanalyzer/ppstracks");
   PPSTreeMessenger(TFile *File, std::string TreeName = "ppsanalyzer/ppstracks");
   PPSTreeMessenger(TTree *PPSTree);
   bool Initialize(TTree *PPSTree);
   bool Initialize();
   bool GetEntry(int iEntry);
};

class FSCTreeMessenger
{
public:
   TTree *Tree;
   int n;
   int zside[FSCMAXN];
   int section[FSCMAXN];
   int channel[FSCMAXN];

   int adcTs0[FSCMAXN];
   int adcTs1[FSCMAXN];
   int adcTs2[FSCMAXN];
   int adcTs3[FSCMAXN];
   int adcTs4[FSCMAXN];
   int adcTs5[FSCMAXN];

   float chargefCTs0[FSCMAXN];
   float chargefCTs1[FSCMAXN];
   float chargefCTs2[FSCMAXN];
   float chargefCTs3[FSCMAXN];
   float chargefCTs4[FSCMAXN];
   float chargefCTs5[FSCMAXN];

   int tdcTs0[FSCMAXN];
   int tdcTs1[FSCMAXN];
   int tdcTs2[FSCMAXN];
   int tdcTs3[FSCMAXN];
   int tdcTs4[FSCMAXN];
   int tdcTs5[FSCMAXN];

public:
   FSCTreeMessenger(TFile &File, std::string TreeName = "fscanalyzer/fscdigi");
   FSCTreeMessenger(TFile *File, std::string TreeName = "fscanalyzer/fscdigi");
   FSCTreeMessenger(TTree *FSCTree);
   bool Initialize(TTree *FSCTree);
   bool Initialize();
   bool GetEntry(int iEntry);
};

class HFAdcMessenger
{
public:
   TTree *Tree;
   int mMaxL1HFAdcPlus, mMaxL1HFAdcMinus;

public:
   HFAdcMessenger(TFile &File, std::string TreeName = "HFAdcana/adc");
   HFAdcMessenger(TFile *File, std::string TreeName = "HFAdcana/adc");
   HFAdcMessenger(TTree *HFAdcTree);
   bool Initialize(TTree *HFAdcTree);
   bool Initialize();
   bool GetEntry(int iEntry);
};

class ZHadronMessenger
{
public:
   TTree *Tree;
   int Run;
   long long Event;
   int Lumi;

   int hiBin;
   int hiBinUp;
   int hiBinDown;
   float hiHF;

   float SignalHF;
   float BackgroundHF;
   float SubEvent0HF;
   float SubEventAllHF;
   float SignalVZ;

   float EventWeight;
   float NCollWeight;
   float InterSampleZWeight;
   float ZWeight;
   float VZWeight;
   float ExtraZWeight[12];

   int NVertex;
   float VX, VY, VZ, VXError, VYError, VZError;
   int NPU;

   std::vector<float> *zMass;
   std::vector<float> *zEta;
   std::vector<float> *zY;
   std::vector<float> *zPhi;
   std::vector<float> *zPt;
   std::vector<float> *genZMass;
   std::vector<float> *genZEta;
   std::vector<float> *genZY;
   std::vector<float> *genZPhi;
   std::vector<float> *genZPt;
   std::vector<float> *trackPt;
   std::vector<float> *trackPDFId;
   std::vector<float> *trackEta;
   std::vector<float> *trackY;
   std::vector<float> *trackPhi;
   std::vector<bool> *trackMuTagged;
   std::vector<float> *trackMuDR;
   std::vector<float> *trackWeight;
   std::vector<float> *trackResidualWeight;
   std::vector<int> *trackCharge;
   std::vector<int> *subevent;

   std::vector<float> *muEta1;
   std::vector<float> *muEta2;
   std::vector<float> *muPhi1;
   std::vector<float> *muPhi2;
   std::vector<float> *muPt1;
   std::vector<float> *muPt2;

   std::vector<float> *muDeta;
   std::vector<float> *muDphi;
   std::vector<float> *muDR;
   std::vector<float> *muDphiS;

   std::vector<float> *genMuPt1;
   std::vector<float> *genMuPt2;
   std::vector<float> *genMuEta1;
   std::vector<float> *genMuEta2;
   std::vector<float> *genMuPhi1;
   std::vector<float> *genMuPhi2;

   std::vector<float> *genMuDeta;
   std::vector<float> *genMuDphi;
   std::vector<float> *genMuDR;
   std::vector<float> *genMuDphiS;

public:   // Derived quantities
   bool GoodGenZ;
   bool GoodRecoZ;

private:
   bool WriteMode;
   bool Initialized;

public:
   ZHadronMessenger(TFile &File, std::string TreeName = "tree", bool SkipTrack = false);
   ZHadronMessenger(TFile *File, std::string TreeName = "tree", bool SkipTrack = false);
   ZHadronMessenger(TTree *ZHadronTree = nullptr, bool SkipTrack = false);
   ~ZHadronMessenger();
   bool Initialize(TTree *ZHadronTree, bool SkipTrack = false);
   bool Initialize(bool SkipTrack = false);
   int GetEntries();
   bool GetEntry(int iEntry);
   bool SetBranch(TTree *T);
   void Clear();
   void CopyNonTrack(ZHadronMessenger &M);
   bool FillEntry();
};

class DzeroUPCTreeMessenger
{
public:
   TTree *Tree;
   int Run;
   long long Event;
   int Lumi;
   //FIXME: these refer to best vertex positions calculated from the track tree
   float VX, VY, VZ, VXError, VYError, VZError; //best vertex from track tree
   int nVtx;
   bool isL1ZDCOr, isL1ZDCXORJet8, isL1ZDCXORJet12, isL1ZDCXORJet16;
   bool isZeroBias;
   bool isL1ZDCOr_Min400, isL1ZDCOr_Max400;
   bool isZeroBias_Min400, isZeroBias_Max400;
   bool selectedBkgFilter, selectedVtxFilter;
   float ZDCsumPlus;
   float ZDCsumMinus;
   float HFEMaxPlus;
   float HFEMaxMinus;
   //booleans
   bool ZDCgammaN, ZDCNgamma;
   bool gapgammaN, gapNgamma;
   std::vector<bool>  *gammaN;
   std::vector<bool>  *Ngamma;
   int nTrackInAcceptanceHP;
   //D reco quantities
   int Dsize;
   std::vector<float> *Dpt;
   std::vector<float> *Dphi;
   std::vector<float> *Dy;
   std::vector<float> *Dmass;
   std::vector<float> *Dtrk1P;
   std::vector<float> *Dtrk1Pt;
   std::vector<float> *Dtrk1PtErr;
   std::vector<float> *Dtrk1Eta;
   std::vector<float> *Dtrk1dedx;
   std::vector<float> *Dtrk1MassHypo;
   std::vector<float> *Dtrk1PixelHit;
   std::vector<float> *Dtrk1StripHit;
   std::vector<float> *Dtrk1PionScore;
   std::vector<float> *Dtrk1KaonScore;
   std::vector<float> *Dtrk1ProtScore;
   std::vector<float> *Dtrk2P;
   std::vector<float> *Dtrk2Pt;
   std::vector<float> *Dtrk2PtErr;
   std::vector<float> *Dtrk2Eta;
   std::vector<float> *Dtrk2dedx;
   std::vector<float> *Dtrk2MassHypo;
   std::vector<float> *Dtrk2PixelHit;
   std::vector<float> *Dtrk2StripHit;
   std::vector<float> *Dtrk2PionScore;
   std::vector<float> *Dtrk2KaonScore;
   std::vector<float> *Dtrk2ProtScore;
   std::vector<float> *Dchi2cl;
   std::vector<float> *DsvpvDistance;
   std::vector<float> *DsvpvDisErr;
   std::vector<float> *DsvpvDistance_2D;
   std::vector<float> *DsvpvDisErr_2D;
   std::vector<float> *Dip3d;
   std::vector<float> *Dip3derr;
   std::vector<float> *Dalpha;
   std::vector<float> *Ddtheta;
   std::vector<bool>  *DpassCut23PAS;
   std::vector<bool>  *DpassCut23LowPt;
   std::vector<bool>  *DpassCut23PASSystDsvpvSig;
   std::vector<bool>  *DpassCut23PASSystDtrkPt;
   std::vector<bool>  *DpassCut23PASSystDalpha;
   std::vector<bool>  *DpassCut23PASSystDchi2cl;
   std::vector<bool>  *DpassCutNominal;
   std::vector<bool>  *DpassCutLoose;
   std::vector<bool>  *DpassCutSystDsvpvSig;
   std::vector<bool>  *DpassCutSystDtrkPt;
   std::vector<bool>  *DpassCutSystDalpha;
   std::vector<bool>  *DpassCutSystDdtheta;
   std::vector<bool>  *DpassCutSystDalphaDdtheta;
   std::vector<bool>  *DpassCutSystDchi2cl;
   std::vector<int>   *Dgen;
   std::vector<bool>  *DisSignalCalc;
   std::vector<bool>  *DisSignalCalcPrompt;
   std::vector<bool>  *DisSignalCalcFeeddown;
   //MC only quantities
   int Gsize;
   std::vector<float> *Gpt;
   std::vector<float> *Gy;
   std::vector<bool>  *GisSignalCalc;
   std::vector<bool>  *GisSignalCalcPrompt;
   std::vector<bool>  *GisSignalCalcFeeddown;

   ///////////////////
   // Defining the rapidity gap energy threshold array for the systematics study -- 1
   // [Change accordingly] function gammaN_EThresh*()
   ///////////////////
   const int N_gapEThresh = 9;
   // from tight to loose
   const std::vector<float> gapEThresh_gammaN = {4.3, 5.5, 6.4, 7.8, 9.2, 10.6, 12.5, 15.0, 16.2};
   const std::vector<float> gapEThresh_Ngamma = {4.5, 5.5, 6.5, 7.6, 8.6, 10.0, 12.0, 15.0, 16.0};

public:   // Derived quantities
   bool GoodPhotonuclear; //FIXME: currently not implemented

private:
   bool WriteMode;
   bool Initialized;

public:
   DzeroUPCTreeMessenger(TFile &File, std::string TreeName = "tree", bool Debug = false);
   DzeroUPCTreeMessenger(TFile *File, std::string TreeName = "tree", bool Debug = false);
   DzeroUPCTreeMessenger(TTree *DzeroUPCTree = nullptr, bool Debug = false);
   ~DzeroUPCTreeMessenger();
   bool Initialize(TTree *DzeroUPCTree, bool Debug = false);
   bool Initialize(bool Debug = false);
   int GetEntries();
   bool GetEntry(int iEntry);
   bool SetBranch(TTree *T);
   void Clear();
   void CopyNonTrack(DzeroUPCTreeMessenger &M);
   bool FillEntry();

   ///////////////////
   // Utility functions to examine passing a specific rapidity gap energy threshold -- 2
   // [Change accordingly] the declaration of gapEThresh_*
   ///////////////////
   bool gammaN_EThreshTight()   { if (this->gammaN->size()!=N_gapEThresh) return false; return this->gammaN->at(0); }
   bool gammaN_EThreshLoose()   { if (this->gammaN->size()!=N_gapEThresh) return false; return this->gammaN->at(N_gapEThresh-1); }
   bool gammaN_EThreshNominal() { if (this->gammaN->size()!=N_gapEThresh) return false; return this->gammaN->at(N_gapEThresh/2); }
   bool gammaN_EThreshSyst5p5() { if (this->gammaN->size()!=N_gapEThresh) return false; return this->gammaN->at(1); }
   bool gammaN_EThreshSyst15()  { if (this->gammaN->size()!=N_gapEThresh) return false; return this->gammaN->at(7); }
   bool gammaN_EThreshCustom(float threshold)  { return ( this->ZDCgammaN && this->HFEMaxPlus <= threshold ); }

   bool Ngamma_EThreshTight()   { if (this->Ngamma->size()!=N_gapEThresh) return false; return this->Ngamma->at(0); }
   bool Ngamma_EThreshLoose()   { if (this->Ngamma->size()!=N_gapEThresh) return false; return this->Ngamma->at(N_gapEThresh-1); }
   bool Ngamma_EThreshNominal() { if (this->Ngamma->size()!=N_gapEThresh) return false; return this->Ngamma->at(N_gapEThresh/2); }
   bool Ngamma_EThreshSyst5p5() { if (this->Ngamma->size()!=N_gapEThresh) return false; return this->Ngamma->at(1); }
   bool Ngamma_EThreshSyst15()  { if (this->Ngamma->size()!=N_gapEThresh) return false; return this->Ngamma->at(7); }
   bool Ngamma_EThreshCustom(float threshold)  { return ( this->ZDCNgamma && this->HFEMaxMinus <= threshold ); }

};

class LambdaCpksUPCTreeMessenger
{
public:
  TTree *Tree;
  int Run;
  long long Event;
  int Lumi;
  //FIXME: these refer to best vertex positions calculated from the track tree
  float VX, VY, VZ, VXError, VYError, VZError; //best vertex from track tree
  int nVtx;
  bool isL1ZDCOr, isL1ZDCXORJet8, isL1ZDCXORJet12, isL1ZDCXORJet16;
  bool isZeroBias;
  bool isL1ZDCOr_Min400, isL1ZDCOr_Max400;
  bool isZeroBias_Min400, isZeroBias_Max400;
  bool selectedBkgFilter, selectedVtxFilter;
  float ZDCsumPlus;
  float ZDCsumMinus;
  float HFEMaxPlus;
  float HFEMaxMinus;
  //booleans
  bool ZDCgammaN, ZDCNgamma;
  bool gapgammaN, gapNgamma;
  std::vector<bool>  *gammaN;
  std::vector<bool>  *Ngamma;
  int nTrackInAcceptanceHP;

  //LambdaC reco quantities
  int Dsize;
  std::vector<float> *Dpt;
  std::vector<float> *Dphi;
  std::vector<float> *Dy;
  std::vector<float> *Dmass;
  std::vector<float> *Dchi2cl;
  std::vector<float> *DsvpvDistance;
  std::vector<float> *DsvpvDisErr;
  std::vector<float> *DsvpvDistance_2D;
  std::vector<float> *DsvpvDisErr_2D;
  std::vector<float> *Dalpha;
  std::vector<float> *Ddtheta;
  std::vector<float> *DtktkResmass;
  std::vector<bool>  *DpassCutNominal;
  std::vector<bool>  *DpassCutLoose;
  std::vector<bool>  *DpassCutSystDsvpvSig;
  std::vector<bool>  *DpassCutSystDtrkPt;
  std::vector<bool>  *DpassCutSystDalpha;
  std::vector<bool>  *DpassCutSystDdtheta;
  std::vector<bool>  *DpassCutSystDchi2cl;
  std::vector<int>   *Dgen;
  std::vector<bool>  *DisSignalCalc;
  std::vector<bool>  *DisSignalCalcPrompt;
  std::vector<bool>  *DisSignalCalcFeeddown;

  std::vector<float> *Dtrk2P;
  std::vector<float> *Dtrk2Pt;
  std::vector<float> *Dtrk2PtErr;
  std::vector<float> *Dtrk2Eta;
  std::vector<float> *Dtrk2dedx;
  std::vector<float> *Dtrk2MassHypo;
  std::vector<float> *Dtrk2PixelHit;
  std::vector<float> *Dtrk2StripHit;
  std::vector<float> *Dtrk2PionScore;
  std::vector<float> *Dtrk2KaonScore;
  std::vector<float> *Dtrk2ProtScore;

  std::vector<float> *DRestrk1P;
  std::vector<float> *DRestrk1Pt;
  std::vector<float> *DRestrk1PtErr;
  std::vector<float> *DRestrk1Eta;
  std::vector<float> *DRestrk1dedx;
  std::vector<float> *DRestrk1MassHypo;
  std::vector<float> *DRestrk1PionScore;
  std::vector<float> *DRestrk1KaonScore;
  std::vector<float> *DRestrk1ProtScore;

  std::vector<float> *DRestrk2P;
  std::vector<float> *DRestrk2Pt;
  std::vector<float> *DRestrk2PtErr;
  std::vector<float> *DRestrk2Eta;
  std::vector<float> *DRestrk2dedx;
  std::vector<float> *DRestrk2MassHypo;
  std::vector<float> *DRestrk2PionScore;
  std::vector<float> *DRestrk2KaonScore;
  std::vector<float> *DRestrk2ProtScore;

  //MC only quantities
  int Gsize;
  std::vector<float> *Gpt;
  std::vector<float> *Gy;
  std::vector<bool>  *GisSignalCalc;
  std::vector<bool>  *GisSignalCalcPrompt;
  std::vector<bool>  *GisSignalCalcFeeddown;

  ///////////////////
  // Defining the rapidity gap energy threshold array for the systematics study -- 1
  // [Change accordingly] function gammaN_EThresh*()
  ///////////////////
  const int N_gapEThresh = 9;
  // from tight to loose
  const std::vector<float> gapEThresh_gammaN = {4.3, 5.5, 6.4, 7.8, 9.2, 10.6, 12.5, 15.0, 16.2};
  const std::vector<float> gapEThresh_Ngamma = {4.5, 5.5, 6.5, 7.6, 8.6, 10.0, 12.0, 15.0, 16.0};

public:   // Derived quantities
  bool GoodPhotonuclear; //FIXME: currently not implemented

private:
  bool WriteMode;
  bool Initialized;

public:
  LambdaCpksUPCTreeMessenger(TFile &File, std::string TreeName = "tree", bool Debug = false);
  LambdaCpksUPCTreeMessenger(TFile *File, std::string TreeName = "tree", bool Debug = false);
  LambdaCpksUPCTreeMessenger(TTree *LambdaCpksUPCTree = nullptr, bool Debug = false);
  ~LambdaCpksUPCTreeMessenger();
  bool Initialize(TTree *LambdaCpksUPCTree, bool Debug = false);
  bool Initialize(bool Debug = false);
  int GetEntries();
  bool GetEntry(int iEntry);
  bool SetBranch(TTree *T);
  void Clear();
  void CopyNonTrack(LambdaCpksUPCTreeMessenger &M);
  bool FillEntry();

  ///////////////////
  // Utility functions to examine passing a specific rapidity gap energy threshold -- 2
  // [Change accordingly] the declaration of gapEThresh_*
  ///////////////////
  bool gammaN_EThreshTight()   { if (this->gammaN->size()!=N_gapEThresh) return false; return this->gammaN->at(0); }
  bool gammaN_EThreshLoose()   { if (this->gammaN->size()!=N_gapEThresh) return false; return this->gammaN->at(N_gapEThresh-1); }
  bool gammaN_EThreshNominal() { if (this->gammaN->size()!=N_gapEThresh) return false; return this->gammaN->at(N_gapEThresh/2); }
  bool gammaN_EThreshSyst5p5() { if (this->gammaN->size()!=N_gapEThresh) return false; return this->gammaN->at(1); }
  bool gammaN_EThreshSyst15()  { if (this->gammaN->size()!=N_gapEThresh) return false; return this->gammaN->at(7); }
  bool gammaN_EThreshCustom(float threshold)  { return ( this->ZDCgammaN && this->HFEMaxPlus <= threshold ); }

  bool Ngamma_EThreshTight()   { if (this->Ngamma->size()!=N_gapEThresh) return false; return this->Ngamma->at(0); }
  bool Ngamma_EThreshLoose()   { if (this->Ngamma->size()!=N_gapEThresh) return false; return this->Ngamma->at(N_gapEThresh-1); }
  bool Ngamma_EThreshNominal() { if (this->Ngamma->size()!=N_gapEThresh) return false; return this->Ngamma->at(N_gapEThresh/2); }
  bool Ngamma_EThreshSyst5p5() { if (this->Ngamma->size()!=N_gapEThresh) return false; return this->Ngamma->at(1); }
  bool Ngamma_EThreshSyst15()  { if (this->Ngamma->size()!=N_gapEThresh) return false; return this->Ngamma->at(7); }
  bool Ngamma_EThreshCustom(float threshold)  { return ( this->ZDCNgamma && this->HFEMaxMinus <= threshold ); }
};

class LambdaCpkpiUPCTreeMessenger
{
public:
  TTree *Tree;
  int Run;
  long long Event;
  int Lumi;
  //FIXME: these refer to best vertex positions calculated from the track tree
  float VX, VY, VZ, VXError, VYError, VZError; //best vertex from track tree
  int nVtx;
  bool isL1ZDCOr, isL1ZDCXORJet8, isL1ZDCXORJet12, isL1ZDCXORJet16;
  bool isZeroBias;
  bool isL1ZDCOr_Min400, isL1ZDCOr_Max400;
  bool isZeroBias_Min400, isZeroBias_Max400;
  bool selectedBkgFilter, selectedVtxFilter;
  float ZDCsumPlus;
  float ZDCsumMinus;
  float HFEMaxPlus;
  float HFEMaxMinus;
  //booleans
  bool ZDCgammaN, ZDCNgamma;
  bool gapgammaN, gapNgamma;
  std::vector<bool>  *gammaN;
  std::vector<bool>  *Ngamma;
  int nTrackInAcceptanceHP;

  //LambdaC reco quantities
  int Dsize;
  std::vector<float> *Dpt;
  std::vector<float> *Dphi;
  std::vector<float> *Dy;
  std::vector<float> *Dmass;
  std::vector<float> *Dchi2cl;
  std::vector<float> *DsvpvDistance;
  std::vector<float> *DsvpvDisErr;
  std::vector<float> *DsvpvDistance_2D;
  std::vector<float> *DsvpvDisErr_2D;
  std::vector<float> *Dalpha;
  std::vector<float> *Ddtheta;
  std::vector<bool>  *DpassCutNominal;
  std::vector<bool>  *DpassCutLoose;
  std::vector<bool>  *DpassCutSystDsvpvSig;
  std::vector<bool>  *DpassCutSystDtrkPt;
  std::vector<bool>  *DpassCutSystDalpha;
  std::vector<bool>  *DpassCutSystDdtheta;
  std::vector<bool>  *DpassCutSystDchi2cl;
  std::vector<int>   *Dgen;
  std::vector<bool>  *DisSignalCalc;
  std::vector<bool>  *DisSignalCalcPrompt;
  std::vector<bool>  *DisSignalCalcFeeddown;

  std::vector<float> *Dtrk1P;
  std::vector<float> *Dtrk1Pt;
  std::vector<float> *Dtrk1PtErr;
  std::vector<float> *Dtrk1Eta;
  std::vector<float> *Dtrk1dedx;
  std::vector<float> *Dtrk1MassHypo;
  std::vector<float> *Dtrk1PixelHit;
  std::vector<float> *Dtrk1StripHit;
  std::vector<float> *Dtrk1PionScore;
  std::vector<float> *Dtrk1KaonScore;
  std::vector<float> *Dtrk1ProtScore;

  std::vector<float> *Dtrk2P;
  std::vector<float> *Dtrk2Pt;
  std::vector<float> *Dtrk2PtErr;
  std::vector<float> *Dtrk2Eta;
  std::vector<float> *Dtrk2dedx;
  std::vector<float> *Dtrk2MassHypo;
  std::vector<float> *Dtrk2PixelHit;
  std::vector<float> *Dtrk2StripHit;
  std::vector<float> *Dtrk2PionScore;
  std::vector<float> *Dtrk2KaonScore;
  std::vector<float> *Dtrk2ProtScore;

  std::vector<float> *Dtrk3P;
  std::vector<float> *Dtrk3Pt;
  std::vector<float> *Dtrk3PtErr;
  std::vector<float> *Dtrk3Eta;
  std::vector<float> *Dtrk3dedx;
  std::vector<float> *Dtrk3MassHypo;
  std::vector<float> *Dtrk3PixelHit;
  std::vector<float> *Dtrk3StripHit;
  std::vector<float> *Dtrk3PionScore;
  std::vector<float> *Dtrk3KaonScore;
  std::vector<float> *Dtrk3ProtScore;

  //MC only quantities
  int Gsize;
  std::vector<float> *Gpt;
  std::vector<float> *Gy;
  std::vector<bool>  *GisSignalCalc;
  std::vector<bool>  *GisSignalCalcPrompt;
  std::vector<bool>  *GisSignalCalcFeeddown;

  ///////////////////
  // Defining the rapidity gap energy threshold array for the systematics study -- 1
  // [Change accordingly] function gammaN_EThresh*()
  ///////////////////
  const int N_gapEThresh = 9;
  // from tight to loose
  const std::vector<float> gapEThresh_gammaN = {4.3, 5.5, 6.4, 7.8, 9.2, 10.6, 12.5, 15.0, 16.2};
  const std::vector<float> gapEThresh_Ngamma = {4.5, 5.5, 6.5, 7.6, 8.6, 10.0, 12.0, 15.0, 16.0};

public:   // Derived quantities
  bool GoodPhotonuclear; //FIXME: currently not implemented

private:
  bool WriteMode;
  bool Initialized;

public:
  LambdaCpkpiUPCTreeMessenger(TFile &File, std::string TreeName = "tree", bool Debug = false);
  LambdaCpkpiUPCTreeMessenger(TFile *File, std::string TreeName = "tree", bool Debug = false);
  LambdaCpkpiUPCTreeMessenger(TTree *LambdaCpkpiUPCTree = nullptr, bool Debug = false);
  ~LambdaCpkpiUPCTreeMessenger();
  bool Initialize(TTree *LambdaCpkpiUPCTree, bool Debug = false);
  bool Initialize(bool Debug = false);
  int GetEntries();
  bool GetEntry(int iEntry);
  bool SetBranch(TTree *T);
  void Clear();
  void CopyNonTrack(LambdaCpkpiUPCTreeMessenger &M);
  bool FillEntry();

  ///////////////////
  // Utility functions to examine passing a specific rapidity gap energy threshold -- 2
  // [Change accordingly] the declaration of gapEThresh_*
  ///////////////////
  bool gammaN_EThreshTight()   { if (this->gammaN->size()!=N_gapEThresh) return false; return this->gammaN->at(0); }
  bool gammaN_EThreshLoose()   { if (this->gammaN->size()!=N_gapEThresh) return false; return this->gammaN->at(N_gapEThresh-1); }
  bool gammaN_EThreshNominal() { if (this->gammaN->size()!=N_gapEThresh) return false; return this->gammaN->at(N_gapEThresh/2); }
  bool gammaN_EThreshSyst5p5() { if (this->gammaN->size()!=N_gapEThresh) return false; return this->gammaN->at(1); }
  bool gammaN_EThreshSyst15()  { if (this->gammaN->size()!=N_gapEThresh) return false; return this->gammaN->at(7); }
  bool gammaN_EThreshCustom(float threshold)  { return ( this->ZDCgammaN && this->HFEMaxPlus <= threshold ); }

  bool Ngamma_EThreshTight()   { if (this->Ngamma->size()!=N_gapEThresh) return false; return this->Ngamma->at(0); }
  bool Ngamma_EThreshLoose()   { if (this->Ngamma->size()!=N_gapEThresh) return false; return this->Ngamma->at(N_gapEThresh-1); }
  bool Ngamma_EThreshNominal() { if (this->Ngamma->size()!=N_gapEThresh) return false; return this->Ngamma->at(N_gapEThresh/2); }
  bool Ngamma_EThreshSyst5p5() { if (this->Ngamma->size()!=N_gapEThresh) return false; return this->Ngamma->at(1); }
  bool Ngamma_EThreshSyst15()  { if (this->Ngamma->size()!=N_gapEThresh) return false; return this->Ngamma->at(7); }
  bool Ngamma_EThreshCustom(float threshold)  { return ( this->ZDCNgamma && this->HFEMaxMinus <= threshold ); }
};

class ChargedHadronRAATreeMessenger
{
public:
   TTree *Tree;
   int Run;
   long long Event;
   int Lumi;
   int hiBin;
   float VX, VY, VZ, VXError, VYError, VZError; //best vertex from track tree
   bool isFakeVtx;                              //best vertex from track tree
   int nTracksVtx, bestVtxIndx;                 //best vertex from track tree
   float chi2Vtx, ndofVtx;                      //best vertex from track tree
   float ptSumVtx;
   int nVtx;
   int nTrk, multiplicityEta2p4, multiplicityEta1p0; // different kinds of multiplicity definitions 
   float HFEMaxPlus;
   float HFEMaxPlus2;
   float HFEMaxPlus3;
   float HFEMaxMinus;
   float HFEMaxMinus2;
   float HFEMaxMinus3;
   float ZDCsumPlus;
   float ZDCsumMinus;
   int ClusterCompatibilityFilter;
   int PVFilter;
   int mMaxL1HFAdcPlus, mMaxL1HFAdcMinus;
   float hiHF_pf, hiHFPlus_pf, hiHFMinus_pf;
   float Npart;
   float Ncoll;
   float leadingPtEta1p0_sel;
   int sampleType;
   float VZ_pf;
   float eventEfficiencyWeight_Nominal;
   float eventEfficiencyWeight_Loose;
   float eventEfficiencyWeight_Tight;
   float MC_VZReweight;
   float MC_MultReweight;
   
   bool passBaselineEventSelection; // Store default event selection decision, excluding any HF cut, different for OO and PP
   bool passL1HFAND_16_Online;
   bool passL1HFOR_16_Online;
   bool passL1HFAND_14_Online;
   bool passL1HFOR_14_Online;
   bool passHFAND_10_Offline;
   bool passHFAND_13_Offline;
   bool passHFAND_19_Offline;

   // Trigger bits

   bool HLT_PPRefZeroBias_v6;
   // pO and OO triggers
   bool HLT_OxyZeroBias_v1;
   bool HLT_OxyZDC1nOR_v1;
   bool HLT_OxySingleMuOpen_NotMBHF2OR_v1;
   bool HLT_OxySingleJet8_ZDC1nAsymXOR_v1;
   bool HLT_OxyNotMBHF2_v1;
   bool HLT_OxyZeroBias_SinglePixelTrackLowPt_MaxPixelCluster400_v1;
   bool HLT_OxyZeroBias_MinPixelCluster400_v1;
   bool HLT_MinimumBiasHF_OR_BptxAND_v1;
   bool HLT_MinimumBiasHF_AND_BptxAND_v1;

   bool HLT_OxySingleJet16_ZDC1nAsymXOR_v1;
   bool HLT_OxySingleJet16_ZDC1nXOR_v1;
   bool HLT_OxySingleJet24_ZDC1nAsymXOR_v1;
   bool HLT_OxySingleJet24_ZDC1nXOR_v1;
   bool HLT_OxyL1SingleJet20_v1;

   
   std::vector<float> *trkPt;
   std::vector<float> *trkPhi;
   std::vector<float> *trkPtError;
   std::vector<float> *trkEta;
   std::vector<bool> *highPurity;
   std::vector<float> *trkDxyAssociatedVtx;
   std::vector<float> *trkDzAssociatedVtx;
   std::vector<float> *trkDxyErrAssociatedVtx;
   std::vector<float> *trkDzErrAssociatedVtx;
   std::vector<int> *trkAssociatedVtxIndx;
   std::vector<char> *trkCharge;
   std::vector<char> *trkNHits;
   std::vector<char> *trkNPixHits;
   std::vector<char> *trkNLayers;
   std::vector<float> *trkNormChi2;
   std::vector<float> *pfEnergy;

   //track selection booleans
   std::vector<bool> *trkPassChargedHadron_Nominal;
   std::vector<bool> *trkPassChargedHadron_Loose;
   std::vector<bool> *trkPassChargedHadron_Tight;

   // weighting properties
   std::vector<float> *trackWeight;
   std::vector<float> *trackingEfficiency_Nominal;
   std::vector<float> *trackingEfficiency_Loose;
   std::vector<float> *trackingEfficiency_Tight;
   std::vector<float> *MC_TrkPtReweight;
   std::vector<float> *MC_TrkDCAReweight;
   std::vector<float> *TrkSpeciesWeight_pp;
   std::vector<float> *TrkSpeciesWeight_dNdEta40;
   std::vector<float> *TrkSpeciesWeight_dNdEta100;
   

   // Debug mode quantities
   std::vector<float> *AllxVtx;
   std::vector<float> *AllyVtx;
   std::vector<float> *AllzVtx;
   std::vector<float> *AllxVtxError;
   std::vector<float> *AllyVtxError;
   std::vector<float> *AllzVtxError;
   std::vector<bool> *AllisFakeVtx;
   std::vector<int> *AllnTracksVtx;
   std::vector<float> *Allchi2Vtx;
   std::vector<float> *AllndofVtx;
   std::vector<float> *AllptSumVtx;

   // PPS tracks variables
   std::vector<float> *PPSStation0M_x;
   std::vector<float> *PPSStation0M_y;
   std::vector<float> *PPSStation2M_x;
   std::vector<float> *PPSStation2M_y;

   //FSC variables
   std::vector<int> *FSC2topM_adc;
   std::vector<float> *FSC2topM_chargefC;
   std::vector<int> *FSC2topM_tdc;

   std::vector<int> *FSC2bottomM_adc;
   std::vector<float> *FSC2bottomM_chargefC;
   std::vector<int> *FSC2bottomM_tdc;

   std::vector<int> *FSC3bottomleftM_adc;
   std::vector<float> *FSC3bottomleftM_chargefC;
   std::vector<int> *FSC3bottomleftM_tdc;

   std::vector<int> *FSC3bottomrightM_adc;
   std::vector<float> *FSC3bottomrightM_chargefC;
   std::vector<int> *FSC3bottomrightM_tdc;

   std::vector<int> *FSC3topleftM_adc;
   std::vector<float> *FSC3topleftM_chargefC;
   std::vector<int> *FSC3topleftM_tdc;

   std::vector<int> *FSC3toprightM_adc;
   std::vector<float> *FSC3toprightM_chargefC;
   std::vector<int> *FSC3toprightM_tdc;

public:   // Derived quantities
   //bool GoodPhotonuclear; //FIXME: currently not implemented

private:
   bool WriteMode;
   bool Initialized;
   bool DebugMode;
   bool includeFSCandPPSMode;
   int saveTriggerBitsMode; // 0 for no HLT bits saved, 1 for HLT OO, 2 for HLT pO

public:
   ChargedHadronRAATreeMessenger(TFile &File, std::string TreeName = "tree", int saveTriggerBits = 0, bool Debug = false, bool includeFSCandPPS = false);
   ChargedHadronRAATreeMessenger(TFile *File, std::string TreeName = "tree", int saveTriggerBits = 0, bool Debug = false, bool includeFSCandPPS = false);
   ChargedHadronRAATreeMessenger(TTree *ChargedHadRAATree = nullptr, int saveTriggerBits = 0, bool Debug = false, bool includeFSCandPPS = false);
   ~ChargedHadronRAATreeMessenger();
   bool Initialize(TTree *ChargedHadRAATree, int saveTriggerBits = 0, bool Debug = false, bool includeFSCandPPS = false);
   bool Initialize(int saveTriggerBits = 0, bool Debug = false, bool includeFSCandPPS = false);
   int GetEntries();
   bool GetEntry(int iEntry);
   bool SetBranch(TTree *T, int saveTriggerBits = 0, bool Debug = false, bool includeFSCandPPS = false);
   void Clear();
   //void CopyNonTrack(ChargedHadronRAATreeMessenger &M);
   bool FillEntry();

};

/* Class for the UPC EEC analysis */
class UPCEECTreeMessenger
{
public:
   TTree *Tree;
   int Run;
   long long Event;
   int Lumi;
   bool isL1ZDCOr, isL1ZDCXORJet8, isL1ZDCXORJet12, isL1ZDCXORJet16, isGammaN;
   
   // particle flow info
   std::vector<float> *PT;
   std::vector<float> *E;
   std::vector<float> *Eta;
   std::vector<float> *Phi;
   std::vector<float> *M;

   //charged hadron info
   int Nch;
   std::vector<float> *trkPt;
   std::vector<float> *trkEta;
   std::vector<float> *trkPhi;
   std::vector<float> *pfEnergy; 

   // inclusive jet quantites
   int JetCount;
   std::vector<float> *JetPt;
   std::vector<float> *JetEta;
   std::vector<float> *JetY;
   std::vector<float> *JetPhi;

   //MC only quantities
   float pthat;
   std::vector<float> *GenJetPt;
   std::vector<float> *GenJetEta;
   std::vector<float> *GenJetY;
   std::vector<float> *GenJetPhi;
   std::vector<float> *RefJetPt;
   std::vector<float> *RefJetEta;
   std::vector<float> *RefJetY;
   std::vector<float> *RefJetPhi;

private:
   bool WriteMode;
   bool Initialized;

public:
   UPCEECTreeMessenger(TFile &File, std::string TreeName = "tree", bool Debug = false);
   UPCEECTreeMessenger(TFile *File, std::string TreeName = "tree", bool Debug = false);
   UPCEECTreeMessenger(TTree *HFJetUPCTree = nullptr, bool Debug = false);
   ~UPCEECTreeMessenger();
   bool Initialize(TTree *HFJetUPCTree, bool Debug = false);
   bool Initialize(bool Debug = false);
   int GetEntries();
   bool GetEntry(int iEntry);
   bool SetBranch(TTree *T);
   void Clear();
   bool FillEntry();
};


class MuMuJetMessenger
{
public:
   TTree *Tree;
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
   std::vector<float> *JetPT;
   std::vector<float> *JetEta;
   std::vector<float> *JetPhi;
   std::vector<bool> *IsMuMuTagged;
   std::vector<float> *GenJetPT;
   std::vector<float> *GenJetEta;
   std::vector<float> *GenJetPhi;
   std::vector<int> *GenJetMatchIdx;
   std::vector<bool> *GenIsMuMuTagged;
   std::vector<float> *muPt1;
   std::vector<float> *muPt2;
   std::vector<float> *muEta1;
   std::vector<float> *muEta2;
   std::vector<float> *muPhi1;
   std::vector<float> *muPhi2;
   std::vector<int> *muCharge1;
   std::vector<int> *muCharge2;
   std::vector<float> *muDiDxy1;
   std::vector<float> *muDiDxy1Err;
   std::vector<float> *muDiDxy2;
   std::vector<float> *muDiDxy2Err;
   std::vector<float> *muDiDz1;
   std::vector<float> *muDiDz1Err;
   std::vector<float> *muDiDz2;
   std::vector<float> *muDiDz2Err;
   std::vector<float> *muDiDxy1Dxy2;
   std::vector<float> *muDiDxy1Dxy2Err;
   std::vector<float> *mumuMass;
   std::vector<float> *mumuEta;
   std::vector<float> *mumuY;
   std::vector<float> *mumuPhi;
   std::vector<float> *mumuPt;
   std::vector<bool> *mumuIsGenMatched;
   //std::vector<int> *mumuisOnia;
   std::vector<float> *DRJetmu1;
   std::vector<float> *DRJetmu2;
   std::vector<float> *muDeta;
   std::vector<float> *muDphi;
   std::vector<float> *muDR;
   std::vector<std::vector<float>> *ExtraMuWeight;
   std::vector<float> *MuMuWeight;

   std::vector<float> *GenMuPt1;
   std::vector<float> *GenMuPt2;
   std::vector<float> *GenMuEta1;
   std::vector<float> *GenMuEta2;
   std::vector<float> *GenMuPhi1;
   std::vector<float> *GenMuPhi2;
   std::vector<float> *GenMuMuMass;
   std::vector<float> *GenMuMuEta;
   std::vector<float> *GenMuMuY;
   std::vector<float> *GenMuMuPhi;
   std::vector<float> *GenMuMuPt;
   std::vector<float> *GenMuDeta;
   std::vector<float> *GenMuDphi;
   std::vector<float> *GenMuDR;

   //

   std::vector<int> *MJTHadronFlavor;
   std::vector<int> *MJTNcHad;
   std::vector<int> *MJTNbHad;

   //

   std::vector<int> *jtNsvtx;
   std::vector<int> *jtNtrk;
   std::vector<float> *jtptCh;

   // 

   int nsvtx;
   std::vector<std::vector<int>> *svtxJetId;
   std::vector<std::vector<int>> *svtxNtrk;
   std::vector<std::vector<float>> *svtxdl;
   std::vector<std::vector<float>> *svtxdls;
   std::vector<std::vector<float>> *svtxdl2d;
   std::vector<std::vector<float>> *svtxdls2d;
   std::vector<std::vector<float>> *svtxm;
   std::vector<std::vector<float>> *svtxmcorr;
   std::vector<std::vector<float>> *svtxpt;
   std::vector<std::vector<float>> *svtxnormchi2;
   std::vector<std::vector<float>> *svtxchi2;

   std::vector<int> *svtxIdx_mu1;
   std::vector<int> *svtxIdx_mu2;

   //

   int ntrk;
   std::vector<std::vector<int>> *trkJetId;
   std::vector<std::vector<int>> *trkSvtxId;
   std::vector<std::vector<float>> *trkPt;
   std::vector<std::vector<float>> *trkEta;
   std::vector<std::vector<float>> *trkPhi;
   std::vector<std::vector<float>> *trkIp3d;
   std::vector<std::vector<float>> *trkIp3dSig;
   std::vector<std::vector<float>> *trkIp2d;
   std::vector<std::vector<float>> *trkIp2dSig;
   std::vector<std::vector<float>> *trkDistToAxis;
   std::vector<std::vector<float>> *trkDistToAxisSig;
   std::vector<std::vector<float>> *trkIpProb3d;
   std::vector<std::vector<float>> *trkIpProb2d;
   std::vector<std::vector<float>> *trkDz;
   std::vector<std::vector<int>> *trkPdgId;
   std::vector<std::vector<int>> *trkMatchSta;

   std::vector<int> *trkIdx_mu1;
   std::vector<int> *trkIdx_mu2;

   
private:
   bool WriteMode;
   bool Initialized;
public:
   MuMuJetMessenger(TFile &File, std::string TreeName = "tree");
   MuMuJetMessenger(TFile *File, std::string TreeName = "tree");
   MuMuJetMessenger(TTree *MuMuJetTree = nullptr);
   ~MuMuJetMessenger();
   bool Initialize(TTree *MuMuJetTree);
   bool Initialize();
   int GetEntries();
   bool GetEntry(int iEntry);
   bool SetBranch(TTree *T);
   void Clear();
   void CopyNonTrack(MuMuJetMessenger &M);
   bool FillEntry();
};

#endif
