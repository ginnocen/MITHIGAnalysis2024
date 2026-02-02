

void trigger_turnons(){

    gStyle->SetOptStat(0);

    TFile* f_higheg = TFile::Open("/data00/g2ccbar/data2018/highEGfull.root");
    TFile* f_loweg = TFile::Open("/data00/g2ccbar/data2018/lowEGfull.root");
    TFile* f_mc = TFile::Open("/data00/g2ccbar/mc2018/skim_011226_full.root");

    TTree* T_hi = (TTree*)f_higheg->Get("Tree");
    TTree* T_lo = (TTree*)f_loweg->Get("Tree");
    TTree* T_mc = (TTree*)f_mc->Get("Tree");

    TH1D* h_40 = new TH1D("h_40", "HLT_HIAK4PFJet40 Turn-on; Jet pT ;Entries", 100, 0, 500);
    TH1D* h_60 = new TH1D("h_60", "HLT_HIAK4PFJet60 Turn-on; Jet pT ;Entries", 100, 0, 500);
    TH1D* h_80 = new TH1D("h_80", "HLT_HIAK4PFJet80 Turn-on; Jet pT ;Entries", 100, 0, 500);
    
    T_hi->Project("h_80", "JetPT", "HLT_HIAK4PFJet80_v1");
    T_lo->Project("h_60", "JetPT", "HLT_HIAK4PFJet60_v1");
    T_lo->Project("h_40", "JetPT", "HLT_HIAK4PFJet40_v1");

    TCanvas* c= new TCanvas("c_trigger_turnons", "", 800, 600);
    c->SetLogy();
    h_40->SetMaximum(1.5 * TMath::Max(h_40->GetMaximum(), TMath::Max(h_60->GetMaximum(), h_80->GetMaximum())));
    h_40->SetLineColor(kGreen+2);
    h_40->SetLineWidth(2);
    h_60->SetLineColor(kBlue);
    h_60->SetLineWidth(2);
    h_80->SetLineColor(kRed);
    h_80->SetLineWidth(2);
    h_40->Draw();
    h_60->Draw("SAME");
    h_80->Draw("SAME");

    TLegend* L = new TLegend(0.55,0.65,0.88,0.88);
    L->AddEntry(h_40, "HLT_HIAK4PFJet40", "l");
    L->AddEntry(h_60, "HLT_HIAK4PFJet60", "l");
    L->AddEntry(h_80, "HLT_HIAK4PFJet80", "l");
    L->Draw();  

    c->SaveAs("trigger_turnons.pdf");

    TH1D* h_80_on_60 = (TH1D*)h_80->Clone("h_80_on_60");
    TH1D* h_60_on_40 = (TH1D*)h_60->Clone("h_60_on_40");
    TH1D* h_80_on_40 = (TH1D*)h_80->Clone("h_80_on_40");

    h_80_on_60->Divide(h_60);
    h_60_on_40->Divide(h_40);
    h_80_on_40->Divide(h_40);


    TCanvas* c3 = new TCanvas("c_trigger_efficiencies", "", 800, 600);
    c3->cd();
    c3->SetLogy();
    c3->SetGridx();
    c3->SetGridy();
    h_80_on_60->SetLineColor(kRed);
    h_60_on_40->SetLineColor(kBlue);
    h_80_on_40->SetLineColor(kGreen+2);
    h_80_on_60->SetLineWidth(2);
    h_60_on_40->SetLineWidth(2);
    h_80_on_40->SetLineWidth(2);
    h_80_on_60->SetMaximum(50);
    h_80_on_60->SetMinimum(0.5);
    h_80_on_60->SetTitle("Trigger Turn-on Curves; Jet pT; Ratio");

    TLegend* L2 = new TLegend(0.55,0.65,0.88,0.88);
    L2->AddEntry(h_80_on_60, "HLT_HIAK4PFJet80 / HLT_HIAK4PFJet60", "l");
    L2->AddEntry(h_60_on_40, "HLT_HIAK4PFJet60 / HLT_HIAK4PFJet40", "l");
    L2->AddEntry(h_80_on_40, "HLT_HIAK4PFJet80 / HLT_HIAK4PFJet40", "l");

    h_80_on_60->Draw("EP");
    h_60_on_40->Draw("EP SAME");
    h_80_on_40->Draw("EP SAME");
    L2->Draw();
    c3->SaveAs("trigger_turnon_ratios.pdf");

    for(int i = 0; i < 100; i++){
        float pt = h_80_on_60->GetBinCenter(i+1);
        float eff_80_on_60 = h_80_on_60->GetBinContent(i+1) / 6.338;
        float eff_60_on_40 = h_60_on_40->GetBinContent(i+1) / 5.350;
        float eff_80_on_40 = h_80_on_40->GetBinContent(i+1) / 33.910;
        
        printf("JetPT: %.1f GeV, HLT_HIAK4PFJet80 / HLT_HIAK4PFJet60 Efficiency: %.4f, HLT_HIAK4PFJet60 / HLT_HIAK4PFJet40 Efficiency: %.4f, HLT_HIAK4PFJet80 / HLT_HIAK4PFJet40 Efficiency: %.4f\n", pt, eff_80_on_60, eff_60_on_40, eff_80_on_40);

    }
    

    TH1D* h_40_s = new TH1D("h_40_s", "HLT_HIAK4PFJet40 Turn-on; Jet pT ;Entries", 100, 0, 500);
    TH1D* h_60_s = new TH1D("h_60_s", "HLT_HIAK4PFJet60 Turn-on; Jet pT ;Entries", 100, 0, 500);

    T_lo->Project("h_60_s", "JetPT", "HLT_HIAK4PFJet60_v1 && !HLT_HIAK4PFJet80_v1");
    T_lo->Project("h_40_s", "JetPT", "HLT_HIAK4PFJet40_v1 && !HLT_HIAK4PFJet60_v1 && !HLT_HIAK4PFJet80_v1");

    TH1D* h_mc = new TH1D("h_mc", "MC Dimuon pT Spectrum; Jet pT ;Entries", 100, 0, 500);
    T_mc->Project("h_mc", "JetPT", "EventWeight * (IsMuMuTagged || !IsMuMuTagged)");

    TH1D* h_data_s = (TH1D*)h_80->Clone("h_data_s");
    h_data_s->Add(h_60_s, 1);
    h_data_s->Add(h_40_s, 33.910);

    TCanvas* c2= new TCanvas("c_data_mc_triggered", "", 800, 600);
    c2->SetLogy();
    h_mc->Scale(1.0/h_mc->Integral(h_mc->FindBin(80), h_mc->FindBin(500)));
    h_data_s->Scale(1.0/h_data_s->Integral(h_data_s->FindBin(80), h_data_s->FindBin(500)));
    h_mc->SetLineColor(kRed);
    h_mc->SetLineWidth(2);
    h_data_s->SetLineColor(kBlack);
    h_data_s->SetLineWidth(2);
    h_mc->SetMaximum(1.2 * TMath::Max(h_mc->GetMaximum(), h_data_s->GetMaximum()));
    h_mc->Draw("HIST"); 
    h_data_s->Draw("HIST SAME");

    TLegend* L3 = new TLegend(0.55,0.65,0.88,0.88);
    L3->AddEntry(h_data_s, "Data", "l");
    L3->AddEntry(h_mc, "MC with pThat weight", "l");
    L3->Draw();

    c2->SaveAs("data_mc_triggered.pdf");

    // Create THStack showing individual trigger components
    TCanvas* c_stack = new TCanvas("c_stack", "", 800, 600);
    c_stack->SetLogy();
    
    TH1D* h_40_stack = (TH1D*)h_40_s->Clone("h_40_stack");
    TH1D* h_60_stack = (TH1D*)h_60_s->Clone("h_60_stack");
    TH1D* h_80_stack = (TH1D*)h_80->Clone("h_80_stack");

    h_40_stack->SetMinimum(1000);
    h_60_stack->SetMinimum(1000);
    h_80_stack->SetMinimum(1000);
    
    h_40_stack->Scale(33.910);
    h_40_stack->SetFillColor(kGreen+2);
    h_40_stack->SetLineColor(kGreen+2);
    
    h_60_stack->SetFillColor(kBlue);
    h_60_stack->SetLineColor(kBlue);
    
    h_80_stack->SetFillColor(kRed);
    h_80_stack->SetLineColor(kRed);
    
    THStack* hs = new THStack("hs", "Trigger Components; Jet pT; Entries");
    hs->Add(h_40_stack);
    hs->Add(h_60_stack);
    hs->Add(h_80_stack);
    hs->Draw("HIST");
    hs->SetMinimum(1000);
    
    TLegend* L_stack = new TLegend(0.55,0.65,0.88,0.88);
    L_stack->AddEntry(h_40_stack, "HLT_HIAK4PFJet40 (prescale 33.910)", "f");
    L_stack->AddEntry(h_60_stack, "HLT_HIAK4PFJet60 (no prescale)", "f");
    L_stack->AddEntry(h_80_stack, "HLT_HIAK4PFJet80 (no prescale)", "f");
    L_stack->Draw();
    
    c_stack->SaveAs("trigger_stack.pdf");




}
