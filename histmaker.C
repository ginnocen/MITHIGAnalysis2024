#include <TCanvas.h>
#include <TCut.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TLegend.h>
#include <TNtuple.h>
#include <TTree.h>
using namespace std;

TH1D* h1(
    const char* infile, 
    const char* histogram, 
    const char* title,
    const char* xlabel, 
    const char* ylabel, 
    int color,
    double xlow,
    double xhi,
    double ylow,
    double yhi,
    int rebinn
    ){
    
     cout << infile << endl;
    TFile*a0 = TFile::Open(infile,"READ"); 
    TH1D*h = (TH1D*)a0->Get(histogram);

    h->SetStats(0);
    h->SetLineWidth(3);
    h->SetLineColor(color);
    
    h->SetTitle(title);
    h->GetXaxis()->SetTitle(xlabel);
    h->GetYaxis()->SetTitle(ylabel);

    //cout << h->GetNbinsX() << endl;
    //h->Scale(1/h->Integral());
    h->Rebin(rebinn);
    
    if(xhi != xlow){h->GetXaxis()->SetRangeUser(xlow, xhi);}
    if(yhi != ylow){h->GetYaxis()->SetRangeUser(ylow, yhi);}


    h->SetFillColorAlpha(color,0.1);

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

    //h->Scale(1/h->Integral());

    return h;

}

void histmaker(){

    // 8,077,9gm1gm1 <-Data
    //10,008,000 <-MC

    TH1D*h_1 = h1("dec23_3_gm1.root","hdRData","","dR (rad)","Counts",1,0,0.5,0,250,1);
    TH1D*h_2 = h1("dec23_3_cc_gm1.root","hdRData","","dR (rad)","Counts",2,0,0.5,0,250,1);
    TH1D*h_3 = h1("dec23_3_ccbb_gm1.root","hdRData","","dR (rad)","Counts",3,0,0.5,0,250,1);
    TH1D*h_4 = h1("dec23_3_cb_gm1.root","hdRData","","dR (rad)","Counts",4,0,0.5,0,250,1);
    TH1D*h_5 = h1("dec23_3_c_gm1.root","hdRData","","dR (rad)","Counts",6,0,0.5,0,250,1);
    TH1D*h_6 = h1("dec23_3_00_gm1.root","hdRData","","dR (rad)","Counts",7,0,0.5,0,250,1);
    TH1D*h_7 = h1("dec23_3_b_gm1.root","hdRData","","dR (rad)","Counts",8,0,0.5,0,250,1);
    



    TH1D*h_9 = (TH1D*)h_1->Clone("h8");
    h_9->Add(h_2, -1); // Subtract h_2
    h_9->Add(h_3, -1); // Subtract h_3
    h_9->Add(h_4, -1); // Subtract h_4
    h_9->Add(h_5, -1); // Subtract h_5
    h_9->Add(h_6, -1); // Subtract h_6
    h_9->Add(h_7, -1); // Subtract h_7

    vector<TH1D*> hists = {h_2,h_3,h_4,h_5,h_6,h_7};
    



    TLegend*G = new TLegend(0.48,0.55,0.80,0.87);
    //G->AddEntry(h_1, "All Jets", "lf");
    G->AddEntry(h_2, "C Hadrons = 2, B hadrons = 0", "lf");
    G->AddEntry(h_3, "C Hadrons = 2, B hadrons = 2", "lf");
    G->AddEntry(h_4, "C Hadrons = 1, B hadrons = 1", "lf");
    G->AddEntry(h_5, "C Hadrons = 1, B hadrons = 0", "lf");
    G->AddEntry(h_7, "C Hadrons = 0, B hadrons = 1", "lf");
    G->AddEntry(h_6, "C Hadrons = 0, B hadrons = 0", "lf");
    G->AddEntry(h_9, "Other", "lf");
    G->SetBorderSize(0); // Removes border
    G->SetFillStyle(0);  // Removes background fill
    //G->AddEntry(h_8, "C Hadrons = 3, B hadrons = 0", "lf");
    
 


    TCanvas*a = new TCanvas("a","a",100-7,100);
    a->SetMargin(0.13, 0.13, 0.13, 0.13);
    //a->SetLogx();
    //a->SetLogy();

    
   h_6->Draw("colz same");
   h_4->Draw("colz same");
   h_3->Draw("colz same");
   h_7->Draw("colz same");
   h_9->Draw("colz same");
   h_2->Draw("colz same");
   h_5->Draw("colz same");
    G->Draw();
   // h_1->Draw("colz");
    

    a->Update();
    a->SaveAs("h.pdf");


}