//=====================================================================-*-C++-*-
// File and Version Information:
//      $Id$
//
// Description:
//      Simple example usage of the RooUnfold package using toy MC.
//
// Authors: Tim Adye <T.J.Adye@rl.ac.uk> and Fergus Wilson <fwilson@slac.stanford.edu>
//
//==============================================================================

#if !(defined(__CINT__) || defined(__CLING__)) || defined(__ACLIC__)
#include <iostream>
using std::cout;
using std::endl;
using std::max;
#include "TRandom.h"
#include "TH1D.h"
#include "TCanvas.h"

#include "RooUnfoldResponse.h"
#include "RooUnfoldBayes.h"
//#include "RooUnfoldSvd.h"
//#include "RooUnfoldTUnfold.h"
//#include "RooUnfoldIds.h"
#endif

void RooUnfoldExample()
{


   JetCorrectionUncertainty *jecUnc;
   vector<JetCorrectionUncertainty*> vsrc;
   jecUnc = new JetCorrectionUncertainty("/home/cbaldene/Unfolding/RooUnfoldSetupForLuis/examples/JERC2018_HI/Autumn18_HI_V8_MC_Uncertainty_AK4PF.txt");
//MC, Frag, Flavor,AbsStat, AbsScale, AbsMPF, ECAL, HCAL, RelStat, RelPt, RelBal, RelJER, L3Res, Total
const int nsrc = 14;
const char* srcnames[nsrc] =
  {"MC", "Frag", "Flavor", "AbsStat", "AbsScale", "AbsMPF", "ECAL", "HCAL", "RelStat", "RelPt", "RelBal", "RelJER", "L3Res", "Total"};


//   jecUnc = new JetCorrectionUncertainty("/home/cbaldene/Unfolding/RooUnfoldSetupForLuis/examples/JEC2018_MC/Summer19UL18_V5_MC_Uncertainty_AK4PFchs.txt");


for (int isrc = 0; isrc < nsrc; isrc++)
{
   const char *name = srcnames[isrc];
   JetCorrectorParameters p("/home/cbaldene/Unfolding/RooUnfoldSetupForLuis/examples/JERC2018_HI/Autumn18_HI_V8_MC_UncertaintySources_AK3PF.txt", name);
   vsrc.push_back(new JetCorrectionUncertainty(p));
}


  JME::JetResolution resolution ;
  JME::JetResolutionScaleFactor resolution_sf ;
  resolution_sf = JME::JetResolutionScaleFactor("/home/cbaldene/Unfolding/RooUnfoldSetupForLuis/examples/JERC2018_HI/Autumn18_RunD_V7b_MC_SF_AK4PF.txt");

  TRandom3* rand = new TRandom3(0);
  /*double pTmin = 75; double pTmax  = 200;
  int nBins = 4;
        Double_t ybins_true[5];
         ybins_true[0]=pTmin;
         ybins_true[1]=95.;
         ybins_true[2]=120.;
         ybins_true[3]=145.;
         ybins_true[4]=pTmax;

*/
   double pTmin = 75.; double pTmax = 220.; int nBins = 6;
        Double_t ybins_true[7];
         ybins_true[0]=pTmin;
         ybins_true[1]=90.;
         ybins_true[2]=105.;
         ybins_true[3]=125.;
         ybins_true[4]=145.;
         ybins_true[5]=165.;
	 ybins_true[6]=pTmax;
//         ybins_true[7]=pTmax;


  TH1D *purityNumerator(0);
   purityNumerator = new TH1D("purityNum", "purityNum", nBins, ybins_true);

        TH1D *pTjetResponse(0);
   pTjetResponse = new TH1D("pTjetRatio", "pTjeResponse", 100, 0., 2.);
   
        TH1D *pTjetResponse_nom(0);
   pTjetResponse_nom = new TH1D("pTjetRatio_nom", "pTjeResponse", 100, 0., 2.);

        TH1D *pTjetResponse_JERup(0);
   pTjetResponse_JERup = new TH1D("pTjetRatio_JERup", "pTjeResponse", 100, 0., 2.);

        TH1D *pTjetResponse_JERdown(0);
   pTjetResponse_JERdown = new TH1D("pTjetRatio_JERdown", "pTjeResponse", 100, 0., 2.);

        TH2D *pTjetRatio_vs_pThat(0);
   pTjetRatio_vs_pThat = new TH2D("pTjetRatio_vs_pThat", "pTjet_reco", 100, 75, 200., 100, 0., 10.);

        TH2D *weight_vs_pTjet(0);
   weight_vs_pTjet = new TH2D("weight_vs_pTjet", "pTjet_reco", 100, 75, 200., 100, -7, 1.);

        TH1D *stabilityNumerator(0);
   stabilityNumerator = new TH1D("stabilityNum", "stabilityNum", nBins, ybins_true);

        TH1D *pTjet_reco(0);
   pTjet_reco = new TH1D("pTjet_reco", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_recoJECup(0);
   pTjet_recoJECup = new TH1D("pTjet_recoJECup", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_recoJECdown(0);
   pTjet_recoJECdown = new TH1D("pTjet_recoJECdown", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_recoJERup(0);
   pTjet_recoJERup = new TH1D("pTjet_recoJERup", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_recoJERdown(0);
   pTjet_recoJERdown = new TH1D("pTjet_recoJERdown", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_HFreco(0);
   pTjet_HFreco = new TH1D("pTjet_HFreco", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_HFrecoJECup(0);
   pTjet_HFrecoJECup = new TH1D("pTjet_HFrecoJECup", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_HFrecoJECdown(0);
   pTjet_HFrecoJECdown = new TH1D("pTjet_HFrecoJECdown", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_HFrecoJERup(0);
   pTjet_HFrecoJERup = new TH1D("pTjet_HFrecoJERup", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_HFrecoJERdown(0);
   pTjet_HFrecoJERdown = new TH1D("pTjet_HFrecoJERdown", "pTjet_reco", nBins, ybins_true);

        TH1D *fake(0);
   fake = new TH1D("fake", "pTjet_reco", nBins, ybins_true);

        TH1D *miss(0);
   miss = new TH1D("miss", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_recoTest(0);
   pTjet_recoTest = new TH1D("pTjet_recoTest", "pTjet_recoTest", nBins, ybins_true);

        TH1D *pTjet_recoTraining(0);
   pTjet_recoTraining = new TH1D("pTjet_recoTraining", "pTjet_recoTraining", nBins, ybins_true);

        TH1D *pTjet_matchedReco(0);
   pTjet_matchedReco = new TH1D("pTjet_matchedReco", "pTjet_matchedReco", nBins, ybins_true);

        TH1D *pTjet_matchedTruth(0);
   pTjet_matchedTruth = new TH1D("pTjet_matchedTruth", "pTjet_matchedTruth", nBins, ybins_true);

        TH1D *pTjet_matchedRecoTest(0);
   pTjet_matchedRecoTest = new TH1D("pTjet_matchedRecoTest", "pTjet_matchedRecoTest", nBins, ybins_true);

        TH1D *pTjet_matchedRecoTraining(0);
   pTjet_matchedRecoTraining = new TH1D("pTjet_matchedRecoTraining", "pTjet_matchedRecoTraining", nBins, ybins_true);

        TH1D *pTjet_true(0);
   pTjet_true = new TH1D("pTjet_true", "pTjet_true", nBins, ybins_true);

        TH1D *pTjet_trueTest(0);
   pTjet_trueTest = new TH1D("pTjet_trueTest", "pTjet_trueTest", nBins, ybins_true);

        TH1D *pTjet_trueTraining(0);
   pTjet_trueTraining = new TH1D("pTjet_trueTraining", "pTjet_trueTraining", nBins, ybins_true);


        TH1D *purity_recoJECup(0);
   purity_recoJECup = new TH1D("purity_recoJECup", "pTjet_reco", nBins, ybins_true);

        TH1D *purity_recoJECdown(0);
   purity_recoJECdown = new TH1D("purity_recoJECdown", "pTjet_reco", nBins, ybins_true);

        TH1D *purity_recoJERup(0);
   purity_recoJERup = new TH1D("purity_recoJERup", "pTjet_reco", nBins, ybins_true);

        TH1D *purity_recoJERdown(0);
   purity_recoJERdown = new TH1D("purity_recoJERdown", "pTjet_reco", nBins, ybins_true); 

        TH1D *eff_recoJECup(0);
   eff_recoJECup = new TH1D("eff_recoJECup", "pTjet_reco", nBins, ybins_true);

        TH1D *eff_recoJECdown(0);
   eff_recoJECdown = new TH1D("eff_recoJECdown", "pTjet_reco", nBins, ybins_true);

        TH1D *eff_recoJERup(0);
   eff_recoJERup = new TH1D("eff_recoJERup", "pTjet_reco", nBins, ybins_true);

        TH1D *eff_recoJERdown(0);
   eff_recoJERdown = new TH1D("eff_recoJERdown", "pTjet_reco", nBins, ybins_true);


    RooUnfoldResponse pT_response(pTjet_reco, pTjet_true);
    RooUnfoldResponse pT_responseJECup(pTjet_recoJECup, pTjet_true);
    RooUnfoldResponse pT_responseJECdown(pTjet_recoJECdown, pTjet_true);
    RooUnfoldResponse pT_responseJERup(pTjet_recoJERup, pTjet_true);
    RooUnfoldResponse pT_responseJERdown(pTjet_recoJERdown, pTjet_true); 

    RooUnfoldResponse pT_HFresponse(pTjet_HFreco, pTjet_true);
    RooUnfoldResponse pT_HFresponseJECup(pTjet_HFrecoJECup, pTjet_true);
    RooUnfoldResponse pT_HFresponseJECdown(pTjet_HFrecoJECdown, pTjet_true);
    RooUnfoldResponse pT_HFresponseJERup(pTjet_HFrecoJERup, pTjet_true);
    RooUnfoldResponse pT_HFresponseJERdown(pTjet_HFrecoJERdown, pTjet_true);

    RooUnfoldResponse pT_responseSplit(pTjet_recoTest, pTjet_trueTest);

      TFile *input2=TFile::Open("/data00/g2ccbar/mc2018/skimTest/mergedfile_02132025.root");

      TTree *T = (TTree*) input2->Get("Tree");
      vector<float>   *JetPT;
      TBranch        *b_JetPT; 
      vector<float>   *JetEta;
      TBranch        *b_JetEta; 
      vector<float>   *genJetPT;
      TBranch        *b_genJetPT;
      vector<int>     *MJTNcHad;
      vector<int>     *MJTNbHad;
      TBranch        *b_MJTNcHad;   //!
      TBranch        *b_MJTNbHad;   //!
      float NCollWeight;
      TBranch *b_NCollWeight;
      float EventWeight;
      TBranch *b_EventWeight;
      float PTHat;
      TBranch *b_PTHat;

      // Set object pointer

      JetPT = 0;
      JetEta = 0;
      genJetPT = 0;
      MJTNcHad = 0;
      MJTNbHad = 0;

      if (!T) return;
      T->SetMakeClass(1);

 
      Int_t nEvmc=T->GetEntries();
      T->SetBranchAddress("JetPT", &JetPT, &b_JetPT);
      T->SetBranchAddress("PTHat", &PTHat, &b_PTHat);
      T->SetBranchAddress("JetEta", &JetEta, &b_JetEta);
      T->SetBranchAddress("genJetPT", &genJetPT, &b_genJetPT);
      T->SetBranchAddress("NCollWeight", &NCollWeight, &b_NCollWeight);
      T->SetBranchAddress("EventWeight", &EventWeight, &b_EventWeight);
      T->SetBranchAddress("MJTNcHad", &MJTNcHad, &b_MJTNcHad);
      T->SetBranchAddress("MJTNbHad", &MJTNbHad, &b_MJTNbHad);
for(int iEntry=0; iEntry< nEvmc; iEntry++)
{
    T->GetEntry(iEntry);
    if (JetPT->size() == 0) continue;
    double weight = EventWeight*NCollWeight;
    weight = EventWeight*NCollWeight;
    for (int j = 0; j < JetPT->size(); j++)
    {
//    if (JetPT->at(j) < pTmin ) continue;
//    if (JetPT->at(0)/PTHat > 5) cout << PTHat << endl;
//    if (JetPT->at(0)/PTHat > 2.5 ) continue;
//    double weightMax = -2.34231+1.9-0.0145288*JetPT->at(0);
//    if ( log10(EventWeight) > weightMax+0.5 && JetPT->at(0) < 90 && JetPT->at(0) > 75 ) continue;
//    if ( log10(EventWeight) > weightMax && JetPT->at(0) > 90) continue;
    if (JetPT->at(0) > 75){pTjetRatio_vs_pThat->Fill(JetPT->at(0), JetPT->at(0)/PTHat ); weight_vs_pTjet->Fill(JetPT->at(0), log10(EventWeight));}
    if (JetPT->at(0)/PTHat > 5. ) continue;

    jecUnc->setJetEta(JetEta->at(j));
    jecUnc->setJetPt(JetPT->at(j));
    double uncUp = 1+fabs(jecUnc->getUncertainty(1));
    jecUnc->setJetEta(JetEta->at(j));
    jecUnc->setJetPt(JetPT->at(j));
    double uncDown = 1-fabs(jecUnc->getUncertainty(-1));

  
    if (JetPT->at(j) > 200 && fabs(JetEta->at(j) ) < 2. ) cout << uncUp << " " << uncDown << endl;

    JME::JetParameters parameters_1;
    parameters_1.setJetPt(JetPT->at(j));
    parameters_1.setJetEta(JetEta->at(j));
    parameters_1.setRho(100);

    float sfUp = resolution_sf.getScaleFactor(parameters_1, Variation::UP);
    float sfNom = resolution_sf.getScaleFactor(parameters_1);
    float sfDown = resolution_sf.getScaleFactor(parameters_1, Variation::DOWN);

    if (genJetPT->at(j) > 0)
    {
     sfUp = 1+gRandom->Gaus(0,std::sqrt(0.06*0.06+0.8*0.8/(genJetPT->at(j) ) ) )*std::sqrt(sfUp*sfUp-1);
     sfNom = 1+gRandom->Gaus(0,std::sqrt(0.06*0.06+0.8*0.8/(genJetPT->at(j) ) ) )*std::sqrt(sfNom*sfNom-1);
     sfDown = 1+gRandom->Gaus(0,std::sqrt(0.06*0.06+0.8*0.8/(genJetPT->at(j) ) ) )*std::sqrt(sfDown*sfDown-1);
    }

   if (genJetPT->at(j) > 100)
   {
   pTjetResponse->Fill(JetPT->at(j)/genJetPT->at(j), weight);
   pTjetResponse_nom->Fill(JetPT->at(j)/genJetPT->at(j)*sfNom, weight);
   pTjetResponse_JERup->Fill(JetPT->at(j)/genJetPT->at(j)*sfUp, weight);
   pTjetResponse_JERdown->Fill(JetPT->at(j)/genJetPT->at(j)*sfDown, weight);
   }
// JECup
    pTjet_recoJECup->Fill(JetPT->at(j)*uncUp*sfNom,weight);
    pTjet_recoJECdown->Fill(JetPT->at(j)*uncDown*sfNom,weight);
    pTjet_recoJERup->Fill(JetPT->at(j)*sfUp,weight);
    pTjet_recoJERdown->Fill(JetPT->at(j)*sfDown,weight);
    pTjet_true->Fill(genJetPT->at(j),weight);

    if ( (JetPT->at(j)*sfNom*uncUp > pTmin && genJetPT->at(j) > pTmin ) && (JetPT->at(j)*sfNom*uncUp < pTmax && genJetPT->at(j) < pTmax)) 
    {
    pT_responseJECup.Fill(JetPT->at(j)*uncUp*sfNom, genJetPT->at(j),weight);
    if ( MJTNcHad->at(j) > 0 || MJTNbHad->at(j) > 0 )  pT_HFresponseJECup.Fill(JetPT->at(j)*uncUp*sfNom, genJetPT->at(j),weight);
    purity_recoJECup->Fill(JetPT->at(j)*uncUp*sfNom,weight);
    eff_recoJECup->Fill(genJetPT->at(j), weight);
    }
// JECdown


    if ( (JetPT->at(j)*sfNom*uncDown > pTmin && genJetPT->at(j) > pTmin ) && (JetPT->at(j)*sfNom*uncDown < pTmax && genJetPT->at(j) < pTmax)) 
    {
    pT_responseJECdown.Fill(JetPT->at(j)*uncDown*sfNom, genJetPT->at(j),weight);
    if ( MJTNcHad->at(j) > 0 || MJTNbHad->at(j) > 0 )  pT_HFresponseJECdown.Fill(JetPT->at(j)*uncDown*sfNom, genJetPT->at(j),weight);

    purity_recoJECdown->Fill(JetPT->at(j)*uncDown*sfNom,weight);
    eff_recoJECdown->Fill(genJetPT->at(j),weight);


    }
// JERdown

    if ( (JetPT->at(j)*sfDown > pTmin && genJetPT->at(j) > pTmin ) && (JetPT->at(j)*sfDown < pTmax && genJetPT->at(j) < pTmax)) 
    {
    pT_responseJERdown.Fill(JetPT->at(j)*sfDown, genJetPT->at(j),weight);
    if ( MJTNcHad->at(j) > 0 || MJTNbHad->at(j) > 0 )  pT_HFresponseJERdown.Fill(JetPT->at(j)*sfDown, genJetPT->at(j),weight); 

    purity_recoJERdown->Fill(JetPT->at(j)*sfDown,weight);
    eff_recoJERdown->Fill(genJetPT->at(j),weight);
    }

// JERup
    if ( (JetPT->at(j)*sfUp > pTmin && genJetPT->at(j) > pTmin ) && (JetPT->at(j)*sfUp < pTmax && genJetPT->at(j) < pTmax)) 
    {
    pT_responseJERup.Fill(JetPT->at(j)*sfUp, genJetPT->at(j),weight);
    purity_recoJERup->Fill(JetPT->at(j)*sfUp,weight);
    eff_recoJERup->Fill(genJetPT->at(j),weight);

    if ( MJTNcHad->at(j) > 0 || MJTNbHad->at(j) > 0 )  pT_HFresponseJERup.Fill(JetPT->at(j)*sfUp, genJetPT->at(j),weight); 
    }

    
    
/////////
    double split = rand->Rndm();
    bool splitPass = split < 0.01;
    bool splitDoesNotPass = split > 0.7;

    if (splitPass){ 
        pTjet_recoTest->Fill(JetPT->at(j), weight);
        pTjet_trueTest->Fill(genJetPT->at(j),weight);
    } else{ 
        pTjet_recoTraining->Fill(JetPT->at(j)*sfNom,weight); 
        pTjet_trueTraining->Fill(genJetPT->at(j),weight);
    }

/*    if (!(genJetPT->at(j) > pTmin && genJetPT->at(j) < pTmax) && 
        (JetPT->at(j) > pTmin && JetPT->at(j) < pTmax) && !(std::min(JetPT->at(j), genJetPT->at(j) )/std::max(JetPT->at(j), genJetPT->at(j)) > 0.1) ) 
    {
        pT_response.Fake(JetPT->at(j),weight);
        if ( MJTNcHad->at(j) > 0 || MJTNbHad->at(j) > 0 ) pT_HFresponse.Fake(JetPT->at(j),weight);
//        if (!splitPass){ pT_responseSplit.Fake(JetPT->at(j),weight); fake->Fill(JetPT->at(j),weight); }
    }
    
    if ((genJetPT->at(j) > pTmin && genJetPT->at(j) < pTmax) && 
        !(JetPT->at(j) > pTmin && JetPT->at(j) < pTmax) && !(std::min(JetPT->at(j), genJetPT->at(j) )/std::max(JetPT->at(j), genJetPT->at(j)) > 0.1 ) )
    {
        pT_response.Miss(genJetPT->at(j),weight);
        if ( MJTNcHad->at(j) > 0 || MJTNbHad->at(j) > 0 ) pT_HFresponse.Miss(genJetPT->at(j),weight);
//        if (!splitPass){ pT_responseSplit.Miss( genJetPT->at(j),weight); miss->Fill(genJetPT->at(j), weight); }
    } */

    if ( (JetPT->at(j)*sfNom > pTmin && genJetPT->at(j) > pTmin ) && (JetPT->at(j)*sfNom < pTmax && genJetPT->at(j) < pTmax) /*&& (std::min(JetPT->at(j), genJetPT->at(j) )/std::max(JetPT->at(j), genJetPT->at(j)) > 0.3 )*/  ) 
    {
    if (pTjet_matchedReco->FindBin(JetPT->at(j)) == pTjet_matchedReco->FindBin(genJetPT->at(j))){
        stabilityNumerator->Fill(genJetPT->at(j),weight);
        purityNumerator->Fill(JetPT->at(j),weight);
    }

    if (splitPass){ 
        pTjet_matchedRecoTest->Fill(JetPT->at(j)*sfNom,weight/NCollWeight);
    } else { 
        pT_responseSplit.Fill(JetPT->at(j)*sfNom, genJetPT->at(j),weight); 
        pTjet_matchedRecoTraining->Fill(JetPT->at(j)*sfNom,weight);
	fake->Fill(JetPT->at(j)*sfNom,weight);
	miss->Fill(genJetPT->at(j),weight);
    }
   
//    pTjet_reco->Fill(JetPT->at(j),weight);
//    pTjet_true->Fill(genJetPT->at(j),weight);
   } 
}


/*    T->GetEntry(iEntry);
    if (JetPT->size() == 0) continue;
    if (JetPT->at(0) < pTmin ) continue;
      jecUnc->setJetEta(JetEta->at(0));
      jecUnc->setJetPt(JetPT->at(0));
      double unc = jecUnc->getUncertainty(true);
//      cout << unc << endl;
      JME::JetParameters parameters_1;
      parameters_1.setJetPt(JetPT->at(0));
      parameters_1.setJetEta(JetEta->at(0));


    float sfDown = resolution_sf.getScaleFactor(parameters_1, Variation::DOWN);
//     cout << sfDown << endl;
//     if (genJetPT->at(0) < 0) cout << genJetPT->at(0) << " " << JetPT->at(0) << endl;
//     cout << 1+gRandom->Gaus(0,std::sqrt(0.6*0.6+0.8*0.8/(genJetPT->at(0)*genJetPT->at(0)) ) )*std::sqrt(sfDown*sfDown-1) << endl;
    pTjet_reco->Fill(JetPT->at(0));
    if (genJetPT->size() > 0) pTjet_true->Fill(genJetPT->at(0));
    double split = rand->Rndm();
    bool splitPass = split < 0.50;
    bool splitDoesNotPass = split > 0.50;
    if (splitPass == true){pTjet_recoTest->Fill(JetPT->at(0));  pTjet_trueTest->Fill(genJetPT->at(0));}
    if (splitPass == false){ pTjet_recoTraining->Fill( JetPT->at(0) ); pTjet_trueTraining->Fill(genJetPT->at(0));}

    if ( !(genJetPT->at(0) > pTmin && genJetPT->at(0) < pTmax ) && ( JetPT->at(0) > pTmin && JetPT->at(0) < pTmax  )){
       pT_response.Fake(JetPT->at(0));
       if (splitPass == false) pT_responseSplit.Fake(JetPT->at(0));
    }
    if ((genJetPT->at(0) > pTmin && genJetPT->at(0) < pTmax ) && !( JetPT->at(0) > pTmin && JetPT->at(0) < pTmax  ))
    {
      pT_response.Miss(genJetPT->at(0));
      if (splitPass == true) pT_responseSplit.Miss(genJetPT->at(0));
    } 

    if (!(JetPT->at(0) > pTmin && genJetPT->at(0) > pTmin )) continue;
    if (!(JetPT->at(0) < pTmax && genJetPT->at(0) < pTmax)) continue;
    pTjet_matchedReco->Fill(JetPT->at(0));
    pTjet_matchedTruth->Fill(genJetPT->at(0));
    pT_response.Fill(JetPT->at(0), genJetPT->at(0));
    if (pTjet_matchedReco->FindBin(JetPT->at(0)) == pTjet_matchedReco->FindBin(genJetPT->at(0))){
	stabilityNumerator->Fill(genJetPT->at(0));
	purityNumerator->Fill(JetPT->at(0));
	}
    if (splitPass == true){pTjet_matchedRecoTest->Fill(JetPT->at(0));}
    if (splitPass == false){pT_responseSplit.Fill(JetPT->at(0), genJetPT->at(0)); pTjet_matchedRecoTraining->Fill(JetPT->at(0));}
    pTjet_reco->Fill(JetPT->at(0));
    pTjet_true->Fill(genJetPT->at(0));
   // cout << JetPT->size() << endl;
//    cout << JetPT->at(0)/genJetPT->at(0) << endl;
*/   }
       TFile *fout=new TFile("/home/cbaldene/Unfolding/RooUnfoldSetupForLuis/examples/response3.root","RECREATE");
//       cout << pT_response.GetBinContent(pTjet_reco,4) << endl;
       auto TMatrix_pT = pT_response.Mresponse();
       TMatrix_pT.Write();
       pT_response.Print();
       pT_response.Write();
       pT_responseJECup.Write();
       pT_responseJECdown.Write();
       pT_responseJERup.Write();
       pT_responseJERdown.Write();
       pT_HFresponse.Write();
       pT_HFresponseJECup.Write();
       pT_HFresponseJECdown.Write();
       pT_HFresponseJERup.Write();
       pT_HFresponseJERdown.Write();       

    eff_recoJECup->Divide(pTjet_true);
    eff_recoJECdown->Divide(pTjet_true);
    eff_recoJERup->Divide(pTjet_true);
    eff_recoJERdown->Divide(pTjet_true);
//    pTjet_true->Fill(genJetPT->at(j),weight);

    eff_recoJECup->Write();
    eff_recoJECdown->Write();
    eff_recoJERup->Write();
    eff_recoJERdown->Write();



    purity_recoJECup->Divide(pTjet_recoJECup);
    purity_recoJECdown->Divide(pTjet_recoJECdown);
    purity_recoJERup->Divide(pTjet_recoJERup);
    purity_recoJERdown->Divide(pTjet_recoJERdown);
//    pTjet_true->Fill(genJetPT->at(j),weight);

    purity_recoJECup->Write();
    purity_recoJECdown->Write();
    purity_recoJERup->Write();
    purity_recoJERdown->Write();

       pTjet_matchedReco->Write();
       miss->Divide(pTjet_trueTraining);
       fake->Divide(pTjet_recoTraining);
       miss->Write();
       fake->Write();
       pTjet_reco->Write();
       pTjet_true->Write();
       auto TMatrix_pTsplit = pT_responseSplit.Mresponse();
       TMatrix_pTsplit.Write();
       pT_responseSplit.Print();
       pT_responseSplit.Write();
       pTjet_matchedRecoTest->Write();
       pTjet_recoTest->Write();
       pTjet_trueTest->Write();
       pTjet_matchedRecoTraining->Write();
       pTjet_recoTraining->Write();
       pTjet_trueTraining->Write();
       stabilityNumerator->Divide(pTjet_matchedTruth);
       purityNumerator->Divide(pTjet_matchedReco);
       stabilityNumerator->Write();
       purityNumerator->Write();
       pTjetRatio_vs_pThat->Write();
       weight_vs_pTjet->Write();
       pTjetResponse->Write();
       pTjetResponse_nom->Write();
       pTjetResponse_JERup->Write();
       pTjetResponse_JERdown->Write();
       fout->Close();

}

#ifndef __CINT__
int main () { RooUnfoldExample(); return 0; }  // Main program when run stand-alone
#endif
