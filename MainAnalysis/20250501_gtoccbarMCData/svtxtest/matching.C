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
#include <cmath>
using namespace std;

float min_angle(float phi1, float phi2) {
    constexpr float PI = M_PI;

    // Normalize both angles to (-π, π]
    auto normalize = [](float phi) {
        phi = std::fmod(phi + PI, 2 * PI);
        if (phi < 0) phi += 2 * PI;
        return phi - PI;
    };

    float a = normalize(phi1);
    float b = normalize(phi2);

    float diff = std::fmod(std::abs(a - b), 2 * PI);
    if (diff > PI)
        diff = 2 * PI - diff;

    return diff;
}

void matching(){
    gStyle->SetOptStat(0);

    cout<< "1" << endl;

    TFile*f = new TFile("/data00/g2ccbar/mc2018/skim_05012025/mergedfile.root");
    TTree *T = (TTree*)f->Get("Tree");

    vector<vector<float>> * trkPhi = nullptr;
    vector<vector<float>> * trkEta = nullptr;
    vector<vector<float>> * trkPT = nullptr;

    T->SetBranchAddress("trkPhi", &trkPhi);
    T->SetBranchAddress("trkEta", &trkEta);
    T->SetBranchAddress("trkPT", &trkPT);

    vector<float> *muPhi1 = nullptr;
    vector<float> *muPhi2 = nullptr;
    T->SetBranchAddress("muPhi1", &muPhi1);
    T->SetBranchAddress("muPhi2", &muPhi2);
    vector<float> *muEta1 = nullptr;
    vector<float> *muEta2 = nullptr;   
    T->SetBranchAddress("muEta1", &muEta1);
    T->SetBranchAddress("muEta2", &muEta2);
    
    vector<bool> *isMuMu = nullptr;
    T->SetBranchAddress("IsMuMuTagged", &isMuMu);
    vector<float> *mu1pt = nullptr;
    T->SetBranchAddress("muPt1", &mu1pt);
    vector<float> *mu2pt = nullptr;
    T->SetBranchAddress("muPt2", &mu2pt);
    vector<float> *jetpt = nullptr;
    T->SetBranchAddress("JetPT", &jetpt);

    vector<vector<int>> *trkSvtxId = nullptr;
    T->SetBranchAddress("trkSvtxId", &trkSvtxId);

    vector<int> *nch = nullptr;
    T->SetBranchAddress("MJTNcHad", &nch);
    vector<int> *nbh = nullptr;
    T->SetBranchAddress("MJTNbHad", &nbh);

    cout<< "2" << endl;


    TH1D*nndR = new TH1D("nndR",";log10 dR from nearest track to muon;counts",50,-7,1);
    TH1D*nnndR = new TH1D("nnndR",";log10 dR from nearest track to muon;counts",50,-7,1);

    float dr1_min = 0;
    float dr2_min = 0;

    float dr1_next = 0;
    float dr2_next = 0;


    int goodtrk1 = 0;
    int goodtrk2 = 0;
    int nmatched1 = 0;
    int nmatched2 = 0;
    int nmatchedsvtx1 = 0;
    int nmatchedsvtx2 = 0;
    int svtxid1 = 0;
    int svtxid2 = 0;

    int n_samevertex = 0;
    int n_diffvertex = 0;
    int n_vertexonly1 = 0;
    int n_vertexonly2 = 0;
    int n_none = 0;
    
    int nsame = 0;
    int ntot = 0;

    int count1 = 0;
    int count2 = 0;

    for(int i = 0; i<T->GetEntries(); i++){
        T->GetEntry(i);
        //cout << i << endl;
        if(i % 10000 == 0) cout << 1.0*i/T->GetEntries() << endl;
        for(int j = 0; j< isMuMu->size(); j++){
            if(!isMuMu->at(j)) continue;
            if(mu1pt->at(j) < 4 || mu2pt->at(j) < 4) continue;
            if(jetpt->at(j) < 70) continue;
            //if(nch->at(j) != 2 || nbh->at(j) != 0) continue;

            ntot += 1;
            dr1_min = 9999;
            dr2_min = 9999;

            goodtrk1 = 0;
            goodtrk2 = 0;

            count1 = 0;
            count2 = 0;

            for(int k = 0; k<trkPhi->at(j).size(); k++){

                float dR1 = sqrt(pow(min_angle(trkPhi->at(j)[k],muPhi1->at(j)),2) + pow(trkEta->at(j)[k] - muEta1->at(j),2));
                float dR2 = sqrt(pow(min_angle(trkPhi->at(j)[k],muPhi2->at(j)),2) + pow(trkEta->at(j)[k] - muEta2->at(j),2));
                
                if(dR1 < 0.0003){
                    count1 += 1;
                    goodtrk1 = k;
                }
                if(dR2 < 0.0003){
                    count2 += 1;
                    goodtrk2 = k;
                }

                

            }

            svtxid1 = -1;
            svtxid2 = -1;

            if(count1 == 1){
                nmatched1 += 1;
                if(trkSvtxId->at(j)[goodtrk1] > -1){
                    nmatchedsvtx1 += 1;
                    svtxid1 = trkSvtxId->at(j)[goodtrk1];
                }
            }
            if(count2 == 1){
                nmatched2 += 1;
                if(trkSvtxId->at(j)[goodtrk2] > -1){
                    nmatchedsvtx2 += 1;
                    svtxid2 = trkSvtxId->at(j)[goodtrk1];
                }
            }

            if(count1 == 1 && count2 == 1){nsame += 1;}

            if((svtxid1 == svtxid2) && (svtxid1 > -1)){
                n_samevertex += 1;
                
            }
            else if((svtxid1 != svtxid2) && (svtxid1 > -1) && (svtxid2 > -1)){
                n_diffvertex += 1;
            }
            else if(svtxid1 == -1 && svtxid2 == -1){
                n_none += 1;
            }
            else if((svtxid1 != -1) && (svtxid2 == -1)){
                n_vertexonly1 += 1;
            }
            else if((svtxid2 != -1) && (svtxid1 == -1)){
                n_vertexonly2 += 1;
            }
            

            /*for(int k = 0; k< trkPhi->at(j).size(); k++){
                float dR1 = sqrt(pow(min_angle(trkPhi->at(j)[k],muPhi1->at(j)),2) + pow(trkEta->at(j)[k] - muEta1->at(j),2));
                float dR2 = sqrt(pow(min_angle(trkPhi->at(j)[k],muPhi2->at(j)),2) + pow(trkEta->at(j)[k] - muEta2->at(j),2));
                
                if(dR1 < dr1_min){
                    dr1_min = dR1;
                    goodtrk1 = k;
                }
                if(dR2 < dr2_min){
                    dr2_min = dR2;
                    goodtrk2 = k;

                }

        
                
            }
            dr1_next = 9999;
            dr2_next = 9999;
            */
            /*for(int k = 0; k< trkPhi->at(j).size(); k++){
                float dR1 = sqrt(pow(min_angle(trkPhi->at(j)[k],muPhi1->at(j)),2) + pow(trkEta->at(j)[k] - muEta1->at(j),2));
                float dR2 = sqrt(pow(min_angle(trkPhi->at(j)[k],muPhi2->at(j)),2) + pow(trkEta->at(j)[k] - muEta2->at(j),2));
                
                if((dR1 < dr1_next) && (dR1 != dr1_min)){
                    dr1_next = dR1;
                }
                if((dR2 < dr2_next) && (dR2 != dr2_min)){
                    dr2_next = dR2;
                }

                
            }*/

            //nndR->Fill(log10(dr1_min));
            //nndR->Fill(log10(dr2_min));
            //nnndR->Fill(log10(dr1_next));
            //nnndR->Fill(log10(dr2_next));

        }

    }

    /*
    nnndR->SetLineColor(2);
    TCanvas *c1 = new TCanvas("c1", "c1", 1000, 1000);
    nndR->Draw("colz");
    //nnndR->Draw("colz same");
    c1->SaveAs("nndR.png");
    */
    cout << "dimuons " << ntot << endl;
    cout << "matched track 1: " << nmatched1 << endl;
    cout << "matched track 2: " << nmatched2 << endl;
    cout << "both matched tracks" << nsame << endl;
    cout << "matched svtx 1: " << nmatchedsvtx1 << endl;
    cout << "matched svtx 2: " << nmatchedsvtx2 << endl;
    cout << "same vertex: " << n_samevertex << endl;
    cout << "different vertex: " << n_diffvertex << endl;
    cout << "vertex only 1: " << n_vertexonly1 << endl;
    cout << "vertex only 2: " << n_vertexonly2 << endl;
    cout << "none: " << n_none << endl;
}