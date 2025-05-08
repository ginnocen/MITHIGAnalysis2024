#include <TCanvas.h>
#include <TCut.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TLegend.h>
#include <TNtuple.h>
#include <TTree.h>
#include <iostream>
using namespace std;



void look(){
    gStyle->SetOptStat(0);

    TFile*f = new TFile("/data00/g2ccbar/mc2018/skim_05012025/mergedfile.root");
    TTree *T = (TTree*)f->Get("Tree");

    vector<int> *nsvtx = nullptr;
    T->SetBranchAddress("jtNsvtx", &nsvtx);
    
    vector<int> *jtNtrk = nullptr;
    T->SetBranchAddress("jtNtrk", &jtNtrk);

    vector<vector<int>> *svtxNtrk = nullptr;
    T->SetBranchAddress("svtxNtrk", &svtxNtrk);

    vector<bool> *isMuMu = nullptr;
    T->SetBranchAddress("IsMuMuTagged", &isMuMu);

    vector<float> *mu1pt = nullptr;
    T->SetBranchAddress("muPt1", &mu1pt);
    vector<float> *mu2pt = nullptr;
    T->SetBranchAddress("muPt2", &mu2pt);
    vector<float> *jetpt = nullptr;
    T->SetBranchAddress("JetPT", &jetpt);

    vector<int> *nch = nullptr;
    T->SetBranchAddress("MJTNcHad", &nch);
    vector<int> *nbh = nullptr;
    T->SetBranchAddress("MJTNbHad", &nbh);

    float xlo = -0.5;
    float xhi = 15.5;
    float nbins = 16;


    TH1D*incl = new TH1D("incl", "incl", nbins, xlo, xhi);

    TH1D*cc = new TH1D("cc", "cc", nbins, xlo, xhi);
    TH1D*bb = new TH1D("bb", "bb", nbins, xlo, xhi);
    TH1D*c = new TH1D("c", "c", nbins, xlo, xhi);
    TH1D*b = new TH1D("b", "b", nbins, xlo, xhi);
    TH1D*light = new TH1D("light", "light", nbins, xlo, xhi);

    int mucount = 0;
    int cccount = 0;
    cout << T->GetEntries() << endl;    
    int trackcount = 0;

    for(int i = 0 ; i<T->GetEntries(); i++){
        if(i % 1000 == 0) cout << 1.0*i/T->GetEntries() << endl;
        T->GetEntry(i);
        if(isMuMu->size() == 0) continue;
        for(int j = 0; j < isMuMu->size(); j++){
            if(!isMuMu->at(j)) continue;
            if(mu1pt->at(j) < 4 || mu2pt->at(j) < 4) continue;
            if(jetpt->at(j) < 70) continue;
            trackcount = 0;
            trackcount += jtNtrk->at(j);

            mucount +=1;
            incl->Fill(trackcount);
            if(nch->at(j) == 2 && nbh->at(j) == 0){
                cc->Fill(trackcount);
                cccount += 1;
            }
            else if(nbh->at(j) == 2){
                bb->Fill(trackcount);
            }
            else if(nch->at(j) == 1 && nbh->at(j) == 0){
                c->Fill(trackcount);
            }
            else if(nbh->at(j) == 1){
                b->Fill(trackcount);
            }
            else if(nch->at(j) == 0 && nbh->at(j) == 0){
                light->Fill(trackcount);
            }
            
        }
    }

    cc->SetLineColor(2);
    bb->SetLineColor(3);
    c->SetLineColor(4);
    b->SetLineColor(6);
    light->SetLineColor(7);

    cc->SetLineWidth(2);
    bb->SetLineWidth(2);
    c->SetLineWidth(2);
    b->SetLineWidth(2);
    light->SetLineWidth(2);
    incl->SetLineWidth(2);

    TLegend*L = new TLegend(0.6,0.9,0.6,0.9);
    L->AddEntry(cc, "cc", "l");
    L->AddEntry(bb, "bb", "l");
    L->AddEntry(c, "c", "l");
    L->AddEntry(b, "b", "l");
    L->AddEntry(light, "light", "l");
    //L->AddEntry(incl, "incl", "l");

    TCanvas *c1 = new TCanvas("c1", "c1", 1000, 1000);
    c1->cd();
    incl->Draw("hist");

    c1->SaveAs("incl.pdf");

    TCanvas* c2 = new TCanvas("c2", "c2", 1000, 1000);
    c2->cd();
    
    b->Draw("hist same");
    bb->Draw("hist same");
    c->Draw("hist same");
    light->Draw("hist same");
    cc->Draw("hist same");
    L->Draw();
    c2->SaveAs("flavors.pdf");

    cout << cccount << " " << mucount << endl;

}