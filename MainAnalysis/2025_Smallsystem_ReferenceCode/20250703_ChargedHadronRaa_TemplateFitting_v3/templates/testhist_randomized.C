TH1D* thr(const char* th1dname = "hMult") {

    vector<double> fracs = {0.8, 0.15, 0.05, 0};
    vector<string> files= {"template_OO.root", "template_SD.root", "template_DD.root","template_aO.root"};



    TH1D* hSum = nullptr;
    int nentries = 10000000;

    for (size_t i = 0; i < files.size(); ++i) {
        TFile* f = TFile::Open(files[i].c_str());
        if (!f || f->IsZombie()) {
            cout << "Could not open file: " << files[i] << endl;
            continue;
        }
        TH1D* h = (TH1D*)f->Get(th1dname);
        if (!h) {
            cout << "Could not find histogram" << th1dname << " in " << files[i] << endl;
            continue;
        }
       
        TH1D* h_fill = (TH1D*)h->Clone();
        h_fill->Reset();
        cout << h_fill->GetName() << " cloned from " << files[i] << endl;
        int entries = fracs[i] * nentries;
        for(int j = 0; j < entries; j++){
            double value = h->GetRandom();
            h_fill->Fill(value);
        }
        if (!hSum) {
            hSum = (TH1D*)h_fill->Clone(th1dname);
            hSum->Reset();
        }
        hSum->Add(h_fill); // Add the cloned histogram to the sum
        cout << "Added histogram from file: " << files[i] << endl;

    }

    hSum->SetName(th1dname);
    hSum->Scale(1.0 / hSum->Integral()); // Normalize the summed histogram

    if (hSum) {
        cout << "Contents of summed histogram:" << endl;
        for (int bin = 1; bin <= hSum->GetNbinsX(); ++bin) {
            cout << "  Bin " << bin << ": " << hSum->GetBinContent(bin) << endl;
        }
      
    }

    return hSum;

}


void testhist_randomized(){

    TH1D* mult = thr("hMult");
    TH1D* hfe = thr("hHFEsum");

    // Save to output file
        TFile* fout = new TFile("data.root", "RECREATE");
        mult->Write();
        hfe->Write();
        fout->Close();


}