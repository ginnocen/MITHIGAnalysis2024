

void validate_fitting4(){

    int chargesel = 1;
    TFile*f = TFile::Open("../../alternate_yields/testyields.root");
    TFile* ftruth = TFile::Open("../../mcdistros.root");

    // fit results
    TH1D* h_dr_lf = (TH1D*)f->Get("Yields_uds");
    TH1D* h_dr_hf = (TH1D*)f->Get("Yields_hf");
    TH1D* h_dr_fullyields = (TH1D*)f->Get("Yields_full");
    TH1D* h_dr_fractions = (TH1D*)f->Get("HF_Fraction");

    // truth tntuples
    TNtuple* nt_uds = (TNtuple*)ftruth->Get("nt_uds");
    TNtuple* nt_other = (TNtuple*)ftruth->Get("nt_other");
    TNtuple* nt_c = (TNtuple*)ftruth->Get("nt_c");
    TNtuple* nt_cc = (TNtuple*)ftruth->Get("nt_cc");
    TNtuple* nt_b = (TNtuple*)ftruth->Get("nt_b");
    TNtuple* nt_bb = (TNtuple*)ftruth->Get("nt_bb");

    TH1D* h_truelight = new TH1D("h_truelight", "True Light Flavor Distribution", h_dr_lf->GetNbinsX(), h_dr_lf->GetXaxis()->GetXbins()->GetArray());
    TH1D* h_trueheavy = new TH1D("h_trueheavy", "True Heavy Flavor Distribution", h_dr_hf->GetNbinsX(), h_dr_hf->GetXaxis()->GetXbins()->GetArray());
    TH1D* h_truefractions = new TH1D("h_truefractions", "True Heavy Flavor Fraction", h_dr_fractions->GetNbinsX(), h_dr_fractions->GetXaxis()->GetXbins()->GetArray());
    TH1D* h_truefullyields = new TH1D("h_truefullyields", "True Full Yield Distribution", h_dr_fullyields->GetNbinsX(), h_dr_fullyields->GetXaxis()->GetXbins()->GetArray());

    for(int iBin = 1; iBin <= h_dr_lf->GetNbinsX(); iBin++) {
        float ptMin = h_dr_lf->GetXaxis()->GetBinLowEdge(iBin);
        float ptMax = h_dr_lf->GetXaxis()->GetBinUpEdge(iBin);
        float n_uds = 0;
        float n_other = 0;
        float n_c = 0;
        float n_cc = 0;
        float n_b = 0;
        float n_bb = 0;

        if(chargesel == 0){
            n_uds = nt_uds->GetEntries(Form("JetPT < %f && JetPT >= %f", ptMax, ptMin));
            n_other = nt_other->GetEntries(Form("JetPT < %f && JetPT >= %f", ptMax, ptMin));
            n_c = nt_c->GetEntries(Form("JetPT < %f && JetPT >= %f", ptMax, ptMin));
            n_cc = nt_cc->GetEntries(Form("JetPT < %f && JetPT >= %f", ptMax, ptMin));
            n_b = nt_b->GetEntries(Form("JetPT < %f && JetPT >= %f", ptMax, ptMin));
            n_bb = nt_bb->GetEntries(Form("JetPT < %f && JetPT >= %f", ptMax, ptMin));
        }
        else{
            n_uds = nt_uds->GetEntries(Form("JetPT < %f && JetPT >= %f && Charge == %d", ptMax, ptMin, chargesel));
            n_other = nt_other->GetEntries(Form("JetPT < %f && JetPT >= %f && Charge == %d", ptMax, ptMin, chargesel));
            n_c = nt_c->GetEntries(Form("JetPT < %f && JetPT >= %f && Charge == %d", ptMax, ptMin, chargesel));
            n_cc = nt_cc->GetEntries(Form("JetPT < %f && JetPT >= %f && Charge == %d", ptMax, ptMin, chargesel));
            n_b = nt_b->GetEntries(Form("JetPT < %f && JetPT >= %f && Charge == %d", ptMax, ptMin, chargesel));
            n_bb = nt_bb->GetEntries(Form("JetPT < %f && JetPT >= %f && Charge == %d", ptMax, ptMin, chargesel));
        }

        float n_light = n_uds;
        float n_heavy = n_other + n_c + n_cc + n_b + n_bb;
        float n_full = n_light + n_heavy;

        float binWidth = h_dr_lf->GetXaxis()->GetBinWidth(iBin);
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
    h_dr_lf->SetLineColor(kGreen+2);
    h_dr_lf->SetLineWidth(2);
    h_dr_lf->Draw("LPE SAME");
    TLegend* leg1 = new TLegend(0.45, 0.65, 0.70, 0.88);
    leg1->AddEntry(h_truelight, "True Light Flavor", "l");
    leg1->AddEntry(h_dr_lf, "pT Fit Light Flavor", "l");
    leg1->Draw();
    
    pad1_bottom->cd();
    TH1D* ratio1_dr = (TH1D*)h_dr_lf->Clone("ratio1_dr");
    ratio1_dr->Divide(h_truelight);
    ratio1_dr->SetTitle(";Jet p_{T} [GeV];Fit / Truth");
    ratio1_dr->GetYaxis()->SetRangeUser(0.9, 1.1);
    ratio1_dr->GetYaxis()->SetNdivisions(505);
    ratio1_dr->GetYaxis()->SetTitleSize(0.1);
    ratio1_dr->GetYaxis()->SetLabelSize(0.1);
    ratio1_dr->GetXaxis()->SetTitleSize(0.1);
    ratio1_dr->GetXaxis()->SetLabelSize(0.1);
    ratio1_dr->GetYaxis()->SetTitleOffset(0.5);
    ratio1_dr->Draw("LP");
    TLine* line1 = new TLine(ratio1_dr->GetXaxis()->GetXmin(), 1, ratio1_dr->GetXaxis()->GetXmax(), 1);
    line1->SetLineStyle(2);
    line1->Draw();
    
    c1->SaveAs("validate_lightflavoryield4.pdf");

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
    h_dr_hf->SetLineColor(kGreen+2);
    h_dr_hf->SetLineWidth(2);
    h_dr_hf->Draw("LPE SAME");
    TLegend* leg2 = new TLegend(0.45, 0.65, 0.70, 0.88);
    leg2->AddEntry(h_trueheavy, "True Heavy Flavor", "l");
    leg2->AddEntry(h_dr_hf, "pT Fit Heavy Flavor", "l");
    leg2->Draw();
    
    pad2_bottom->cd();
    TH1D* ratio2_dr = (TH1D*)h_dr_hf->Clone("ratio2_dr");
    ratio2_dr->Divide(h_trueheavy);
    ratio2_dr->SetTitle(";Jet p_{T} [GeV];Fit / Truth");
    ratio2_dr->GetYaxis()->SetRangeUser(0.9, 1.1);
    ratio2_dr->GetYaxis()->SetNdivisions(505);
    ratio2_dr->GetYaxis()->SetTitleSize(0.1);
    ratio2_dr->GetYaxis()->SetLabelSize(0.1);
    ratio2_dr->GetXaxis()->SetTitleSize(0.1);
    ratio2_dr->GetXaxis()->SetLabelSize(0.1);
    ratio2_dr->GetYaxis()->SetTitleOffset(0.5);
    ratio2_dr->Draw("LP");
    TLine* line2 = new TLine(ratio2_dr->GetXaxis()->GetXmin(), 1, ratio2_dr->GetXaxis()->GetXmax(), 1);
    line2->SetLineStyle(2);
    line2->Draw();
    
    c2->SaveAs("validate_heavyflavoryield4.pdf");

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
    h_dr_fractions->SetLineColor(kGreen+2);
    h_dr_fractions->SetLineWidth(2);
    h_dr_fractions->Draw("LPE SAME");
    TLegend* leg3 = new TLegend(0.45, 0.65, 0.70, 0.88);
    leg3->AddEntry(h_truefractions, "True Heavy Flavor Fraction", "l");
    leg3->AddEntry(h_dr_fractions, "pT Fit Heavy Flavor Fraction", "l");
    leg3->Draw();
    
    pad3_bottom->cd();
    TH1D* ratio3_dr = (TH1D*)h_dr_fractions->Clone("ratio3_dr");
    ratio3_dr->Divide(h_truefractions);
    ratio3_dr->SetTitle(";Jet p_{T} [GeV];Fit / Truth");
    ratio3_dr->GetYaxis()->SetRangeUser(0.9, 1.1);
    ratio3_dr->GetYaxis()->SetNdivisions(505);
    ratio3_dr->GetYaxis()->SetTitleSize(0.1);
    ratio3_dr->GetYaxis()->SetLabelSize(0.1);
    ratio3_dr->GetXaxis()->SetTitleSize(0.1);
    ratio3_dr->GetXaxis()->SetLabelSize(0.1);
    ratio3_dr->GetYaxis()->SetTitleOffset(0.5);
    ratio3_dr->Draw("LP");
    TLine* line3 = new TLine(ratio3_dr->GetXaxis()->GetXmin(), 1, ratio3_dr->GetXaxis()->GetXmax(), 1);
    line3->SetLineStyle(2);
    line3->Draw();
    
    c3->SaveAs("validate_heavyflavorfraction4.pdf"); 

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
    h_dr_fullyields->SetLineColor(kGreen+2);
    h_dr_fullyields->SetLineWidth(2);
    h_dr_fullyields->Draw("LPE SAME");
    TLegend* leg4 = new TLegend(0.45, 0.65, 0.70, 0.88);
    leg4->AddEntry(h_truefullyields, "True Full Yield", "l");
    leg4->AddEntry(h_dr_fullyields, "pT Fit Full Yield", "l");
    leg4->Draw();
    
    pad4_bottom->cd();
    TH1D* ratio4_dr = (TH1D*)h_dr_fullyields->Clone("ratio4_dr");
    ratio4_dr->Divide(h_truefullyields);
    ratio4_dr->SetTitle(";Jet p_{T} [GeV];Fit / Truth");
    ratio4_dr->GetYaxis()->SetRangeUser(0.9, 1.1);
    ratio4_dr->GetYaxis()->SetNdivisions(505);
    ratio4_dr->GetYaxis()->SetTitleSize(0.1);
    ratio4_dr->GetYaxis()->SetLabelSize(0.1);
    ratio4_dr->GetXaxis()->SetTitleSize(0.1);
    ratio4_dr->GetXaxis()->SetLabelSize(0.1);
    ratio4_dr->GetYaxis()->SetTitleOffset(0.5);
    ratio4_dr->Draw("LP");
    TLine* line4 = new TLine(ratio4_dr->GetXaxis()->GetXmin(), 1, ratio4_dr->GetXaxis()->GetXmax(), 1);
    line4->SetLineStyle(2);
    line4->Draw();
    
    c4->SaveAs("validate_fullyield4.pdf");


}