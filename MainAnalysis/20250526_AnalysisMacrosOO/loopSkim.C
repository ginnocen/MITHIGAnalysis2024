#include "TFile.h"
#include "TTree.h"
#include <vector>

using namespace std;

int loopSkim(const char* inFileName){
   TFile  f(inFileName);
   TTree* t = nullptr;
   f.GetObject("Tree", t);
   if (!t) { fprintf(stderr,"Tree not found\n"); return 1; }

   int Run;
   vector<float>* trkPt = nullptr;
   vector<float>* trkEta = nullptr;
   t->SetBranchAddress("Run",   &Run);
   t->SetBranchAddress("trkPt", &trkPt);

   const Long64_t nEnt = t->GetEntries();
   for (Long64_t i = 0; i < nEnt; ++i)
   {
      t->GetEntry(i);
      printf("Entry %lld  evt = %d  nTrk = %zu\n",
             i, Run, trkPt->size());

      for (float pt : *trkPt) printf("   pt = %7.3f\n", pt);
   }
   return 0;
}
