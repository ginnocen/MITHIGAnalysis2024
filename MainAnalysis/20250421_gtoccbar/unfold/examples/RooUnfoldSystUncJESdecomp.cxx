//=====================================================================-*-C++-*-
// File and Version Information:
//      $Id: RooUnfoldExample.cxx 279 2011-02-11 18:23:44Z T.J.Adye $
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

#include "TRandom.h"
#include "TH1D.h"

#include "TFile.h"
#include "TVectorD.h"

#include "TROOT.h"
#include "TString.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TRandom.h"
#include "TPostScript.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TFile.h"
#include "TLine.h"
#include "TNtuple.h"
#include "TProfile.h"

#include "RooUnfoldResponse.h"
#include "RooUnfoldBayes.h"
#include "sstream"
//#include "RooUnfoldTestHarness2D.h"
#endif

//==============================================================================
// Global definitions
//==============================================================================

const Double_t cutdummy= -99999.0;

/*
 *
 *ReadResponseRun2_JECdown.root
ReadResponseRun2_JECup.root
ReadResponseRun2_JERdown.root
ReadResponseRun2_JERup.root
ReadResponseRun2_PUdown.root
ReadResponseRun2_PUup.root
ReadResponseRun2_TrackIneff.root
ReadResponseRun2_nom.root

ReadResponseRun2V2_HEM1516_ak4.root
ReadResponseRun2V2_JECdown_ak4.root
ReadResponseRun2V2_JECup_ak4.root
ReadResponseRun2V2_JERdown_ak4.root
ReadResponseRun2V2_JERup_ak4.root
ReadResponseRun2V2_PUdown_ak4.root
ReadResponseRun2V2_PUup_ak4.root
ReadResponseRun2V2_herwig7_ak4.root
ReadResponseRun2V2_nom_ak4.root
ReadResponseRun2V2_trackIneff_ak4.root

for 2016:

ReadResponseLumiWeightTrackIneffV2pythia82016_trackIneff_ak4.root



Redone:ReadResponseLumiWeightTrackIneffV2pythia8_Run2_ak4.root
 *
 *
 * */

void RooUnfoldSystUncJESdecomp(std::string file_mc="/home/cbaldene/Unfolding/RooUnfoldSetupForLuis/examples/response3.root", 
std::string file_data="/home/cbaldene/Unfolding/RooUnfoldSetupForLuis/Nc2_v2_jetReco.root", std::string date = "response",int flag=19)
{
  RooUnfold::ErrorTreatment errorTreatment = RooUnfold::kCovariance;
  TFile* f_mc=new TFile(file_mc.c_str());
  TFile* f_data=new TFile(file_data.c_str());

  TH1D *fake=(TH1D*)f_mc->Get("fake");
  TH1D *miss=(TH1D*)f_mc->Get("miss");

  RooUnfoldResponse* response = (RooUnfoldResponse*)f_mc->Get("pTjet_recoTest_pTjet_trueTest");

  TH1D *fakeJECup=(TH1D*)f_mc->Get("purity_recoJECup");
  TH1D *missJECup=(TH1D*)f_mc->Get("eff_recoJECup");

  RooUnfoldResponse* responseJECup = (RooUnfoldResponse*)f_mc->Get("pTjet_recoJECup_pTjet_true");

  TH1D *fakeJERup=(TH1D*)f_mc->Get("purity_recoJERup");
  TH1D *missJERup=(TH1D*)f_mc->Get("eff_recoJERup");

  RooUnfoldResponse* responseJERup = (RooUnfoldResponse*)f_mc->Get("pTjet_recoJERup_pTjet_true");


  TH1D *fakeJERdown=(TH1D*)f_mc->Get("purity_recoJERdown");
  TH1D *missJERdown=(TH1D*)f_mc->Get("eff_recoJERdown");

  RooUnfoldResponse* responseJERdown = (RooUnfoldResponse*)f_mc->Get("pTjet_recoJERdown_pTjet_true");


  TH1D *fakeJECdown=(TH1D*)f_mc->Get("purity_recoJECdown");
  TH1D *missJECdown=(TH1D*)f_mc->Get("eff_recoJECdown");

  RooUnfoldResponse* responseJECdown = (RooUnfoldResponse*)f_mc->Get("pTjet_recoJECdown_pTjet_true");

   TH1D *fakeJECMCup = (TH1D*)f_mc->Get("purity_recoJECMCup");
   TH1D *missJECMCup = (TH1D*)f_mc->Get("eff_recoJECMCup");
   RooUnfoldResponse* responseJECMCup = (RooUnfoldResponse*)f_mc->Get("pTjet_recoJECMCup_pTjet_true");

   TH1D *fakeJECFragup = (TH1D*)f_mc->Get("purity_recoJECFragup");
   TH1D *missJECFragup = (TH1D*)f_mc->Get("eff_recoJECFragup");
   RooUnfoldResponse* responseJECFragup = (RooUnfoldResponse*)f_mc->Get("pTjet_recoJECFragup_pTjet_true");

   TH1D *fakeJECFlavorup = (TH1D*)f_mc->Get("purity_recoJECFlavorup");
   TH1D *missJECFlavorup = (TH1D*)f_mc->Get("eff_recoJECFlavorup");
   RooUnfoldResponse* responseJECFlavorup = (RooUnfoldResponse*)f_mc->Get("pTjet_recoJECFlavorup_pTjet_true");

   TH1D *fakeJECAbsStatup = (TH1D*)f_mc->Get("purity_recoJECAbsStatup");
   TH1D *missJECAbsStatup = (TH1D*)f_mc->Get("eff_recoJECAbsStatup");
   RooUnfoldResponse* responseJECAbsStatup = (RooUnfoldResponse*)f_mc->Get("pTjet_recoJECAbsStatup_pTjet_true");

   TH1D *fakeJECAbsScaleup = (TH1D*)f_mc->Get("purity_recoJECAbsScaleup");
   TH1D *missJECAbsScaleup = (TH1D*)f_mc->Get("eff_recoJECAbsScaleup");
   RooUnfoldResponse* responseJECAbsScaleup = (RooUnfoldResponse*)f_mc->Get("pTjet_recoJECAbsScaleup_pTjet_true");

   TH1D *fakeJECAbsMPFup = (TH1D*)f_mc->Get("purity_recoJECAbsMPFup");
   TH1D *missJECAbsMPFup = (TH1D*)f_mc->Get("eff_recoJECAbsMPFup");
   RooUnfoldResponse* responseJECAbsMPFup = (RooUnfoldResponse*)f_mc->Get("pTjet_recoJECAbsMPFup_pTjet_true");

   TH1D *fakeJECECALup = (TH1D*)f_mc->Get("purity_recoJECECALup");
   TH1D *missJECECALup = (TH1D*)f_mc->Get("eff_recoJECECALup");
   RooUnfoldResponse* responseJECECALup = (RooUnfoldResponse*)f_mc->Get("pTjet_recoJECECALup_pTjet_true");

   TH1D *fakeJECHCALup = (TH1D*)f_mc->Get("purity_recoJECHCALup");
   TH1D *missJECHCALup = (TH1D*)f_mc->Get("eff_recoJECHCALup");
   RooUnfoldResponse* responseJECHCALup = (RooUnfoldResponse*)f_mc->Get("pTjet_recoJECHCALup_pTjet_true");

   TH1D *fakeJECRelStatup = (TH1D*)f_mc->Get("purity_recoJECRelStatup");
   TH1D *missJECRelStatup = (TH1D*)f_mc->Get("eff_recoJECRelStatup");
   RooUnfoldResponse* responseJECRelStatup = (RooUnfoldResponse*)f_mc->Get("pTjet_recoJECRelStatup_pTjet_true");

   TH1D *fakeJECRelPtup = (TH1D*)f_mc->Get("purity_recoJECRelPtup");
   TH1D *missJECRelPtup = (TH1D*)f_mc->Get("eff_recoJECRelPtup");
   RooUnfoldResponse* responseJECRelPtup = (RooUnfoldResponse*)f_mc->Get("pTjet_recoJECRelPtup_pTjet_true");

   TH1D *fakeJECRelBalup = (TH1D*)f_mc->Get("purity_recoJECRelBalup");
   TH1D *missJECRelBalup = (TH1D*)f_mc->Get("eff_recoJECRelBalup");
   RooUnfoldResponse* responseJECRelBalup = (RooUnfoldResponse*)f_mc->Get("pTjet_recoJECRelBalup_pTjet_true");

   TH1D *fakeJECRelJERup = (TH1D*)f_mc->Get("purity_recoJECRelJERup");
   TH1D *missJECRelJERup = (TH1D*)f_mc->Get("eff_recoJECRelJERup");
   RooUnfoldResponse* responseJECRelJERup = (RooUnfoldResponse*)f_mc->Get("pTjet_recoJECRelJERup_pTjet_true");

   TH1D *fakeJECL3Resup = (TH1D*)f_mc->Get("purity_recoJECL3Resup");
   TH1D *missJECL3Resup = (TH1D*)f_mc->Get("eff_recoJECL3Resup");
   RooUnfoldResponse* responseJECL3Resup = (RooUnfoldResponse*)f_mc->Get("pTjet_recoJECL3Resup_pTjet_true");




   TH1D *fakeJECMCdown = (TH1D*)f_mc->Get("purity_recoJECMCdown");
   TH1D *missJECMCdown = (TH1D*)f_mc->Get("eff_recoJECMCdown");
   RooUnfoldResponse* responseJECMCdown = (RooUnfoldResponse*)f_mc->Get("pTjet_recoJECMCdown_pTjet_true");

   TH1D *fakeJECFragdown = (TH1D*)f_mc->Get("purity_recoJECFragdown");
   TH1D *missJECFragdown = (TH1D*)f_mc->Get("eff_recoJECFragdown");
   RooUnfoldResponse* responseJECFragdown = (RooUnfoldResponse*)f_mc->Get("pTjet_recoJECFragdown_pTjet_true");

   TH1D *fakeJECFlavordown = (TH1D*)f_mc->Get("purity_recoJECFlavordown");
   TH1D *missJECFlavordown = (TH1D*)f_mc->Get("eff_recoJECFlavordown");
   RooUnfoldResponse* responseJECFlavordown = (RooUnfoldResponse*)f_mc->Get("pTjet_recoJECFlavordown_pTjet_true");

   TH1D *fakeJECAbsStatdown = (TH1D*)f_mc->Get("purity_recoJECAbsStatdown");
   TH1D *missJECAbsStatdown = (TH1D*)f_mc->Get("eff_recoJECAbsStatdown");
   RooUnfoldResponse* responseJECAbsStatdown = (RooUnfoldResponse*)f_mc->Get("pTjet_recoJECAbsStatdown_pTjet_true");

   TH1D *fakeJECAbsScaledown = (TH1D*)f_mc->Get("purity_recoJECAbsScaledown");
   TH1D *missJECAbsScaledown = (TH1D*)f_mc->Get("eff_recoJECAbsScaledown");
   RooUnfoldResponse* responseJECAbsScaledown = (RooUnfoldResponse*)f_mc->Get("pTjet_recoJECAbsScaledown_pTjet_true");

   TH1D *fakeJECAbsMPFdown = (TH1D*)f_mc->Get("purity_recoJECAbsMPFdown");
   TH1D *missJECAbsMPFdown = (TH1D*)f_mc->Get("eff_recoJECAbsMPFdown");
   RooUnfoldResponse* responseJECAbsMPFdown = (RooUnfoldResponse*)f_mc->Get("pTjet_recoJECAbsMPFdown_pTjet_true");

   TH1D *fakeJECECALdown = (TH1D*)f_mc->Get("purity_recoJECECALdown");
   TH1D *missJECECALdown = (TH1D*)f_mc->Get("eff_recoJECECALdown");
   RooUnfoldResponse* responseJECECALdown = (RooUnfoldResponse*)f_mc->Get("pTjet_recoJECECALdown_pTjet_true");

   TH1D *fakeJECHCALdown = (TH1D*)f_mc->Get("purity_recoJECHCALdown");
   TH1D *missJECHCALdown = (TH1D*)f_mc->Get("eff_recoJECHCALdown");
   RooUnfoldResponse* responseJECHCALdown = (RooUnfoldResponse*)f_mc->Get("pTjet_recoJECHCALdown_pTjet_true");

   TH1D *fakeJECRelStatdown = (TH1D*)f_mc->Get("purity_recoJECRelStatdown");
   TH1D *missJECRelStatdown = (TH1D*)f_mc->Get("eff_recoJECRelStatdown");
   RooUnfoldResponse* responseJECRelStatdown = (RooUnfoldResponse*)f_mc->Get("pTjet_recoJECRelStatdown_pTjet_true");

   TH1D *fakeJECRelPtdown = (TH1D*)f_mc->Get("purity_recoJECRelPtdown");
   TH1D *missJECRelPtdown = (TH1D*)f_mc->Get("eff_recoJECRelPtdown");
   RooUnfoldResponse* responseJECRelPtdown = (RooUnfoldResponse*)f_mc->Get("pTjet_recoJECRelPtdown_pTjet_true");

   TH1D *fakeJECRelBaldown = (TH1D*)f_mc->Get("purity_recoJECRelBaldown");
   TH1D *missJECRelBaldown = (TH1D*)f_mc->Get("eff_recoJECRelBaldown");
   RooUnfoldResponse* responseJECRelBaldown = (RooUnfoldResponse*)f_mc->Get("pTjet_recoJECRelBaldown_pTjet_true");

   TH1D *fakeJECRelJERdown = (TH1D*)f_mc->Get("purity_recoJECRelJERdown");
   TH1D *missJECRelJERdown = (TH1D*)f_mc->Get("eff_recoJECRelJERdown");
   RooUnfoldResponse* responseJECRelJERdown = (RooUnfoldResponse*)f_mc->Get("pTjet_recoJECRelJERdown_pTjet_true");

   TH1D *fakeJECL3Resdown = (TH1D*)f_mc->Get("purity_recoJECL3Resdown");
   TH1D *missJECL3Resdown = (TH1D*)f_mc->Get("eff_recoJECL3Resdown");
   RooUnfoldResponse* responseJECL3Resdown = (RooUnfoldResponse*)f_mc->Get("pTjet_recoJECL3Resdown_pTjet_true");




/*  KEY: RooUnfoldResponse	pTjet_reco_pTjet_true;1	Response pTjet_true #rightarrow pTjet_reco
  KEY: RooUnfoldResponse	pTjet_recoJECup_pTjet_true;1	Response pTjet_true #rightarrow pTjet_reco
  KEY: RooUnfoldResponse	pTjet_recoJECdown_pTjet_true;1	Response pTjet_true #rightarrow pTjet_reco
  KEY: RooUnfoldResponse	pTjet_recoJERup_pTjet_true;1	Response pTjet_true #rightarrow pTjet_reco
  KEY: RooUnfoldResponse	pTjet_recoJERdown_pTjet_true;1	Response pTjet_true #rightarrow pTjet_reco
  KEY: RooUnfoldResponse	pTjet_HFreco_pTjet_true;1	Response pTjet_true #rightarrow pTjet_reco
  KEY: RooUnfoldResponse	pTjet_HFrecoJECup_pTjet_true;1	Response pTjet_true #rightarrow pTjet_reco
  KEY: RooUnfoldResponse	pTjet_HFrecoJECdown_pTjet_true;1	Response pTjet_true #rightarrow pTjet_reco
  KEY: RooUnfoldResponse	pTjet_HFrecoJERup_pTjet_true;1	Response pTjet_true #rightarrow pTjet_reco
  KEY: RooUnfoldResponse	pTjet_HFrecoJERdown_pTjet_true;1	Response pTjet_true #rightarrow pTjet_reco

*/

   map<string,TH1F*> histosTH1F;
   map<string,TH2F*> histosTH2F;
   map<string,TH2D*> histosTH2D;

   for(int i=0; i<15; i++)
   {
   char ptbinsRead[100];
   sprintf(ptbinsRead, "_%d", i);
   string histnameRead(ptbinsRead);

   histosTH1F[("jetPtDimuon"+histnameRead).c_str()] = (TH1F*) f_data->Get(("jetPt"+histnameRead).c_str());

   }

   for(map<string,TH1F*>::const_iterator it = histosTH1F.begin(); it != histosTH1F.end(); ++it)
      it->second->Sumw2();

   for(map<string,TH2F*>::const_iterator it = histosTH2F.begin(); it != histosTH2F.end(); ++it)
      it->second->Sumw2();


      TDecompSVD *svd= new TDecompSVD (response->Mresponse()); //this is the singular value decomposition (SVD) matrix
      auto singular_values = svd->GetSig(); //this is a vector with the singular values, i.e., the diagonal elements of S. They are ordered from largest to smallest.

      svd->Print();

  TFile* fout = new TFile("output.root", "recreate");
//   TMatrixT<double> covariance();

     for(int jar=0;jar<1;jar++)
     {
      Int_t iter=jar;
      cout<<"iteration"<<iter<<endl;
      cout<<"==============Unfold h1====================="<<endl;
      char ptbinsRead[100];
      sprintf(ptbinsRead, "_%d", jar);
      string histnameRead(ptbinsRead);
      TH1F* pTjet_smeared = histosTH1F[("jetPtDimuon"+histnameRead).c_str()];
//      pTjet_smeared->Multiply(fake);

     RooUnfoldBayes   unfoldInvert(response, pTjet_smeared , 4); 
//      RooUnfoldInvert   unfoldInvert(response, histosTH1F["jetPt_0"] );
      TH1D* hunf= (TH1D*) unfoldInvert.Hreco(errorTreatment);
      TH1D* hfold= (TH1D*)response->ApplyToTruth(hunf,"");
      TH1D *htempUnf=(TH1D*)hunf->Clone("htempUnf");
      htempUnf->SetName(Form("unfoldingSolution_%d",iter));
      htempUnf->Divide(miss);
      htempUnf->Write();
      pTjet_smeared->Divide(fake);

      pTjet_smeared->Multiply(fakeJERup);
      RooUnfoldBayes   unfoldInvertJERup(responseJERup, pTjet_smeared , 4); 
      TH1D* hunfJERup= (TH1D*) unfoldInvertJERup.Hreco(errorTreatment);
      TH1D *htempUnfJERup=(TH1D*)hunfJERup->Clone("htempUnf");
      htempUnfJERup->SetName(Form("unfoldingSolutionJERup_%d",iter));
      htempUnfJERup->Divide(missJERup);
      htempUnfJERup->Write();
      pTjet_smeared->Divide(fakeJERup);


      pTjet_smeared->Multiply(fakeJERdown);
      RooUnfoldBayes   unfoldInvertJERdown(responseJERdown, pTjet_smeared , 4); 
      TH1D* hunfJERdown= (TH1D*) unfoldInvertJERdown.Hreco(errorTreatment);
      TH1D *htempUnfJERdown=(TH1D*)hunfJERdown->Clone("htempUnf");
      htempUnfJERdown->SetName(Form("unfoldingSolutionJERdown_%d",iter));
      htempUnfJERdown->Divide(missJERdown);
      htempUnfJERdown->Write();
      pTjet_smeared->Divide(fakeJERdown);

      pTjet_smeared->Multiply(fakeJECup);
      RooUnfoldBayes   unfoldInvertJECup(responseJECup, pTjet_smeared , 4); 
      TH1D* hunfJECup= (TH1D*) unfoldInvertJECup.Hreco(errorTreatment);
      TH1D *htempUnfJECup=(TH1D*)hunfJECup->Clone("htempUnf");
      htempUnfJECup->SetName(Form("unfoldingSolutionJECup_%d",iter));
      htempUnfJECup->Divide(missJECup);
      htempUnfJECup->Write();
      pTjet_smeared->Divide(fakeJECup);

      pTjet_smeared->Multiply(fakeJECMCup);
      RooUnfoldBayes unfoldInvertJECMCup(responseJECMCup, pTjet_smeared, 4); 
      TH1D* hunfJECMCup = (TH1D*) unfoldInvertJECMCup.Hreco(errorTreatment);
      TH1D* htempUnfJECMCup = (TH1D*) hunfJECMCup->Clone("htempUnf");
      htempUnfJECMCup->SetName(Form("unfoldingSolutionJECMCup_%d", iter));
      htempUnfJECMCup->Divide(missJECMCup);
      htempUnfJECMCup->Write();
      pTjet_smeared->Divide(fakeJECMCup);

      pTjet_smeared->Multiply(fakeJECFragup);
      RooUnfoldBayes unfoldInvertJECFragup(responseJECFragup, pTjet_smeared, 4); 
      TH1D* hunfJECFragup = (TH1D*) unfoldInvertJECFragup.Hreco(errorTreatment);
      TH1D* htempUnfJECFragup = (TH1D*) hunfJECFragup->Clone("htempUnf");
      htempUnfJECFragup->SetName(Form("unfoldingSolutionJECFragup_%d", iter));
      htempUnfJECFragup->Divide(missJECFragup);
      htempUnfJECFragup->Write();
      pTjet_smeared->Divide(fakeJECFragup);

      pTjet_smeared->Multiply(fakeJECFlavorup);
      RooUnfoldBayes unfoldInvertJECFlavorup(responseJECFlavorup, pTjet_smeared, 4); 
      TH1D* hunfJECFlavorup = (TH1D*) unfoldInvertJECFlavorup.Hreco(errorTreatment);
      TH1D* htempUnfJECFlavorup = (TH1D*) hunfJECFlavorup->Clone("htempUnf");
      htempUnfJECFlavorup->SetName(Form("unfoldingSolutionJECFlavorup_%d", iter));
      htempUnfJECFlavorup->Divide(missJECFlavorup);
      htempUnfJECFlavorup->Write();
      pTjet_smeared->Divide(fakeJECFlavorup);

      pTjet_smeared->Multiply(fakeJECAbsStatup);
      RooUnfoldBayes unfoldInvertJECAbsStatup(responseJECAbsStatup, pTjet_smeared, 4); 
      TH1D* hunfJECAbsStatup = (TH1D*) unfoldInvertJECAbsStatup.Hreco(errorTreatment);
      TH1D* htempUnfJECAbsStatup = (TH1D*) hunfJECAbsStatup->Clone("htempUnf");
      htempUnfJECAbsStatup->SetName(Form("unfoldingSolutionJECAbsStatup_%d", iter));
      htempUnfJECAbsStatup->Divide(missJECAbsStatup);
      htempUnfJECAbsStatup->Write();
      pTjet_smeared->Divide(fakeJECAbsStatup);

      pTjet_smeared->Multiply(fakeJECAbsScaleup);
      RooUnfoldBayes unfoldInvertJECAbsScaleup(responseJECAbsScaleup, pTjet_smeared, 4); 
      TH1D* hunfJECAbsScaleup = (TH1D*) unfoldInvertJECAbsScaleup.Hreco(errorTreatment);
      TH1D* htempUnfJECAbsScaleup = (TH1D*) hunfJECAbsScaleup->Clone("htempUnf");
      htempUnfJECAbsScaleup->SetName(Form("unfoldingSolutionJECAbsScaleup_%d", iter));
      htempUnfJECAbsScaleup->Divide(missJECAbsScaleup);
      htempUnfJECAbsScaleup->Write();
      pTjet_smeared->Divide(fakeJECAbsScaleup);

      pTjet_smeared->Multiply(fakeJECAbsMPFup);
      RooUnfoldBayes unfoldInvertJECAbsMPFup(responseJECAbsMPFup, pTjet_smeared, 4); 
      TH1D* hunfJECAbsMPFup = (TH1D*) unfoldInvertJECAbsMPFup.Hreco(errorTreatment);
      TH1D* htempUnfJECAbsMPFup = (TH1D*) hunfJECAbsMPFup->Clone("htempUnf");
      htempUnfJECAbsMPFup->SetName(Form("unfoldingSolutionJECAbsMPFup_%d", iter));
      htempUnfJECAbsMPFup->Divide(missJECAbsMPFup);
      htempUnfJECAbsMPFup->Write();
      pTjet_smeared->Divide(fakeJECAbsMPFup);

      pTjet_smeared->Multiply(fakeJECECALup);
      RooUnfoldBayes unfoldInvertJECECALup(responseJECECALup, pTjet_smeared, 4); 
      TH1D* hunfJECECALup = (TH1D*) unfoldInvertJECECALup.Hreco(errorTreatment);
      TH1D* htempUnfJECECALup = (TH1D*) hunfJECECALup->Clone("htempUnf");
      htempUnfJECECALup->SetName(Form("unfoldingSolutionJECECALup_%d", iter));
      htempUnfJECECALup->Divide(missJECECALup);
      htempUnfJECECALup->Write();
      pTjet_smeared->Divide(fakeJECECALup);

      pTjet_smeared->Multiply(fakeJECHCALup);
      RooUnfoldBayes unfoldInvertJECHCALup(responseJECHCALup, pTjet_smeared, 4); 
      TH1D* hunfJECHCALup = (TH1D*) unfoldInvertJECHCALup.Hreco(errorTreatment);
      TH1D* htempUnfJECHCALup = (TH1D*) hunfJECHCALup->Clone("htempUnf");
      htempUnfJECHCALup->SetName(Form("unfoldingSolutionJECHCALup_%d", iter));
      htempUnfJECHCALup->Divide(missJECHCALup);
      htempUnfJECHCALup->Write();
      pTjet_smeared->Divide(fakeJECHCALup);

      pTjet_smeared->Multiply(fakeJECRelStatup);
      RooUnfoldBayes unfoldInvertJECRelStatup(responseJECRelStatup, pTjet_smeared, 4); 
      TH1D* hunfJECRelStatup = (TH1D*) unfoldInvertJECRelStatup.Hreco(errorTreatment);
      TH1D* htempUnfJECRelStatup = (TH1D*) hunfJECRelStatup->Clone("htempUnf");
      htempUnfJECRelStatup->SetName(Form("unfoldingSolutionJECRelStatup_%d", iter));
      htempUnfJECRelStatup->Divide(missJECRelStatup);
      htempUnfJECRelStatup->Write();
      pTjet_smeared->Divide(fakeJECRelStatup);

      pTjet_smeared->Multiply(fakeJECRelPtup);
      RooUnfoldBayes unfoldInvertJECRelPtup(responseJECRelPtup, pTjet_smeared, 4); 
      TH1D* hunfJECRelPtup = (TH1D*) unfoldInvertJECRelPtup.Hreco(errorTreatment);
      TH1D* htempUnfJECRelPtup = (TH1D*) hunfJECRelPtup->Clone("htempUnf");
      htempUnfJECRelPtup->SetName(Form("unfoldingSolutionJECRelPtup_%d", iter));
      htempUnfJECRelPtup->Divide(missJECRelPtup);
      htempUnfJECRelPtup->Write();
      pTjet_smeared->Divide(fakeJECRelPtup);

      pTjet_smeared->Multiply(fakeJECRelBalup);
      RooUnfoldBayes unfoldInvertJECRelBalup(responseJECRelBalup, pTjet_smeared, 4); 
      TH1D* hunfJECRelBalup = (TH1D*) unfoldInvertJECRelBalup.Hreco(errorTreatment);
      TH1D* htempUnfJECRelBalup = (TH1D*) hunfJECRelBalup->Clone("htempUnf");
      htempUnfJECRelBalup->SetName(Form("unfoldingSolutionJECRelBalup_%d", iter));
      htempUnfJECRelBalup->Divide(missJECRelBalup);
      htempUnfJECRelBalup->Write();
      pTjet_smeared->Divide(fakeJECRelBalup);

      pTjet_smeared->Multiply(fakeJECRelJERup);
      RooUnfoldBayes unfoldInvertJECRelJERup(responseJECRelJERup, pTjet_smeared, 4); 
      TH1D* hunfJECRelJERup = (TH1D*) unfoldInvertJECRelJERup.Hreco(errorTreatment);
      TH1D* htempUnfJECRelJERup = (TH1D*) hunfJECRelJERup->Clone("htempUnf");
      htempUnfJECRelJERup->SetName(Form("unfoldingSolutionJECRelJERup_%d", iter));
      htempUnfJECRelJERup->Divide(missJECRelJERup);
      htempUnfJECRelJERup->Write();
      pTjet_smeared->Divide(fakeJECRelJERup);

      pTjet_smeared->Multiply(fakeJECL3Resup);
      RooUnfoldBayes unfoldInvertJECL3Resup(responseJECL3Resup, pTjet_smeared, 4); 
      TH1D* hunfJECL3Resup = (TH1D*) unfoldInvertJECL3Resup.Hreco(errorTreatment);
      TH1D* htempUnfJECL3Resup = (TH1D*) hunfJECL3Resup->Clone("htempUnf");
      htempUnfJECL3Resup->SetName(Form("unfoldingSolutionJECL3Resup_%d", iter));
      htempUnfJECL3Resup->Divide(missJECL3Resup);
      htempUnfJECL3Resup->Write();
      pTjet_smeared->Divide(fakeJECL3Resup);

       pTjet_smeared->Multiply(fakeJECMCdown);
      RooUnfoldBayes unfoldInvertJECMCdown(responseJECMCdown, pTjet_smeared, 4); 
      TH1D* hunfJECMCdown = (TH1D*) unfoldInvertJECMCdown.Hreco(errorTreatment);
      TH1D* htempUnfJECMCdown = (TH1D*) hunfJECMCdown->Clone("htempUnf");
      htempUnfJECMCdown->SetName(Form("unfoldingSolutionJECMCdown_%d", iter));
      htempUnfJECMCdown->Divide(missJECMCdown);
      htempUnfJECMCdown->Write();
      pTjet_smeared->Divide(fakeJECMCdown);

      pTjet_smeared->Multiply(fakeJECFragdown);
      RooUnfoldBayes unfoldInvertJECFragdown(responseJECFragdown, pTjet_smeared, 4); 
      TH1D* hunfJECFragdown = (TH1D*) unfoldInvertJECFragdown.Hreco(errorTreatment);
      TH1D* htempUnfJECFragdown = (TH1D*) hunfJECFragdown->Clone("htempUnf");
      htempUnfJECFragdown->SetName(Form("unfoldingSolutionJECFragdown_%d", iter));
      htempUnfJECFragdown->Divide(missJECFragdown);
      htempUnfJECFragdown->Write();
      pTjet_smeared->Divide(fakeJECFragdown);

      pTjet_smeared->Multiply(fakeJECFlavordown);
      RooUnfoldBayes unfoldInvertJECFlavordown(responseJECFlavordown, pTjet_smeared, 4); 
      TH1D* hunfJECFlavordown = (TH1D*) unfoldInvertJECFlavordown.Hreco(errorTreatment);
      TH1D* htempUnfJECFlavordown = (TH1D*) hunfJECFlavordown->Clone("htempUnf");
      htempUnfJECFlavordown->SetName(Form("unfoldingSolutionJECFlavordown_%d", iter));
      htempUnfJECFlavordown->Divide(missJECFlavordown);
      htempUnfJECFlavordown->Write();
      pTjet_smeared->Divide(fakeJECFlavordown);

      pTjet_smeared->Multiply(fakeJECAbsStatdown);
      RooUnfoldBayes unfoldInvertJECAbsStatdown(responseJECAbsStatdown, pTjet_smeared, 4); 
      TH1D* hunfJECAbsStatdown = (TH1D*) unfoldInvertJECAbsStatdown.Hreco(errorTreatment);
      TH1D* htempUnfJECAbsStatdown = (TH1D*) hunfJECAbsStatdown->Clone("htempUnf");
      htempUnfJECAbsStatdown->SetName(Form("unfoldingSolutionJECAbsStatdown_%d", iter));
      htempUnfJECAbsStatdown->Divide(missJECAbsStatdown);
      htempUnfJECAbsStatdown->Write();
      pTjet_smeared->Divide(fakeJECAbsStatdown);

      pTjet_smeared->Multiply(fakeJECAbsScaledown);
      RooUnfoldBayes unfoldInvertJECAbsScaledown(responseJECAbsScaledown, pTjet_smeared, 4); 
      TH1D* hunfJECAbsScaledown = (TH1D*) unfoldInvertJECAbsScaledown.Hreco(errorTreatment);
      TH1D* htempUnfJECAbsScaledown = (TH1D*) hunfJECAbsScaledown->Clone("htempUnf");
      htempUnfJECAbsScaledown->SetName(Form("unfoldingSolutionJECAbsScaledown_%d", iter));
      htempUnfJECAbsScaledown->Divide(missJECAbsScaledown);
      htempUnfJECAbsScaledown->Write();
      pTjet_smeared->Divide(fakeJECAbsScaledown);

      pTjet_smeared->Multiply(fakeJECAbsMPFdown);
      RooUnfoldBayes unfoldInvertJECAbsMPFdown(responseJECAbsMPFdown, pTjet_smeared, 4); 
      TH1D* hunfJECAbsMPFdown = (TH1D*) unfoldInvertJECAbsMPFdown.Hreco(errorTreatment);
      TH1D* htempUnfJECAbsMPFdown = (TH1D*) hunfJECAbsMPFdown->Clone("htempUnf");
      htempUnfJECAbsMPFdown->SetName(Form("unfoldingSolutionJECAbsMPFdown_%d", iter));
      htempUnfJECAbsMPFdown->Divide(missJECAbsMPFdown);
      htempUnfJECAbsMPFdown->Write();
      pTjet_smeared->Divide(fakeJECAbsMPFdown);

      pTjet_smeared->Multiply(fakeJECECALdown);
      RooUnfoldBayes unfoldInvertJECECALdown(responseJECECALdown, pTjet_smeared, 4); 
      TH1D* hunfJECECALdown = (TH1D*) unfoldInvertJECECALdown.Hreco(errorTreatment);
      TH1D* htempUnfJECECALdown = (TH1D*) hunfJECECALdown->Clone("htempUnf");
      htempUnfJECECALdown->SetName(Form("unfoldingSolutionJECECALdown_%d", iter));
      htempUnfJECECALdown->Divide(missJECECALdown);
      htempUnfJECECALdown->Write();
      pTjet_smeared->Divide(fakeJECECALdown);

      pTjet_smeared->Multiply(fakeJECHCALdown);
      RooUnfoldBayes unfoldInvertJECHCALdown(responseJECHCALdown, pTjet_smeared, 4); 
      TH1D* hunfJECHCALdown = (TH1D*) unfoldInvertJECHCALdown.Hreco(errorTreatment);
      TH1D* htempUnfJECHCALdown = (TH1D*) hunfJECHCALdown->Clone("htempUnf");
      htempUnfJECHCALdown->SetName(Form("unfoldingSolutionJECHCALdown_%d", iter));
      htempUnfJECHCALdown->Divide(missJECHCALdown);
      htempUnfJECHCALdown->Write();
      pTjet_smeared->Divide(fakeJECHCALdown);

      pTjet_smeared->Multiply(fakeJECRelStatdown);
      RooUnfoldBayes unfoldInvertJECRelStatdown(responseJECRelStatdown, pTjet_smeared, 4); 
      TH1D* hunfJECRelStatdown = (TH1D*) unfoldInvertJECRelStatdown.Hreco(errorTreatment);
      TH1D* htempUnfJECRelStatdown = (TH1D*) hunfJECRelStatdown->Clone("htempUnf");
      htempUnfJECRelStatdown->SetName(Form("unfoldingSolutionJECRelStatdown_%d", iter));
      htempUnfJECRelStatdown->Divide(missJECRelStatdown);
      htempUnfJECRelStatdown->Write();
      pTjet_smeared->Divide(fakeJECRelStatdown);

      pTjet_smeared->Multiply(fakeJECRelPtdown);
      RooUnfoldBayes unfoldInvertJECRelPtdown(responseJECRelPtdown, pTjet_smeared, 4); 
      TH1D* hunfJECRelPtdown = (TH1D*) unfoldInvertJECRelPtdown.Hreco(errorTreatment);
      TH1D* htempUnfJECRelPtdown = (TH1D*) hunfJECRelPtdown->Clone("htempUnf");
      htempUnfJECRelPtdown->SetName(Form("unfoldingSolutionJECRelPtdown_%d", iter));
      htempUnfJECRelPtdown->Divide(missJECRelPtdown);
      htempUnfJECRelPtdown->Write();
      pTjet_smeared->Divide(fakeJECRelPtdown);

      pTjet_smeared->Multiply(fakeJECRelBaldown);
      RooUnfoldBayes unfoldInvertJECRelBaldown(responseJECRelBaldown, pTjet_smeared, 4); 
      TH1D* hunfJECRelBaldown = (TH1D*) unfoldInvertJECRelBaldown.Hreco(errorTreatment);
      TH1D* htempUnfJECRelBaldown = (TH1D*) hunfJECRelBaldown->Clone("htempUnf");
      htempUnfJECRelBaldown->SetName(Form("unfoldingSolutionJECRelBaldown_%d", iter));
      htempUnfJECRelBaldown->Divide(missJECRelBaldown);
      htempUnfJECRelBaldown->Write();
      pTjet_smeared->Divide(fakeJECRelBaldown);

      pTjet_smeared->Multiply(fakeJECRelJERdown);
      RooUnfoldBayes unfoldInvertJECRelJERdown(responseJECRelJERdown, pTjet_smeared, 4); 
      TH1D* hunfJECRelJERdown = (TH1D*) unfoldInvertJECRelJERdown.Hreco(errorTreatment);
      TH1D* htempUnfJECRelJERdown = (TH1D*) hunfJECRelJERdown->Clone("htempUnf");
      htempUnfJECRelJERdown->SetName(Form("unfoldingSolutionJECRelJERdown_%d", iter));
      htempUnfJECRelJERdown->Divide(missJECRelJERdown);
      htempUnfJECRelJERdown->Write();
      pTjet_smeared->Divide(fakeJECRelJERdown);

      pTjet_smeared->Multiply(fakeJECL3Resdown);
      RooUnfoldBayes unfoldInvertJECL3Resdown(responseJECL3Resdown, pTjet_smeared, 4); 
      TH1D* hunfJECL3Resdown = (TH1D*) unfoldInvertJECL3Resdown.Hreco(errorTreatment);
      TH1D* htempUnfJECL3Resdown = (TH1D*) hunfJECL3Resdown->Clone("htempUnf");
      htempUnfJECL3Resdown->SetName(Form("unfoldingSolutionJECL3Resdown_%d", iter));
      htempUnfJECL3Resdown->Divide(missJECL3Resdown);
      htempUnfJECL3Resdown->Write();
      pTjet_smeared->Divide(fakeJECL3Resdown);

/*      pTjet_smeared->Multiply(fakeJECdown);
      RooUnfoldBayes   unfoldInvertJECdown(responseJECdown, pTjet_smeared , 4); 
      TH1D* hunfJECdown= (TH1D*) unfoldInvertJECdown.Hreco(errorTreatment);
      TH1D *htempUnfJECdown=(TH1D*)hunfJECdown->Clone("htempUnf");
      htempUnfJECdown->SetName(Form("unfoldingSolutionJECdown_%d",iter));
      htempUnfJECdown->Divide(missJECdown);
      htempUnfJECdown->Write();
      pTjet_smeared->Divide(fakeJECdown);*/

     }
          fout->Close();	   
}

#ifndef __CINT__
int main () { RooUnfoldSystUncJESdecomp(); return 0; }  // Main program when run stand-alone
#endif
