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

    // open the files
    TFile* inputFile = TFile::Open(input.at(0).c_str());
    inputFile->ls();
    
    TDirectoryFile* paramDirectory = (TDirectoryFile*)inputFile->Get("par");
    TH1D* hTrigChoice = (TH1D*)paramDirectory->Get("parTriggerChoice");
    TH1D* hMinJetPt = (TH1D*)paramDirectory->Get("parMinJetPT");
    TH1D* hMaxJetPt = (TH1D*)paramDirectory->Get("parMaxJetPT");
    TH1D* hMaxJetY = (TH1D*)paramDirectory->Get("parMaxJetY");
    TH1D* hMinD0Pt = (TH1D*)paramDirectory->Get("parMinDzeroPT");
    TH1D* hMaxD0Y  = (TH1D*)paramDirectory->Get("parMaxDzeroY");
    TH1D* hIsData = (TH1D*)paramDirectory->Get("parIsData");
    
    TFile* inputFileMC = TFile::Open(input.at(1).c_str());
    inputFileMC->ls();

    // draw the histograms
    TLatex* cms; 
    if(hIsData->GetBinContent(1) == 0) cms = new TLatex(0.12,0.92,"#bf{CMS} #it{Simulation} PYTHIA8 #gammaN (5.36 TeV)");
    else cms = new TLatex(0.12,0.92,"#bf{CMS} UPC PbPb (5.36 TeV)");
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

    TLegend* leg = new TLegend(0.16, 0.7, 0.5, 0.85);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->SetTextFont(42);
    leg->SetTextSize(0.035);
    
    TLegend* legDataOnly = new TLegend(0.16, 0.7, 0.5, 0.85);
    legDataOnly->SetBorderSize(0);
    legDataOnly->SetFillStyle(0);
    legDataOnly->SetTextFont(42);
    legDataOnly->SetTextSize(0.035);

  
    TLegend* legDijet = new TLegend(0.16, 0.7, 0.5, 0.85);
    legDijet->SetBorderSize(0);
    legDijet->SetFillStyle(0);
    legDijet->SetTextFont(42);
    legDijet->SetTextSize(0.035);


    TLegend* leg2 = new TLegend(0.16, 0.7, 0.5, 0.85);
    leg2->SetBorderSize(0);
    leg2->SetFillStyle(0);
    leg2->SetTextFont(42);
    leg2->SetTextSize(0.035);
    
    TLegend* legDataMC = new TLegend(0.56, 0.7, 0.8, 0.85);
    legDataMC->SetBorderSize(0);
    legDataMC->SetFillStyle(0);
    legDataMC->SetTextFont(42);
    legDataMC->SetTextSize(0.035);

    static vector<int> Colors = GetCVDColors6();


    //--------------------------------
    // loop over all of the input files
    // hJetPt
    //-------------------------------

    // get the histograms
    TH1D* hNev = (TH1D*)inputFile->Get("hNev");
    hNev->SetName(Form("hNev_%d", 0));
    int nEvents = hNev->GetBinContent(1);

    TH1D* hEECInclusive = (TH1D*)inputFile->Get("hEECInclusive");
    hEECInclusive->SetName(Form("hEECInclusive_%d", 0));

    TH1D* hEEC_D0Tagged = (TH1D*)inputFile->Get("hEEC_D0Tagged");
    hEEC_D0Tagged->SetName(Form("hEEC_D0Tagged_%d", 0));


    TH1D* hEECInclusiveMC = (TH1D*)inputFileMC->Get("hEECInclusive");
    hEECInclusiveMC->SetName(Form("hEECInclusiveMC_%d", 0));

    TH1D* hEEC_D0TaggedMC = (TH1D*)inputFileMC->Get("hEEC_D0Tagged");
    hEEC_D0TaggedMC->SetName(Form("hEEC_D0TaggedMC_%d", 0));

    TH1D* hEECInclusiveDijet = (TH1D*)inputFile->Get("hEECInclusiveDijet");
    hEECInclusiveDijet->SetName(Form("hEECInclusiveDijet_%d", 0));

    TH1D* hEEC_D0TaggedDijet = (TH1D*)inputFile->Get("hEEC_D0TaggedDijet");
    hEEC_D0TaggedDijet->SetName(Form("hEEC_D0TaggedDijet_%d", 0));
    
    TH1D* hEventSelData = (TH1D*)inputFile->Get("hEventSel"); 
    hEventSelData->SetName("hEventSelData"); 
    
    TH1D* hEventSelMC = (TH1D*)inputFileMC->Get("hEventSel"); 
    hEventSelMC->SetName("hEventSelMC"); 
    
    TH1D* hJetPtData = (TH1D*)inputFile->Get("hJetPt"); 
    hJetPtData->SetName("hJetPtData"); 
    
    TH1D* hJetPtMC = (TH1D*)inputFileMC->Get("hJetPt"); 
    hJetPtMC->SetName("hJetPtMC"); 
    
       
    TH1D* hD0JetPtData = (TH1D*)inputFile->Get("hDOJetPt"); 
    hD0JetPtData->SetName("hD0JetPtData"); 
    
    TH1D* hD0JetPtMC = (TH1D*)inputFileMC->Get("hDOJetPt"); 
    hD0JetPtMC->SetName("hD0JetPtMC"); 

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
    leg->AddEntry(hEECInclusiveMC, "Monte Carlo"); 
    leg->AddEntry(hEEC_D0Tagged, Form("#gammaN Events w/ a D0 jet,  #it{p}_{T, jet}  > %0.0f GeV", hMinJetPt->GetBinContent(1))); 
    leg->AddEntry(hEEC_D0TaggedMC, "Monte Carlo"); 

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
    
    hEECInclusiveMC->Scale(1.0/hEECInclusiveMC->Integral(), "width"); 
    hEECInclusiveMC->GetYaxis()->SetRangeUser(0.05, 5);
    hEECInclusiveMC->SetMarkerColor(Colors[0]); 
    hEECInclusiveMC->SetLineColor(Colors[0]); 
    hEECInclusiveMC->SetLineWidth(2); 
    hEECInclusiveMC->SetMarkerStyle(24); 

    hEEC_D0TaggedMC->Scale(1.0/hEEC_D0TaggedMC->Integral(), "width"); 
    hEEC_D0TaggedMC->SetMarkerColor(Colors[1]); 
    hEEC_D0TaggedMC->SetLineColor(Colors[1]); 
    hEEC_D0TaggedMC->SetLineWidth(2); 
    hEEC_D0TaggedMC->SetMarkerStyle(24); 
    hEECInclusive->GetXaxis()->SetTitle("#Delta#it{R}"); 
    hEECInclusive->GetYaxis()->SetTitle("Normalized E2C"); 

    hEECInclusive->Draw(); 
    hEEC_D0Tagged->Draw("same"); 
    hEECInclusiveMC->Draw("same"); 
    hEEC_D0TaggedMC->Draw("same"); 
    cms->Draw(); 
    //TrigString->Draw(); 
    leg->Draw(); 
    // JetString->Draw(); 
    // D0pTString->Draw(); 
    c->SaveAs(Form("EECDataMCComparison_%s.pdf", tag.c_str()));
    
    
    TCanvas* c1 = new TCanvas("c1", "c1", 600, 600);
    c1->SetTickx(1);
    c1->SetTicky(1);
    c1->SetLogx(); 
    c1->SetLogy(); 
    c1->SetRightMargin(0.05);
    c1->SetLeftMargin(0.13);
    
    legDataOnly->AddEntry(hEECInclusive, Form("#gammaN Events w/ a jet, #it{p}_{T, jet}  > %0.0f GeV", hMinJetPt->GetBinContent(1) )); 
    legDataOnly->AddEntry(hEEC_D0Tagged, Form("#gammaN Events w/ a D0 jet,  #it{p}_{T, jet}  > %0.0f GeV", hMinJetPt->GetBinContent(1))); 
    
    hEECInclusive->Draw(); 
    hEEC_D0Tagged->Draw("same"); 
    cms->Draw(); 
    legDataOnly->Draw(); 
    c1->SaveAs(Form("EECDataOnly_%s.pdf", tag.c_str())); 
    
    TCanvas* c2 = new TCanvas("c2", "c2", 600, 600);
    c2->SetTickx(1);
    c2->SetTicky(1);
    c2->SetLogx(); 
    c2->SetLogy(); 
    c2->SetRightMargin(0.05);
    c2->SetLeftMargin(0.13);
    
    legDijet->AddEntry(hEECInclusive, Form("#gammaN Events w/ a jet, #it{p}_{T, jet}  > %0.0f GeV", hMinJetPt->GetBinContent(1) )); 
    legDijet->AddEntry(hEECInclusiveDijet, "w/ Dijet Selection"); 
    legDijet->AddEntry(hEEC_D0Tagged, Form("#gammaN Events w/ a D0 jet,  #it{p}_{T, jet}  > %0.0f GeV", hMinJetPt->GetBinContent(1))); 
    legDijet->AddEntry(hEEC_D0TaggedDijet, "w/ Dijet Selection"); 
    
    hEECInclusiveDijet->Scale(1.0/hEECInclusiveDijet->Integral(), "width"); 
    hEECInclusiveDijet->GetYaxis()->SetRangeUser(0.05, 5);
    hEECInclusiveDijet->SetMarkerColor(Colors[0]); 
    hEECInclusiveDijet->SetLineColor(Colors[0]); 
    hEECInclusiveDijet->SetLineWidth(2); 
    hEECInclusiveDijet->SetMarkerStyle(21); 

    hEEC_D0TaggedDijet->Scale(1.0/hEEC_D0TaggedDijet->Integral(), "width"); 
    hEEC_D0TaggedDijet->SetMarkerColor(Colors[1]); 
    hEEC_D0TaggedDijet->SetLineColor(Colors[1]); 
    hEEC_D0TaggedDijet->SetLineWidth(2); 
    hEEC_D0TaggedDijet->SetMarkerStyle(21); 
    
    hEECInclusive->Draw(); 
    hEEC_D0Tagged->Draw("same");
    hEECInclusiveDijet->Draw("same");
    hEEC_D0TaggedDijet->Draw("same");
    cms->Draw(); 
    legDijet->Draw(); 
    c2->SaveAs(Form("EECDijetFirstLook_%s.pdf", tag.c_str()));
    
    TCanvas* c3 = new TCanvas("c3", "c3", 600, 600);
    c3->SetTickx(1);
    c3->SetTicky(1);
    c3->SetLogy(); 
    c3->SetRightMargin(0.05);
    c3->SetLeftMargin(0.13);
    hEventSelData->Scale(1./hEventSelData->GetBinContent(1)); 
    hEventSelMC->Scale(1./hEventSelMC->GetBinContent(1)); 
    hEventSelMC->SetLineColor(kRed);
    hEventSelData->Draw("TEXT0 HIST");     
    hEventSelMC->Draw("TEXT0 HIST same"); 
    cms->Draw(); 
    c3->SaveAs(Form("EventSelection_%s.pdf", tag.c_str())); 
    
    TCanvas* c4 = new TCanvas("c4", "c4", 600, 600);
    c4->SetTickx(1);
    c4->SetTicky(1);
    c4->SetLogy(); 
    c4->SetRightMargin(0.05);
    c4->SetLeftMargin(0.13);
    
    hJetPtData->Scale(1.0/hJetPtData->Integral(), "width"); 
    hJetPtData->SetMarkerColor(Colors[0]); 
    hJetPtData->SetLineColor(Colors[0]); 
    hJetPtData->SetLineWidth(2); 
    hJetPtData->SetMarkerStyle(20); 
    
    hJetPtMC->Scale(1.0/hJetPtMC->Integral(), "width"); 
    hJetPtMC->SetMarkerColor(Colors[1]); 
    hJetPtMC->SetLineColor(Colors[1]); 
    hJetPtMC->SetLineWidth(2); 
    hJetPtMC->SetMarkerStyle(20); 
    
    hD0JetPtData->Scale(1.0/hD0JetPtData->Integral(), "width"); 
    hD0JetPtData->SetMarkerColor(Colors[0]); 
    hD0JetPtData->SetLineColor(Colors[0]); 
    hD0JetPtData->SetLineWidth(2); 
    hD0JetPtData->SetMarkerStyle(24); 
    
    hD0JetPtMC->Scale(1.0/hD0JetPtMC->Integral(), "width"); 
    hD0JetPtMC->SetMarkerColor(Colors[1]); 
    hD0JetPtMC->SetLineColor(Colors[1]); 
    hD0JetPtMC->SetLineWidth(2); 
    hD0JetPtMC->SetMarkerStyle(24); 
    
    legDataMC->AddEntry(hJetPtData, "Inclusive Jets Data");
    legDataMC->AddEntry(hJetPtMC, "Inclusive Jets MC");
    // legDataMC->AddEntry(hD0JetPtData, "D^{0} Jets Data");
    // legDataMC->AddEntry(hD0JetPtMC, "D^{0} Jets MC");
    hJetPtData->GetXaxis()->SetTitle("p_{T, jet} (GeV)"); 
    hJetPtData->GetXaxis()->SetTitleOffset(1.1); 
    hJetPtData->GetYaxis()->SetTitle("Area and Bin Width Norm.");
    hJetPtData->Draw("same");
    hJetPtMC->Draw("same"); 
    // hD0JetPtData->Draw("same");
    // hD0JetPtMC->Draw("same"); 
    legDataMC->Draw(); 
    cms->Draw(); 
    c4->SaveAs(Form("JetPtSpectrum_%s.pdf", tag.c_str() ));
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
