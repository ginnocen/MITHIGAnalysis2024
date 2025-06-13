#include <TCanvas.h>
#include <TCut.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TLegend.h>
#include <TTree.h>

TH1D* hist(const char* file, const char* name, const char* label, int linecolor, float xmin, float xmax, float ymin, float ymax){

    TFile*f = TFile::Open(file);
    TH1D*h = (TH1D*)f->Get(name);

    //h->SetTitle(label);
    h->SetLineColor(linecolor);
    h->SetLineWidth(2);
    h->SetStats(0);
    if(xmin != xmax){
    h->GetXaxis()->SetRangeUser(xmin, xmax);
    }
    if(ymin != ymax){
    h->GetYaxis()->SetRangeUser(ymin, ymax);
    }
    return h;
}

void plotter(){

    TH1D*mult_hij = hist("output_hijing.root", "hMult", "Hijing", kBlack, 0, 100, 0, 0);
    TH1D*mult_dd = hist("output_dd.root", "hMult", "Double Diffractive", kBlue, 0, 100, 0, 0);
    TH1D*mult_sd = hist("output_sd.root", "hMult", "Single Diffractive", kGreen, 0, 100, 0, 0);
    TH1D*mult_ao = hist("output_alphaO.root", "hMult", "Alpha-Oxygen", kRed, 0, 100, 0, 0);

    TLegend* leg = new TLegend(0.6, 0.7, 0.9, 0.9);
    leg->SetBorderSize(0);
    leg->SetFillColor(0);
    leg->SetTextSize(0.04);
    leg->AddEntry(mult_hij, "Hijing", "l");
    leg->AddEntry(mult_dd, "Double Diffractive", "l");
    leg->AddEntry(mult_sd, "Single Diffractive", "l");
    leg->AddEntry(mult_ao, "Alpha-Oxygen", "l");

    TCanvas *c = new TCanvas("c", "Multiplicity Comparison", 1000, 1000);
    mult_hij->Draw("HIST SAME");
    mult_dd->Draw("HIST SAME");
    mult_sd->Draw("HIST SAME");
    mult_ao->Draw("HIST SAME");
    leg->Draw();

    c->SetLogy();
    c->SaveAs("MultiplicityComparison.pdf");

}

