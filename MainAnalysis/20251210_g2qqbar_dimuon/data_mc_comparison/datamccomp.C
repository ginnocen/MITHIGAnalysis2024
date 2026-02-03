
void datamc(
    TTree* T_hi,
    TTree* T_lo,
    TTree* T_mc,
    int trigger,
    const char* label,
    const char* varname,
    const char* xlabel,
    int xbins,
    float xmin,
    float xmax,
    int chargesel,
    float mupt_cut1,
    float mupt_cut2,
    float zlo,
    float zhi,
    float dcaxy,
    float jetpt_min,
    float jetpt_max,
    bool logy,
    bool doNoweights,
    bool cutoutJpsi,
    const char* pdfname

){


    // project histograms
    TH1D* h_40 = new TH1D("h_40", Form("LowEG_40 %s Spectrum; %s ;Entries", xlabel, xlabel), xbins, xmin, xmax);
    TH1D* h_80 = new TH1D("h_80", Form("HighEG %s Spectrum; %s ;Entries", xlabel, xlabel), xbins, xmin, xmax);
    TH1D* h_60 = new TH1D("h_60", Form("LowEG_60 %s Spectrum; %s ;Entries", xlabel, xlabel), xbins, xmin, xmax);
    TH1D* h_mc = new TH1D("h_mc", Form("MC %s Spectrum; %s ;Entries", xlabel, xlabel), xbins, xmin, xmax);
    TH1D* h_mc_noweights = new TH1D("h_mc_noweights", Form("MC no weights %s Spectrum; %s ;Entries", xlabel, xlabel), xbins, xmin, xmax);


    if(cutoutJpsi){
        cout << "AHJH" << endl;
        T_hi->Project("h_80", varname, Form("IsMuMuTagged && HLT_HIAK4PFJet80_v1 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0) && (mumuMass < 2.9 || mumuMass > 3.3) && ((mumuPt / JetPT) < %.1f && (mumuPt / JetPT) > %.1f) && log10(abs(muDiDxy1Dxy2/muDiDxy1Dxy2Err)) > %.1f", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, zhi, zlo, dcaxy));
        T_lo->Project("h_60", varname, Form("IsMuMuTagged && HLT_HIAK4PFJet60_v1 && !HLT_HIAK4PFJet80_v1 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0) && (mumuMass < 2.9 || mumuMass > 3.3) && ((mumuPt / JetPT) < %.1f && (mumuPt / JetPT) > %.1f) && log10(abs(muDiDxy1Dxy2/muDiDxy1Dxy2Err)) > %.1f", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, zhi, zlo, dcaxy));
        T_lo->Project("h_40", varname, Form("IsMuMuTagged && HLT_HIAK4PFJet40_v1 && !HLT_HIAK4PFJet60_v1 && !HLT_HIAK4PFJet80_v1 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0) && (mumuMass < 2.9 || mumuMass > 3.3) && ((mumuPt / JetPT) < %.1f && (mumuPt / JetPT) > %.1f) && log10(abs(muDiDxy1Dxy2/muDiDxy1Dxy2Err)) > %.1f", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, zhi, zlo, dcaxy));
        T_mc->Project("h_mc", varname, Form("EventWeight * (IsMuMuTagged && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0) && (mumuMass < 2.9 || mumuMass > 3.3) && ((mumuPt / JetPT) < %.1f && (mumuPt / JetPT) > %.1f) && log10(abs(muDiDxy1Dxy2/muDiDxy1Dxy2Err)) > %.1f)", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, zhi, zlo, dcaxy));
        T_mc->Project("h_mc_noweights", varname, Form("IsMuMuTagged && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0) && (mumuMass < 2.9 || mumuMass > 3.3) && ((mumuPt / JetPT) < %.1f && (mumuPt / JetPT) > %.1f) && log10(abs(muDiDxy1Dxy2/muDiDxy1Dxy2Err)) > %.1f", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, zhi, zlo, dcaxy));
    }
    else{
        T_hi->Project("h_80", varname, Form("IsMuMuTagged && HLT_HIAK4PFJet80_v1 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0) && ((mumuPt / JetPT) < %.1f && (mumuPt / JetPT) > %.1f) && log10(abs(muDiDxy1Dxy2/muDiDxy1Dxy2Err)) > %.1f", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, zhi, zlo, dcaxy));
        T_lo->Project("h_60", varname, Form("IsMuMuTagged && HLT_HIAK4PFJet60_v1 && !HLT_HIAK4PFJet80_v1 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0) && ((mumuPt / JetPT) < %.1f && (mumuPt / JetPT) > %.1f) && log10(abs(muDiDxy1Dxy2/muDiDxy1Dxy2Err)) > %.1f", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, zhi, zlo, dcaxy));
        T_lo->Project("h_40", varname, Form("IsMuMuTagged && HLT_HIAK4PFJet40_v1 && !HLT_HIAK4PFJet60_v1 && !HLT_HIAK4PFJet80_v1 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0) && ((mumuPt / JetPT) < %.1f && (mumuPt / JetPT) > %.1f) && log10(abs(muDiDxy1Dxy2/muDiDxy1Dxy2Err)) > %.1f", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, zhi, zlo, dcaxy));
        T_mc->Project("h_mc", varname, Form("EventWeight * (IsMuMuTagged && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0) && ((mumuPt / JetPT) < %.1f && (mumuPt / JetPT) > %.1f) && log10(abs(muDiDxy1Dxy2/muDiDxy1Dxy2Err)) > %.1f)", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, zhi, zlo, dcaxy));
        T_mc->Project("h_mc_noweights", varname, Form("IsMuMuTagged && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0) && ((mumuPt / JetPT) < %.1f && (mumuPt / JetPT) > %.1f) && log10(abs(muDiDxy1Dxy2/muDiDxy1Dxy2Err)) > %.1f", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, zhi, zlo, dcaxy));
    }
    cout << "Done projecting histograms" << endl;

    TH1D* h_data = (TH1D*)h_80->Clone("h_data");
    

    if(trigger == 40){
        h_data->Add(h_40, 33.910);
        h_data->Add(h_60, 1);
    }
    else if(trigger == 60){
        h_data->Add(h_60, 6.338);
    }

    // plot data vs mc comparison
    TCanvas* c = new TCanvas("", "", 800, 800);
    
    // Create two pads - upper for main plot, lower for ratio
    TPad* pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0.02);
    pad1->Draw();
    pad1->cd();
    if(logy){ pad1->SetLogy(); }

    h_data->Scale(1.0/h_data->Integral());
    h_mc->Scale(1.0/h_mc->Integral());
    if(doNoweights){
        h_mc_noweights->Scale(1.0/h_mc_noweights->Integral());
    }
    
    double maxval = TMath::Max(h_data->GetMaximum(), h_mc->GetMaximum());
    if(doNoweights){
        maxval = TMath::Max(maxval, h_mc_noweights->GetMaximum());
    }
    h_data->SetMaximum(1.5 * maxval);
    h_mc->SetMaximum(1.5 * maxval);
    h_mc_noweights->SetMaximum(1.5 * maxval);
    if(!logy){
        h_data->SetMinimum(0);
        h_mc->SetMinimum(0);
    }
    h_mc->SetLineColor(kRed);
    h_mc->SetLineWidth(2);
    h_mc_noweights->SetLineColor(kBlue);
    h_mc_noweights->SetLineWidth(2);
    h_data->SetLineColor(kBlack);
    h_data->SetLineWidth(2);
    
    // Remove x-axis labels from upper pad
    h_mc->GetXaxis()->SetLabelSize(0);
    h_mc->GetXaxis()->SetTitleSize(0);
    h_data->GetXaxis()->SetLabelSize(0);
    h_data->GetXaxis()->SetTitleSize(0);
    h_mc_noweights->GetXaxis()->SetLabelSize(0);
    h_mc_noweights->GetXaxis()->SetTitleSize(0);

    TLegend* leg = new TLegend(0.55,0.76,0.88,0.89);
    leg->AddEntry(h_data, "Data", "l");
    leg->AddEntry(h_mc, "MC with pThat weight", "l");
    if(doNoweights){
        leg->AddEntry(h_mc_noweights, "MC without pThat weight", "l");
    }
    if(doNoweights){
        h_mc_noweights->Draw("HIST E SAME");
    }
    h_mc->Draw("HIST E SAME");
    h_data->Draw("HIST E SAME");  
    leg->Draw();

    TLatex* text = new TLatex();
    text->SetNDC();
    text->SetTextSize(0.05);
    text->SetTextFont(42);
    text->DrawLatex(0.52, 0.72, Form("Triggered dataset: %d", trigger));
    text->DrawLatex(0.52, 0.65, Form("Mu1 p_{T} > %.1f, Mu2 p_{T} > %.1f", mupt_cut1, mupt_cut2));
    text->DrawLatex(0.52, 0.58, Form("Jet p_{T} #in [%.0f, %.0f] GeV", jetpt_min, jetpt_max));
    text->DrawLatex(0.52, 0.51, chargesel == 0 ? "Inclusive Charge" : (chargesel == -1 ? "Opposite-sign" : "Same-sign"));
    //text->DrawLatex(0.55, 0.44, Form("Z cut: %.2f < Z < %.2f", zlo, zhi));
    if(cutoutJpsi){
        text->DrawLatex(0.52, 0.44, "J/#psi mass cut applied");
    }

    // Create ratio panel
    c->cd();
    TPad* pad2 = new TPad("pad2", "pad2", 0, 0.0, 1, 0.3);
    pad2->SetTopMargin(0.02);
    pad2->SetBottomMargin(0.3);
    pad2->Draw();
    pad2->cd();
    
    // Create ratio histogram
    TH1D* h_ratio = (TH1D*)h_mc->Clone("h_ratio");
    h_ratio->Divide(h_data);
    h_ratio->SetTitle("");
    h_ratio->SetLineColor(kBlack);
    h_ratio->SetLineWidth(2);
    h_ratio->SetMarkerStyle(20);
    h_ratio->SetMarkerSize(0.8);
    
    // Set y-axis for ratio
    h_ratio->GetYaxis()->SetTitle("MC / Data");
    h_ratio->GetYaxis()->SetTitleSize(0.12);
    h_ratio->GetYaxis()->SetTitleOffset(0.4);
    h_ratio->GetYaxis()->SetLabelSize(0.10);
    h_ratio->GetYaxis()->SetNdivisions(505);
    h_ratio->SetMinimum(0.5);
    h_ratio->SetMaximum(1.5);
    
    // Set x-axis for ratio
    h_ratio->GetXaxis()->SetTitle(xlabel);
    h_ratio->GetXaxis()->SetTitleSize(0.12);
    h_ratio->GetXaxis()->SetTitleOffset(1.0);
    h_ratio->GetXaxis()->SetLabelSize(0.10);
    
    h_ratio->Draw("EP");
    
    // Draw horizontal line at 1
    TLine* line = new TLine(xmin, 1.0, xmax, 1.0);
    line->SetLineStyle(2);
    line->SetLineColor(kRed);
    line->Draw("SAME");

    c->cd();
    c->SaveAs(pdfname);

    delete c;
    delete h_40;
    delete h_80;
    delete h_60;
    delete h_mc;
    delete h_mc_noweights;
    delete h_data;

}

void templates(
    TTree* T_hi,
    TTree* T_lo,
    TTree* T_mc,
    int trigger,
    const char* label,
    const char* varname,
    const char* xlabel,
    int xbins,
    float xmin,
    float xmax,
    int chargesel,
    float mupt_cut1,
    float mupt_cut2,
    float zlo,
    float zhi,
    float dcaxy,
    float jetpt_min,
    float jetpt_max,
    bool logy,
    bool doNoweights,
    bool cutoutJpsi,
    const char* pdfname
){

    TH1D* h_mc = new TH1D("h_mc_2", Form("MC %s Spectrum; %s ;Entries", xlabel, xlabel), xbins, xmin, xmax);
    TH1D* h_other = new TH1D("h_other", Form("MC %s Spectrum (Other); %s ;Entries", xlabel, xlabel), xbins, xmin, xmax);
    TH1D* h_udsg = new TH1D("h_udsg", Form("MC %s Spectrum (UDSG); %s ;Entries", xlabel, xlabel), xbins, xmin, xmax);
    TH1D* h_c = new TH1D("h_c", Form("MC %s Spectrum (C); %s ;Entries", xlabel, xlabel), xbins, xmin, xmax);
    TH1D* h_b = new TH1D("h_b", Form("MC %s Spectrum (B); %s ;Entries", xlabel, xlabel), xbins, xmin, xmax);
    TH1D* h_cc = new TH1D("h_cc", Form("MC %s Spectrum (CC); %s ;Entries", xlabel, xlabel), xbins, xmin, xmax);
    TH1D* h_bb = new TH1D("h_bb", Form("MC %s Spectrum (BB); %s ;Entries", xlabel, xlabel), xbins, xmin, xmax);

    if(cutoutJpsi){
        if(doNoweights){
        T_mc->Project("h_mc_2", varname, Form("(IsMuMuTagged && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0) && (mumuMass > 3.3 || mumuMass < 2.9) && ((mumuPt / JetPT) < %.1f && (mumuPt / JetPT) > %.1f) && log10(abs(muDiDxy1Dxy2/muDiDxy1Dxy2Err)) > %.1f)", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, zhi, zlo, dcaxy));   
        T_mc->Project("h_other", varname, Form("(IsMuMuTagged && (NbHad > 2 || (NbHad == 0 && NcHad > 2)) && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0) && (mumuMass > 3.3 || mumuMass < 2.9) && ((mumuPt / JetPT) < %.1f && (mumuPt / JetPT) > %.1f) && log10(abs(muDiDxy1Dxy2/muDiDxy1Dxy2Err)) > %.1f)", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, zhi, zlo, dcaxy));
        T_mc->Project("h_udsg", varname, Form("(IsMuMuTagged && NbHad == 0 && NcHad == 0 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0) && (mumuMass > 3.3 || mumuMass < 2.9) && ((mumuPt / JetPT) < %.1f && (mumuPt / JetPT) > %.1f) && log10(abs(muDiDxy1Dxy2/muDiDxy1Dxy2Err)) > %.1f)", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, zhi, zlo, dcaxy));
        T_mc->Project("h_c", varname, Form("(IsMuMuTagged && NbHad == 0 && NcHad == 1 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0) && (mumuMass > 3.3 || mumuMass < 2.9) && ((mumuPt / JetPT) < %.1f && (mumuPt / JetPT) > %.1f) && log10(abs(muDiDxy1Dxy2/muDiDxy1Dxy2Err)) > %.1f)", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, zhi, zlo, dcaxy));
        T_mc->Project("h_b", varname, Form("(IsMuMuTagged && NbHad == 1 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0) && (mumuMass > 3.3 || mumuMass < 2.9) && ((mumuPt / JetPT) < %.1f && (mumuPt / JetPT) > %.1f) && log10(abs(muDiDxy1Dxy2/muDiDxy1Dxy2Err)) > %.1f)", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, zhi, zlo, dcaxy));
        T_mc->Project("h_cc", varname, Form("(IsMuMuTagged && NbHad == 0 && NcHad == 2 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0) && (mumuMass > 3.3 || mumuMass < 2.9) && ((mumuPt / JetPT) < %.1f && (mumuPt / JetPT) > %.1f) && log10(abs(muDiDxy1Dxy2/muDiDxy1Dxy2Err)) > %.1f)", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, zhi, zlo, dcaxy));
        T_mc->Project("h_bb", varname, Form("(IsMuMuTagged && NbHad == 2 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0) && (mumuMass > 3.3 || mumuMass < 2.9) && ((mumuPt / JetPT) < %.1f && (mumuPt / JetPT) > %.1f) && log10(abs(muDiDxy1Dxy2/muDiDxy1Dxy2Err)) > %.1f)", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, zhi, zlo, dcaxy));
    }
    else{   
        T_mc->Project("h_mc_2", varname, Form("EventWeight * (IsMuMuTagged && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0) && (mumuMass > 3.3 || mumuMass < 2.9) && ((mumuPt / JetPT) < %.1f && (mumuPt / JetPT) > %.1f)) && log10(abs(muDiDxy1Dxy2 / muDiDxy1Dxy2Err)) > %.1f", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, zhi, zlo, dcaxy));   
        T_mc->Project("h_other", varname, Form("EventWeight * (IsMuMuTagged && (NbHad > 2 || (NbHad == 0 && NcHad > 2)) && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0) && (mumuMass > 3.3 || mumuMass < 2.9) && ((mumuPt / JetPT) < %.1f && (mumuPt / JetPT) > %.1f) && log10(abs(muDiDxy1Dxy2 / muDiDxy1Dxy2Err)) > %.1f)", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, zhi, zlo, dcaxy));
        T_mc->Project("h_udsg", varname, Form("EventWeight * (IsMuMuTagged && NbHad == 0 && NcHad == 0 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0) && (mumuMass > 3.3 || mumuMass < 2.9) && ((mumuPt / JetPT) < %.1f && (mumuPt / JetPT) > %.1f)) && log10(abs(muDiDxy1Dxy2 / muDiDxy1Dxy2Err)) > %.1f", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, zhi, zlo, dcaxy));
        T_mc->Project("h_c", varname, Form("EventWeight * (IsMuMuTagged && NbHad == 0 && NcHad == 1 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0) && (mumuMass > 3.3 || mumuMass < 2.9) && ((mumuPt / JetPT) < %.1f && (mumuPt / JetPT) > %.1f)) && log10(abs(muDiDxy1Dxy2 / muDiDxy1Dxy2Err)) > %.1f", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, zhi, zlo, dcaxy));
        T_mc->Project("h_b", varname, Form("EventWeight * (IsMuMuTagged && NbHad == 1 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0) && (mumuMass > 3.3 || mumuMass < 2.9) && ((mumuPt / JetPT) < %.1f && (mumuPt / JetPT) > %.1f)) && log10(abs(muDiDxy1Dxy2 / muDiDxy1Dxy2Err)) > %.1f", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, zhi, zlo, dcaxy));
        T_mc->Project("h_cc", varname, Form("EventWeight * (IsMuMuTagged && NbHad == 0 && NcHad == 2 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0) && (mumuMass > 3.3 || mumuMass < 2.9) && ((mumuPt / JetPT) < %.1f && (mumuPt / JetPT) > %.1f)) && log10(abs(muDiDxy1Dxy2 / muDiDxy1Dxy2Err)) > %.1f", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, zhi, zlo, dcaxy));
        T_mc->Project("h_bb", varname, Form("EventWeight * (IsMuMuTagged && NbHad == 2 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0) && (mumuMass > 3.3 || mumuMass < 2.9) && ((mumuPt / JetPT) < %.1f && (mumuPt / JetPT) > %.1f) && log10(abs(muDiDxy1Dxy2 / muDiDxy1Dxy2Err)) > %.1f)", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, zhi, zlo, dcaxy));
    }
    }
    else{
        if(doNoweights){
            T_mc->Project("h_mc_2", varname, Form("(IsMuMuTagged && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0)) && log10(abs(muDiDxy1Dxy2/muDiDxy1Dxy2Err)) > %.1f", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, dcaxy));   
            T_mc->Project("h_other", varname, Form("(IsMuMuTagged && (NbHad > 2 || (NbHad == 0 && NcHad > 2)) && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0) && log10(abs(muDiDxy1Dxy2 / muDiDxy1Dxy2Err)) > %.1f)", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, dcaxy));
            T_mc->Project("h_udsg", varname, Form("(IsMuMuTagged && NbHad == 0 && NcHad == 0 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0) && log10(abs(muDiDxy1Dxy2 / muDiDxy1Dxy2Err)) > %.1f)", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, dcaxy));
            T_mc->Project("h_c", varname, Form("(IsMuMuTagged && NbHad == 0 && NcHad == 1 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0))", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel));
            T_mc->Project("h_b", varname, Form("(IsMuMuTagged && NbHad == 1 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0))", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel));
            T_mc->Project("h_cc", varname, Form("(IsMuMuTagged && NbHad == 0 && NcHad == 2 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0))", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel));
            T_mc->Project("h_bb", varname, Form("(IsMuMuTagged && NbHad == 2 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0))", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel));
        }
        else{   
            T_mc->Project("h_mc_2", varname, Form("EventWeight * (IsMuMuTagged && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0) && log10(abs(muDiDxy1Dxy2 / muDiDxy1Dxy2Err)) > %.1f)", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, dcaxy));   
            T_mc->Project("h_other", varname, Form("EventWeight * (IsMuMuTagged && (NbHad > 2 || (NbHad == 0 && NcHad > 2)) && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0) && log10(abs(muDiDxy1Dxy2 / muDiDxy1Dxy2Err)) > %.1f)", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, dcaxy));
            T_mc->Project("h_udsg", varname, Form("EventWeight * (IsMuMuTagged && NbHad == 0 && NcHad == 0 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0) && log10(abs(muDiDxy1Dxy2 / muDiDxy1Dxy2Err)) > %.1f)", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, dcaxy));
            T_mc->Project("h_c", varname, Form("EventWeight * (IsMuMuTagged && NbHad == 0 && NcHad == 1 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0) && log10(abs(muDiDxy1Dxy2 / muDiDxy1Dxy2Err)) > %.1f)", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, dcaxy));
            T_mc->Project("h_b", varname, Form("EventWeight * (IsMuMuTagged && NbHad == 1 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0) && log10(abs(muDiDxy1Dxy2 / muDiDxy1Dxy2Err)) > %.1f)", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, dcaxy));
            T_mc->Project("h_cc", varname, Form("EventWeight * (IsMuMuTagged && NbHad == 0 && NcHad == 2 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0) && log10(abs(muDiDxy1Dxy2 / muDiDxy1Dxy2Err)) > %.1f)", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, dcaxy));
            T_mc->Project("h_bb", varname, Form("EventWeight * (IsMuMuTagged && NbHad == 2 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0) && log10(abs(muDiDxy1Dxy2 / muDiDxy1Dxy2Err)) > %.1f)", mupt_cut1, mupt_cut2, jetpt_min, jetpt_max, chargesel, chargesel, dcaxy));
        }
    }
    
    cout << "Done projecting histograms" << endl;

    TCanvas* c = new TCanvas("", "", 800, 600);
    if(logy){ c->SetLogy(); }
    h_mc->SetLineColor(kBlack);
    h_mc->SetLineWidth(2);
    h_other->SetLineColor(kGray+1);
    h_other->SetLineWidth(2);
    h_udsg->SetLineColor(kBlue);
    h_udsg->SetLineWidth(2);
    h_c->SetLineColor(kCyan);
    h_c->SetLineWidth(2);
    h_b->SetLineColor(kGreen+2);
    h_b->SetLineWidth(2);
    h_cc->SetLineColor(kRed);
    h_cc->SetLineWidth(2);
    h_bb->SetLineColor(kMagenta);
    h_bb->SetLineWidth(2);
    h_mc->SetMaximum(1.5 * h_mc->GetMaximum());
    if(!logy){
        h_mc->SetMinimum(0);
    }
    h_mc->Draw("HIST ");
    h_other->Draw("HIST  SAME");
    h_udsg->Draw("HIST  SAME");
    h_c->Draw("HIST  SAME");
    h_b->Draw("HIST  SAME");
    h_cc->Draw("HIST  SAME");
    h_bb->Draw("HIST  SAME");    


    TLegend* leg = new TLegend(0.55,0.55,0.88,0.88);
    leg->AddEntry(h_mc, "Inclusive MC", "l");
    leg->AddEntry(h_other, "Other", "l");
    leg->AddEntry(h_udsg, "UDSG", "l");
    leg->AddEntry(h_c, "C", "l");
    leg->AddEntry(h_b, "B", "l");
    leg->AddEntry(h_cc, "CC", "l");
    leg->AddEntry(h_bb, "BB", "l");
    leg->Draw();

    TLatex* text = new TLatex();
    text->SetNDC();
    text->SetTextSize(0.02);
    text->SetTextFont(42);
    text->DrawLatex(0.55, 0.50, Form("Mu1 p_{T} > %.1f, Mu2 p_{T} > %.1f", mupt_cut1, mupt_cut2));
    text->DrawLatex(0.55, 0.47, Form("Jet p_{T} #in [%.0f, %.0f] GeV", jetpt_min, jetpt_max));
    text->DrawLatex(0.55, 0.44, chargesel == 0 ? "Inclusive Charge" : (chargesel == -1 ? "Opposite-sign" : "Same-sign"));
    text->DrawLatex(0.55, 0.41, Form("other fraction: %.3f", h_other->Integral() / h_mc->Integral()));
    text->DrawLatex(0.55, 0.38, Form("udsg fraction: %.3f", h_udsg->Integral() / h_mc->Integral()));
    text->DrawLatex(0.55, 0.35, Form("c fraction: %.3f", h_c->Integral() / h_mc->Integral()));
    text->DrawLatex(0.55, 0.32, Form("b fraction: %.3f", h_b->Integral() / h_mc->Integral()));
    text->DrawLatex(0.55, 0.29, Form("cc fraction: %.3f", h_cc->Integral() / h_mc->Integral()));
    text->DrawLatex(0.55, 0.26, Form("bb fraction: %.3f", h_bb->Integral() / h_mc->Integral()));
    if(cutoutJpsi){
        text->DrawLatex(0.55, 0.23, "J/#psi mass cut applied");
    }
    c->SaveAs(pdfname);


    delete c;
    delete h_mc;
    delete h_other;
    delete h_udsg;
    delete h_c;
    delete h_b;
    delete h_cc;
    delete h_bb;

}

void datamccomp(){

    gStyle->SetOptStat(0);
    //gErrorIgnoreLevel = kWarning;
    //gROOT->SetBatch(kTRUE);

    vector<float> ptbins = {60,80,100,120,160,200,250,300,400,500};

    TFile* f = TFile::Open("smallskims.root");

    TTree* T_hi = (TTree*)f->Get("Tree_HighEG");
    TTree* T_lo = (TTree*)f->Get("Tree_LowEG");
    TTree* T_mc = (TTree*)f->Get("Tree_MC");
 
    for(int i = 0; i < ptbins.size() - 1; i++){
        int trigger = 40;
        float jptmin = ptbins[i];
        float jptmax = ptbins[i+1];

        
        // JetPT 
        datamc(T_hi, T_lo, T_mc, trigger, "Jet pT", "JetPT", "Jet pT (GeV)", 15, jptmin, jptmax, 0, 4, 4, 0, 1, -3, jptmin, jptmax, false, false, false, Form("plots/datamc_jetpt_%.0f_%.0f.pdf", jptmin, jptmax));
        templates(T_hi, T_lo, T_mc, trigger, "Jet pT", "JetPT", "Jet pT (GeV)", 15, jptmin, jptmax, 0, 4, 4, 0, 1, -3, jptmin, jptmax, false, false, false, Form("plots/templates_jetpt_%.0f_%.0f.pdf", jptmin, jptmax));

        // Single Muon pT 1
        datamc(T_hi, T_lo, T_mc, trigger, "Single Muon pT 1", "muPt1", "Single Muon pT 1 (GeV)", 40, 0, 100, 0, 4, 4, 0, 1, -3, jptmin, jptmax, false, false, false, Form("plots/datamc_singlemupt1_%.0f_%.0f.pdf", jptmin, jptmax));
        templates(T_hi, T_lo, T_mc, trigger, "Single Muon pT 1", "muPt1", "Single Muon pT 1 (GeV)", 40, 0, 100, 0, 4, 4, 0, 1, -3, jptmin, jptmax, false, false, false, Form("plots/templates_singlemupt1_%.0f_%.0f.pdf", jptmin, jptmax));

        // Single Muon pT 2
        datamc(T_hi, T_lo, T_mc, trigger, "Single Muon pT 2", "muPt2", "Single Muon pT 2 (GeV)", 20, 0, 50, 0, 4, 4, 0, 1, -3, jptmin, jptmax, false, false, false, Form("plots/datamc_singlemupt2_%.0f_%.0f.pdf", jptmin, jptmax));
        templates(T_hi, T_lo, T_mc, trigger, "Single Muon pT 2", "muPt2", "Single Muon pT 2 (GeV)", 20, 0, 50, 0, 4, 4, 0, 1, -3, jptmin, jptmax, false, false, false, Form("plots/templates_singlemupt2_%.0f_%.0f.pdf", jptmin, jptmax));
        
        // Dimuon pT 
        datamc(T_hi, T_lo, T_mc, trigger, "Dimuon pT", "mumuPt", "Dimuon pT (GeV)", 30, 0, 200, 0, 4, 4, 0, 1, -3, jptmin, jptmax, false, false, false, Form("plots/datamc_dimuonpt_%.0f_%.0f.pdf", jptmin, jptmax));
        templates(T_hi, T_lo, T_mc, trigger, "Dimuon pT", "mumuPt", "Dimuon pT (GeV)", 30, 0, 200, 0, 4, 4, 0, 1, -3, jptmin, jptmax, false, true, false, Form("plots/templates_dimuonpt_%.0f_%.0f.pdf", jptmin, jptmax));

        // Z
        datamc(T_hi, T_lo, T_mc, trigger, "Fragmentation Function", "mumuPt / JetPT", "Z", 20, 0, 1, 0, 4, 4, 0, 1, -3, jptmin, jptmax, false, false, false, Form("plots/datamc_z_%.0f_%.0f.pdf", jptmin, jptmax));
        templates(T_hi, T_lo, T_mc, trigger, "Fragmentation Function", "mumuPt / JetPT", "Z", 20, 0, 1, 0, 4, 4, 0, 1, -3, jptmin, jptmax, false, true, false, Form("plots/templates_z_%.0f_%.0f.pdf", jptmin, jptmax));

        // Mass
        datamc(T_hi, T_lo, T_mc, trigger, "Dimuon Mass", "mumuMass", "Mass (GeV)", 50, 0, 7, 0, 4, 4, 0, 1, -3, jptmin, jptmax, false, false, false, Form("plots/datamc_mass_%.0f_%.0f.pdf", jptmin, jptmax));
        templates(T_hi, T_lo, T_mc, trigger, "Dimuon Mass", "mumuMass", "Mass (GeV)", 50, 0, 7, 0, 4, 4, 0, 1, -3, jptmin, jptmax, false, true, false, Form("plots/templates_mass_%.0f_%.0f.pdf", jptmin, jptmax));

        // dR
        datamc(T_hi, T_lo, T_mc, trigger, "DR", "muDR", "#DeltaR", 20, 0, 0.6, 0, 4, 4, 0, 1, -3, jptmin, jptmax, false, false, false, Form("plots/datamc_dR_%.0f_%.0f.pdf", jptmin, jptmax));
        templates(T_hi, T_lo, T_mc, trigger, "DR", "muDR", "#DeltaR", 20, 0, 0.6, 0, 4, 4, 0, 1, -3, jptmin, jptmax, false, true, false, Form("plots/templates_dR_%.0f_%.0f.pdf", jptmin, jptmax));

        // DCA
        datamc(T_hi, T_lo, T_mc, trigger, "Dimuon DCA", "log10(abs(muDiDxy1Dxy2 / muDiDxy1Dxy2Err))", "DCAxy Product Significance", 20, -3, 4, -1, 4, 4, 0, 1, -3, jptmin, jptmax, false, false, false, Form("plots/datamc_DCA_%.0f_%.0f.pdf", jptmin, jptmax));
        templates(T_hi, T_lo, T_mc, trigger, "Dimuon DCA", "log10(abs(muDiDxy1Dxy2 / muDiDxy1Dxy2Err))", "DCAxy Product Significance", 20, -3, 4, -1, 4, 4, 0, 1, -3, jptmin, jptmax, false, true, false, Form("plots/templates_DCA_%.0f_%.0f.pdf", jptmin, jptmax));

    }
    
    
    


}