


void spectra(){

    cout << "start" << endl;

    int n = 100000;

    

    gStyle->SetOptStat(0);
    TFile*f = new TFile("/data00/g2ccbar/mc2018/skim_5202025/mergedfile.root");
    TTree* T = (TTree*)f->Get("Tree");

    cout << "got tree" << endl;

    vector<float> *jetPT = nullptr;
    float pthat = 0;



    T->SetBranchAddress("JetPT", &jetPT);
    T->SetBranchAddress("PTHat", &pthat);

    cout << "set branch" << endl;
    
    TH1D* h1 = new TH1D("inc",";Jet pT (GeV); Counts",50,0,220);
    TH1D* h2 = new TH1D("h15",";Jet pT (GeV); Counts",50,0,220);
    TH1D* h3 = new TH1D("h25",";Jet pT (GeV); Counts",50,0,220);
    TH1D* h4 = new TH1D("h40",";Jet pT (GeV); Counts",50,0,220);
    TH1D* h5 = new TH1D("h80",";Jet pT (GeV); Counts",50,0,220);

    //int nmax = min(n,int(T->GetEntries()));

    for(int i = 0; i<n; i++){
        if(i % 100 == 0){cout << i*1.0/n << endl;}
        T->GetEntry(i);
        cout << pthat << endl;
        for(int j = 0; j<jetPT->size(); j++){

            h1->Fill(jetPT->at(j));

            if(pthat > 15){

                h2->Fill(jetPT->at(j));                

            }

            if(pthat > 25){

                h3->Fill(jetPT->at(j));

            }
            if(pthat > 40){

                h4->Fill(jetPT->at(j));

            }
            if(pthat > 80){

                h5->Fill(jetPT->at(j));

            }

        }
        
    }

    h1->SetLineColor(kBlack);
    h2->SetLineColor(kRed);
    h3->SetLineColor(kBlue);
    h4->SetLineColor(kGreen);
    h5->SetLineColor(kMagenta);
    h1->SetLineWidth(2);
    h2->SetLineWidth(2);
    h3->SetLineWidth(2);
    h4->SetLineWidth(2);
    h5->SetLineWidth(2);
    h1->GetYaxis()->SetRangeUser(1e0,1e7);
    h2->GetYaxis()->SetRangeUser(1e0,1e7);
    h3->GetYaxis()->SetRangeUser(1e0,1e7);
    h4->GetYaxis()->SetRangeUser(1e0,1e7);
    h5->GetYaxis()->SetRangeUser(1e0,1e7);
    
    TLegend* leg = new TLegend(0.6,0.6,0.9,0.9);
    leg->AddEntry(h1,"Inclusive","l");
    leg->AddEntry(h2,"pthat > 15","l");
    leg->AddEntry(h3,"pthat > 25","l");
    leg->AddEntry(h4,"pthat > 40","l");
    leg->AddEntry(h5,"pthat > 80","l");

    TCanvas* c = new TCanvas("c","c",800,800);
    c->cd();
    c->SetLogy();
    h1->Draw("hist same");
    h2->Draw("hist same");
    h3->Draw("hist same");
    h4->Draw("hist same");
    h5->Draw("hist same");
    leg->Draw("same");
    c->SaveAs("spectra.pdf");


}
