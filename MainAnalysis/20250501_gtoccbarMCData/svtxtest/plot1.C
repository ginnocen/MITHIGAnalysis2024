
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

void plot1(){

    int nccbar = 0;
    int nmumu = 0;
    int rawmumu = 0;
    
    TFile *f = new TFile("/data00/g2ccbar/mc2018/skim_05012025/mergedfile.root");
    TTree *T = (TTree*)f->Get("Tree");

    vector<float> *mumupt = nullptr;    
    T->SetBranchAddress("mumuPt", &mumupt);

    vector<int> *nch = nullptr;
    T->SetBranchAddress("MJTNcHad", &nch);

    vector<int> *nbh = nullptr;
    T->SetBranchAddress("MJTNbHad", &nbh);

    vector<float> *mu1trk = nullptr;
    T->SetBranchAddress("mu1trk", &mu1trk);

    vector<float> *mu2trk = nullptr;
    T->SetBranchAddress("mu2trk", &mu2trk);

    vector<float> *mu1svtx = nullptr;
    T->SetBranchAddress("mu1svtx", &mu1svtx);

    vector<float> *mu2svtx = nullptr;      
    T->SetBranchAddress("mu2svtx", &mu2svtx);

    vector<float> *jtNtrk = nullptr;
    T->SetBranchAddress("jtNtrk", &jtNtrk);

    vector<float> *jtNsvtx = nullptr;
    T->SetBranchAddress("jtNsvtx", &jtNsvtx);

    vector<vector<float>> *svtxNtrk = nullptr;
    T->SetBranchAddress("svtxNtrk", &svtxNtrk);

    vector<float> *dxy1 = nullptr;
    T->SetBranchAddress("muDiDxy1", &dxy1);

    vector<float> *jetpt = nullptr;
    T->SetBranchAddress("JetPT", &jetpt);

    vector<float> *mu1pt = nullptr;
    T->SetBranchAddress("muPt1", &mu1pt);

    vector<float> *mu2pt = nullptr;
    T->SetBranchAddress("muPt2", &mu2pt);

    vector<vector<float>> *svtxm = nullptr;
    T->SetBranchAddress("svtxmcorr", &svtxm);

    bool ismumu = false;
    int ntracks = 0;
    int nsvtx = 0;
    int nsvtxtracks = 0;
    float svmass = 0;

    vector<float> cc_d1 = {};
    vector<float> bb_d1 = {};
    vector<float> c_d1 = {};
    vector<float> b_d1 = {};
    vector<float> light_d1 = {};

    vector<float> cc_d2 = {};
    vector<float> bb_d2 = {};
    vector<float> c_d2 = {};
    vector<float> b_d2 = {};
    vector<float> light_d2 = {};


    cout << T->GetEntries() << endl;
    for(int i = 0; i< T->GetEntries(); i++){
        if(i % 1000 == 0) cout << 1.0*i/T->GetEntries() << endl;
        T->GetEntry(i);
        if(!mumupt) continue;
        for(int k = 0; k < mumupt->size(); k++){
            if(mumupt->at(k) == -999){continue;}
            rawmumu+=1;
            if(jetpt->at(k) < 70 || mu1pt->at(k) < 4 || mu2pt->at(k) < 4){continue;}
            nmumu+=1;           

            nsvtxtracks = 0;
            svmass = 0;
            ntracks = jtNtrk->at(k);
            nsvtx = jtNsvtx->at(k);
            for(int j = 0; j< nsvtx; j++){
                nsvtxtracks += svtxNtrk->at(k)[j];
                svmass += svtxm->at(k)[j];
            }

            
            //cout << "c1" << endl;
            //cout << nch->at(k) << " " << nbh->at(k) << endl;
            if(nbh->at(k) == 1){
                b_d2.push_back(1.0*ntracks);
                b_d1.push_back(1.0*svmass);
            }
            else if (nbh->at(k)== 2){
                bb_d2.push_back(1.0*ntracks);
                bb_d1.push_back(1.0*svmass);
            }
            else if((nch->at(k) == 1)&&(nbh->at(k) == 0)){
                c_d2.push_back(1.0*ntracks);
                c_d1.push_back(1.0*svmass);
            }
            else if((nch->at(k) == 2)&&(nbh->at(k) == 0)){
                nccbar+=1;
                cc_d2.push_back(1.0*ntracks);
                cc_d1.push_back(1.0*svmass);
            }
            else if(nch->at(k) == 0 && nbh->at(k) == 0){
                light_d2.push_back(1.0*ntracks);
                light_d1.push_back(1.0*svmass);
            }

            cout << nch->at(k) << " " << nbh->at(k) << endl;
        }
    }


    TGraph* gcc = new TGraph(cc_d1.size(), cc_d1.data(), cc_d2.data());
    TGraph* gbb = new TGraph(bb_d1.size(), bb_d1.data(), bb_d2.data());
    TGraph* gc = new TGraph(c_d1.size(), c_d1.data(), c_d2.data());
    TGraph* gb = new TGraph(b_d1.size(), b_d1.data(), b_d2.data());
    TGraph* glight = new TGraph(light_d1.size(), light_d1.data(), light_d2.data());

    gcc->SetTitle(";number of svtx;number of tracks "); 
    gbb->SetTitle(";number of svtx;number of tracks "); 
    gc->SetTitle(";number of svtx;number of tracks "); 
    gb->SetTitle(";number of svtx;number of tracks "); 
    glight->SetTitle(";number of svtx;number of tracks "); 

    gcc->SetMarkerColorAlpha(2,0.5);
    gbb->SetMarkerColorAlpha(3,0.5);
    gc->SetMarkerColorAlpha(4,0.5);
    gb->SetMarkerColorAlpha(6,0.5);
    glight->SetMarkerColorAlpha(7,0.5);

    gcc->SetMarkerStyle(20);
    gbb->SetMarkerStyle(20);
    gc->SetMarkerStyle(20);
    gb->SetMarkerStyle(20);
    glight->SetMarkerStyle(20);


    TLegend*L = new TLegend(0.1,0.7,0.3,0.9);
    L->AddEntry(gcc, "cc", "p");
    L->AddEntry(gbb, "bb", "p");
    L->AddEntry(gc, "c", "p");
    L->AddEntry(gb, "b", "p");
    L->AddEntry(glight, "light", "p");

    TCanvas *c1 = new TCanvas("c1", "c1", 1000, 1000);
    gb->Draw("AP");   
    gbb->Draw("P");
    gc->Draw("P");       
    glight->Draw("P");
    gcc->Draw("P");
    L->Draw("same");
    c1->SaveAs("dxy1.png");

    cout << "out of N entries  " << T->GetEntries() << endl;
    cout << "nccbar " << nccbar << endl;
    cout << "nmumu " << nmumu << endl;
    cout << "rawmumu " << rawmumu << endl;

}
