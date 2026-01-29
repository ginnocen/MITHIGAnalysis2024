

void ptdistributions(){



    gStyle->SetOptStat(0);

    TFile* f_higheg = TFile::Open("/data00/g2ccbar/data2018/highEGfull.root");
    TFile* f_loweg = TFile::Open("/data00/g2ccbar/data2018/lowEGfull.root");
    TFile* f_mc = TFile::Open("/data00/g2ccbar/mc2018/skim_011226_full.root");

    TTree* T_hi = (TTree*)f_higheg->Get("Tree");
    TTree* T_lo = (TTree*)f_loweg->Get("Tree");
    TTree* T_mc = (TTree*)f_mc->Get("Tree");


    TH1D* h_60_lo = new TH1D("h_60_lo", "HighEG Data Dimuon pT Spectrum; Jet pT ;Entries", 100, 0, 500);
    TH1D* h_80_lo = new TH1D("h_80_lo", "LowEG Data Dimuon pT Spectrum; Jet pT ;Entries", 100, 0, 500);

    TH1D* h_60_hi = new TH1D("h_60_hi", "HighEG Data Dimuon pT Spectrum; Jet pT ;Entries", 100, 0, 500);
    TH1D* h_80_hi = new TH1D("h_80_hi", "LowEG Data Dimuon pT Spectrum; Jet pT ;Entries", 100, 0, 500);

    TH1D* h_mc = new TH1D("h_mc", "MC Dimuon pT Spectrum; Jet pT ;Entries", 100, 0, 500);

    
    T_lo->Project("h_80_lo", "JetPT", "HLT_HIAK4PFJet80_v1");
    T_lo->Project("h_60_lo", "JetPT", "HLT_HIAK4PFJet60_v1 && !HLT_HIAK4PFJet80_v1");

    T_hi->Project("h_80_hi", "JetPT", "HLT_HIAK4PFJet80_v1");
    T_hi->Project("h_60_hi", "JetPT", "HLT_HIAK4PFJet60_v1 && !HLT_HIAK4PFJet80_v1");

    T_mc->Project("h_mc", "JetPT", "EventWeight * (IsMuMuTagged || !IsMuMuTagged)");

    TH1D* h_60 = (TH1D*)h_60_lo->Clone("h_60");
    TH1D* h_80 = (TH1D*)h_80_hi->Clone("h_80");

    TH1D* h_data = (TH1D*)h_80->Clone("h_data");
    h_data->Add(h_60, 6.338);

    TCanvas* c = new TCanvas("c_dimuonpt_comparison", "", 800, 600);
    c->SetLogy();
    h_mc->Scale(1.0/h_mc->Integral(h_mc->FindBin(180), h_mc->FindBin(500)));
    h_data->Scale(1.0/h_data->Integral(h_data->FindBin(180), h_data->FindBin(500)));
    h_mc->SetLineColor(kRed);
    h_mc->SetLineWidth(2);
    h_data->SetLineColor(kBlack);
    h_data->SetLineWidth(2); 
    h_mc->Draw("HIST");
    h_data->Draw("HIST SAME");  

    TLegend* leg = new TLegend(0.45,0.65,0.88,0.88);
    leg->AddEntry(h_data, "Data", "l");
    leg->AddEntry(h_mc, "MC with pThat weight", "l");
    leg->Draw();

    c->SaveAs("data_mc_comparison.pdf");

    // Create THStack of 60 and 80 components
    TCanvas* c2 = new TCanvas("c_stack", "", 800, 600);
    c2->SetLogy();
    
    THStack* hs = new THStack("hs", "Jet pT Components; Jet pT; Entries");
    h_60->Scale(6.338);
    h_60->SetFillColor(kBlue);
    h_60->SetLineColor(kBlue);
    h_80->SetFillColor(kRed);
    h_80->SetLineColor(kRed);
    
    hs->Add(h_60);
    hs->Add(h_80);
    hs->Draw("HIST");
    
    TLegend* leg2 = new TLegend(0.45,0.65,0.88,0.88);
    leg2->AddEntry(h_60, "HLT_HIAK4PFJet60 (prescale 6.338)", "f");
    leg2->AddEntry(h_80, "HLT_HIAK4PFJet80", "f");
    leg2->Draw();
    
    c2->SaveAs("data_stack_comparison.pdf");

}