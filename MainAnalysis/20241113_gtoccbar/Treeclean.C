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

void Treeclean(){

    //TFile*f = new TFile("/data00/g2ccbar/mc2018/skim_01272025/mergedfile_01262025.root");
    TFile*f = new TFile("/data00/g2ccbar/mc2018/skimTest/mergedfile_02102025.root");
    TTree*T = (TTree*)f->Get("Tree");

    TFile*f2 = new TFile("/data00/g2ccbar/data2018/minBias_skims_02052025/mergedfileData_02042025_mbAll.root");
    TTree*T2 = (TTree*)f2->Get("Tree");

    int centrality = 0;
    float ncollweight = 0;

    float EventWeight = 0;
    float PTHat = 0;

    vector<float>* mumumass = nullptr;
    vector<float>* JetPT = nullptr;
    vector<float>* JetPTData = nullptr;
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
    //vector<float>* genJetPT = nullptr;
    

    

    T->SetBranchAddress("mumuMass", &mumumass);
    T->SetBranchAddress("JetPT", &JetPT);
    T2->SetBranchAddress("JetPT", &JetPTData);

    T->SetBranchAddress("muPt1", &mupt1);
    T->SetBranchAddress("muPt2", &mupt2);
    T->SetBranchAddress("muDiDxy1", &dca1);
    T->SetBranchAddress("muDiDxy2", &dca2);
    T->SetBranchAddress("MJTNcHad", &nch);
    T->SetBranchAddress("MJTNbHad", &nbh);
    T->SetBranchAddress("hiBin", &centrality);
    T->SetBranchAddress("NCollWeight", &ncollweight);
    T->SetBranchAddress("EventWeight", &EventWeight);
    T->SetBranchAddress("PTHat", &PTHat);
    //T->SetBranchAddress("isleading", &_isleading);

    T->SetBranchAddress("genIsMuMuTagged", &genismumutagged);
    T->SetBranchAddress("genJetPT",&genJetPT);
    T->SetBranchAddress("IsMuMuTagged", &ismumutagged);
    T->SetBranchAddress("muDiDxy1Dxy2", &muDiDxy1Dxy2);
    T->SetBranchAddress("genJetPT", &genJetPT);


    TH1D* h_data = new TH1D("","",50,0,250);
    TH1D* hgen = new TH1D("","",50,0,250);
    TH1D* hgen_unweight = new TH1D("","",50,0,250);
    TH1D* h = new TH1D("","",50,0,250);
    TH1D* h_unweight = new TH1D("","",50,0,250);


    
    TH1D* weights = new TH1D("","; log10(Event Weight);counts",500,-12,0);
    TH2D* weights2 = new TH2D("",";log10(Event Weight); log10(NCollWeight)",100,-9,2,100,0,4);
    TH2D* pTratio = new TH2D("","; pTHat; Leading Jet pT / pTHat ",100,0,500,200,0,10);
    TH2D* pTcorr = new TH2D("",";Gen JetPT;log10(Event Weight)",100,0,500,200,-10,5);
    pTratio->SetStats(0);
    pTcorr->SetStats(0);


    cout << "starting" << endl;

    //int satis = 0;
    int counter = 0; 
    for (int i = 0; i<5000000; i++){
        if(i%5000 ==0){cout << "prog " << 1.0*i/(T->GetEntries()) << endl;}  
        T->GetEntry(i);
        T2->GetEntry(i);
        if(JetPT->size() > 0){

            //if(JetPT->at(0)/PTHat > 2.5){
            ///        continue;
            //    }
                
            for(int j = 0; j< JetPT->size(); j++){

                

                if(JetPT->at(j) > 70){

                h_unweight->Fill(JetPT->at(j),ncollweight);
                h->Fill(JetPT->at(j),ncollweight*EventWeight);

                hgen_unweight->Fill(genJetPT->at(j),ncollweight);
                hgen->Fill(genJetPT->at(j),ncollweight*EventWeight);
                //cout << ncollweight << " " << ncollweight * EventWeight << " " << EventWeight << endl;
                if(ismumutagged->at(j)){
                //weights->Fill(log10(ncollweight*EventWeight));
                weights->Fill(log10(EventWeight));
                weights2->Fill(log10(EventWeight),log10(ncollweight));
                
                }
                
                if(j == 0){
                //pTratio->Fill(JetPT->at(j),(JetPT->at(j)/PTHat));
                pTcorr->Fill(genJetPT->at(j),log10(EventWeight),ncollweight);
                }
                }

                if(genJetPT->at(j) > 80){
                    hgen_unweight->Fill(genJetPT->at(j),ncollweight);
                    hgen->Fill(genJetPT->at(j),ncollweight*EventWeight);
                }

            }
        
            
        }      

        if(JetPTData->size() > 0){

            for(int j = 0; j< JetPTData->size(); j++){

                if(JetPTData->at(j) > 80){
                h_data->Fill(JetPTData->at(j),1);
                }
            }

        }

    }


    TLegend*leg = new TLegend(0.45,0.75,0.80,0.95);
    leg->AddEntry(h_data, "Data", "lf");


    h->Scale(1.0/h->Integral());
    h->SetLineWidth(3);
    h->SetLineColor(1);
    h->SetFillColorAlpha(1,0.1);
    h->SetStats(0);
    h->GetXaxis()->SetTitle("Jet pT (GeV)");
    h->GetYaxis()->SetTitle("Counts");
    //h->GetYaxis()->SetRangeUser(0, 0.2);
    //h->GetXaxis()->SetRangeUser(0, 0.2);
    leg->AddEntry(h, "pThat weighted", "lf");
    
    h_unweight->Scale(1.0/h_unweight->Integral());
    h_unweight->SetLineWidth(3);
    h_unweight->SetLineColor(2);
    h_unweight->SetFillColorAlpha(2,0.1);
    h_unweight->SetStats(0);
    h_unweight->GetXaxis()->SetTitle("Jet pT (GeV)");
    h_unweight->GetYaxis()->SetTitle("Counts");
    //h_unweight->GetYaxis()->SetRangeUser(0, 0.2);
    leg->AddEntry(h_unweight, "Unweighted", "lf");

    h_data->Scale(1.0/h_data->Integral());
    h_data->SetLineWidth(3);
    h_data->SetLineColor(3);
    h_data->SetFillColorAlpha(3,0.1);
    h_data->SetStats(0);
    h_data->GetXaxis()->SetTitle("Jet pT (GeV)");
    h_data->GetYaxis()->SetTitle("Counts");
    //h_data->GetYaxis()->SetRangeUser(0, 0.2);
    


    hgen->Scale(1.0/hgen->Integral());
    hgen->SetLineWidth(3);
    hgen->SetLineColor(6);
    //hgen->SetLineStyle(4);
    hgen->SetFillColorAlpha(1,0.1);
    hgen->SetStats(0);
    hgen->GetXaxis()->SetTitle("Jet pT (GeV)");
    hgen->GetYaxis()->SetTitle("Counts");
    //hgen->GetYaxis()->SetRangeUser(0, 0.2);
    //hgen->GetXaxis()->SetRangeUser(0, 0.2);
    //leg->AddEntry(hgen, "Gen Jets pThat weighted", "lf");

    hgen_unweight->Scale(1.0/hgen_unweight->Integral());
    hgen_unweight->SetLineWidth(3);
    hgen_unweight->SetLineColor(7);
    //hgen_unweight->SetLineStyle(4);
    hgen_unweight->SetFillColorAlpha(2,0.1);
    hgen_unweight->SetStats(0);
    hgen_unweight->GetXaxis()->SetTitle("Jet pT (GeV)");
    hgen_unweight->GetYaxis()->SetTitle("Counts");
    //h_unweight->GetYaxis()->SetRangeUser(0, 0.2);
    //leg->AddEntry(hgen_unweight, "Gen Jets Unweighted", "lf");


    weights->SetLineWidth(3);
    weights->SetLineColor(1);
    //weights->SetStats(0);
    weights->GetXaxis()->SetTitle("log10(NCollweight x EventWeight (pT Hat))");
    weights->GetYaxis()->SetTitle("Counts");




    TCanvas* c= new TCanvas("","",100,100);
    //c->SetLogy();
    //c->SetLogx();
    //h->Draw("colz same");
    //h_data->Draw("colz same");
    //h_unweight->Draw("colz same");

    //hgen->Draw("colz same");
    //hgen_unweight->Draw("colz same");
    //leg->Draw();
    weights2->Draw("Colz");
    //pTratio->Draw("Colz");
    //pTcorr->Draw("Colz");

    c->SaveAs("cey.pdf");

}



