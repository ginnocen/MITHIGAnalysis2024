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


void plotPtHat(std::vector<string> input, std::vector<string> labels, string tag){

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
    c->SetLogy();
    c->SetTickx(1);
    c->SetTicky(1);
    c->SetRightMargin(0.05);
    c->SetLeftMargin(0.13);

    TLegend* leg = new TLegend(0.6, 0.55, 0.9, 0.8);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->SetTextFont(42);
    leg->SetTextSize(0.035);

    static vector<int> Colors = GetCVDColors6();


    //--------------------------------
    // loop over all of the input files
    // hJetPt
    //-------------------------------
    for(int f=0; f < input.size(); f++){
        // open the files
        TFile* inputFile = TFile::Open(input.at(f).c_str());

        // get the histograms
        TH1D* hNev = (TH1D*)inputFile->Get("hNev");
        hNev->SetName(Form("hNev_%d", f));
        int nEvents = hNev->GetBinContent(1);
        TH1D* hJetPt = (TH1D*)inputFile->Get("hJetPt");
        hJetPt->SetName(Form("hJetPt_%d", f));

        TDirectoryFile* paramDirectory = (TDirectoryFile*)inputFile->Get("par");
        TH1D* hTrigChoice = (TH1D*)paramDirectory->Get("parTriggerChoice");
        TH1D* hMinJetPt = (TH1D*)paramDirectory->Get("parMinJetPT");
        TH1D* hMaxJetPt = (TH1D*)paramDirectory->Get("parMaxJetPT");
        TH1D* hMaxJetY = (TH1D*)paramDirectory->Get("parMaxJetY");
        TH1D* hMinD0Pt = (TH1D*)paramDirectory->Get("parMinDzeroPT");
        TH1D* hMaxD0Y  = (TH1D*)paramDirectory->Get("parMaxDzeroY");
        TH1D* hIsData = (TH1D*)paramDirectory->Get("parIsData");


        TLatex* JetString = new TLatex(0.57,0.86,Form("p_{T, jet} > %0.0f GeV, |y_{jet}| <  %0.0f ", hMinJetPt->GetBinContent(1), hMaxJetY->GetBinContent(1)));
        JetString->SetNDC();
        JetString->SetTextSize(0.035);
        JetString->SetTextFont(42);


        hJetPt->SetMarkerStyle(20);
        hJetPt->SetMarkerColor(Colors[f]);
        hJetPt->SetLineColor(Colors[f]);
        // will need to think again how to normalize this, but for now do the number of jets
        hJetPt->Scale(1./hJetPt->Integral(), "width");



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

            // hJetPt->GetYaxis()->SetRangeUser(1e-5, 5e1);
            hJetPt->GetYaxis()->SetRangeUser(5e-6, 1e0);
            hJetPt->GetXaxis()->SetTitleOffset(1.1);
            hJetPt->GetXaxis()->SetTitle("#it{p}_{T, jet} (GeV)");
            hJetPt->GetYaxis()->SetTitle("Area Norm.");
            hJetPt->Draw();
            TrigString->Draw();
            JetString->Draw();

        }
        else{
            hJetPt->Draw("same");
        }
        leg->AddEntry(hJetPt, labels.at(f).c_str());

    }
    leg->Draw();
    cms->Draw();
    c->SaveAs(Form("JetPt_%s.pdf", tag.c_str()));
    //-------------------------------------------------------------



    //--------------------------------
    // loop over all of the input files
    // hGenJetPt
    //-------------------------------

    TCanvas* c2 = new TCanvas("c2", "c2", 600, 600);
    c2->SetLogy();
    c2->SetTickx(1);
    c2->SetTicky(1);
    c2->SetRightMargin(0.05);
    c2->SetLeftMargin(0.13);

    for(int f=0; f < input.size(); f++){
        // open the files
        TFile* inputFile = TFile::Open(input.at(f).c_str());

        // get the histograms
        TH1D* hNev = (TH1D*)inputFile->Get("hNev");
        hNev->SetName(Form("hNev_%d", f));
        int nEvents = hNev->GetBinContent(1);
        TH1D* hGenJetPt = (TH1D*)inputFile->Get("hGenJetPt");
        hGenJetPt->SetName(Form("hGenJetPt_%d", f));

        TDirectoryFile* paramDirectory = (TDirectoryFile*)inputFile->Get("par");
        TH1D* hTrigChoice = (TH1D*)paramDirectory->Get("parTriggerChoice");
        TH1D* hMinJetPt = (TH1D*)paramDirectory->Get("parMinJetPT");
        TH1D* hMaxJetPt = (TH1D*)paramDirectory->Get("parMaxJetPT");
        TH1D* hMaxJetY = (TH1D*)paramDirectory->Get("parMaxJetY");
        TH1D* hMinD0Pt = (TH1D*)paramDirectory->Get("parMinDzeroPT");
        TH1D* hMaxD0Y  = (TH1D*)paramDirectory->Get("parMaxDzeroY");
        TH1D* hIsData = (TH1D*)paramDirectory->Get("parIsData");

        hGenJetPt->SetMarkerStyle(20);
        hGenJetPt->SetMarkerColor(Colors[f]);
        hGenJetPt->SetLineColor(Colors[f]);
        // will need to think again how to normalize this, but for now do the number of jets
        hGenJetPt->Scale(1./hGenJetPt->Integral(), "width");


        TLatex* JetString = new TLatex(0.57,0.86,Form("p_{T, jet} > %0.0f GeV, |y_{jet}| <  %0.0f ", hMinJetPt->GetBinContent(1), hMaxJetY->GetBinContent(1)));
        JetString->SetNDC();
        JetString->SetTextSize(0.035);
        JetString->SetTextFont(42);

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

            // hJetPt->GetYaxis()->SetRangeUser(1e-5, 5e1);
            hGenJetPt->GetYaxis()->SetRangeUser(1e-5, 1e0);
            hGenJetPt->GetXaxis()->SetTitleOffset(1.1);
            hGenJetPt->GetXaxis()->SetTitle("#it{p}^{gen}_{T, jet} (GeV)");
            hGenJetPt->GetYaxis()->SetTitle("Area Norm.");
            hGenJetPt->Draw();
            TrigString->Draw();
            JetString->Draw();

        }
        else{
            hGenJetPt->Draw("same");
        }


    }

    leg->Draw();
    cms->Draw();
    c2->SaveAs(Form("GenJetPt_%s.pdf", tag.c_str()));
    //-------------------------------------------------------------


    //--------------------------------
    // loop over all of the input files
    // hGenJetPt
    //-------------------------------

    TCanvas* c3 = new TCanvas("c3", "c3", 600, 600);
    c3->SetLogy();
    c3->SetTickx(1);
    c3->SetTicky(1);
    c3->SetRightMargin(0.05);
    c3->SetLeftMargin(0.13);

    for(int f=0; f < input.size(); f++){
        // open the files
        TFile* inputFile = TFile::Open(input.at(f).c_str());

        // get the histograms
        TH1D* hNev = (TH1D*)inputFile->Get("hNev");
        hNev->SetName(Form("hNev_%d", f));
        int nEvents = hNev->GetBinContent(1);
        TH1D* hRefJetPt = (TH1D*)inputFile->Get("hRefJetPt");
        hRefJetPt->SetName(Form("hRefJetPt_%d", f));

        TDirectoryFile* paramDirectory = (TDirectoryFile*)inputFile->Get("par");
        TH1D* hTrigChoice = (TH1D*)paramDirectory->Get("parTriggerChoice");
        TH1D* hMinJetPt = (TH1D*)paramDirectory->Get("parMinJetPT");
        TH1D* hMaxJetPt = (TH1D*)paramDirectory->Get("parMaxJetPT");
        TH1D* hMaxJetY = (TH1D*)paramDirectory->Get("parMaxJetY");
        TH1D* hMinD0Pt = (TH1D*)paramDirectory->Get("parMinDzeroPT");
        TH1D* hMaxD0Y  = (TH1D*)paramDirectory->Get("parMaxDzeroY");
        TH1D* hIsData = (TH1D*)paramDirectory->Get("parIsData");

        hRefJetPt->SetMarkerStyle(20);
        hRefJetPt->SetMarkerColor(Colors[f]);
        hRefJetPt->SetLineColor(Colors[f]);
        // will need to think again how to normalize this, but for now do the number of jets
        hRefJetPt->Scale(1./hRefJetPt->Integral(), "width");

        TLatex* JetString = new TLatex(0.57,0.86,Form("p_{T, jet} > %0.0f GeV, |y_{jet}| <  %0.0f ", hMinJetPt->GetBinContent(1), hMaxJetY->GetBinContent(1)));
        JetString->SetNDC();
        JetString->SetTextSize(0.035);
        JetString->SetTextFont(42);

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

            hRefJetPt->GetXaxis()->SetTitleOffset(1.1);
            hRefJetPt->GetYaxis()->SetRangeUser(1e-5, 1e0);
            hRefJetPt->GetXaxis()->SetTitle("#it{p}^{ref}_{T, jet} (GeV)");
            hRefJetPt->GetYaxis()->SetTitle("Area Norm.");
            hRefJetPt->Draw();
            TrigString->Draw();
            JetString->Draw();
        }
        else{
            hRefJetPt->Draw("same");
        }


    }




    leg->Draw();
    cms->Draw();
    c3->SaveAs(Form("RefJetPt_%s.pdf", tag.c_str()));
    //-------------------------------------------------------------



    //--------------------------------
    // loop over all of the input files
    // hD0JetPt
    //-------------------------------
     TCanvas* c4= new TCanvas("c4", "c4", 600, 600);
    c4->SetLogy();
    c4->SetTickx(1);
    c4->SetTicky(1);
    c4->SetRightMargin(0.05);
    c4->SetLeftMargin(0.13);
    for(int f=0; f < input.size(); f++){
        // open the files
        TFile* inputFile = TFile::Open(input.at(f).c_str());

        // get the histograms
        TH1D* hNev = (TH1D*)inputFile->Get("hNev");
        hNev->SetName(Form("hNev_%d", f));
        int nEvents = hNev->GetBinContent(1);
        TH1D* hD0JetPt = (TH1D*)inputFile->Get("hD0JetPt");
        hD0JetPt->SetName(Form("hJetPt_%d", f));

        TDirectoryFile* paramDirectory = (TDirectoryFile*)inputFile->Get("par");
        TH1D* hTrigChoice = (TH1D*)paramDirectory->Get("parTriggerChoice");
        TH1D* hMinJetPt = (TH1D*)paramDirectory->Get("parMinJetPT");
        TH1D* hMaxJetPt = (TH1D*)paramDirectory->Get("parMaxJetPT");
        TH1D* hMaxJetY = (TH1D*)paramDirectory->Get("parMaxJetY");
        TH1D* hMinD0Pt = (TH1D*)paramDirectory->Get("parMinDzeroPT");
        TH1D* hMaxD0Y  = (TH1D*)paramDirectory->Get("parMaxDzeroY");
        TH1D* hIsData = (TH1D*)paramDirectory->Get("parIsData");

        hD0JetPt->SetMarkerStyle(20);
        hD0JetPt->SetMarkerColor(Colors[f]);
        hD0JetPt->SetLineColor(Colors[f]);
        // will need to think again how to normalize this, but for now do the number of jets
        hD0JetPt->Scale(1./hD0JetPt->Integral(), "width");

        TLatex* JetString = new TLatex(0.57,0.86,Form("p_{T, jet} > %0.0f GeV, |y_{jet}| <  %0.0f ", hMinJetPt->GetBinContent(1), hMaxJetY->GetBinContent(1)));
        JetString->SetNDC();
        JetString->SetTextSize(0.035);
        JetString->SetTextFont(42);

        TLatex* D0string = new TLatex(0.16, 0.86, Form("p_{T, D^{0}} > %0.0f GeV, |D^{0} y| <  %0.1f ", hMinD0Pt->GetBinContent(1), hMaxD0Y->GetBinContent(1)));
        D0string->SetNDC();
        D0string->SetTextSize(0.035);
        D0string->SetTextFont(42);


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

            // hJetPt->GetYaxis()->SetRangeUser(1e-5, 5e1);
            hD0JetPt->GetYaxis()->SetRangeUser(1e-5, 1e0);
            hD0JetPt->GetXaxis()->SetTitleOffset(1.1);
            hD0JetPt->GetXaxis()->SetTitle("#it{p}_{T, D^{0} jet} (GeV)");
            hD0JetPt->GetYaxis()->SetTitle("Area Norm.");
            hD0JetPt->Draw();
            TrigString->Draw();
            JetString->Draw();
            D0string->Draw();
        }
        else{
            hD0JetPt->Draw("same");
        }

    }


    leg->Draw();
    cms->Draw();
    c4->SaveAs(Form("D0JetPt_%s.pdf", tag.c_str()));
    //-------------------------------------------------------------


    //--------------------------------
    // loop over all of the input files
    // plot the pT hat distribution
    //-------------------------------
     TCanvas* c5= new TCanvas("c5", "c5", 600, 600);
    c5->SetLogy();
    c5->SetTickx(1);
    c5->SetTicky(1);
    c5->SetRightMargin(0.05);
    c5->SetLeftMargin(0.13);
    for(int f=0; f < input.size(); f++){
        // open the files
        TFile* inputFile = TFile::Open(input.at(f).c_str());

        // get the histograms
        TH1D* hNev = (TH1D*)inputFile->Get("hNev");
        hNev->SetName(Form("hNev_%d", f));
        int nEvents = hNev->GetBinContent(1);
        TH2D* hJetPtvsPtHat = (TH2D*)inputFile->Get("hJetPtvsPtHat");
        hJetPtvsPtHat->SetName(Form("hJetPtvsPtHat_%d", f));
        TH1D* pthat = (TH1D*)hJetPtvsPtHat->ProjectionY(Form("hPtHat_%d", f));

        TDirectoryFile* paramDirectory = (TDirectoryFile*)inputFile->Get("par");
        TH1D* hTrigChoice = (TH1D*)paramDirectory->Get("parTriggerChoice");
        TH1D* hMinJetPt = (TH1D*)paramDirectory->Get("parMinJetPT");
        TH1D* hMaxJetPt = (TH1D*)paramDirectory->Get("parMaxJetPT");
        TH1D* hMaxJetY = (TH1D*)paramDirectory->Get("parMaxJetY");
        TH1D* hMinD0Pt = (TH1D*)paramDirectory->Get("parMinDzeroPT");
        TH1D* hMaxD0Y  = (TH1D*)paramDirectory->Get("parMaxDzeroY");
        TH1D* hIsData = (TH1D*)paramDirectory->Get("parIsData");

        pthat->SetMarkerStyle(20);
        pthat->SetMarkerColor(Colors[f]);
        pthat->SetLineColor(Colors[f]);
        // will need to think again how to normalize this, but for now do the number of jets
        pthat->Scale(1./pthat->Integral(), "width");

        TLatex* JetString = new TLatex(0.57,0.86,Form("p_{T, jet} > %0.0f GeV, |y_{jet}| <  %0.0f ", hMinJetPt->GetBinContent(1), hMaxJetY->GetBinContent(1)));
        JetString->SetNDC();
        JetString->SetTextSize(0.035);
        JetString->SetTextFont(42);


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

            pthat->GetYaxis()->SetRangeUser(1e-8, 1e0);
            pthat->GetXaxis()->SetTitleOffset(1.1);
            pthat->GetXaxis()->SetTitle("#hat{#it{p}_{T}} (GeV)");
            pthat->GetYaxis()->SetTitle("Area Norm.");
            pthat->Draw();
            TrigString->Draw();
        }
        else{
            pthat->Draw("same");
        }

    }


    leg->Draw();
    cms->Draw();
    c5->SaveAs(Form("pthat_%s.pdf", tag.c_str()));
    //-------------------------------------------------------------


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
  std::vector<string> labels = CL.GetStringVector("Labels", ""); //  labels for the different pT hard bins

  string tag = CL.Get("tag", "test");
  plotPtHat(input, labels, tag);
}