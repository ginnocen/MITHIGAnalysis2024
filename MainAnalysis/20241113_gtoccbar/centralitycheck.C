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

vector<TH1D*>filler(const char* filename){

    TFile*f = new TFile(filename);
    TTree*T = (TTree*)f->Get("Tree");


    vector<float>* mumumass = nullptr;
    vector<float>* JetPT = nullptr;
    vector<float>* muDiDxy1Dxy2 = nullptr;
    vector<float>* genJetPT = nullptr;
    vector<float>* mupt1 = nullptr;
    vector<float>* mupt2 = nullptr;
    vector<float>* dca1 = nullptr;
    vector<float>* dca2 = nullptr;
    vector<float>* nch = nullptr;
    vector<float>* nbh = nullptr;
    vector<bool>* ismumutagged = nullptr;
    vector<bool>* genismumutagged = nullptr;

    int centrality = 0;
    float ncollweight = 0;

    T->SetBranchAddress("mumuMass", &mumumass);
    T->SetBranchAddress("JetPT", &JetPT);
    T->SetBranchAddress("muPt1", &mupt1);
    T->SetBranchAddress("muPt2", &mupt2);
    T->SetBranchAddress("muDiDxy1", &dca1);
    T->SetBranchAddress("muDiDxy2", &dca2);
    T->SetBranchAddress("MJTNcHad", &nch);
    T->SetBranchAddress("MJTNbHad", &nbh);
    T->SetBranchAddress("hiBin", &centrality);
    T->SetBranchAddress("NCollWeight", &ncollweight);

    T->SetBranchAddress("genIsMuMuTagged", &genismumutagged);
    T->SetBranchAddress("IsMuMuTagged", &ismumutagged);
    T->SetBranchAddress("muDiDxy1Dxy2", &muDiDxy1Dxy2);
    T->SetBranchAddress("genJetPT", &genJetPT);


    //TH1D*h = new TH1D("","",10,0,1);
    TH1D* h_incl = new TH1D("a","a",20,0,100);
    TH1D* h_dimuon = new TH1D("b","b",20,0,100);
    

    int satis = 0;
    int counter = 0; 
    for (int i = 0; i<T->GetEntries(); i++){
        if(i%500000 ==0){cout << "prog " << 1.0*i/(T->GetEntries()) << endl;}  
        T->GetEntry(i);
        if(JetPT->size() > 0){
            for(int j = 0; j<JetPT->size(); j++){
                if(JetPT->at(j) > 100){
                    h_incl->Fill(centrality/2.0, ncollweight);
                    //h_incl->Fill(centrality/2.0);
                     break;
                }  
            }
            for(int j = 0; j<JetPT->size(); j++){
                if((JetPT->at(j) > 100)&&(ismumutagged->at(j) > 0)){
                    //h_dimuon->Fill(centrality/2.0);
                    h_dimuon->Fill(centrality/2.0,ncollweight);
                     break;
                }
            }

        }      
    }

    h_incl->Scale(1.0/h_incl->Integral());
    h_incl->SetLineWidth(3);
    h_incl->SetFillColorAlpha(1,0.1);
    h_incl->SetStats(0);
    h_incl->GetXaxis()->SetTitle("Centrality %");
    h_incl->GetYaxis()->SetTitle("Counts");
    h_incl->GetYaxis()->SetRangeUser(0, 0.25);
    //leg->AddEntry(h_incl, "Inclusive Jets", "lf");

    
    h_dimuon->Scale(1.0/h_dimuon->Integral());
    h_dimuon->SetLineWidth(3);
    h_dimuon->SetFillColorAlpha(2,0.1);
    h_dimuon->SetLineColor(2);
    h_dimuon->SetStats(0);
    h_dimuon->GetXaxis()->SetTitle("Centrality %");
    h_dimuon->GetYaxis()->SetTitle("Counts");
    h_dimuon->GetYaxis()->SetRangeUser(0, 0.25);
    //leg->AddEntry(h_dimuon, "Dimuon Jets", "lf");

    vector<TH1D*> r = {h_incl,h_dimuon};
    
    return r;


}

void centralitycheck(){

                                
    //vector<TH1D*> MC = filler("/data00/g2ccbar/mc2018/skim_01292025/mergedfile_01292025.root");
    vector<TH1D*> MC = filler("/data00/g2ccbar/mc2018/skim_01272025/mergedfile_01262025.root");
    vector<TH1D*> Data = filler("/data00/g2ccbar/data2018/hardProbes2018_skims/mergedfileData_01262025.root");

    MC[0]->SetLineStyle(6);
    MC[1]->SetLineStyle(6);

    TLegend*leg = new TLegend(0.45,0.55,0.87,0.87);
    leg->AddEntry(MC[0], "MC Inclusive Jets", "lf");
    leg->AddEntry(MC[1], "MC Dimuon Jets", "lf");
    leg->AddEntry(Data[0], "Data Inclusive Jets", "lf");
    leg->AddEntry(Data[1], "Data Dimuon Jets", "lf");
    leg->SetBorderSize(0); // Removes border
    leg->SetFillStyle(0);

    TCanvas*C = new TCanvas("","",100,100);
    MC[0]->Draw("colz same");
    MC[1]->Draw("colz same");
    Data[0]->Draw("colz same");
    Data[1]->Draw("colz same");
    leg->Draw();

    C->SaveAs("c.pdf");



}