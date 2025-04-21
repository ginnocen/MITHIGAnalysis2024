//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Jan 29 12:06:49 2025 by ROOT version 6.32.08
// from TTree Tree/Tree for MuMu tagged jet analysis (V8)
// found on file: mergedfile_01282025_hannah.root
//////////////////////////////////////////////////////////

#ifndef MyClass_h
#define MyClass_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "vector"
#include "vector"

class MyClass {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           Run;
   Long64_t        Event;
   Int_t           Lumi;
   Int_t           hiBin;
   Float_t         hiHF;
   Int_t           NVertex;
   Float_t         VX;
   Float_t         VY;
   Float_t         VZ;
   Float_t         VXError;
   Float_t         VYError;
   Float_t         VZError;
   Int_t           NPU;
   Float_t         NCollWeight;
   Float_t         ExtraMuWeight[12];
   vector<float>   *JetPT;
   vector<float>   *JetEta;
   vector<float>   *JetPhi;
   vector<bool>    *IsMuMuTagged;
   vector<float>   *genJetPT;
   vector<float>   *genJetEta;
   vector<float>   *genJetPhi;
   vector<bool>    *genIsMuMuTagged;
   vector<float>   *unmatchedGenJetPT;
   vector<float>   *unmatchedGenJetEta;
   vector<float>   *unmatchedGenJetPhi;
   vector<bool>    *unmatchedGenIsMuMuTagged;
   vector<float>   *muPt1;
   vector<float>   *muPt2;
   vector<float>   *muEta1;
   vector<float>   *muEta2;
   vector<float>   *muPhi1;
   vector<float>   *muPhi2;
   vector<float>   *muDiDxy1;
   vector<float>   *muDiDxy1Err;
   vector<float>   *muDiDxy2;
   vector<float>   *muDiDxy2Err;
   vector<float>   *muDiDz1;
   vector<float>   *muDiDz1Err;
   vector<float>   *muDiDz2;
   vector<float>   *muDiDz2Err;
   vector<float>   *muDiDxy1Dxy2;
   vector<float>   *muDiDxy1Dxy2Err;
   vector<float>   *mumuMass;
   vector<float>   *mumuEta;
   vector<float>   *mumuY;
   vector<float>   *mumuPhi;
   vector<float>   *mumuPt;
   vector<float>   *DRJetmu1;
   vector<float>   *DRJetmu2;
   vector<float>   *muDeta;
   vector<float>   *muDphi;
   vector<float>   *muDR;
   vector<float>   *genMuPt1;
   vector<float>   *genMuPt2;
   vector<float>   *genMuEta1;
   vector<float>   *genMuEta2;
   vector<float>   *genMuPhi1;
   vector<float>   *genMuPhi2;
   vector<float>   *genMuMuMass;
   vector<float>   *genMuMuEta;
   vector<float>   *genMuMuY;
   vector<float>   *genMuMuPhi;
   vector<float>   *genMuMuPt;
   vector<float>   *genMuDeta;
   vector<float>   *genMuDphi;
   vector<float>   *genMuDR;
   vector<int>     *MJTHadronFlavor;
   vector<int>     *MJTNcHad;
   vector<int>     *MJTNbHad;

   // List of branches
   TBranch        *b_Run;   //!
   TBranch        *b_Event;   //!
   TBranch        *b_Lumi;   //!
   TBranch        *b_hiBin;   //!
   TBranch        *b_hiHF;   //!
   TBranch        *b_NVertex;   //!
   TBranch        *b_VX;   //!
   TBranch        *b_VY;   //!
   TBranch        *b_VZ;   //!
   TBranch        *b_VXError;   //!
   TBranch        *b_VYError;   //!
   TBranch        *b_VZError;   //!
   TBranch        *b_NPU;   //!
   TBranch        *b_NCollWeight;   //!
   TBranch        *b_ExtraMuWeight;   //!
   TBranch        *b_JetPT;   //!
   TBranch        *b_JetEta;   //!
   TBranch        *b_JetPhi;   //!
   TBranch        *b_IsMuMuTagged;   //!
   TBranch        *b_genJetPT;   //!
   TBranch        *b_genJetEta;   //!
   TBranch        *b_genJetPhi;   //!
   TBranch        *b_genIsMuMuTagged;   //!
   TBranch        *b_unmatchedGenJetPT;   //!
   TBranch        *b_unmatchedGenJetEta;   //!
   TBranch        *b_unmatchedGenJetPhi;   //!
   TBranch        *b_unmatchedGenIsMuMuTagged;   //!
   TBranch        *b_muPt1;   //!
   TBranch        *b_muPt2;   //!
   TBranch        *b_muEta1;   //!
   TBranch        *b_muEta2;   //!
   TBranch        *b_muPhi1;   //!
   TBranch        *b_muPhi2;   //!
   TBranch        *b_muDiDxy1;   //!
   TBranch        *b_muDiDxy1Err;   //!
   TBranch        *b_muDiDxy2;   //!
   TBranch        *b_muDiDxy2Err;   //!
   TBranch        *b_muDiDz1;   //!
   TBranch        *b_muDiDz1Err;   //!
   TBranch        *b_muDiDz2;   //!
   TBranch        *b_muDiDz2Err;   //!
   TBranch        *b_muDiDxy1Dxy2;   //!
   TBranch        *b_muDiDxy1Dxy2Err;   //!
   TBranch        *b_mumuMass;   //!
   TBranch        *b_mumuEta;   //!
   TBranch        *b_mumuY;   //!
   TBranch        *b_mumuPhi;   //!
   TBranch        *b_mumuPt;   //!
   TBranch        *b_DRJetmu1;   //!
   TBranch        *b_DRJetmu2;   //!
   TBranch        *b_muDeta;   //!
   TBranch        *b_muDphi;   //!
   TBranch        *b_muDR;   //!
   TBranch        *b_genMuPt1;   //!
   TBranch        *b_genMuPt2;   //!
   TBranch        *b_genMuEta1;   //!
   TBranch        *b_genMuEta2;   //!
   TBranch        *b_genMuPhi1;   //!
   TBranch        *b_genMuPhi2;   //!
   TBranch        *b_genMuMuMass;   //!
   TBranch        *b_genMuMuEta;   //!
   TBranch        *b_genMuMuY;   //!
   TBranch        *b_genMuMuPhi;   //!
   TBranch        *b_genMuMuPt;   //!
   TBranch        *b_genMuDeta;   //!
   TBranch        *b_genMuDphi;   //!
   TBranch        *b_genMuDR;   //!
   TBranch        *b_MJTHadronFlavor;   //!
   TBranch        *b_MJTNcHad;   //!
   TBranch        *b_MJTNbHad;   //!

   MyClass(TTree *tree=0);
   virtual ~MyClass();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual bool     Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef MyClass_cxx
MyClass::MyClass(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("mergedfileData_02042025_mbAll.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("mergedfileData_02042025_mbAll.root");
      }
      f->GetObject("Tree",tree);

   }
   Init(tree);
}

MyClass::~MyClass()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t MyClass::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t MyClass::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void MyClass::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   JetPT = 0;
   JetEta = 0;
   JetPhi = 0;
   IsMuMuTagged = 0;
   genJetPT = 0;
   genJetEta = 0;
   genJetPhi = 0;
   genIsMuMuTagged = 0;
   unmatchedGenJetPT = 0;
   unmatchedGenJetEta = 0;
   unmatchedGenJetPhi = 0;
   unmatchedGenIsMuMuTagged = 0;
   muPt1 = 0;
   muPt2 = 0;
   muEta1 = 0;
   muEta2 = 0;
   muPhi1 = 0;
   muPhi2 = 0;
   muDiDxy1 = 0;
   muDiDxy1Err = 0;
   muDiDxy2 = 0;
   muDiDxy2Err = 0;
   muDiDz1 = 0;
   muDiDz1Err = 0;
   muDiDz2 = 0;
   muDiDz2Err = 0;
   muDiDxy1Dxy2 = 0;
   muDiDxy1Dxy2Err = 0;
   mumuMass = 0;
   mumuEta = 0;
   mumuY = 0;
   mumuPhi = 0;
   mumuPt = 0;
   DRJetmu1 = 0;
   DRJetmu2 = 0;
   muDeta = 0;
   muDphi = 0;
   muDR = 0;
   genMuPt1 = 0;
   genMuPt2 = 0;
   genMuEta1 = 0;
   genMuEta2 = 0;
   genMuPhi1 = 0;
   genMuPhi2 = 0;
   genMuMuMass = 0;
   genMuMuEta = 0;
   genMuMuY = 0;
   genMuMuPhi = 0;
   genMuMuPt = 0;
   genMuDeta = 0;
   genMuDphi = 0;
   genMuDR = 0;
   MJTHadronFlavor = 0;
   MJTNcHad = 0;
   MJTNbHad = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Run", &Run, &b_Run);
   fChain->SetBranchAddress("Event", &Event, &b_Event);
   fChain->SetBranchAddress("Lumi", &Lumi, &b_Lumi);
   fChain->SetBranchAddress("hiBin", &hiBin, &b_hiBin);
   fChain->SetBranchAddress("hiHF", &hiHF, &b_hiHF);
   fChain->SetBranchAddress("NVertex", &NVertex, &b_NVertex);
   fChain->SetBranchAddress("VX", &VX, &b_VX);
   fChain->SetBranchAddress("VY", &VY, &b_VY);
   fChain->SetBranchAddress("VZ", &VZ, &b_VZ);
   fChain->SetBranchAddress("VXError", &VXError, &b_VXError);
   fChain->SetBranchAddress("VYError", &VYError, &b_VYError);
   fChain->SetBranchAddress("VZError", &VZError, &b_VZError);
   fChain->SetBranchAddress("NPU", &NPU, &b_NPU);
   fChain->SetBranchAddress("NCollWeight", &NCollWeight, &b_NCollWeight);
   fChain->SetBranchAddress("ExtraMuWeight", ExtraMuWeight, &b_ExtraMuWeight);
   fChain->SetBranchAddress("JetPT", &JetPT, &b_JetPT);
   fChain->SetBranchAddress("JetEta", &JetEta, &b_JetEta);
   fChain->SetBranchAddress("JetPhi", &JetPhi, &b_JetPhi);
   fChain->SetBranchAddress("IsMuMuTagged", &IsMuMuTagged, &b_IsMuMuTagged);
   fChain->SetBranchAddress("genJetPT", &genJetPT, &b_genJetPT);
   fChain->SetBranchAddress("genJetEta", &genJetEta, &b_genJetEta);
   fChain->SetBranchAddress("genJetPhi", &genJetPhi, &b_genJetPhi);
   fChain->SetBranchAddress("genIsMuMuTagged", &genIsMuMuTagged, &b_genIsMuMuTagged);
   fChain->SetBranchAddress("unmatchedGenJetPT", &unmatchedGenJetPT, &b_unmatchedGenJetPT);
   fChain->SetBranchAddress("unmatchedGenJetEta", &unmatchedGenJetEta, &b_unmatchedGenJetEta);
   fChain->SetBranchAddress("unmatchedGenJetPhi", &unmatchedGenJetPhi, &b_unmatchedGenJetPhi);
   fChain->SetBranchAddress("unmatchedGenIsMuMuTagged", &unmatchedGenIsMuMuTagged, &b_unmatchedGenIsMuMuTagged);
   fChain->SetBranchAddress("muPt1", &muPt1, &b_muPt1);
   fChain->SetBranchAddress("muPt2", &muPt2, &b_muPt2);
   fChain->SetBranchAddress("muEta1", &muEta1, &b_muEta1);
   fChain->SetBranchAddress("muEta2", &muEta2, &b_muEta2);
   fChain->SetBranchAddress("muPhi1", &muPhi1, &b_muPhi1);
   fChain->SetBranchAddress("muPhi2", &muPhi2, &b_muPhi2);
   fChain->SetBranchAddress("muDiDxy1", &muDiDxy1, &b_muDiDxy1);
   fChain->SetBranchAddress("muDiDxy1Err", &muDiDxy1Err, &b_muDiDxy1Err);
   fChain->SetBranchAddress("muDiDxy2", &muDiDxy2, &b_muDiDxy2);
   fChain->SetBranchAddress("muDiDxy2Err", &muDiDxy2Err, &b_muDiDxy2Err);
   fChain->SetBranchAddress("muDiDz1", &muDiDz1, &b_muDiDz1);
   fChain->SetBranchAddress("muDiDz1Err", &muDiDz1Err, &b_muDiDz1Err);
   fChain->SetBranchAddress("muDiDz2", &muDiDz2, &b_muDiDz2);
   fChain->SetBranchAddress("muDiDz2Err", &muDiDz2Err, &b_muDiDz2Err);
   fChain->SetBranchAddress("muDiDxy1Dxy2", &muDiDxy1Dxy2, &b_muDiDxy1Dxy2);
   fChain->SetBranchAddress("muDiDxy1Dxy2Err", &muDiDxy1Dxy2Err, &b_muDiDxy1Dxy2Err);
   fChain->SetBranchAddress("mumuMass", &mumuMass, &b_mumuMass);
   fChain->SetBranchAddress("mumuEta", &mumuEta, &b_mumuEta);
   fChain->SetBranchAddress("mumuY", &mumuY, &b_mumuY);
   fChain->SetBranchAddress("mumuPhi", &mumuPhi, &b_mumuPhi);
   fChain->SetBranchAddress("mumuPt", &mumuPt, &b_mumuPt);
   fChain->SetBranchAddress("DRJetmu1", &DRJetmu1, &b_DRJetmu1);
   fChain->SetBranchAddress("DRJetmu2", &DRJetmu2, &b_DRJetmu2);
   fChain->SetBranchAddress("muDeta", &muDeta, &b_muDeta);
   fChain->SetBranchAddress("muDphi", &muDphi, &b_muDphi);
   fChain->SetBranchAddress("muDR", &muDR, &b_muDR);
   fChain->SetBranchAddress("genMuPt1", &genMuPt1, &b_genMuPt1);
   fChain->SetBranchAddress("genMuPt2", &genMuPt2, &b_genMuPt2);
   fChain->SetBranchAddress("genMuEta1", &genMuEta1, &b_genMuEta1);
   fChain->SetBranchAddress("genMuEta2", &genMuEta2, &b_genMuEta2);
   fChain->SetBranchAddress("genMuPhi1", &genMuPhi1, &b_genMuPhi1);
   fChain->SetBranchAddress("genMuPhi2", &genMuPhi2, &b_genMuPhi2);
   fChain->SetBranchAddress("genMuMuMass", &genMuMuMass, &b_genMuMuMass);
   fChain->SetBranchAddress("genMuMuEta", &genMuMuEta, &b_genMuMuEta);
   fChain->SetBranchAddress("genMuMuY", &genMuMuY, &b_genMuMuY);
   fChain->SetBranchAddress("genMuMuPhi", &genMuMuPhi, &b_genMuMuPhi);
   fChain->SetBranchAddress("genMuMuPt", &genMuMuPt, &b_genMuMuPt);
   fChain->SetBranchAddress("genMuDeta", &genMuDeta, &b_genMuDeta);
   fChain->SetBranchAddress("genMuDphi", &genMuDphi, &b_genMuDphi);
   fChain->SetBranchAddress("genMuDR", &genMuDR, &b_genMuDR);
   fChain->SetBranchAddress("MJTHadronFlavor", &MJTHadronFlavor, &b_MJTHadronFlavor);
   fChain->SetBranchAddress("MJTNcHad", &MJTNcHad, &b_MJTNcHad);
   fChain->SetBranchAddress("MJTNbHad", &MJTNbHad, &b_MJTNbHad);
   Notify();
}

bool MyClass::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return true;
}

void MyClass::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t MyClass::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef MyClass_cxx
