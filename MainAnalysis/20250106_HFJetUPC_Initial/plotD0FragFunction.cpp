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
#include "SetStyle.h"
#include "ProgressBar.h" // Yi's fish progress bar
#include "helpMessage.h" // Print out help message
#include "parameter.h"   // The parameters used in the analysis
#include "utilities.h"   // Yen-Jie's random utility functions


// function to plot prompt, non-prompt Dzero results
void plotD0FragFunction(std::vector<string> input, string tag){

    // input files
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);


    // "#bf{CMS} UPC PbPb 2023 (5.36 TeV)"
    //"#bf{CMS} #it{Simulation} PYTHIA8 #gammaN (5.36 TeV)"
    // draw the histograms
    TLatex* cms = new TLatex(0.12,0.92,"#bf{CMS} #it{Simulation} PYTHIA8 #gammaN (5.36 TeV)");
    cms->SetNDC();
    cms->SetTextSize(0.05);
    cms->SetTextFont(42);



    // =====================================================
    // D0 fragmentation function
    // =====================================================

    TCanvas* c3 = new TCanvas("c3", "c3", 600, 600);
    c3->SetLogy();
    c3->SetTickx(1);
    c3->SetTicky(1);
    c3->SetRightMargin(0.05);
    c3->SetLeftMargin(0.13);

    TLegend* leg = new TLegend(0.35, 0.55, 0.8, 0.8);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->SetTextFont(42);
    leg->SetTextSize(0.035);


    static vector<int> Colors = GetCVDColors6();

    // loop over all of the input files
    for(int f=0; f < input.size(); f++){
        // open the files
        TFile* inputFile = TFile::Open(input.at(f).c_str());

        // get the histograms
        TH1D* hNev = (TH1D*)inputFile->Get("hNev");
        hNev->SetName(Form("hNev_%d", f));
        int nEvents = hNev->GetBinContent(1);
        TH1D* hD0FragFraction = (TH1D*)inputFile->Get("hD0FragFraction");
        hD0FragFraction->SetName(Form("hD0FragFraction_%d", f));

        TDirectoryFile* paramDirectory = (TDirectoryFile*)inputFile->Get("par");
        TH1D* hTrigChoice = (TH1D*)paramDirectory->Get("parTriggerChoice");
        TH1D* hMinJetPt = (TH1D*)paramDirectory->Get("parMinJetPT");
        TH1D* hMaxJetPt = (TH1D*)paramDirectory->Get("parMaxJetPT");
        TH1D* hMaxJetY = (TH1D*)paramDirectory->Get("parMaxJetY");
        TH1D* hMinD0Pt = (TH1D*)paramDirectory->Get("parMinDzeroPT");
        TH1D* hMaxD0Y  = (TH1D*)paramDirectory->Get("parMaxDzeroY");
        hD0FragFraction->SetMarkerStyle(20);
        hD0FragFraction->SetMarkerColor(Colors[f]);
        hD0FragFraction->SetLineColor(Colors[f]);
        // normalize by the number of jets
        hD0FragFraction->Scale(1./hD0FragFraction->GetEntries(), "width");
        if(hMaxJetPt->GetBinContent(1) > 100){
            leg->AddEntry(hD0FragFraction, Form("p_{T, jet} > %0.0f GeV, |#it{y}_{jet}| <  %0.1f ",hMinJetPt->GetBinContent(1), hMaxJetY->GetBinContent(1)));
        }
        else{
            leg->AddEntry(hD0FragFraction, Form(" %0.0f < p_{T, jet} < %0.0f GeV, |#it{y}_{jet}| <  %0.1f ",hMinJetPt->GetBinContent(1),  hMaxJetPt->GetBinContent(1), hMaxJetY->GetBinContent(1)));
        }



        // handle extra things draing the first iteration
        if(f == 0){
            TLatex* TrigString;
            if(hTrigChoice->GetBinContent(1) == 0){
            TrigString = new TLatex(0.62,0.81,"No Trigger Selection");
            }
            else if(hTrigChoice->GetBinContent(1) == 1){
            TrigString = new TLatex(0.65,0.82,"ZDC 1n OR");
            }
            else if(hTrigChoice->GetBinContent(1) == 2){
            TrigString = new TLatex(0.65,0.82,"ZDC XOR + Jet 8");
            }
            else{
            std::cout << "[Error]: Unrecognized trigger option " << hTrigChoice->GetBinContent(1) << std::endl;
            return;
            }

            TrigString->SetNDC();
            TrigString->SetTextSize(0.035);
            TrigString->SetTextFont(42);


            TLatex* D0string = new TLatex(0.55, 0.85, Form("p_{T, D^{0}} > %0.0f GeV, |D^{0} y| <  %0.1f ", hMinD0Pt->GetBinContent(1), hMaxD0Y->GetBinContent(1)));
            D0string->SetNDC();
            D0string->SetTextSize(0.035);
            D0string->SetTextFont(42);


            hD0FragFraction->GetYaxis()->SetRangeUser(1e-3, 5e3);
            hD0FragFraction->GetXaxis()->SetTitleOffset(1.1);
            hD0FragFraction->GetXaxis()->SetTitle("z = #it{p}_{T, #it{D}^{0}}/#it{p}_{T, jet}");
            hD0FragFraction->GetYaxis()->SetTitle("1/#it{N}_{jet}d#it{N}/dz");
            hD0FragFraction->Draw();
            D0string->Draw();
            TrigString->Draw();
        }
        else{
            hD0FragFraction->Draw("same");
        }

    }

    leg->Draw();
    cms->Draw();
    c3->SaveAs(Form("D0FragFunction_%s.pdf", tag.c_str()));

    // =====================================================
    // D0 in jet distributions
    // =====================================================
    TCanvas* c4 = new TCanvas("c4", "c4", 600, 600);
    c4->SetLogy();
    c4->SetTickx(1);
    c4->SetTicky(1);
    c4->SetRightMargin(0.05);
    c4->SetLeftMargin(0.13);

        // loop over all of the input files
    for(int f=0; f < input.size(); f++){
        // open the files
        TFile* inputFile = TFile::Open(input.at(f).c_str());

        // get the histograms
        TH1D* hNev = (TH1D*)inputFile->Get("hNev");
        hNev->SetName(Form("hNev_%d", f));
        int nEvents = hNev->GetBinContent(1);
        TH1D* hD0inJetPt = (TH1D*)inputFile->Get("hD0inJetPt");
        hD0inJetPt->SetName(Form("hD0inJetPt_%d", f));

        TDirectoryFile* paramDirectory = (TDirectoryFile*)inputFile->Get("par");
        TH1D* hTrigChoice = (TH1D*)paramDirectory->Get("parTriggerChoice");
        TH1D* hMinJetPt = (TH1D*)paramDirectory->Get("parMinJetPT");
        TH1D* hMaxJetPt = (TH1D*)paramDirectory->Get("parMaxJetPT");
        TH1D* hMaxJetY = (TH1D*)paramDirectory->Get("parMaxJetY");
        TH1D* hMinD0Pt = (TH1D*)paramDirectory->Get("parMinDzeroPT");
        TH1D* hMaxD0Y  = (TH1D*)paramDirectory->Get("parMaxDzeroY");
        TH1D* hIsData = (TH1D*)paramDirectory->Get("parIsData");

        hD0inJetPt->SetMarkerStyle(20);
        hD0inJetPt->SetMarkerColor(Colors[f]);
        hD0inJetPt->SetLineColor(Colors[f]);
        // normalize by the number of jets
        hD0inJetPt->Scale(1./hD0inJetPt->GetEntries(), "width");
        hD0inJetPt->Rebin(2);



        // handle extra things draing the first iteration
        if(f == 0){
            TLatex* TrigString;
            if(hTrigChoice->GetBinContent(1) == 0){
            TrigString = new TLatex(0.62,0.81,"No Trigger Selection");
            }
            else if(hTrigChoice->GetBinContent(1) == 1){
            TrigString = new TLatex(0.65,0.82,"ZDC 1n OR");
            }
            else if(hTrigChoice->GetBinContent(1) == 2){
            TrigString = new TLatex(0.65,0.82,"ZDC XOR + Jet 8");
            }
            else{
            std::cout << "[Error]: Unrecognized trigger option " << hTrigChoice->GetBinContent(1) << std::endl;
            return;
            }

            TrigString->SetNDC();
            TrigString->SetTextSize(0.035);
            TrigString->SetTextFont(42);


            TLatex* D0string = new TLatex(0.55, 0.85, Form("p_{T, D^{0}} > %0.0f GeV, |D^{0} y| <  %0.1f ", hMinD0Pt->GetBinContent(1), hMaxD0Y->GetBinContent(1)));
            D0string->SetNDC();
            D0string->SetTextSize(0.035);
            D0string->SetTextFont(42);


            hD0inJetPt->GetYaxis()->SetRangeUser(1e-5, 5e1);
            hD0inJetPt->GetXaxis()->SetTitleOffset(1.1);
            hD0inJetPt->GetXaxis()->SetTitle("#it{p}_{T, #it{D}^{0}}");
            hD0inJetPt->GetYaxis()->SetTitle("1/#it{N}_{jet}d#it{N}/d#it{p}_{T}");
            hD0inJetPt->Draw();
            D0string->Draw();
            TrigString->Draw();
        }
        else{
            hD0inJetPt->Draw("same");
        }

    }

    leg->Draw();
    cms->Draw();
    c4->SaveAs(Form("D0inJetPt_%s.pdf", tag.c_str()));

    // =====================================================
    // Delta Phi distributions
    // =====================================================
    TCanvas* c5 = new TCanvas("c5", "c5", 600, 600);
    c5->SetLogy();
    c5->SetTickx(1);
    c5->SetTicky(1);
    c5->SetRightMargin(0.05);
    c5->SetLeftMargin(0.13);

    // loop over all of the input files
    for(int f=0; f < input.size(); f++){
        // open the files
        TFile* inputFile = TFile::Open(input.at(f).c_str());

        // get the histograms
        TH1D* hNev = (TH1D*)inputFile->Get("hNev");
        hNev->SetName(Form("hNev_%d", f));
        int nEvents = hNev->GetBinContent(1);


        TDirectoryFile* paramDirectory = (TDirectoryFile*)inputFile->Get("par");
        TH1D* hTrigChoice = (TH1D*)paramDirectory->Get("parTriggerChoice");
        TH1D* hMinJetPt = (TH1D*)paramDirectory->Get("parMinJetPT");
        TH1D* hMaxJetPt = (TH1D*)paramDirectory->Get("parMaxJetPT");
        TH1D* hMaxJetY = (TH1D*)paramDirectory->Get("parMaxJetY");
        TH1D* hMinD0Pt = (TH1D*)paramDirectory->Get("parMinDzeroPT");
        TH1D* hMaxD0Y  = (TH1D*)paramDirectory->Get("parMaxDzeroY");
        TH1D* hIsData = (TH1D*)paramDirectory->Get("parIsData");
        TH1D* hDeltaPhiLeadingJetvLeadingD = (TH1D*)inputFile->Get("hDeltaPhiLeadingJetvLeadingD");
        hDeltaPhiLeadingJetvLeadingD->SetName(Form("hDeltaPhiLeadingJetvLeadingD_%d", f));

        TH1D* hDeltaPhiSubleadingJetvLeadingD = (TH1D*)inputFile->Get("hDeltaPhiSubleadingJetvLeadingD");
        hDeltaPhiSubleadingJetvLeadingD->SetName(Form("hDeltaPhiSubleadingJetvLeadingD_%d", f));

        hDeltaPhiLeadingJetvLeadingD->SetMarkerStyle(24);
        hDeltaPhiLeadingJetvLeadingD->SetMarkerColor(Colors[f]);
        hDeltaPhiLeadingJetvLeadingD->SetLineColor(Colors[f]);

        hDeltaPhiSubleadingJetvLeadingD->SetMarkerStyle(20);
        hDeltaPhiSubleadingJetvLeadingD->SetMarkerColor(Colors[f]);
        hDeltaPhiSubleadingJetvLeadingD->SetLineColor(Colors[f]);
        hDeltaPhiLeadingJetvLeadingD->GetXaxis()->SetTitleOffset(1.2);

        hDeltaPhiLeadingJetvLeadingD->Scale(1./hDeltaPhiLeadingJetvLeadingD->GetEntries(), "width");
        hDeltaPhiSubleadingJetvLeadingD->Scale(1./hDeltaPhiSubleadingJetvLeadingD->GetEntries(), "width");

        // handle extra things draing the first iteration
        if(f == 0){
            TLatex* TrigString;
            if(hTrigChoice->GetBinContent(1) == 0){
            TrigString = new TLatex(0.62,0.81,"No Trigger Selection");
            }
            else if(hTrigChoice->GetBinContent(1) == 1){
            TrigString = new TLatex(0.65,0.82,"ZDC 1n OR");
            }
            else if(hTrigChoice->GetBinContent(1) == 2){
            TrigString = new TLatex(0.65,0.82,"ZDC XOR + Jet 8");
            }
            else{
            std::cout << "[Error]: Unrecognized trigger option " << hTrigChoice->GetBinContent(1) << std::endl;
            return;
            }

            TrigString->SetNDC();
            TrigString->SetTextSize(0.035);
            TrigString->SetTextFont(42);


            TLatex* D0string = new TLatex(0.55, 0.85, Form("p_{T, D^{0}} > %0.0f GeV, |D^{0} y| <  %0.1f ", hMinD0Pt->GetBinContent(1), hMaxD0Y->GetBinContent(1)));
            D0string->SetNDC();
            D0string->SetTextSize(0.035);
            D0string->SetTextFont(42);



            hDeltaPhiLeadingJetvLeadingD->GetYaxis()->SetRangeUser(5e-4, 5e2);
            hDeltaPhiLeadingJetvLeadingD->GetXaxis()->SetTitle("|#Delta #phi| = | #phi_{#it{D}^{0}} - #phi_{jet} |");
            hDeltaPhiLeadingJetvLeadingD->GetYaxis()->SetTitle("1/#it{N}_{jet}d#it{N}/d#Delta#phi (rad)^{-1}");
            hDeltaPhiLeadingJetvLeadingD->Draw();
            hDeltaPhiSubleadingJetvLeadingD->Draw("same");
            D0string->Draw();
            TrigString->Draw();
        }
        else{
            hDeltaPhiLeadingJetvLeadingD->Draw("same");
            hDeltaPhiSubleadingJetvLeadingD->Draw("same");
        }

    }

    leg->Draw();
    cms->Draw();
    c5->SaveAs(Form("DeltaPhi_%s.pdf", tag.c_str()));

    // =====================================================
    // D0 mass in jet distributions
    // =====================================================
    TCanvas* c6 = new TCanvas("c6", "c6", 600, 600);
    //c6->SetLogy();
    c6->SetTickx(1);
    c6->SetTicky(1);
    c6->SetRightMargin(0.05);
    c6->SetLeftMargin(0.13);

        // loop over all of the input files
    for(int f=0; f < input.size(); f++){
        // open the files
        TFile* inputFile = TFile::Open(input.at(f).c_str());
        // get the histograms
        TH1D* hNev = (TH1D*)inputFile->Get("hNev");
        hNev->SetName(Form("hNev_%d", f));
        int nEvents = hNev->GetBinContent(1);
        TH1D* hD0massInJet = (TH1D*)inputFile->Get("hDmassInJet");
        hD0massInJet->SetName(Form("hD0massInJet_%d", f));

        TDirectoryFile* paramDirectory = (TDirectoryFile*)inputFile->Get("par");
        TH1D* hTrigChoice = (TH1D*)paramDirectory->Get("parTriggerChoice");
        TH1D* hMinJetPt = (TH1D*)paramDirectory->Get("parMinJetPT");
        TH1D* hMaxJetPt = (TH1D*)paramDirectory->Get("parMaxJetPT");
        TH1D* hMaxJetY = (TH1D*)paramDirectory->Get("parMaxJetY");
        TH1D* hMinD0Pt = (TH1D*)paramDirectory->Get("parMinDzeroPT");
        TH1D* hMaxD0Y  = (TH1D*)paramDirectory->Get("parMaxDzeroY");
        TH1D* hIsData = (TH1D*)paramDirectory->Get("parIsData");

        hD0massInJet->SetMarkerStyle(20);
        hD0massInJet->SetMarkerColor(Colors[f]);
        hD0massInJet->SetLineColor(Colors[f]);
        // normalize by the number of jets
        hD0massInJet->Scale(1./hD0massInJet->GetEntries(), "width");



        // handle extra things draing the first iteration
        if(f == 0){
            TLatex* TrigString;
            if(hTrigChoice->GetBinContent(1) == 0){
            TrigString = new TLatex(0.62,0.81,"No Trigger Selection");
            }
            else if(hTrigChoice->GetBinContent(1) == 1){
            TrigString = new TLatex(0.65,0.82,"ZDC 1n OR");
            }
            else if(hTrigChoice->GetBinContent(1) == 2){
            TrigString = new TLatex(0.65,0.82,"ZDC XOR + Jet 8");
            }
            else{
            std::cout << "[Error]: Unrecognized trigger option " << hTrigChoice->GetBinContent(1) << std::endl;
            return;
            }

            TrigString->SetNDC();
            TrigString->SetTextSize(0.035);
            TrigString->SetTextFont(42);


            TLatex* D0string = new TLatex(0.55, 0.85, Form("p_{T, D^{0}} > %0.0f GeV, |D^{0} y| <  %0.1f ", hMinD0Pt->GetBinContent(1), hMaxD0Y->GetBinContent(1)));
            D0string->SetNDC();
            D0string->SetTextSize(0.035);
            D0string->SetTextFont(42);


            hD0massInJet->GetXaxis()->SetTitleOffset(1.1);
            hD0massInJet->GetYaxis()->SetRangeUser(0, 35);
            hD0massInJet->GetXaxis()->SetTitle("#it{m}_{#it{D}^{0}}");
            hD0massInJet->GetYaxis()->SetTitle("1/#it{N}_{jet}d#it{N}/d#it{m}");
            hD0massInJet->Draw();
            D0string->Draw();
            TrigString->Draw();
        }
        else{
            hD0massInJet->Draw("same");
        }

    }

    leg->Draw();
    cms->Draw();
    c6->SaveAs(Form("D0massInJet_%s.pdf", tag.c_str()));




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
  plotD0FragFunction(input, tag);
}