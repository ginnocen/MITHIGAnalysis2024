#define MyClass_cxx
#include "MyClass.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void muonCorrections(){

MyClass t; t.Loop();

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
//   Float_t bins[] = {-4, -3.5, -3.0, -2.9, -2.8, -2.7, -2.6, -2.5, -2.4, -2.3, -2.2, -2.1, -2.0, -1.9, -1.8, -1.7, -1.6, -1.5, -1.4, -1.3, -1.2, -1.1, -1.0, -0.9, -0.8, -0.7, -0.6, -0.5, -0.4, -0.3, -0.2, -0.1, 0};
   Float_t bins[] = {-4, -3.4, -3.0, -2.8, -2.6, -2.4, -2.2, -2.0, -1.8,-1.6,  -1.4,  -1.2, -1.0, -0.8, -0.6, -0.4, -0.2, 0};
   Int_t  binnum = sizeof(bins)/sizeof(Float_t) - 1;

   Float_t bins2[] = {0.025, 0.225, 0.425, 0.625, 0.825, 1.025, 1.225, 1.425, 1.625, 1.825, 2.025, 2.225, 2.425, 2.825, 4.0};
   Int_t  binnum2 = sizeof(bins2)/sizeof(Float_t) - 1;

   histosTH1F["weight"] = new TH1F("weight", "weight", 50, -5,1.);

   for(int i=0; i<15; i++)
   {
   char ptbins[100];
   sprintf(ptbins, "_%d", i);
   string histname(ptbins);

   histosTH1F[("dimuonMass"+histname).c_str()] = new TH1F(("dimuonMass"+histname).c_str(), "dimuonMass",50,0.,10.);
   histosTH1F[("deltaPhi"+histname).c_str()] = new TH1F(("deltaPhi"+histname).c_str(), "deltaPhi",32,0.,M_PI);
   histosTH1F[("dimuonRapidity"+histname).c_str()] = new TH1F(("dimuonRapidity"+histname).c_str(), "dimuonRapidity",6,-2.4,2.4);
   histosTH1F[("dimuonPt"+histname).c_str()] = new TH1F(("dimuonPt"+histname).c_str(), "dimuonPt",5,0,100);
   histosTH1F[("jetPtResponseInclusive"+histname).c_str()] = new TH1F(("jetPtResponseInclusive"+histname).c_str(), "jetPtResponseInclusive",70,0,2);
   histosTH1F[("jetPtResponseMuMu"+histname).c_str()] = new TH1F(("jetPtResponseMuMu"+histname).c_str(), "jetPtResponseMuMu",70,0,2);
   histosTH1F[("zMuMu"+histname).c_str()] = new TH1F(("zMuMu"+histname).c_str(), "zMuMu",50,0,2);
   histosTH1F[("genDimuonRapidity"+histname).c_str()] = new TH1F(("genDimuonRapidity"+histname).c_str(), "genDimuonRapidity",6,-2.4,2.4);
   histosTH1F[("genDimuonPt"+histname).c_str()] = new TH1F(("genDimuonPt"+histname).c_str(), "genDimuonPt",5,0,100);
   histosTH1F[("DCA"+histname).c_str()] = new TH1F(("DCA"+histname).c_str(), "DCA",50,0.,.5);
   histosTH2F[("DCA_2D"+histname).c_str()] = new TH2F(("DCA_2D"+histname).c_str(), "DCA_2D",40, 0.,.2, 40, 0., .2);
   histosTH2F[("logDCA_2D"+histname).c_str()] = new TH2F(("logDCA_2D"+histname).c_str(), "logDCA_2D",100, -5.,0., 100, -5, 0.);
   histosTH2F[("logDCA_vs_significance"+histname).c_str()] = new TH2F(("logDCA_vs_significance"+histname).c_str(), "logDCA_vs_significance",100, -5.,0., 100, 0, 10.);   
   
   histosTH1F[("DCA1DCA2"+histname).c_str()] = new TH1F(("DCA1DCA2"+histname).c_str(), "DCA1DCA2",500, 0.,.5);
   histosTH1F[("logDCA1DCA2"+histname).c_str()] = new TH1F(("logDCA1DCA2"+histname).c_str(), "logDCA1DCA2",100, -5.,0.);
   histosTH1F[("unsignedDCA1DCA2"+histname).c_str()] = new TH1F(("unsignedDCA1DCA2"+histname).c_str(), "DCA1DCA2",100, 0.,.5);
   histosTH1F[("bJetPt"+histname).c_str()] = new TH1F(("bJetPt"+histname).c_str(), "bJetPt",50,0.,300);
   histosTH1F[("jetPt1"+histname).c_str()] = new TH1F(("jetPt1"+histname).c_str(), "jetPt1",50,0.,300);
   histosTH1F[("normDCA"+histname).c_str()] = new TH1F(("normDCA"+histname).c_str(), "normDCA",100,0.,10);
   histosTH2F[("normDCA_2D"+histname).c_str()] = new TH2F(("normDCA_2D"+histname).c_str(), "normDCA_2D",40, 0.,.2, 40, 0., .2);
   histosTH2F[("numerator_dimuon"+histname).c_str()] = new TH2F(("numerator"+histname).c_str(), "numerator",12, -2.4, 2.4, 10, 0,100);
   histosTH2F[("denominator_dimuon"+histname).c_str()] = new TH2F(("denominator"+histname).c_str(), "numerator",12, -2.4, 2.4, 10, 0,100);
   histosTH2F[("recoPt_vs_genPt"+histname).c_str()] = new TH2F(("recoPt_vs_genPtr"+histname).c_str(), "recoPt_vs_genPt",15, 0, 60, 15, 0,60);
   }

   for(map<string,TH1F*>::const_iterator it = histosTH1F.begin(); it != histosTH1F.end(); ++it)
      it->second->Sumw2();

   for(map<string,TH2F*>::const_iterator it = histosTH2F.begin(); it != histosTH2F.end(); ++it)
      it->second->Sumw2();


   TFile* output = new TFile("muonCorrections.root","RECREATE");

// 1, 2, 3, 4, 7, 8, 9, 10,
   int numberOfSingleBjets = 0;
   int numberOfDoubleSingleBjets = 0;
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry < nentries;jentry++) {
      bool dimuonSingleBjet = false;
      bool dimuonSingleBjetNc1 = false;
      bool dimuonSingleBjetNc2 = false;
      bool singleBjet = false;
      bool singleBjetNc1 = false;
      bool singleBjetNc2 = false;

      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      if (!(JetPT->size() > 0)) continue;
      double weight = NCollWeight*EventWeight;

      int nSingleBjets = 0;
      vector<double> bJetPt, bJetEta, bJetPhi, bGenJetPt;
      vector<double> jetPt, jetEta, jetPhi;

       for (Long64_t j =0; j < JetPT->size() ; j++)
     { 
      jetPt.push_back(JetPT->at(j)); jetEta.push_back(JetEta->at(j)); jetPhi.push_back(JetPhi->at(j));
//      cout << JetPT->at(j) << endl;
       if (genMuMuPt->size() > 0  && JetPT->at(j) > 75. && genMuPt1->at(j) > 4 && genMuPt2->at(j) > 4 )
      {

       histosTH2F["denominator_dimuon_0"]->Fill( genMuMuY->at(j), genMuMuPt->at(j) , weight);
       histosTH2F["denominator_dimuon_10"]->Fill( genMuMuY->at(j), genMuMuPt->at(j) ,weight/EventWeight);
       histosTH2F["denominator_dimuon_9"]->Fill( genMuMuY->at(j), genMuMuPt->at(j) ,weight/NCollWeight);
       if ( MJTNcHad->at(j) > 0  && MJTNbHad->at(j) == 0  )         histosTH2F["denominator_dimuon_11"]->Fill( genMuMuY->at(j), genMuMuPt->at(j) ,weight);
       if ( MJTNbHad->at(j) > 0  )      histosTH2F["denominator_dimuon_12"]->Fill( genMuMuY->at(j), genMuMuPt->at(j) ,weight);
       if ( MJTNbHad->at(j) > 0  )      histosTH2F["denominator_dimuon_13"]->Fill( genMuMuY->at(j), genMuMuPt->at(j) ,weight/2.); //Systematic variation, enhances b and double b by factor of 2
       if ( MJTNbHad->at(j) == 0  )      histosTH2F["denominator_dimuon_13"]->Fill( genMuMuY->at(j), genMuMuPt->at(j) ,weight);
       if ( MJTNbHad->at(j) > 0  )      histosTH2F["denominator_dimuon_14"]->Fill( genMuMuY->at(j), genMuMuPt->at(j) ,weight*2.); //Systematic variation, suppresses b and double b by factor of 2
       if ( MJTNbHad->at(j) == 0  )      histosTH2F["denominator_dimuon_14"]->Fill( genMuMuY->at(j), genMuMuPt->at(j) ,weight);

       histosTH1F["genDimuonPt_0"]->Fill(genMuMuPt->at(j),weight);
      }
      if (mumuMass->size() > 0 && mumuMass->at(j) > 0 && JetPT->at(j) > 75 && IsMuMuTagged->at(j) == true && fabs(muDiDxy1->at(j)) > 0.01 && fabs(muDiDxy2->at(j)) > 0.01 && muPt1->at(j)  > 4. && muPt2->at(j) >  4.  )
      {

        if ( genMuMuPt->size() == 0) continue;
 
         if      (! (    ( (muEta2->at(j)-genMuEta1->at(j))*(muEta2->at(j)-genMuEta1->at(j))+ ( muPhi2->at(j)-genMuPhi1->at(j))*( muPhi2->at(j)-genMuPhi1->at(j))  <  0.0009 ) ||
                    ( ( muEta2->at(j)-genMuEta2->at(j))*( muEta2->at(j)-genMuEta2->at(j))+ ( muPhi2->at(j)-genMuPhi2->at(j))*( muPhi2->at(j)-genMuPhi2->at(j))  < 0.0009 )) ) continue;
         if      (! (    ( (muEta1->at(j)-genMuEta1->at(j))*(muEta1->at(j)-genMuEta1->at(j))+ ( muPhi1->at(j)-genMuPhi1->at(j))*( muPhi1->at(j)-genMuPhi1->at(j))  <  0.0009 ) ||
                    ( ( muEta1->at(j)-genMuEta2->at(j))*( muEta1->at(j)-genMuEta2->at(j))+ ( muPhi1->at(j)-genMuPhi2->at(j))*( muPhi1->at(j)-genMuPhi2->at(j))  < 0.0009 )))  continue;
         {

          histosTH2F["numerator_dimuon_0"]->Fill(mumuY->at(j) , mumuPt->at(j), ExtraMuWeight[0]*weight);
          histosTH2F["numerator_dimuon_1"]->Fill(mumuY->at(j) , mumuPt->at(j), ExtraMuWeight[0]*ExtraMuWeight[1]*weight);
          histosTH2F["numerator_dimuon_2"]->Fill(mumuY->at(j) , mumuPt->at(j), ExtraMuWeight[0]*ExtraMuWeight[2]*weight);
          histosTH2F["numerator_dimuon_3"]->Fill(mumuY->at(j) , mumuPt->at(j), ExtraMuWeight[0]*ExtraMuWeight[3]*weight);
          histosTH2F["numerator_dimuon_4"]->Fill(mumuY->at(j) , mumuPt->at(j), ExtraMuWeight[0]*ExtraMuWeight[4]*weight);
          histosTH2F["numerator_dimuon_5"]->Fill(mumuY->at(j) , mumuPt->at(j), ExtraMuWeight[0]*ExtraMuWeight[7]*weight);
          histosTH2F["numerator_dimuon_6"]->Fill(mumuY->at(j) , mumuPt->at(j), ExtraMuWeight[0]*ExtraMuWeight[8]*weight);
          histosTH2F["numerator_dimuon_7"]->Fill(mumuY->at(j) , mumuPt->at(j), ExtraMuWeight[0]*ExtraMuWeight[9]*weight);
          histosTH2F["numerator_dimuon_8"]->Fill(mumuY->at(j) , mumuPt->at(j), ExtraMuWeight[0]*ExtraMuWeight[10]*weight);
          histosTH2F["numerator_dimuon_9"]->Fill(mumuY->at(j) , mumuPt->at(j), ExtraMuWeight[0]*weight/NCollWeight);
          histosTH2F["numerator_dimuon_10"]->Fill(mumuY->at(j) , mumuPt->at(j), ExtraMuWeight[0]*weight/EventWeight);
          if ( MJTNcHad->at(j) > 0  && MJTNbHad->at(j) == 0  )   histosTH2F["numerator_dimuon_11"]->Fill(mumuY->at(j) , mumuPt->at(j), ExtraMuWeight[0]*weight);
          if ( MJTNbHad->at(j) > 0  )   histosTH2F["numerator_dimuon_12"]->Fill(mumuY->at(j) , mumuPt->at(j), ExtraMuWeight[0]*weight);
          if ( MJTNbHad->at(j) > 0  )   histosTH2F["numerator_dimuon_13"]->Fill(mumuY->at(j) , mumuPt->at(j), ExtraMuWeight[0]*weight/2.);
          if ( MJTNbHad->at(j) == 0  )   histosTH2F["numerator_dimuon_13"]->Fill(mumuY->at(j) , mumuPt->at(j), ExtraMuWeight[0]*weight);
          if ( MJTNbHad->at(j) > 0  )   histosTH2F["numerator_dimuon_14"]->Fill(mumuY->at(j) , mumuPt->at(j), ExtraMuWeight[0]*weight*2.);
          if ( MJTNbHad->at(j) == 0  )   histosTH2F["numerator_dimuon_14"]->Fill(mumuY->at(j) , mumuPt->at(j), ExtraMuWeight[0]*weight);
          histosTH1F["dimuonPt_0"]->Fill(mumuPt->at(j), ExtraMuWeight[0]*weight);
          histosTH1F["dimuonPt_1"]->Fill(mumuPt->at(j), ExtraMuWeight[0]*ExtraMuWeight[1]*weight);
          histosTH1F["dimuonPt_2"]->Fill(mumuPt->at(j), ExtraMuWeight[0]*ExtraMuWeight[2]*weight );
          histosTH1F["dimuonPt_3"]->Fill(mumuPt->at(j), ExtraMuWeight[0]*ExtraMuWeight[3]*weight );
          histosTH1F["dimuonPt_4"]->Fill(mumuPt->at(j), ExtraMuWeight[0]*ExtraMuWeight[4]*weight );
          histosTH1F["dimuonPt_7"]->Fill(mumuPt->at(j), ExtraMuWeight[0]*ExtraMuWeight[7]*weight );
          histosTH1F["dimuonPt_8"]->Fill(mumuPt->at(j), ExtraMuWeight[0]*ExtraMuWeight[8]*weight );
          histosTH1F["dimuonPt_9"]->Fill(mumuPt->at(j), ExtraMuWeight[0]*ExtraMuWeight[9]*weight );
          histosTH1F["dimuonPt_10"]->Fill(mumuPt->at(j), ExtraMuWeight[0]*ExtraMuWeight[10]*weight );
          histosTH2F["recoPt_vs_genPt_0"]->Fill( genMuMuPt->at(j) , mumuPt->at(j), weight);
         }
      }
     }
   }
          histosTH2F["numerator_dimuon_0"]->Divide(histosTH2F["denominator_dimuon_0"]);
          histosTH2F["numerator_dimuon_1"]->Divide(histosTH2F["denominator_dimuon_0"]);
          histosTH2F["numerator_dimuon_2"]->Divide(histosTH2F["denominator_dimuon_0"]);
          histosTH2F["numerator_dimuon_3"]->Divide(histosTH2F["denominator_dimuon_0"]);
          histosTH2F["numerator_dimuon_4"]->Divide(histosTH2F["denominator_dimuon_0"]);
          histosTH2F["numerator_dimuon_5"]->Divide(histosTH2F["denominator_dimuon_0"]);
          histosTH2F["numerator_dimuon_6"]->Divide(histosTH2F["denominator_dimuon_0"]);
          histosTH2F["numerator_dimuon_7"]->Divide(histosTH2F["denominator_dimuon_0"]);
          histosTH2F["numerator_dimuon_8"]->Divide(histosTH2F["denominator_dimuon_0"]);
          histosTH2F["numerator_dimuon_9"]->Divide(histosTH2F["denominator_dimuon_9"]);
	  histosTH2F["numerator_dimuon_10"]->Divide(histosTH2F["denominator_dimuon_10"]);
          histosTH2F["numerator_dimuon_11"]->Divide(histosTH2F["denominator_dimuon_11"]);
          histosTH2F["numerator_dimuon_12"]->Divide(histosTH2F["denominator_dimuon_12"]);
          histosTH2F["numerator_dimuon_13"]->Divide(histosTH2F["denominator_dimuon_13"]);
          histosTH2F["numerator_dimuon_14"]->Divide(histosTH2F["denominator_dimuon_14"]);

  output->cd();
  for(map<string,TH1F*>::iterator it_histo = histosTH1F.begin();
                                  it_histo != histosTH1F.end(); ++it_histo)
     (*it_histo).second->Write();

  for(map<string,TH2F*>::iterator it_histo = histosTH2F.begin();
                                  it_histo != histosTH2F.end(); ++it_histo)
     (*it_histo).second->Write();

  output->Close();

}
