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


        TH1D *pTjet_recoJECMCup(0);
   pTjet_recoJECMCup = new TH1D("pTjet_recoJECMCup", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_recoJECFragup(0);
   pTjet_recoJECFragup = new TH1D("pTjet_recoJECFragup", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_recoJECFlavorup(0);
   pTjet_recoJECFlavorup = new TH1D("pTjet_recoJECFlavorup", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_recoJECAbsStatup(0);
   pTjet_recoJECAbsStatup = new TH1D("pTjet_recoJECAbsStatup", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_recoJECAbsScaleup(0);
   pTjet_recoJECAbsScaleup = new TH1D("pTjet_recoJECAbsScaleup", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_recoJECAbsMPFup(0);
   pTjet_recoJECAbsMPFup = new TH1D("pTjet_recoJECAbsMPFup", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_recoJECECALup(0);
   pTjet_recoJECECALup = new TH1D("pTjet_recoJECECALup", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_recoJECHCALup(0);
   pTjet_recoJECHCALup = new TH1D("pTjet_recoJECHCALup", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_recoJECRelStatup(0);
   pTjet_recoJECRelStatup = new TH1D("pTjet_recoJECRelStatup", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_recoJECRelPtup(0);
   pTjet_recoJECRelPtup = new TH1D("pTjet_recoJECRelPtup", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_recoJECRelBalup(0);
   pTjet_recoJECRelBalup = new TH1D("pTjet_recoJECRelBalup", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_recoJECRelJERup(0);
   pTjet_recoJECRelJERup = new TH1D("pTjet_recoJECRelJERup", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_recoJECL3Resup(0);
   pTjet_recoJECL3Resup = new TH1D("pTjet_recoJECL3Resup", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_recoJECdown(0);
   pTjet_recoJECdown = new TH1D("pTjet_recoJECdown", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_recoJECMCdown(0);
   pTjet_recoJECMCdown = new TH1D("pTjet_recoJECMCdown", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_recoJECFragdown(0);
   pTjet_recoJECFragdown = new TH1D("pTjet_recoJECFragdown", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_recoJECFlavordown(0);
   pTjet_recoJECFlavordown = new TH1D("pTjet_recoJECFlavordown", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_recoJECAbsStatdown(0);
   pTjet_recoJECAbsStatdown = new TH1D("pTjet_recoJECAbsStatdown", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_recoJECAbsScaledown(0);
   pTjet_recoJECAbsScaledown = new TH1D("pTjet_recoJECAbsScaledown", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_recoJECAbsMPFdown(0);
   pTjet_recoJECAbsMPFdown = new TH1D("pTjet_recoJECAbsMPFdown", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_recoJECECALdown(0);
   pTjet_recoJECECALdown = new TH1D("pTjet_recoJECECALdown", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_recoJECHCALdown(0);
   pTjet_recoJECHCALdown = new TH1D("pTjet_recoJECHCALdown", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_recoJECRelStatdown(0);
   pTjet_recoJECRelStatdown = new TH1D("pTjet_recoJECRelStatdown", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_recoJECRelPtdown(0);
   pTjet_recoJECRelPtdown = new TH1D("pTjet_recoJECRelPtdown", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_recoJECRelBaldown(0);
   pTjet_recoJECRelBaldown = new TH1D("pTjet_recoJECRelBaldown", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_recoJECRelJERdown(0);
   pTjet_recoJECRelJERdown = new TH1D("pTjet_recoJECRelJERdown", "pTjet_reco", nBins, ybins_true);

        TH1D *pTjet_recoJECL3Resdown(0);
   pTjet_recoJECL3Resdown = new TH1D("pTjet_recoJECL3Resdown", "pTjet_reco", nBins, ybins_true);

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

        TH1D *purity_recoJECMCup(0);
   purity_recoJECMCup = new TH1D("purity_recoJECMCup", "pTjet_reco", nBins, ybins_true);

        TH1D *purity_recoJECFragup(0);
   purity_recoJECFragup = new TH1D("purity_recoJECFragup", "pTjet_reco", nBins, ybins_true);

        TH1D *purity_recoJECFlavorup(0);
   purity_recoJECFlavorup = new TH1D("purity_recoJECFlavorup", "pTjet_reco", nBins, ybins_true);

        TH1D *purity_recoJECAbsStatup(0);
   purity_recoJECAbsStatup = new TH1D("purity_recoJECAbsStatup", "pTjet_reco", nBins, ybins_true);

        TH1D *purity_recoJECAbsScaleup(0);
   purity_recoJECAbsScaleup = new TH1D("purity_recoJECAbsScaleup", "pTjet_reco", nBins, ybins_true);

        TH1D *purity_recoJECAbsMPFup(0);
   purity_recoJECAbsMPFup = new TH1D("purity_recoJECAbsMPFup", "pTjet_reco", nBins, ybins_true);

        TH1D *purity_recoJECECALup(0);
   purity_recoJECECALup = new TH1D("purity_recoJECECALup", "pTjet_reco", nBins, ybins_true);

        TH1D *purity_recoJECHCALup(0);
   purity_recoJECHCALup = new TH1D("purity_recoJECHCALup", "pTjet_reco", nBins, ybins_true);

        TH1D *purity_recoJECRelStatup(0);
   purity_recoJECRelStatup = new TH1D("purity_recoJECRelStatup", "pTjet_reco", nBins, ybins_true);

        TH1D *purity_recoJECRelPtup(0);
   purity_recoJECRelPtup = new TH1D("purity_recoJECRelPtup", "pTjet_reco", nBins, ybins_true);

        TH1D *purity_recoJECRelBalup(0);
   purity_recoJECRelBalup = new TH1D("purity_recoJECRelBalup", "pTjet_reco", nBins, ybins_true);

        TH1D *purity_recoJECRelJERup(0);
   purity_recoJECRelJERup = new TH1D("purity_recoJECRelJERup", "pTjet_reco", nBins, ybins_true);

        TH1D *purity_recoJECL3Resup(0);
   purity_recoJECL3Resup = new TH1D("purity_recoJECL3Resup", "pTjet_reco", nBins, ybins_true);

        TH1D *purity_recoJECTotalup(0);
   purity_recoJECTotalup = new TH1D("purity_recoJECTotalup", "pTjet_reco", nBins, ybins_true);


        TH1D *purity_recoJECdown(0);
   purity_recoJECdown = new TH1D("purity_recoJECdown", "pTjet_reco", nBins, ybins_true);

        TH1D *purity_recoJECMCdown(0);
   purity_recoJECMCdown = new TH1D("purity_recoJECMCdown", "pTjet_reco", nBins, ybins_true);

        TH1D *purity_recoJECFragdown(0);
   purity_recoJECFragdown = new TH1D("purity_recoJECFragdown", "pTjet_reco", nBins, ybins_true);

        TH1D *purity_recoJECFlavordown(0);
   purity_recoJECFlavordown = new TH1D("purity_recoJECFlavordown", "pTjet_reco", nBins, ybins_true);

        TH1D *purity_recoJECAbsStatdown(0);
   purity_recoJECAbsStatdown = new TH1D("purity_recoJECAbsStatdown", "pTjet_reco", nBins, ybins_true);

        TH1D *purity_recoJECAbsScaledown(0);
   purity_recoJECAbsScaledown = new TH1D("purity_recoJECAbsScaledown", "pTjet_reco", nBins, ybins_true);

        TH1D *purity_recoJECAbsMPFdown(0);
   purity_recoJECAbsMPFdown = new TH1D("purity_recoJECAbsMPFdown", "pTjet_reco", nBins, ybins_true);

        TH1D *purity_recoJECECALdown(0);
   purity_recoJECECALdown = new TH1D("purity_recoJECECALdown", "pTjet_reco", nBins, ybins_true);

        TH1D *purity_recoJECHCALdown(0);
   purity_recoJECHCALdown = new TH1D("purity_recoJECHCALdown", "pTjet_reco", nBins, ybins_true);

        TH1D *purity_recoJECRelStatdown(0);
   purity_recoJECRelStatdown = new TH1D("purity_recoJECRelStatdown", "pTjet_reco", nBins, ybins_true);

        TH1D *purity_recoJECRelPtdown(0);
   purity_recoJECRelPtdown = new TH1D("purity_recoJECRelPtdown", "pTjet_reco", nBins, ybins_true);

        TH1D *purity_recoJECRelBaldown(0);
   purity_recoJECRelBaldown = new TH1D("purity_recoJECRelBaldown", "pTjet_reco", nBins, ybins_true);

        TH1D *purity_recoJECRelJERdown(0);
   purity_recoJECRelJERdown = new TH1D("purity_recoJECRelJERdown", "pTjet_reco", nBins, ybins_true);

        TH1D *purity_recoJECL3Resdown(0);
   purity_recoJECL3Resdown = new TH1D("purity_recoJECL3Resdown", "pTjet_reco", nBins, ybins_true);

        TH1D *purity_recoJECTotaldown(0);
   purity_recoJECTotaldown = new TH1D("purity_recoJECTotaldown", "pTjet_reco", nBins, ybins_true);

        TH1D *purity_recoJERup(0);
   purity_recoJERup = new TH1D("purity_recoJERup", "pTjet_reco", nBins, ybins_true);

        TH1D *purity_recoJERdown(0);
   purity_recoJERdown = new TH1D("purity_recoJERdown", "pTjet_reco", nBins, ybins_true); 

        TH1D *eff_recoJECup(0);
   eff_recoJECup = new TH1D("eff_recoJECup", "pTjet_reco", nBins, ybins_true);

        TH1D *eff_recoJECMCup(0);
   eff_recoJECMCup = new TH1D("eff_recoJECMCup", "pTjet_reco", nBins, ybins_true);

        TH1D *eff_recoJECFragup(0);
   eff_recoJECFragup = new TH1D("eff_recoJECFragup", "pTjet_reco", nBins, ybins_true);

        TH1D *eff_recoJECFlavorup(0);
   eff_recoJECFlavorup = new TH1D("eff_recoJECFlavorup", "pTjet_reco", nBins, ybins_true);

        TH1D *eff_recoJECAbsStatup(0);
   eff_recoJECAbsStatup = new TH1D("eff_recoJECAbsStatup", "pTjet_reco", nBins, ybins_true);

        TH1D *eff_recoJECAbsScaleup(0);
   eff_recoJECAbsScaleup = new TH1D("eff_recoJECAbsScaleup", "pTjet_reco", nBins, ybins_true);

        TH1D *eff_recoJECAbsMPFup(0);
   eff_recoJECAbsMPFup = new TH1D("eff_recoJECAbsMPFup", "pTjet_reco", nBins, ybins_true);

        TH1D *eff_recoJECECALup(0);
   eff_recoJECECALup = new TH1D("eff_recoJECECALup", "pTjet_reco", nBins, ybins_true);

        TH1D *eff_recoJECHCALup(0);
   eff_recoJECHCALup = new TH1D("eff_recoJECHCALup", "pTjet_reco", nBins, ybins_true);

        TH1D *eff_recoJECRelStatup(0);
   eff_recoJECRelStatup = new TH1D("eff_recoJECRelStatup", "pTjet_reco", nBins, ybins_true);

        TH1D *eff_recoJECRelPtup(0);
   eff_recoJECRelPtup = new TH1D("eff_recoJECRelPtup", "pTjet_reco", nBins, ybins_true);

        TH1D *eff_recoJECRelBalup(0);
   eff_recoJECRelBalup = new TH1D("eff_recoJECRelBalup", "pTjet_reco", nBins, ybins_true);

        TH1D *eff_recoJECRelJERup(0);
   eff_recoJECRelJERup = new TH1D("eff_recoJECRelJERup", "pTjet_reco", nBins, ybins_true);

        TH1D *eff_recoJECL3Resup(0);
   eff_recoJECL3Resup = new TH1D("eff_recoJECL3Resup", "pTjet_reco", nBins, ybins_true);

        TH1D *eff_recoJECTotalup(0);
   eff_recoJECTotalup = new TH1D("eff_recoJECTotalup", "pTjet_reco", nBins, ybins_true);


        TH1D *eff_recoJECdown(0);
   eff_recoJECdown = new TH1D("eff_recoJECdown", "pTjet_reco", nBins, ybins_true);

        TH1D *eff_recoJECMCdown(0);
   eff_recoJECMCdown = new TH1D("eff_recoJECMCdown", "pTjet_reco", nBins, ybins_true);

        TH1D *eff_recoJECFragdown(0);
   eff_recoJECFragdown = new TH1D("eff_recoJECFragdown", "pTjet_reco", nBins, ybins_true);

        TH1D *eff_recoJECFlavordown(0);
   eff_recoJECFlavordown = new TH1D("eff_recoJECFlavordown", "pTjet_reco", nBins, ybins_true);

        TH1D *eff_recoJECAbsStatdown(0);
   eff_recoJECAbsStatdown = new TH1D("eff_recoJECAbsStatdown", "pTjet_reco", nBins, ybins_true);

        TH1D *eff_recoJECAbsScaledown(0);
   eff_recoJECAbsScaledown = new TH1D("eff_recoJECAbsScaledown", "pTjet_reco", nBins, ybins_true);

        TH1D *eff_recoJECAbsMPFdown(0);
   eff_recoJECAbsMPFdown = new TH1D("eff_recoJECAbsMPFdown", "pTjet_reco", nBins, ybins_true);

        TH1D *eff_recoJECECALdown(0);
   eff_recoJECECALdown = new TH1D("eff_recoJECECALdown", "pTjet_reco", nBins, ybins_true);

        TH1D *eff_recoJECHCALdown(0);
   eff_recoJECHCALdown = new TH1D("eff_recoJECHCALdown", "pTjet_reco", nBins, ybins_true);

        TH1D *eff_recoJECRelStatdown(0);
   eff_recoJECRelStatdown = new TH1D("eff_recoJECRelStatdown", "pTjet_reco", nBins, ybins_true);

        TH1D *eff_recoJECRelPtdown(0);
   eff_recoJECRelPtdown = new TH1D("eff_recoJECRelPtdown", "pTjet_reco", nBins, ybins_true);

        TH1D *eff_recoJECRelBaldown(0);
   eff_recoJECRelBaldown = new TH1D("eff_recoJECRelBaldown", "pTjet_reco", nBins, ybins_true);

        TH1D *eff_recoJECRelJERdown(0);
   eff_recoJECRelJERdown = new TH1D("eff_recoJECRelJERdown", "pTjet_reco", nBins, ybins_true);

        TH1D *eff_recoJECL3Resdown(0);
   eff_recoJECL3Resdown = new TH1D("eff_recoJECL3Resdown", "pTjet_reco", nBins, ybins_true);

        TH1D *eff_recoJECTotaldown(0);
   eff_recoJECTotaldown = new TH1D("eff_recoJECTotaldown", "pTjet_reco", nBins, ybins_true);


        TH1D *eff_recoJERup(0);
   eff_recoJERup = new TH1D("eff_recoJERup", "pTjet_reco", nBins, ybins_true);

        TH1D *eff_recoJERdown(0);
   eff_recoJERdown = new TH1D("eff_recoJERdown", "pTjet_reco", nBins, ybins_true);


    RooUnfoldResponse pT_response(pTjet_reco, pTjet_true);
    RooUnfoldResponse pT_responseJECup(pTjet_recoJECup, pTjet_true);


    RooUnfoldResponse pT_responseJECdown(pTjet_recoJECdown, pTjet_true);
    RooUnfoldResponse pT_responseJERup(pTjet_recoJERup, pTjet_true);
    RooUnfoldResponse pT_responseJERdown(pTjet_recoJERdown, pTjet_true); 
    RooUnfoldResponse pT_responseJECMCup(pTjet_recoJECMCup, pTjet_true);
    RooUnfoldResponse pT_responseJECFragup(pTjet_recoJECFragup, pTjet_true);
    RooUnfoldResponse pT_responseJECFlavorup(pTjet_recoJECFlavorup, pTjet_true);
    RooUnfoldResponse pT_responseJECAbsStatup(pTjet_recoJECAbsStatup, pTjet_true);
    RooUnfoldResponse pT_responseJECAbsScaleup(pTjet_recoJECAbsScaleup, pTjet_true);
    RooUnfoldResponse pT_responseJECAbsMPFup(pTjet_recoJECAbsMPFup, pTjet_true);
    RooUnfoldResponse pT_responseJECECALup(pTjet_recoJECECALup, pTjet_true);
    RooUnfoldResponse pT_responseJECHCALup(pTjet_recoJECHCALup, pTjet_true);
    RooUnfoldResponse pT_responseJECRelStatup(pTjet_recoJECRelStatup, pTjet_true);
    RooUnfoldResponse pT_responseJECRelPtup(pTjet_recoJECRelPtup, pTjet_true);
    RooUnfoldResponse pT_responseJECRelBalup(pTjet_recoJECRelBalup, pTjet_true);
    RooUnfoldResponse pT_responseJECRelJERup(pTjet_recoJECRelJERup, pTjet_true);
    RooUnfoldResponse pT_responseJECL3Resup(pTjet_recoJECL3Resup, pTjet_true);
    RooUnfoldResponse pT_responseJECMCdown(pTjet_recoJECMCdown, pTjet_true);
    RooUnfoldResponse pT_responseJECFragdown(pTjet_recoJECFragdown, pTjet_true);
    RooUnfoldResponse pT_responseJECFlavordown(pTjet_recoJECFlavordown, pTjet_true);
    RooUnfoldResponse pT_responseJECAbsStatdown(pTjet_recoJECAbsStatdown, pTjet_true);
    RooUnfoldResponse pT_responseJECAbsScaledown(pTjet_recoJECAbsScaledown, pTjet_true);
    RooUnfoldResponse pT_responseJECAbsMPFdown(pTjet_recoJECAbsMPFdown, pTjet_true);
    RooUnfoldResponse pT_responseJECECALdown(pTjet_recoJECECALdown, pTjet_true);
    RooUnfoldResponse pT_responseJECHCALdown(pTjet_recoJECHCALdown, pTjet_true);
    RooUnfoldResponse pT_responseJECRelStatdown(pTjet_recoJECRelStatdown, pTjet_true);
    RooUnfoldResponse pT_responseJECRelPtdown(pTjet_recoJECRelPtdown, pTjet_true);
    RooUnfoldResponse pT_responseJECRelBaldown(pTjet_recoJECRelBaldown, pTjet_true);
    RooUnfoldResponse pT_responseJECRelJERdown(pTjet_recoJECRelJERdown, pTjet_true);
    RooUnfoldResponse pT_responseJECL3Resdown(pTjet_recoJECL3Resdown, pTjet_true);

 
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
{   if (iEntry%10000 == true) cout << iEntry << endl;
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
    vector<double> vuncUp;
    vector<double> vuncDown;

    for (int isrc = 0; isrc < nsrc; isrc++)
    {
      JetCorrectionUncertainty *uncTemp = vsrc[isrc];
      uncTemp->setJetPt( JetPT->at(j));
      uncTemp->setJetEta(JetEta->at(j) );
      vuncUp.push_back(1+fabs(uncTemp->getUncertainty(true)));
      uncTemp->setJetPt( JetPT->at(j));
      uncTemp->setJetEta(JetEta->at(j) );
      vuncDown.push_back(1-fabs(uncTemp->getUncertainty(true)));
//      if (JetPT->at(j) > 500 && fabs(JetEta->at(j)) < 1.5 ) cout << srcnames[isrc] << " " << 100.*vuncUp.at(isrc) << endl;
    } // for isrc
    
  
//    if (JetPT->at(j) > 200 && fabs(JetEta->at(j) ) < 2. ) cout << uncUp << " " << uncDown << endl;

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
    pTjet_recoJECMCup->Fill(JetPT->at(j) * vuncUp.at(0) * sfNom, weight);
    pTjet_recoJECFragup->Fill(JetPT->at(j) * vuncUp.at(1) * sfNom, weight);
    pTjet_recoJECFlavorup->Fill(JetPT->at(j) * vuncUp.at(2) * sfNom, weight);
    pTjet_recoJECAbsStatup->Fill(JetPT->at(j) * vuncUp.at(3) * sfNom, weight);
    pTjet_recoJECAbsScaleup->Fill(JetPT->at(j) * vuncUp.at(4) * sfNom, weight);
    pTjet_recoJECAbsMPFup->Fill(JetPT->at(j) * vuncUp.at(5) * sfNom, weight);
    pTjet_recoJECECALup->Fill(JetPT->at(j) * vuncUp.at(6) * sfNom, weight);
    pTjet_recoJECHCALup->Fill(JetPT->at(j) * vuncUp.at(7) * sfNom, weight);
    pTjet_recoJECRelStatup->Fill(JetPT->at(j) * vuncUp.at(8) * sfNom, weight);
    pTjet_recoJECRelPtup->Fill(JetPT->at(j) * vuncUp.at(9) * sfNom, weight);
    pTjet_recoJECRelBalup->Fill(JetPT->at(j) * vuncUp.at(10) * sfNom, weight);
    pTjet_recoJECRelJERup->Fill(JetPT->at(j) * vuncUp.at(11) * sfNom, weight);
    pTjet_recoJECL3Resup->Fill(JetPT->at(j) * vuncUp.at(12) * sfNom, weight);


//JECdown
    pTjet_recoJECdown->Fill(JetPT->at(j)*uncDown*sfNom,weight);
    pTjet_recoJECMCdown->Fill(JetPT->at(j) * vuncDown.at(0) * sfNom, weight);
    pTjet_recoJECFragdown->Fill(JetPT->at(j) * vuncDown.at(1) * sfNom, weight);
    pTjet_recoJECFlavordown->Fill(JetPT->at(j) * vuncDown.at(2) * sfNom, weight);
    pTjet_recoJECAbsStatdown->Fill(JetPT->at(j) * vuncDown.at(3) * sfNom, weight);
    pTjet_recoJECAbsScaledown->Fill(JetPT->at(j) * vuncDown.at(4) * sfNom, weight);
    pTjet_recoJECAbsMPFdown->Fill(JetPT->at(j) * vuncDown.at(5) * sfNom, weight);
    pTjet_recoJECECALdown->Fill(JetPT->at(j) * vuncDown.at(6) * sfNom, weight);
    pTjet_recoJECHCALdown->Fill(JetPT->at(j) * vuncDown.at(7) * sfNom, weight);
    pTjet_recoJECRelStatdown->Fill(JetPT->at(j) * vuncDown.at(8) * sfNom, weight);
    pTjet_recoJECRelPtdown->Fill(JetPT->at(j) * vuncDown.at(9) * sfNom, weight);
    pTjet_recoJECRelBaldown->Fill(JetPT->at(j) * vuncDown.at(10) * sfNom, weight);
    pTjet_recoJECRelJERdown->Fill(JetPT->at(j) * vuncDown.at(11) * sfNom, weight);
    pTjet_recoJECL3Resdown->Fill(JetPT->at(j) * vuncDown.at(12) * sfNom, weight);


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

if ((JetPT->at(j) * sfNom * vuncUp.at(0) > pTmin && genJetPT->at(j) > pTmin) && 
    (JetPT->at(j) * sfNom * vuncUp.at(0) < pTmax && genJetPT->at(j) < pTmax)) 
{
    pT_responseJECMCup.Fill(JetPT->at(j) * vuncUp.at(0) * sfNom, genJetPT->at(j), weight);
    purity_recoJECMCup->Fill(JetPT->at(j) * vuncUp.at(0) * sfNom, weight);
    eff_recoJECMCup->Fill(genJetPT->at(j), weight);
}

if ((JetPT->at(j) * sfNom * vuncUp.at(1) > pTmin && genJetPT->at(j) > pTmin) && 
    (JetPT->at(j) * sfNom * vuncUp.at(1) < pTmax && genJetPT->at(j) < pTmax)) 
{
    pT_responseJECFragup.Fill(JetPT->at(j) * vuncUp.at(1) * sfNom, genJetPT->at(j), weight);
    purity_recoJECFragup->Fill(JetPT->at(j) * vuncUp.at(1) * sfNom, weight);
    eff_recoJECFragup->Fill(genJetPT->at(j), weight);
}

if ((JetPT->at(j) * sfNom * vuncUp.at(2) > pTmin && genJetPT->at(j) > pTmin) && 
    (JetPT->at(j) * sfNom * vuncUp.at(2) < pTmax && genJetPT->at(j) < pTmax)) 
{
    pT_responseJECFlavorup.Fill(JetPT->at(j) * vuncUp.at(2) * sfNom, genJetPT->at(j), weight);
    purity_recoJECFlavorup->Fill(JetPT->at(j) * vuncUp.at(2) * sfNom, weight);
    eff_recoJECFlavorup->Fill(genJetPT->at(j), weight);
}

if ((JetPT->at(j) * sfNom * vuncUp.at(3) > pTmin && genJetPT->at(j) > pTmin) && 
    (JetPT->at(j) * sfNom * vuncUp.at(3) < pTmax && genJetPT->at(j) < pTmax)) 
{
    pT_responseJECAbsStatup.Fill(JetPT->at(j) * vuncUp.at(3) * sfNom, genJetPT->at(j), weight);
    purity_recoJECAbsStatup->Fill(JetPT->at(j) * vuncUp.at(3) * sfNom, weight);
    eff_recoJECAbsStatup->Fill(genJetPT->at(j), weight);
}

if ((JetPT->at(j) * sfNom * vuncUp.at(4) > pTmin && genJetPT->at(j) > pTmin) && 
    (JetPT->at(j) * sfNom * vuncUp.at(4) < pTmax && genJetPT->at(j) < pTmax)) 
{
    pT_responseJECAbsScaleup.Fill(JetPT->at(j) * vuncUp.at(4) * sfNom, genJetPT->at(j), weight);
    purity_recoJECAbsScaleup->Fill(JetPT->at(j) * vuncUp.at(4) * sfNom, weight);
    eff_recoJECAbsScaleup->Fill(genJetPT->at(j), weight);
}

if ((JetPT->at(j) * sfNom * vuncUp.at(5) > pTmin && genJetPT->at(j) > pTmin) && 
    (JetPT->at(j) * sfNom * vuncUp.at(5) < pTmax && genJetPT->at(j) < pTmax)) 
{
    pT_responseJECAbsMPFup.Fill(JetPT->at(j) * vuncUp.at(5) * sfNom, genJetPT->at(j), weight);
    purity_recoJECAbsMPFup->Fill(JetPT->at(j) * vuncUp.at(5) * sfNom, weight);
    eff_recoJECAbsMPFup->Fill(genJetPT->at(j), weight);
}

if ((JetPT->at(j) * sfNom * vuncUp.at(6) > pTmin && genJetPT->at(j) > pTmin) && 
    (JetPT->at(j) * sfNom * vuncUp.at(6) < pTmax && genJetPT->at(j) < pTmax)) 
{
    pT_responseJECECALup.Fill(JetPT->at(j) * vuncUp.at(6) * sfNom, genJetPT->at(j), weight);
    purity_recoJECECALup->Fill(JetPT->at(j) * vuncUp.at(6) * sfNom, weight);
    eff_recoJECECALup->Fill(genJetPT->at(j), weight);
}

if ((JetPT->at(j) * sfNom * vuncUp.at(7) > pTmin && genJetPT->at(j) > pTmin) && 
    (JetPT->at(j) * sfNom * vuncUp.at(7) < pTmax && genJetPT->at(j) < pTmax)) 
{
    pT_responseJECHCALup.Fill(JetPT->at(j) * vuncUp.at(7) * sfNom, genJetPT->at(j), weight);
    purity_recoJECHCALup->Fill(JetPT->at(j) * vuncUp.at(7) * sfNom, weight);
    eff_recoJECHCALup->Fill(genJetPT->at(j), weight);
}



if ((JetPT->at(j) * sfNom * vuncUp.at(8) > pTmin && genJetPT->at(j) > pTmin) && 
    (JetPT->at(j) * sfNom * vuncUp.at(8) < pTmax && genJetPT->at(j) < pTmax)) 
{
    pT_responseJECRelStatup.Fill(JetPT->at(j) * vuncUp.at(8) * sfNom, genJetPT->at(j), weight);
    purity_recoJECRelStatup->Fill(JetPT->at(j) * vuncUp.at(8) * sfNom, weight);
    eff_recoJECRelStatup->Fill(genJetPT->at(j), weight);
}

if ((JetPT->at(j) * sfNom * vuncUp.at(9) > pTmin && genJetPT->at(j) > pTmin) && 
    (JetPT->at(j) * sfNom * vuncUp.at(9) < pTmax && genJetPT->at(j) < pTmax)) 
{
    pT_responseJECRelPtup.Fill(JetPT->at(j) * vuncUp.at(9) * sfNom, genJetPT->at(j), weight);
    purity_recoJECRelPtup->Fill(JetPT->at(j) * vuncUp.at(9) * sfNom, weight);
    eff_recoJECRelPtup->Fill(genJetPT->at(j), weight);
}

if ((JetPT->at(j) * sfNom * vuncUp.at(10) > pTmin && genJetPT->at(j) > pTmin) && 
    (JetPT->at(j) * sfNom * vuncUp.at(10) < pTmax && genJetPT->at(j) < pTmax)) 
{
    pT_responseJECRelBalup.Fill(JetPT->at(j) * vuncUp.at(10) * sfNom, genJetPT->at(j), weight);
    purity_recoJECRelBalup->Fill(JetPT->at(j) * vuncUp.at(10) * sfNom, weight);
    eff_recoJECRelBalup->Fill(genJetPT->at(j), weight);
}

if ((JetPT->at(j) * sfNom * vuncUp.at(11) > pTmin && genJetPT->at(j) > pTmin) && 
    (JetPT->at(j) * sfNom * vuncUp.at(11) < pTmax && genJetPT->at(j) < pTmax)) 
{
    pT_responseJECRelJERup.Fill(JetPT->at(j) * vuncUp.at(11) * sfNom, genJetPT->at(j), weight);
    purity_recoJECRelJERup->Fill(JetPT->at(j) * vuncUp.at(11) * sfNom, weight);
    eff_recoJECRelJERup->Fill(genJetPT->at(j), weight);
}

if ((JetPT->at(j) * sfNom * vuncUp.at(12) > pTmin && genJetPT->at(j) > pTmin) && 
    (JetPT->at(j) * sfNom * vuncUp.at(12) < pTmax && genJetPT->at(j) < pTmax)) 
{
    pT_responseJECL3Resup.Fill(JetPT->at(j) * vuncUp.at(12) * sfNom, genJetPT->at(j), weight);
    purity_recoJECL3Resup->Fill(JetPT->at(j) * vuncUp.at(12) * sfNom, weight);
    eff_recoJECL3Resup->Fill(genJetPT->at(j), weight);
}



    if ( (JetPT->at(j)*sfNom*uncDown > pTmin && genJetPT->at(j) > pTmin ) && (JetPT->at(j)*sfNom*uncDown < pTmax && genJetPT->at(j) < pTmax)) 
    {
    pT_responseJECdown.Fill(JetPT->at(j)*uncDown*sfNom, genJetPT->at(j),weight);
    if ( MJTNcHad->at(j) > 0 || MJTNbHad->at(j) > 0 )  pT_HFresponseJECdown.Fill(JetPT->at(j)*uncDown*sfNom, genJetPT->at(j),weight);

    purity_recoJECdown->Fill(JetPT->at(j)*uncDown*sfNom,weight);
    eff_recoJECdown->Fill(genJetPT->at(j),weight);
    }

if ((JetPT->at(j) * sfNom * vuncDown.at(0) > pTmin && genJetPT->at(j) > pTmin) &&
    (JetPT->at(j) * sfNom * vuncDown.at(0) < pTmax && genJetPT->at(j) < pTmax))
{
    pT_responseJECMCdown.Fill(JetPT->at(j) * vuncDown.at(0) * sfNom, genJetPT->at(j), weight);
    purity_recoJECMCdown->Fill(JetPT->at(j) * vuncDown.at(0) * sfNom, weight);
    eff_recoJECMCdown->Fill(genJetPT->at(j), weight);
}

if ((JetPT->at(j) * sfNom * vuncDown.at(1) > pTmin && genJetPT->at(j) > pTmin) &&
    (JetPT->at(j) * sfNom * vuncDown.at(1) < pTmax && genJetPT->at(j) < pTmax))
{
    pT_responseJECFragdown.Fill(JetPT->at(j) * vuncDown.at(1) * sfNom, genJetPT->at(j), weight);
    purity_recoJECFragdown->Fill(JetPT->at(j) * vuncDown.at(1) * sfNom, weight);
    eff_recoJECFragdown->Fill(genJetPT->at(j), weight);
}

if ((JetPT->at(j) * sfNom * vuncDown.at(2) > pTmin && genJetPT->at(j) > pTmin) &&
    (JetPT->at(j) * sfNom * vuncDown.at(2) < pTmax && genJetPT->at(j) < pTmax))
{
    pT_responseJECFlavordown.Fill(JetPT->at(j) * vuncDown.at(2) * sfNom, genJetPT->at(j), weight);
    purity_recoJECFlavordown->Fill(JetPT->at(j) * vuncDown.at(2) * sfNom, weight);
    eff_recoJECFlavordown->Fill(genJetPT->at(j), weight);
}

if ((JetPT->at(j) * sfNom * vuncDown.at(3) > pTmin && genJetPT->at(j) > pTmin) &&
    (JetPT->at(j) * sfNom * vuncDown.at(3) < pTmax && genJetPT->at(j) < pTmax))
{
    pT_responseJECAbsStatdown.Fill(JetPT->at(j) * vuncDown.at(3) * sfNom, genJetPT->at(j), weight);
    purity_recoJECAbsStatdown->Fill(JetPT->at(j) * vuncDown.at(3) * sfNom, weight);
    eff_recoJECAbsStatdown->Fill(genJetPT->at(j), weight);
}

if ((JetPT->at(j) * sfNom * vuncDown.at(4) > pTmin && genJetPT->at(j) > pTmin) &&
    (JetPT->at(j) * sfNom * vuncDown.at(4) < pTmax && genJetPT->at(j) < pTmax))
{
    pT_responseJECAbsScaledown.Fill(JetPT->at(j) * vuncDown.at(4) * sfNom, genJetPT->at(j), weight);
    purity_recoJECAbsScaledown->Fill(JetPT->at(j) * vuncDown.at(4) * sfNom, weight);
    eff_recoJECAbsScaledown->Fill(genJetPT->at(j), weight);
}

if ((JetPT->at(j) * sfNom * vuncDown.at(5) > pTmin && genJetPT->at(j) > pTmin) &&
    (JetPT->at(j) * sfNom * vuncDown.at(5) < pTmax && genJetPT->at(j) < pTmax))
{
    pT_responseJECAbsMPFdown.Fill(JetPT->at(j) * vuncDown.at(5) * sfNom, genJetPT->at(j), weight);
    purity_recoJECAbsMPFdown->Fill(JetPT->at(j) * vuncDown.at(5) * sfNom, weight);
    eff_recoJECAbsMPFdown->Fill(genJetPT->at(j), weight);
}

if ((JetPT->at(j) * sfNom * vuncDown.at(6) > pTmin && genJetPT->at(j) > pTmin) &&
    (JetPT->at(j) * sfNom * vuncDown.at(6) < pTmax && genJetPT->at(j) < pTmax))
{
    pT_responseJECECALdown.Fill(JetPT->at(j) * vuncDown.at(6) * sfNom, genJetPT->at(j), weight);
    purity_recoJECECALdown->Fill(JetPT->at(j) * vuncDown.at(6) * sfNom, weight);
    eff_recoJECECALdown->Fill(genJetPT->at(j), weight);
}

if ((JetPT->at(j) * sfNom * vuncDown.at(7) > pTmin && genJetPT->at(j) > pTmin) &&
    (JetPT->at(j) * sfNom * vuncDown.at(7) < pTmax && genJetPT->at(j) < pTmax))
{
    pT_responseJECHCALdown.Fill(JetPT->at(j) * vuncDown.at(7) * sfNom, genJetPT->at(j), weight);
    purity_recoJECHCALdown->Fill(JetPT->at(j) * vuncDown.at(7) * sfNom, weight);
    eff_recoJECHCALdown->Fill(genJetPT->at(j), weight);
}

if ((JetPT->at(j) * sfNom * vuncDown.at(8) > pTmin && genJetPT->at(j) > pTmin) &&
    (JetPT->at(j) * sfNom * vuncDown.at(8) < pTmax && genJetPT->at(j) < pTmax))
{
    pT_responseJECRelStatdown.Fill(JetPT->at(j) * vuncDown.at(8) * sfNom, genJetPT->at(j), weight);
    purity_recoJECRelStatdown->Fill(JetPT->at(j) * vuncDown.at(8) * sfNom, weight);
    eff_recoJECRelStatdown->Fill(genJetPT->at(j), weight);
}

if ((JetPT->at(j) * sfNom * vuncDown.at(9) > pTmin && genJetPT->at(j) > pTmin) &&
    (JetPT->at(j) * sfNom * vuncDown.at(9) < pTmax && genJetPT->at(j) < pTmax))
{
    pT_responseJECRelPtdown.Fill(JetPT->at(j) * vuncDown.at(9) * sfNom, genJetPT->at(j), weight);
    purity_recoJECRelPtdown->Fill(JetPT->at(j) * vuncDown.at(9) * sfNom, weight);
    eff_recoJECRelPtdown->Fill(genJetPT->at(j), weight);
}

if ((JetPT->at(j) * sfNom * vuncDown.at(10) > pTmin && genJetPT->at(j) > pTmin) && 
    (JetPT->at(j) * sfNom * vuncDown.at(10) < pTmax && genJetPT->at(j) < pTmax)) 
{
    pT_responseJECRelBaldown.Fill(JetPT->at(j) * vuncDown.at(10) * sfNom, genJetPT->at(j), weight);
    purity_recoJECRelBaldown->Fill(JetPT->at(j) * vuncDown.at(10) * sfNom, weight);
    eff_recoJECRelBaldown->Fill(genJetPT->at(j), weight);
}

if ((JetPT->at(j) * sfNom * vuncDown.at(11) > pTmin && genJetPT->at(j) > pTmin) && 
    (JetPT->at(j) * sfNom * vuncDown.at(11) < pTmax && genJetPT->at(j) < pTmax)) 
{
    pT_responseJECRelJERdown.Fill(JetPT->at(j) * vuncDown.at(11) * sfNom, genJetPT->at(j), weight);
    purity_recoJECRelJERdown->Fill(JetPT->at(j) * vuncDown.at(11) * sfNom, weight);
    eff_recoJECRelJERdown->Fill(genJetPT->at(j), weight);
}


if ((JetPT->at(j) * sfNom * vuncDown.at(12) > pTmin && genJetPT->at(j) > pTmin) &&
    (JetPT->at(j) * sfNom * vuncDown.at(12) < pTmax && genJetPT->at(j) < pTmax))
{
    pT_responseJECL3Resdown.Fill(JetPT->at(j) * vuncDown.at(12) * sfNom, genJetPT->at(j), weight);
    purity_recoJECL3Resdown->Fill(JetPT->at(j) * vuncDown.at(12) * sfNom, weight);
    eff_recoJECL3Resdown->Fill(genJetPT->at(j), weight);
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


pT_responseJECMCup.Write();
pT_responseJECMCdown.Write();

eff_recoJECMCup->Divide(pTjet_true);
eff_recoJECMCdown->Divide(pTjet_true);

eff_recoJECMCup->Write();
eff_recoJECMCdown->Write();

purity_recoJECMCup->Divide(pTjet_recoJECMCup);
purity_recoJECMCdown->Divide(pTjet_recoJECMCdown);
purity_recoJECMCup->Write();
purity_recoJECMCdown->Write();

pT_responseJECFragup.Write();
pT_responseJECFragdown.Write();

eff_recoJECFragup->Divide(pTjet_true);
eff_recoJECFragdown->Divide(pTjet_true);

eff_recoJECFragup->Write();
eff_recoJECFragdown->Write();

purity_recoJECFragup->Divide(pTjet_recoJECFragup);
purity_recoJECFragdown->Divide(pTjet_recoJECFragdown);
purity_recoJECFragup->Write();
purity_recoJECFragdown->Write();

pT_responseJECFlavorup.Write();
pT_responseJECFlavordown.Write();

eff_recoJECFlavorup->Divide(pTjet_true);
eff_recoJECFlavordown->Divide(pTjet_true);

eff_recoJECFlavorup->Write();
eff_recoJECFlavordown->Write();

purity_recoJECFlavorup->Divide(pTjet_recoJECFlavorup);
purity_recoJECFlavordown->Divide(pTjet_recoJECFlavordown);
purity_recoJECFlavorup->Write();
purity_recoJECFlavordown->Write();

pT_responseJECAbsStatup.Write();
pT_responseJECAbsStatdown.Write();

eff_recoJECAbsStatup->Divide(pTjet_true);
eff_recoJECAbsStatdown->Divide(pTjet_true);

eff_recoJECAbsStatup->Write();
eff_recoJECAbsStatdown->Write();

purity_recoJECAbsStatup->Divide(pTjet_recoJECAbsStatup);
purity_recoJECAbsStatdown->Divide(pTjet_recoJECAbsStatdown);
purity_recoJECAbsStatup->Write();
purity_recoJECAbsStatdown->Write();

pT_responseJECAbsScaleup.Write();
pT_responseJECAbsScaledown.Write();

eff_recoJECAbsScaleup->Divide(pTjet_true);
eff_recoJECAbsScaledown->Divide(pTjet_true);

eff_recoJECAbsScaleup->Write();
eff_recoJECAbsScaledown->Write();

purity_recoJECAbsScaleup->Divide(pTjet_recoJECAbsScaleup);
purity_recoJECAbsScaledown->Divide(pTjet_recoJECAbsScaledown);
purity_recoJECAbsScaleup->Write();
purity_recoJECAbsScaledown->Write();

pT_responseJECAbsMPFup.Write();
pT_responseJECAbsMPFdown.Write();

eff_recoJECAbsMPFup->Divide(pTjet_true);
eff_recoJECAbsMPFdown->Divide(pTjet_true);

eff_recoJECAbsMPFup->Write();
eff_recoJECAbsMPFdown->Write();

purity_recoJECAbsMPFup->Divide(pTjet_recoJECAbsMPFup);
purity_recoJECAbsMPFdown->Divide(pTjet_recoJECAbsMPFdown);
purity_recoJECAbsMPFup->Write();
purity_recoJECAbsMPFdown->Write();

pT_responseJECECALup.Write();
pT_responseJECECALdown.Write();

eff_recoJECECALup->Divide(pTjet_true);
eff_recoJECECALdown->Divide(pTjet_true);

eff_recoJECECALup->Write();
eff_recoJECECALdown->Write();

purity_recoJECECALup->Divide(pTjet_recoJECECALup);
purity_recoJECECALdown->Divide(pTjet_recoJECECALdown);
purity_recoJECECALup->Write();
purity_recoJECECALdown->Write();

pT_responseJECHCALup.Write();
pT_responseJECHCALdown.Write();

eff_recoJECHCALup->Divide(pTjet_true);
eff_recoJECHCALdown->Divide(pTjet_true);

eff_recoJECHCALup->Write();
eff_recoJECHCALdown->Write();

purity_recoJECHCALup->Divide(pTjet_recoJECHCALup);
purity_recoJECHCALdown->Divide(pTjet_recoJECHCALdown);
purity_recoJECHCALup->Write();
purity_recoJECHCALdown->Write();

pT_responseJECRelStatup.Write();
pT_responseJECRelStatdown.Write();

eff_recoJECRelStatup->Divide(pTjet_true);
eff_recoJECRelStatdown->Divide(pTjet_true);

eff_recoJECRelStatup->Write();
eff_recoJECRelStatdown->Write();

purity_recoJECRelStatup->Divide(pTjet_recoJECRelStatup);
purity_recoJECRelStatdown->Divide(pTjet_recoJECRelStatdown);
purity_recoJECRelStatup->Write();
purity_recoJECRelStatdown->Write();

pT_responseJECRelPtup.Write();
pT_responseJECRelPtdown.Write();

eff_recoJECRelPtup->Divide(pTjet_true);
eff_recoJECRelPtdown->Divide(pTjet_true);

eff_recoJECRelPtup->Write();
eff_recoJECRelPtdown->Write();

purity_recoJECRelPtup->Divide(pTjet_recoJECRelPtup);
purity_recoJECRelPtdown->Divide(pTjet_recoJECRelPtdown);
purity_recoJECRelPtup->Write();
purity_recoJECRelPtdown->Write();

pT_responseJECRelBalup.Write();
pT_responseJECRelBaldown.Write();

eff_recoJECRelBalup->Divide(pTjet_true);
eff_recoJECRelBaldown->Divide(pTjet_true);

eff_recoJECRelBalup->Write();
eff_recoJECRelBaldown->Write();

purity_recoJECRelBalup->Divide(pTjet_recoJECRelBalup);
purity_recoJECRelBaldown->Divide(pTjet_recoJECRelBaldown);
purity_recoJECRelBalup->Write();
purity_recoJECRelBaldown->Write();

pT_responseJECRelJERup.Write();
pT_responseJECRelJERdown.Write();

eff_recoJECRelJERup->Divide(pTjet_true);
eff_recoJECRelJERdown->Divide(pTjet_true);

eff_recoJECRelJERup->Write();
eff_recoJECRelJERdown->Write();

purity_recoJECRelJERup->Divide(pTjet_recoJECRelJERup);
purity_recoJECRelJERdown->Divide(pTjet_recoJECRelJERdown);
purity_recoJECRelJERup->Write();
purity_recoJECRelJERdown->Write();

pT_responseJECL3Resup.Write();
pT_responseJECL3Resdown.Write();

eff_recoJECL3Resup->Divide(pTjet_true);
eff_recoJECL3Resdown->Divide(pTjet_true);

eff_recoJECL3Resup->Write();
eff_recoJECL3Resdown->Write();

purity_recoJECL3Resup->Divide(pTjet_recoJECL3Resup);
purity_recoJECL3Resdown->Divide(pTjet_recoJECL3Resdown);
purity_recoJECL3Resup->Write();
purity_recoJECL3Resdown->Write();



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
