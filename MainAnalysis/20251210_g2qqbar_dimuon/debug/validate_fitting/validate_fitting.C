

void validate_fitting(){

    TFile*f = TFile::Open("../../testyields.root");
    TFile* ftruth = TFile::Open("../../testdistros.root");

    // fit results
    TH1D* h_dca_lf = (TH1D*)f->Get("LightYields_DCA");
    TH1D* h_mass_lf = (TH1D*)f->Get("LightYields_InvMass");
    TH1D* h_dr_lf = (TH1D*)f->Get("LightYields_DR");

    TH1D* h_dca_hf = (TH1D*)f->Get("HeavyYields_DCA");
    TH1D* h_mass_hf = (TH1D*)f->Get("HeavyYields_InvMass");
    TH1D* h_dr_hf = (TH1D*)f->Get("HeavyYields_DR");

    TH1D* h_dca_fractions = (TH1D*)f->Get("Fractions_DCA");
    TH1D* h_mass_fractions = (TH1D*)f->Get("Fractions_InvMass");
    TH1D* h_dr_fractions = (TH1D*)f->Get("Fractions_DR");

    TH1D* h_dca_fullyields = (TH1D*)f->Get("FullYields_DCA");
    TH1D* h_mass_fullyields = (TH1D*)f->Get("FullYields_InvMass");
    TH1D* h_dr_fullyields = (TH1D*)f->Get("FullYields_DR");
    
    // Divide fit results by bin width (except fractions)
    for(int iBin = 1; iBin <= h_dca_lf->GetNbinsX(); iBin++) {
        double binWidth = h_dca_lf->GetXaxis()->GetBinWidth(iBin);
        h_dca_lf->SetBinContent(iBin, h_dca_lf->GetBinContent(iBin) / binWidth);
        h_dca_lf->SetBinError(iBin, h_dca_lf->GetBinError(iBin) / binWidth);
        h_mass_lf->SetBinContent(iBin, h_mass_lf->GetBinContent(iBin) / binWidth);
        h_mass_lf->SetBinError(iBin, h_mass_lf->GetBinError(iBin) / binWidth);
        h_dr_lf->SetBinContent(iBin, h_dr_lf->GetBinContent(iBin) / binWidth);
        h_dr_lf->SetBinError(iBin, h_dr_lf->GetBinError(iBin) / binWidth);
        
        h_dca_hf->SetBinContent(iBin, h_dca_hf->GetBinContent(iBin) / binWidth);
        h_dca_hf->SetBinError(iBin, h_dca_hf->GetBinError(iBin) / binWidth);
        h_mass_hf->SetBinContent(iBin, h_mass_hf->GetBinContent(iBin) / binWidth);
        h_mass_hf->SetBinError(iBin, h_mass_hf->GetBinError(iBin) / binWidth);
        h_dr_hf->SetBinContent(iBin, h_dr_hf->GetBinContent(iBin) / binWidth);
        h_dr_hf->SetBinError(iBin, h_dr_hf->GetBinError(iBin) / binWidth);
        
        h_dca_fullyields->SetBinContent(iBin, h_dca_fullyields->GetBinContent(iBin) / binWidth);
        h_dca_fullyields->SetBinError(iBin, h_dca_fullyields->GetBinError(iBin) / binWidth);
        h_mass_fullyields->SetBinContent(iBin, h_mass_fullyields->GetBinContent(iBin) / binWidth);
        h_mass_fullyields->SetBinError(iBin, h_mass_fullyields->GetBinError(iBin) / binWidth);
        h_dr_fullyields->SetBinContent(iBin, h_dr_fullyields->GetBinContent(iBin) / binWidth);
        h_dr_fullyields->SetBinError(iBin, h_dr_fullyields->GetBinError(iBin) / binWidth);
    }

    // truth tntuples
    TNtuple* nt_uds = (TNtuple*)ftruth->Get("nt_uds");
    TNtuple* nt_other = (TNtuple*)ftruth->Get("nt_other");
    TNtuple* nt_c = (TNtuple*)ftruth->Get("nt_c");
    TNtuple* nt_cc = (TNtuple*)ftruth->Get("nt_cc");
    TNtuple* nt_b = (TNtuple*)ftruth->Get("nt_b");
    TNtuple* nt_bb = (TNtuple*)ftruth->Get("nt_bb");

    TH1D* h_truelight = new TH1D("h_truelight", "True Light Flavor Distribution", h_dca_lf->GetNbinsX(), h_dca_lf->GetXaxis()->GetXbins()->GetArray());
    TH1D* h_trueheavy = new TH1D("h_trueheavy", "True Heavy Flavor Distribution", h_dca_hf->GetNbinsX(), h_dca_hf->GetXaxis()->GetXbins()->GetArray());
    TH1D* h_truefractions = new TH1D("h_truefractions", "True Heavy Flavor Fraction", h_dca_fractions->GetNbinsX(), h_dca_fractions->GetXaxis()->GetXbins()->GetArray());
    TH1D* h_truefullyields = new TH1D("h_truefullyields", "True Full Yield Distribution", h_dca_fullyields->GetNbinsX(), h_dca_fullyields->GetXaxis()->GetXbins()->GetArray());

    for(int iBin = 1; iBin <= h_dca_lf->GetNbinsX(); iBin++) {
        float ptMin = h_dca_lf->GetXaxis()->GetBinLowEdge(iBin);
        float ptMax = h_dca_lf->GetXaxis()->GetBinUpEdge(iBin);

        float n_uds = nt_uds->GetEntries(Form("JetPT < %f && JetPT >= %f", ptMax, ptMin));
        float n_other = nt_other->GetEntries(Form("JetPT < %f && JetPT >= %f", ptMax, ptMin));
        float n_c = nt_c->GetEntries(Form("JetPT < %f && JetPT >= %f", ptMax, ptMin));
        float n_cc = nt_cc->GetEntries(Form("JetPT < %f && JetPT >= %f", ptMax, ptMin));
        float n_b = nt_b->GetEntries(Form("JetPT < %f && JetPT >= %f", ptMax, ptMin));
        float n_bb = nt_bb->GetEntries(Form("JetPT < %f && JetPT >= %f", ptMax, ptMin));

        float n_light = n_uds;
        float n_heavy = n_other + n_c + n_cc + n_b + n_bb;
        float n_full = n_light + n_heavy;

        float binWidth = h_dca_lf->GetXaxis()->GetBinWidth(iBin);
        h_truelight->SetBinContent(iBin, n_light / binWidth);
        h_truelight->SetBinError(iBin, sqrt(n_light) / binWidth);
        h_trueheavy->SetBinContent(iBin, n_heavy / binWidth);
        h_trueheavy->SetBinError(iBin, sqrt(n_heavy) / binWidth);
        
        float fraction = n_heavy / n_full;
        float fraction_error = (n_full > 0 && n_heavy > 0) ? fraction * sqrt(1.0/n_heavy + 1.0/n_full) : 0;
        h_truefractions->SetBinContent(iBin, fraction);
        h_truefractions->SetBinError(iBin, fraction_error);
        
        h_truefullyields->SetBinContent(iBin, n_full / binWidth);
        h_truefullyields->SetBinError(iBin, sqrt(n_full) / binWidth);
    }

    TCanvas* c1 = new TCanvas("c1", "", 800, 800);
    TPad* pad1_top = new TPad("pad1_top", "", 0, 0.3, 1, 1);
    TPad* pad1_bottom = new TPad("pad1_bottom", "", 0, 0, 1, 0.3);
    pad1_top->SetBottomMargin(0.02);
    pad1_bottom->SetTopMargin(0.02);
    pad1_bottom->SetBottomMargin(0.3);
    pad1_top->Draw();
    pad1_bottom->Draw();
    
    pad1_top->cd();
    h_truelight->SetLineColor(kBlack);
    h_truelight->SetLineWidth(2);
    h_truelight->SetTitle("Light Flavor Yield Comparison;Jet p_{T} [GeV];Entries");
    h_truelight->GetXaxis()->SetLabelSize(0);
    h_truelight->Draw("LPE"); 
    h_dca_lf->SetLineColor(kRed);
    h_dca_lf->SetLineWidth(2);
    h_dca_lf->Draw("LPE SAME");  
    h_mass_lf->SetLineColor(kBlue);
    h_mass_lf->SetLineWidth(2);
    h_mass_lf->Draw("LPE SAME"); 
    h_dr_lf->SetLineColor(kGreen+2);
    h_dr_lf->SetLineWidth(2);
    h_dr_lf->Draw("LPE SAME");
    TLegend* leg1 = new TLegend(0.45, 0.65, 0.70, 0.88);
    leg1->AddEntry(h_truelight, "True Light Flavor", "l");
    leg1->AddEntry(h_dca_lf, "DCA Fit Light Flavor", "l");
    leg1->AddEntry(h_mass_lf, "Inv Mass Fit Light Flavor", "l");
    leg1->AddEntry(h_dr_lf, "DR Fit Light Flavor", "l");
    leg1->Draw();
    
    pad1_bottom->cd();
    TH1D* ratio1_dca = (TH1D*)h_dca_lf->Clone("ratio1_dca");
    TH1D* ratio1_mass = (TH1D*)h_mass_lf->Clone("ratio1_mass");
    TH1D* ratio1_dr = (TH1D*)h_dr_lf->Clone("ratio1_dr");
    ratio1_dca->Divide(h_truelight);
    ratio1_mass->Divide(h_truelight);
    ratio1_dr->Divide(h_truelight);
    ratio1_dca->SetTitle(";Jet p_{T} [GeV];Fit / Truth");
    ratio1_dca->GetYaxis()->SetRangeUser(0.9, 1.1);
    ratio1_dca->GetYaxis()->SetNdivisions(505);
    ratio1_dca->GetYaxis()->SetTitleSize(0.1);
    ratio1_dca->GetYaxis()->SetLabelSize(0.1);
    ratio1_dca->GetXaxis()->SetTitleSize(0.1);
    ratio1_dca->GetXaxis()->SetLabelSize(0.1);
    ratio1_dca->GetYaxis()->SetTitleOffset(0.5);
    ratio1_dca->Draw("LP");
    ratio1_mass->Draw("LP SAME");
    ratio1_dr->Draw("LP SAME");
    TLine* line1 = new TLine(ratio1_dca->GetXaxis()->GetXmin(), 1, ratio1_dca->GetXaxis()->GetXmax(), 1);
    line1->SetLineStyle(2);
    line1->Draw();
    
    c1->SaveAs("validate_lightflavoryield.pdf");

    TCanvas* c2 = new TCanvas("c2", "", 800, 800);
    TPad* pad2_top = new TPad("pad2_top", "", 0, 0.3, 1, 1);
    TPad* pad2_bottom = new TPad("pad2_bottom", "", 0, 0, 1, 0.3);
    pad2_top->SetBottomMargin(0.02);
    pad2_bottom->SetTopMargin(0.02);
    pad2_bottom->SetBottomMargin(0.3);
    pad2_top->Draw();
    pad2_bottom->Draw();
    
    pad2_top->cd();
    h_trueheavy->SetLineColor(kBlack);
    h_trueheavy->SetLineWidth(2);
    h_trueheavy->SetTitle("Heavy Flavor Yield Comparison;Jet p_{T} [GeV];Entries");
    h_trueheavy->GetXaxis()->SetLabelSize(0);
    h_trueheavy->Draw("LPE"); 
    h_dca_hf->SetLineColor(kRed);
    h_dca_hf->SetLineWidth(2);
    h_dca_hf->Draw("LPE SAME");  
    h_mass_hf->SetLineColor(kBlue);
    h_mass_hf->SetLineWidth(2);
    h_mass_hf->Draw("LPE SAME"); 
    h_dr_hf->SetLineColor(kGreen+2);
    h_dr_hf->SetLineWidth(2);
    h_dr_hf->Draw("LPE SAME");
    TLegend* leg2 = new TLegend(0.45, 0.65, 0.70, 0.88);
    leg2->AddEntry(h_trueheavy, "True Heavy Flavor", "l");
    leg2->AddEntry(h_dca_hf, "DCA Fit Heavy Flavor", "l");
    leg2->AddEntry(h_mass_hf, "Inv Mass Fit Heavy Flavor", "l");
    leg2->AddEntry(h_dr_hf, "DR Fit Heavy Flavor", "l");
    leg2->Draw();
    
    pad2_bottom->cd();
    TH1D* ratio2_dca = (TH1D*)h_dca_hf->Clone("ratio2_dca");
    TH1D* ratio2_mass = (TH1D*)h_mass_hf->Clone("ratio2_mass");
    TH1D* ratio2_dr = (TH1D*)h_dr_hf->Clone("ratio2_dr");
    ratio2_dca->Divide(h_trueheavy);
    ratio2_mass->Divide(h_trueheavy);
    ratio2_dr->Divide(h_trueheavy);
    ratio2_dca->SetTitle(";Jet p_{T} [GeV];Fit / Truth");
    ratio2_dca->GetYaxis()->SetRangeUser(0.9, 1.1);
    ratio2_dca->GetYaxis()->SetNdivisions(505);
    ratio2_dca->GetYaxis()->SetTitleSize(0.1);
    ratio2_dca->GetYaxis()->SetLabelSize(0.1);
    ratio2_dca->GetXaxis()->SetTitleSize(0.1);
    ratio2_dca->GetXaxis()->SetLabelSize(0.1);
    ratio2_dca->GetYaxis()->SetTitleOffset(0.5);
    ratio2_dca->Draw("LP");
    ratio2_mass->Draw("LP SAME");
    ratio2_dr->Draw("LP SAME");
    TLine* line2 = new TLine(ratio2_dca->GetXaxis()->GetXmin(), 1, ratio2_dca->GetXaxis()->GetXmax(), 1);
    line2->SetLineStyle(2);
    line2->Draw();
    
    c2->SaveAs("validate_heavyflavoryield.pdf");

    TCanvas* c3 = new TCanvas("c3", "", 800, 800);
    TPad* pad3_top = new TPad("pad3_top", "", 0, 0.3, 1, 1);
    TPad* pad3_bottom = new TPad("pad3_bottom", "", 0, 0, 1, 0.3);
    pad3_top->SetBottomMargin(0.02);
    pad3_bottom->SetTopMargin(0.02);
    pad3_bottom->SetBottomMargin(0.3);
    pad3_top->Draw();
    pad3_bottom->Draw();
    
    pad3_top->cd();
    h_truefractions->SetLineColor(kBlack);
    h_truefractions->SetLineWidth(2);
    h_truefractions->SetTitle("Heavy Flavor Fraction Comparison;Jet p_{T} [GeV];Fraction");
    h_truefractions->GetXaxis()->SetLabelSize(0);
    h_truefractions->Draw("LPE"); 
    h_dca_fractions->SetLineColor(kRed);
    h_dca_fractions->SetLineWidth(2);
    h_dca_fractions->Draw("LPE SAME");  
    h_mass_fractions->SetLineColor(kBlue);
    h_mass_fractions->SetLineWidth(2);
    h_mass_fractions->Draw("LPE SAME");
    h_dr_fractions->SetLineColor(kGreen+2);
    h_dr_fractions->SetLineWidth(2);
    h_dr_fractions->Draw("LPE SAME");
    TLegend* leg3 = new TLegend(0.45, 0.65, 0.70, 0.88);
    leg3->AddEntry(h_truefractions, "True Heavy Flavor Fraction", "l");
    leg3->AddEntry(h_dca_fractions, "DCA Fit Heavy Flavor Fraction", "l");
    leg3->AddEntry(h_mass_fractions, "Inv Mass Fit Heavy Flavor Fraction", "l");
    leg3->AddEntry(h_dr_fractions, "DR Fit Heavy Flavor Fraction", "l");
    leg3->Draw();
    
    pad3_bottom->cd();
    TH1D* ratio3_dca = (TH1D*)h_dca_fractions->Clone("ratio3_dca");
    TH1D* ratio3_mass = (TH1D*)h_mass_fractions->Clone("ratio3_mass");
    TH1D* ratio3_dr = (TH1D*)h_dr_fractions->Clone("ratio3_dr");
    ratio3_dca->Divide(h_truefractions);
    ratio3_mass->Divide(h_truefractions);
    ratio3_dr->Divide(h_truefractions);
    ratio3_dca->SetTitle(";Jet p_{T} [GeV];Fit / Truth");
    ratio3_dca->GetYaxis()->SetRangeUser(0.9, 1.1);
    ratio3_dca->GetYaxis()->SetNdivisions(505);
    ratio3_dca->GetYaxis()->SetTitleSize(0.1);
    ratio3_dca->GetYaxis()->SetLabelSize(0.1);
    ratio3_dca->GetXaxis()->SetTitleSize(0.1);
    ratio3_dca->GetXaxis()->SetLabelSize(0.1);
    ratio3_dca->GetYaxis()->SetTitleOffset(0.5);
    ratio3_dca->Draw("LP");
    ratio3_mass->Draw("LP SAME");
    ratio3_dr->Draw("LP SAME");
    TLine* line3 = new TLine(ratio3_dca->GetXaxis()->GetXmin(), 1, ratio3_dca->GetXaxis()->GetXmax(), 1);
    line3->SetLineStyle(2);
    line3->Draw();
    
    c3->SaveAs("validate_heavyflavorfraction.pdf"); 

    TCanvas* c4 = new TCanvas("c4", "", 800, 800);
    TPad* pad4_top = new TPad("pad4_top", "", 0, 0.3, 1, 1);
    TPad* pad4_bottom = new TPad("pad4_bottom", "", 0, 0, 1, 0.3);
    pad4_top->SetBottomMargin(0.02);
    pad4_bottom->SetTopMargin(0.02);
    pad4_bottom->SetBottomMargin(0.3);
    pad4_top->Draw();
    pad4_bottom->Draw();
    
    pad4_top->cd();
    h_truefullyields->SetLineColor(kBlack);
    h_truefullyields->SetLineWidth(2);
    h_truefullyields->SetTitle("Full Yield Comparison;Jet p_{T} [GeV];Entries");
    h_truefullyields->GetXaxis()->SetLabelSize(0);
    h_truefullyields->Draw("LPE"); 
    h_dca_fullyields->SetLineColor(kRed);
    h_dca_fullyields->SetLineWidth(2);
    h_dca_fullyields->Draw("LPE SAME");  
    h_mass_fullyields->SetLineColor(kBlue);
    h_mass_fullyields->SetLineWidth(2);
    h_mass_fullyields->Draw("LPE SAME"); 
    h_dr_fullyields->SetLineColor(kGreen+2);
    h_dr_fullyields->SetLineWidth(2);
    h_dr_fullyields->Draw("LPE SAME");
    TLegend* leg4 = new TLegend(0.45, 0.65, 0.70, 0.88);
    leg4->AddEntry(h_truefullyields, "True Full Yield", "l");
    leg4->AddEntry(h_dca_fullyields, "DCA Fit Full Yield", "l");
    leg4->AddEntry(h_mass_fullyields, "Inv Mass Fit Full Yield", "l");
    leg4->AddEntry(h_dr_fullyields, "DR Fit Full Yield", "l");
    leg4->Draw();
    
    pad4_bottom->cd();
    TH1D* ratio4_dca = (TH1D*)h_dca_fullyields->Clone("ratio4_dca");
    TH1D* ratio4_mass = (TH1D*)h_mass_fullyields->Clone("ratio4_mass");
    TH1D* ratio4_dr = (TH1D*)h_dr_fullyields->Clone("ratio4_dr");
    ratio4_dca->Divide(h_truefullyields);
    ratio4_mass->Divide(h_truefullyields);
    ratio4_dr->Divide(h_truefullyields);
    ratio4_dca->SetTitle(";Jet p_{T} [GeV];Fit / Truth");
    ratio4_dca->GetYaxis()->SetRangeUser(0.9, 1.1);
    ratio4_dca->GetYaxis()->SetNdivisions(505);
    ratio4_dca->GetYaxis()->SetTitleSize(0.1);
    ratio4_dca->GetYaxis()->SetLabelSize(0.1);
    ratio4_dca->GetXaxis()->SetTitleSize(0.1);
    ratio4_dca->GetXaxis()->SetLabelSize(0.1);
    ratio4_dca->GetYaxis()->SetTitleOffset(0.5);
    ratio4_dca->Draw("LP");
    ratio4_mass->Draw("LP SAME");
    ratio4_dr->Draw("LP SAME");
    TLine* line4 = new TLine(ratio4_dca->GetXaxis()->GetXmin(), 1, ratio4_dca->GetXaxis()->GetXmax(), 1);
    line4->SetLineStyle(2);
    line4->Draw();
    
    c4->SaveAs("validate_fullyield.pdf");


}