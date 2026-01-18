

void validate_fitting2(){

    TFile*f = TFile::Open("../../alternate_yields/testyields.root");
    TFile* ftruth = TFile::Open("../../mcdistros.root");

    // fit results - get individual components
    TH1D* h_dca_uds = (TH1D*)f->Get("udsYields_muDiDxy1Dxy2Sig");
    TH1D* h_dca_other = (TH1D*)f->Get("OtherYields_muDiDxy1Dxy2Sig");
    TH1D* h_dca_c = (TH1D*)f->Get("CYields_muDiDxy1Dxy2Sig");
    TH1D* h_dca_cc = (TH1D*)f->Get("CCYields_muDiDxy1Dxy2Sig");
    TH1D* h_dca_b = (TH1D*)f->Get("BYields_muDiDxy1Dxy2Sig");
    TH1D* h_dca_bb = (TH1D*)f->Get("BBYields_muDiDxy1Dxy2Sig");
    
    TH1D* h_mass_uds = (TH1D*)f->Get("udsYields_mumuMass");
    TH1D* h_mass_other = (TH1D*)f->Get("OtherYields_mumuMass");
    TH1D* h_mass_c = (TH1D*)f->Get("CYields_mumuMass");
    TH1D* h_mass_cc = (TH1D*)f->Get("CCYields_mumuMass");
    TH1D* h_mass_b = (TH1D*)f->Get("BYields_mumuMass");
    TH1D* h_mass_bb = (TH1D*)f->Get("BBYields_mumuMass");
    
    TH1D* h_dr_uds = (TH1D*)f->Get("udsYields_muDR");
    TH1D* h_dr_other = (TH1D*)f->Get("OtherYields_muDR");
    TH1D* h_dr_c = (TH1D*)f->Get("CYields_muDR");
    TH1D* h_dr_cc = (TH1D*)f->Get("CCYields_muDR");
    TH1D* h_dr_b = (TH1D*)f->Get("BYields_muDR");
    TH1D* h_dr_bb = (TH1D*)f->Get("BBYields_muDR");
    
    // truth tntuples
    TNtuple* nt_uds = (TNtuple*)ftruth->Get("nt_uds");
    TNtuple* nt_other = (TNtuple*)ftruth->Get("nt_other");
    TNtuple* nt_c = (TNtuple*)ftruth->Get("nt_c");
    TNtuple* nt_cc = (TNtuple*)ftruth->Get("nt_cc");
    TNtuple* nt_b = (TNtuple*)ftruth->Get("nt_b");
    TNtuple* nt_bb = (TNtuple*)ftruth->Get("nt_bb");

    TH1D* h_true_uds = new TH1D("h_true_uds", "True uds Distribution", h_dca_uds->GetNbinsX(), h_dca_uds->GetXaxis()->GetXbins()->GetArray());
    TH1D* h_true_other = new TH1D("h_true_other", "True other Distribution", h_dca_other->GetNbinsX(), h_dca_other->GetXaxis()->GetXbins()->GetArray());
    TH1D* h_true_c = new TH1D("h_true_c", "True c Distribution", h_dca_c->GetNbinsX(), h_dca_c->GetXaxis()->GetXbins()->GetArray());
    TH1D* h_true_cc = new TH1D("h_true_cc", "True cc Distribution", h_dca_cc->GetNbinsX(), h_dca_cc->GetXaxis()->GetXbins()->GetArray());
    TH1D* h_true_b = new TH1D("h_true_b", "True b Distribution", h_dca_b->GetNbinsX(), h_dca_b->GetXaxis()->GetXbins()->GetArray());
    TH1D* h_true_bb = new TH1D("h_true_bb", "True bb Distribution", h_dca_bb->GetNbinsX(), h_dca_bb->GetXaxis()->GetXbins()->GetArray());



    for(int iBin = 1; iBin <= h_dca_uds->GetNbinsX(); iBin++) {
        float ptMin = h_dca_uds->GetXaxis()->GetBinLowEdge(iBin);
        float ptMax = h_dca_uds->GetXaxis()->GetBinUpEdge(iBin);

        float n_uds = nt_uds->GetEntries(Form("JetPT < %f && JetPT >= %f", ptMax, ptMin));
        float n_other = nt_other->GetEntries(Form("JetPT < %f && JetPT >= %f", ptMax, ptMin));
        float n_c = nt_c->GetEntries(Form("JetPT < %f && JetPT >= %f", ptMax, ptMin));
        float n_cc = nt_cc->GetEntries(Form("JetPT < %f && JetPT >= %f", ptMax, ptMin));
        float n_b = nt_b->GetEntries(Form("JetPT < %f && JetPT >= %f", ptMax, ptMin));
        float n_bb = nt_bb->GetEntries(Form("JetPT < %f && JetPT >= %f", ptMax, ptMin));

        float binWidth = h_dca_uds->GetXaxis()->GetBinWidth(iBin);
        h_true_uds->SetBinContent(iBin, n_uds / binWidth);
        h_true_uds->SetBinError(iBin, sqrt(n_uds) / binWidth);
        h_true_other->SetBinContent(iBin, n_other / binWidth);
        h_true_other->SetBinError(iBin, sqrt(n_other) / binWidth);
        h_true_c->SetBinContent(iBin, n_c / binWidth);
        h_true_c->SetBinError(iBin, sqrt(n_c) / binWidth);
        h_true_cc->SetBinContent(iBin, n_cc / binWidth);
        h_true_cc->SetBinError(iBin, sqrt(n_cc) / binWidth);
        h_true_b->SetBinContent(iBin, n_b / binWidth);
        h_true_b->SetBinError(iBin, sqrt(n_b) / binWidth);
        h_true_bb->SetBinContent(iBin, n_bb / binWidth);
        h_true_bb->SetBinError(iBin, sqrt(n_bb) / binWidth);
    }

    // ========== UDS PLOT ==========
    TCanvas* c_uds = new TCanvas("c_uds", "", 800, 800);
    TPad* pad_uds_top = new TPad("pad_uds_top", "", 0, 0.3, 1, 1);
    TPad* pad_uds_bottom = new TPad("pad_uds_bottom", "", 0, 0, 1, 0.3);
    pad_uds_top->SetBottomMargin(0.02);
    pad_uds_bottom->SetTopMargin(0.02);
    pad_uds_bottom->SetBottomMargin(0.3);
    pad_uds_top->Draw();
    pad_uds_bottom->Draw();
    
    pad_uds_top->cd();
    h_true_uds->SetLineColor(kBlack);
    h_true_uds->SetLineWidth(2);
    h_true_uds->SetTitle("Light Flavor (uds) Yield Comparison;Jet p_{T} [GeV];dN/dp_{T}");
    h_true_uds->GetXaxis()->SetLabelSize(0);
    h_true_uds->Draw("LPE");
    h_dca_uds->SetLineColor(kRed);
    h_dca_uds->SetLineWidth(2);
    h_dca_uds->Draw("LPE SAME");
    h_mass_uds->SetLineColor(kBlue);
    h_mass_uds->SetLineWidth(2);
    h_mass_uds->Draw("LPE SAME");
    h_dr_uds->SetLineColor(kGreen+2);
    h_dr_uds->SetLineWidth(2);
    h_dr_uds->Draw("LPE SAME");
    TLegend* leg_uds = new TLegend(0.45, 0.65, 0.70, 0.88);
    leg_uds->AddEntry(h_true_uds, "True uds", "l");
    leg_uds->AddEntry(h_dca_uds, "DCA Fit", "l");
    leg_uds->AddEntry(h_mass_uds, "Mass Fit", "l");
    leg_uds->AddEntry(h_dr_uds, "DR Fit", "l");
    leg_uds->Draw();
    
    pad_uds_bottom->cd();
    TH1D* ratio_uds_dca = (TH1D*)h_dca_uds->Clone("ratio_uds_dca");
    TH1D* ratio_uds_mass = (TH1D*)h_mass_uds->Clone("ratio_uds_mass");
    TH1D* ratio_uds_dr = (TH1D*)h_dr_uds->Clone("ratio_uds_dr");
    ratio_uds_dca->Divide(h_true_uds);
    ratio_uds_mass->Divide(h_true_uds);
    ratio_uds_dr->Divide(h_true_uds);
    ratio_uds_dca->SetTitle(";Jet p_{T} [GeV];Fit / Truth");
    ratio_uds_dca->GetYaxis()->SetRangeUser(0.7, 1.3);
    ratio_uds_dca->GetYaxis()->SetNdivisions(505);
    ratio_uds_dca->GetYaxis()->SetTitleSize(0.1);
    ratio_uds_dca->GetYaxis()->SetLabelSize(0.1);
    ratio_uds_dca->GetXaxis()->SetTitleSize(0.1);
    ratio_uds_dca->GetXaxis()->SetLabelSize(0.1);
    ratio_uds_dca->GetYaxis()->SetTitleOffset(0.5);
    ratio_uds_dca->Draw("LP");
    ratio_uds_mass->Draw("LP SAME");
    ratio_uds_dr->Draw("LP SAME");
    TLine* line_uds = new TLine(ratio_uds_dca->GetXaxis()->GetXmin(), 1, ratio_uds_dca->GetXaxis()->GetXmax(), 1);
    line_uds->SetLineStyle(2);
    line_uds->Draw();
    c_uds->SaveAs("validate_uds_yields2.pdf");

    // ========== OTHER PLOT ==========
    TCanvas* c_other = new TCanvas("c_other", "", 800, 800);
    TPad* pad_other_top = new TPad("pad_other_top", "", 0, 0.3, 1, 1);
    TPad* pad_other_bottom = new TPad("pad_other_bottom", "", 0, 0, 1, 0.3);
    pad_other_top->SetBottomMargin(0.02);
    pad_other_bottom->SetTopMargin(0.02);
    pad_other_bottom->SetBottomMargin(0.3);
    pad_other_top->Draw();
    pad_other_bottom->Draw();
    
    pad_other_top->cd();
    h_true_other->SetLineColor(kBlack);
    h_true_other->SetLineWidth(2);
    h_true_other->SetTitle("Heavy Flavor Other Yield Comparison;Jet p_{T} [GeV];dN/dp_{T}");
    h_true_other->GetXaxis()->SetLabelSize(0);
    h_true_other->Draw("LPE");
    h_dca_other->SetLineColor(kRed);
    h_dca_other->SetLineWidth(2);
    h_dca_other->Draw("LPE SAME");
    h_mass_other->SetLineColor(kBlue);
    h_mass_other->SetLineWidth(2);
    h_mass_other->Draw("LPE SAME");
    h_dr_other->SetLineColor(kGreen+2);
    h_dr_other->SetLineWidth(2);
    h_dr_other->Draw("LPE SAME");
    TLegend* leg_other = new TLegend(0.45, 0.65, 0.70, 0.88);
    leg_other->AddEntry(h_true_other, "True Other", "l");
    leg_other->AddEntry(h_dca_other, "DCA Fit", "l");
    leg_other->AddEntry(h_mass_other, "Mass Fit", "l");
    leg_other->AddEntry(h_dr_other, "DR Fit", "l");
    leg_other->Draw();
    
    pad_other_bottom->cd();
    TH1D* ratio_other_dca = (TH1D*)h_dca_other->Clone("ratio_other_dca");
    TH1D* ratio_other_mass = (TH1D*)h_mass_other->Clone("ratio_other_mass");
    TH1D* ratio_other_dr = (TH1D*)h_dr_other->Clone("ratio_other_dr");
    ratio_other_dca->Divide(h_true_other);
    ratio_other_mass->Divide(h_true_other);
    ratio_other_dr->Divide(h_true_other);
    ratio_other_dca->SetTitle(";Jet p_{T} [GeV];Fit / Truth");
    ratio_other_dca->GetYaxis()->SetRangeUser(0.7, 1.3);
    ratio_other_dca->GetYaxis()->SetNdivisions(505);
    ratio_other_dca->GetYaxis()->SetTitleSize(0.1);
    ratio_other_dca->GetYaxis()->SetLabelSize(0.1);
    ratio_other_dca->GetXaxis()->SetTitleSize(0.1);
    ratio_other_dca->GetXaxis()->SetLabelSize(0.1);
    ratio_other_dca->GetYaxis()->SetTitleOffset(0.5);
    ratio_other_dca->Draw("LP");
    ratio_other_mass->Draw("LP SAME");
    ratio_other_dr->Draw("LP SAME");
    TLine* line_other = new TLine(ratio_other_dca->GetXaxis()->GetXmin(), 1, ratio_other_dca->GetXaxis()->GetXmax(), 1);
    line_other->SetLineStyle(2);
    line_other->Draw();
    c_other->SaveAs("validate_other_yields2.pdf");

    // ========== C PLOT ==========
    TCanvas* c_c = new TCanvas("c_c", "", 800, 800);
    TPad* pad_c_top = new TPad("pad_c_top", "", 0, 0.3, 1, 1);
    TPad* pad_c_bottom = new TPad("pad_c_bottom", "", 0, 0, 1, 0.3);
    pad_c_top->SetBottomMargin(0.02);
    pad_c_bottom->SetTopMargin(0.02);
    pad_c_bottom->SetBottomMargin(0.3);
    pad_c_top->Draw();
    pad_c_bottom->Draw();
    
    pad_c_top->cd();
    h_true_c->SetLineColor(kBlack);
    h_true_c->SetLineWidth(2);
    h_true_c->SetTitle("Heavy Flavor c Yield Comparison;Jet p_{T} [GeV];dN/dp_{T}");
    h_true_c->GetXaxis()->SetLabelSize(0);
    h_true_c->Draw("LPE");
    h_dca_c->SetLineColor(kRed);
    h_dca_c->SetLineWidth(2);
    h_dca_c->Draw("LPE SAME");
    h_mass_c->SetLineColor(kBlue);
    h_mass_c->SetLineWidth(2);
    h_mass_c->Draw("LPE SAME");
    h_dr_c->SetLineColor(kGreen+2);
    h_dr_c->SetLineWidth(2);
    h_dr_c->Draw("LPE SAME");
    TLegend* leg_c = new TLegend(0.45, 0.65, 0.70, 0.88);
    leg_c->AddEntry(h_true_c, "True c", "l");
    leg_c->AddEntry(h_dca_c, "DCA Fit", "l");
    leg_c->AddEntry(h_mass_c, "Mass Fit", "l");
    leg_c->AddEntry(h_dr_c, "DR Fit", "l");
    leg_c->Draw();
    
    pad_c_bottom->cd();
    TH1D* ratio_c_dca = (TH1D*)h_dca_c->Clone("ratio_c_dca");
    TH1D* ratio_c_mass = (TH1D*)h_mass_c->Clone("ratio_c_mass");
    TH1D* ratio_c_dr = (TH1D*)h_dr_c->Clone("ratio_c_dr");
    ratio_c_dca->Divide(h_true_c);
    ratio_c_mass->Divide(h_true_c);
    ratio_c_dr->Divide(h_true_c);
    ratio_c_dca->SetTitle(";Jet p_{T} [GeV];Fit / Truth");
    ratio_c_dca->GetYaxis()->SetRangeUser(0.7, 1.3);
    ratio_c_dca->GetYaxis()->SetNdivisions(505);
    ratio_c_dca->GetYaxis()->SetTitleSize(0.1);
    ratio_c_dca->GetYaxis()->SetLabelSize(0.1);
    ratio_c_dca->GetXaxis()->SetTitleSize(0.1);
    ratio_c_dca->GetXaxis()->SetLabelSize(0.1);
    ratio_c_dca->GetYaxis()->SetTitleOffset(0.5);
    ratio_c_dca->Draw("LP");
    ratio_c_mass->Draw("LP SAME");
    ratio_c_dr->Draw("LP SAME");
    TLine* line_c = new TLine(ratio_c_dca->GetXaxis()->GetXmin(), 1, ratio_c_dca->GetXaxis()->GetXmax(), 1);
    line_c->SetLineStyle(2);
    line_c->Draw();
    c_c->SaveAs("validate_c_yields2.pdf");

    // ========== CC PLOT ==========
    TCanvas* c_cc = new TCanvas("c_cc", "", 800, 800);
    TPad* pad_cc_top = new TPad("pad_cc_top", "", 0, 0.3, 1, 1);
    TPad* pad_cc_bottom = new TPad("pad_cc_bottom", "", 0, 0, 1, 0.3);
    pad_cc_top->SetBottomMargin(0.02);
    pad_cc_bottom->SetTopMargin(0.02);
    pad_cc_bottom->SetBottomMargin(0.3);
    pad_cc_top->Draw();
    pad_cc_bottom->Draw();
    
    pad_cc_top->cd();
    h_true_cc->SetLineColor(kBlack);
    h_true_cc->SetLineWidth(2);
    h_true_cc->SetTitle("Heavy Flavor cc Yield Comparison;Jet p_{T} [GeV];dN/dp_{T}");
    h_true_cc->GetXaxis()->SetLabelSize(0);
    h_true_cc->Draw("LPE");
    h_dca_cc->SetLineColor(kRed);
    h_dca_cc->SetLineWidth(2);
    h_dca_cc->Draw("LPE SAME");
    h_mass_cc->SetLineColor(kBlue);
    h_mass_cc->SetLineWidth(2);
    h_mass_cc->Draw("LPE SAME");
    h_dr_cc->SetLineColor(kGreen+2);
    h_dr_cc->SetLineWidth(2);
    h_dr_cc->Draw("LPE SAME");
    TLegend* leg_cc = new TLegend(0.45, 0.65, 0.70, 0.88);
    leg_cc->AddEntry(h_true_cc, "True cc", "l");
    leg_cc->AddEntry(h_dca_cc, "DCA Fit", "l");
    leg_cc->AddEntry(h_mass_cc, "Mass Fit", "l");
    leg_cc->AddEntry(h_dr_cc, "DR Fit", "l");
    leg_cc->Draw();
    
    pad_cc_bottom->cd();
    TH1D* ratio_cc_dca = (TH1D*)h_dca_cc->Clone("ratio_cc_dca");
    TH1D* ratio_cc_mass = (TH1D*)h_mass_cc->Clone("ratio_cc_mass");
    TH1D* ratio_cc_dr = (TH1D*)h_dr_cc->Clone("ratio_cc_dr");
    ratio_cc_dca->Divide(h_true_cc);
    ratio_cc_mass->Divide(h_true_cc);
    ratio_cc_dr->Divide(h_true_cc);
    ratio_cc_dca->SetTitle(";Jet p_{T} [GeV];Fit / Truth");
    ratio_cc_dca->GetYaxis()->SetRangeUser(0.7, 1.3);
    ratio_cc_dca->GetYaxis()->SetNdivisions(505);
    ratio_cc_dca->GetYaxis()->SetTitleSize(0.1);
    ratio_cc_dca->GetYaxis()->SetLabelSize(0.1);
    ratio_cc_dca->GetXaxis()->SetTitleSize(0.1);
    ratio_cc_dca->GetXaxis()->SetLabelSize(0.1);
    ratio_cc_dca->GetYaxis()->SetTitleOffset(0.5);
    ratio_cc_dca->Draw("LP");
    ratio_cc_mass->Draw("LP SAME");
    ratio_cc_dr->Draw("LP SAME");
    TLine* line_cc = new TLine(ratio_cc_dca->GetXaxis()->GetXmin(), 1, ratio_cc_dca->GetXaxis()->GetXmax(), 1);
    line_cc->SetLineStyle(2);
    line_cc->Draw();
    c_cc->SaveAs("validate_cc_yields2.pdf");

    // ========== B PLOT ==========
    TCanvas* c_b = new TCanvas("c_b", "", 800, 800);
    TPad* pad_b_top = new TPad("pad_b_top", "", 0, 0.3, 1, 1);
    TPad* pad_b_bottom = new TPad("pad_b_bottom", "", 0, 0, 1, 0.3);
    pad_b_top->SetBottomMargin(0.02);
    pad_b_bottom->SetTopMargin(0.02);
    pad_b_bottom->SetBottomMargin(0.3);
    pad_b_top->Draw();
    pad_b_bottom->Draw();
    
    pad_b_top->cd();
    h_true_b->SetLineColor(kBlack);
    h_true_b->SetLineWidth(2);
    h_true_b->SetTitle("Heavy Flavor b Yield Comparison;Jet p_{T} [GeV];dN/dp_{T}");
    h_true_b->GetXaxis()->SetLabelSize(0);
    h_true_b->Draw("LPE");
    h_dca_b->SetLineColor(kRed);
    h_dca_b->SetLineWidth(2);
    h_dca_b->Draw("LPE SAME");
    h_mass_b->SetLineColor(kBlue);
    h_mass_b->SetLineWidth(2);
    h_mass_b->Draw("LPE SAME");
    h_dr_b->SetLineColor(kGreen+2);
    h_dr_b->SetLineWidth(2);
    h_dr_b->Draw("LPE SAME");
    TLegend* leg_b = new TLegend(0.45, 0.65, 0.70, 0.88);
    leg_b->AddEntry(h_true_b, "True b", "l");
    leg_b->AddEntry(h_dca_b, "DCA Fit", "l");
    leg_b->AddEntry(h_mass_b, "Mass Fit", "l");
    leg_b->AddEntry(h_dr_b, "DR Fit", "l");
    leg_b->Draw();
    
    pad_b_bottom->cd();
    TH1D* ratio_b_dca = (TH1D*)h_dca_b->Clone("ratio_b_dca");
    TH1D* ratio_b_mass = (TH1D*)h_mass_b->Clone("ratio_b_mass");
    TH1D* ratio_b_dr = (TH1D*)h_dr_b->Clone("ratio_b_dr");
    ratio_b_dca->Divide(h_true_b);
    ratio_b_mass->Divide(h_true_b);
    ratio_b_dr->Divide(h_true_b);
    ratio_b_dca->SetTitle(";Jet p_{T} [GeV];Fit / Truth");
    ratio_b_dca->GetYaxis()->SetRangeUser(0.7, 1.3);
    ratio_b_dca->GetYaxis()->SetNdivisions(505);
    ratio_b_dca->GetYaxis()->SetTitleSize(0.1);
    ratio_b_dca->GetYaxis()->SetLabelSize(0.1);
    ratio_b_dca->GetXaxis()->SetTitleSize(0.1);
    ratio_b_dca->GetXaxis()->SetLabelSize(0.1);
    ratio_b_dca->GetYaxis()->SetTitleOffset(0.5);
    ratio_b_dca->Draw("LP");
    ratio_b_mass->Draw("LP SAME");
    ratio_b_dr->Draw("LP SAME");
    TLine* line_b = new TLine(ratio_b_dca->GetXaxis()->GetXmin(), 1, ratio_b_dca->GetXaxis()->GetXmax(), 1);
    line_b->SetLineStyle(2);
    line_b->Draw();
    c_b->SaveAs("validate_b_yields2.pdf");

    // ========== BB PLOT ==========
    TCanvas* c_bb = new TCanvas("c_bb", "", 800, 800);
    TPad* pad_bb_top = new TPad("pad_bb_top", "", 0, 0.3, 1, 1);
    TPad* pad_bb_bottom = new TPad("pad_bb_bottom", "", 0, 0, 1, 0.3);
    pad_bb_top->SetBottomMargin(0.02);
    pad_bb_bottom->SetTopMargin(0.02);
    pad_bb_bottom->SetBottomMargin(0.3);
    pad_bb_top->Draw();
    pad_bb_bottom->Draw();
    
    pad_bb_top->cd();
    h_true_bb->SetLineColor(kBlack);
    h_true_bb->SetLineWidth(2);
    h_true_bb->SetTitle("Heavy Flavor bb Yield Comparison;Jet p_{T} [GeV];dN/dp_{T}");
    h_true_bb->GetXaxis()->SetLabelSize(0);
    h_true_bb->Draw("LPE");
    h_dca_bb->SetLineColor(kRed);
    h_dca_bb->SetLineWidth(2);
    h_dca_bb->Draw("LPE SAME");
    h_mass_bb->SetLineColor(kBlue);
    h_mass_bb->SetLineWidth(2);
    h_mass_bb->Draw("LPE SAME");
    h_dr_bb->SetLineColor(kGreen+2);
    h_dr_bb->SetLineWidth(2);
    h_dr_bb->Draw("LPE SAME");
    TLegend* leg_bb = new TLegend(0.45, 0.65, 0.70, 0.88);
    leg_bb->AddEntry(h_true_bb, "True bb", "l");
    leg_bb->AddEntry(h_dca_bb, "DCA Fit", "l");
    leg_bb->AddEntry(h_mass_bb, "Mass Fit", "l");
    leg_bb->AddEntry(h_dr_bb, "DR Fit", "l");
    leg_bb->Draw();
    
    pad_bb_bottom->cd();
    TH1D* ratio_bb_dca = (TH1D*)h_dca_bb->Clone("ratio_bb_dca");
    TH1D* ratio_bb_mass = (TH1D*)h_mass_bb->Clone("ratio_bb_mass");
    TH1D* ratio_bb_dr = (TH1D*)h_dr_bb->Clone("ratio_bb_dr");
    ratio_bb_dca->Divide(h_true_bb);
    ratio_bb_mass->Divide(h_true_bb);
    ratio_bb_dr->Divide(h_true_bb);
    ratio_bb_dca->SetTitle(";Jet p_{T} [GeV];Fit / Truth");
    ratio_bb_dca->GetYaxis()->SetRangeUser(0.7, 1.3);
    ratio_bb_dca->GetYaxis()->SetNdivisions(505);
    ratio_bb_dca->GetYaxis()->SetTitleSize(0.1);
    ratio_bb_dca->GetYaxis()->SetLabelSize(0.1);
    ratio_bb_dca->GetXaxis()->SetTitleSize(0.1);
    ratio_bb_dca->GetXaxis()->SetLabelSize(0.1);
    ratio_bb_dca->GetYaxis()->SetTitleOffset(0.5);
    ratio_bb_dca->Draw("LP");
    ratio_bb_mass->Draw("LP SAME");
    ratio_bb_dr->Draw("LP SAME");
    TLine* line_bb = new TLine(ratio_bb_dca->GetXaxis()->GetXmin(), 1, ratio_bb_dca->GetXaxis()->GetXmax(), 1);
    line_bb->SetLineStyle(2);
    line_bb->Draw();
    c_bb->SaveAs("validate_bb_yields2.pdf");

}