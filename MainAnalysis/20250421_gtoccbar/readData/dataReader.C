#define MyClass_cxx
#include "MyClass.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void dataReader()
{
MyClass t;
t.Loop();
}

void MyClass::Loop()
{
//   In a ROOT session, you can do:
//      root> .L MyAnalyzer.C
//      root> MyAnalyzer t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;


   map<string,TH1F*> histosTH1F;
   map<string,TH2F*> histosTH2F;
   map<string,TH2D*> histosTH2D;
//   Float_t bins[] = {-4, -3.5, -3.0, -2.9, -2.8, -2.7, -2.6, -2.5, -2.4, -2.3, -2.2, -2.1, -2.0, -1.9, -1.8, -1.7, -1.6, -1.5, -1.4, -1.3, -1.2, -1.1, -1.0, -0.9, -0.8, -0.7, -0.6, -0.5, -0.4, -0.3, -0.2, -0.1, 0};
   Float_t bins[] = {-4, -3.4, -3.0, -2.8, -2.6, -2.4, -2.2, -2.0, -1.8,-1.6,  -1.4,  -1.2, -1.0, -0.8, -0.6, -0.4, -0.2, 0};
   Int_t  binnum = sizeof(bins)/sizeof(Float_t) - 1;

   Float_t bins2[] = {0.025, 0.225, 0.425, 0.625, 0.825, 1.025, 1.225, 1.425, 1.625, 1.825, 2.025, 2.225, 2.425, 2.825, 4.0};
   Int_t  binnum2 = sizeof(bins2)/sizeof(Float_t) - 1;

   double pTmin = 75.; double pTmax = 220.; int nBins = 6;
        Double_t ybins_true[7];
         ybins_true[0]=pTmin;
         ybins_true[1]=90.;
         ybins_true[2]=105.;
         ybins_true[3]=125.;
         ybins_true[4]=145.;
         ybins_true[5]=165.;
	 ybins_true[6]=pTmax;


        TH1D *pTjet_reco(0);
   pTjet_reco = new TH1D("pTjet_reco", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_recoDimuon(0);
   pTjet_recoDimuon = new TH1D("pTjet_recoDimuon", "pTjet_recoDimuon", nBins, ybins_true);

        TH1D *pTjet_recoDimuonNumerator(0);
   pTjet_recoDimuonNumerator = new TH1D("pTjet_recoDimuonNumerator", "pTjet_recoDimuonNumerator", nBins, ybins_true);


   for(int i=0; i<15; i++)
   {
   char ptbins[100];
   sprintf(ptbins, "_%d", i);
   string histname(ptbins);

   histosTH1F[("jetPtDimuon"+histname).c_str()] = new TH1F(("jetPtDimuon"+histname).c_str(), "jetPtDimuon", nBins, ybins_true);
   histosTH1F[("jetPt"+histname).c_str()] = new TH1F(("jetPt"+histname).c_str(), "jetPt", nBins, ybins_true);
   }

   for(map<string,TH1F*>::const_iterator it = histosTH1F.begin(); it != histosTH1F.end(); ++it)
      it->second->Sumw2();

   for(map<string,TH2F*>::const_iterator it = histosTH2F.begin(); it != histosTH2F.end(); ++it)
      it->second->Sumw2();

   TFile* muonCorrections = new TFile("muonCorrections.root","READ");

   for(int i=0; i<15; i++)
   {
   char ptbinsRead[100];
   sprintf(ptbinsRead, "_%d", i);
   string histnameRead(ptbinsRead);

   histosTH2D[("numerator_dimuon"+histnameRead).c_str()] = (TH2D*) muonCorrections->Get(("numerator"+histnameRead).c_str());
   }


   TFile* output = new TFile("readData.root","RECREATE");

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry< nentries; jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      if (!(JetPT->size() > 0)) continue;     
      
       for (Long64_t j =0; j < JetPT->size() ; j++)
     { 
      histosTH1F["jetPt_0"]->Fill(JetPT->at(j));
      if (!(JetPT->size() > 0 && mumuMass->size() > 0 )) continue;
      if ( JetPT->at(j) < 75 ) continue;
      pTjet_reco->Fill(JetPT->at(j));
      if (IsMuMuTagged->at(j) == false) continue;
      if (!( muPt1->at(j)  > 4. && muPt2->at(j) >  4. )) continue;
      if ( !(fabs(muDiDxy2->at(j)) > 0.01 && fabs(muDiDxy1->at(j)) > 0.01 )) continue;
       pTjet_recoDimuon->Fill(JetPT->at(j));
       pTjet_recoDimuonNumerator->Fill(JetPT->at(j)); 

   for(int i=0; i<15; i++) //For loop iteration over systematic uncertainties for acceptance*efficiency maps.
   {
   char ptbinsRead[100];
   sprintf(ptbinsRead, "_%d", i);
   string histnameRead(ptbinsRead);
   double weight;
   weight =  histosTH2D[("numerator_dimuon"+histnameRead).c_str()]->GetBinContent(histosTH2D[("numerator_dimuon"+histnameRead).c_str()]->FindBin(mumuY->at(j), mumuPt->at(j))) ;
   if (weight > 0) weight = 1/weight; //this is to correct the data
   if (weight > 100 ) weight = 0; //set empty bins to 0

   histosTH1F[("jetPtDimuon"+histnameRead).c_str()]->Fill(JetPT->at(j),weight); //weight corrects for dimuon acceptance*efficiency

   }


     }
   }

  pTjet_recoDimuonNumerator->Divide(pTjet_reco);
  pTjet_recoDimuonNumerator->Write();
  pTjet_recoDimuon->Write();
  pTjet_reco->Write();
  output->cd();
  for(map<string,TH1F*>::iterator it_histo = histosTH1F.begin();
                                  it_histo != histosTH1F.end(); ++it_histo)
     (*it_histo).second->Write();

  for(map<string,TH2F*>::iterator it_histo = histosTH2F.begin();
                                  it_histo != histosTH2F.end(); ++it_histo)
     (*it_histo).second->Write();

  output->Close();

}
