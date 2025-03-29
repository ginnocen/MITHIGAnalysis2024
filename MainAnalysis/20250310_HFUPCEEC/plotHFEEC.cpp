#include <TCanvas.h>
#include <TCut.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TLegend.h>
#include <TNtuple.h>
#include <TTree.h>
#include <TStyle.h>
#include <TLatex.h>
#include <TGraphAsymmErrors.h>
#include <TLine.h>
#include <TF1.h>
#include <TProfile.h>

#include <iostream>
#include <string>

using namespace std;
#include "CommandLine.h" // Yi's Commandline bundle
#include "Messenger.h"   // Yi's Messengers for reading data files
#include "SetStyle.h"
#include "ProgressBar.h" // Yi's fish progress bar
#include "helpMessage.h" // Print out help message
#include "parameter.h"   // The parameters used in the analysis
#include "utilities.h"   // Yen-Jie's random utility functions


void plotHFJetPerf(std::vector<string> input, string tag){

    // input files
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);


    // draw the histograms
    TLatex* cms = new TLatex(0.12,0.92,"#bf{CMS} #it{Simulation} PYTHIA8 #gammaN (5.36 TeV)");
    cms->SetNDC();
    cms->SetTextSize(0.05);
    cms->SetTextFont(42);



    // =====================================================
    // Jet pT distributions
    // =====================================================
    TCanvas* c = new TCanvas("c", "c", 600, 600);
    c->SetTickx(1);
    c->SetTicky(1);
    c->SetLogx(); 
    c->SetLogy(); 
    c->SetRightMargin(0.05);
    c->SetLeftMargin(0.13);

    TLegend* leg = new TLegend(0.16, 0.75, 0.5, 0.85);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->SetTextFont(42);
    leg->SetTextSize(0.035);


    TLegend* leg2 = new TLegend(0.16, 0.75, 0.5, 0.85);
    leg2->SetBorderSize(0);
    leg2->SetFillStyle(0);
    leg2->SetTextFont(42);
    leg2->SetTextSize(0.035);

    static vector<int> Colors = GetCVDColors6();


    //--------------------------------
    // loop over all of the input files
    // hJetPt
    //-------------------------------
    // open the files
    TFile* inputFile = TFile::Open(input.at(0).c_str());
    inputFile->ls();
    // get the histograms
    TH1D* hNev = (TH1D*)inputFile->Get("hNev");
    hNev->SetName(Form("hNev_%d", 0));
    int nEvents = hNev->GetBinContent(1);

    TH1D* hEECInclusive = (TH1D*)inputFile->Get("hEECInclusive");
    hEECInclusive->SetName(Form("hEECInclusive_%d", 0));

    TH1D* hEEC_D0Tagged = (TH1D*)inputFile->Get("hEEC_D0Tagged");
    hEEC_D0Tagged->SetName(Form("hEEC_D0Tagged_%d", 0));



    TDirectoryFile* paramDirectory = (TDirectoryFile*)inputFile->Get("par");
    TH1D* hTrigChoice = (TH1D*)paramDirectory->Get("parTriggerChoice");
    TH1D* hMinJetPt = (TH1D*)paramDirectory->Get("parMinJetPT");
    TH1D* hMaxJetPt = (TH1D*)paramDirectory->Get("parMaxJetPT");
    TH1D* hMaxJetY = (TH1D*)paramDirectory->Get("parMaxJetY");
    TH1D* hMinD0Pt = (TH1D*)paramDirectory->Get("parMinDzeroPT");
    TH1D* hMaxD0Y  = (TH1D*)paramDirectory->Get("parMaxDzeroY");
    TH1D* hIsData = (TH1D*)paramDirectory->Get("parIsData");

    TLatex* D0pTString = new TLatex(0.4,0.16,Form("D^{0} #it{p}_{T} > %0.0f GeV (D^{0}-Tagged Jets Only) ", hMinD0Pt->GetBinContent(1)));
    D0pTString->SetNDC();
    D0pTString->SetTextSize(0.035);
    D0pTString->SetTextFont(42);



    TLatex* JetString = new TLatex(0.57,0.85,Form("#it{p}^{ref}_{T, jet} > %0.0f GeV, |y_{jet}| <  %0.0f ", hMinJetPt->GetBinContent(1), hMaxJetY->GetBinContent(1)));
    JetString->SetNDC();
    JetString->SetTextSize(0.035);
    JetString->SetTextFont(42);

    TLatex* RecoJetString = new TLatex(0.57,0.85,Form("#it{p}_{T, jet} > %0.0f GeV, |y_{jet}| <  %0.0f ", hMinJetPt->GetBinContent(1), hMaxJetY->GetBinContent(1)));
    RecoJetString->SetNDC();
    RecoJetString->SetTextSize(0.035);
    RecoJetString->SetTextFont(42);



    TLatex* JetString2D = new TLatex(0.45,0.85,Form("#it{p}^{ref}_{T, jet} > %0.0f GeV, |y_{jet}| <  %0.0f ", hMinJetPt->GetBinContent(1), hMaxJetY->GetBinContent(1)));
    JetString2D->SetNDC();
    JetString2D->SetTextSize(0.035);
    JetString2D->SetTextFont(42);


    TLatex* RecoJetString2D = new TLatex(0.45,0.85,Form("#it{p}_{T, jet} > %0.0f GeV, |y_{jet}| <  %0.0f ", hMinJetPt->GetBinContent(1), hMaxJetY->GetBinContent(1)));
    RecoJetString2D->SetNDC();
    RecoJetString2D->SetTextSize(0.035);
    RecoJetString2D->SetTextFont(42);


    TLatex* TrigString;
    TLatex* TrigString2D;
    if(hTrigChoice->GetBinContent(1) == 0){
    TrigString = new TLatex(0.62,0.80,"No Trigger Selection");
    TrigString2D = new TLatex(0.55,0.80,"No Trigger Selection");
    }
    else if(hTrigChoice->GetBinContent(1) == 1){
    TrigString = new TLatex(0.65,0.80,"ZDC 1n OR");
    TrigString2D = new TLatex(0.58,0.80,"ZDC 1n OR");
    }
    else if(hTrigChoice->GetBinContent(1) == 2){
    TrigString = new TLatex(0.65,0.80,"ZDC XOR + Jet 8");
    TrigString2D = new TLatex(0.58,0.80,"ZDC XOR + Jet 8");
    }
    else{
    std::cout << "[Error]: Unrecognized trigger option " << hTrigChoice->GetBinContent(1) << std::endl;
    return;
    }

    TrigString->SetNDC();
    TrigString->SetTextSize(0.035);
    TrigString->SetTextFont(42);

    TrigString2D->SetNDC();
    TrigString2D->SetTextSize(0.035);
    TrigString2D->SetTextFont(42);
    
    // leg->AddEntry(hEECInclusive, Form("#gammaN Events w/ a jet, %0.0f < #it{p}_{T, jet}  < %0.0f GeV", hMinJetPt->GetBinContent(1),hMaxJetPt->GetBinContent(1) )); 
    // leg->AddEntry(hEEC_D0Tagged, Form("#gammaN Events w/ a D0 jet, %0.0f < #it{p}_{T, jet}  < %0.0f GeV", hMinJetPt->GetBinContent(1),hMaxJetPt->GetBinContent(1) )); 

    leg->AddEntry(hEECInclusive, Form("#gammaN Events w/ a jet, #it{p}_{T, jet}  > %0.0f GeV", hMinJetPt->GetBinContent(1) )); 
    leg->AddEntry(hEEC_D0Tagged, Form("#gammaN Events w/ a D0 jet,  #it{p}_{T, jet}  > %0.0f GeV", hMinJetPt->GetBinContent(1))); 

    hEECInclusive->Scale(1.0/hEECInclusive->Integral(), "width"); 
    hEECInclusive->GetYaxis()->SetRangeUser(0.05, 5);
    hEECInclusive->SetMarkerColor(Colors[0]); 
    hEECInclusive->SetLineColor(Colors[0]); 
    hEECInclusive->SetLineWidth(2); 
    hEECInclusive->SetMarkerStyle(20); 

    hEEC_D0Tagged->Scale(1.0/hEEC_D0Tagged->Integral(), "width"); 
    hEEC_D0Tagged->SetMarkerColor(Colors[1]); 
    hEEC_D0Tagged->SetLineColor(Colors[1]); 
    hEEC_D0Tagged->SetLineWidth(2); 
    hEEC_D0Tagged->SetMarkerStyle(20); 
    hEECInclusive->GetXaxis()->SetTitle("#Delta#it{R}"); 
    hEECInclusive->GetYaxis()->SetTitle("Normalized E2C"); 
    hEECInclusive->Draw(); 
    hEEC_D0Tagged->Draw("same"); 
    cms->Draw(); 
    //TrigString->Draw(); 
    leg->Draw(); 
    // JetString->Draw(); 
    // D0pTString->Draw(); 
    c->SaveAs("EECFirstLook.pdf");

    

}

//============================================================//
// Main analysis
//============================================================//
int main(int argc, char *argv[]) {
  if (printHelpMessage(argc, argv))
    return 0;
  CommandLine CL(argc, argv);
  // input to this step is the result of running
  std::vector<string> input = CL.GetStringVector("Input", ""); // Input files

  string tag = CL.Get("tag", "test");
  plotHFJetPerf(input, tag);
}
