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

#include <iostream>
#include <string>

using namespace std;
#include "CommandLine.h" // Yi's Commandline bundle
#include "Messenger.h"   // Yi's Messengers for reading data files
#include "ProgressBar.h" // Yi's fish progress bar
#include "helpMessage.h" // Print out help message
#include "parameter.h"   // The parameters used in the analysis
#include "utilities.h"   // Yen-Jie's random utility functions


// function to plot prompt, non-prompt Dzero results
void plotHFjet(string input, string tag){
    // input files
    TFile* inputFile = TFile::Open(input.c_str());

    // get the histograms
    TH1D* hNev = (TH1D*)inputFile->Get("hNev");
    TH1D* hJetPt = (TH1D*)inputFile->Get("hJetPt");
    TH1D* hD0JetPt = (TH1D*)inputFile->Get("hD0JetPt");
    TH1D* hD0FragFraction = (TH1D*)inputFile->Get("hD0FragFraction");
    TH1D* hDeltaPhiLeadingJetvLeadingD = (TH1D*)inputFile->Get("hDeltaPhiLeadingJetvLeadingD");
    TH1D* hDeltaPhiSubleadingJetvLeadingD = (TH1D*)inputFile->Get("hDeltaPhiSubleadingJetvLeadingD");
    TDirectoryFile* paramDirectory = (TDirectoryFile*)inputFile->Get("par");
    TH1D* hTrigChoice = (TH1D*)paramDirectory->Get("parTriggerChoice");
    TH1D* hMinJetPt = (TH1D*)paramDirectory->Get("parMinJetPT");
    TH1D* hMinD0pT  = (TH1D*)paramDirectory->Get("parMinDzeroPT");
    TH1D* hMaxJetY = (TH1D*)paramDirectory->Get("parMaxJetY");
    TH1D* hIsData = (TH1D*)paramDirectory->Get("parIsData");

    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);


    // draw the histograms
    // create strings for the parameters
    string cmsString;


    TLatex* cms;
    if(hIsData->GetBinContent(1) == 0)cms = new TLatex(0.12,0.92, " #bf{CMS} #it{Simulation} PYTHIA8 #gammaN (5.36 TeV)");
    else cms = new TLatex(0.12,0.92, "#bf{CMS} UPC PbPb 2023 (5.36 TeV)");
    cms->SetNDC();
    cms->SetTextSize(0.05);
    cms->SetTextFont(42);

    TLatex* TrigString;
    if(hTrigChoice->GetBinContent(1) == 0){
      TrigString = new TLatex(0.6,0.75,"No Trigger Selection");
    }
    else if(hTrigChoice->GetBinContent(1) == 1){
      TrigString = new TLatex(0.68,0.75,"ZDC 1n OR");
    }
    else if(hTrigChoice->GetBinContent(1) == 2){
      TrigString = new TLatex(0.68,0.75,"ZDC XOR + Jet 8");
    }
    else{
      std::cout << "[Error]: Unrecognized trigger option " << hTrigChoice->GetBinContent(1) << std::endl;
      return;
    }

    TrigString->SetNDC();
    TrigString->SetTextSize(0.035);
    TrigString->SetTextFont(42);

    TLatex* JetString = new TLatex(0.57,0.85,Form("p_{T, jet} > %0.0f GeV, |jet y| <  %0.0f ", hMinJetPt->GetBinContent(1), hMaxJetY->GetBinContent(1)));
    JetString->SetNDC();
    JetString->SetTextSize(0.035);
    JetString->SetTextFont(42);

    TLatex* D0string = new TLatex(0.57, 0.8, Form("p_{T, D^{0}} >  %0.0f GeV, |D^{0} y | < 2", hMinD0pT->GetBinContent(1)));
    D0string->SetNDC();
    D0string->SetTextSize(0.035);
    D0string->SetTextFont(42);

    // ------------------------------------------------------------
    // ------ Jet Pt Histogram ------------------------------------
    TCanvas* c = new TCanvas("c", "c", 600, 600);
    c->SetLogy();
    c->SetTickx(1);
    c->SetTicky(1);
    c->SetRightMargin(0.05);
    c->SetLeftMargin(0.13);

    TLegend* leg = new TLegend(0.5, 0.65, 0.9, 0.75);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->SetTextFont(42);
    leg->SetTextSize(0.035);
    leg->AddEntry(hJetPt, "Inclusive Jets");
    leg->AddEntry(hD0JetPt, "#it{D}^{0} Tagged Jets");


    hJetPt->SetMarkerStyle(20);
    hJetPt->SetMarkerColor(kBlack);
    hJetPt->SetLineColor(kBlack);

    hD0JetPt->SetMarkerStyle(20);
    hD0JetPt->SetMarkerColor(kBlue);
    hD0JetPt->SetLineColor(kBlue);
    hJetPt->GetXaxis()->SetTitle("#it{p}_{T, jet} (GeV)");
    hJetPt->GetYaxis()->SetTitle("1/#it{N}_{event}d#it{N}/d#it{p}_{T} (GeV)^{-1}");

    int nEvents = hNev->GetBinContent(1);
    hJetPt->Scale(1./nEvents, "width");
    hD0JetPt->Scale(1./nEvents, "width");
    hJetPt->GetYaxis()->SetRangeUser(1e-9, 1);

    hJetPt->Draw();
    hD0JetPt->Draw("same");
    leg->Draw("same");
    cms->Draw();
    TrigString->Draw();
    JetString->Draw();
    D0string->Draw();

    c->SaveAs(Form("jetpt_%s.pdf", tag.c_str()));
    // ------------------------------------------------------------


    // ------------------------------------------------------------
    // delta phi distributions
    // ------------------------------------------------------------
    TCanvas* c2 = new TCanvas("c2", "c2", 600, 600);
    c2->SetLogy();
    c2->SetTickx(1);
    c2->SetTicky(1);
    c2->SetRightMargin(0.05);
    c2->SetLeftMargin(0.13);

    TLegend* leg2 = new TLegend(0.5, 0.65, 0.9, 0.75);
    leg2->SetBorderSize(0);
    leg2->SetFillStyle(0);
    leg2->SetTextFont(42);
    leg2->SetTextSize(0.035);
    leg2->AddEntry(hDeltaPhiLeadingJetvLeadingD,  "Leading jet");
    leg2->AddEntry(hDeltaPhiSubleadingJetvLeadingD, "Subleading jet");

    hDeltaPhiLeadingJetvLeadingD->SetMarkerStyle(20);
    hDeltaPhiLeadingJetvLeadingD->SetMarkerColor(kBlack);
    hDeltaPhiLeadingJetvLeadingD->SetLineColor(kBlack);

    hDeltaPhiSubleadingJetvLeadingD->SetMarkerStyle(20);
    hDeltaPhiSubleadingJetvLeadingD->SetMarkerColor(kBlue);
    hDeltaPhiSubleadingJetvLeadingD->SetLineColor(kBlue);
    hDeltaPhiLeadingJetvLeadingD->GetXaxis()->SetTitle("|#Delta #phi| = | #phi_{#it{D}^{0}} - #phi_{jet} |");
    hDeltaPhiLeadingJetvLeadingD->GetYaxis()->SetTitle("1/#it{N}_{jet}d#it{N}/d#Delta#phi (rad)^{-1}");
    hDeltaPhiLeadingJetvLeadingD->GetXaxis()->SetTitleOffset(1.2);




    hDeltaPhiLeadingJetvLeadingD->Scale(1./hDeltaPhiLeadingJetvLeadingD->GetEntries(), "width");
    hDeltaPhiSubleadingJetvLeadingD->Scale(1./hDeltaPhiSubleadingJetvLeadingD->GetEntries(), "width");

    hDeltaPhiLeadingJetvLeadingD->GetYaxis()->SetRangeUser(5e-3, 10);
    hDeltaPhiLeadingJetvLeadingD->Draw();
    hDeltaPhiSubleadingJetvLeadingD->Draw("same");
    leg2->Draw("same");
    cms->Draw();
    TrigString->Draw();
    JetString->Draw();
    D0string->Draw();

    c2->SaveAs(Form("deltaPhi_%s.pdf", tag.c_str()));

    // ------------------------------------------------------------
    // fragmentation functions
    // ------------------------------------------------------------

    TCanvas* c3 = new TCanvas("c3", "c3", 600, 600);
    c3->SetLogy();
    c3->SetTickx(1);
    c3->SetTicky(1);
    c3->SetRightMargin(0.05);
    c3->SetLeftMargin(0.13);

    hD0FragFraction->SetMarkerStyle(20);
    hD0FragFraction->SetMarkerColor(kBlack);
    hD0FragFraction->SetLineColor(kBlack);

    hD0FragFraction->GetXaxis()->SetTitle("z = #it{p}_{T, #it{D}^{0}}/#it{p}_{T, jet}");
    hD0FragFraction->GetYaxis()->SetTitle("1/#it{N}_{jet}d#it{N}/dz");
    hD0FragFraction->Scale(1./hD0FragFraction->GetEntries(), "width");

    hD0FragFraction->GetYaxis()->SetRangeUser(1e-5, 100);
    hD0FragFraction->GetXaxis()->SetTitleOffset(1.1);


    hD0FragFraction->Draw();
    TrigString->Draw();
    JetString->Draw();
    cms->Draw();
    D0string->Draw();


    c3->SaveAs(Form("D0FragFunction_%s.pdf", tag.c_str()));

}

//============================================================//
// Main analysis
//============================================================//
int main(int argc, char *argv[]) {
  if (printHelpMessage(argc, argv))
    return 0;
  CommandLine CL(argc, argv);
  // input to this step is the result of running
  string input = CL.Get("Input", "output.root"); // Input file, by default take as output of the previous step
  string tag = CL.Get("tag", "test");
  plotHFjet(input, tag);
}