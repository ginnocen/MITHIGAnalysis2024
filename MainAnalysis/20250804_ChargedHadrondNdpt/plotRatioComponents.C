void plotRatioComponents() {

    // Open NeNe and pp files as in your code
    TFile* fNeNe = TFile::Open("output_NeNeReferenceCentralValue/MergedOutput.root");
    TFile* fNeNeSyst = TFile::Open("systematic_variations/nenesystematicsSmooth.root");

    TH1D* hNeNe = (TH1D*)fNeNe->Get("hTrkPt");
    TH1D* hNeNe_Tracks = (TH1D*)fNeNeSyst->Get("hSystematic_TracksSmooth");
    TH1D* hNeNe_Evt   = (TH1D*)fNeNeSyst->Get("hSystematic_EvtSelSmooth");
    TH1D* hNeNe_Species = (TH1D*)fNeNeSyst->Get("hSystematic_SpeciesSmooth");

    TFile* fPP = TFile::Open("ResultsUIC/pp_OO_raa_systematics_20250813-2.root");

    TH1F* hPP = (TH1F*)fPP->Get("pp_Nominal_data_points");
    TH1F* hPP_pT = (TH1F*)fPP->Get("pp_pT_smearing_syst");
    TH1F* hPP_PU = (TH1F*)fPP->Get("pp_PU_syst");
    TH1F* hPP_track = (TH1F*)fPP->Get("pp_track_DCA_syst");

    int nBins = hNeNe->GetNbinsX();

    // Histograms for individual contributions
    TH1D* hSystSpecies = (TH1D*)hNeNe->Clone("Species");
    TH1D* hSystEvt     = (TH1D*)hNeNe->Clone("EventSelection");
    TH1D* hSystTrack   = (TH1D*)hNeNe->Clone("Track");
    TH1D* hSystPU      = (TH1D*)hNeNe->Clone("Pileup");
    TH1D* hSystTotal   = (TH1D*)hNeNe->Clone("Total");

    hSystSpecies->Reset(); hSystEvt->Reset(); hSystTrack->Reset(); hSystPU->Reset(); hSystTotal->Reset();

    for (int i=1; i<=nBins; ++i) {
        double NeNeVal = hNeNe->GetBinContent(i);
        double ppVal   = hPP->GetBinContent(i);
        if (NeNeVal <= 0 || ppVal <= 0) continue;

        // NeNe systematics
        double systTrack = hNeNe_Tracks->GetBinError(i);
        double systEvt   = hNeNe_Evt->GetBinError(i);
        double systSpec  = hNeNe_Species->GetBinError(i);
        

        double pp_track_rel = hPP_track->GetBinContent(i) / 100.0;
        double pp_PU_rel    = hPP_PU->GetBinContent(i) / 100.0;

        // Fully correlated: track
        double rel_track = fabs(systTrack/NeNeVal - pp_track_rel);

        // Relative contributions
        if (systSpec/NeNeVal < 0.00643009 ) {systSpec = 0.00643009*NeNeVal; }
        hSystSpecies->SetBinContent(i, systSpec / NeNeVal);
        hSystEvt->SetBinContent(i, systEvt / NeNeVal);
        hSystTrack->SetBinContent(i, rel_track);
        hSystPU->SetBinContent(i, pp_PU_rel);

        // Total (quadrature sum of uncorrelated + track)
        // Tracking efficiency cancels in NeNe/pp ratio, pT track resolution cancels in ratio
        double totalRel = sqrt( (systSpec/NeNeVal)*(systSpec/NeNeVal)
//                               + (systEvt/NeNeVal)*(systEvt/NeNeVal) //Decided to remove this to match OO
                               + rel_track*rel_track
                               + pp_PU_rel*pp_PU_rel );
        hSystTotal->SetBinContent(i, totalRel);
    }

    // --- Plot ---
    TCanvas* c = new TCanvas("cRatioSyst","NeNe/pp Ratio Systematics",800,600);

c->SetLogx();
c->SetLogy();

hSystTotal->GetXaxis()->SetRangeUser(3, 103);   // X-axis range in GeV
hSystTotal->GetYaxis()->SetRangeUser(0.001, 0.1); // Y-axis range
hSystTotal->Draw("HIST");

    hSystTotal->SetLineColor(kBlack);
    hSystTotal->SetLineWidth(2);
    hSystTotal->SetTitle("Relative systematic uncertainties vs pT");
    hSystTotal->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    hSystTotal->GetYaxis()->SetTitle("Relative uncertainty");

    hSystSpecies->SetLineColor(kRed);
    hSystEvt->SetLineColor(kBlue);
    hSystTrack->SetLineColor(kGreen+2);
    hSystPU->SetLineColor(kMagenta);

    hSystTotal->Draw("HIST");
    hSystSpecies->Draw("HIST SAME");
//    hSystEvt->Draw("HIST SAME"); //removing it from NeNe
    hSystTrack->Draw("HIST SAME");
    hSystPU->Draw("HIST SAME");

    TLegend* leg = new TLegend(0.6,0.6,0.88,0.88);
    leg->AddEntry(hSystSpecies,"Species correction","l");
//    leg->AddEntry(hSystEvt,"Event selection","l");
    leg->AddEntry(hSystTrack,"Secondary track rejection","l");
    leg->AddEntry(hSystPU,"Pileup (ppRef)","l");
    leg->AddEntry(hSystTotal,"Total","l");
    leg->Draw();

    c->SetLogy();
}

