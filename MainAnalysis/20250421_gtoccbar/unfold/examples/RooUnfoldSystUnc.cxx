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

void RooUnfoldSystUnc(std::string file_mc="/home/cbaldene/Unfolding/RooUnfoldSetupForLuis/examples/response3.root", 
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

     for(int jar=0;jar<15;jar++)
     {
      Int_t iter=jar;
      cout<<"iteration"<<iter<<endl;
      cout<<"==============Unfold h1====================="<<endl;
      char ptbinsRead[100];
      sprintf(ptbinsRead, "_%d", jar);
      string histnameRead(ptbinsRead);
      TH1F* pTjet_smeared = histosTH1F[("jetPtDimuon"+histnameRead).c_str()];
      pTjet_smeared->Multiply(fake);

      RooUnfoldBayes   unfoldInvert(response, pTjet_smeared , 4); 
//      RooUnfoldInvert   unfoldInvert(response, histosTH1F["jetPt_0"] );
      TH1D* hunf= (TH1D*) unfoldInvert.Hreco(errorTreatment);
//      TH1D* hfold= (TH1D*)response->ApplyToTruth(hunf,"");
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

      pTjet_smeared->Multiply(fakeJECdown);
      RooUnfoldBayes   unfoldInvertJECdown(responseJECdown, pTjet_smeared , 4); 
      TH1D* hunfJECdown= (TH1D*) unfoldInvertJECdown.Hreco(errorTreatment);
      TH1D *htempUnfJECdown=(TH1D*)hunfJECdown->Clone("htempUnf");
      htempUnfJECdown->SetName(Form("unfoldingSolutionJECdown_%d",iter));
      htempUnfJECdown->Divide(missJECdown);
      htempUnfJECdown->Write();
      pTjet_smeared->Divide(fakeJECdown);

     }
          fout->Close();	   
}

#ifndef __CINT__
int main () { RooUnfoldSystUnc(); return 0; }  // Main program when run stand-alone
#endif
