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
    c->SetRightMargin(0.05);
    c->SetLeftMargin(0.13);

    TLegend* leg = new TLegend(0.16, 0.65, 0.5, 0.85);
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

    TH2D* h2PtJetPtRef = (TH2D*)inputFile->Get("h2PtJetPtRef");
    h2PtJetPtRef->SetName(Form("h2PtJetPtRef_%d", 0));

    TH2D* h2D0JetPtD0Ref = (TH2D*) inputFile->Get("h2D0JetPtD0Ref");
    h2D0JetPtD0Ref->SetName(Form("h2D0JetPtD0Ref_%d", 0));

    TH2D* h2D0PtJetPtRefvsY = (TH2D*)inputFile->Get("h2D0PtJetPtRefvsY");
    h2D0PtJetPtRefvsY->SetName(Form("h2D0PtJetPtRefvsY_%d", 0));

    TH2D* h2PtJetPtRefvsY = (TH2D*)inputFile->Get("h2PtJetPtRefvsY");
    h2PtJetPtRefvsY->SetName(Form("h2PtJetPtRefvsY_%d", 0));

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


    TLine *line = new TLine(1, 1e-6, 1,  0.02);
    line->SetLineColor(kGray+2);
    line->SetLineStyle(2);
    line->SetLineWidth(2);

    //-------------------------------------------------------------
    TLine *hline = new TLine(15, 1, 50, 1);
    hline->SetLineColor(kGray+2);
    hline->SetLineStyle(2);
    hline->SetLineWidth(2);


    // =====================================================
    // Jet Energy Resolution - as a function of ref jet pt
    // =====================================================
    std::vector<double> jetPtBins = {15, 20, 25, 35, 50};
    int nbinsX  = jetPtBins.size()-1;
    // Create a new histogram for standard deviations
    TH1F *hist_std_dev = new TH1F("hist_std_dev", "Projection Std Dev;#it{p}^{ref}_{T, jet} (GeV); #sigma ( #it{p}_{T, jet}/#it{p}^{ref}_{T, jet} )",  jetPtBins.size()-1, jetPtBins.data());
    TH1F *hist_std_dev_D0 = new TH1F("hist_std_dev_D0", "Projection Std Dev;#it{p}^{ref}_{T, jet} (GeV); #sigma ( #it{p}_{T, jet}/#it{p}^{ref}_{T, jet} )",  jetPtBins.size()-1, jetPtBins.data());
    TH1F *hist_mean = new TH1F("hist_mean", "Projection Mean;#it{p}^{ref}_{T, jet} (GeV); #mu( #it{p}_{T, jet}/#it{p}^{ref}_{T, jet} )",  jetPtBins.size()-1, jetPtBins.data());
    TH1F *hist_mean_D0 = new TH1F("hist_meanD0", "Projection Mean;#it{p}^{ref}_{T, jet} (GeV); #mu( #it{p}_{T, jet}/#it{p}^{ref}_{T, jet} )",  jetPtBins.size()-1, jetPtBins.data());

    // Loop over all X bins
    for (int i = 1; i <= nbinsX; ++i) { // ROOT bins start at 1
        TH1D *y_projection = h2PtJetPtRef->ProjectionY(Form("y_proj_%d", i), i, i);
        TH1D *y_projectionD0 = h2D0JetPtD0Ref->ProjectionY(Form("y_proj_d0_%d", i), i, i);
        if (y_projection->GetEntries() > 0) {
            if (i != 5) y_projection->Rebin(2);
             // Find the bin with the max value (peak position)
            int max_bin = y_projection->GetMaximumBin();
            double max_value = y_projection->GetXaxis()->GetBinCenter(max_bin);

            // Estimate initial sigma from RMS as a fallback
            double estimated_sigma = y_projection->GetRMS();

            // Define fit range: [max_value - 2σ, max_value + 2σ]
            double fit_min = max_value - 1.5*estimated_sigma;
            double fit_max = max_value + 1.5*estimated_sigma;

            // Create and perform Gaussian fit within the range
            TF1 *gaus = new TF1("gaus", "gaus", fit_min, fit_max);
            y_projection->Fit(gaus, "QR"); // "Q" = quiet, "R" = restrict range

            // extract the mean and its error
            double mean = gaus->GetParameter(1);
            double mean_err = gaus->GetParError(1);
            hist_mean->SetBinContent(i, mean);
            hist_mean->SetBinError(i, mean_err);

            // Extract sigma (std deviation) and its error
            double sigma = gaus->GetParameter(2);     // Standard deviation
            double sigma_err = gaus->GetParError(2);  // Error in standard deviation

            // Set bin content and error in the new histogram
            hist_std_dev->SetBinContent(i, sigma);
            hist_std_dev->SetBinError(i, sigma_err);

            // Create a canvas to draw projection and fit
            TCanvas *c8 = new TCanvas(Form("c_proj_%d", i), Form("Projection for X bin %d", i), 800, 600);
            y_projection->SetLineColor(kBlue);
            y_projection->Draw("E"); // Draw with error bars

            gaus->SetLineColor(kRed);
            gaus->Draw("SAME"); // Draw the fit on top of the projection

            // Add legend
            TLegend *leg3 = new TLegend(0.6, 0.7, 0.88, 0.88);
            leg3->AddEntry(y_projection, "Y Projection", "l");
            leg3->AddEntry(gaus, "Gaussian Fit", "l");
            leg3->Draw();

            // Save canvas as image
            c8->SaveAs(Form("projection_fit_xbin_%d.png", i));

            delete c8;
            delete gaus;
        }
        if (y_projectionD0->GetEntries() > 0) {
            // Fit with a Gaussian
            if(i!=3) y_projectionD0->Rebin(5);
            else y_projectionD0->Rebin(2);
            int max_bin = y_projectionD0->GetMaximumBin();
            double max_value = y_projectionD0->GetXaxis()->GetBinCenter(max_bin);

            // Estimate initial sigma from RMS as a fallback
            double estimated_sigma = y_projection->GetRMS();

            // Define fit range: [max_value - 2σ, max_value + 2σ]
            double sigmaFactor = 1.5;
            if(i ==3)sigmaFactor = 1.0;
            double fit_min = max_value - sigmaFactor*estimated_sigma;
            double fit_max = max_value + sigmaFactor*estimated_sigma;

            // Create and perform Gaussian fit within the range
            TF1 *gausD0 = new TF1("gausD0", "gaus", fit_min, fit_max);
            y_projectionD0->Fit(gausD0, "QR"); // "Q" for quiet mode

            // extract mean and its error
            double meanD0 = gausD0->GetParameter(1);
            double meanD0_err = gausD0->GetParError(1);
            hist_mean_D0->SetBinContent(i, meanD0);
            hist_mean_D0->SetBinError(i, meanD0_err);

            // Extract sigma (std deviation) and its error
            double sigmaD0 = gausD0->GetParameter(2);     // Standard deviation
            double sigmaD0_err = gausD0->GetParError(2);  // Error in standard deviation

            // Set bin content and error in the new histogram
            hist_std_dev_D0->SetBinContent(i, sigmaD0);
            hist_std_dev_D0->SetBinError(i, sigmaD0_err);

            if(i==3){
              std::cout << "meanD0: " << meanD0 << " meanD0Error: " << meanD0_err << " sigmaD0: " << sigmaD0 << " sigmaD0Err " << sigmaD0_err << std::endl;
            }

            // Create a canvas to draw projection and fit
            TCanvas *c11 = new TCanvas(Form("c11_proj_%d", i), Form("Projection for X bin %d", i), 800, 600);
            y_projectionD0->SetLineColor(kBlue);
            y_projectionD0->Draw("E"); // Draw with error bars

            gausD0->SetLineColor(kRed);
            gausD0->Draw("SAME"); // Draw the fit on top of the projection

            // Add legend
            TLegend *leg10 = new TLegend(0.6, 0.7, 0.88, 0.88);
            leg10->AddEntry(y_projectionD0, "Y Projection", "l");
            leg10->AddEntry(gausD0, "Gaussian Fit", "l");
            leg10->Draw();

            // Save canvas as image
            c11->SaveAs(Form("projectionD0_fit_xbin_%d.png", i));

            delete c11;
            delete gausD0;
        }
        delete y_projection;
        delete y_projectionD0;
    }

    // Draw and save the standard deviation histogram
    TCanvas *c9 = new TCanvas("c9", "", 600, 600);
    c9->SetTickx(1);
    c9->SetTicky(1);
    c9->SetLogz();
    c9->SetRightMargin(0.05);
    c9->SetLeftMargin(0.13);

    leg2->AddEntry(hist_std_dev, "Inclusive Jets");
    leg2->AddEntry(hist_std_dev_D0, "D^{0}-Tagged Jets");

    hist_std_dev->SetLineColor(Colors[0]);
    hist_std_dev->SetMarkerStyle(20);
    hist_std_dev->SetMarkerColor(Colors[0]);
    hist_std_dev->Scale(1.0);
    hist_std_dev->GetYaxis()->SetRangeUser(0.0, 0.3);
    hist_std_dev->GetXaxis()->SetTitleOffset(1.1);
    hist_std_dev->Draw("same");
    hist_std_dev_D0->SetLineColor(Colors[1]);
    hist_std_dev_D0->SetMarkerStyle(20);
    hist_std_dev_D0->SetMarkerColor(Colors[1]);
    hist_std_dev_D0->Draw("same");
    leg2->Draw("same");
    JetString->Draw();
    TrigString->Draw();
    cms->Draw("same");
    D0pTString->Draw();
    c9->SaveAs(Form("JER_%s.pdf", tag.c_str()));

    // do the same for the JES
    TCanvas *c10 = new TCanvas("c10", "", 600, 600);
    c10->SetTickx(1);
    c10->SetTicky(1);
    c10->SetLogz();
    c10->SetRightMargin(0.05);
    c10->SetLeftMargin(0.13);

    hist_mean->SetLineColor(Colors[0]);
    hist_mean->SetMarkerStyle(20);
    hist_mean->SetMarkerColor(Colors[0]);
    hist_mean->Scale(1.0);
    hist_mean->GetYaxis()->SetRangeUser(0.8,1.3);
    hist_mean->GetXaxis()->SetTitleOffset(1.1);
    hist_mean->Draw();
    hist_mean_D0->SetLineColor(Colors[1]);
    hist_mean_D0->SetMarkerStyle(20);
    hist_mean_D0->SetMarkerColor(Colors[1]);
    hist_mean_D0->Draw("same");
    leg2->Draw("same");
    hline->Draw("same");
    JetString->Draw();
    TrigString->Draw();
    cms->Draw("same");
    D0pTString->Draw();
    c10->SaveAs(Form("JES_%s.pdf", tag.c_str()));

     // =====================================================
    // Jet Energy Resolution - as a function of Y
    // =====================================================
    //std::vector<double> yBins = {-2.0,  -1.0,  0.0, 1.0, 2.0};
    std::vector<double> yBins = {-5.0, -4.0, -3.0, -2.0, -1.0, 0.0, 1.0, 2.0, 3.0, 4.0, 5.0};
    // Create a new histogram for standard deviations
    TH1F *hist_std_dev_y = new TH1F("hist_std_dev_y", ";#it{y}_{ref}; #sigma ( #it{p}_{T, jet}/#it{p}^{ref}_{T, jet} )",  yBins.size()-1, yBins.data());
    TH1F *hist_std_dev_D0_y = new TH1F("hist_std_dev_D0_y", ";#it{y}_{ref}; #sigma ( #it{p}_{T, jet}/#it{p}^{ref}_{T, jet} )",  yBins.size()-1, yBins.data());
    TH1F *hist_mean_y = new TH1F("hist_mean_y", ";#it{y}_{ref}; #mu( #it{p}_{T, jet}/#it{p}^{ref}_{T, jet} )",  yBins.size()-1, yBins.data());
    TH1F *hist_mean_D0_y = new TH1F("hist_meanD0_y", "Projection Mean;#it{y}_{ref}; #mu( #it{p}_{T, jet}/#it{p}^{ref}_{T, jet} )",  yBins.size()-1, yBins.data());


    // Loop over all X bins
    for (int i = 1; i < yBins.size(); ++i) { // ROOT bins start at 1
        TH1D *y_projection_y = h2PtJetPtRefvsY->ProjectionY(Form("y_proj_y%d", i), i, i);
        TH1D *y_projectionD0_y = h2D0PtJetPtRefvsY->ProjectionY(Form("y_proj_d0_y%d", i), i, i);
        if (y_projection_y->GetEntries() > 0) {
            y_projection_y->Rebin(2);
             // Find the bin with the max value (peak position)
            int max_bin = y_projection_y->GetMaximumBin();
            double max_value = y_projection_y->GetXaxis()->GetBinCenter(max_bin);

            // Estimate initial sigma from RMS as a fallback
            double estimated_sigma = y_projection_y->GetRMS();

            // Define fit range: [max_value - 2σ, max_value + 2σ]
            double fit_min = max_value - 1.5*estimated_sigma;
            double fit_max = max_value + 1.5*estimated_sigma;

            // Create and perform Gaussian fit within the range
            TF1 *gaus = new TF1("gaus", "gaus", fit_min, fit_max);
            y_projection_y->Fit(gaus, "QR"); // "Q" = quiet, "R" = restrict range

            // extract the mean and its error
            double mean = gaus->GetParameter(1);
            double mean_err = gaus->GetParError(1);
            hist_mean_y->SetBinContent(i, mean);
            hist_mean_y->SetBinError(i, mean_err);

            // Extract sigma (std deviation) and its error
            double sigma = gaus->GetParameter(2);     // Standard deviation
            double sigma_err = gaus->GetParError(2);  // Error in standard deviation

            // Set bin content and error in the new histogram
            hist_std_dev_y->SetBinContent(i, sigma);
            hist_std_dev_y->SetBinError(i, sigma_err);

            // Create a canvas to draw projection and fit
            TCanvas *c12 = new TCanvas(Form("c_proj_%d", i), Form("Projection for X bin %d", i), 800, 600);
            y_projection_y->SetLineColor(kBlue);
            y_projection_y->Draw("E"); // Draw with error bars

            gaus->SetLineColor(kRed);
            gaus->Draw("SAME"); // Draw the fit on top of the projection

            // Add legend
            TLegend *leg3 = new TLegend(0.6, 0.7, 0.88, 0.88);
            leg3->AddEntry(y_projection_y, "Y Projection", "l");
            leg3->AddEntry(gaus, "Gaussian Fit", "l");
            leg3->Draw();

            // Save canvas as image
            c12->SaveAs(Form("projection_fit_xbin_vsy_%d.png", i));

            delete c12;
            delete gaus;
        }
        if (y_projectionD0_y->GetEntries() > 0) {
            // Fit with a Gaussian
            if(i == 3) y_projectionD0_y->Rebin(2);
            else y_projectionD0_y->Rebin(5);
            //y_projectionD0_y->Rebin(5);
            int max_bin = y_projectionD0_y->GetMaximumBin();
            double max_value = y_projectionD0_y->GetXaxis()->GetBinCenter(max_bin);

            // Estimate initial sigma from RMS as a fallback
            double estimated_sigma = y_projection_y->GetRMS();

            // Define fit range: [max_value - 2σ, max_value + 2σ]
            double sigmaFactor = 1.5;
            if(i==3)sigmaFactor = 2.0;
            double fit_min = max_value - sigmaFactor*estimated_sigma;
            double fit_max = max_value + sigmaFactor*estimated_sigma;

            // Create and perform Gaussian fit within the range
            TF1 *gausD0 = new TF1("gausD0", "gaus", fit_min, fit_max);
            y_projectionD0_y->Fit(gausD0, "QR"); // "Q" for quiet mode

            // extract mean and its error
            double meanD0 = gausD0->GetParameter(1);
            double meanD0_err = gausD0->GetParError(1);
            hist_mean_D0_y->SetBinContent(i, meanD0);
            hist_mean_D0_y->SetBinError(i, meanD0_err);

            // Extract sigma (std deviation) and its error
            double sigmaD0 = gausD0->GetParameter(2);     // Standard deviation
            double sigmaD0_err = gausD0->GetParError(2);  // Error in standard deviation

            // Set bin content and error in the new histogram
            hist_std_dev_D0_y->SetBinContent(i, sigmaD0);
            hist_std_dev_D0_y->SetBinError(i, sigmaD0_err);

            if(i==3){
              std::cout << "meanD0: " << meanD0 << " meanD0Error: " << meanD0_err << " sigmaD0: " << sigmaD0 << " sigmaD0Err " << sigmaD0_err << std::endl;
            }

            // Create a canvas to draw projection and fit
            TCanvas *c13 = new TCanvas(Form("c11_proj_%d", i), Form("Projection for X bin %d", i), 800, 600);
            y_projectionD0_y->SetLineColor(kBlue);
            y_projectionD0_y->Draw("E"); // Draw with error bars

            gausD0->SetLineColor(kRed);
            gausD0->Draw("SAME"); // Draw the fit on top of the projection

            // Add legend
            TLegend *leg10 = new TLegend(0.6, 0.7, 0.88, 0.88);
            leg10->AddEntry(y_projectionD0_y, "Y Projection", "l");
            leg10->AddEntry(gausD0, "Gaussian Fit", "l");
            leg10->Draw();

            // Save canvas as image
            c13->SaveAs(Form("projectionD0_fit_xbin_vsy_%d.png", i));

            delete c13;
            delete gausD0;
        }
        delete y_projection_y;
        delete y_projectionD0_y;
    }

    // Draw and save the standard deviation histogram
    TCanvas *c14 = new TCanvas("c14", "", 600, 600);
    c14->SetTickx(1);
    c14->SetTicky(1);
    c14->SetLogz();
    c14->SetRightMargin(0.05);
    c14->SetLeftMargin(0.13);

    hist_std_dev_y->SetLineColor(Colors[0]);
    hist_std_dev_y->SetMarkerStyle(20);
    hist_std_dev_y->SetMarkerColor(Colors[0]);
    hist_std_dev_y->Scale(1.0);
    hist_std_dev_y->GetYaxis()->SetRangeUser(0.0, 0.5);
    hist_std_dev_y->GetXaxis()->SetTitleOffset(1.1);
    hist_std_dev_y->Draw("same");
    hist_std_dev_D0_y->SetLineColor(Colors[1]);
    hist_std_dev_D0_y->SetMarkerStyle(20);
    hist_std_dev_D0_y->SetMarkerColor(Colors[1]);
    hist_std_dev_D0_y->Draw("same");
    leg2->Draw("same");
    JetString->Draw();
    TrigString->Draw();
    cms->Draw("same");
    D0pTString->Draw();
    c14->SaveAs(Form("JER_vsy_%s.pdf", tag.c_str()));

    // do the same for the JES
    TCanvas *c15 = new TCanvas("c15", "", 600, 600);
    c15->SetTickx(1);
    c15->SetTicky(1);
    c15->SetLogz();
    c15->SetRightMargin(0.05);
    c15->SetLeftMargin(0.13);
    hist_mean_y->SetLineColor(Colors[0]);
    hist_mean_y->SetMarkerStyle(20);
    hist_mean_y->SetMarkerColor(Colors[0]);
    hist_mean_y->Scale(1.0);
    hist_mean_y->GetYaxis()->SetRangeUser(0.8,1.3);
    hist_mean_y->GetXaxis()->SetTitleOffset(1.1);
    hist_mean_y->Draw();
    hist_mean_D0_y->SetLineColor(Colors[1]);
    hist_mean_D0_y->SetMarkerStyle(20);
    hist_mean_D0_y->SetMarkerColor(Colors[1]);
    hist_mean_D0_y->Draw("same");
    leg2->Draw("same");
    JetString->Draw("same");
    TrigString->Draw("same");
    cms->Draw("same");

    TLine *yhline = new TLine(-5.0, 1, 5.0, 1);
    yhline->SetLineColor(kGray+2);
    yhline->SetLineStyle(2);
    yhline->SetLineWidth(2);
    yhline->Draw("same");
    D0pTString->Draw();
    c15->SaveAs(Form("JES_vsy_%s.pdf", tag.c_str()));



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
