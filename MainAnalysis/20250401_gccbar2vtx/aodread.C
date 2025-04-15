#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TTreeReader.h"
#include "TError.h"
#include "TString.h"
#include "TObject.h"
#include <iostream>
#include <vector>
#include <string>
#include <TStyle.h>

using namespace std;


int flavorclass(int nch, int nbh){
    if(nbh == 1){return 1;} // 1 ----- Single B
    if(nbh == 2){return 2;} // 2 ----- Double B
    if(nbh == 0){
        if(nch == 1){return 3;} // 3 ---- Single C
        if(nch == 2){return 4;} // 4 ---- double C
        if(nch == 0){return 0;} // 0 ---- Light
        else{return -1;}
    }
    else{
        return -1;
    }
}


void aodread(){
    gStyle->SetOptStat(0);
    TFile*f = new TFile("HiForestMiniAOD.root");


    TDirectoryFile *muons = (TDirectoryFile*)f->Get("muonAnalyzer");
    TDirectoryFile *jets = (TDirectoryFile*)f->Get("akCs3PFJetAnalyzer");

    TTree* T = (TTree*)jets->Get("t");
    TTree* M = (TTree*)muons->Get("MuonTree");

    int nref;
    int nsvtx;
    int ntrk;

    int nch[100];
    int nbh[100];

    //Tracks
    int trkJetId[100];
    int trkSvtxId[100];
    float trkPt[100];
    float trkEta[100];
    float trkPhi[100];
    float trkIp3d[100];
    float trkIp3dsig[100];
    float trkIp2d[100];
    float trkIp2dsig[100];
    float trkDistToAxisSig[100];
    float trkDistToAxis[100];
    float trkIpProb3d[100];
    float trkIpProb2d[100];
    float trkDz[100];
    int trkPdgId[100];
    int trkMatchSta[100];

    //Svtx 

    int svtxJetId[100];
    int svtxNtrk[100];
    float svtxdl[100];
    float svtxdls[100];
    float svtxdl2d[100];
    float svtxdls2d[100];
    float svtxm[100];
    float svtxmcorr[100];
    float svtxpt[100];
    float svtxnormchi2[100];
    float svtxchi2[100];


    T->SetBranchAddress("nref", &nref);
    T->SetBranchAddress("nsvtx", &nsvtx);
    T->SetBranchAddress("ntrk", &ntrk);

    T->SetBranchAddress("mjtNcHad",&nch);
    T->SetBranchAddress("mjtNbHad",&nbh);
    T->SetBranchAddress("trkJetId",&trkJetId);
    T->SetBranchAddress("trkSvtxId",&trkSvtxId);
    T->SetBranchAddress("trkPt",&trkPt);
    T->SetBranchAddress("trkEta",&trkEta);
    T->SetBranchAddress("trkPhi",&trkPhi);
    T->SetBranchAddress("trkIp3d",&trkIp3d);
    T->SetBranchAddress("trkIp3dsig",&trkIp3dsig);
    T->SetBranchAddress("trkIp2d",&trkIp2d);
    T->SetBranchAddress("trkIp2dsig", &trkIp2dsig);
    T->SetBranchAddress("trkDistToAxisSig", &trkDistToAxisSig);
    T->SetBranchAddress("trkDistToAxis", &trkDistToAxis);
    T->SetBranchAddress("trkIpProb3d", &trkIpProb3d);
    T->SetBranchAddress("trkIpProb2d",&trkIpProb2d);
    T->SetBranchAddress("trkDz",&trkDz);
    T->SetBranchAddress("trkPdgId",&trkPdgId);
    T->SetBranchAddress("trkMatchSta",&trkMatchSta);

    T->SetBranchAddress("svtxJetId",&svtxJetId);
    T->SetBranchAddress("svtxNtrk",&svtxNtrk);
    T->SetBranchAddress("svtxdl",&svtxdl);
    T->SetBranchAddress("svtxdls",&svtxdls);
    T->SetBranchAddress("svtxdl2d",&svtxdl2d);
    T->SetBranchAddress("svtxdls2d",&svtxdls2d);
    T->SetBranchAddress("svtxm",&svtxm);
    T->SetBranchAddress("svtxmcorr",&svtxmcorr);
    T->SetBranchAddress("svtxpt",&svtxpt);
    T->SetBranchAddress("svtxnormchi2",&svtxnormchi2);
    T->SetBranchAddress("svtxchi2",&svtxchi2);
    
    //T->Show(0);
    //TH2D*h = new TH2D("",";# B Hadrons; # C Hadrons",5,-0.5,4.5,5,-0.5,4.5);
    TH1D*h = new TH1D("",";Secondary Vertex # of Tracks;au",10,-0.5,9.5);
    TH1D*h0 = new TH1D("",";Secondary Vertex # of Tracks;au",10,-0.5,9.5);
    TH1D*h1 = new TH1D("",";Secondary Vertex # of Tracks;au",10,-0.5,9.5);
    TH1D*h2 = new TH1D("",";Secondary Vertex # of Tracks;au",10,-0.5,9.5);
    TH1D*h3 = new TH1D("",";Secondary Vertex # of Tracks;au",10,-0.5,9.5);
    TH1D*h4 = new TH1D("",";Secondary Vertex # of Tracks;au",10,-0.5,9.5);
    

    for(int i = 0; i<T->GetEntries(); i++){
        T->GetEntry(i);
        
        for(int j = 0; j< nsvtx; j++){
            h->Fill(svtxNtrk[j]);
            if(flavorclass(nch[svtxJetId[j]],nbh[svtxJetId[j]]) == 0){
                h0->Fill(svtxNtrk[j]);
            }
            if(flavorclass(nch[svtxJetId[j]],nbh[svtxJetId[j]]) == 1){
                h1->Fill(svtxNtrk[j]);
            }
            if(flavorclass(nch[svtxJetId[j]],nbh[svtxJetId[j]]) == 2){
                h2->Fill(svtxNtrk[j]);
            }
            if(flavorclass(nch[svtxJetId[j]],nbh[svtxJetId[j]]) == 3){
                h3->Fill(svtxNtrk[j]);
            }
            if(flavorclass(nch[svtxJetId[j]],nbh[svtxJetId[j]]) == 4){
                h4->Fill(svtxNtrk[j]);
            }
           cout << svtxJetId[j] << endl;
            

        }
             
        
    }

    h0->SetLineColor(7);
    h1->SetLineColor(5);
    h2->SetLineColor(6);
    h3->SetLineColor(3);
    h4->SetLineColor(2);

    h0->SetLineWidth(3);
    h1->SetLineWidth(3);
    h2->SetLineWidth(3);
    h3->SetLineWidth(3);
    h4->SetLineWidth(3);
    h->SetLineWidth(3);

    TLegend* g= new TLegend(0.7,0.7,0.9,0.9);
    g->AddEntry(h,"inclusive","l");
    g->AddEntry(h0,"light","l");
    g->AddEntry(h1,"B = 1","l");
    g->AddEntry(h2,"B = 2","l");
    g->AddEntry(h3,"B = 0, C = 1","l");
    g->AddEntry(h4,"B = 0, C = 2","l");

    TCanvas*c= new TCanvas();
    h->Draw("colz same");
    h0->Draw("colz same");
    h1->Draw("colz same");
    h2->Draw("colz same");
    h3->Draw("colz same");
    h4->Draw("colz same");
    g->Draw();
    c->SaveAs("c.pdf");

    cout << "hi" <<endl;

    


 
    



    
    


}


