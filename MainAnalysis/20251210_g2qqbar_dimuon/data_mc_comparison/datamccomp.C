
void datamc(
    TTree* T_hi,
    TTree* T_lo,
    TTree* T_mc,
    const char* label,
    const char* varname,
    const char* xlabel,
    int xbins,
    float xmin,
    float xmax,
    int chargesel,
    float mupt_cut,
    float jetpt_min,
    float jetpt_max,
    bool logy,
    bool doNoweights,
    const char* pdfname

){


    // project histograms
    TH1D* h_80 = new TH1D("h_80", Form("HighEG %s Spectrum; %s ;Entries", xlabel, xlabel), xbins, xmin, xmax);
    TH1D* h_60 = new TH1D("h_60", Form("LowEG %s Spectrum; %s ;Entries", xlabel, xlabel), xbins, xmin, xmax);
    TH1D* h_mc = new TH1D("h_mc", Form("MC %s Spectrum; %s ;Entries", xlabel, xlabel), xbins, xmin, xmax);
    TH1D* h_mc_noweights = new TH1D("h_mc_noweights", Form("MC no weights %s Spectrum; %s ;Entries", xlabel, xlabel), xbins, xmin, xmax);

    T_hi->Project("h_80", varname, Form("IsMuMuTagged && HLT_HIAK4PFJet80_v1 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0)", mupt_cut, mupt_cut, jetpt_min, jetpt_max, chargesel, chargesel));
    T_lo->Project("h_60", varname, Form("IsMuMuTagged && HLT_HIAK4PFJet60_v1 && !HLT_HIAK4PFJet80_v1 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0)", mupt_cut, mupt_cut, jetpt_min, jetpt_max, chargesel, chargesel));
    T_mc->Project("h_mc", varname, Form("EventWeight * (IsMuMuTagged && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0))", mupt_cut, mupt_cut, jetpt_min, jetpt_max, chargesel, chargesel));
    T_mc->Project("h_mc_noweights", varname, Form("IsMuMuTagged && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0)", mupt_cut, mupt_cut, jetpt_min, jetpt_max, chargesel, chargesel));
    cout << "Done projecting histograms" << endl;


    TH1D* h_data = (TH1D*)h_80->Clone("h_data");
    h_data->Add(h_60, 6.338);

    // plot data vs mc comparison
    TCanvas* c = new TCanvas("c_datamccomp", "", 800, 600);
    if(logy){ c->SetLogy(); }

    h_data->Scale(1.0/h_data->Integral());
    h_mc->Scale(1.0/h_mc->Integral());
    if(doNoweights){
        h_mc_noweights->Scale(1.0/h_mc_noweights->Integral());
    }
    
    double maxval = TMath::Max(h_data->GetMaximum(), h_mc->GetMaximum());
    if(doNoweights){
        maxval = TMath::Max(maxval, h_mc_noweights->GetMaximum());
    }
    h_data->SetMaximum(1.2 * maxval);
    h_mc->SetMaximum(1.2 * maxval);
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

    TLegend* leg = new TLegend(0.55,0.75,0.88,0.88);
    leg->AddEntry(h_data, "Data", "l");
    leg->AddEntry(h_mc, "MC with pThat weight", "l");
    if(doNoweights){
        leg->AddEntry(h_mc_noweights, "MC without pThat weight", "l");
    }
    h_mc->Draw("HIST");
    h_data->Draw("HIST SAME");  
    if(doNoweights){
        h_mc_noweights->Draw("HIST SAME");
    }
    leg->Draw();

    TLatex* text = new TLatex();
    text->SetNDC();
    text->SetTextSize(0.04);
    text->SetTextFont(42);
    text->DrawLatex(0.55, 0.70, Form("Muon p_{T} > %.1f GeV", mupt_cut));
    text->DrawLatex(0.55, 0.65, Form("Jet p_{T} #in [%.0f, %.0f] GeV", jetpt_min, jetpt_max));
    text->DrawLatex(0.55, 0.60, chargesel == 0 ? "Inclusive Charge" : (chargesel == 1 ? "Opposite-sign" : "Same-sign"));

    c->SaveAs(pdfname);

    delete c;
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
    const char* label,
    const char* varname,
    const char* xlabel,
    int xbins,
    float xmin,
    float xmax,
    int chargesel,
    float mupt_cut,
    float jetpt_min,
    float jetpt_max,
    bool logy,
    bool doNoweights,
    const char* pdfname
){

    TH1D* h_mc = new TH1D("h_mc_2", Form("MC %s Spectrum; %s ;Entries", xlabel, xlabel), xbins, xmin, xmax);
    TH1D* h_other = new TH1D("h_other", Form("MC %s Spectrum (Other); %s ;Entries", xlabel, xlabel), xbins, xmin, xmax);
    TH1D* h_udsg = new TH1D("h_udsg", Form("MC %s Spectrum (UDSG); %s ;Entries", xlabel, xlabel), xbins, xmin, xmax);
    TH1D* h_c = new TH1D("h_c", Form("MC %s Spectrum (C); %s ;Entries", xlabel, xlabel), xbins, xmin, xmax);
    TH1D* h_b = new TH1D("h_b", Form("MC %s Spectrum (B); %s ;Entries", xlabel, xlabel), xbins, xmin, xmax);
    TH1D* h_cc = new TH1D("h_cc", Form("MC %s Spectrum (CC); %s ;Entries", xlabel, xlabel), xbins, xmin, xmax);
    TH1D* h_bb = new TH1D("h_bb", Form("MC %s Spectrum (BB); %s ;Entries", xlabel, xlabel), xbins, xmin, xmax);

    if(doNoweights){
        T_mc->Project("h_mc_2", varname, Form("(IsMuMuTagged && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0))", mupt_cut, mupt_cut, jetpt_min, jetpt_max, chargesel, chargesel));   
        T_mc->Project("h_other", varname, Form("(IsMuMuTagged && (NbHad > 2 || (NbHad == 0 && NcHad > 2)) && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0))", mupt_cut, mupt_cut, jetpt_min, jetpt_max, chargesel, chargesel));
        T_mc->Project("h_udsg", varname, Form("(IsMuMuTagged && NbHad == 0 && NcHad == 0 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0))", mupt_cut, mupt_cut, jetpt_min, jetpt_max, chargesel, chargesel));
        T_mc->Project("h_c", varname, Form("(IsMuMuTagged && NbHad == 0 && NcHad == 1 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0))", mupt_cut, mupt_cut, jetpt_min, jetpt_max, chargesel, chargesel));
        T_mc->Project("h_b", varname, Form("(IsMuMuTagged && NbHad == 1 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0))", mupt_cut, mupt_cut, jetpt_min, jetpt_max, chargesel, chargesel));
        T_mc->Project("h_cc", varname, Form("(IsMuMuTagged && NbHad == 0 && NcHad == 2 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0))", mupt_cut, mupt_cut, jetpt_min, jetpt_max, chargesel, chargesel));
        T_mc->Project("h_bb", varname, Form("(IsMuMuTagged && NbHad == 2 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0))", mupt_cut, mupt_cut, jetpt_min, jetpt_max, chargesel, chargesel));
    }
    else{   
        T_mc->Project("h_mc_2", varname, Form("EventWeight * (IsMuMuTagged && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0))", mupt_cut, mupt_cut, jetpt_min, jetpt_max, chargesel, chargesel));   
        T_mc->Project("h_other", varname, Form("EventWeight * (IsMuMuTagged && (NbHad > 2 || (NbHad == 0 && NcHad > 2)) && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0))", mupt_cut, mupt_cut, jetpt_min, jetpt_max, chargesel, chargesel));
        T_mc->Project("h_udsg", varname, Form("EventWeight * (IsMuMuTagged && NbHad == 0 && NcHad == 0 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0))", mupt_cut, mupt_cut, jetpt_min, jetpt_max, chargesel, chargesel));
        T_mc->Project("h_c", varname, Form("EventWeight * (IsMuMuTagged && NbHad == 0 && NcHad == 1 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0))", mupt_cut, mupt_cut, jetpt_min, jetpt_max, chargesel, chargesel));
        T_mc->Project("h_b", varname, Form("EventWeight * (IsMuMuTagged && NbHad == 1 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0))", mupt_cut, mupt_cut, jetpt_min, jetpt_max, chargesel, chargesel));
        T_mc->Project("h_cc", varname, Form("EventWeight * (IsMuMuTagged && NbHad == 0 && NcHad == 2 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0))", mupt_cut, mupt_cut, jetpt_min, jetpt_max, chargesel, chargesel));
        T_mc->Project("h_bb", varname, Form("EventWeight * (IsMuMuTagged && NbHad == 2 && muPt1 > %.1f && muPt2 > %.1f && JetPT > %.1f && JetPT < %.1f && (muCharge1 * muCharge2 == %d || %d == 0))", mupt_cut, mupt_cut, jetpt_min, jetpt_max, chargesel, chargesel));
    }
        cout << "Done projecting histograms" << endl;

    TCanvas* c = new TCanvas("c_templates", "", 800, 600);
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
    h_mc->Draw("HIST");
    h_other->Draw("HIST SAME");
    h_udsg->Draw("HIST SAME");
    h_c->Draw("HIST SAME");
    h_b->Draw("HIST SAME");
    h_cc->Draw("HIST SAME");
    h_bb->Draw("HIST SAME");    

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
    text->DrawLatex(0.55, 0.50, Form("Muon p_{T} > %.1f GeV", mupt_cut));
    text->DrawLatex(0.55, 0.47, Form("Jet p_{T} #in [%.0f, %.0f] GeV", jetpt_min, jetpt_max));
    text->DrawLatex(0.55, 0.44, chargesel == 0 ? "Inclusive Charge" : (chargesel == 1 ? "Opposite-sign" : "Same-sign"));
    text->DrawLatex(0.55, 0.41, Form("other fraction: %.3f", h_other->Integral() / h_mc->Integral()));
    text->DrawLatex(0.55, 0.38, Form("udsg fraction: %.3f", h_udsg->Integral() / h_mc->Integral()));
    text->DrawLatex(0.55, 0.35, Form("c fraction: %.3f", h_c->Integral() / h_mc->Integral()));
    text->DrawLatex(0.55, 0.32, Form("b fraction: %.3f", h_b->Integral() / h_mc->Integral()));
    text->DrawLatex(0.55, 0.29, Form("cc fraction: %.3f", h_cc->Integral() / h_mc->Integral()));
    text->DrawLatex(0.55, 0.26, Form("bb fraction: %.3f", h_bb->Integral() / h_mc->Integral()));
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

    vector<float> ptbins = {60,80,100,120,160,200,250,300};

    TFile* f = TFile::Open("smallskims.root");

    TTree* T_hi = (TTree*)f->Get("Tree_HighEG");
    TTree* T_lo = (TTree*)f->Get("Tree_LowEG");
    TTree* T_mc = (TTree*)f->Get("Tree_MC");

    // datamc(TREES stay,     LABEL,VARNAME,XLABEL, BINS, MIN,MAX, CHARGESEL, MUPT_CUT, JETPT_MIN, JETPT_MAX, LOGY, UNWEIGHTED, PDF FILENAME)

    datamc(T_hi, T_lo, T_mc, "Charge", "muCharge1 * muCharge2", "Charge", 2, -2, 2, 0, 3.5, 100, 500, false, false, "datamc_Charge1.pdf");
    datamc(T_hi, T_lo, T_mc, "Charge", "muCharge1 * muCharge2", "Charge", 2, -2, 2, 0, 8, 100, 500, false, false, "datamc_Charge2.pdf");
    
    templates(T_hi, T_lo, T_mc, "Charge", "muCharge1 * muCharge2", "Charge", 2, -2, 2, 0, 3.5, 100, 500, false, false, "templates_Charge1.pdf");
    templates(T_hi, T_lo, T_mc, "Charge", "muCharge1 * muCharge2", "Charge", 2, -2, 2, 0, 8, 100, 500, false, false, "templates_Charge2.pdf");


}