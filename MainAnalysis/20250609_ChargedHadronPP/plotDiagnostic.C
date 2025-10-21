#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include <iostream>

#include "include/plotting.h" // Kyle's plotting utilities

void plotDiagnostic() {

    const char* input =   "output/20250706_Skim_ppref2024_debug_noTrackEventSelection_min04.root";
    const char* input_TrackWeighted = "output/20250706_Skim_ppref2024_debug_noTrackEventSelection_min04_trackWeighted.root";
    const char* output =  "plots/diagnostic_ppref2024_debug_noTrackEventSelection_min04";

    TFile* fin = TFile::Open(input, "READ");
    if (!fin || fin->IsZombie()) {
        std::cerr << "Error: Unable to open file " << fin << std::endl;
        return;
    }

    TFile* fin_corr = TFile::Open(input_TrackWeighted, "READ");
    if (!fin_corr || fin_corr->IsZombie()) {
        std::cerr << "Error: Unable to open file " << input_TrackWeighted << std::endl;
        return;
    }

    TH1D* hNEvtPassCuts = (TH1D*)fin->Get("hNEvtPassCuts");
    TH1D* hNTrkPassCuts = (TH1D*)fin->Get("hNTrkPassCuts");
    TH2D* hTrkPtEta = (TH2D*)fin->Get("hTrkPtEta");
    TH1D* hTrkPt = (TH1D*) hTrkPtEta->ProjectionX("hTrkPt");
    TH1D* hTrkEta = (TH1D*) hTrkPtEta->ProjectionY("hTrkEta");
    TH1D* hMult = (TH1D*)fin->Get("hMult");

    TH2D* hTrkPtEta_corr = (TH2D*)fin_corr->Get("hTrkPtEta");
    hTrkPtEta_corr->SetName("hTrkPtEta_corr");
    TH1D* hTrkPt_corr = (TH1D*) hTrkPtEta_corr->ProjectionX("hTrkPt_corr");
    TH1D* hTrkEta_corr = (TH1D*) hTrkPtEta_corr->ProjectionY("hTrkEta_corr");
    TH1D* hMult_corr = (TH1D*)fin_corr->Get("hMult");
    TH1D* hTrkWeight = (TH1D*)fin_corr->Get("hTrkWeight");
    TH2D* hTrkWeightPt = (TH2D*)fin_corr->Get("hTrkWeightPt");
    TH2D* hTrkWeightEta = (TH2D*)fin_corr->Get("hTrkWeightEta");

    // Divide by bin width
    divideByWidth(hTrkPt);
    divideByWidth(hTrkEta);
    divideByWidth(hTrkPt_corr);
    divideByWidth(hTrkEta_corr);

    // make canvas
    TCanvas* c1 = new TCanvas("c1", "c1", 1600, 2400);
    c1->Divide(2, 4);

    c1->cd(1);
    plotSimple(
        {hNEvtPassCuts}, "NEvtPassCuts", {"NEvtPassCuts"},
        "", -1, -1,
        "Counts", -1, -1,
        false, false,
        true
    );

    c1->cd(2);
    plotSimple(
        {hNTrkPassCuts}, "NTrkPassCuts", {"NTrkPassCuts"},
        "", -1, -1,
        "Counts", -1, -1,
        false, false,
        true
    );
    // print bin contents without scientific notation
    std::cout<< "NTrkPassCuts bin contents: "<<endl;
    std::cout << std::fixed;
    for (int i = 1; i <= hNTrkPassCuts->GetNbinsX(); ++i) {
        std::cout << hNTrkPassCuts->GetBinContent(i) <<endl;
    }
    std::cout << std::endl;

    c1->cd(3);
    plotSimple(
        {hTrkPt, hTrkPt_corr}, "hTrkPt", {"hTrkPt", "hTrkPt w trkCorr"},
        "Track pT [GeV/c]", 1, 20,
        "dN/dp_{T}", 1, 1e7,
        false, true
    );
    
    c1->cd(4);
    plotSimple(
        {hTrkEta, hTrkEta_corr}, "hTrkEta", {"hTrkEta", "hTrkEta w trkCorr"},
        "Track #eta", -3, 3,
        "dN/d#eta", 1, 1e7,
        false, true
    );

    c1->cd(5);
    plotSimple(
        {hMult, hMult_corr}, "hMult", {"hMult", "hMult w trkCorr"},
        "Multiplicity", 0, 200,
        "dN/dN_{ch}", -1, -1,
        false, false
    );

    c1->cd(6);
    plotSimple(
        {hTrkWeight}, "hTrkWeight", {"hTrkWeight"},
        "Track Weight", 0, 5,
        "Counts", -1, -1,
        false, false
    );

    c1->cd(7);
    plot2D(
        hTrkWeightPt, "hTrkWeightPt",
        "Track pT [GeV/c]", 0.4, 20,
        "Track Weight", 0.9, 1.5
    );

    c1->cd(8);
    plot2D(
        hTrkWeightEta, "hTrkWeightEta",
        "Track #eta", -1, 1,
        "Track Weight", 0.9, 1.5
    );

    c1->SaveAs(Form("%s.png", output));

}