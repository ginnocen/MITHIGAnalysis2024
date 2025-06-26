#include <iostream>
#include <vector>
#include <cmath>
#include "TString.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TF1.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TPad.h"
#include "TLatex.h"

#include "helper.h"

//  0 < p < 2.5 (GeV);   0 < log(dE/dx) < 5 (MeV/cm)

void pid(int idx = 6) {

    double p_min = 0, p_max = 2.5, delta = .05;
    double p_cutoff = .3; // no data below this point (where the fits will fit)
    int n_iter = round((p_max - p_min)/delta);
    std::vector<double> x_p, sigma_x, y_pion, y_kaon, y_prot, sigma_pion, sigma_kaon, sigma_prot;
    auto *histf = TFile::Open("mergedBkg.root"); //root file where dE/dx, p 2dhist is located
    TH2D *TH2hist = (TH2D*) histf->Get("hist"); //bkg 2d histogram
    TFile *f = TFile::Open("mc_histograms.root"); //root file where all p-binned hists are located
    std::vector<TH1D*> hists;
    for(int i = 0; i <= n_iter; i++) {
        double p_lo = p_min + i*delta; double p_hi = p_lo + delta;
        auto *hist = (TH1D*)f->Get(Form("hist%.2f_%.2f", p_lo, p_hi));
        hists.push_back(hist);
    }

    // ******************** Collecting mean, sigma data ********************
    for(int i = 0; i < n_iter; i++){
        double min = p_min + i*delta;
        double max = min + delta;
        double mid = (min + max)/2;
        // std::cout << "Processing Iteration Number " << i+1 <<"\n";
        TH1D *hist = hists[i];
        std::vector<std::vector<double>> x = PID_fit(min, max, hist); //PID_fit() does the actual fitting for min-max range
        x_p.push_back(mid);
        sigma_x.push_back(delta/2);

        y_pion.push_back(x[0][0]);
        y_kaon.push_back(x[1][0]);
        y_prot.push_back(x[2][0]);

        sigma_pion.push_back(x[0][1]);
        sigma_kaon.push_back(x[1][1]);
        sigma_prot.push_back(x[2][1]);
    }

    std::vector<double> y_pion_hi, y_pion_lo, y_kaon_hi, y_kaon_lo, y_prot_hi, y_prot_lo;
    for(int i = 0; i < n_iter; i++) {
        y_pion_hi.push_back(y_pion[i] + sigma_pion[i]);
        y_pion_lo.push_back(y_pion[i] - sigma_pion[i]);

        y_kaon_hi.push_back(y_kaon[i] + sigma_kaon[i]);
        y_kaon_lo.push_back(y_kaon[i] - sigma_kaon[i]);

        y_prot_hi.push_back(y_prot[i] + sigma_prot[i]);
        y_prot_lo.push_back(y_prot[i] - sigma_prot[i]);
    }

    // ******************** Fitting ********************

    //  the pion distribution is pretty flat, so the quadratic term in the exponential is dropped
    auto f_pion = new TF1("f_pion", "[0] + exp([1] + [2]*x + [3]*x*x)", p_cutoff, p_max);
    auto f_kaon = new TF1("f_kaon", "[0] + exp([1] + [2]*x + [3]*x*x)", p_cutoff, p_max);
    auto f_prot = new TF1("f_prot", "[0] + exp([1] + [2]*x + [3]*x*x)", p_cutoff, p_max);

    auto f_pion_hi = new TF1("f_pion_hi", "[0] + exp([1] + [2]*x + [3]*x*x)", p_cutoff, p_max);
    auto f_kaon_hi = new TF1("f_kaon_hi", "[0] + exp([1] + [2]*x + [3]*x*x)", p_cutoff, p_max);
    auto f_prot_hi = new TF1("f_prot_hi", "[0] + exp([1] + [2]*x + [3]*x*x)", p_cutoff, p_max);

    auto f_pion_lo = new TF1("f_pion_lo", "[0] + exp([1] + [2]*x + [3]*x*x)", p_cutoff, p_max);
    auto f_kaon_lo = new TF1("f_kaon_lo", "[0] + exp([1] + [2]*x + [3]*x*x)", p_cutoff, p_max);
    auto f_prot_lo = new TF1("f_prot_lo", "[0] + exp([1] + [2]*x + [3]*x*x)", p_cutoff, p_max);

    // initial guesses for parameters (note the 3rd and 4th parameters must be negative)
    const double pars_pion[] = { 0.943044, 1.49651, -14.1073, 8.49368};
    const double pars_kaon[] = { 0.912017, 1.63328, -5.24217, -1.75516 };
    const double pars_prot[] = { 0.89, 1.9, -1, -.5 };

    f_kaon->SetParLimits(3, -10, 0); f_kaon_hi->SetParLimits(3, -10, 0); f_kaon_lo->SetParLimits(3, -10, 0); //3rd par must be negative
    f_kaon->SetParLimits(2, -10, 0);//2nd parameter must also be negative for kaon (this help fixes an overfitting problem encountered)
    f_prot->SetParLimits(3, -5, 0); f_prot_hi->SetParLimits(3, -5, 0); f_prot_lo->SetParLimits(3, -5, 0); //3rd par for prot is also negative

    f_pion->SetParameters(pars_pion); f_kaon->SetParameters(pars_kaon); f_prot->SetParameters(pars_prot);
    f_pion_hi->SetParameters(pars_pion); f_kaon_hi->SetParameters(pars_kaon); f_prot_hi->SetParameters(pars_prot);
    f_pion_lo->SetParameters(pars_pion); f_kaon_lo->SetParameters(pars_kaon); f_prot_lo->SetParameters(pars_prot);

    TGraphErrors *pion = new TGraphErrors(x_p.size(), &x_p[0], &y_pion[0], &sigma_x[0], &sigma_pion[0]);
    TGraphErrors *kaon = new TGraphErrors(x_p.size(), &x_p[0], &y_kaon[0], &sigma_x[0], &sigma_kaon[0]);
    TGraphErrors *prot = new TGraphErrors(x_p.size(), &x_p[0], &y_prot[0], &sigma_x[0], &sigma_prot[0]);

    TGraph *pion_hi = new TGraph(x_p.size(), &x_p[0], &y_pion_hi[0]);
    TGraph *kaon_hi = new TGraph(x_p.size(), &x_p[0], &y_kaon_hi[0]);
    TGraph *prot_hi = new TGraph(x_p.size(), &x_p[0], &y_prot_hi[0]);

    TGraph *pion_lo = new TGraph(x_p.size(), &x_p[0], &y_pion_lo[0]);
    TGraph *kaon_lo = new TGraph(x_p.size(), &x_p[0], &y_kaon_lo[0]);
    TGraph *prot_lo = new TGraph(x_p.size(), &x_p[0], &y_prot_lo[0]);

    for(int jdx = 0; jdx < 7; jdx++)
    {

        kaon_hi->RemovePoint(0);
        kaon->RemovePoint(0);
        kaon_lo->RemovePoint(0);

        prot_hi->RemovePoint(0);
        prot->RemovePoint(0);
        prot_lo->RemovePoint(0);

        if(jdx != 6)
        {
            pion->RemovePoint(0);
            pion_hi->RemovePoint(0);
            pion_lo->RemovePoint(0);
        }
    }

    auto pion_copy = (TGraphErrors*) pion->Clone("pion_copy");
    auto pion_lo_copy = (TGraph*) pion_lo->Clone("pion_lo_copy");
    auto pion_hi_copy = (TGraph*) pion_hi->Clone("pion_hi_copy");

    auto kaon_copy = (TGraphErrors*) kaon->Clone("kaon_copy");
    auto kaon_lo_copy = (TGraph*) kaon_lo->Clone("kaon_lo_copy");
    auto kaon_hi_copy = (TGraph*) kaon_hi->Clone("kaon_hi_copy");

    auto prot_copy = (TGraphErrors*) prot->Clone("prot_copy");
    auto prot_lo_copy = (TGraph*) prot_lo->Clone("prot_lo_copy");
    auto prot_hi_copy = (TGraph*) prot_hi->Clone("prot_hi_copy");

    for(int jdx = 10; jdx < 18; jdx++)
    {
        pion->RemovePoint(10+1);
        kaon->RemovePoint(10);

        pion_hi->RemovePoint(10+1);
        kaon_hi->RemovePoint(10);

        pion_lo->RemovePoint(10+1);
        kaon_lo->RemovePoint(10);
    }

    for(int jdx = 24; jdx < 33; jdx++)
    {
        prot->RemovePoint(24);
        prot_hi->RemovePoint(24);
        prot_lo->RemovePoint(24);
    }


    //Fits silently and over the range and not plotting immediately
    pion->Fit("f_pion", "RQ0"); kaon->Fit("f_kaon", "RQ0"); prot->Fit("f_prot", "RQ0");
    pion_hi->Fit("f_pion_hi", "RQ0"); kaon_hi->Fit("f_kaon_hi", "RQ0"); prot_hi->Fit("f_prot_hi", "RQ0");
    pion_lo->Fit("f_pion_lo", "RQ0"); kaon_lo->Fit("f_kaon_lo", "RQ0"); prot_lo->Fit("f_prot_lo", "RQ0");

    //Mean
    double A1_pion, A2_pion, A3_pion, A4_pion;
    double A1_kaon, A2_kaon, A3_kaon, A4_kaon;
    double A1_prot, A2_prot, A3_prot, A4_prot;
    A1_pion = f_pion->GetParameter(0); A2_pion = f_pion->GetParameter(1); A3_pion = f_pion->GetParameter(2); A4_pion = f_pion->GetParameter(3);
    A1_kaon = f_kaon->GetParameter(0); A2_kaon = f_kaon->GetParameter(1); A3_kaon = f_kaon->GetParameter(2); A4_kaon = f_kaon->GetParameter(3);
    A1_prot = f_prot->GetParameter(0); A2_prot = f_prot->GetParameter(1); A3_prot = f_prot->GetParameter(2); A4_prot = f_prot->GetParameter(3);
    std::cout << "pion" << "{ " << A1_pion << " , " << A2_pion << " , " << A3_pion << " , " << A4_pion << "}" << "\n";
    std::cout << "kaon" << "{ " << A1_kaon << " , " << A2_kaon << " , " << A3_kaon << " , " << A4_kaon << "}" << "\n";
    std::cout << "prot" << "{ " << A1_prot << " , " << A2_prot << " , " << A3_prot << " , " << A4_prot << "}" << "\n";

    //Upper Bound (+1sigma)
    double A1_pion_hi, A2_pion_hi, A3_pion_hi, A4_pion_hi;
    double A1_kaon_hi, A2_kaon_hi, A3_kaon_hi, A4_kaon_hi;
    double A1_prot_hi, A2_prot_hi, A3_prot_hi, A4_prot_hi;
    A1_pion_hi = f_pion_hi->GetParameter(0); A2_pion_hi = f_pion_hi->GetParameter(1); A3_pion_hi = f_pion_hi->GetParameter(2); A4_pion_hi = f_pion_hi->GetParameter(3);
    A1_kaon_hi = f_kaon_hi->GetParameter(0); A2_kaon_hi = f_kaon_hi->GetParameter(1); A3_kaon_hi = f_kaon_hi->GetParameter(2); A4_kaon_hi = f_kaon_hi->GetParameter(3);
    A1_prot_hi = f_prot_hi->GetParameter(0); A2_prot_hi = f_prot_hi->GetParameter(1); A3_prot_hi = f_prot_hi->GetParameter(2); A4_prot_hi = f_prot_hi->GetParameter(3);
    std::cout << "pion_hi" << "{ " << A1_pion_hi << " , " << A2_pion_hi << " , " << A3_pion_hi << " , " << A4_pion_hi << "}" << "\n";
    std::cout << "kaon_hi" << "{ " << A1_kaon_hi << " , " << A2_kaon_hi << " , " << A3_kaon_hi << " , " << A4_kaon_hi << "}" << "\n";
    std::cout << "prot_hi" << "{ " << A1_prot_hi << " , " << A2_prot_hi << " , " << A3_prot_hi << " , " << A4_prot_hi << "}" << "\n";

    //lower bound (-1sigma)
    double A1_pion_lo, A2_pion_lo, A3_pion_lo, A4_pion_lo;
    double A1_kaon_lo, A2_kaon_lo, A3_kaon_lo, A4_kaon_lo;
    double A1_prot_lo, A2_prot_lo, A3_prot_lo, A4_prot_lo;
    A1_pion_lo = f_pion_lo->GetParameter(0); A2_pion_lo = f_pion_lo->GetParameter(1); A3_pion_lo = f_pion_lo->GetParameter(2); A4_pion_lo = f_pion_lo->GetParameter(3);
    A1_kaon_lo = f_kaon_lo->GetParameter(0); A2_kaon_lo = f_kaon_lo->GetParameter(1); A3_kaon_lo = f_kaon_lo->GetParameter(2); A4_kaon_lo = f_kaon_lo->GetParameter(3);
    A1_prot_lo = f_prot_lo->GetParameter(0); A2_prot_lo = f_prot_lo->GetParameter(1); A3_prot_lo = f_prot_lo->GetParameter(2); A4_prot_lo = f_prot_lo->GetParameter(3);
    std::cout << "pion_lo" << "{ " << A1_pion_lo << " , " << A2_pion_lo << " , " << A3_pion_lo << " , " << A4_pion_lo << "}" << "\n";
    std::cout << "kaon_lo" << "{ " << A1_kaon_lo << " , " << A2_kaon_lo << " , " << A3_kaon_lo << " , " << A4_kaon_lo << "}" << "\n";
    std::cout << "prot_lo" << "{ " << A1_prot_lo << " , " << A2_prot_lo << " , " << A3_prot_lo << " , " << A4_prot_lo << "}" << "\n";

    // ******************** Visuals + Saving into new Functions + Plotting ********************

    int pion_col = 857, kaon_col = 873, prot_col = 897; // blue, purple, red respectively

    //  mean
    auto fit_pion = new TF1("fdedxPionCenter", "[0] + exp([1] + [2]*x + [3]*x*x)", p_cutoff, p_max);
    auto fit_kaon = new TF1("fdedxKaonCenter", "[0] + exp([1] + [2]*x + [3]*x*x)", p_cutoff, p_max);
    auto fit_prot = new TF1("fdedxProtCenter", "[0] + exp([1] + [2]*x + [3]*x*x)", p_cutoff, p_max);
    fit_pion->SetParameter(0, A1_pion); fit_pion->SetParameter(1, A2_pion); fit_pion->SetParameter(2, A3_pion); fit_pion->SetParameter(3, A4_pion);
    fit_kaon->SetParameter(0, A1_kaon); fit_kaon->SetParameter(1, A2_kaon); fit_kaon->SetParameter(2, A3_kaon); fit_kaon->SetParameter(3, A4_kaon);
    fit_prot->SetParameter(0, A1_prot); fit_prot->SetParameter(1, A2_prot); fit_prot->SetParameter(2, A3_prot); fit_prot->SetParameter(3, A4_prot);

    fit_pion->SetLineColor(pion_col); fit_pion->SetLineWidth(2);
    fit_kaon->SetLineColor(kaon_col); fit_kaon->SetLineWidth(2);
    fit_prot->SetLineColor(prot_col); fit_prot->SetLineWidth(2);

    //  upper bound (+1sigma)
    auto fit_pion_hi = new TF1("fdedxPionSigmaHi", "[0] + exp([1] + [2]*x + [3]*x*x)", p_cutoff, p_max);
    auto fit_kaon_hi = new TF1("fdedxKaonSigmaHi", "[0] + exp([1] + [2]*x + [3]*x*x)", p_cutoff, p_max);
    auto fit_prot_hi = new TF1("fdedxProtSigmaHi", "[0] + exp([1] + [2]*x + [3]*x*x)", p_cutoff, p_max);
    fit_pion_hi->SetParameter(0, A1_pion_hi); fit_pion_hi->SetParameter(1, A2_pion_hi); fit_pion_hi->SetParameter(2, A3_pion_hi); fit_pion_hi->SetParameter(3, A4_pion_hi);
    fit_kaon_hi->SetParameter(0, A1_kaon_hi); fit_kaon_hi->SetParameter(1, A2_kaon_hi); fit_kaon_hi->SetParameter(2, A3_kaon_hi); fit_kaon_hi->SetParameter(3, A4_kaon_hi);
    fit_prot_hi->SetParameter(0, A1_prot_hi); fit_prot_hi->SetParameter(1, A2_prot_hi); fit_prot_hi->SetParameter(2, A3_prot_hi); fit_prot_hi->SetParameter(3, A4_prot_hi);

    fit_pion_hi->SetLineColor(pion_col); fit_pion_hi->SetLineWidth(2); fit_pion_hi->SetLineStyle(9);
    fit_kaon_hi->SetLineColor(kaon_col); fit_kaon_hi->SetLineWidth(2); fit_kaon_hi->SetLineStyle(9);
    fit_prot_hi->SetLineColor(prot_col); fit_prot_hi->SetLineWidth(2); fit_prot_hi->SetLineStyle(9);

    //  lower bound (-1sigma)
    auto fit_pion_lo = new TF1("fdedxPionSigmaLo", "[0] + exp([1] + [2]*x + [3]*x*x)", p_cutoff, p_max);
    auto fit_kaon_lo = new TF1("fdedxKaonSigmaLo", "[0] + exp([1] + [2]*x + [3]*x*x)", p_cutoff, p_max);
    auto fit_prot_lo = new TF1("fdedxProtSigmaLo", "[0] + exp([1] + [2]*x + [3]*x*x)", p_cutoff, p_max);
    fit_pion_lo->SetParameter(0, A1_pion_lo); fit_pion_lo->SetParameter(1, A2_pion_lo); fit_pion_lo->SetParameter(2, A3_pion_lo); fit_pion_lo->SetParameter(3, A4_pion_lo);
    fit_kaon_lo->SetParameter(0, A1_kaon_lo); fit_kaon_lo->SetParameter(1, A2_kaon_lo); fit_kaon_lo->SetParameter(2, A3_kaon_lo); fit_kaon_lo->SetParameter(3, A4_kaon_lo);
    fit_prot_lo->SetParameter(0, A1_prot_lo); fit_prot_lo->SetParameter(1, A2_prot_lo); fit_prot_lo->SetParameter(2, A3_prot_lo); fit_prot_lo->SetParameter(3, A4_prot_lo);

    fit_pion_lo->SetLineColor(pion_col); fit_pion_lo->SetLineWidth(2); fit_pion_lo->SetLineStyle(9);
    fit_kaon_lo->SetLineColor(kaon_col); fit_kaon_lo->SetLineWidth(2); fit_kaon_lo->SetLineStyle(9);
    fit_prot_lo->SetLineColor(prot_col); fit_prot_lo->SetLineWidth(2); fit_prot_lo->SetLineStyle(9);

    //  Canvas + Output Plots

    // Actual PID plot
    TCanvas *c = new TCanvas("c","PID",800,800);
    gPad->SetRightMargin(0.15);
    gPad->SetLogz();
    gStyle->SetPalette(kGreyScale);
    TH2hist->Draw("COLZ");

    pion_copy->Draw("same");
    kaon_copy->Draw("same");
    prot_copy->Draw("same");

    fit_pion->Draw("L same");
    fit_kaon->Draw("L same");
    fit_prot->Draw("L same");

    fit_pion_hi->Draw("L same");
    fit_kaon_hi->Draw("L same");
    fit_prot_hi->Draw("L same");

    fit_pion_lo->Draw("L same");
    fit_kaon_lo->Draw("L same");
    fit_prot_lo->Draw("L same");

    TLegend *leg = new TLegend(0.65, 0.65, 0.90, 0.90);
    leg->SetTextFont(42); leg->SetTextSize(0.04);
    leg->SetBorderSize(0); leg->SetFillStyle(0);
    leg->AddEntry(fit_pion,"pion","L");
    leg->AddEntry(fit_kaon,"kaon","L");
    leg->AddEntry(fit_prot,"proton","L");

    leg->Draw();
    c->Update();

    // No TGraph Data plot
    TCanvas *cfuncs = new TCanvas("cfunc","PID functions",800,800);
    gPad->SetRightMargin(0.15);
    gPad->SetLogz();
    gStyle->SetPalette(kGreyScale);
    TH2hist->Draw("COLZ");

    fit_pion->Draw("L same");
    fit_kaon->Draw("L same");
    fit_prot->Draw("L same");

    fit_pion_hi->Draw("L same");
    fit_kaon_hi->Draw("L same");
    fit_prot_hi->Draw("L same");

    fit_pion_lo->Draw("L same");
    fit_kaon_lo->Draw("L same");
    fit_prot_lo->Draw("L same");

    TLegend *leg1 = new TLegend(0.65, 0.65, 0.90, 0.90);
    leg1->SetTextFont(42); leg1->SetTextSize(0.04);
    leg1->SetBorderSize(0); leg1->SetFillStyle(0);
    leg1->AddEntry(fit_pion,"pion","L");
    leg1->AddEntry(fit_kaon,"kaon","L");
    leg1->AddEntry(fit_prot,"proton","L");

    cfuncs->Update();


    //Exporting the fitting functions into a TFile
    TFile *fexport = new TFile("pidfuncs.root", "RECREATE");
    fexport->cd();

    f_pion->Write();
    f_pion_hi->Write();
    f_pion_lo->Write();

    f_kaon->Write();
    f_kaon_hi->Write();
    f_kaon_lo->Write();

    f_prot->Write();
    f_prot_hi->Write();
    f_prot_lo->Write();

    fexport->Close();
    delete fexport;
}

void genHists(const double p_lo = 0, const double p_hi = 2.5, const double p_delta = .05, const int Nbins = 75) {
    std::vector<TH1D*> hists;
    std::vector<TCanvas*> cs;
    double x_min, x_max;
    x_min = 0; //1.5 for linear scale
    x_max = 5;
    const int num_iter = 1 + (p_hi - p_lo)/p_delta;
    for(int i = 0; i < num_iter; i++) {
        double p_min = p_lo + i*p_delta; double p_max = p_min + p_delta;

        TH1D *hist = new TH1D(Form("hist%.2f_%.2f", p_min, p_max), Form("dE/dx %.2f - %.2f", p_min, p_max), Nbins, x_min, x_max);
        hist->GetXaxis()->SetTitle("log(dE/dx) (MeV/cm)");
        hist->GetYaxis()->SetTitle("Entries");
        hist->Sumw2();
        hists.push_back(hist);
    }

    // TFile *fInput = new TFile("20250623_Skim_2023Data_Feb2025ReReco_HIForward10-11.root");
    TFile *fInput = TFile::Open("combined.root");
    TTree *tree = (TTree*)fInput->Get("Tree");
    long int nentries = tree->GetEntries();

    int Dsize; //how many D-meson candidates there are
    std::vector<double> *Dtrk1Pt = nullptr;
    std::vector<double> *Dtrk1Eta = nullptr; //pseudorapidity
    std::vector<double> *Dtrk1dedx = nullptr;
    tree->SetBranchAddress("Dsize", &Dsize);
    tree->SetBranchAddress("Dtrk1Eta", &Dtrk1Eta);
    tree->SetBranchAddress("Dtrk1Pt", &Dtrk1Pt);
    tree->SetBranchAddress("Dtrk1dedx", &Dtrk1dedx);

    for (long int jentry=0; jentry<nentries;jentry++) {
    tree->GetEntry(jentry);
    // if (jentry%1'000'000==0) std::cout << "Processing entry " << jentry << std::endl;
    for (int i=0; i<Dsize; i++) {
        double p = (Dtrk1Pt->at(i)) * cosh(Dtrk1Eta->at(i)); //change p_value name
        for(int k = 0; k < num_iter; k++){
            double p_min = p_lo + k*p_delta; double p_max = p_min + p_delta;
            if (p < p_min || p > p_max) continue;
            hists[k]->Fill(log(Dtrk1dedx->at(i)));
        }}}

    TFile *f = new TFile("mc_histograms.root", "RECREATE");
    for(int i = 0; i < num_iter; i++) {
        f->cd();
        hists[i]->Write();
    }
    f->Close();
    delete f;

    for(int i = 0; i < num_iter; i++){
        double p_min = p_lo + i*p_delta; double p_max = p_min + p_delta;
        TCanvas *c = new TCanvas(Form("c%.2f_%.2f", p_min, p_max), Form("c%.2f-%.2f", p_min, p_max));
        hists[i]->Draw();
        cs.push_back(c);
    }
}

void TH2Gen(double xmin = 0, double xmax = 2.5, double ymin = 0, double ymax = 5, int ybins = 75){
    //  Generates the dE/dx vs p 2D histogram (outputs a TFile containing the TH2)
    // TFile *fInput = new TFile("20250623_Skim_2023Data_Feb2025ReReco_HIForward10-11.root");
    TFile *fInput = new TFile("combined.root");
    TTree *tree = (TTree*)fInput->Get("Tree");

    long int nentries = tree->GetEntries();
    int Dsize; //how many D-meson candidates there are
    std::vector<double> *Dtrk1Pt = nullptr;
    std::vector<double> *Dtrk1Eta = nullptr; //pseudorapidity
    std::vector<double> *Dtrk1dedx = nullptr;

    tree->SetBranchAddress("Dsize", &Dsize);
    tree->SetBranchAddress("Dtrk1Eta", &Dtrk1Eta);
    tree->SetBranchAddress("Dtrk1Pt", &Dtrk1Pt);
    tree->SetBranchAddress("Dtrk1dedx", &Dtrk1dedx);

    int xbins = 50;

    TH2D *hist = new TH2D("hist", "log(dE/dx) vs p", xbins, xmin, xmax, ybins, ymin, ymax);
    hist->GetXaxis()->SetTitle("p (GeV)");
    hist->GetYaxis()->SetTitle("log(dE/dx) (MeV/cm)");
    hist->SetStats(0);

    for (long int jentry = 0; jentry < nentries; jentry++) {
        tree->GetEntry(jentry);
        // if (jentry%1'000'000==0) std::cout << "Processing entry " << jentry << std::endl;
        for (int i=0; i < Dsize; i++) {
            double pT = Dtrk1Pt->at(i);
            double eta = Dtrk1Eta->at(i);
            double dedx = Dtrk1dedx->at(i);
            double p = pT * std::cosh(eta);

            hist->Fill(p, log(dedx));
        }
    }

    TCanvas *c = new TCanvas("dedx_p_2dhist", "log(dE/dx) vs p", 1000, 1000);
    c->SetLogz();
    hist->Draw("COLZ");

    TFile *f = new TFile("mergedBkg.root", "RECREATE");
    f->cd();
    hist->Write();
    f->Close();
    delete f;

}

// ******************** Debugging/Helper functions ********************

void plot(double p_min, double p_delta = .05){
    //plots one of the histograms corresponding to a certain p_min to p_max range
    auto *f = TFile::Open("logscale_histograms.root");
    double p_max = p_min + p_delta;

    TH1D *hist = (TH1D*) f->Get(Form("hist%.2f_%.2f", p_min, p_max));

    TCanvas *c = new TCanvas(Form("c%.2f", p_min * 10), Form("%.2f < p < %.2f", p_min, p_max));
    hist->Draw();
}

void histFit(double p_min = .3, double removeWindow = .3) {
    double p_max = p_min + .05;

    TFile *f = TFile::Open("data_histograms.root");
    TH1D *hist = (TH1D*)f->Get(Form("hist%.2f_%.2f", p_min, p_max));

    TCanvas *dEdxDist = new TCanvas("dEdxDist", "dEdxDist", 1000, 500);
    hist->Draw(); TLatex latex; latex.SetNDC(); latex.SetTextSize(0.04);
    latex.DrawLatex(0.65, 0.85, Form("%.2f < p < %.2f",p_min, p_max));

    std::vector<std::pair<double, double>> peaks;
    TH1D* histCopy = (TH1D*) hist->Clone("htmp");

    if(p_min < .3) std::__throw_range_error("incorrect range");

    else if(p_min < .9 && p_min >= .299)
    {
        for(int i=0; i < 3; i++)
        { //loops through and finds 3 maximum bins for initial guesses
            int maxBin = histCopy->GetMaximumBin();
            double mu = histCopy->GetBinCenter(maxBin);
            double A = histCopy->GetBinContent(maxBin);
            peaks.emplace_back(mu, A);
            int bLow = histCopy->GetXaxis()->FindBin(mu - removeWindow);
            int bHigh = histCopy->GetXaxis()->FindBin(mu + removeWindow);
            for(int b=bLow; b <= bHigh; b++) histCopy->SetBinContent(b, 0);
        }
        delete histCopy;

        std::sort(peaks.begin(), peaks.end(), //puts into correct order
                    [](auto &a, auto &b){ return a.first < b.first; });

        TF1 *fit = new TF1( "fit",
                            "gaus(0) + gaus(3) + gaus(6)",
                            hist->GetXaxis()->GetXmin(),
                            hist->GetXaxis()->GetXmax());

        for(int i=0; i<3; i++) {
            double mu = peaks[i].first;
            double A = peaks[i].second;
            fit->SetParameter(0+3*i, A);
            fit->SetParameter(1+3*i, mu);
            fit->SetParLimits(1+3*i, mu - removeWindow, mu + removeWindow);
            fit->SetParameter(2+3*i, .1);
            fit->SetParLimits(2+3*i, .01, .5);
        }

        hist->Fit(fit, "RQ");
        double  mean_pion = fit->GetParameter(1), sigma_pion = fit->GetParameter(2),
                mean_kaon = fit->GetParameter(4), sigma_kaon = fit->GetParameter(5),
                mean_proton = fit->GetParameter(7), sigma_proton = fit->GetParameter(8);

        std::cout   << "\n *********Results********* \n"
                    << "Pion:   mu = " << mean_pion << "    sigma = " << sigma_pion << "\n"
                    << "Kaon:   mu = " << mean_kaon << "    sigma = " << sigma_kaon << "\n"
                    << "Proton: mu = " << mean_proton << "    sigma = " << sigma_proton << "\n";

        fit->SetLineColor(kBlack);
        fit->Draw("SAME");
    }
    else if(p_min >= .9 && p_min < 1.70)
    {
        for(int i=0; i < 2; i++)
        {   //loops through and finds 2 maximum bins for initial guesses
            int maxBin = histCopy->GetMaximumBin();
            double mu = histCopy->GetBinCenter(maxBin);
            double A = histCopy->GetBinContent(maxBin);
            peaks.emplace_back(mu, A);
            int bLow = histCopy->GetXaxis()->FindBin(mu - removeWindow);
            int bHigh = histCopy->GetXaxis()->FindBin(mu + removeWindow);
            for(int b=bLow; b <= bHigh; b++) histCopy->SetBinContent(b, 0);
        }
        delete histCopy;

        std::sort(peaks.begin(), peaks.end(), //puts into correct order
                    [](auto &a, auto &b){ return a.first < b.first; });

        TF1 *fit = new TF1( "fit",
                            "gaus(0) + gaus(3)",
                            hist->GetXaxis()->GetXmin(),
                            hist->GetXaxis()->GetXmax());

        for(int i=0; i<2; i++)
        {
            double mu = peaks[i].first;
            double A = peaks[i].second;
            fit->SetParameter(0+3*i, A);
            fit->SetParameter(1+3*i, mu);
            fit->SetParLimits(1+3*i, mu - removeWindow, mu + removeWindow);
            fit->SetParameter(2+3*i, .1);
            fit->SetParLimits(2+3*i, .01, .5);
        }

        hist->Fit(fit, "RQ");
        double  mean_pion = fit->GetParameter(1), sigma_pion = fit->GetParameter(2),
                mean_kaon = fit->GetParameter(1), sigma_kaon = fit->GetParameter(2),
                mean_proton = fit->GetParameter(4), sigma_proton = fit->GetParameter(5);

        std::cout   << "\n *********Results********* \n"
                        << "Pion:   mu = " << mean_pion << "    sigma = " << sigma_pion << "\n"
                        << "Kaon:   mu = " << mean_kaon << "    sigma = " << sigma_kaon << "\n"
                        << "Proton: mu = " << mean_proton << "    sigma = " << sigma_proton << "\n";

        fit->SetLineColor(kBlack);
        fit->Draw("SAME");
    }

    else{
        TF1 *fit = new TF1( "fit",
                            "gaus",
                            hist->GetXaxis()->GetXmin(),
                            hist->GetXaxis()->GetXmax());

        hist->Fit(fit, "RQ0");
        double  mean_pion = fit->GetParameter(1), sigma_pion = fit->GetParameter(2),
                mean_kaon = fit->GetParameter(1), sigma_kaon = fit->GetParameter(2),
                mean_proton = fit->GetParameter(1), sigma_proton = fit->GetParameter(2);

        std::cout   << "\n *********Results********* \n"
                        << "Pion:   mu = " << mean_pion << "    sigma = " << sigma_pion << "\n"
                        << "Kaon:   mu = " << mean_kaon << "    sigma = " << sigma_kaon << "\n"
                        << "Proton: mu = " << mean_proton << "    sigma = " << sigma_proton << "\n";

        fit->SetLineColor(kBlack);
        fit->Draw("SAME");
    }
}





//     if(p_min < 1.1){
//     TH1F* dedxCopy = (TH1F*) hist->Clone("htmp");
//     std::vector<std::pair<double, double>> peaks; // (mean, amplitude pairs)

//     for(int i=0; i < 3; i++) { //loops through and finds 3 maximum bins for initial guesses
//         int maxBin = dedxCopy->GetMaximumBin();
//         double mu = dedxCopy->GetBinCenter(maxBin);
//         double A = dedxCopy->GetBinContent(maxBin);
//         peaks.emplace_back(mu, A);
//         int bLow = dedxCopy->GetXaxis()->FindBin(mu - removeWindow);
//         int bHigh = dedxCopy->GetXaxis()->FindBin(mu + removeWindow);
//         for(int b=bLow; b <= bHigh; b++){
//             dedxCopy->SetBinContent(b, 0);
//         }
//     }
//     delete dedxCopy;

//     std::sort(peaks.begin(), peaks.end(), //puts into correct order
//                 [](auto &a, auto &b){ return a.first < b.first; });

//     TF1 *fit = new TF1( "fit",
//                         "gaus(0) + gaus(3) + gaus(6)",
//                         hist->GetXaxis()->GetXmin(),
//                         hist->GetXaxis()->GetXmax());

//     for(int i=0; i<3; i++) {
//         double mu = peaks[i].first;
//         double A = peaks[i].second;
//         fit->SetParameter(0+3*i, A);
//         fit->SetParameter(1+3*i, mu);
//         fit->SetParLimits(1+3*i, mu - removeWindow, mu + removeWindow);
//         fit->SetParameter(2+3*i, .1);
//         fit->SetParLimits(2+3*i, .01, .5);
//     }

    // hist->Fit(fit, "RQ");
    // double  mean_pion = fit->GetParameter(1), sigma_pion = fit->GetParameter(2),
    //         mean_kaon = fit->GetParameter(4), sigma_kaon = fit->GetParameter(5),
    //         mean_proton = fit->GetParameter(7), sigma_proton = fit->GetParameter(8);

    // std::cout   << "\n *********Results********* \n"
    //             << "Pion:   mu = " << mean_pion << "    sigma = " << sigma_pion << "\n"
    //             << "Kaon:   mu = " << mean_kaon << "    sigma = " << sigma_kaon << "\n"
    //             << "Proton: mu = " << mean_proton << "    sigma = " << sigma_proton << "\n";

    // fit->SetLineColor(kBlack);
    // fit->Draw("SAME");}
    // //2-fit (pion-kaon indistinguishability)
    // else{
    // TH1F* dedxCopy = (TH1F*) hist->Clone("htmp");
    // std::vector<std::pair<double, double>> peaks; // (mean, amplitude pairs)

//     for(int i=0; i < 2; i++) { //loops through and finds 2 maximum bins for initial guesses
//         int maxBin = dedxCopy->GetMaximumBin();
//         double mu = dedxCopy->GetBinCenter(maxBin);
//         double A = dedxCopy->GetBinContent(maxBin);
//         peaks.emplace_back(mu, A);
//         int bLow = dedxCopy->GetXaxis()->FindBin(mu - removeWindow);
//         int bHigh = dedxCopy->GetXaxis()->FindBin(mu + removeWindow);
//         for(int b=bLow; b <= bHigh; b++){
//             dedxCopy->SetBinContent(b, 0);
//         }
//     }
//     delete dedxCopy;

//     std::sort(peaks.begin(), peaks.end(), //puts into correct order
//                 [](auto &a, auto &b){ return a.first < b.first; });

//     TF1 *fit = new TF1( "fit",
//                         "gaus(0) + gaus(3)",
//                         hist->GetXaxis()->GetXmin(),
//                         hist->GetXaxis()->GetXmax());

//     for(int i=0; i<2; i++) {
//         double mu = peaks[i].first;
//         double A = peaks[i].second;
//         fit->SetParameter(0+3*i, A);
//         fit->SetParameter(1+3*i, mu);
//         fit->SetParLimits(1+3*i, mu - removeWindow, mu + removeWindow);
//         fit->SetParameter(2+3*i, .1);
//         fit->SetParLimits(2+3*i, .01, .5);
//     }

//     hist->Fit(fit, "RQ");
//     double  mean_pion = fit->GetParameter(1), sigma_pion = fit->GetParameter(2),
//             mean_kaon = fit->GetParameter(1), sigma_kaon = fit->GetParameter(2),
//             mean_proton = fit->GetParameter(4), sigma_proton = fit->GetParameter(5);

//     std::cout   << "\n *********Results********* \n"
//                 << "Pion:   mu = " << mean_pion << "    sigma = " << sigma_pion << "\n"
//                 << "Kaon:   mu = " << mean_kaon << "    sigma = " << sigma_kaon << "\n"
//                 << "Proton: mu = " << mean_proton << "    sigma = " << sigma_proton << "\n";

//     fit->SetLineColor(kBlack);
//     fit->Draw("SAME");}
