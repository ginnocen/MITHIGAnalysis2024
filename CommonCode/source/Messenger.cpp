#include <algorithm>
#include <iostream>
#include <vector>

#include "TFile.h"
#include "TTree.h"

#include "Messenger.h"

namespace EmptyVectors
{
   std::vector<double> EmptyVectorDouble;
   std::vector<float> EmptyVectorFloat;
   std::vector<int> EmptyVectorInt;
   std::vector<std::vector<float> > EmptyVectorVectorFloat;
}

HiEventTreeMessenger::HiEventTreeMessenger(TFile &File)
{
   Tree = (TTree *)File.Get("hiEvtAnalyzer/HiTree");
   Initialize();
}

HiEventTreeMessenger::HiEventTreeMessenger(TFile *File)
{
   if(File != nullptr)
      Tree = (TTree *)File->Get("hiEvtAnalyzer/HiTree");
   else
      Tree = nullptr;
   Initialize();
}

HiEventTreeMessenger::HiEventTreeMessenger(TTree *HiEventTree)
{
   Initialize(HiEventTree);
}
   
bool HiEventTreeMessenger::Initialize(TTree *HiEventTree)
{
   Tree = HiEventTree;
   return Initialize();
}

bool HiEventTreeMessenger::Initialize()
{
   if(Tree == nullptr)
      return false;

   npus = nullptr;
   tnpus = nullptr;

   if(Tree->GetBranch("hiHF"))          Tree->SetBranchAddress("hiHF", &hiHF);
   else                                 hiHF = 0;
   if(Tree->GetBranch("hiBin"))         Tree->SetBranchAddress("hiBin", &hiBin);
   else                                 hiBin = 0;
   if(Tree->GetBranch("run"))           Tree->SetBranchAddress("run", &Run);
   else                                 Run = 1;
   if(Tree->GetBranch("evt"))           Tree->SetBranchAddress("evt", &Event);
   else                                 Event = 1;
   if(Tree->GetBranch("lumi"))          Tree->SetBranchAddress("lumi", &Lumi);
   else                                 Lumi = 1;
   if(Tree->GetBranch("weight"))        Tree->SetBranchAddress("weight", &weight);
   else                                 weight = 1;
   if(Tree->GetBranch("vx"))            Tree->SetBranchAddress("vx", &vx);
   else                                 vx = -999;
   if(Tree->GetBranch("vy"))            Tree->SetBranchAddress("vy", &vy);
   else                                 vy = -999;
   if(Tree->GetBranch("vz"))            Tree->SetBranchAddress("vz", &vz);
   else                                 vz = -999;
   if(Tree->GetBranch("npus"))          Tree->SetBranchAddress("npus", &npus);
   else                                 npus = &EmptyVectors::EmptyVectorInt;
   if(Tree->GetBranch("tnpus"))         Tree->SetBranchAddress("tnpus", &tnpus);
   else                                 tnpus = &EmptyVectors::EmptyVectorFloat;
   if(Tree->GetBranch("hiHFplus"))      Tree->SetBranchAddress("hiHFplus", &hiHFplus);
   else                                 hiHFplus = 0;
   if(Tree->GetBranch("hiHFminus"))     Tree->SetBranchAddress("hiHFminus", &hiHFminus);
   else                                 hiHFminus = 0;
   if(Tree->GetBranch("hiHFplusEta4"))  Tree->SetBranchAddress("hiHFplusEta4", &hiHFplusEta4);
   else                                 hiHFplusEta4 = 0;
   if(Tree->GetBranch("hiHFminusEta4")) Tree->SetBranchAddress("hiHFminusEta4", &hiHFminusEta4);
   else                                 hiHFminusEta4 = 0;
   if(Tree->GetBranch("hiNevtPlane"))   Tree->SetBranchAddress("hiNevtPlane", &hiNevtPlane);
   else                                 hiNevtPlane = 0;
   if(Tree->GetBranch("hiEvtPlanes"))   Tree->SetBranchAddress("hiEvtPlanes", &hiEvtPlanes);
   
   return true;
}

bool HiEventTreeMessenger::GetEntry(int iEntry)
{
   if(Tree == nullptr)
      return false;

   Tree->GetEntry(iEntry);

   return true;
}
   
int HiEventTreeMessenger::GetEntries()
{
   if(Tree == nullptr)
      return 0;

   return Tree->GetEntries();
}

METFilterTreeMessenger::METFilterTreeMessenger(TFile &File)
{
   Tree = (TTree *)File.Get("l1MetFilterRecoTree/MetFilterRecoTree");
   Initialize();
}

METFilterTreeMessenger::METFilterTreeMessenger(TFile *File)
{
   if(File != nullptr)
      Tree = (TTree *)File->Get("l1MetFilterRecoTree/MetFilterRecoTree");
   else
      Tree = nullptr;
   Initialize();
}

METFilterTreeMessenger::METFilterTreeMessenger(TTree *METFilterTree)
{
   Initialize(METFilterTree);
}

bool METFilterTreeMessenger::Initialize(TTree *METFilterTree)
{
   Tree = METFilterTree;
   return Initialize();
}

bool METFilterTreeMessenger::Initialize()
{
   if(Tree == nullptr)
      return false;

  if (Tree->GetBranch("cscTightHalo2015Filter"))
    Tree->SetBranchAddress("cscTightHalo2015Filter", &cscTightHalo2015Filter);
  else
    cscTightHalo2015Filter = false;
  return true;
}
bool METFilterTreeMessenger::GetEntry(int iEntry)
{
   if(Tree == nullptr)
      return false;

   Tree->GetEntry(iEntry);
   return true;
}

GGTreeMessenger::GGTreeMessenger(TFile &File, std::string TreeName)
{
   Tree = (TTree *)File.Get(TreeName.c_str());
   Initialize();
}

GGTreeMessenger::GGTreeMessenger(TFile *File, std::string TreeName)
{
   if(File != nullptr)
      Tree = (TTree *)File->Get(TreeName.c_str());
   else
      Tree = nullptr;
   Initialize();
}

GGTreeMessenger::GGTreeMessenger(TTree *EventTree)
{
   Tree = EventTree;
   Initialize();
}

bool GGTreeMessenger::Initialize(TTree *EventTree)
{
   Tree = EventTree;
   return Initialize();
}

bool GGTreeMessenger::Initialize()
{
   if(Tree == nullptr)
      return false;

   PUCount = nullptr;
   PUBX = nullptr;
   PUTrue = nullptr;

   if(Tree->GetBranch("nPUInfo"))   Tree->SetBranchAddress("nPUInfo", &NPUInfo);
   else                             NPUInfo = 0;
   if(Tree->GetBranch("nPU"))       Tree->SetBranchAddress("nPU", &PUCount);
   else                             PUCount = &EmptyVectors::EmptyVectorInt;
   if(Tree->GetBranch("puBX"))      Tree->SetBranchAddress("puBX", &PUBX);
   else                             PUBX = &EmptyVectors::EmptyVectorInt;
   if(Tree->GetBranch("puTrue"))    Tree->SetBranchAddress("puTrue", &PUTrue);
   else                             PUTrue = &EmptyVectors::EmptyVectorFloat;
   
   PFJetPT = nullptr;
   PFJetEta = nullptr;
   PFJetPhi = nullptr;
   CaloJetPT = nullptr;
   CaloJetEta = nullptr;
   CaloJetPhi = nullptr;
   GenJetPT = nullptr;
   GenJetEta = nullptr;
   GenJetPhi = nullptr;

   if(Tree->GetBranch("nPFJt"))         Tree->SetBranchAddress("nPFJt", &PFJetCount);
   else                                 PFJetCount = 0;
   if(Tree->GetBranch("pfJtPt"))        Tree->SetBranchAddress("pfJtPt", &PFJetPT);
   else                                 PFJetPT = &EmptyVectors::EmptyVectorFloat;
   if(Tree->GetBranch("pfJtEta"))       Tree->SetBranchAddress("pfJtEta", &PFJetEta);
   else                                 PFJetEta = &EmptyVectors::EmptyVectorFloat;
   if(Tree->GetBranch("pfJtPhi"))       Tree->SetBranchAddress("pfJtPhi", &PFJetPhi);
   else                                 PFJetPhi = &EmptyVectors::EmptyVectorFloat;
   if(Tree->GetBranch("nCaloJt"))       Tree->SetBranchAddress("nCaloJt", &CaloJetCount);
   else                                 CaloJetCount = 0;
   if(Tree->GetBranch("caloJtPt"))      Tree->SetBranchAddress("caloJtPt", &CaloJetPT);
   else                                 CaloJetPT = &EmptyVectors::EmptyVectorFloat;
   if(Tree->GetBranch("caloJtEta"))     Tree->SetBranchAddress("caloJtEta", &CaloJetEta);
   else                                 CaloJetEta = &EmptyVectors::EmptyVectorFloat;
   if(Tree->GetBranch("caloJtPhi"))     Tree->SetBranchAddress("caloJtPhi", &CaloJetPhi);
   else                                 CaloJetPhi = &EmptyVectors::EmptyVectorFloat;
   if(Tree->GetBranch("nGenJt"))        Tree->SetBranchAddress("nGenJt", &GenJetCount);
   else                                 GenJetCount = 0;
   if(Tree->GetBranch("genJtPt"))       Tree->SetBranchAddress("genJtPt", &GenJetPT);
   else                                 GenJetPT = &EmptyVectors::EmptyVectorFloat;
   if(Tree->GetBranch("genJtEta"))      Tree->SetBranchAddress("genJtEta", &GenJetEta);
   else                                 GenJetEta = &EmptyVectors::EmptyVectorFloat;
   if(Tree->GetBranch("genJtPhi"))      Tree->SetBranchAddress("genJtPhi", &GenJetPhi);
   else                                 GenJetPhi = &EmptyVectors::EmptyVectorFloat;

   return true;
}

bool GGTreeMessenger::GetEntry(int iEntry)
{
   if(Tree == nullptr)
      return false;

   Tree->GetEntry(iEntry);
   return true;
}

RhoTreeMessenger::RhoTreeMessenger(TFile &File, std::string TreeName)
{
   Tree = (TTree *)File.Get(TreeName.c_str());
   Initialize();
}

RhoTreeMessenger::RhoTreeMessenger(TFile *File, std::string TreeName)
{
   if(File != nullptr)
      Tree = (TTree *)File->Get(TreeName.c_str());
   else
      Tree = nullptr;
   Initialize();
}

RhoTreeMessenger::RhoTreeMessenger(TTree *RhoTree)
{
   Initialize(RhoTree);
}
   
bool RhoTreeMessenger::Initialize(TTree *RhoTree)
{
   Tree = RhoTree;
   return Initialize();
}

bool RhoTreeMessenger::Initialize()
{
   if(Tree == nullptr)
      return false;

   EtaMin = nullptr;
   EtaMax = nullptr;
   Rho = nullptr;
   RhoM = nullptr;

   if(Tree->GetBranch("etaMin")) Tree->SetBranchAddress("etaMin", &EtaMin);
   else                          EtaMin = &EmptyVectors::EmptyVectorDouble;
   if(Tree->GetBranch("etaMax")) Tree->SetBranchAddress("etaMax", &EtaMax);
   else                          EtaMax = &EmptyVectors::EmptyVectorDouble;
   if(Tree->GetBranch("rho"))    Tree->SetBranchAddress("rho", &Rho);
   else                          Rho = &EmptyVectors::EmptyVectorDouble;
   if(Tree->GetBranch("rhom"))   Tree->SetBranchAddress("rhom", &RhoM);
   else                          RhoM = &EmptyVectors::EmptyVectorDouble;

   return true;
}

bool RhoTreeMessenger::GetEntry(int iEntry)
{
   if(Tree == nullptr)
      return false;

   Tree->GetEntry(iEntry);
   return true;
}

SkimTreeMessenger::SkimTreeMessenger(TFile &File)
{
   Tree = (TTree *)File.Get("skimanalysis/HltTree");
   Initialize();
}

SkimTreeMessenger::SkimTreeMessenger(TFile *File)
{
   if(File != nullptr)
      Tree = (TTree *)File->Get("skimanalysis/HltTree");
   else
      Tree = nullptr;
   Initialize();
}

SkimTreeMessenger::SkimTreeMessenger(TTree *SkimTree)
{
   Initialize(SkimTree);
}

bool SkimTreeMessenger::Initialize(TTree *SkimTree)
{
   Tree = SkimTree;
   return Initialize();
}

bool SkimTreeMessenger::Initialize()
{
   if(Tree == nullptr)
      return false;

   if(Tree->GetBranch("HBHENoiseFilterResult"))
      Tree->SetBranchAddress("HBHENoiseFilterResult", &HBHENoise);
   else
      HBHENoise = 1;
   if(Tree->GetBranch("HBHENoiseFilterResultRun2Loose"))
      Tree->SetBranchAddress("HBHENoiseFilterResultRun2Loose", &HBHENoiseRun2Loose);
   else
      HBHENoiseRun2Loose = 1;
   if(Tree->GetBranch("pprimaryVertexFilter"))
      Tree->SetBranchAddress("pprimaryVertexFilter", &PVFilter);
   else if(Tree->GetBranch("pPAprimaryVertexFilter"))
      Tree->SetBranchAddress("pPAprimaryVertexFilter", &PVFilter);
   else
      PVFilter = 1;
   if(Tree->GetBranch("pBeamScrapingFilter"))
      Tree->SetBranchAddress("pBeamScrapingFilter", &BeamScrapingFilter);
   else
      BeamScrapingFilter = 1;
   if(Tree->GetBranch("pclusterCompatibilityFilter"))
      Tree->SetBranchAddress("pclusterCompatibilityFilter", &ClusterCompatibilityFilter);
   else
      ClusterCompatibilityFilter = 1;
   if(Tree->GetBranch("phfCoincFilter3"))
      Tree->SetBranchAddress("phfCoincFilter3", &HFCoincidenceFilter);
   else
      HFCoincidenceFilter = 1;
   if(Tree->GetBranch("phfCoincFilter2Th4"))
      Tree->SetBranchAddress("phfCoincFilter2Th4", &HFCoincidenceFilter2Th4);
   else
      HFCoincidenceFilter2Th4 = 1;
   if(Tree->GetBranch("pcollisionEventSelection"))
      Tree->SetBranchAddress("pcollisionEventSelection", &CollisionEventSelection);
   else
      CollisionEventSelection = 1;

   return true;
}

bool SkimTreeMessenger::PassBasicFilter()
{
   if(HBHENoise == 0)
      return false;
   if(HBHENoiseRun2Loose == 0)
      return false;
   if(PVFilter == 0)
      return false;
   if(ClusterCompatibilityFilter == 0)
      return false;
   if(HFCoincidenceFilter2Th4 == 0)
      return false;
   if(CollisionEventSelection == 0)
      return false;

   return true;
}

bool SkimTreeMessenger::PassBasicFilterLoose()
{
   if(HBHENoiseRun2Loose == 0)
      return false;
   if(PVFilter == 0)
      return false;
   if(ClusterCompatibilityFilter == 0)
      return false;
   if(HFCoincidenceFilter2Th4 == 0)
      return false;
   if(CollisionEventSelection == 0)
      return false;

   return true;
}

bool SkimTreeMessenger::GetEntry(int iEntry)
{
   if(Tree == nullptr)
      return false;

   Tree->GetEntry(iEntry);
   return true;
}

JetTreeMessenger::JetTreeMessenger(TFile &File, std::string TreeName)
{
   Tree = (TTree *)File.Get(TreeName.c_str());
   Initialize();
}

JetTreeMessenger::JetTreeMessenger(TFile *File, std::string TreeName)
{
   if(File != nullptr)
      Tree = (TTree *)File->Get(TreeName.c_str());
   else
      Tree = nullptr;
   Initialize();
}

JetTreeMessenger::JetTreeMessenger(TTree *JetTree)
{
   Initialize(JetTree);
}

bool JetTreeMessenger::Initialize(TTree *JetTree)
{
   Tree = JetTree;
   return Initialize();
}

bool JetTreeMessenger::Initialize()
{
   if(Tree == nullptr)
      return false;

   JetSubJetPT = nullptr;
   JetSubJetEta = nullptr;
   JetSubJetPhi = nullptr;
   JetSubJetM = nullptr;
   RefGSubJetPT = nullptr;
   RefGSubJetEta = nullptr;
   RefGSubJetPhi = nullptr;
   RefGSubJetM = nullptr;

   if(Tree->GetBranch("pthat"))   Tree->SetBranchAddress("pthat", &PTHat);

   if(Tree->GetBranch("nref"))    Tree->SetBranchAddress("nref", &JetCount);
   else                           JetCount = 0;
   if(Tree->GetBranch("rawpt"))   Tree->SetBranchAddress("rawpt", &JetRawPT);
   if(Tree->GetBranch("jtpt"))    Tree->SetBranchAddress("jtpt", &JetPT);
   if(Tree->GetBranch("jteta"))   Tree->SetBranchAddress("jteta", &JetEta);
   if(Tree->GetBranch("jty"))     Tree->SetBranchAddress("jty", &JetY);
   if(Tree->GetBranch("jtphi"))   Tree->SetBranchAddress("jtphi", &JetPhi);
   if(Tree->GetBranch("jtpu"))    Tree->SetBranchAddress("jtpu", &JetPU);
   if(Tree->GetBranch("jtm"))     Tree->SetBranchAddress("jtm", &JetM);
   if(Tree->GetBranch("jttau1"))  Tree->SetBranchAddress("jttau1", &JetTau1);
   if(Tree->GetBranch("jttau2"))  Tree->SetBranchAddress("jttau2", &JetTau2);
   if(Tree->GetBranch("jttau3"))  Tree->SetBranchAddress("jttau3", &JetTau3);
   if(Tree->GetBranch("jtarea"))  Tree->SetBranchAddress("jtarea", &JetArea);
   if(Tree->GetBranch("discr_csvV1"))  Tree->SetBranchAddress("discr_csvV1", &JetCSVV1);
   if(Tree->GetBranch("discr_csvV2"))  Tree->SetBranchAddress("discr_csvV2", &JetCSVV2);
   if(Tree->GetBranch("ndiscr_csvV1")) Tree->SetBranchAddress("ndiscr_csvV1", &JetCSVV1N);
   if(Tree->GetBranch("ndiscr_csvV2")) Tree->SetBranchAddress("ndiscr_csvV2", &JetCSVV2N);
   if(Tree->GetBranch("pdiscr_csvV1")) Tree->SetBranchAddress("pdiscr_csvV1", &JetCSVV1P);
   if(Tree->GetBranch("pdiscr_csvV2")) Tree->SetBranchAddress("pdiscr_csvV2", &JetCSVV2P);

   if(Tree->GetBranch("jtSubJetPt"))
      Tree->SetBranchAddress("jtSubJetPt", &JetSubJetPT);
   else
      JetSubJetPT = &EmptyVectors::EmptyVectorVectorFloat;
   if(Tree->GetBranch("jtSubJetEta"))
      Tree->SetBranchAddress("jtSubJetEta", &JetSubJetEta);
   else
      JetSubJetEta = &EmptyVectors::EmptyVectorVectorFloat;
   if(Tree->GetBranch("jtSubJetPhi"))
      Tree->SetBranchAddress("jtSubJetPhi", &JetSubJetPhi);
   else
      JetSubJetPhi = &EmptyVectors::EmptyVectorVectorFloat;
   if(Tree->GetBranch("jtSubJetM"))
      Tree->SetBranchAddress("jtSubJetM", &JetSubJetM);
   else
      JetSubJetM = &EmptyVectors::EmptyVectorVectorFloat;
   if(Tree->GetBranch("jtsym"))
      Tree->SetBranchAddress("jtsym", &JetSym);
   else
      for(int i = 0; i < JETCOUNTMAX; i++)
         JetSym[i] = -1;
   if(Tree->GetBranch("jtdroppedBranches"))
      Tree->SetBranchAddress("jtdroppedBranches", &JetDroppedBranches);
   else
      for(int i = 0; i < JETCOUNTMAX; i++)
         JetDroppedBranches[i] = -1;
   
   if(Tree->GetBranch("refpt"))   Tree->SetBranchAddress("refpt", &RefPT);
   if(Tree->GetBranch("refeta"))  Tree->SetBranchAddress("refeta", &RefEta);
   if(Tree->GetBranch("refy"))    Tree->SetBranchAddress("refy", &RefY);
   if(Tree->GetBranch("refphi"))  Tree->SetBranchAddress("refphi", &RefPhi);
   if(Tree->GetBranch("refm"))    Tree->SetBranchAddress("refm", &RefM);
   if(Tree->GetBranch("refarea")) Tree->SetBranchAddress("refarea", &RefArea);
   
   if(Tree->GetBranch("refptG"))  Tree->SetBranchAddress("refptG", &RefGPT);
   if(Tree->GetBranch("refetaG")) Tree->SetBranchAddress("refetaG", &RefGEta);
   if(Tree->GetBranch("refphiG")) Tree->SetBranchAddress("refphiG", &RefGPhi);
   if(Tree->GetBranch("refmG"))   Tree->SetBranchAddress("refmG", &RefGM);

   if(Tree->GetBranch("refparton_pt"))
      Tree->SetBranchAddress("refparton_pt", &RefPartonPT);
   if(Tree->GetBranch("refparton_flavor"))
      Tree->SetBranchAddress("refparton_flavor", &RefPartonFlavor);
   if(Tree->GetBranch("refparton_flavorForB"))
      Tree->SetBranchAddress("refparton_flavorForB", &RefPartonFlavorForB);
   
   if(Tree->GetBranch("refSubJetPt"))
      Tree->SetBranchAddress("refSubJetPt", &RefGSubJetPT);
   else
      RefGSubJetPT = &EmptyVectors::EmptyVectorVectorFloat;
   if(Tree->GetBranch("refSubJetEta"))
      Tree->SetBranchAddress("refSubJetEta", &RefGSubJetEta);
   else
      RefGSubJetEta = &EmptyVectors::EmptyVectorVectorFloat;
   if(Tree->GetBranch("refSubJetPhi"))
      Tree->SetBranchAddress("refSubJetPhi", &RefGSubJetPhi);
   else
      RefGSubJetPhi = &EmptyVectors::EmptyVectorVectorFloat;
   if(Tree->GetBranch("refSubJetM"))
      Tree->SetBranchAddress("refSubJetM", &RefGSubJetM);
   else
      RefGSubJetM = &EmptyVectors::EmptyVectorVectorFloat;
   
   if(Tree->GetBranch("ngen"))    Tree->SetBranchAddress("ngen", &GenCount);
   else                           GenCount = 0;
   
   if(Tree->GetBranch("genmatchindex"))
      Tree->SetBranchAddress("genmatchindex", &GenMatchIndex);
   else
      for(int i = 0; i < JETCOUNTMAX; i++)
         GenMatchIndex[i] = -1;
   if(Tree->GetBranch("genpt"))   Tree->SetBranchAddress("genpt", &GenPT);
   if(Tree->GetBranch("geneta"))  Tree->SetBranchAddress("geneta", &GenEta);
   if(Tree->GetBranch("geny"))    Tree->SetBranchAddress("geny", &GenY);
   if(Tree->GetBranch("genphi"))  Tree->SetBranchAddress("genphi", &GenPhi);
   if(Tree->GetBranch("genm"))    Tree->SetBranchAddress("genm", &GenM);

   if(Tree->GetBranch("hcalSum")) Tree->SetBranchAddress("hcalSum", &HcalSum);
   if(Tree->GetBranch("ecalSum")) Tree->SetBranchAddress("ecalSum", &EcalSum);

   if(Tree->GetBranch("jtPfCHF")) Tree->SetBranchAddress("jtPfCHF", &JetPFCHF);
   if(Tree->GetBranch("jtPfNHF")) Tree->SetBranchAddress("jtPfNHF", &JetPFNHF);
   if(Tree->GetBranch("jtPfCEF")) Tree->SetBranchAddress("jtPfCEF", &JetPFCEF);
   if(Tree->GetBranch("jtPfNEF")) Tree->SetBranchAddress("jtPfNEF", &JetPFNEF);
   if(Tree->GetBranch("jtPfMUF")) Tree->SetBranchAddress("jtPfMUF", &JetPFMUF);
   if(Tree->GetBranch("jtPfCHM")) Tree->SetBranchAddress("jtPfCHM", &JetPFCHM);
   if(Tree->GetBranch("jtPfNHM")) Tree->SetBranchAddress("jtPfNHM", &JetPFNHM);
   if(Tree->GetBranch("jtPfCEM")) Tree->SetBranchAddress("jtPfCEM", &JetPFCEM);
   if(Tree->GetBranch("jtPfNEM")) Tree->SetBranchAddress("jtPfNEM", &JetPFNEM);
   if(Tree->GetBranch("jtPfMUM")) Tree->SetBranchAddress("jtPfMUM", &JetPFMUM);

   return true;
}

bool JetTreeMessenger::GetEntry(int iEntry)
{
   if(Tree == nullptr)
      return false;

   Tree->GetEntry(iEntry);
   return true;
}

GenParticleTreeMessenger::GenParticleTreeMessenger(TFile &File)
{
   Tree = (TTree *)File.Get("HiGenParticleAna/hi");
   Initialize();
}

GenParticleTreeMessenger::GenParticleTreeMessenger(TFile *File)
{
   if(File != nullptr)
      Tree = (TTree *)File->Get("HiGenParticleAna/hi");
   else
      Tree = nullptr;
   Initialize();
}

GenParticleTreeMessenger::GenParticleTreeMessenger(TTree *GenParticleTree)
{
   Initialize(GenParticleTree);
}

bool GenParticleTreeMessenger::Initialize(TTree *GenParticleTree)
{
   Tree = GenParticleTree;
   return Initialize();
}

bool GenParticleTreeMessenger::Initialize()
{
   if(Tree == nullptr)
      return false;

   PT = nullptr;
   Eta = nullptr;
   Phi = nullptr;
   ID = nullptr;
   Charge = nullptr;
   DaughterCount = nullptr;
   SubEvent = nullptr;

   if(Tree->GetBranch("npart")) Tree->SetBranchAddress("npart", &NPart);
   else                         NPart = 0;
   if(Tree->GetBranch("ncoll")) Tree->SetBranchAddress("ncoll", &NColl);
   else                         NColl = 0;
   if(Tree->GetBranch("nhard")) Tree->SetBranchAddress("nhard", &NHard);
   else                         NHard = 0;
   if(Tree->GetBranch("phi0"))  Tree->SetBranchAddress("phi0", &Phi0);
   else                         Phi0 = 0;

   if(Tree->GetBranch("mult"))  Tree->SetBranchAddress("mult", &Mult);
   else                         Mult = 0;
   if(Tree->GetBranch("pt"))    Tree->SetBranchAddress("pt", &PT);
   else                         PT = &EmptyVectors::EmptyVectorFloat;
   if(Tree->GetBranch("eta"))   Tree->SetBranchAddress("eta", &Eta);
   else                         Eta = &EmptyVectors::EmptyVectorFloat;
   if(Tree->GetBranch("phi"))   Tree->SetBranchAddress("phi", &Phi);
   else                         Phi = &EmptyVectors::EmptyVectorFloat;
   if(Tree->GetBranch("pdg"))   Tree->SetBranchAddress("pdg", &ID);
   else                         ID = &EmptyVectors::EmptyVectorInt;
   if(Tree->GetBranch("chg")) Tree->SetBranchAddress("chg", &Charge);
   else                         Charge = &EmptyVectors::EmptyVectorInt;
   if(Tree->GetBranch("nDaughters")) Tree->SetBranchAddress("nDaughters", &DaughterCount);
   else                         DaughterCount = &EmptyVectors::EmptyVectorInt;
   if(Tree->GetBranch("sube"))  Tree->SetBranchAddress("sube", &SubEvent);
   else                         SubEvent = &EmptyVectors::EmptyVectorInt;

   return true;
}

bool GenParticleTreeMessenger::GetEntry(int iEntry)
{
   if(Tree == nullptr)
      return false;

   Tree->GetEntry(iEntry);
   return true;
}

PFTreeMessenger::PFTreeMessenger(TFile &File, std::string TreeName)
{
   Initialize(File, TreeName);
}

PFTreeMessenger::PFTreeMessenger(TFile *File, std::string TreeName)
{
   Initialize(File, TreeName);
}

PFTreeMessenger::PFTreeMessenger(TTree *PFTree)
{
   Initialize(PFTree);
}

bool PFTreeMessenger::Initialize(TFile &File, std::string TreeName)
{
   Tree = (TTree *)File.Get(TreeName.c_str());
   return Initialize();
}

bool PFTreeMessenger::Initialize(TFile *File, std::string TreeName)
{
   if(File != nullptr)
      Tree = (TTree *)File->Get(TreeName.c_str());
   else
      Tree = nullptr;
   return Initialize();
}

bool PFTreeMessenger::Initialize(TTree *PFTree)
{
   Tree = PFTree;
   return Initialize();
}

bool PFTreeMessenger::Initialize()
{
   if(Tree == nullptr)
      return false;

   ID = nullptr;
   PT = nullptr;
   E = nullptr;
   Eta = nullptr;
   Phi = nullptr;
   M = nullptr;

   if(Tree->GetBranch("pfId"))     Tree->SetBranchAddress("pfId", &ID);
   else                            ID = &EmptyVectors::EmptyVectorInt;
   if(Tree->GetBranch("pfPt"))     Tree->SetBranchAddress("pfPt", &PT);
   else                            PT = &EmptyVectors::EmptyVectorFloat;
   if(Tree->GetBranch("pfEnergy")) Tree->SetBranchAddress("pfEnergy", &E);
   else if(Tree->GetBranch("pfE")) Tree->SetBranchAddress("pfE", &E);
   else                            E = &EmptyVectors::EmptyVectorFloat;
   if(Tree->GetBranch("pfEta"))    Tree->SetBranchAddress("pfEta", &Eta);
   else                            Eta = &EmptyVectors::EmptyVectorFloat;
   if(Tree->GetBranch("pfPhi"))    Tree->SetBranchAddress("pfPhi", &Phi);
   else                            Phi = &EmptyVectors::EmptyVectorFloat;
   if(Tree->GetBranch("pfM"))      Tree->SetBranchAddress("pfM", &M);
   else                            M = &EmptyVectors::EmptyVectorFloat;

   return true;
}

bool PFTreeMessenger::GetEntry(int iEntry)
{
   if(Tree == nullptr)
      return false;

   Tree->GetEntry(iEntry);
   return true;
}

TriggerTreeMessenger::TriggerTreeMessenger(TFile &File, std::string TreeName)
{
   Tree = (TTree *)File.Get(TreeName.c_str());
   Initialize();
}

TriggerTreeMessenger::TriggerTreeMessenger(TFile *File, std::string TreeName)
{
   if(File != nullptr)
      Tree = (TTree *)File->Get(TreeName.c_str());
   else
      Tree = nullptr;
   Initialize();
}

TriggerTreeMessenger::TriggerTreeMessenger(TTree *TriggerTree)
{
   Initialize(TriggerTree);
}

bool TriggerTreeMessenger::Initialize(TTree *TriggerTree)
{
   Tree = TriggerTree;
   return Initialize();
}

bool TriggerTreeMessenger::Initialize()
{
   if(Tree == nullptr)
      return false;

   if(Tree->GetBranch("Run"))   Tree->SetBranchAddress("Run", &Run);
   else                         Run = 0;
   if(Tree->GetBranch("LumiBlock")) Tree->SetBranchAddress("LumiBlock", &Lumi);
   else                         Lumi = 0;
   if(Tree->GetBranch("Event")) Tree->SetBranchAddress("Event", &Event);
   else                         Event = 0;

   FillTriggerNames();

   for(int i = 0; i < (int)Name.size(); i++)
   {
      if(Tree->GetBranch(Name[i].c_str()))
      {
         Tree->SetBranchAddress(Name[i].c_str(), &Decision[i]);
         Exist[i] = true;
         
         if(Tree->GetBranch((Name[i] + "_Prescl").c_str()))
         {
            Tree->SetBranchAddress((Name[i] + "_Prescl").c_str(), &Prescale[i]);
            PrescaleExist[i] = true;
         }
         else
         {
            Prescale[i] = -1;
            PrescaleExist[i] = false;
         }
      }
      else
      {
         Decision[i] = false;
         Exist[i] = false;
      }
   }

   return true;
}

bool TriggerTreeMessenger::GetEntry(int iEntry)
{
   if(Tree == nullptr)
      return false;

   Tree->GetEntry(iEntry);
   return true;
}
   
void TriggerTreeMessenger::FillTriggerNames()
{
   Name.clear();
   Decision.clear();
   
   // 2023 triggers UPCs
   Name.push_back("HLT_HIUPC_SingleJet8_ZDC1nXOR_MaxPixelCluster50000_v2");
   Name.push_back("HLT_HIUPC_SingleJet8_ZDC1nAsymXOR_MaxPixelCluster50000_v1");
   Name.push_back("HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000_v8");
   Name.push_back("HLT_HIUPC_ZDC1nOR_SinglePixelTrackLowPt_MaxPixelCluster400_v8");

   // 2018 triggers
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixBypass_part0_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixBypass_part1_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixBypass_part2_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixBypass_part3_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixBypass_part4_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixBypass_part5_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixBypass_part6_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixBypass_part7_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixBypass_part8_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixBypass_part9_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixBypass_part10_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixBypass_part11_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixBypass_part12_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixBypass_part13_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixBypass_part14_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixBypass_part15_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixBypass_part16_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixBypass_part17_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixBypass_part18_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixBypass_part19_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixGated_part0_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixGated_part1_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixGated_part2_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixGated_part3_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixGated_part4_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixGated_part5_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixGated_part6_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixGated_part7_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixGated_part8_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixGated_part9_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixGated_part10_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixGated_part11_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixGated_part12_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixGated_part13_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixGated_part14_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixGated_part15_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixGated_part16_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixGated_part17_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixGated_part18_v1");
   Name.push_back("HLT_HIMinimumBias_SinglePixelTrack_NpixGated_part19_v1");

   // 2018 AA trigger
   Name.push_back("HLT_HIPuAK4CaloJet100Eta5p1_v1");
   Name.push_back("HLT_HIPuAK4CaloJet80Eta5p1_v1");
   Name.push_back("HLT_HIPuAK4CaloJet60Eta5p1_v1");
   Name.push_back("HLT_HIPuAK4CaloJet60Eta5p1_v1");
   Name.push_back("HLT_HIPuAK4CaloJet40Eta5p1_v1");

   // AA trigger
   Name.push_back("HLT_HIPuAK4CaloJet100_Eta5p1_v1");
   Name.push_back("HLT_HIPuAK4CaloJet80_Eta5p1_v1");
   Name.push_back("HLT_HIPuAK4CaloJet60_Eta5p1_v1");
   Name.push_back("HLT_HIPuAK4CaloJet60_Eta5p1_v1");
   Name.push_back("HLT_HIPuAK4CaloJet40_Eta5p1_v1");

   Name.push_back("HLT_PAAK4PFJet40_Eta5p1v2");
   Name.push_back("HLT_PAAK4PFJet60_Eta5p1_v2");
   Name.push_back("HLT_PAAK4PFJet80_Eta5p1_v2");

   // pp trigger
   Name.push_back("HLT_AK4PFJet80_Eta5p1_v1");
   Name.push_back("HLT_HIAK4PFJet40_v1");
   Name.push_back("HLT_HIAK4PFJet60_v1");
   Name.push_back("HLT_HIAK4PFJet80_v1");
   Name.push_back("HLT_HIAK4PFJet100_v1");
   Name.push_back("HLT_HIAK4PFJet120_v1");

   // L1 pass through
   Name.push_back("HLT_L1SingleJet8_v1_BptxAND_v1");
   Name.push_back("HLT_L1SingleJet12_v1_BptxAND_v1");
   Name.push_back("HLT_L1SingleJet16_v1");
   Name.push_back("HLT_L1SingleJet20_v1");
   Name.push_back("HLT_L1SingleJet35_v1");
   Name.push_back("HLT_L1SingleJet40_v1");
   Name.push_back("HLT_L1SingleJet44_v1");
   Name.push_back("HLT_L1SingleJet60_v1");

   // pA temporary triggers - v16
   // Name.push_back("HLT_PAAK4CaloBJetCSVv160_Eta2p1_v1");   // same in v18
   // Name.push_back("HLT_PAAK4CaloBJetCSVv260_Eta2p1_v1");   // same in v18
   Name.push_back("HLT_PAAK4PFJetCSV60_Eta2p1_v2");

   // pA temporary triggers - v22
   Name.push_back("HLT_PAAK4CaloJet60_Eta2p1_v1");
   Name.push_back("HLT_PAAK4CaloJet80_Eta2p1_v1");
   Name.push_back("HLT_PAAK4CaloJet100_Eta2p1_v1");
   Name.push_back("HLT_PAAK4CaloBJetCSVv160_Eta2p1_v1");
   Name.push_back("HLT_PAAK4CaloBJetCSVv180_Eta2p1_v1");
   Name.push_back("HLT_PAAK4CaloBJetCSVv1100_Eta2p1_v1");
   Name.push_back("HLT_PAAK4CaloBJetCSVv260_Eta2p1_v1");
   Name.push_back("HLT_PAAK4CaloBJetCSVv280_Eta2p1_v1");
   Name.push_back("HLT_PAAK4CaloBJetCSVv2100_Eta2p1_v1");
   Name.push_back("HLT_PAAK4PFJet60_Eta2p1_v1");
   Name.push_back("HLT_PAAK4PFJet80_Eta2p1_v1");
   Name.push_back("HLT_PAAK4PFJet100_Eta2p1_v1");
   Name.push_back("HLT_PAAK4PFBJetCSV60_Eta2p1_v1");
   Name.push_back("HLT_PAAK4PFBJetCSV80_Eta2p1_v1");
   Name.push_back("HLT_PAAK4PFBJetCSV100_Eta2p1_v1");

   // pA temporary triggers - v24
   Name.push_back("HLT_L1ZeroBias_BptxAND_v1");
   
   // pA temporary triggers - v27
   Name.push_back("HLT_PAAK4CaloJet40_Eta2p1_v1");
   Name.push_back("HLT_PAAK4CaloBJetCSV40_Eta2p1_v1");
   Name.push_back("HLT_PAAK4CaloBJetCSV60_Eta2p1_v1");
   Name.push_back("HLT_PAAK4CaloBJetCSV80_Eta2p1_v1");
   Name.push_back("HLT_PAAK4PFJet40_Eta2p1_v1");
   Name.push_back("HLT_PAAK4PFBJetCSV40_Eta2p1_v1");
   
   // pA temporary triggers - v31 / JCv1
   Name.push_back("HLT_PAAK4CaloBJetCSV40_CommonTracking_Eta2p1_v1");
   Name.push_back("HLT_PAAK4CaloBJetCSV60_CommonTracking_Eta2p1_v1");
   Name.push_back("HLT_PAAK4CaloBJetCSV80_CommonTracking_Eta2p1_v1");

   // Chris V14
   Name.push_back("HLT_PAAK4CaloJet40_Eta5p1_v2");
   Name.push_back("HLT_PAAK4CaloJet60_Eta5p1_v2");
   Name.push_back("HLT_PAAK4CaloJet80_Eta5p1_v2");
   Name.push_back("HLT_PAAK4CaloJet40_Eta1p9toEta5p1_v2");
   Name.push_back("HLT_PAAK4CaloJet60_Eta1p9toEta5p1_v2");
   Name.push_back("HLT_PAAK4CaloJet40_Eta2p9toEta5p1_v2");
   Name.push_back("HLT_PAAK4CaloJet40_Eta5p1_PAL3Mu3_v2");
   Name.push_back("HLT_PAAK4CaloJet40_Eta5p1_PAL3Mu5_v2");
   Name.push_back("HLT_PAAK4CaloJet60_Eta5p1_PAL3Mu3_v2");
   Name.push_back("HLT_PAAK4CaloJet60_Eta5p1_PAL3Mu5_v2");
   Name.push_back("HLT_PAAK4PFJet40_Eta5p1_v2");
   Name.push_back("HLT_PAAK4PFJet60_Eta5p1_v2");
   Name.push_back("HLT_PAAK4PFJet80_Eta5p1_v2");
   Name.push_back("HLT_PAAK4PFJet60_Eta1p9toEta5p1_v2");
   Name.push_back("HLT_PAAK4PFJet40_Eta1p9toEta5p1_v2");
   Name.push_back("HLT_PAAK4PFJet40_Eta2p9toEta5p1_v2");
   Name.push_back("HLT_PADiAK4CaloJetAve40_Eta5p1_v2");
   Name.push_back("HLT_PADiAK4CaloJetAve60_Eta5p1_v2");
   Name.push_back("HLT_PADiAK4CaloJetAve80_Eta5p1_v2");
   Name.push_back("HLT_PADiAK4PFJetAve40_Eta5p1_v2");
   Name.push_back("HLT_PADiAK4PFJetAve60_Eta5p1_v2");
   Name.push_back("HLT_PADiAK4PFJetAve80_Eta5p1_v2");
   Name.push_back("HLT_PASinglePhoton10_Eta3p1_v2");
   Name.push_back("HLT_PASinglePhoton15_Eta3p1_v2");
   Name.push_back("HLT_PASinglePhoton20_Eta3p1_v2");
   Name.push_back("HLT_PASinglePhoton30_Eta3p1_v2");
   Name.push_back("HLT_PADoublePhoton15_Eta3p1_Mass50_1000_v2");
   Name.push_back("HLT_PASinglePhoton10_Eta3p1_PAL3Mu3_v2");
   Name.push_back("HLT_PASinglePhoton10_Eta3p1_PAL3Mu5_v2");
   Name.push_back("HLT_PASinglePhoton15_Eta3p1_PAL3Mu3_v2");
   Name.push_back("HLT_PASinglePhoton15_Eta3p1_PAL3Mu5_v2");
   Name.push_back("HLT_PASinglePhoton20_Eta3p1_PAL3Mu3_v2");
   Name.push_back("HLT_PASinglePhoton20_Eta3p1_PAL3Mu5_v2");

   Name.push_back("L1_SingleJet12_BptxAND");
   
   Name.push_back("HLT_PAAK4CaloJet40_Eta5p1_v1");
   Name.push_back("HLT_PAAK4CaloJet60_Eta5p1_v1");
   Name.push_back("HLT_PAAK4CaloJet80_Eta5p1_v1");
   Name.push_back("HLT_PAAK4CaloJet100_Eta5p1_v1");
   Name.push_back("HLT_PAAK4PFJet40_Eta5p1_v1");
   Name.push_back("HLT_PAAK4PFJet60_Eta5p1_v1");
   Name.push_back("HLT_PAAK4PFJet60_Eta5p1_v3");
   Name.push_back("HLT_PAAK4PFJet80_Eta5p1_v1");
   Name.push_back("HLT_PAAK4PFJet100_Eta5p1_v1");

   Name.push_back("L1_MinimumBiasHF0_OR_BptxAND_Initial");
   Name.push_back("L1_MinimumBiasHF0_OR_BptxAND_Final");
   Name.push_back("L1_MinimumBiasHF0_OR_BptxAND");
   Name.push_back("HLT_PAL1MinimumBiasHF_AND_SinglePixelTrack_v1");
   Name.push_back("HLT_PAL1MinimumBiasHF_AND_SinglePixelTrack_v2");
   Name.push_back("HLT_PAL1MinimumBiasHF_AND_SinglePixelTrack_v3");
   Name.push_back("HLT_PAL1MinimumBiasHF_AND_SinglePixelTrack_v4");
   Name.push_back("HLT_PAL1MinimumBiasHF_AND_SinglePixelTrack_v5");

   Name.push_back("L1_SingleJet12_BptxAND_Initial");
   Name.push_back("L1_SingleJet12_BptxAND_Final");
   Name.push_back("L1_SingleJet16_BptxAND_Initial");
   Name.push_back("L1_SingleJet16_BptxAND_Final");
   Name.push_back("L1_SingleJet24_BptxAND_Initial");
   Name.push_back("L1_SingleJet24_BptxAND_Final");
   Name.push_back("L1_SingleJet36_BptxAND_Initial");
   Name.push_back("L1_SingleJet36_BptxAND_Final");
   Name.push_back("L1_SingleJet52_BptxAND_Initial");
   Name.push_back("L1_SingleJet52_BptxAND_Final");
   
   Name.push_back("L1_SingleJet12_ForEta1p5_BptxAND_Initial");
   Name.push_back("L1_SingleJet12_ForEta1p5_BptxAND_Final");
   Name.push_back("L1_SingleJet12_ForEta2p5_BptxAND_Initial");
   Name.push_back("L1_SingleJet12_ForEta2p5_BptxAND_Final");
   Name.push_back("L1_SingleJet24_ForEta1p5_BptxAND_Initial");
   Name.push_back("L1_SingleJet24_ForEta1p5_BptxAND_Final");
   Name.push_back("L1_SingleJet24_ForEta2p5_BptxAND_Initial");
   Name.push_back("L1_SingleJet24_ForEta2p5_BptxAND_Final");

   Name.push_back("HLT_PAAK4CaloJet40_Eta5p1_v3");
   Name.push_back("HLT_PAAK4CaloJet60_Eta5p1_v3");
   Name.push_back("HLT_PAAK4CaloJet80_Eta5p1_v3");
   Name.push_back("HLT_PAAK4CaloJet40_Eta1p9toEta5p1_v3");
   Name.push_back("HLT_PAAK4CaloJet60_Eta1p9toEta5p1_v3");
   Name.push_back("HLT_PAAK4CaloJet40_Eta2p9toEta5p1_v3");
   Name.push_back("HLT_PAAK4CaloJet40_Eta5p1_PAL3Mu3_v3");
   Name.push_back("HLT_PAAK4CaloJet40_Eta5p1_PAL3Mu5_v3");
   Name.push_back("HLT_PAAK4CaloJet60_Eta5p1_PAL3Mu3_v3");
   Name.push_back("HLT_PAAK4CaloJet60_Eta5p1_PAL3Mu5_v3");
   Name.push_back("HLT_PAAK4PFJet40_Eta5p1_v3");
   Name.push_back("HLT_PAAK4PFJet60_Eta5p1_v3");
   Name.push_back("HLT_PAAK4PFJet60_Eta5p1_v4");
   Name.push_back("HLT_PAAK4PFJet80_Eta5p1_v3");
   Name.push_back("HLT_PAAK4PFJet60_Eta1p9toEta5p1_v3");
   Name.push_back("HLT_PAAK4PFJet40_Eta1p9toEta5p1_v3");
   Name.push_back("HLT_PAAK4PFJet40_Eta2p9toEta5p1_v3");
   Name.push_back("HLT_PADiAK4CaloJetAve40_Eta5p1_v3");
   Name.push_back("HLT_PADiAK4CaloJetAve60_Eta5p1_v3");
   Name.push_back("HLT_PADiAK4CaloJetAve80_Eta5p1_v3");
   Name.push_back("HLT_PADiAK4PFJetAve40_Eta5p1_v3");
   Name.push_back("HLT_PADiAK4PFJetAve60_Eta5p1_v3");
   Name.push_back("HLT_PADiAK4PFJetAve80_Eta5p1_v3");
   Name.push_back("HLT_PASinglePhoton10_Eta3p1_v3");
   Name.push_back("HLT_PASinglePhoton15_Eta3p1_v3");
   Name.push_back("HLT_PASinglePhoton20_Eta3p1_v3");
   Name.push_back("HLT_PASinglePhoton30_Eta3p1_v3");
   Name.push_back("HLT_PADoublePhoton15_Eta3p1_Mass50_1000_v3");
   Name.push_back("HLT_PASinglePhoton10_Eta3p1_PAL3Mu3_v3");
   Name.push_back("HLT_PASinglePhoton10_Eta3p1_PAL3Mu5_v3");
   Name.push_back("HLT_PASinglePhoton15_Eta3p1_PAL3Mu3_v3");
   Name.push_back("HLT_PASinglePhoton15_Eta3p1_PAL3Mu5_v3");
   Name.push_back("HLT_PASinglePhoton20_Eta3p1_PAL3Mu3_v3");
   Name.push_back("HLT_PASinglePhoton20_Eta3p1_PAL3Mu5_v3");

   // PP Reference Runs - v4
   Name.push_back("HLT_AK4PFJet30_v15");
   Name.push_back("HLT_AK4PFJet50_v15");
   Name.push_back("HLT_AK4PFJet80_v15");
   Name.push_back("HLT_AK4PFJet100_v15");
   Name.push_back("HLT_AK4PFJet120_v14");

   Name.push_back("HLT_AK4CaloJet30_v10");
   Name.push_back("HLT_AK4CaloJet40_v9");
   Name.push_back("HLT_AK4CaloJet50_v9");
   Name.push_back("HLT_AK4CaloJet80_v9");
   Name.push_back("HLT_AK4CaloJet100_v9");
   Name.push_back("HLT_AK4CaloJet120_v8");

   Name.push_back("HLT_L1SingleJet16_Eta5p1_v1");
   Name.push_back("HLT_L1SingleJet20_Eta5p1_v1");
   Name.push_back("HLT_L1SingleJet28_Eta5p1_v1");
   Name.push_back("HLT_L1SingleJet40_Eta5p1_v1");
   Name.push_back("HLT_L1SingleJet60_Eta5p1_v1");

   // pp reference runs - v6
   Name.push_back("HLT_L1SingleJet20FWD");
   Name.push_back("HLT_L1SingleJet35FWD");
   Name.push_back("HLT_L1SingleJet40FWD");
   Name.push_back("HLT_L1SingleJet50FWD");
   Name.push_back("HLT_L1SingleJet60FWD");
   
   Name.push_back("HLT_AK4PFJet30FWD_v14");
   Name.push_back("HLT_AK4PFJet50FWD_v14");
   Name.push_back("HLT_AK4PFJet60FWD_v14");
   Name.push_back("HLT_AK4PFJet70FWD_v14");
   Name.push_back("HLT_AK4PFJet80FWD_v14");

   // muon triggers
   Name.push_back("HLT_HIL2Mu12_v1");
   Name.push_back("HLT_HIL2Mu12_v2");
   Name.push_back("HLT_HIL2Mu12_v3");
   Name.push_back("HLT_HIL2Mu12_v4");
   Name.push_back("HLT_HIL2Mu12_v5");
   Name.push_back("HLT_HIL3Mu12_v1");
   Name.push_back("HLT_HIL3Mu12_v2");
   Name.push_back("HLT_HIL3Mu12_v3");
   Name.push_back("HLT_HIL3Mu15_v1");
   Name.push_back("HLT_HIL3Mu15_v2");
   Name.push_back("HLT_HIL3Mu15_v3");
   Name.push_back("HLT_HIL3Mu20_v1");
   Name.push_back("HLT_HIL3Mu20_v2");
   Name.push_back("HLT_HIL3Mu20_v3");

   std::sort(Name.begin(), Name.end());
   std::vector<std::string>::iterator iter = std::unique(Name.begin(), Name.end());
   Name.erase(iter, Name.end());
   Decision.resize(Name.size());
   Prescale.resize(Name.size());
   Exist.resize(Name.size());
   PrescaleExist.resize(Name.size());
}

int TriggerTreeMessenger::FindIndex(std::string Trigger)
{
   std::vector<std::string>::iterator iter
      = std::lower_bound(Name.begin(), Name.end(), Trigger);

   if(iter == Name.end())
      return -1;
   if(Trigger == *iter)
      return (iter - Name.begin());
   return -1;
}

int TriggerTreeMessenger::CheckTrigger(std::string Trigger)
{
   return CheckTrigger(FindIndex(Trigger));
}
   
int TriggerTreeMessenger::CheckTriggerStartWith(std::string Trigger)
{
   bool Found = false;
   int Result = 0;

   for(int i = 0; i < (int)Name.size(); i++)
   {
      if(Exist[i] == false || Name[i].find(Trigger) != 0)
         continue;

      Found = true;
      Result = Result + CheckTrigger(i);
   }

   if(Found == true)
      return Result;
   return -1;
}
   
int TriggerTreeMessenger::CheckTrigger(int Index)
{
   if(Index < 0 || Index >= (int)Name.size())
      return -1;
   if(Exist[Index] == false)
      return -1;

   return Decision[Index];
}

int TriggerTreeMessenger::GetPrescale(std::string Trigger)
{
   return GetPrescale(FindIndex(Trigger));
}
   
int TriggerTreeMessenger::GetPrescaleStartWith(std::string Trigger)
{
   for(int i = 0; i < (int)Name.size(); i++)
   {
      if(Exist[i] == false || Name[i].find(Trigger) != 0)
         continue;
      return GetPrescale(i);
   }

   return -1;
}

int TriggerTreeMessenger::GetPrescale(int Index)
{
   if(Index < 0 || Index >= (int)Name.size())
      return -1;
   if(Exist[Index] == false)
      return -1;
   if(PrescaleExist[Index] == false)
      return 1;

   return Prescale[Index];
}

TriggerObjectTreeMessenger::TriggerObjectTreeMessenger(TFile &File, std::string TreeName)
{
   Tree = (TTree *)File.Get(TreeName.c_str());
   Initialize();
}

TriggerObjectTreeMessenger::TriggerObjectTreeMessenger(TFile *File, std::string TreeName)
{
   if(File != nullptr)
      Tree = (TTree *)File->Get(TreeName.c_str());
   else
      Tree = nullptr;
   Initialize();
}

TriggerObjectTreeMessenger::TriggerObjectTreeMessenger(TTree *TriggerTree)
{
   Initialize(TriggerTree);
}

bool TriggerObjectTreeMessenger::Initialize(TTree *TriggerTree)
{
   Tree = TriggerTree;
   return Initialize();
}

bool TriggerObjectTreeMessenger::Initialize()
{
   ID = nullptr;
   PT = nullptr;
   Eta = nullptr;
   Phi = nullptr;
   Mass = nullptr;

   if(Tree == nullptr)
      return false;

   Tree->SetBranchAddress("TriggerObjID", &ID);
   Tree->SetBranchAddress("pt", &PT);
   Tree->SetBranchAddress("eta", &Eta);
   Tree->SetBranchAddress("phi", &Phi);
   Tree->SetBranchAddress("mass", &Mass);

   return true;
}

bool TriggerObjectTreeMessenger::GetEntry(int iEntry)
{
   if(Tree == nullptr)
      return false;

   Tree->GetEntry(iEntry);

   return true;
}

TrackTreeMessenger::TrackTreeMessenger(TFile &File, std::string TreeName)
{
   Tree = (TTree *)File.Get(TreeName.c_str());
   Initialize();
}

TrackTreeMessenger::TrackTreeMessenger(TFile *File, std::string TreeName)
{
   if(File != nullptr)
      Tree = (TTree *)File->Get(TreeName.c_str());
   else
      Tree = nullptr;
   Initialize();
}

TrackTreeMessenger::TrackTreeMessenger(TTree *TrackTree)
{
   Initialize(TrackTree);
}

bool TrackTreeMessenger::Initialize(TTree *TrackTree)
{
   Tree = TrackTree;
   return Initialize();
}

bool TrackTreeMessenger::Initialize()
{
   nVtx = 0;

   if(Tree == nullptr)
      return false;

   Tree->SetBranchAddress("nVtx", &nVtx);
   Tree->SetBranchAddress("nTrkVtx", &nTrkVtx);
   Tree->SetBranchAddress("normChi2Vtx", &normChi2Vtx);
   Tree->SetBranchAddress("sumPtVtx", &sumPtVtx);
   Tree->SetBranchAddress("xVtx", &xVtx);
   Tree->SetBranchAddress("yVtx", &yVtx);
   Tree->SetBranchAddress("zVtx", &zVtx);
   Tree->SetBranchAddress("xVtxErr", &xVtxErr);
   Tree->SetBranchAddress("yVtxErr", &yVtxErr);
   Tree->SetBranchAddress("zVtxErr", &zVtxErr);
   Tree->SetBranchAddress("vtxDist2D", &vtxDist2D);
   Tree->SetBranchAddress("vtxDist2DErr", &vtxDist2DErr);
   Tree->SetBranchAddress("vtxDist2DSig", &vtxDist2DSig);
   Tree->SetBranchAddress("vtxDist3D", &vtxDist3D);
   Tree->SetBranchAddress("vtxDist3DErr", &vtxDist3DErr);
   Tree->SetBranchAddress("vtxDist3DSig", &vtxDist3DSig);
   Tree->SetBranchAddress("nTrk", &nTrk);
   Tree->SetBranchAddress("trkPt", &trkPt);
   Tree->SetBranchAddress("trkPtError", &trkPtError);
   Tree->SetBranchAddress("trkNHit", &trkNHit);
   Tree->SetBranchAddress("trkNlayer", &trkNlayer);
   Tree->SetBranchAddress("trkEta", &trkEta);
   Tree->SetBranchAddress("trkPhi", &trkPhi);
   Tree->SetBranchAddress("trkCharge", &trkCharge);
   Tree->SetBranchAddress("trkNVtx", &trkNVtx);
   Tree->SetBranchAddress("nTrkTimesnVtx", &nTrkTimesnVtx);
   Tree->SetBranchAddress("trkAssocVtx", &trkAssocVtx);
   Tree->SetBranchAddress("trkDxyOverDxyError", &trkDxyOverDxyError);
   Tree->SetBranchAddress("trkDzOverDzError", &trkDzOverDzError);
   Tree->SetBranchAddress("highPurity", &highPurity);
   Tree->SetBranchAddress("tight", &tight);
   Tree->SetBranchAddress("loose", &loose);
   Tree->SetBranchAddress("trkChi2", &trkChi2);
   Tree->SetBranchAddress("trkNdof", &trkNdof);
   Tree->SetBranchAddress("trkDxy1", &trkDxy1);
   Tree->SetBranchAddress("trkDxyError1", &trkDxyError1);
   Tree->SetBranchAddress("trkDz1", &trkDz1);
   Tree->SetBranchAddress("trkDzError1", &trkDzError1);
   Tree->SetBranchAddress("trkFake", &trkFake);
   Tree->SetBranchAddress("trkAlgo", &trkAlgo);
   Tree->SetBranchAddress("trkOriginalAlgo", &trkOriginalAlgo);

   return true;
}

bool TrackTreeMessenger::GetEntry(int iEntry)
{
   if(Tree == nullptr)
      return false;

   nVtx = 0;
   Tree->GetEntry(iEntry);

   return true;
}

int TrackTreeMessenger::GetBestVertexIndex()
{
   if(nVtx == 0)
      return -1;
   if(nVtx == 1)
      return 0;

   int BestIndex = 0;
   double BestSumPT = 0;
   for(int iV = 0; iV < nVtx; iV++)
   {
      if(BestSumPT < sumPtVtx[iV])
      {
         BestIndex = iV;
         BestSumPT = sumPtVtx[iV];
      }
   }

   return BestIndex;
}

bool TrackTreeMessenger::PassZHadron2022Cut(int index)
{
   if(index >= nTrk)
      return false;

   if(highPurity[index] == false)
      return false;

   double RelativeUncertainty = trkPtError[index] / trkPt[index];
   if(RelativeUncertainty > 0.1)
      return false;

   // if(trkDxyOverDxyError[index] > 3)
   //    return false;
   if(fabs(trkDxy1[index]) / trkDxyError1[index] > 3)
      return false;

   // if(trkDzOverDzError[index] > 3)
   //    return false;
   if(fabs(trkDz1[index]) / trkDzError1[index] > 3)
      return false;

   if(trkEta[index] < -2.4 || trkEta[index] > 2.4)
      return false;

   return true;
}

bool TrackTreeMessenger::PassZHadron2022CutLoose(int index)
{
   // TODO

   if(index >= nTrk)
      return false;

   if(highPurity[index] == false)
      return false;

   double RelativeUncertainty = trkPtError[index] / trkPt[index];
   if(RelativeUncertainty > 0.15)
      return false;

   // if(trkDxyOverDxyError[index] > 3)
   //    return false;
   if(fabs(trkDxy1[index]) / trkDxyError1[index] > 5)
      return false;

   // if(trkDzOverDzError[index] > 3)
   //    return false;
   if(fabs(trkDz1[index]) / trkDzError1[index] > 5)
      return false;

   if(trkEta[index] < -2.4 || trkEta[index] > 2.4)
      return false;

   return true;
}

bool TrackTreeMessenger::PassZHadron2022CutTight(int index)
{
   // TODO
   
   if(index >= nTrk)
      return false;

   if(highPurity[index] == false)
      return false;

   double RelativeUncertainty = trkPtError[index] / trkPt[index];
   if(RelativeUncertainty > 0.15)
      return false;

   // if(trkDxyOverDxyError[index] > 3)
   //    return false;
   if(fabs(trkDxy1[index]) / trkDxyError1[index] > 2)
      return false;

   // if(trkDzOverDzError[index] > 3)
   //    return false;
   if(fabs(trkDz1[index]) / trkDzError1[index] > 2)
      return false;

   if(trkEta[index] < -2.4 || trkEta[index] > 2.4)
      return false;

   return true;
}

DzeroTreeMessenger::DzeroTreeMessenger(TFile &File, std::string TreeName)
{
   Tree = (TTree *)File.Get(TreeName.c_str());
   Initialize();
}

DzeroTreeMessenger::DzeroTreeMessenger(TFile *File, std::string TreeName)
{
   if(File != nullptr)
      Tree = (TTree *)File->Get(TreeName.c_str());
   else
      Tree = nullptr;
   Initialize();
}

DzeroTreeMessenger::DzeroTreeMessenger(TTree *DzeroTree)
{
   Initialize(DzeroTree);
}

bool DzeroTreeMessenger::Initialize(TTree *DzeroTree)
{
   Tree = DzeroTree;
   return Initialize();
}

bool DzeroTreeMessenger::Initialize()
{
   if(Tree == nullptr)
      return false;
   Tree->SetBranchAddress("Dsize", &Dsize);
   Tree->SetBranchAddress("Dpt", &Dpt);
   Tree->SetBranchAddress("Dy", &Dy);
   Tree->SetBranchAddress("Dmass", &Dmass);
   Tree->SetBranchAddress("Dtrk1Pt", &Dtrk1Pt);
   Tree->SetBranchAddress("Dtrk1Eta", &Dtrk1Eta);
   Tree->SetBranchAddress("Dtrk1PtErr", &Dtrk1PtErr);
   Tree->SetBranchAddress("Dtrk1highPurity", &Dtrk1highPurity);
   Tree->SetBranchAddress("Dtrk2Pt", &Dtrk2Pt);
   Tree->SetBranchAddress("Dtrk2Eta", &Dtrk2Eta);
   Tree->SetBranchAddress("Dtrk2PtErr", &Dtrk2PtErr);
   Tree->SetBranchAddress("Dtrk2highPurity", &Dtrk2highPurity);
   Tree->SetBranchAddress("Dchi2cl", &Dchi2cl);
   Tree->SetBranchAddress("DsvpvDistance", &DsvpvDistance);
   Tree->SetBranchAddress("DsvpvDisErr", &DsvpvDisErr);
   Tree->SetBranchAddress("DsvpvDistance_2D", &DsvpvDistance_2D);
   Tree->SetBranchAddress("DsvpvDisErr_2D", &DsvpvDisErr_2D);
   Tree->SetBranchAddress("Dalpha", &Dalpha);
   Tree->SetBranchAddress("Ddtheta", &Ddtheta);

   return true;
}

bool DzeroTreeMessenger::GetEntry(int iEntry)
{
   if(Tree == nullptr)
      return false;

   Tree->GetEntry(iEntry);

   return true;
}

bool DzeroTreeMessenger::PassUPCDzero2023Cut(int index)
{
   if(index >= Dsize)
      return false;
  //FIXME : need to be cross-checked
  if(Dalpha[index] > 0.4)
      return false;
  if(Ddtheta[index] > 0.5)
      return false;
  if(Dchi2cl[index] < 0.1)
      return false;
  if(fabs(Dtrk1PtErr[index] / Dtrk1Pt[index]) > 0.1)
      return false;
  if(fabs(Dtrk2PtErr[index] / Dtrk2Pt[index]) > 0.1)
      return false;
  if (Dtrk1highPurity[index] == 0 || Dtrk2highPurity[index] == 0)
      return false;
  if (DsvpvDistance[index]/DsvpvDisErr[index] < 2.)
      return false;
   return true;
}

DzeroGenTreeMessenger::DzeroGenTreeMessenger(TFile &File, std::string TreeName)
{
   Tree = (TTree *)File.Get(TreeName.c_str());
   Initialize();
}

DzeroGenTreeMessenger::DzeroGenTreeMessenger(TFile *File, std::string TreeName)
{
   if(File != nullptr)
      Tree = (TTree *)File->Get(TreeName.c_str());
   else
      Tree = nullptr;
   Initialize();
}

DzeroGenTreeMessenger::DzeroGenTreeMessenger(TTree *DzeroGenTree)
{
   Initialize(DzeroGenTree);
}

bool DzeroGenTreeMessenger::Initialize(TTree *DzeroGenTree)
{
   Tree = DzeroGenTree;
   return Initialize();
}

bool DzeroGenTreeMessenger::Initialize()
{
   if(Tree == nullptr)
      return false;

   Tree->SetBranchAddress("Gsize", &Gsize);
   Tree->SetBranchAddress("Gpt", &Gpt);
   Tree->SetBranchAddress("Gy", &Gy);
   Tree->SetBranchAddress("GpdgId", &GpdgId);
   Tree->SetBranchAddress("GisSignal", &GisSignal);
   Tree->SetBranchAddress("GcollisionId", &GcollisionId);
   Tree->SetBranchAddress("GSignalType", &GSignalType);

   return true;
}

bool DzeroGenTreeMessenger::GetEntry(int iEntry)
{
   if(Tree == nullptr)
      return false;

   Tree->GetEntry(iEntry);

   return true;
}

MuTreeMessenger::MuTreeMessenger(TFile &File, std::string TreeName)
{
   Tree = (TTree *)File.Get(TreeName.c_str());
   Initialize();
}

MuTreeMessenger::MuTreeMessenger(TFile *File, std::string TreeName)
{
   if(File != nullptr)
      Tree = (TTree *)File->Get(TreeName.c_str());
   else
      Tree = nullptr;
   Initialize();
}

MuTreeMessenger::MuTreeMessenger(TTree *MuTree)
{
   Initialize(MuTree);
}
   
bool MuTreeMessenger::Initialize(TTree *MuTree)
{
   Tree = MuTree;
   return Initialize();
}

bool MuTreeMessenger::Initialize()
{
   if(Tree == nullptr)
      return false;

   Tree->SetBranchAddress("Run", &Run);
   Tree->SetBranchAddress("Event", &Event);
   Tree->SetBranchAddress("Lumi", &Lumi);
   Tree->SetBranchAddress("vx", &Vx);
   Tree->SetBranchAddress("vy", &Vy);
   Tree->SetBranchAddress("vz", &Vz);
   Tree->SetBranchAddress("Gen_nptl", &NGen);
   Tree->SetBranchAddress("Gen_pid", &GenPID);
   Tree->SetBranchAddress("Gen_mom", &GenMom);
   Tree->SetBranchAddress("Gen_status", &GenStatus);
   Tree->SetBranchAddress("Gen_p", &GenP);
   Tree->SetBranchAddress("Gen_pt", &GenPT);
   Tree->SetBranchAddress("Gen_eta", &GenEta);
   Tree->SetBranchAddress("Gen_phi", &GenPhi);
   Tree->SetBranchAddress("Glb_nptl", &NGlb);
   Tree->SetBranchAddress("Glb_charge", &GlbCharge);
   Tree->SetBranchAddress("Glb_p", &GlbP);
   Tree->SetBranchAddress("Glb_pt", &GlbPT);
   Tree->SetBranchAddress("Glb_eta", &GlbEta);
   Tree->SetBranchAddress("Glb_phi", &GlbPhi);
   Tree->SetBranchAddress("Glb_dxy", &GlbDxy);
   Tree->SetBranchAddress("Glb_dz", &GlbDz);
   Tree->SetBranchAddress("Glb_nValMuHits", &GlbNValMuHits);
   Tree->SetBranchAddress("Glb_nValTrkHits", &GlbNValTrkHits);
   Tree->SetBranchAddress("Glb_nValPixHits", &GlbNValPixHits);
   Tree->SetBranchAddress("Glb_trkLayerWMeas", &GlbTrkLayerWMeas);
   Tree->SetBranchAddress("Glb_nMatchedStations", &GlbNMatchedStations);
   Tree->SetBranchAddress("Glb_nTrkFound", &GlbNTrkFound);
   Tree->SetBranchAddress("Glb_glbChi2_ndof", &GlbGlbChi2NDof);
   Tree->SetBranchAddress("Glb_trkChi2_ndof", &GlbTrkChi2NDof);
   Tree->SetBranchAddress("Glb_pixLayerWMeas", &GlbPixLayerWMeas);
   Tree->SetBranchAddress("Glb_trkDxy", &GlbTrkDxy);
   Tree->SetBranchAddress("Glb_trkDz", &GlbTrkDz);
   Tree->SetBranchAddress("Sta_nptl", &NSta);
   Tree->SetBranchAddress("Sta_charge", &StaCharge);
   Tree->SetBranchAddress("Sta_p", &StaP);
   Tree->SetBranchAddress("Sta_pt", &StaPT);
   Tree->SetBranchAddress("Sta_eta", &StaEta);
   Tree->SetBranchAddress("Sta_phi", &StaPhi);
   Tree->SetBranchAddress("Sta_dxy", &StaDxy);
   Tree->SetBranchAddress("Sta_dz", &StaDz);
   Tree->SetBranchAddress("Glb_isArbitrated", &GlbIsArbitrated);
   Tree->SetBranchAddress("Di_npair", &NDi);
   Tree->SetBranchAddress("Di_vProb", &DiVProb);
   Tree->SetBranchAddress("Di_mass", &DiMass);
   Tree->SetBranchAddress("Di_e", &DiE);
   Tree->SetBranchAddress("Di_pt", &DiPT);
   Tree->SetBranchAddress("Di_pt1", &DiPT1);
   Tree->SetBranchAddress("Di_pt2", &DiPT2);
   Tree->SetBranchAddress("Di_eta", &DiEta);
   Tree->SetBranchAddress("Di_eta1", &DiEta1);
   Tree->SetBranchAddress("Di_eta2", &DiEta2);
   Tree->SetBranchAddress("Di_rapidity", &DiRapidity);
   Tree->SetBranchAddress("Di_phi", &DiPhi);
   Tree->SetBranchAddress("Di_phi1", &DiPhi1);
   Tree->SetBranchAddress("Di_phi2", &DiPhi2);
   Tree->SetBranchAddress("Di_charge", &DiCharge);
   Tree->SetBranchAddress("Di_charge1", &DiCharge1);
   Tree->SetBranchAddress("Di_charge2", &DiCharge2);
   Tree->SetBranchAddress("Di_isArb1", &DiIsArb1);
   Tree->SetBranchAddress("Di_isArb2", &DiIsArb2);
   Tree->SetBranchAddress("Di_nTrkHit1", &DiNTrkHit1);
   Tree->SetBranchAddress("Di_nTrkHit2", &DiNTrkHit2);
   Tree->SetBranchAddress("Di_nMuHit1", &DiNMuHit1);
   Tree->SetBranchAddress("Di_nMuHit2", &DiNMuHit2);
   Tree->SetBranchAddress("Di_nTrkLayers1", &DiNTrkLayers1);
   Tree->SetBranchAddress("Di_nTrkLayers2", &DiNTrkLayers2);
   Tree->SetBranchAddress("Di_nPixHit1", &DiNPixHit1);
   Tree->SetBranchAddress("Di_nPixHit2", &DiNPixHit2);
   Tree->SetBranchAddress("Di_nMatchedStations1", &DiNMatchedStations1);
   Tree->SetBranchAddress("Di_nMatchedStations2", &DiNMatchedStations2);
   Tree->SetBranchAddress("Di_trkChi2_1", &DiTrkChi21);
   Tree->SetBranchAddress("Di_trkChi2_2", &DiTrkChi22);
   Tree->SetBranchAddress("Di_glbChi2_1", &DiGlbChi21);
   Tree->SetBranchAddress("Di_glbChi2_2", &DiGlbChi22);
   Tree->SetBranchAddress("Di_dxy1", &DiDxy1);
   Tree->SetBranchAddress("Di_dxy2", &DiDxy2);
   Tree->SetBranchAddress("Di_dz1", &DiDz1);
   Tree->SetBranchAddress("Di_dz2", &DiDz2);
   
   return true;
}

bool MuTreeMessenger::GetEntry(int iEntry)
{
   if(Tree == nullptr)
      return false;

   Tree->GetEntry(iEntry);
   return true;
}

bool MuTreeMessenger::DimuonPassTightCut(int index)
{
   // If somebody requested a pair that does not exist, we return false
   if(index >= NDi)
      return false;

   bool TightCut = true;

   if(DiGlbChi21[index] >= 10. || DiGlbChi22[index] >= 10.)
      TightCut = false;
   if(DiNMuHit1[index] <= 0 || DiNMuHit2[index] <= 0)
      TightCut = false;
   if(DiNMatchedStations1[index] <=1 || DiNMatchedStations2[index] <= 1)
      TightCut = false;
   if(fabs(DiDxy1[index]) >= 0.2 || fabs(DiDxy2[index]) >= 0.2)
      TightCut = false;
   if(fabs(DiDz1[index]) >= 0.5 || fabs(DiDz2[index]) >= 0.5)
      TightCut = false;
   if(DiNPixHit1[index] <= 0 || DiNPixHit2[index] <= 0)
      TightCut = false;
   if(DiNTrkLayers1[index] <= 5 || DiNTrkLayers2[index] <= 5)
      TightCut = false;

   // Note that the PF muon is not checked right now due to information not stored in the tree
   
   return TightCut;
}

PbPbTrackTreeMessenger::PbPbTrackTreeMessenger(TFile &File, std::string TreeName)
{
   Tree = (TTree *)File.Get(TreeName.c_str());
   Initialize();
}

PbPbTrackTreeMessenger::PbPbTrackTreeMessenger(TFile *File, std::string TreeName)
{
   if(File != nullptr)
      Tree = (TTree *)File->Get(TreeName.c_str());
   else
      Tree = nullptr;
   Initialize();
}

PbPbTrackTreeMessenger::PbPbTrackTreeMessenger(TTree *PbPbTrackTree)
{
   Initialize(PbPbTrackTree);
}
   
bool PbPbTrackTreeMessenger::Initialize(TTree *PbPbTrackTree)
{
   Tree = PbPbTrackTree;
   return Initialize();
}

bool PbPbTrackTreeMessenger::Initialize()
{
   if(Tree == nullptr)
      return false;

   VX = nullptr;
   VY = nullptr;
   VZ = nullptr;
   VXError = nullptr;
   VYError = nullptr;
   VZError = nullptr;
   VChi2 = nullptr;
   VNDof = nullptr;
   VIsFake = nullptr;
   VNTracks = nullptr;
   VPTSum = nullptr;
   TrackPT = nullptr;
   TrackPTError = nullptr;
   TrackEta = nullptr;
   TrackPhi = nullptr;
   TrackCharge = nullptr;
   TrackPDFID = nullptr;
   TrackNHits = nullptr;
   TrackNPixHits = nullptr;
   TrackNLayers = nullptr;
   TrackNormChi2 = nullptr;
   TrackHighPurity = nullptr;
   PFEnergy = nullptr;
   PFEcal = nullptr;
   PFHcal = nullptr;
   TrackAssociatedVertexIndex = nullptr;
   TrackAssociatedVertexQuality = nullptr;
   TrackAssociatedVertexDz = nullptr;
   TrackAssociatedVertexDzError = nullptr;
   TrackAssociatedVertexDxy = nullptr;
   TrackAssociatedVertexDxyError = nullptr;
   TrackFirstVertexQuality = nullptr;
   TrackFirstVertexDz = nullptr;
   TrackFirstVertexDzError = nullptr;
   TrackFirstVertexDxy = nullptr;
   TrackFirstVertexDxyError = nullptr;   

   Tree->SetBranchAddress("nRun", &Run);
   Tree->SetBranchAddress("nEv", &Event);
   Tree->SetBranchAddress("nLumi", &Lumi);
   Tree->SetBranchAddress("xVtx", &VX);
   Tree->SetBranchAddress("yVtx", &VY);
   Tree->SetBranchAddress("zVtx", &VZ);
   Tree->SetBranchAddress("xErrVtx", &VXError);
   Tree->SetBranchAddress("yErrVtx", &VYError);
   Tree->SetBranchAddress("zErrVtx", &VZError);
   Tree->SetBranchAddress("chi2Vtx", &VChi2);
   Tree->SetBranchAddress("ndofVtx", &VNDof);
   Tree->SetBranchAddress("isFakeVtx", &VIsFake);
   Tree->SetBranchAddress("nTracksVtx", &VNTracks);
   Tree->SetBranchAddress("ptSumVtx", &VPTSum);
   Tree->SetBranchAddress("trkPt", &TrackPT);
   Tree->SetBranchAddress("trkPtError", &TrackPTError);
   Tree->SetBranchAddress("trkEta", &TrackEta);
   Tree->SetBranchAddress("trkPhi", &TrackPhi);
   Tree->SetBranchAddress("trkCharge", &TrackCharge);
   Tree->SetBranchAddress("trkPDFId", &TrackPDFID);
   Tree->SetBranchAddress("trkNHits", &TrackNHits);
   Tree->SetBranchAddress("trkNPixHits", &TrackNPixHits);
   Tree->SetBranchAddress("trkNLayers", &TrackNLayers);
   Tree->SetBranchAddress("trkNormChi2", &TrackNormChi2);
   Tree->SetBranchAddress("highPurity", &TrackHighPurity);
   Tree->SetBranchAddress("pfEnergy", &PFEnergy);
   Tree->SetBranchAddress("pfEcal", &PFEcal);
   Tree->SetBranchAddress("pfHcal", &PFHcal);
   Tree->SetBranchAddress("trkAssociatedVtxIndx", &TrackAssociatedVertexIndex);
   Tree->SetBranchAddress("trkAssociatedVtxQuality", &TrackAssociatedVertexQuality);
   Tree->SetBranchAddress("trkDzAssociatedVtx", &TrackAssociatedVertexDz);
   Tree->SetBranchAddress("trkDzErrAssociatedVtx", &TrackAssociatedVertexDzError);
   Tree->SetBranchAddress("trkDxyAssociatedVtx", &TrackAssociatedVertexDxy);
   Tree->SetBranchAddress("trkDxyErrAssociatedVtx", &TrackAssociatedVertexDxyError);
   Tree->SetBranchAddress("trkFirstVtxQuality", &TrackFirstVertexQuality);
   Tree->SetBranchAddress("trkDzFirstVtx", &TrackFirstVertexDz);
   Tree->SetBranchAddress("trkDzErrFirstVtx", &TrackFirstVertexDzError);
   Tree->SetBranchAddress("trkDxyFirstVtx", &TrackFirstVertexDxy);
   Tree->SetBranchAddress("trkDxyErrFirstVtx", &TrackFirstVertexDxyError);

   return true;
}

bool PbPbTrackTreeMessenger::GetEntry(int iEntry)
{
   if(Tree == nullptr)
      return false;

   Tree->GetEntry(iEntry);
   return true;
}

bool PbPbTrackTreeMessenger::PassZHadron2022Cut(int index)
{
   if(TrackPT == nullptr)
      return false;
   if(index >= TrackPT->size())
      return false;

   if(TrackHighPurity->at(index) == false)
      return false;

   double RelativeUncertainty = TrackPTError->at(index) / TrackPT->at(index);
   if(RelativeUncertainty >= 0.1)
      return false;

   // double XYVertexSignificance = fabs(TrackAssociatedVertexDxy->at(index) / TrackAssociatedVertexDxyError->at(index));
   double XYVertexSignificance = fabs(TrackFirstVertexDxy->at(index) / TrackFirstVertexDxyError->at(index));
   if(XYVertexSignificance >= 3)
      return false;

   // double ZVertexSignificance = fabs(TrackAssociatedVertexDz->at(index) / TrackAssociatedVertexDzError->at(index));
   double ZVertexSignificance = fabs(TrackFirstVertexDz->at(index) / TrackFirstVertexDzError->at(index));
   if(ZVertexSignificance >= 3)
      return false;

   if(TrackNHits->at(index) < 11)
      return false;

   if(TrackNormChi2->at(index) / TrackNLayers->at(index) >= 0.18)
      return false;

   double ECAL = -1, HCAL = -1;
   if(PFEcal != nullptr && PFEcal->size() > index)   ECAL = PFEcal->at(index);
   if(PFHcal != nullptr && PFHcal->size() > index)   HCAL = PFHcal->at(index);
   // if(TrackPT->at(index) > 20 && ((ECAL + HCAL) / cosh(TrackEta->at(index)) < 0.5 * TrackPT->at(index)))
   //    return false;

   if(fabs(TrackEta->at(index)) > 2.4)
      return false;

   return true;
}

bool PbPbTrackTreeMessenger::PassZHadron2022CutNoVZ(int index)
{
   if(TrackPT == nullptr)
      return false;
   if(index >= TrackPT->size())
      return false;

   if(TrackHighPurity->at(index) == false)
      return false;

   double RelativeUncertainty = TrackPTError->at(index) / TrackPT->at(index);
   if(RelativeUncertainty >= 0.1)
      return false;

   // double XYVertexSignificance = fabs(TrackAssociatedVertexDxy->at(index) / TrackAssociatedVertexDxyError->at(index));
   double XYVertexSignificance = fabs(TrackFirstVertexDxy->at(index) / TrackFirstVertexDxyError->at(index));
   if(XYVertexSignificance >= 3)
      return false;

   // double ZVertexSignificance = fabs(TrackAssociatedVertexDz->at(index) / TrackAssociatedVertexDzError->at(index));
   // double ZVertexSignificance = fabs(TrackFirstVertexDz->at(index) / TrackFirstVertexDzError->at(index));
   // if(ZVertexSignificance >= 3)
   //    return false;

   if(TrackNHits->at(index) < 11)
      return false;

   if(TrackNormChi2->at(index) / TrackNLayers->at(index) >= 0.18)
      return false;

   double ECAL = -1, HCAL = -1;
   if(PFEcal != nullptr && PFEcal->size() > index)   ECAL = PFEcal->at(index);
   if(PFHcal != nullptr && PFHcal->size() > index)   HCAL = PFHcal->at(index);
   // if(TrackPT->at(index) > 20 && ((ECAL + HCAL) / cosh(TrackEta->at(index)) < 0.5 * TrackPT->at(index)))
   //    return false;

   if(fabs(TrackEta->at(index)) > 2.4)
      return false;

   return true;
}

bool PbPbTrackTreeMessenger::PassZHadron2022CutLoose(int index)
{
   if(TrackPT == nullptr)
      return false;
   if(index >= TrackPT->size())
      return false;

   if(TrackHighPurity->at(index) == false)
      return false;

   double RelativeUncertainty = TrackPTError->at(index) / TrackPT->at(index);
   if(RelativeUncertainty >= 0.15)
      return false;

   // double XYVertexSignificance = fabs(TrackAssociatedVertexDxy->at(index) / TrackAssociatedVertexDxyError->at(index));
   double XYVertexSignificance = fabs(TrackFirstVertexDxy->at(index) / TrackFirstVertexDxyError->at(index));
   if(XYVertexSignificance >= 5)
      return false;

   // double ZVertexSignificance = fabs(TrackAssociatedVertexDz->at(index) / TrackAssociatedVertexDzError->at(index));
   double ZVertexSignificance = fabs(TrackFirstVertexDz->at(index) / TrackFirstVertexDzError->at(index));
   if(ZVertexSignificance >= 5)
      return false;

   if(TrackNHits->at(index) < 11)
      return false;

   if(TrackNormChi2->at(index) / TrackNLayers->at(index) >= 0.18)
      return false;

   double ECAL = -1, HCAL = -1;
   if(PFEcal != nullptr && PFEcal->size() > index)   ECAL = PFEcal->at(index);
   if(PFHcal != nullptr && PFHcal->size() > index)   HCAL = PFHcal->at(index);
   // if(TrackPT->at(index) > 20 && ((ECAL + HCAL) / cosh(TrackEta->at(index)) < 0.5 * TrackPT->at(index)))
   //    return false;

   if(fabs(TrackEta->at(index)) > 2.4)
      return false;

   return true;
}

bool PbPbTrackTreeMessenger::PassZHadron2022CutTight(int index)
{
   if(TrackPT == nullptr)
      return false;
   if(index >= TrackPT->size())
      return false;

   if(TrackHighPurity->at(index) == false)
      return false;

   double RelativeUncertainty = TrackPTError->at(index) / TrackPT->at(index);
   if(RelativeUncertainty >= 0.05)
      return false;

   // double XYVertexSignificance = fabs(TrackAssociatedVertexDxy->at(index) / TrackAssociatedVertexDxyError->at(index));
   double XYVertexSignificance = fabs(TrackFirstVertexDxy->at(index) / TrackFirstVertexDxyError->at(index));
   if(XYVertexSignificance >= 2)
      return false;

   // double ZVertexSignificance = fabs(TrackAssociatedVertexDz->at(index) / TrackAssociatedVertexDzError->at(index));
   double ZVertexSignificance = fabs(TrackFirstVertexDz->at(index) / TrackFirstVertexDzError->at(index));
   if(ZVertexSignificance >= 2)
      return false;

   if(TrackNHits->at(index) < 11)
      return false;

   if(TrackNormChi2->at(index) / TrackNLayers->at(index) >= 0.15)
      return false;

   double ECAL = -1, HCAL = -1;
   if(PFEcal != nullptr && PFEcal->size() > index)   ECAL = PFEcal->at(index);
   if(PFHcal != nullptr && PFHcal->size() > index)   HCAL = PFHcal->at(index);
   // if(TrackPT->at(index) > 20 && ((ECAL + HCAL) / cosh(TrackEta->at(index)) < 0.5 * TrackPT->at(index)))
   //    return false;

   if(fabs(TrackEta->at(index)) > 2.4)
      return false;

   return true;
}

PbPbUPCTrackTreeMessenger::PbPbUPCTrackTreeMessenger(TFile &File, std::string TreeName)
{
   Tree = (TTree *)File.Get(TreeName.c_str());
   Initialize();
}

PbPbUPCTrackTreeMessenger::PbPbUPCTrackTreeMessenger(TFile *File, std::string TreeName)
{
   if(File != nullptr)
      Tree = (TTree *)File->Get(TreeName.c_str());
   else
      Tree = nullptr;
   Initialize();
}

PbPbUPCTrackTreeMessenger::PbPbUPCTrackTreeMessenger(TTree *PbPbUPCTrackTree)
{
   Initialize(PbPbUPCTrackTree);
}

bool PbPbUPCTrackTreeMessenger::Initialize(TTree *PbPbUPCTrackTree)
{
   Tree = PbPbUPCTrackTree;
   return Initialize();
}

bool PbPbUPCTrackTreeMessenger::Initialize()
{
   if(Tree == nullptr)
      return false;

   nVtx = 0;
   nTrk = 0;

   ptSumVtx = nullptr;
   xVtx = nullptr;
   yVtx = nullptr;
   zVtx = nullptr;
   xErrVtx = nullptr;
   yErrVtx = nullptr;
   zErrVtx = nullptr;
   trkPt = nullptr;
   trkEta = nullptr;
   highPurity = nullptr;

   Tree->SetBranchAddress("nVtx", &nVtx);
   Tree->SetBranchAddress("nTrk", &nTrk);
   Tree->SetBranchAddress("ptSumVtx", &ptSumVtx);
   Tree->SetBranchAddress("xVtx", &xVtx);
   Tree->SetBranchAddress("yVtx", &yVtx);
   Tree->SetBranchAddress("zVtx", &zVtx);
   Tree->SetBranchAddress("xErrVtx", &xErrVtx);
   Tree->SetBranchAddress("yErrVtx", &yErrVtx);
   Tree->SetBranchAddress("zErrVtx", &zErrVtx);
   Tree->SetBranchAddress("trkPt", &trkPt);
   Tree->SetBranchAddress("trkEta", &trkEta);
   Tree->SetBranchAddress("highPurity", &highPurity);
   return true;
}

bool PbPbUPCTrackTreeMessenger::GetEntry(int iEntry)
{
   if(Tree == nullptr)
      return false;

   Tree->GetEntry(iEntry);
   return true;
}

ZDCTreeMessenger::ZDCTreeMessenger(TFile &File, std::string TreeName)
{
   Tree = (TTree *)File.Get(TreeName.c_str());
   Initialize();
}

ZDCTreeMessenger::ZDCTreeMessenger(TFile *File, std::string TreeName)
{
   if(File != nullptr)
      Tree = (TTree *)File->Get(TreeName.c_str());
   else
      Tree = nullptr;
   Initialize();
}

ZDCTreeMessenger::ZDCTreeMessenger(TTree *ZDCTree)
{
   Initialize(ZDCTree);
}

bool ZDCTreeMessenger::Initialize(TTree *ZDCTree)
{
   Tree = ZDCTree;
   return Initialize();
}

bool ZDCTreeMessenger::Initialize()
{
   if(Tree == nullptr)
      return false;
   sumPlus = 0.0;
   sumMinus = 0.0;

   Tree->SetBranchAddress("sumPlus", &sumPlus);
   Tree->SetBranchAddress("sumMinus", &sumMinus);

   return true;
}

bool ZDCTreeMessenger::GetEntry(int iEntry)
{
   if(Tree == nullptr)
      return false;

   Tree->GetEntry(iEntry);
   return true;
}

ZHadronMessenger::ZHadronMessenger(TFile &File, std::string TreeName, bool SkipTrack)
{
   Initialized = false;
   WriteMode = false;
   
   Tree = (TTree *)File.Get(TreeName.c_str());
   Initialize(SkipTrack);
}

ZHadronMessenger::ZHadronMessenger(TFile *File, std::string TreeName, bool SkipTrack)
{
   Initialized = false;
   WriteMode = false;
   
   if(File != nullptr)
      Tree = (TTree *)File->Get(TreeName.c_str());
   else
      Tree = nullptr;
   Initialize(SkipTrack);
}

ZHadronMessenger::ZHadronMessenger(TTree *ZHadronTree, bool SkipTrack)
{
   Initialized = false;
   WriteMode = false;

   Initialize(ZHadronTree, SkipTrack);
}
   
ZHadronMessenger::~ZHadronMessenger()
{
   if(Initialized == true && WriteMode == true)
   {
      delete zMass;
      delete zEta;
      delete zY;
      delete zPhi;
      delete zPt;
      delete genZMass;
      delete genZEta;
      delete genZY;
      delete genZPhi;
      delete genZPt;
      delete trackPt;
      delete trackMuTagged;
      delete trackMuDR;
      delete trackPDFId;
      delete trackEta;
      delete trackY;
      delete trackPhi;
      delete trackCharge;

      delete muEta1;
      delete muEta2;
      delete muPhi1;
      delete muPhi2;
      delete muPt1;
      delete muPt2;

      delete muDeta;
      delete muDphi;
      delete muDR;
      delete muDphiS;

      delete genMuPt1;
      delete genMuPt2;
      delete genMuEta1;
      delete genMuEta2;
      delete genMuPhi1;
      delete genMuPhi2;

      delete genMuDeta;
      delete genMuDphi;
      delete genMuDR;
      delete genMuDphiS;
   }
}
   
bool ZHadronMessenger::Initialize(TTree *ZHadronTree, bool SkipTrack)
{
   Tree = ZHadronTree;
   return Initialize(SkipTrack);
}

bool ZHadronMessenger::Initialize(bool SkipTrack)
{
   if(Tree == nullptr)
      return false;

   Initialized = true;

   zMass = nullptr;
   zEta = nullptr;
   zY = nullptr;
   zPhi = nullptr;
   zPt = nullptr;
   genZMass = nullptr;
   genZEta = nullptr;
   genZY = nullptr;
   genZPhi = nullptr;
   genZPt = nullptr;
   trackPt = nullptr;
   trackPDFId = nullptr;
   trackEta = nullptr;
   trackY = nullptr;
   trackPhi = nullptr;
   trackMuTagged = nullptr;
   trackMuDR = nullptr;
   trackWeight = nullptr;
   trackResidualWeight = nullptr;
   trackCharge = nullptr;
   subevent = nullptr;

   muEta1 = nullptr;
   muEta2 = nullptr;
   muPhi1 = nullptr;
   muPhi2 = nullptr;
   muPt1 = nullptr;
   muPt2 = nullptr;

   muDeta = nullptr;
   muDphi = nullptr;
   muDR = nullptr;
   muDphiS = nullptr;

   genMuPt1 = nullptr;
   genMuPt2 = nullptr;
   genMuEta1 = nullptr;
   genMuEta2 = nullptr;
   genMuPhi1 = nullptr;
   genMuPhi2 = nullptr;

   genMuDeta = nullptr;
   genMuDphi = nullptr;
   genMuDR = nullptr;
   genMuDphiS = nullptr;

   Tree->SetBranchAddress("Run", &Run);
   Tree->SetBranchAddress("Event", &Event);
   Tree->SetBranchAddress("Lumi", &Lumi);

   Tree->SetBranchAddress("hiBin", &hiBin);
   Tree->SetBranchAddress("hiBinUp", &hiBinUp);
   Tree->SetBranchAddress("hiBinDown", &hiBinDown);
   Tree->SetBranchAddress("hiHF", &hiHF);
   
   Tree->SetBranchAddress("SignalHF", &SignalHF);
   Tree->SetBranchAddress("BackgroundHF", &BackgroundHF);
   Tree->SetBranchAddress("SubEvent0HF", &SubEvent0HF);
   Tree->SetBranchAddress("SubEventAllHF", &SubEventAllHF);
   Tree->SetBranchAddress("SignalVZ", &SignalVZ);

   Tree->SetBranchAddress("EventWeight", &EventWeight);
   Tree->SetBranchAddress("NCollWeight", &NCollWeight);
   Tree->SetBranchAddress("InterSampleZWeight", &InterSampleZWeight);
   Tree->SetBranchAddress("ZWeight", &ZWeight);
   Tree->SetBranchAddress("VZWeight", &VZWeight);
   Tree->SetBranchAddress("ExtraZWeight", &ExtraZWeight);

   Tree->SetBranchAddress("NVertex", &NVertex);
   Tree->SetBranchAddress("VX", &VX);
   Tree->SetBranchAddress("VY", &VY);
   Tree->SetBranchAddress("VZ", &VZ);
   Tree->SetBranchAddress("VXError", &VXError);
   Tree->SetBranchAddress("VYError", &VYError);
   Tree->SetBranchAddress("VZError", &VZError);
   Tree->SetBranchAddress("NPU", &NPU);
   
   Tree->SetBranchAddress("zMass", &zMass);
   Tree->SetBranchAddress("zEta", &zEta);
   Tree->SetBranchAddress("zY", &zY);
   Tree->SetBranchAddress("zPhi", &zPhi);
   Tree->SetBranchAddress("zPt", &zPt);
   Tree->SetBranchAddress("genZMass", &genZMass);
   Tree->SetBranchAddress("genZEta", &genZEta);
   Tree->SetBranchAddress("genZY", &genZY);
   Tree->SetBranchAddress("genZPhi", &genZPhi);
   Tree->SetBranchAddress("genZPt", &genZPt);
   if(SkipTrack == false)
   {
      Tree->SetBranchAddress("trackPt", &trackPt);
      Tree->SetBranchAddress("trackPDFId", &trackPDFId);
      Tree->SetBranchAddress("trackEta", &trackEta);
      Tree->SetBranchAddress("trackY", &trackY);
      Tree->SetBranchAddress("trackPhi", &trackPhi);
      Tree->SetBranchAddress("trackMuTagged", &trackMuTagged);
      Tree->SetBranchAddress("trackMuDR", &trackMuDR);
      Tree->SetBranchAddress("trackWeight", &trackWeight);
      Tree->SetBranchAddress("trackResidualWeight", &trackResidualWeight);
      Tree->SetBranchAddress("trackCharge", &trackCharge);
      Tree->SetBranchAddress("subevent", &subevent);
   }
   else
   {
      Tree->SetBranchStatus("trackPt", false);
      Tree->SetBranchStatus("trackPDFId", false);
      Tree->SetBranchStatus("trackEta", false);
      Tree->SetBranchStatus("trackY", false);
      Tree->SetBranchStatus("trackPhi", false);
      Tree->SetBranchStatus("trackMuTagged", false);
      Tree->SetBranchStatus("trackMuDR", false);
      Tree->SetBranchStatus("trackWeight", false);
      Tree->SetBranchStatus("trackResidualWeight", false);
      Tree->SetBranchStatus("trackCharge", false);
      Tree->SetBranchStatus("subevent", false);
   }

   Tree->SetBranchAddress("muEta1", &muEta1);
   Tree->SetBranchAddress("muEta2", &muEta2);
   Tree->SetBranchAddress("muPhi1", &muPhi1);
   Tree->SetBranchAddress("muPhi2", &muPhi2);
   Tree->SetBranchAddress("muPt1", &muPt1);
   Tree->SetBranchAddress("muPt2", &muPt2);
 
   Tree->SetBranchAddress("muDeta", &muDeta);
   Tree->SetBranchAddress("muDphi", &muDphi);
   Tree->SetBranchAddress("muDR", &muDR);
   Tree->SetBranchAddress("muDphiS", &muDphiS);
  
   Tree->SetBranchAddress("genMuPt1", &genMuPt1);
   Tree->SetBranchAddress("genMuPt2", &genMuPt2);
   Tree->SetBranchAddress("genMuEta1", &genMuEta1);
   Tree->SetBranchAddress("genMuEta2", &genMuEta2);
   Tree->SetBranchAddress("genMuPhi1", &genMuPhi1);
   Tree->SetBranchAddress("genMuPhi2", &genMuPhi2);
   
   Tree->SetBranchAddress("genMuDeta", &genMuDeta);
   Tree->SetBranchAddress("genMuDphi", &genMuDphi);
   Tree->SetBranchAddress("genMuDR", &genMuDR);
   Tree->SetBranchAddress("genMuDphiS", &genMuDphiS);
   
   return true;
}

int ZHadronMessenger::GetEntries()
{
   if(Tree == nullptr)
      return 0;
   return Tree->GetEntries();
}

bool ZHadronMessenger::GetEntry(int iEntry)
{
   if(Tree == nullptr)
      return false;

   Tree->GetEntry(iEntry);

   // fill derived quantities
   GoodGenZ = true;
   if(genZY == nullptr)
      GoodGenZ = false;
   else if(genZY->size() == 0)
      GoodGenZ = false;
   else
   {
      if(genZMass->at(0) > 120 || genZMass->at(0) < 60)   GoodGenZ = false;
      if(genZY->at(0) < -2.4 || genZY->at(0) > 2.4)       GoodGenZ = false;
   }
   
   GoodRecoZ = true;
   if(zY == nullptr)
      GoodRecoZ = false;
   else if(zY->size() == 0)
      GoodRecoZ = false;
   else
   {
      if(zMass->at(0) > 120 || zMass->at(0) < 60)   GoodRecoZ = false;
      if(zY->at(0) < -2.4 || zY->at(0) > 2.4)       GoodRecoZ = false;
   }

   return true;
}

bool ZHadronMessenger::SetBranch(TTree *T)
{
   if(T == nullptr)
      return false;

   Initialized = true;
   WriteMode = true;

   zMass = new std::vector<float>();
   zEta = new std::vector<float>();
   zY = new std::vector<float>();
   zPhi = new std::vector<float>();
   zPt = new std::vector<float>();
   genZMass = new std::vector<float>();
   genZEta = new std::vector<float>();
   genZY = new std::vector<float>();
   genZPhi = new std::vector<float>();
   genZPt = new std::vector<float>();
   trackPt = new std::vector<float>();
   trackPDFId = new std::vector<float>();
   trackEta = new std::vector<float>();
   trackY = new std::vector<float>();
   trackPhi = new std::vector<float>();
   trackMuTagged = new std::vector<bool>();
   trackMuDR = new std::vector<float>();
   trackWeight = new std::vector<float>();
   trackResidualWeight = new std::vector<float>();
   trackCharge = new std::vector<int>();
   subevent = new std::vector<int>();

   muEta1 = new std::vector<float>();
   muEta2 = new std::vector<float>();
   muPhi1 = new std::vector<float>();
   muPhi2 = new std::vector<float>();
   muPt1 = new std::vector<float>();
   muPt2 = new std::vector<float>();

   muDeta = new std::vector<float>();
   muDphi = new std::vector<float>();
   muDR = new std::vector<float>();
   muDphiS = new std::vector<float>();

   genMuPt1 = new std::vector<float>();
   genMuPt2 = new std::vector<float>();
   genMuEta1 = new std::vector<float>();
   genMuEta2 = new std::vector<float>();
   genMuPhi1 = new std::vector<float>();
   genMuPhi2 = new std::vector<float>();

   genMuDeta = new std::vector<float>();
   genMuDphi = new std::vector<float>();
   genMuDR = new std::vector<float>();
   genMuDphiS = new std::vector<float>();

   Tree = T;

   Tree->Branch("run",                    &Run,          "run/I");
   Tree->Branch("event",                  &Event,        "event/L");
   Tree->Branch("lumi",                   &Lumi,         "lumi/I");
   Tree->Branch("hiBin",                  &hiBin,        "hiBin/I");
   Tree->Branch("hiBinUp",                &hiBinUp,      "hiBinUp/I");
   Tree->Branch("hiBinDown",              &hiBinDown,    "hiBinDown/I");
   Tree->Branch("hiHF",                   &hiHF,         "hiHF/F");

   Tree->Branch("SignalHF",               &SignalHF,     "SignalHF/F");
   Tree->Branch("BackgroundHF",           &BackgroundHF, "BackgroundHF/F");
   Tree->Branch("SubEvent0HF",            &SubEvent0HF,  "SubEvent0HF/F");
   Tree->Branch("SubEventAllHF",            &SubEventAllHF,  "SubEventAllHF/F");
   Tree->Branch("SignalVZ",               &SignalVZ,     "SignalVZ/F");

   Tree->Branch("EventWeight",            &EventWeight,  "EventWeight/F");
   Tree->Branch("NCollWeight",            &NCollWeight,  "NCollWeight/F");
   Tree->Branch("InterSampleZWeight",     &InterSampleZWeight,"InterSampleZWeight/F");
   Tree->Branch("ZWeight",                &ZWeight,      "ZWeight/F");
   Tree->Branch("VZWeight",               &VZWeight,     "VZWeight/F");
   Tree->Branch("ExtraZWeight",           &ExtraZWeight, "ExtraZWeight[12]/F");
   
   Tree->Branch("NVertex",                &NVertex,      "NVertex/I");
   Tree->Branch("VX",                     &VX,           "VX/F");
   Tree->Branch("VY",                     &VY,           "VY/F");
   Tree->Branch("VZ",                     &VZ,           "VZ/F");
   Tree->Branch("VXError",                &VXError,      "VXError/F");
   Tree->Branch("VYError",                &VYError,      "VYError/F");
   Tree->Branch("VZError",                &VZError,      "VZError/F");
   Tree->Branch("NPU",                    &NPU,          "NPU/I");

   Tree->Branch("zMass",                  &zMass);
   Tree->Branch("zEta",                   &zEta);
   Tree->Branch("zY",                     &zY);
   Tree->Branch("zPhi",                   &zPhi);
   Tree->Branch("zPt",                    &zPt);
   Tree->Branch("genZMass",               &genZMass);
   Tree->Branch("genZEta",                &genZEta);
   Tree->Branch("genZY",                  &genZY);
   Tree->Branch("genZPhi",                &genZPhi);
   Tree->Branch("genZPt",                 &genZPt);
   Tree->Branch("trackPt",                &trackPt);
   Tree->Branch("trackPDFId",             &trackPDFId);
   Tree->Branch("trackPhi",               &trackPhi);
   Tree->Branch("trackEta",               &trackEta);
   Tree->Branch("trackY",                 &trackY);
   Tree->Branch("trackMuTagged",          &trackMuTagged);
   // Tree->Branch("trackMuDR",              &trackMuDR);
   Tree->Branch("trackWeight",            &trackWeight);
   Tree->Branch("trackResidualWeight",    &trackResidualWeight);
   Tree->Branch("trackCharge",            &trackCharge);
   Tree->Branch("subevent",               &subevent);
   
   Tree->Branch("muEta1",                 &muEta1);
   Tree->Branch("muEta2",                 &muEta2);
   Tree->Branch("muPhi1",                 &muPhi1);
   Tree->Branch("muPhi2",                 &muPhi2);
   Tree->Branch("muPt1",                  &muPt1);
   Tree->Branch("muPt2",                  &muPt2);

   Tree->Branch("genMuPt1",               &genMuPt1);
   Tree->Branch("genMuPt2",               &genMuPt2);
   Tree->Branch("genMuEta1",              &genMuEta1);
   Tree->Branch("genMuEta2",              &genMuEta2);
   Tree->Branch("genMuPhi1",              &genMuPhi1);
   Tree->Branch("genMuPhi2",              &genMuPhi2);

   Tree->Branch("muDeta",                 &muDeta);
   Tree->Branch("muDphi",                 &muDphi);
   Tree->Branch("muDR",                   &muDR);
   Tree->Branch("muDphiS",                &muDphiS);

   Tree->Branch("genMuDeta",              &genMuDeta);
   Tree->Branch("genMuDphi",              &genMuDphi);
   Tree->Branch("genMuDR",                &genMuDR);
   Tree->Branch("genMuDphiS",             &genMuDphiS);

   return true;
}
   
void ZHadronMessenger::Clear()
{
   if(Initialized == false)
      return;

   SignalHF = -1;
   BackgroundHF = -1;
   SubEvent0HF = -1;
   SubEventAllHF = -1;
   SignalVZ = -999;

   EventWeight = 1;
   NCollWeight = 1;
   InterSampleZWeight = 1;
   ZWeight = 1;
   VZWeight = 1;
   for(int i = 0; i < 12; i++)
      ExtraZWeight[i] = 1;

   NVertex = 0;
   VX = 0;
   VY = 0;
   VZ = 0;
   VXError = 0;
   VYError = 0;
   VZError = 0;
   NPU = 0;

   zMass->clear();
   zEta->clear();
   zY->clear();
   zPhi->clear();
   zPt->clear();
   genZMass->clear();
   genZEta->clear();
   genZY->clear();
   genZPhi->clear();
   genZPt->clear();
   trackPt->clear();
   trackPDFId->clear();
   trackPhi->clear();
   trackEta->clear();
   trackY->clear();
   trackMuTagged->clear();
   trackMuDR->clear();
   trackWeight->clear();
   trackResidualWeight->clear();
   trackCharge->clear();
   subevent->clear();

   muEta1->clear();
   muEta2->clear();
   muPhi1->clear();
   muPhi2->clear();
   muPt1->clear();
   muPt2->clear();

   genMuPt1->clear();
   genMuPt2->clear();
   genMuEta1->clear();
   genMuEta2->clear();
   genMuPhi1->clear();
   genMuPhi2->clear();

   muDeta->clear();
   muDphi->clear();
   muDR->clear();
   muDphiS->clear();
   genMuDeta->clear();
   genMuDphi->clear();
   genMuDR->clear();
   genMuDphiS->clear();
}

void ZHadronMessenger::CopyNonTrack(ZHadronMessenger &M)
{
   Run          = M.Run;
   Event        = M.Event;
   Lumi         = M.Lumi;
   
   hiBin        = M.hiBin;
   hiBinUp      = M.hiBinUp;
   hiBinDown    = M.hiBinDown;
   hiHF         = M.hiHF;

   SignalHF     = M.SignalHF;
   BackgroundHF = M.BackgroundHF;
   SubEvent0HF  = M.SubEvent0HF;
   SubEventAllHF  = M.SubEventAllHF;
   SignalVZ     = M.SignalVZ;

   EventWeight  = M.EventWeight;
   NCollWeight  = M.NCollWeight;
   InterSampleZWeight = M.InterSampleZWeight;
   ZWeight      = M.ZWeight;
   VZWeight     = M.VZWeight;
   for(int i = 0; i < 12; i++)
      ExtraZWeight[i] = M.ExtraZWeight[i];

   NVertex      = M.NVertex;
   VX           = M.VX;
   VY           = M.VY;
   VZ           = M.VZ;
   VXError      = M.VXError;
   VYError      = M.VYError;
   VZError      = M.VZError;
   NPU          = M.NPU;
  
   if(zMass != nullptr && M.zMass != nullptr)   *zMass = *(M.zMass);
   if(zEta != nullptr && M.zEta != nullptr)   *zEta = *(M.zEta);
   if(zY != nullptr && M.zY != nullptr)   *zY = *(M.zY);
   if(zPhi != nullptr && M.zPhi != nullptr)   *zPhi = *(M.zPhi);
   if(zPt != nullptr && M.zPt != nullptr)   *zPt = *(M.zPt);
   if(genZMass != nullptr && M.genZMass != nullptr)   *genZMass = *(M.genZMass);
   if(genZEta != nullptr && M.genZEta != nullptr)   *genZEta = *(M.genZEta);
   if(genZY != nullptr && M.genZY != nullptr)   *genZY = *(M.genZY);
   if(genZPhi != nullptr && M.genZPhi != nullptr)   *genZPhi = *(M.genZPhi);
   if(genZPt != nullptr && M.genZPt != nullptr)   *genZPt = *(M.genZPt);
   
   if(muEta1 != nullptr && M.muEta1 != nullptr)   *muEta1 = *(M.muEta1);
   if(muEta2 != nullptr && M.muEta2 != nullptr)   *muEta2 = *(M.muEta2);
   if(muPhi1 != nullptr && M.muPhi1 != nullptr)   *muPhi1 = *(M.muPhi1);
   if(muPhi2 != nullptr && M.muPhi2 != nullptr)   *muPhi2 = *(M.muPhi2);
   if(muPt1 != nullptr && M.muPt1 != nullptr)   *muPt1 = *(M.muPt1);
   if(muPt2 != nullptr && M.muPt2 != nullptr)   *muPt2 = *(M.muPt2);

   if(muDeta != nullptr && M.muDeta != nullptr)   *muDeta = *(M.muDeta);
   if(muDphi != nullptr && M.muDphi != nullptr)   *muDphi = *(M.muDphi);
   if(muDR != nullptr && M.muDR != nullptr)   *muDR = *(M.muDR);
   if(muDphiS != nullptr && M.muDphiS != nullptr)   *muDphiS = *(M.muDphiS);

   if(genMuPt1 != nullptr && M.genMuPt1 != nullptr)   *genMuPt1 = *(M.genMuPt1);
   if(genMuPt2 != nullptr && M.genMuPt2 != nullptr)   *genMuPt2 = *(M.genMuPt2);
   if(genMuEta1 != nullptr && M.genMuEta1 != nullptr)   *genMuEta1 = *(M.genMuEta1);
   if(genMuEta2 != nullptr && M.genMuEta2 != nullptr)   *genMuEta2 = *(M.genMuEta2);
   if(genMuPhi1 != nullptr && M.genMuPhi1 != nullptr)   *genMuPhi1 = *(M.genMuPhi1);
   if(genMuPhi2 != nullptr && M.genMuPhi2 != nullptr)   *genMuPhi2 = *(M.genMuPhi2);

   if(genMuDeta != nullptr && M.genMuDeta != nullptr)   *genMuDeta = *(M.genMuDeta);
   if(genMuDphi != nullptr && M.genMuDphi != nullptr)   *genMuDphi = *(M.genMuDphi);
   if(genMuDR != nullptr && M.genMuDR != nullptr)   *genMuDR = *(M.genMuDR);
   if(genMuDphiS != nullptr && M.genMuDphiS != nullptr)   *genMuDphiS = *(M.genMuDphiS);
}

bool ZHadronMessenger::FillEntry()
{
   if(Initialized == false)
      return false;
   if(WriteMode == false)
      return false;

   if(Tree == nullptr)
      return false;

   Tree->Fill();
   Clear();

   return true;
}

DzeroUPCTreeMessenger::DzeroUPCTreeMessenger(TFile &File, std::string TreeName)
{
   Initialized = false;
   WriteMode = false;

   Tree = (TTree *)File.Get(TreeName.c_str());
   Initialize();
}

DzeroUPCTreeMessenger::DzeroUPCTreeMessenger(TFile *File, std::string TreeName)
{
   Initialized = false;
   WriteMode = false;

   if(File != nullptr)
      Tree = (TTree *)File->Get(TreeName.c_str());
   else
      Tree = nullptr;
   Initialize();
}

DzeroUPCTreeMessenger::DzeroUPCTreeMessenger(TTree *DzeroUPCTree)
{
   Initialized = false;
   WriteMode = false;

   Initialize(DzeroUPCTree);
}

DzeroUPCTreeMessenger::~DzeroUPCTreeMessenger()
{
   if(Initialized == true && WriteMode == true)
   {
      delete Dpt;
      delete Dy;
      delete Dmass;
      delete Dtrk1Pt;
      delete Dtrk2Pt;
      delete Dchi2cl;
      delete DsvpvDistance;
      delete DsvpvDisErr;
      delete DsvpvDistance_2D;
      delete DsvpvDisErr_2D;
      delete Dalpha;
      delete Ddtheta;
      delete DnTrackInAcceptanceHP;
      delete Gpt;
      delete Gy;
      delete GpdgId;
      delete GisSignal;
      delete GcollisionId;
      delete GSignalType;
   }
}

bool DzeroUPCTreeMessenger::Initialize(TTree *DzeroUPCTree)
{
   Tree = DzeroUPCTree;
   return Initialize();
}

bool DzeroUPCTreeMessenger::Initialize()
{
   if(Tree == nullptr)
      return false;

   Initialized = true;
   Dpt = nullptr;
   Dy = nullptr;
   Dmass = nullptr;
   Dtrk1Pt = nullptr;
   Dtrk2Pt = nullptr;
   Dchi2cl = nullptr;
   DsvpvDistance = nullptr;
   DsvpvDisErr = nullptr;
   DsvpvDistance_2D = nullptr;
   DsvpvDisErr_2D = nullptr;
   Dalpha = nullptr;
   Ddtheta = nullptr;
   DnTrackInAcceptanceHP = nullptr;
   Gpt = nullptr;
   Gy = nullptr;
   GpdgId = nullptr;
   GisSignal = nullptr;
   GcollisionId = nullptr;
   GSignalType = nullptr;

   Tree->SetBranchAddress("Run", &Run);
   Tree->SetBranchAddress("Event", &Event);
   Tree->SetBranchAddress("Lumi", &Lumi);
   Tree->SetBranchAddress("VX", &VX);
   Tree->SetBranchAddress("VY", &VY);
   Tree->SetBranchAddress("VZ", &VZ);
   Tree->SetBranchAddress("VXError", &VXError);
   Tree->SetBranchAddress("VYError", &VYError);
   Tree->SetBranchAddress("VZError", &VZError);
   Tree->SetBranchAddress("gammaN", &gammaN);
   Tree->SetBranchAddress("Ngamma", &Ngamma);
   Tree->SetBranchAddress("Dpt", &Dpt);
   Tree->SetBranchAddress("Dy", &Dy);
   Tree->SetBranchAddress("Dmass", &Dmass);
   Tree->SetBranchAddress("Dtrk1Pt", &Dtrk1Pt);
   Tree->SetBranchAddress("Dtrk2Pt", &Dtrk2Pt);
   Tree->SetBranchAddress("Dchi2cl", &Dchi2cl);
   Tree->SetBranchAddress("DsvpvDistance", &DsvpvDistance);
   Tree->SetBranchAddress("DsvpvDisErr", &DsvpvDisErr);
   Tree->SetBranchAddress("DsvpvDistance_2D", &DsvpvDistance_2D);
   Tree->SetBranchAddress("DsvpvDisErr_2D", &DsvpvDisErr_2D);
   Tree->SetBranchAddress("Dalpha", &Dalpha);
   Tree->SetBranchAddress("Ddtheta", &Ddtheta);
   Tree->SetBranchAddress("DnTrackInAcceptanceHP", &DnTrackInAcceptanceHP);
   Tree->SetBranchAddress("nTrackInAcceptanceHP", &nTrackInAcceptanceHP);
   Tree->SetBranchAddress("Gpt", &Gpt);
   Tree->SetBranchAddress("Gy", &Gy);
   Tree->SetBranchAddress("GpdgId", &GpdgId);
   Tree->SetBranchAddress("GisSignal", &GisSignal);
   Tree->SetBranchAddress("GcollisionId", &GcollisionId);
   Tree->SetBranchAddress("GSignalType", &GSignalType);
   return true;
}

int DzeroUPCTreeMessenger::GetEntries()
{
   if(Tree == nullptr)
      return 0;
   return Tree->GetEntries();
}

bool DzeroUPCTreeMessenger::GetEntry(int iEntry)
{
   if(Tree == nullptr)
      return false;

   Tree->GetEntry(iEntry);
   //GoodPhotonuclear = true;
   // fill derived quantities
   /*
   GoodGenZ = true;
   if(genZY == nullptr)
      GoodGenZ = false;
   else if(genZY->size() == 0)
      GoodGenZ = false;
   else
   {
      if(genZMass->at(0) > 120 || genZMass->at(0) < 60)   GoodGenZ = false;
      if(genZY->at(0) < -2.4 || genZY->at(0) > 2.4)       GoodGenZ = false;
   }
   GoodRecoZ = true;
   if(zY == nullptr)
      GoodRecoZ = false;
   else if(zY->size() == 0)
      GoodRecoZ = false;
   else
   {
      if(zMass->at(0) > 120 || zMass->at(0) < 60)   GoodRecoZ = false;
      if(zY->at(0) < -2.4 || zY->at(0) > 2.4)       GoodRecoZ = false;
   }
   */
   return true;
}

bool DzeroUPCTreeMessenger::SetBranch(TTree *T)
{
   if(T == nullptr)
      return false;

   Initialized = true;
   WriteMode = true;

   Dpt = new std::vector<float>();
   Dy = new std::vector<float>();
   Dmass = new std::vector<float>();
   Dtrk1Pt = new std::vector<float>();
   Dtrk2Pt = new std::vector<float>();
   Dchi2cl = new std::vector<float>();
   DsvpvDistance = new std::vector<float>();
   DsvpvDisErr = new std::vector<float>();
   DsvpvDistance_2D = new std::vector<float>();
   DsvpvDisErr_2D = new std::vector<float>();
   Dalpha = new std::vector<float>();
   Ddtheta = new std::vector<float>();
   DnTrackInAcceptanceHP = new std::vector<int>();
   Gpt = new std::vector<float>();
   Gy = new std::vector<float>();
   GpdgId = new std::vector<int>();
   GisSignal = new std::vector<int>();
   GcollisionId = new std::vector<int>();
   GSignalType = new std::vector<int>();

   Tree = T;

   Tree->Branch("Run",                   &Run, "Run/I");
   Tree->Branch("Event",                 &Event, "Event/L");
   Tree->Branch("Lumi",                  &Lumi, "Lumi/I");
   Tree->Branch("VX",                    &VX, "VX/F");
   Tree->Branch("VY",                    &VY, "VY/F");
   Tree->Branch("VZ",                    &VZ, "VZ/F");
   Tree->Branch("VXError",               &VXError, "VXError/F");
   Tree->Branch("VYError",               &VYError, "VYError/F");
   Tree->Branch("VZError",               &VZError, "VZError/F");
   Tree->Branch("gammaN",                &gammaN, "gammaN/I");
   Tree->Branch("Ngamma",                &Ngamma, "Ngamma/I");
   Tree->Branch("Dpt",                   &Dpt);
   Tree->Branch("Dy",                    &Dy);
   Tree->Branch("Dmass",                 &Dmass);
   Tree->Branch("Dtrk1Pt",               &Dtrk1Pt);
   Tree->Branch("Dtrk2Pt",               &Dtrk2Pt);
   Tree->Branch("Dchi2cl",               &Dchi2cl);
   Tree->Branch("DsvpvDistance",         &DsvpvDistance);
   Tree->Branch("DsvpvDisErr",           &DsvpvDisErr);
   Tree->Branch("DsvpvDistance_2D",      &DsvpvDistance_2D);
   Tree->Branch("DsvpvDisErr_2D",        &DsvpvDisErr_2D);
   Tree->Branch("Dalpha",                &Dalpha);
   Tree->Branch("Ddtheta",               &Ddtheta);
   Tree->Branch("DnTrackInAcceptanceHP", &DnTrackInAcceptanceHP);
   Tree->Branch("nTrackInAcceptanceHP",  &nTrackInAcceptanceHP, "nTrackInAcceptanceHP/I");
   Tree->Branch("Gpt",                   &Gpt);
   Tree->Branch("Gy",                    &Gy);
   Tree->Branch("GpdgId",                &GpdgId);
   Tree->Branch("GisSignal",             &GisSignal);
   Tree->Branch("GcollisionId",          &GcollisionId);
   Tree->Branch("GSignalType",           &GSignalType);
   return true;
}

void DzeroUPCTreeMessenger::Clear()
{
   if(Initialized == false)
      return;

   VX = 0;
   VY = 0;
   VZ = 0;
   VXError = 0;
   VYError = 0;
   VZError = 0;
   gammaN = -1;
   Ngamma = -1;
   Dpt->clear();
   Dy->clear();
   Dmass->clear();
   Dtrk1Pt->clear();
   Dtrk2Pt->clear();
   Dchi2cl->clear();
   DsvpvDistance->clear();
   DsvpvDisErr->clear();
   DsvpvDistance_2D->clear();
   DsvpvDisErr_2D->clear();
   Dalpha->clear();
   Ddtheta->clear();
   DnTrackInAcceptanceHP->clear();
   nTrackInAcceptanceHP = 0;
   Gpt->clear();
   Gy->clear();
   GpdgId->clear();
   GisSignal->clear();
   GcollisionId->clear();
   GSignalType->clear();
}

void DzeroUPCTreeMessenger::CopyNonTrack(DzeroUPCTreeMessenger &M)
{
   Run          = M.Run;
   Event        = M.Event;
   Lumi         = M.Lumi;
   VX           = M.VX;
   VY           = M.VY;
   VZ           = M.VZ;
   VXError      = M.VXError;
   VYError      = M.VYError;
   VZError      = M.VZError;
   gammaN       = M.gammaN;
   Ngamma       = M.Ngamma;
   if(Dpt != nullptr && M.Dpt != nullptr)   *Dpt = *(M.Dpt);
   if(Dy != nullptr && M.Dy != nullptr)   *Dy = *(M.Dy);
   if(Dmass != nullptr && M.Dmass != nullptr)   *Dmass = *(M.Dmass);
   if(Dtrk1Pt != nullptr && M.Dtrk1Pt != nullptr)   *Dtrk1Pt = *(M.Dtrk1Pt);
   if(Dtrk2Pt != nullptr && M.Dtrk2Pt != nullptr)   *Dtrk2Pt = *(M.Dtrk2Pt);
   if(Dchi2cl != nullptr && M.Dchi2cl != nullptr)   *Dchi2cl = *(M.Dchi2cl);
   if(DsvpvDistance != nullptr && M.DsvpvDistance != nullptr)   *DsvpvDistance = *(M.DsvpvDistance);
   if(DsvpvDisErr != nullptr && M.DsvpvDisErr != nullptr)   *DsvpvDisErr = *(M.DsvpvDisErr);
   if(DsvpvDistance_2D != nullptr && M.DsvpvDistance_2D != nullptr)   *DsvpvDistance_2D = *(M.DsvpvDistance_2D);
   if(DsvpvDisErr_2D != nullptr && M.DsvpvDisErr_2D != nullptr)   *DsvpvDisErr_2D = *(M.DsvpvDisErr_2D);
   if(Dalpha != nullptr && M.Dalpha != nullptr)   *Dalpha = *(M.Dalpha);
   if(Ddtheta != nullptr && M.Ddtheta != nullptr)   *Ddtheta = *(M.Ddtheta);
   if(DnTrackInAcceptanceHP != nullptr && M.DnTrackInAcceptanceHP != nullptr)   *DnTrackInAcceptanceHP = *(M.DnTrackInAcceptanceHP);
   nTrackInAcceptanceHP = M.nTrackInAcceptanceHP;
   if(Gpt != nullptr && M.Gpt != nullptr)   *Gpt = *(M.Gpt);
   if(Gy != nullptr && M.Gy != nullptr)   *Gy = *(M.Gy);
   if(GpdgId != nullptr && M.GpdgId != nullptr)   *GpdgId = *(M.GpdgId);
   if(GisSignal != nullptr && M.GisSignal != nullptr)   *GisSignal = *(M.GisSignal);
   if(GcollisionId != nullptr && M.GcollisionId != nullptr)   *GcollisionId = *(M.GcollisionId);
   if(GSignalType != nullptr && M.GSignalType != nullptr)   *GSignalType = *(M.GSignalType);
}

bool DzeroUPCTreeMessenger::FillEntry()
{
   if(Initialized == false)
      return false;
   if(WriteMode == false)
      return false;

   if(Tree == nullptr)
      return false;

   Tree->Fill();
   Clear();

   return true;
}
