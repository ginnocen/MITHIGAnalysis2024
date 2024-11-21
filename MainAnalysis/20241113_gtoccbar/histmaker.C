#include <TCanvas.h>
#include <TCut.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TLegend.h>
#include <TNtuple.h>
#include <TTree.h>

TH1D* h1(
    const char* infile, 
    const char* histogram, 
    const char* xlabel, 
    const char* ylabel, 
    int color,
    double xlow,
    double xhi,
    double ylow,
    double yhi
    ){

    TFile*a0 = TFile::Open(infile,"READ"); 
    TH1D*h = (TH1D*)a0->Get(histogram);

    h->SetStats(0);
    h->SetLineWidth(3);
    h->SetLineColor(color);
    h->SetFillColorAlpha(color,0.1);
    h->GetXaxis()->SetTitle(xlabel);
    h->GetYaxis()->SetTitle(ylabel);
    
    if(xhi != xlow){h->GetXaxis()->SetRangeUser(xlow, xhi);}
    if(yhi != ylow){h->GetYaxis()->SetRangeUser(ylow, yhi);}

    return h;

}

TH2D* h2(
    const char* infile, 
    const char* histogram, 
    const char* xlabel, 
    const char* ylabel, 
    const char* zlabel, 
    double xlow,
    double xhi,
    double ylow,
    double yhi,
    double zlow,
    double zhi
    ){

    TFile*a0 = TFile::Open(infile,"READ"); 
    TH2D*h = (TH2D*)a0->Get(histogram);

    h->SetStats(0);
    h->GetXaxis()->SetTitle(xlabel);
    h->GetYaxis()->SetTitle(ylabel);
    h->GetZaxis()->SetTitle(zlabel);
    
    if(xhi != xlow){h->GetXaxis()->SetRangeUser(xlow, xhi);}
    if(yhi != ylow){h->GetYaxis()->SetRangeUser(ylow, yhi);}
    if(zhi != zlow){h->GetZaxis()->SetRangeUser(zlow, zhi);}

    return h;

}

void histmaker(){


    TH1D*h_mass = h1(
        "cuts.root",
        "hMuMuMassData",
        "Invariant Mass (GeV)",
        "Counts",
        1,
        0,
        10,
        0,
        0
    );


    TLegend*G = new TLegend(0.8,0.8,0.98,0.98);
    G->AddEntry(h_mass, "Dimuon Invariant Mass", "lf");
 

    TCanvas*a = new TCanvas("a","a",1000,1000);
    a->SetMargin(0.15, 0.1, 0.1, 0.1);
    //a->SetLogx();
    //a->SetLogy();
    
    h_mass->Draw("colz");
    G->Draw();
    

    a->Update();
    a->SaveAs("plots/dimuon_invariantmass.pdf");


}