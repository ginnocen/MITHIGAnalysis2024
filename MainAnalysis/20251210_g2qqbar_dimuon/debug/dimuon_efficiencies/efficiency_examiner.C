
float error(float nx, float ny, float errx, float erry){
    

    if(ny == 0){
        return 0;
    }

    float dfdx = 1.0/ny;
    float dfdy = -nx/(ny*ny);

    return sqrt( (dfdx*errx)*(dfdx*errx) + (dfdy*erry)*(dfdy*erry) );

}


void efficiency_examiner(){

    gStyle->SetOptStat(0);


    TFile* f = TFile::Open("../../testefficiencies_nomatch_a.root");
    TFile* f2 = TFile::Open("../../testefficiencies_match_a.root");

    
    TH2D* effhist = (TH2D*)f->Get("DimJetEfficiency");
    TH2D* effhist2 = (TH2D*)f2->Get("DimJetEfficiency");

    TH2D* hGen = (TH2D*)f->Get("hGenDimJets");
    TH2D* hGen2 = (TH2D*)f2->Get("hGenDimJets");

    TH2D* hReco = (TH2D*)f->Get("hRecoDimJets");
    TH2D* hReco2 = (TH2D*)f2->Get("hRecoDimJets");

    cout << hReco->GetBinContent(2,1) << endl;
    cout << hReco2->GetBinContent(2,1) << endl;
    cout << hGen->GetBinContent(2,1) << endl;

    /// FIRST WE DO 1D SLICES IN JET PT
    int nBinsX = effhist->GetXaxis()->GetNbins();
    int nBinsY = effhist->GetYaxis()->GetNbins();

    float ptlo = 0;
    float pthi = 0;

    for(int i = 0; i < nBinsX; i++){

        TH1D* slice = effhist->ProjectionY(Form("slice_%d", i), i+1, i+1);
        TH1D* slice2 = effhist2->ProjectionY(Form("slice2_%d", i), i+1, i+1);

        ptlo = effhist->GetXaxis()->GetBinLowEdge(i+1);
        pthi = effhist->GetXaxis()->GetBinUpEdge(i+1);   
        

        for(int i = 0; i < slice->GetNbinsX(); i++){

            float gen = hGen->GetBinContent(i+1, i+1);
            float reco = hReco->GetBinContent(i+1, i+1);

            float gen2 = hGen2->GetBinContent(i+1, i+1);
            float reco2 = hReco2->GetBinContent(i+1, i+1);

            float errgen = hGen->GetBinError(i+1, i+1);
            float errreco = hReco->GetBinError(i+1, i+1);

            float errgen2 = hGen2->GetBinError(i+1, i+1);
            float errreco2 = hReco2->GetBinError(i+1, i+1);

            float err = error(reco, gen, errreco, errgen);
            float err2 = error(reco2, gen2, errreco2, errgen2);

            slice->SetBinError(i+1, err);
            slice2->SetBinError(i+1, err2);

        }

        TH1D* slicefraction = (TH1D*)slice2->Clone();
        slicefraction->Divide(slice);


        slice->SetLineColor(kRed);
        slice2->SetLineColor(kBlue);

        slice->SetTitle(Form("Efficiency Slice for %.1f < Jet pT < %.1f", ptlo, pthi));
        slice->GetXaxis()->SetTitle("Jet Eta");
        slice->GetYaxis()->SetTitle("Efficiency");

        slice->SetMaximum(1.5);
        slice->SetMinimum(0);

        TLegend* leg = new TLegend(0.6,0.7,0.9,0.9);
        leg->AddEntry(slice, "Soft Muon Efficiency", "l");
        leg->AddEntry(slice2, "Soft2 Muon Efficiency (no fakes)", "l");
        leg->Draw();

        TCanvas* c1 = new TCanvas(Form("c1_%d", i), Form("c1_%d", i), 800, 600);
        slice->Draw("LPE");
        slice2->Draw("LPE SAME");
        leg->Draw();
        
        TLine* line1 = new TLine(slice->GetXaxis()->GetXmin(), 1, slice->GetXaxis()->GetXmax(), 1);
        line1->SetLineStyle(2);
        line1->SetLineColor(kBlack);
        line1->Draw();
        
        c1->SaveAs(Form("efficiency_slice_ptbin_%d.pdf", i));

        TCanvas* c2 = new TCanvas(Form("c2_%d", i), Form("c2_%d", i), 800, 600);
        slicefraction->SetTitle(Form("Efficiency Ratio Slice for %.1f < Jet pT < %.1f", ptlo, pthi));
        slicefraction->GetXaxis()->SetTitle("Jet Eta");
        slicefraction->GetYaxis()->SetTitle("Efficiency Ratio (Fake Removed / All dimuon)");
        slicefraction->SetMaximum(1.1);
        slicefraction->SetMinimum(0.6);
        slicefraction->Draw("HIST");
        
        TLine* line2 = new TLine(slicefraction->GetXaxis()->GetXmin(), 1, slicefraction->GetXaxis()->GetXmax(), 1);
        line2->SetLineStyle(2);
        line2->SetLineColor(kBlack);
        line2->Draw();
        
        c2->SaveAs(Form("efficiency_ratio_slice_ptbin_%d.pdf", i));

    }


    TCanvas*c = new TCanvas("c", "c", 800, 600);
    effhist->Draw("COLZ");
    c->SaveAs("efficiency2.pdf");

    TH1D*eff_jetpt = effhist->ProjectionX("eff_jetpt");
    TH1D*eff_jetpt2 = effhist2->ProjectionX("eff_jetpt2");

    TH1D*reco_jetpt = hReco->ProjectionX("reco_jetpt");
    TH1D*reco_jetpt2 = hReco2->ProjectionX("reco_jetpt2");
    TH1D*gen_jetpt = hGen->ProjectionX("gen_jetpt");
    TH1D*gen_jetpt2 = hGen2->ProjectionX("gen_jetpt2");

    for(int i = 0; i < eff_jetpt->GetNbinsX(); i++){

        float gen = gen_jetpt->GetBinContent(i+1);
        float reco = reco_jetpt->GetBinContent(i+1);

        float gen2 = gen_jetpt2->GetBinContent(i+1);
        float reco2 = reco_jetpt2->GetBinContent(i+1);

        float errgen = gen_jetpt->GetBinError(i+1);
        float errreco = reco_jetpt->GetBinError(i+1);

        float errgen2 = gen_jetpt2->GetBinError(i+1);
        float errreco2 = reco_jetpt2->GetBinError(i+1);

        float err = error(reco, gen, errreco, errgen);
        float err2 = error(reco2, gen2, errreco2, errgen2);

        eff_jetpt->SetBinContent(i+1, reco/gen);
        eff_jetpt2->SetBinContent(i+1, reco2/gen2);

        eff_jetpt->SetBinError(i+1, err);
        eff_jetpt2->SetBinError(i+1, err2);

    }

    TH1D*fakepercent = (TH1D*)eff_jetpt2->Clone("fakepercent");
    fakepercent->Divide(eff_jetpt);

    TCanvas* c3 = new TCanvas("c3", "c3", 800, 600);
    fakepercent->SetTitle("Efficiency Ratio vs Jet pT (Fake Removed / All dimuon)");
    fakepercent->GetXaxis()->SetTitle("Jet pT");
    fakepercent->GetYaxis()->SetTitle("Efficiency Ratio");
    fakepercent->SetMaximum(1.1);
    fakepercent->SetMinimum(0.6);
    fakepercent->Draw("HIST"); 

    TLine* line = new TLine(fakepercent->GetXaxis()->GetXmin(), 1, fakepercent->GetXaxis()->GetXmax(), 1);
    line->SetLineStyle(2);
    line->SetLineColor(kBlack);
    line->Draw();

    c3->SaveAs("efficiency_ratio_jetpt.pdf"); 


    TCanvas* c4 = new TCanvas("c4", "c4", 800, 600);
    eff_jetpt->SetLineColor(kRed);
    eff_jetpt2->SetLineColor(kBlue);
    eff_jetpt->SetTitle("Efficiency vs Jet pT");
    eff_jetpt->GetXaxis()->SetTitle("Jet pT");
    eff_jetpt->GetYaxis()->SetTitle("Efficiency");
    eff_jetpt->SetMaximum(1.5);
    eff_jetpt->SetMinimum(0);
    eff_jetpt->Draw("LPE");
    eff_jetpt2->Draw("LPE SAME");

    TLegend* leg2 = new TLegend(0.6,0.7,0.9,0.9);
    leg2->AddEntry(eff_jetpt, "Soft dimuon Efficiency", "l");
    leg2->AddEntry(eff_jetpt2, "Soft dimuon Efficiency (no fakes)", "l");
    leg2->Draw();   

    TLine* line3 = new TLine(eff_jetpt->GetXaxis()->GetXmin(), 1, eff_jetpt->GetXaxis()->GetXmax(), 1);
    line3->SetLineStyle(2);
    line3->SetLineColor(kBlack);
    line3->Draw();

    c4->SaveAs("efficiency_jetpt.pdf");

}