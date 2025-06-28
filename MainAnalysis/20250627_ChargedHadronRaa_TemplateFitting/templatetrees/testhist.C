void testhist(const char* th1dname = "hMult"){

    vector<double> fracs = {1, 0.1, 0.02, 0};
    vector<string> files= {"template_OO.root", "template_SD.root", "template_DD.root","template_aO.root"};



    TH1D* hSum = nullptr;

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
        TH1D* hclone = (TH1D*)h->Clone(("hClone_" + to_string(i)).c_str());
        hclone->SetDirectory(0); 
        hclone->Scale(fracs[i]/h->Integral()); // Normalize the histogram

        if (!hSum) {
            hSum = (TH1D*)hclone->Clone("hMult");
            hSum->Reset(); // Reset the summed histogram
        }
        hSum->Add(hclone); // Add the cloned histogram to the sum
        cout << "Added histogram from file: " << files[i] << endl;

    }

    hSum->SetName("hMult");
    //hSum->Scale(1.0 / hSum->Integral()); // Normalize the summed histogram

    if (hSum) {
        cout << "Contents of summed histogram:" << endl;
        for (int bin = 1; bin <= hSum->GetNbinsX(); ++bin) {
            cout << "  Bin " << bin << ": " << hSum->GetBinContent(bin) << endl;
        }
        
        // Save to output file
        TFile* fout = new TFile("data.root", "RECREATE");
        hSum->Write();
        fout->Close();
    }

}