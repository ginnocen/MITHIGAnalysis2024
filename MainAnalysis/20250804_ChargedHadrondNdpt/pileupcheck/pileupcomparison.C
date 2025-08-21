#include <iostream>
#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>

using namespace std;

void pileupcomparison(){
    gStyle->SetOptStat(0);

    cout << "start" << endl;

    TFile* fhipu = TFile::Open("output_hipileup_2/MergedOutput.root");
    TH1D* hipu = (TH1D*)fhipu->Get("hTrkPt");

    TFile* flopu = TFile::Open("output_lowpileup_2/MergedOutput.root");
    TH1D* hlopu = (TH1D*)flopu->Get("hTrkPt");

    TFile* fcentral = TFile::Open("MergedOutput_60PDs.root");
    TH1D* hcentral = (TH1D*)fcentral->Get("hTrkPt");

    //float bril_hipu = 50553.929790248;
    //float bril_lopu = 4032.054115993;
    //float bril_central = 806882.576415303;

    float bril_hipu = 254310.520042996;
    float bril_lopu = 294336.147416517;
    float bril_central = 806882.576415303;

    hipu->Scale(1/bril_hipu);
    hlopu->Scale(1/bril_lopu);
    hcentral->Scale(1/bril_central);

    hlopu->SetLineColor(2);
    hcentral->SetLineColor(3);

    hipu->GetXaxis()->SetTitle("Track p_{T} [GeV]");
    hipu->GetYaxis()->SetTitle("d#sigma / dpT");

    TLegend*l = new TLegend(0.57, 0.67, 0.88, 0.88);
    l->AddEntry(hipu, "High Pileup - 394272 [1, 357] PU = 0.1115", "l");
    l->AddEntry(hlopu, "Low Pileup - 394272 [1, 10] PU = 0.0960", "l");
    l->AddEntry(hcentral, "Central Values", "l");
    l->SetBorderSize(0);

    TCanvas* c = new TCanvas("c", "Pileup Comparison", 800, 600);
    c->cd();
    c->SetLogy();
    c->SetLogx();

    hipu->Draw("HIST E");
    hlopu->Draw("HIST E SAME");
    hcentral->Draw("HIST E SAME");
    l->Draw();

    c->SaveAs("pileup_comparison.pdf");




     TH1D* hipuratio = (TH1D*)hipu->Clone("hipuratio");
     hipuratio->Divide(hcentral);

     TH1D* lopuratio = (TH1D*)hlopu->Clone("lopuratio");
     lopuratio->Divide(hcentral);

     hipuratio->GetXaxis()->SetTitle("Track p_{T} [GeV]");
     hipuratio->GetYaxis()->SetTitle("High/Low Pileup Ratio");
     hipuratio->SetMinimum(0.9);
     hipuratio->SetMaximum(1.1);

     hipuratio->SetLineColor(2);
     lopuratio->SetLineColor(4);

    TLegend* l2 = new TLegend(0.15, 0.67, 0.45, 0.88);
    l2->AddEntry(hipuratio, "High PU / Central values", "l");
    l2->AddEntry(lopuratio, "Low PU / Central values", "l");
    l2->SetBorderSize(0);

    TCanvas* c2 = new TCanvas("c2", "Pileup Ratio Comparison", 800, 600);
    c2->cd();
    c2->SetLogx();

    hipuratio->Draw("HIST E");
    lopuratio->Draw("HIST E SAME");
    l2->Draw();
    
    // Add dashed line at y=1
    TLine* line = new TLine(hipuratio->GetXaxis()->GetXmin(), 1, hipuratio->GetXaxis()->GetXmax(), 1);
    line->SetLineColor(kBlack);
    line->SetLineStyle(2);
    line->Draw("same");

    c2->SaveAs("pileup_ratio_comparison.pdf");


    TH1D* intra = (TH1D*)hipu->Clone("intra");
    intra->Divide(hlopu);

    intra->GetXaxis()->SetTitle("Track p_{T} [GeV]");
    intra->GetYaxis()->SetTitle("High/Low Pileup Ratio");
    intra->SetMinimum(0.9);
    intra->SetMaximum(1.1);


    TCanvas* c3 = new TCanvas("c3", "HIGH / LOW Comparison", 800, 600);
    c3->cd();
    c3->SetLogx();

    intra->Draw("HIST E");

    TLine* line2 = new TLine(intra->GetXaxis()->GetXmin(), 1, intra->GetXaxis()->GetXmax(), 1);
    line2->SetLineColor(kBlack);
    line2->SetLineStyle(2);
    line2->Draw("same");

    c3->SaveAs("intra_pileup_comparison.pdf");


}