#include "DimuonMessenger.h"

DimuonJetMessenger::DimuonJetMessenger(TFile &File, std::string TreeName)
{
   Initialized = false;
   WriteMode = false;

   Tree = (TTree *)File.Get(TreeName.c_str());
   Initialize();
}

DimuonJetMessenger::DimuonJetMessenger(TFile *File, std::string TreeName)
{
   Initialized = false;
   WriteMode = false;

   if(File != nullptr)
      Tree = (TTree *)File->Get(TreeName.c_str());
   else
      Tree = nullptr;
   Initialize();
}

DimuonJetMessenger::DimuonJetMessenger(TTree *DimuonJetTree)
{
   Initialized = false;
   WriteMode = false;

   Initialize(DimuonJetTree);
}

DimuonJetMessenger::~DimuonJetMessenger()
{
   if(Initialized == true && WriteMode == true)
   {

      delete ExtraMuWeight;
      
      delete svtxJetId;
      delete svtxNtrk;
      delete svtxdl;
      delete svtxdls;
      delete svtxdl2d;
      delete svtxdls2d;
      delete svtxm;
      delete svtxmcorr;
      delete svtxpt;
      delete svtxnormchi2;
      delete svtxchi2;

      delete trkJetId;
      delete trkSvtxId;
      delete trkPt;
      delete trkEta;
      delete trkPhi;
      delete trkIp3d;
      delete trkIp3dSig;
      delete trkIp2d;
      delete trkIp2dSig;
      delete trkDistToAxis;
      delete trkDistToAxisSig;
      delete trkIpProb3d;
      delete trkIpProb2d;
      delete trkDz;
      delete trkPdgId;
      delete trkMatchSta;
   }
}

bool DimuonJetMessenger::Initialize(TTree *MuMuJetTree)
{
   Tree = MuMuJetTree;
   return Initialize();
}

bool DimuonJetMessenger::Initialize()
{
   if(Tree == nullptr)
      return false;

   Initialized = true;

   ExtraMuWeight = nullptr;

   svtxJetId = nullptr;
   svtxNtrk = nullptr;
   svtxdl = nullptr;
   svtxdls = nullptr;
   svtxdl2d = nullptr;
   svtxdls2d = nullptr;
   svtxm = nullptr;
   svtxmcorr = nullptr;
   svtxpt = nullptr;
   svtxnormchi2 = nullptr;
   svtxchi2 = nullptr;

   trkJetId = nullptr;
   trkSvtxId = nullptr;
   trkPt = nullptr;
   trkEta = nullptr;
   trkPhi = nullptr;
   trkIp3d = nullptr;
   trkIp3dSig = nullptr;
   trkIp2d = nullptr;
   trkIp2dSig = nullptr;
   trkDistToAxis = nullptr;
   trkDistToAxisSig = nullptr;
   trkIpProb3d = nullptr;
   trkIpProb2d = nullptr;
   trkDz = nullptr;
   trkPdgId = nullptr;
   trkMatchSta = nullptr;

   Tree->SetBranchAddress("Run", &Run);
   Tree->SetBranchAddress("Event", &Event);
   Tree->SetBranchAddress("Lumi", &Lumi);
   Tree->SetBranchAddress("hiBin", &hiBin);
   Tree->SetBranchAddress("hiHF", &hiHF);
   Tree->SetBranchAddress("NVertex", &NVertex);
   Tree->SetBranchAddress("VX", &VX);
   Tree->SetBranchAddress("VY", &VY);
   Tree->SetBranchAddress("VZ", &VZ);
   Tree->SetBranchAddress("VXError", &VXError);
   Tree->SetBranchAddress("VYError", &VYError);
   Tree->SetBranchAddress("VZError", &VZError);
   Tree->SetBranchAddress("NCollWeight", &NCollWeight);
   Tree->SetBranchAddress("EventWeight", &EventWeight);
   Tree->SetBranchAddress("PTHat", &PTHat);
   Tree->SetBranchAddress("NPU", &NPU);
   Tree->SetBranchAddress("JetPT", &JetPT);
   Tree->SetBranchAddress("JetEta", &JetEta);
   Tree->SetBranchAddress("JetPhi", &JetPhi);
   Tree->SetBranchAddress("IsMuMuTagged", &IsMuMuTagged);
   Tree->SetBranchAddress("GenIsMuMuTagged", &GenIsMuMuTagged);
   Tree->SetBranchAddress("muPt1", &muPt1);
   Tree->SetBranchAddress("muPt2", &muPt2);
   Tree->SetBranchAddress("muEta1", &muEta1);
   Tree->SetBranchAddress("muEta2", &muEta2);
   Tree->SetBranchAddress("muPhi1", &muPhi1);
   Tree->SetBranchAddress("muPhi2", &muPhi2);
   Tree->SetBranchAddress("muCharge1", &muCharge1);
   Tree->SetBranchAddress("muCharge2", &muCharge2);
   Tree->SetBranchAddress("muDiDxy1", &muDiDxy1);
   Tree->SetBranchAddress("muDiDxy1Err", &muDiDxy1Err);
   Tree->SetBranchAddress("muDiDxy2", &muDiDxy2);
   Tree->SetBranchAddress("muDiDxy2Err", &muDiDxy2Err);
   Tree->SetBranchAddress("muDiDz1", &muDiDz1);
   Tree->SetBranchAddress("muDiDz1Err", &muDiDz1Err);
   Tree->SetBranchAddress("muDiDz2", &muDiDz2);
   Tree->SetBranchAddress("muDiDz2Err", &muDiDz2Err);
   Tree->SetBranchAddress("muDiDxy1Dxy2", &muDiDxy1Dxy2);
   Tree->SetBranchAddress("muDiDxy1Dxy2Err", &muDiDxy1Dxy2Err);
   Tree->SetBranchAddress("mumuMass", &mumuMass);
   Tree->SetBranchAddress("mumuEta", &mumuEta);
   Tree->SetBranchAddress("mumuY", &mumuY);
   Tree->SetBranchAddress("mumuPhi", &mumuPhi);
   Tree->SetBranchAddress("mumuPt", &mumuPt);
   Tree->SetBranchAddress("mumuIsGenMatched", &mumuIsGenMatched);
   //Tree->SetBranchAddress("mumuisOnia", &mumuisOnia);
   Tree->SetBranchAddress("DRJetmu1", &DRJetmu1);
   Tree->SetBranchAddress("DRJetmu2", &DRJetmu2);
   Tree->SetBranchAddress("muDeta", &muDeta);
   Tree->SetBranchAddress("muDphi", &muDphi);
   Tree->SetBranchAddress("muDR", &muDR);
   Tree->SetBranchAddress("ExtraMuWeight", &ExtraMuWeight);
   Tree->SetBranchAddress("MuMuWeight", &MuMuWeight);

   Tree->SetBranchAddress("GenMuPt1", &GenMuPt1);
   Tree->SetBranchAddress("GenMuPt2", &GenMuPt2);
   Tree->SetBranchAddress("GenMuEta1", &GenMuEta1);
   Tree->SetBranchAddress("GenMuEta2", &GenMuEta2);
   Tree->SetBranchAddress("GenMuPhi1", &GenMuPhi1);
   Tree->SetBranchAddress("GenMuPhi2", &GenMuPhi2);
   Tree->SetBranchAddress("GenMuMuMass", &GenMuMuMass);
   Tree->SetBranchAddress("GenMuMuEta", &GenMuMuEta);
   Tree->SetBranchAddress("GenMuMuY", &GenMuMuY);
   Tree->SetBranchAddress("GenMuMuPhi", &GenMuMuPhi);
   Tree->SetBranchAddress("GenMuMuPt", &GenMuMuPt);
   Tree->SetBranchAddress("GenMuDeta", &GenMuDeta);
   Tree->SetBranchAddress("GenMuDphi", &GenMuDphi);
   Tree->SetBranchAddress("GenMuDR", &GenMuDR);

   Tree->SetBranchAddress("MJTHadronFlavor", &MJTHadronFlavor);
   Tree->SetBranchAddress("MJTNcHad", &MJTNcHad);
   Tree->SetBranchAddress("MJTNbHad", &MJTNbHad);

   Tree->SetBranchAddress("jtNsvtx", &jtNsvtx);
   Tree->SetBranchAddress("jtNtrk", &jtNtrk);
   Tree->SetBranchAddress("jtptCh", &jtptCh);

   Tree->SetBranchAddress("svtxJetId", &svtxJetId);
   Tree->SetBranchAddress("svtxNtrk", &svtxNtrk);
   Tree->SetBranchAddress("svtxdl", &svtxdl);
   Tree->SetBranchAddress("svtxdls", &svtxdls);
   Tree->SetBranchAddress("svtxdl2d", &svtxdl2d);
   Tree->SetBranchAddress("svtxdls2d", &svtxdls2d);
   Tree->SetBranchAddress("svtxm", &svtxm);
   Tree->SetBranchAddress("svtxmcorr", &svtxmcorr);
   Tree->SetBranchAddress("svtxpt", &svtxpt);
   Tree->SetBranchAddress("svtxnormchi2", &svtxnormchi2);
   Tree->SetBranchAddress("svtxchi2", &svtxchi2);
   Tree->SetBranchAddress("svtxIdx_mu1", &svtxIdx_mu1);
   Tree->SetBranchAddress("svtxIdx_mu2", &svtxIdx_mu2);

   Tree->SetBranchAddress("trkJetId", &trkJetId);
   Tree->SetBranchAddress("trkSvtxId", &trkSvtxId);
   Tree->SetBranchAddress("trkPt", &trkPt);
   Tree->SetBranchAddress("trkEta", &trkEta);
   Tree->SetBranchAddress("trkPhi", &trkPhi);
   Tree->SetBranchAddress("trkIp3d", &trkIp3d);
   Tree->SetBranchAddress("trkIp3dSig", &trkIp3dSig);
   Tree->SetBranchAddress("trkIp2d", &trkIp2d);
   Tree->SetBranchAddress("trkIp2dSig", &trkIp2dSig);
   Tree->SetBranchAddress("trkDistToAxis", &trkDistToAxis);
   Tree->SetBranchAddress("trkDistToAxisSig", &trkDistToAxisSig);
   Tree->SetBranchAddress("trkIpProb3d", &trkIpProb3d);
   Tree->SetBranchAddress("trkIpProb2d", &trkIpProb2d);
   Tree->SetBranchAddress("trkDz", &trkDz);
   Tree->SetBranchAddress("trkPdgId", &trkPdgId);
   Tree->SetBranchAddress("trkMatchSta", &trkMatchSta);
   Tree->SetBranchAddress("trkIdx_mu1", &trkIdx_mu1);
   Tree->SetBranchAddress("trkIdx_mu2", &trkIdx_mu2);

   return true;
}

int DimuonJetMessenger::GetEntries()
{
   if(Tree == nullptr)
      return 0;
   return Tree->GetEntries();
}

bool DimuonJetMessenger::GetEntry(int iEntry)
{
   if(Tree == nullptr)
      return false;

   Tree->GetEntry(iEntry);
   return true;
}

bool DimuonJetMessenger::SetBranch(TTree *T)
{
   if(T == nullptr)
      return false;

   Initialized = true;
   WriteMode = true;

   ExtraMuWeight = new std::vector<float>(12,1.0);

   svtxJetId = new std::vector<int>();
   svtxNtrk = new std::vector<int>();
   svtxdl = new std::vector<float>();
   svtxdls = new std::vector<float>();
   svtxdl2d = new std::vector<float>();
   svtxdls2d = new std::vector<float>();
   svtxm = new std::vector<float>();
   svtxmcorr = new std::vector<float>();
   svtxpt = new std::vector<float>();
   svtxnormchi2 = new std::vector<float>();
   svtxchi2 = new std::vector<float>();

   trkJetId = new std::vector<int>();
   trkSvtxId = new std::vector<int>();
   trkPt = new std::vector<float>();
   trkEta = new std::vector<float>();
   trkPhi = new std::vector<float>();
   trkIp3d = new std::vector<float>();
   trkIp3dSig = new std::vector<float>();
   trkIp2d = new std::vector<float>();
   trkIp2dSig = new std::vector<float>();
   trkDistToAxis = new std::vector<float>();
   trkDistToAxisSig = new std::vector<float>();
   trkIpProb3d = new std::vector<float>();
   trkIpProb2d = new std::vector<float>();
   trkDz = new std::vector<float>();
   trkPdgId = new std::vector<int>();
   trkMatchSta = new std::vector<int>();

   Tree = T;

   Tree->Branch("Run", &Run, "Run/I");
   Tree->Branch("Event", &Event, "Event/L");
   Tree->Branch("Lumi", &Lumi, "Lumi/I");
   Tree->Branch("hiBin", &hiBin, "hiBin/I");
   Tree->Branch("hiHF", &hiHF, "hiHF/F");
   Tree->Branch("NVertex", &NVertex, "NVertex/I");
   Tree->Branch("VX", &VX, "VX/F");
   Tree->Branch("VY", &VY, "VY/F");
   Tree->Branch("VZ", &VZ, "VZ/F");
   Tree->Branch("VXError", &VXError, "VXError/F");
   Tree->Branch("VYError", &VYError, "VYError/F");
   Tree->Branch("VZError", &VZError, "VZError/F");
   Tree->Branch("NCollWeight", &NCollWeight,  "NCollWeight/F");
   Tree->Branch("EventWeight", &EventWeight,  "EventWeight/F");
   Tree->Branch("PTHat", &PTHat,  "PTHat/F");
   Tree->Branch("NPU", &NPU, "NPU/I");
   Tree->Branch("JetPT", &JetPT);
   Tree->Branch("JetEta", &JetEta);
   Tree->Branch("JetPhi", &JetPhi);
   Tree->Branch("IsMuMuTagged", &IsMuMuTagged);
   Tree->Branch("GenIsMuMuTagged", &GenIsMuMuTagged);
   Tree->Branch("muPt1", &muPt1);
   Tree->Branch("muPt2", &muPt2);
   Tree->Branch("muEta1", &muEta1);
   Tree->Branch("muEta2", &muEta2);
   Tree->Branch("muPhi1", &muPhi1);
   Tree->Branch("muPhi2", &muPhi2);
   Tree->Branch("muCharge1", &muCharge1);
   Tree->Branch("muCharge2", &muCharge2);
   Tree->Branch("muDiDxy1", &muDiDxy1);
   Tree->Branch("muDiDxy1Err", &muDiDxy1Err);
   Tree->Branch("muDiDxy2", &muDiDxy2);
   Tree->Branch("muDiDxy2Err", &muDiDxy2Err);
   Tree->Branch("muDiDz1", &muDiDz1);
   Tree->Branch("muDiDz1Err", &muDiDz1Err);
   Tree->Branch("muDiDz2", &muDiDz2);
   Tree->Branch("muDiDz2Err", &muDiDz2Err);
   Tree->Branch("muDiDxy1Dxy2", &muDiDxy1Dxy2);
   Tree->Branch("muDiDxy1Dxy2Err", &muDiDxy1Dxy2Err);
   Tree->Branch("mumuMass", &mumuMass);
   Tree->Branch("mumuEta", &mumuEta);
   Tree->Branch("mumuY", &mumuY);
   Tree->Branch("mumuPhi", &mumuPhi);
   Tree->Branch("mumuPt", &mumuPt);
   Tree->Branch("mumuIsGenMatched", &mumuIsGenMatched);
   //Tree->Branch("mumuisOnia", &mumuisOnia);
   Tree->Branch("DRJetmu1", &DRJetmu1);
   Tree->Branch("DRJetmu2", &DRJetmu2);
   Tree->Branch("muDeta", &muDeta);
   Tree->Branch("muDphi", &muDphi);
   Tree->Branch("muDR", &muDR);
   Tree->Branch("ExtraMuWeight", &ExtraMuWeight);
   Tree->Branch("MuMuWeight", &MuMuWeight);

   Tree->Branch("GenMuPt1", &GenMuPt1);
   Tree->Branch("GenMuPt2", &GenMuPt2);
   Tree->Branch("GenMuEta1", &GenMuEta1);
   Tree->Branch("GenMuEta2", &GenMuEta2);
   Tree->Branch("GenMuPhi1", &GenMuPhi1);
   Tree->Branch("GenMuPhi2", &GenMuPhi2);
   Tree->Branch("GenMuMuMass", &GenMuMuMass);
   Tree->Branch("GenMuMuEta", &GenMuMuEta);
   Tree->Branch("GenMuMuY", &GenMuMuY);
   Tree->Branch("GenMuMuPhi", &GenMuMuPhi);
   Tree->Branch("GenMuMuPt", &GenMuMuPt);
   Tree->Branch("GenMuDeta", &GenMuDeta);
   Tree->Branch("GenMuDphi", &GenMuDphi);
   Tree->Branch("GenMuDR", &GenMuDR);

   Tree->Branch("MJTHadronFlavor", &MJTHadronFlavor);
   Tree->Branch("MJTNcHad", &MJTNcHad);
   Tree->Branch("MJTNbHad", &MJTNbHad);

   Tree->Branch("jtNsvtx", &jtNsvtx);
   Tree->Branch("jtNtrk", &jtNtrk);
   Tree->Branch("jtptCh", &jtptCh);

   Tree->Branch("svtxJetId", &svtxJetId);
   Tree->Branch("svtxNtrk", &svtxNtrk);
   Tree->Branch("svtxdl", &svtxdl);
   Tree->Branch("svtxdls", &svtxdls);
   Tree->Branch("svtxdl2d", &svtxdl2d);
   Tree->Branch("svtxdls2d", &svtxdls2d);
   Tree->Branch("svtxm", &svtxm);
   Tree->Branch("svtxmcorr", &svtxmcorr);
   Tree->Branch("svtxpt", &svtxpt);
   Tree->Branch("svtxnormchi2", &svtxnormchi2);
   Tree->Branch("svtxchi2", &svtxchi2);
   Tree->Branch("svtxIdx_mu1", &svtxIdx_mu1);
   Tree->Branch("svtxIdx_mu2", &svtxIdx_mu2);

   Tree->Branch("trkJetId", &trkJetId);
   Tree->Branch("trkSvtxId", &trkSvtxId);
   Tree->Branch("trkPt", &trkPt);
   Tree->Branch("trkEta", &trkEta);
   Tree->Branch("trkPhi", &trkPhi);
   Tree->Branch("trkIp3d", &trkIp3d);
   Tree->Branch("trkIp3dSig", &trkIp3dSig);
   Tree->Branch("trkIp2d", &trkIp2d);
   Tree->Branch("trkIp2dSig", &trkIp2dSig);
   Tree->Branch("trkDistToAxis", &trkDistToAxis);
   Tree->Branch("trkDistToAxisSig", &trkDistToAxisSig);
   Tree->Branch("trkIpProb3d", &trkIpProb3d);
   Tree->Branch("trkIpProb2d", &trkIpProb2d);
   Tree->Branch("trkDz", &trkDz);
   Tree->Branch("trkPdgId", &trkPdgId);
   Tree->Branch("trkMatchSta", &trkMatchSta);
   Tree->Branch("trkIdx_mu1", &trkIdx_mu1);
   Tree->Branch("trkIdx_mu2", &trkIdx_mu2);

   return true;
}

void DimuonJetMessenger::Clear()
{
   if(Initialized == false)
      return;

   Run = -999;
   Event = -999;
   Lumi = -999;
   hiBin = -999;
   hiHF = -999;
   NVertex = 0;
   VX = 0;
   VY = 0;
   VZ = 0;
   VXError = 0;
   VYError = 0;
   VZError = 0;
   NCollWeight = 1;
   PTHat = -999;
   EventWeight = 1;
   nsvtx = 0;
   ntrk = 0;
   NPU = 0;

    JetPT = -999;
    JetEta = -999;
    JetPhi = -999;
    IsMuMuTagged = false;
    GenIsMuMuTagged = false;
    muPt1 = -999;
    muPt2 = -999;
    muEta1 = -999;
    muEta2 = -999;
    muPhi1 = -999;
    muPhi2 = -999;
    muCharge1 = -999;
    muCharge2 = -999;
    muDiDxy1 = -999;
    muDiDxy1Err = -999;
    muDiDxy2 = -999;
    muDiDxy2Err = -999;
    muDiDz1 = -999;
    muDiDz1Err = -999;
    muDiDz2 = -999;
    muDiDz2Err = -999;
    muDiDxy1Dxy2 = -999;
    muDiDxy1Dxy2Err = -999;
    mumuMass = -999;
    mumuEta = -999;
    mumuY = -999;
    mumuPhi = -999;
    mumuPt = -999;
    mumuIsGenMatched = false;
    //mumuisOnia->clear();  
    DRJetmu1 = -999;
    DRJetmu2 = -999;
    muDeta = -999;
    muDphi = -999;
    muDR = -999;
    ExtraMuWeight->assign(12, 1.0);
   MuMuWeight = 1;

   GenMuPt1 = -999;
   GenMuPt2 = -999;
   GenMuEta1 = -999;
   GenMuEta2 = -999;
   GenMuPhi1 = -999;
   GenMuPhi2 = -999;
   GenMuMuMass = -999;
   GenMuMuEta = -999;
   GenMuMuY = -999;
   GenMuMuPhi = -999;
   GenMuMuPt = -999;
   GenMuDeta = -999;
   GenMuDphi = -999;
   GenMuDR = -999;

    MJTHadronFlavor = -999;
    MJTNcHad = -999;
    MJTNbHad = -999;
    jtptCh = -999;
    jtNtrk = -999;
    jtNsvtx = -999;
    svtxIdx_mu1 = -1;
    svtxIdx_mu2 = -1;
    trkIdx_mu1 = -1;
    trkIdx_mu2 = -1;

   svtxJetId->clear();
   svtxNtrk->clear();
   svtxdl->clear();
   svtxdls->clear();
   svtxdl2d->clear();
   svtxdls2d->clear();
   svtxm->clear();
   svtxmcorr->clear();
   svtxpt->clear();
   svtxnormchi2->clear();
   svtxchi2->clear();

   trkJetId->clear();
   trkSvtxId->clear();
   trkPt->clear();
   trkEta->clear();
   trkPhi->clear();
   trkIp3d->clear();
   trkIp3dSig->clear();
   trkIp2d->clear();
   trkIp2dSig->clear();
   trkDistToAxis->clear();
   trkDistToAxisSig->clear();
   trkIpProb3d->clear();
   trkIpProb2d->clear();
   trkDz->clear();
   trkPdgId->clear();
   trkMatchSta->clear();
}

void DimuonJetMessenger::Clear_Jet()
{
   if(Initialized == false)
      return;

    JetPT = -999;
    JetEta = -999;
    JetPhi = -999;
    IsMuMuTagged = false;
    GenIsMuMuTagged = false;
    muPt1 = -999;
    muPt2 = -999;
    muEta1 = -999;
    muEta2 = -999;
    muPhi1 = -999;
    muPhi2 = -999;
    muCharge1 = -999;
    muCharge2 = -999;
    muDiDxy1 = -999;
    muDiDxy1Err = -999;
    muDiDxy2 = -999;
    muDiDxy2Err = -999;
    muDiDz1 = -999;
    muDiDz1Err = -999;
    muDiDz2 = -999;
    muDiDz2Err = -999;
    muDiDxy1Dxy2 = -999;
    muDiDxy1Dxy2Err = -999;
    mumuMass = -999;
    mumuEta = -999;
    mumuY = -999;
    mumuPhi = -999;
    mumuPt = -999;
    mumuIsGenMatched = false;
    //mumuisOnia->clear();  
    DRJetmu1 = -999;
    DRJetmu2 = -999;
    muDeta = -999;
    muDphi = -999;
    muDR = -999;
    ExtraMuWeight->assign(12, 1.0);
   MuMuWeight = 1;

   GenMuPt1 = -999;
   GenMuPt2 = -999;
   GenMuEta1 = -999;
   GenMuEta2 = -999;
   GenMuPhi1 = -999;
   GenMuPhi2 = -999;
   GenMuMuMass = -999;
   GenMuMuEta = -999;
   GenMuMuY = -999;
   GenMuMuPhi = -999;
   GenMuMuPt = -999;
   GenMuDeta = -999;
   GenMuDphi = -999;
   GenMuDR = -999;

    MJTHadronFlavor = -999;
    MJTNcHad = -999;
    MJTNbHad = -999;
    jtptCh = -999;
    jtNtrk = -999;
    jtNsvtx = -999;
    svtxIdx_mu1 = -1;
    svtxIdx_mu2 = -1;
    trkIdx_mu1 = -1;
    trkIdx_mu2 = -1;

   svtxJetId->clear();
   svtxNtrk->clear();
   svtxdl->clear();
   svtxdls->clear();
   svtxdl2d->clear();
   svtxdls2d->clear();
   svtxm->clear();
   svtxmcorr->clear();
   svtxpt->clear();
   svtxnormchi2->clear();
   svtxchi2->clear();

   trkJetId->clear();
   trkSvtxId->clear();
   trkPt->clear();
   trkEta->clear();
   trkPhi->clear();
   trkIp3d->clear();
   trkIp3dSig->clear();
   trkIp2d->clear();
   trkIp2dSig->clear();
   trkDistToAxis->clear();
   trkDistToAxisSig->clear();
   trkIpProb3d->clear();
   trkIpProb2d->clear();
   trkDz->clear();
   trkPdgId->clear();
   trkMatchSta->clear();
}

void DimuonJetMessenger::CopyNonTrack(DimuonJetMessenger &M)
{
   Run          = M.Run;
   Event        = M.Event;
   Lumi         = M.Lumi;
   hiBin        = M.hiBin;
   hiHF         = M.hiHF;
   NVertex      = M.NVertex;
   VX           = M.VX;
   VY           = M.VY;
   VZ           = M.VZ;
   VXError      = M.VXError;
   VYError      = M.VYError;
   VZError      = M.VZError;
   EventWeight  = M.EventWeight;
   NPU          = M.NPU;
   NCollWeight  = M.NCollWeight;
   PTHat        = M.PTHat;
   nsvtx        = M.nsvtx;
   ntrk         = M.ntrk;

   if(ExtraMuWeight != nullptr && M.ExtraMuWeight != nullptr) *ExtraMuWeight = *(M.ExtraMuWeight);

   MuMuWeight   = M.MuMuWeight;
   JetPT        = M.JetPT;
    JetEta       = M.JetEta;
    JetPhi       = M.JetPhi;
    IsMuMuTagged = M.IsMuMuTagged;
    GenIsMuMuTagged = M.GenIsMuMuTagged;
    muPt1       = M.muPt1;
    muPt2       = M.muPt2;
    muEta1      = M.muEta1;
    muEta2      = M.muEta2;
    muPhi1      = M.muPhi1;
    muPhi2      = M.muPhi2;
    muCharge1   = M.muCharge1;
    muCharge2   = M.muCharge2;
    muDiDxy1    = M.muDiDxy1;
    muDiDxy1Err = M.muDiDxy1Err;
    muDiDxy2    = M.muDiDxy2;
    muDiDxy2Err = M.muDiDxy2Err;
    muDiDz1     = M.muDiDz1;
    muDiDz1Err  = M.muDiDz1Err;
    muDiDz2     = M.muDiDz2;
    muDiDz2Err  = M.muDiDz2Err;
    muDiDxy1Dxy2    = M.muDiDxy1Dxy2;
    muDiDxy1Dxy2Err = M.muDiDxy1Dxy2Err;
    mumuMass    = M.mumuMass;
    mumuEta     = M.mumuEta;
    mumuY       = M.mumuY;
    mumuPhi     = M.mumuPhi;
    mumuPt      = M.mumuPt;
    mumuIsGenMatched = M.mumuIsGenMatched;
    //mumuisOnia  = M.mumuisOnia;
    DRJetmu1    = M.DRJetmu1;
    DRJetmu2    = M.DRJetmu2;
    muDeta      = M.muDeta;
    muDphi      = M.muDphi;
    muDR        = M.muDR;
   GenMuPt1    = M.GenMuPt1;
   GenMuPt2    = M.GenMuPt2;
   GenMuEta1   = M.GenMuEta1; 
   GenMuEta2   = M.GenMuEta2;
   GenMuPhi1   = M.GenMuPhi1;
   GenMuPhi2   = M.GenMuPhi2;
   GenMuMuMass = M.GenMuMuMass;
   GenMuMuEta  = M.GenMuMuEta;
   GenMuMuY    = M.GenMuMuY;
   GenMuMuPhi  = M.GenMuMuPhi;
   GenMuMuPt   = M.GenMuMuPt;
   GenMuDeta   = M.GenMuDeta;
   GenMuDphi   = M.GenMuDphi;
   GenMuDR     = M.GenMuDR;
    MJTHadronFlavor = M.MJTHadronFlavor;
    MJTNcHad        = M.MJTNcHad;
    MJTNbHad       = M.MJTNbHad;
    jtNsvtx    = M.jtNsvtx;
    jtNtrk     = M.jtNtrk;
    jtptCh     = M.jtptCh;
    svtxIdx_mu1 = M.svtxIdx_mu1;
    svtxIdx_mu2 = M.svtxIdx_mu2;
    trkIdx_mu1  = M.trkIdx_mu1;
    trkIdx_mu2  = M.trkIdx_mu2;

   if(svtxJetId != nullptr && M.svtxJetId != nullptr) *svtxJetId = *(svtxJetId);
   if(svtxNtrk != nullptr && M.svtxNtrk != nullptr) *svtxNtrk = *(svtxNtrk);
   if(svtxdl != nullptr && M.svtxdl != nullptr) *svtxdl = *(svtxdl);
   if(svtxdls != nullptr && M.svtxdls != nullptr) *svtxdls = *(svtxdls);
   if(svtxdl2d != nullptr && M.svtxdl2d != nullptr) *svtxdl2d = *(svtxdl2d);
   if(svtxdls2d != nullptr && M.svtxdls2d != nullptr) *svtxdls2d = *(svtxdls2d);
   if(svtxm != nullptr && M.svtxm != nullptr) *svtxm = *(svtxm);
   if(svtxmcorr != nullptr && M.svtxmcorr != nullptr) *svtxmcorr = *(svtxmcorr);
   if(svtxpt != nullptr && M.svtxpt != nullptr) *svtxpt = *(svtxpt);
   if(svtxnormchi2 != nullptr && M.svtxnormchi2 != nullptr) *svtxnormchi2 = *(svtxnormchi2);
   if(svtxchi2 != nullptr && M.svtxchi2 != nullptr) *svtxchi2 = *(svtxchi2);

   if(trkJetId != nullptr && M.trkJetId != nullptr) *trkJetId = *(trkJetId);
   if(trkSvtxId != nullptr && M.trkSvtxId != nullptr) *trkSvtxId = *(trkSvtxId);
   if(trkPt != nullptr && M.trkPt != nullptr) *trkPt = *(trkPt);
   if(trkEta != nullptr && M.trkEta != nullptr) *trkEta = *(trkEta);
   if(trkPhi != nullptr && M.trkPhi != nullptr) *trkPhi = *(trkPhi);
   if(trkIp3d != nullptr && M.trkIp3d != nullptr) *trkIp3d = *(trkIp3d);
   if(trkIp3dSig != nullptr && M.trkIp3dSig != nullptr) *trkIp3dSig = *(trkIp3dSig);
   if(trkIp2d != nullptr && M.trkIp2d != nullptr) *trkIp2d = *(trkIp2d);
   if(trkIp2dSig != nullptr && M.trkIp2dSig != nullptr) *trkIp2dSig = *(trkIp2dSig);
   if(trkDistToAxis != nullptr && M.trkDistToAxis != nullptr) *trkDistToAxis = *(trkDistToAxis);
   if(trkDistToAxisSig != nullptr && M.trkDistToAxisSig != nullptr) *trkDistToAxisSig = *(trkDistToAxisSig);
   if(trkIpProb3d != nullptr && M.trkIpProb3d != nullptr) *trkIpProb3d = *(trkIpProb3d);
   if(trkIpProb2d != nullptr && M.trkIpProb2d != nullptr) *trkIpProb2d = *(trkIpProb2d);
   if(trkDz != nullptr && M.trkDz != nullptr) *trkDz = *(trkDz);
   if(trkPdgId != nullptr && M.trkPdgId != nullptr) *trkPdgId = *(trkPdgId);
   if(trkMatchSta != nullptr && M.trkMatchSta != nullptr) *trkMatchSta = *(trkMatchSta);
}



bool DimuonJetMessenger::FillEntry()
{
   if(Initialized == false)
      return false;
   if(WriteMode == false)
      return false;

   if(Tree == nullptr)
      return false;

   Tree->Fill();
   Clear_Jet();

   return true;
}

GenDimuonJetMessenger::GenDimuonJetMessenger(TFile &File, std::string TreeName)
{
   Initialized = false;
   WriteMode = false;

   Tree = (TTree *)File.Get(TreeName.c_str());
   Initialize();
}

GenDimuonJetMessenger::GenDimuonJetMessenger(TFile *File, std::string TreeName)
{
   Initialized = false;
   WriteMode = false;

   if(File != nullptr)
      Tree = (TTree *)File->Get(TreeName.c_str());
   else
      Tree = nullptr;
   Initialize();
}

GenDimuonJetMessenger::GenDimuonJetMessenger(TTree *GenDimuonJetTree)
{
   Initialized = false;
   WriteMode = false;

   Initialize(GenDimuonJetTree);
}

GenDimuonJetMessenger::~GenDimuonJetMessenger()
{
 // filler
}

bool GenDimuonJetMessenger::Initialize(TTree *GenDimuonJetTree)
{
   Tree = GenDimuonJetTree;
   return Initialize();
}

bool GenDimuonJetMessenger::Initialize()
{
   if(Tree == nullptr)
      return false;

    Tree->SetBranchAddress("Run", &Run);
    Tree->SetBranchAddress("Event", &Event);
    Tree->SetBranchAddress("Lumi", &Lumi);
    Tree->SetBranchAddress("hiBin", &hiBin);
    Tree->SetBranchAddress("hiHF", &hiHF);
    Tree->SetBranchAddress("NVertex", &NVertex);
    Tree->SetBranchAddress("VX", &VX);
    Tree->SetBranchAddress("VY", &VY);
    Tree->SetBranchAddress("VZ", &VZ);
    Tree->SetBranchAddress("VXError", &VXError);
    Tree->SetBranchAddress("VYError", &VYError);
    Tree->SetBranchAddress("VZError", &VZError);
    Tree->SetBranchAddress("NCollWeight", &NCollWeight);
    Tree->SetBranchAddress("EventWeight", &EventWeight);
    Tree->SetBranchAddress("PTHat", &PTHat);
    Tree->SetBranchAddress("NPU", &NPU);
    Tree->SetBranchAddress("GenJetPT", &GenJetPT);
    Tree->SetBranchAddress("GenJetEta", &GenJetEta);
    Tree->SetBranchAddress("GenJetPhi", &GenJetPhi);
    Tree->SetBranchAddress("GenJetMatchIdx", &GenJetMatchIdx);
   return true;
}

int GenDimuonJetMessenger::GetEntries()
{
   if(Tree == nullptr)
      return 0;
   return Tree->GetEntries();
}

bool GenDimuonJetMessenger::GetEntry(int iEntry)
{
   if(Tree == nullptr)
      return false;

   Tree->GetEntry(iEntry);
   return true;
}

bool GenDimuonJetMessenger::SetBranch(TTree *T)
{
   if(T == nullptr)
      return false;

   Initialized = true;
   WriteMode = true;

   Tree = T;

   Tree->Branch("Run", &Run, "Run/I");
   Tree->Branch("Event", &Event, "Event/L");
   Tree->Branch("Lumi", &Lumi, "Lumi/I");
   Tree->Branch("hiBin", &hiBin, "hiBin/I");
   Tree->Branch("hiHF", &hiHF, "hiHF/F");
   Tree->Branch("NVertex", &NVertex, "NVertex/I");
   Tree->Branch("VX", &VX, "VX/F");
   Tree->Branch("VY", &VY, "VY/F");
   Tree->Branch("VZ", &VZ, "VZ/F");
   Tree->Branch("VXError", &VXError, "VXError/F");
   Tree->Branch("VYError", &VYError, "VYError/F");
   Tree->Branch("VZError", &VZError, "VZError/F");
   Tree->Branch("NCollWeight", &NCollWeight,  "NCollWeight/F");
   Tree->Branch("EventWeight", &EventWeight,  "EventWeight/F");
   Tree->Branch("PTHat", &PTHat,  "PTHat/F");
   Tree->Branch("NPU", &NPU, "NPU/I");

   Tree->Branch("GenJetPT", &GenJetPT);
   Tree->Branch("GenJetEta", &GenJetEta);
   Tree->Branch("GenJetPhi", &GenJetPhi);
   Tree->Branch("GenJetMatchIdx", &GenJetMatchIdx);


   return true;
}

void GenDimuonJetMessenger::Clear()
{
   if(Initialized == false)
      return;

    Run = -999;
    Event = -999;
    Lumi = -999;
    hiBin = -999;
    hiHF = -999;
    NVertex = 0;
    VX = 0;
    VY = 0;
    VZ = 0;
    VXError = 0;
    VYError = 0;
    VZError = 0;
    NCollWeight = 1;
    PTHat = -999;
    EventWeight = 1;
    NPU = 0;
    NCollWeight = 1;

    GenJetPT = -999;
    GenJetEta = -999;
    GenJetPhi = -999;
    GenJetMatchIdx = -1;
   
}

void GenDimuonJetMessenger::Clear_Jet()
{
   if(Initialized == false)
      return;

    GenJetPT = -999;
    GenJetEta = -999;
    GenJetPhi = -999;
    GenJetMatchIdx = -1;
   
}

void GenDimuonJetMessenger::CopyNonTrack(GenDimuonJetMessenger &M)
{
   Run          = M.Run;
   Event        = M.Event;
   Lumi         = M.Lumi;
   hiBin        = M.hiBin;
   hiHF         = M.hiHF;
   NVertex      = M.NVertex;
   VX           = M.VX;
   VY           = M.VY;
   VZ           = M.VZ;
   VXError      = M.VXError;
   VYError      = M.VYError;
   VZError      = M.VZError;
   NPU          = M.NPU;
   NCollWeight  = M.NCollWeight;
   PTHat        = M.PTHat;
   GenJetPT    = M.GenJetPT;
   GenJetEta   = M.GenJetEta;
   GenJetPhi   = M.GenJetPhi;
   GenJetMatchIdx = M.GenJetMatchIdx;

}

bool GenDimuonJetMessenger::FillEntry()
{
   if(Initialized == false)
      return false;
   if(WriteMode == false)
      return false;

   if(Tree == nullptr)
      return false;

   Tree->Fill();
   Clear_Jet();

   return true;
}