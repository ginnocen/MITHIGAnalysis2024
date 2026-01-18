

void validate_fitting3(){

    TFile*f = TFile::Open("../../alternate_yields/testyields.root");
    TFile* ftruth = TFile::Open("../../mcdistros.root");

    // fit results - get individual components
    TH1D* h_uds = (TH1D*)f->Get("Yields_uds");
    TH1D* h_other = (TH1D*)f->Get("Yields_other");
    TH1D* h_c = (TH1D*)f->Get("Yields_c");
    TH1D* h_cc = (TH1D*)f->Get("Yields_cc");
    TH1D* h_b = (TH1D*)f->Get("Yields_b");
    TH1D* h_bb = (TH1D*)f->Get("Yields_bb");
    
    // truth tntuples
    TNtuple* nt_uds = (TNtuple*)ftruth->Get("nt_uds");
    TNtuple* nt_other = (TNtuple*)ftruth->Get("nt_other");
    TNtuple* nt_c = (TNtuple*)ftruth->Get("nt_c");
    TNtuple* nt_cc = (TNtuple*)ftruth->Get("nt_cc");
    TNtuple* nt_b = (TNtuple*)ftruth->Get("nt_b");
    TNtuple* nt_bb = (TNtuple*)ftruth->Get("nt_bb");

    TH1D* h_true_uds = new TH1D("h_true_uds", "True uds Distribution", h_uds->GetNbinsX(), h_uds->GetXaxis()->GetXbins()->GetArray());
    TH1D* h_true_other = new TH1D("h_true_other", "True other Distribution", h_other->GetNbinsX(), h_other->GetXaxis()->GetXbins()->GetArray());
    TH1D* h_true_c = new TH1D("h_true_c", "True c Distribution", h_c->GetNbinsX(), h_c->GetXaxis()->GetXbins()->GetArray());
    TH1D* h_true_cc = new TH1D("h_true_cc", "True cc Distribution", h_cc->GetNbinsX(), h_cc->GetXaxis()->GetXbins()->GetArray());
    TH1D* h_true_b = new TH1D("h_true_b", "True b Distribution", h_b->GetNbinsX(), h_b->GetXaxis()->GetXbins()->GetArray());
    TH1D* h_true_bb = new TH1D("h_true_bb", "True bb Distribution", h_bb->GetNbinsX(), h_bb->GetXaxis()->GetXbins()->GetArray());



    for(int iBin = 1; iBin <= h_uds->GetNbinsX(); iBin++) {
        float ptMin = h_uds->GetXaxis()->GetBinLowEdge(iBin);
        float ptMax = h_uds->GetXaxis()->GetBinUpEdge(iBin);

        float n_uds = nt_uds->GetEntries(Form("JetPT < %f && JetPT >= %f", ptMax, ptMin));
        float n_other = nt_other->GetEntries(Form("JetPT < %f && JetPT >= %f", ptMax, ptMin));
        float n_c = nt_c->GetEntries(Form("JetPT < %f && JetPT >= %f", ptMax, ptMin));
        float n_cc = nt_cc->GetEntries(Form("JetPT < %f && JetPT >= %f", ptMax, ptMin));
        float n_b = nt_b->GetEntries(Form("JetPT < %f && JetPT >= %f", ptMax, ptMin));
        float n_bb = nt_bb->GetEntries(Form("JetPT < %f && JetPT >= %f", ptMax, ptMin));

        float binWidth = h_uds->GetXaxis()->GetBinWidth(iBin);
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
    h_uds->SetLineColor(kRed);
    h_uds->SetLineWidth(2);
    h_uds->Draw("LPE SAME");
    TLegend* leg_uds = new TLegend(0.45, 0.65, 0.70, 0.88);
    leg_uds->AddEntry(h_true_uds, "True uds", "l");
    leg_uds->AddEntry(h_uds, "Fitted uds", "l");
    leg_uds->Draw();
    
    pad_uds_bottom->cd();
    TH1D* ratio_uds = (TH1D*)h_uds->Clone("ratio_uds");
    ratio_uds->Divide(h_true_uds);
    ratio_uds->SetTitle(";Jet p_{T} [GeV];Fit / Truth");
    ratio_uds->GetYaxis()->SetRangeUser(0.9, 1.1);
    ratio_uds->GetYaxis()->SetNdivisions(505);
    ratio_uds->GetYaxis()->SetTitleSize(0.1);
    ratio_uds->GetYaxis()->SetLabelSize(0.1);
    ratio_uds->GetXaxis()->SetTitleSize(0.1);
    ratio_uds->GetXaxis()->SetLabelSize(0.1);
    ratio_uds->GetYaxis()->SetTitleOffset(0.5);
    ratio_uds->Draw("LP");
    TLine* line_uds = new TLine(ratio_uds->GetXaxis()->GetXmin(), 1, ratio_uds->GetXaxis()->GetXmax(), 1);
    line_uds->SetLineStyle(2);
    line_uds->Draw();
    c_uds->SaveAs("validate_uds_yields3.pdf");

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
    h_other->SetLineColor(kRed);
    h_other->SetLineWidth(2);
    h_other->Draw("LPE SAME");
    TLegend* leg_other = new TLegend(0.45, 0.65, 0.70, 0.88);
    leg_other->AddEntry(h_true_other, "True Other", "l");
    leg_other->AddEntry(h_other, "Fit", "l");
    leg_other->Draw();
    
    pad_other_bottom->cd();
    TH1D* ratio_other = (TH1D*)h_other->Clone("ratio_other");
    ratio_other->Divide(h_true_other);
    ratio_other->SetTitle(";Jet p_{T} [GeV];Fit / Truth");
    ratio_other->GetYaxis()->SetRangeUser(0.9, 1.1);
    ratio_other->GetYaxis()->SetNdivisions(505);
    ratio_other->GetYaxis()->SetTitleSize(0.1);
    ratio_other->GetYaxis()->SetLabelSize(0.1);
    ratio_other->GetXaxis()->SetTitleSize(0.1);
    ratio_other->GetXaxis()->SetLabelSize(0.1);
    ratio_other->GetYaxis()->SetTitleOffset(0.5);
    ratio_other->Draw("LP");
    TLine* line_other = new TLine(ratio_other->GetXaxis()->GetXmin(), 1, ratio_other->GetXaxis()->GetXmax(), 1);
    line_other->SetLineStyle(2);
    line_other->Draw();
    c_other->SaveAs("validate_other_yields3.pdf");

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
    h_c->SetLineColor(kRed);
    h_c->SetLineWidth(2);
    h_c->Draw("LPE SAME");
    TLegend* leg_c = new TLegend(0.45, 0.65, 0.70, 0.88);
    leg_c->AddEntry(h_true_c, "True c", "l");
    leg_c->AddEntry(h_c, "Fit", "l");
    leg_c->Draw();
    
    pad_c_bottom->cd();
    TH1D* ratio_c = (TH1D*)h_c->Clone("ratio_c");
    ratio_c->Divide(h_true_c);
    ratio_c->SetTitle(";Jet p_{T} [GeV];Fit / Truth");
    ratio_c->GetYaxis()->SetRangeUser(0.9, 1.1);
    ratio_c->GetYaxis()->SetNdivisions(505);
    ratio_c->GetYaxis()->SetTitleSize(0.1);
    ratio_c->GetYaxis()->SetLabelSize(0.1);
    ratio_c->GetXaxis()->SetTitleSize(0.1);
    ratio_c->GetXaxis()->SetLabelSize(0.1);
    ratio_c->GetYaxis()->SetTitleOffset(0.5);
    ratio_c->Draw("LP");
    TLine* line_c = new TLine(ratio_c->GetXaxis()->GetXmin(), 1, ratio_c->GetXaxis()->GetXmax(), 1);
    line_c->SetLineStyle(2);
    line_c->Draw();
    c_c->SaveAs("validate_c_yields3.pdf");

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
    h_cc->SetLineColor(kRed);
    h_cc->SetLineWidth(2);
    h_cc->Draw("LPE SAME");
    TLegend* leg_cc = new TLegend(0.45, 0.65, 0.70, 0.88);
    leg_cc->AddEntry(h_true_cc, "True cc", "l");
    leg_cc->AddEntry(h_cc, "Fit", "l");
    leg_cc->Draw();
    
    pad_cc_bottom->cd();
    TH1D* ratio_cc = (TH1D*)h_cc->Clone("ratio_cc");
    ratio_cc->Divide(h_true_cc);
    ratio_cc->SetTitle(";Jet p_{T} [GeV];Fit / Truth");
    ratio_cc->GetYaxis()->SetRangeUser(0.9, 1.1);
    ratio_cc->GetYaxis()->SetNdivisions(505);
    ratio_cc->GetYaxis()->SetTitleSize(0.1);
    ratio_cc->GetYaxis()->SetLabelSize(0.1);
    ratio_cc->GetXaxis()->SetTitleSize(0.1);
    ratio_cc->GetXaxis()->SetLabelSize(0.1);
    ratio_cc->GetYaxis()->SetTitleOffset(0.5);
    ratio_cc->Draw("LP");
    TLine* line_cc = new TLine(ratio_cc->GetXaxis()->GetXmin(), 1, ratio_cc->GetXaxis()->GetXmax(), 1);
    line_cc->SetLineStyle(2);
    line_cc->Draw();
    c_cc->SaveAs("validate_cc_yields3.pdf");

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
    h_b->SetLineColor(kRed);
    h_b->SetLineWidth(2);
    h_b->Draw("LPE SAME");
    TLegend* leg_b = new TLegend(0.45, 0.65, 0.70, 0.88);
    leg_b->AddEntry(h_true_b, "True b", "l");
    leg_b->AddEntry(h_b, "Fit", "l");
    leg_b->Draw();
    
    pad_b_bottom->cd();
    TH1D* ratio_b = (TH1D*)h_b->Clone("ratio_b");
    ratio_b->Divide(h_true_b);
    ratio_b->SetTitle(";Jet p_{T} [GeV];Fit / Truth");
    ratio_b->GetYaxis()->SetRangeUser(0.9, 1.1);
    ratio_b->GetYaxis()->SetNdivisions(505);
    ratio_b->GetYaxis()->SetTitleSize(0.1);
    ratio_b->GetYaxis()->SetLabelSize(0.1);
    ratio_b->GetXaxis()->SetTitleSize(0.1);
    ratio_b->GetXaxis()->SetLabelSize(0.1);
    ratio_b->GetYaxis()->SetTitleOffset(0.5);
    ratio_b->Draw("LP");
    TLine* line_b = new TLine(ratio_b->GetXaxis()->GetXmin(), 1, ratio_b->GetXaxis()->GetXmax(), 1);
    line_b->SetLineStyle(2);
    line_b->Draw();
    c_b->SaveAs("validate_b_yields3.pdf");

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
    h_bb->SetLineColor(kRed);
    h_bb->SetLineWidth(2);
    h_bb->Draw("LPE SAME");
    TLegend* leg_bb = new TLegend(0.45, 0.65, 0.70, 0.88);
    leg_bb->AddEntry(h_true_bb, "True bb", "l");
    leg_bb->AddEntry(h_bb, "Fit", "l");
    leg_bb->Draw();
    
    pad_bb_bottom->cd();
    TH1D* ratio_bb = (TH1D*)h_bb->Clone("ratio_bb");
    ratio_bb->Divide(h_true_bb);
    ratio_bb->SetTitle(";Jet p_{T} [GeV];Fit / Truth");
    ratio_bb->GetYaxis()->SetRangeUser(0.9, 1.1);
    ratio_bb->GetYaxis()->SetNdivisions(505);
    ratio_bb->GetYaxis()->SetTitleSize(0.1);
    ratio_bb->GetYaxis()->SetLabelSize(0.1);
    ratio_bb->GetXaxis()->SetTitleSize(0.1);
    ratio_bb->GetXaxis()->SetLabelSize(0.1);
    ratio_bb->GetYaxis()->SetTitleOffset(0.5);
    ratio_bb->Draw("LP");
    TLine* line_bb = new TLine(ratio_bb->GetXaxis()->GetXmin(), 1, ratio_bb->GetXaxis()->GetXmax(), 1);
    line_bb->SetLineStyle(2);
    line_bb->Draw();
    c_bb->SaveAs("validate_bb_yields3.pdf");

}