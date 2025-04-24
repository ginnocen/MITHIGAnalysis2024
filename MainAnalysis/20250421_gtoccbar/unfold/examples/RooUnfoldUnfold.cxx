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

void RooUnfoldUnfold(std::string file_mc="/home/cbaldene/Unfolding/RooUnfoldSetupForLuis/examples/response2.root", 
std::string file_data="/home/cbaldene/Unfolding/RooUnfoldSetupForLuis/examples/response2.root", std::string date = "response",int flag=19)
{
 /* RooUnfold::ErrorTreatment errorTreatment = RooUnfold::kCovariance;
  TFile* f_mc=new TFile(file_mc.c_str());
  TFile* f_data=new TFile(file_data.c_str());
  TH1D *pTjet_smeared=(TH1D*)f_mc->Get("pTjet_reco");
  pTjet_smeared->SetName("pTjet_smeared");

  TH1D *pTjet_matchedReco=(TH1D*)f_mc->Get("pTjet_matchedReco");

  TH1D *pTjet_true=(TH1D*)f_mc->Get("pTjet_true");
  pTjet_true->SetName("pTjet_true");
  RooUnfoldResponse* response = (RooUnfoldResponse*)f_mc->Get("pTjet_reco_pTjet_true");
*/
 	RooUnfold::ErrorTreatment errorTreatment = RooUnfold::kCovariance;   
  TFile* f_mc=new TFile(file_mc.c_str());
  TFile* f_data=new TFile(file_data.c_str());
  TH1D *pTjet_smeared=(TH1D*)f_mc->Get("pTjet_recoTest");
  pTjet_smeared->SetName("pTjet_smeared");


  TH1D *fake=(TH1D*)f_mc->Get("fake");
  TH1D *miss=(TH1D*)f_mc->Get("miss");
  pTjet_smeared->Multiply(fake);
  TH1D *pTjet_matchedReco=(TH1D*)f_mc->Get("pTjet_matchedRecoTest");

  TH1D *pTjet_true=(TH1D*)f_mc->Get("pTjet_trueTest");
  pTjet_true->SetName("pTjet_true");

  RooUnfoldResponse* response = (RooUnfoldResponse*)f_mc->Get("pTjet_recoTest_pTjet_trueTest");

      TDecompSVD *svd= new TDecompSVD (response->Mresponse()); //this is the singular value decomposition (SVD) matrix
      auto singular_values = svd->GetSig(); //this is a vector with the singular values, i.e., the diagonal elements of S. They are ordered from largest to smallest.

      svd->Print();

  TFile* fout = new TFile("output.root", "recreate");


     for(int jar=0;jar<20;jar++)
     {
      Int_t iter=jar;
      cout<<"iteration"<<iter<<endl;
      cout<<"==============Unfold h1====================="<<endl;
      RooUnfoldInvert   unfoldInvert(response, pTjet_smeared); 
      RooUnfoldBayes   unfold(response, pTjet_smeared, iter); 

          if (iter == 0)
	  {
           TH1D* hunf= (TH1D*) unfoldInvert.Hreco(errorTreatment);
          //FOLD BACK
          TH1D* hfold= (TH1D*)response->ApplyToTruth(hunf,"");
          TH1D *htempUnf=(TH1D*)hunf->Clone("htempUnf");
	  htempUnf->Divide(miss);
     	  htempUnf->SetName(Form("Bayesian_Unfoldediter%d.root",iter));
           
     	  TH1D *htempFold=(TH1D*)hfold->Clone("htempFold");          
     	  htempFold->SetName(Form("Bayesian_Foldediter%d.root",iter));       
	  htempUnf->Divide(pTjet_true);
          htempUnf->Write();
	  htempFold->Sumw2();
	  htempUnf->Delete();
//	  htempFold->Divide(fake);
	  htempFold->Divide(pTjet_smeared);
     	  htempFold->Write();
	  htempFold->Delete();
         }

          if (iter > 0)
	  {
           TH1D* hunf= (TH1D*) unfold.Hreco(errorTreatment);
          //FOLD BACK
          TH1D* hfold= (TH1D*)response->ApplyToTruth(hunf,"");
          TH1D *htempUnf=(TH1D*)hunf->Clone("htempUnf");
	  htempUnf->Divide(miss);
     	  htempUnf->SetName(Form("Bayesian_Unfoldediter%d.root",iter)); 
     	  TH1D *htempFold=(TH1D*)hfold->Clone("htempFold"); 
     	  htempFold->SetName(Form("Bayesian_Foldediter%d.root",iter));       
	  htempUnf->Divide(pTjet_true);
          htempUnf->Write();
	  htempFold->Sumw2();
//	  htempFold->Divide(fake);
	  htempFold->Divide(pTjet_smeared);
     	  htempFold->Write();
         }
     }
          fout->Close();	    
}

#ifndef __CINT__
int main () { RooUnfoldUnfold(); return 0; }  // Main program when run stand-alone
#endif
