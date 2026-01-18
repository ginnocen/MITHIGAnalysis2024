void flavorefficiency(){
    gStyle->SetOptStat(0);

    TFile* f = TFile::Open("../testdistros.root");
    if(!f || f->IsZombie()) {
        cout << "Error: Could not open file testdistros.root" << endl;
        return;
    }
    cout << "File opened successfully" << endl;
    cout << "Objects in file:" << endl;
    f->ls();
    
    TH2D* h_eff_other = (TH2D*)f->Get("hEfficiency_other");
    TH2D* h_eff_b = (TH2D*)f->Get("hEfficiency_b");
    TH2D* h_eff_bb = (TH2D*)f->Get("hEfficiency_bb");
    TH2D* h_eff_c = (TH2D*)f->Get("hEfficiency_c");
    TH2D* h_eff_cc = (TH2D*)f->Get("hEfficiency_cc");
    TH2D* h_eff_udsg = (TH2D*)f->Get("hEfficiency_uds");
    TCanvas* c1 = new TCanvas("c1","c1",800,600);
    
    if(h_eff_other) {
        h_eff_other->GetXaxis()->SetTitle("Jet pT (GeV)");
        h_eff_other->GetYaxis()->SetTitle("Jet |#eta|");
    }
    if(h_eff_b) {
        h_eff_b->GetXaxis()->SetTitle("Jet pT (GeV)");
        h_eff_b->GetYaxis()->SetTitle("Jet |#eta|");
    }
    if(h_eff_bb) {
        h_eff_bb->GetXaxis()->SetTitle("Jet pT (GeV)");
        h_eff_bb->GetYaxis()->SetTitle("Jet |#eta|");
    }
    if(h_eff_c) {
        h_eff_c->GetXaxis()->SetTitle("Jet pT (GeV)");
        h_eff_c->GetYaxis()->SetTitle("Jet |#eta|");
    }
    if(h_eff_cc) {
        h_eff_cc->GetXaxis()->SetTitle("Jet pT (GeV)");
        h_eff_cc->GetYaxis()->SetTitle("Jet |#eta|");
    }
    if(h_eff_udsg) {
        h_eff_udsg->GetXaxis()->SetTitle("Jet pT (GeV)");
        h_eff_udsg->GetYaxis()->SetTitle("Jet |#eta|");
    }

    //if(h_eff_udsg) h_eff_udsg->Draw("colz");
    //c1->SaveAs("h_eff_udsg.pdf");

    if(h_eff_b) h_eff_b->Draw("colz");
    c1->SaveAs("h_eff_b.pdf");
    
}