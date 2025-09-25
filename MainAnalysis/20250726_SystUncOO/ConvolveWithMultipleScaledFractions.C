void ConvolveWithMultipleScaledFractions() {
    // Open files
    TFile* fEff = TFile::Open("AbsoluteEfficiency_ppOfficialHijing_byPdgId.root", "READ");
    TFile* fFrac = TFile::Open("alicehepdataparticlefraction.root", "READ");

    if (!fEff || !fFrac || fEff->IsZombie() || fFrac->IsZombie()) {
        std::cerr << "Error opening one of the ROOT files!" << std::endl;
        return;
    }

    const int nScenarios = 4;
    double dNdetaVals[nScenarios] = {10, 40, 100, 200};
    double sigmaScales[nScenarios] = {1.000, 1.316, 1.469, 1.524};
    double protonScales[nScenarios] = {1.000, 0.945, 0.889, 0.894};
    double kaonScales[nScenarios] = {1.000, 1.043, 1.089, 1.110};

    struct Species {
        const char* effHistName;
        const char* fracHistName;
        const char* label;
        double scale; // will be updated per scenario
    };

    Species speciesNoRemainder[] = {
        {"rEff_1D_211",  "hpionfractionalice",       "Pion",      1.0},
        {"rEff_1D_321",  "hkaonfractionalice",       "Kaon",      1.0},
        {"rEff_1D_2212", "hprotonfractionalice",     "Proton",    1.0},
        {"rEff_1D_3222", "hsigmaplussigmabarminus",  "Sigma+",    1.0},
        {"rEff_1D_3112", "hsigmaminussigmabarplus",  "Sigma-",    1.0}
    };

    Species remainderSpecies = {"rEff_1D_9999", nullptr, "Remainder", 1.0};

    TH1D* hInclusive = (TH1D*) fEff->Get("rEff_1D_0");
    if (!hInclusive) {
        std::cerr << "Could not find inclusive efficiency histogram rEff_1D_0" << std::endl;
        return;
    }

    TH1D* hConvolved[nScenarios];
    TH1D* hRatio[nScenarios];

    for (int sc = 0; sc < nScenarios; ++sc) {
        TString name = TString::Format("hConvolved_dNdeta_%d", (int)dNdetaVals[sc]);
        hConvolved[sc] = (TH1D*) hInclusive->Clone(name);
        hConvolved[sc]->Reset();
        hConvolved[sc]->SetTitle("");
        hConvolved[sc]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
        hConvolved[sc]->GetYaxis()->SetTitle("Efficiency");

        TString rname = TString::Format("hRatio_dNdeta_%d", (int)dNdetaVals[sc]);
        hRatio[sc] = (TH1D*) hInclusive->Clone(rname);
        hRatio[sc]->Reset();
        hRatio[sc]->SetTitle("");
        hRatio[sc]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
        hRatio[sc]->GetYaxis()->SetTitle("Ratio to Inclusive");
    }

    // Compute convolved efficiencies
    for (int i = 1; i <= hInclusive->GetNbinsX(); ++i) {
        double pt = hInclusive->GetBinCenter(i);

        for (int sc = 0; sc < nScenarios; ++sc) {
            speciesNoRemainder[1].scale = kaonScales[sc];
            speciesNoRemainder[2].scale = protonScales[sc];
            speciesNoRemainder[3].scale = sigmaScales[sc];
            speciesNoRemainder[4].scale = sigmaScales[sc];

            std::vector<double> effValues;
            std::vector<double> fracValues;
            double totalScaledFrac = 0;

            for (const auto& sp : speciesNoRemainder) {
                TH1D* hEff = (TH1D*) fEff->Get(sp.effHistName);
                TH1F* hFrac = (TH1F*) fFrac->Get(sp.fracHistName);
                if (!hEff || !hFrac) {
                    effValues.push_back(0);
                    fracValues.push_back(0);
                    continue;
                }
                double eff = hEff->GetBinContent(hEff->FindBin(pt));
                double frac = hFrac->Interpolate(pt) * sp.scale;
                effValues.push_back(eff);
                fracValues.push_back(frac);
                totalScaledFrac += frac;
            }

            for (auto& f : fracValues) {
                if (totalScaledFrac > 0) f /= totalScaledFrac;
                else f = 0;
            }

            double remainderFrac = 1.0;
            for (auto f : fracValues) remainderFrac -= f;

            TH1D* hEffRemainder = (TH1D*) fEff->Get(remainderSpecies.effHistName);
            if (!hEffRemainder) {
                std::cerr << "Missing remainder efficiency histogram" << std::endl;
                continue;
            }
            double effRemainder = hEffRemainder->GetBinContent(hEffRemainder->FindBin(pt));
            effValues.push_back(effRemainder);
            fracValues.push_back(remainderFrac);

            double weightedEff = 0;
            for (size_t j = 0; j < effValues.size(); ++j) {
                weightedEff += effValues[j] * fracValues[j];
            }

            hConvolved[sc]->SetBinContent(i, weightedEff);

            // Calculate ratio to inclusive (avoid division by zero)
            double inclEff = hInclusive->GetBinContent(i);
            if (inclEff > 1e-6)
                hRatio[sc]->SetBinContent(i, weightedEff / inclEff);
            else
                hRatio[sc]->SetBinContent(i, 0);
        }
    }

    // Style histograms
    Color_t colors[nScenarios] = {kRed+1, kGreen+2, kOrange+7, kMagenta+2};

    TH1D* hInclusiveClone = (TH1D*) hInclusive->Clone("hInclusiveClone");
    hInclusiveClone->SetLineColor(kBlue+2);
    hInclusiveClone->SetLineWidth(2);
    hInclusiveClone->SetMarkerStyle(24);
    hInclusiveClone->SetMarkerColor(kBlue+2);

    for (int sc = 0; sc < nScenarios; ++sc) {
        hConvolved[sc]->SetLineColor(colors[sc]);
        hConvolved[sc]->SetLineWidth(2);
        hConvolved[sc]->SetMarkerStyle(20 + sc);
        hConvolved[sc]->SetMarkerColor(colors[sc]);

        hRatio[sc]->SetLineColor(colors[sc]);
        hRatio[sc]->SetLineWidth(2);
        hRatio[sc]->SetMarkerStyle(20 + sc);
        hRatio[sc]->SetMarkerColor(colors[sc]);
    }

    // Create canvas and pads
    TCanvas* c = new TCanvas("cMultiScaleRatio", "Convolved Efficiencies + Ratios", 900, 900);
    c->Divide(1, 2);

    // Top pad: efficiencies
    c->cd(1);
    gPad->SetPad(0, 0.35, 1, 1);
    gPad->SetBottomMargin(0.02);
    gPad->SetGrid();

    hInclusiveClone->SetMaximum(1.0);
    hInclusiveClone->SetMinimum(0.0);
    hInclusiveClone->GetXaxis()->SetRangeUser(0, 20);
    hInclusiveClone->GetYaxis()->SetTitle("Efficiency");
    hInclusiveClone->Draw("E1");

    for (int sc = 0; sc < nScenarios; ++sc) {
        hConvolved[sc]->Draw("E1 SAME");
    }

    // Legend labels
    const char* legendLabels[nScenarios] = {
        "ALICE pp fractions",
        "dN/d#eta = 40 (OO-like, centrality-inclusive)",
        "dN/d#eta = 100 (OO-like, 0-1%)",
        "dN/d#eta = 200 (XeXe peripheral-like)"
    };

// Legend - change "Inclusive Efficiency" to "PYTHIA8"
TLegend* leg = new TLegend(0.15, 0.6, 0.45, 0.85);
leg->AddEntry(hInclusiveClone, "PYTHIA8", "lep");
for (int sc = 0; sc < nScenarios; ++sc) {
    leg->AddEntry(hConvolved[sc], legendLabels[sc], "lep");
}
leg->SetBorderSize(0);
leg->SetFillStyle(0);
leg->Draw();

// Bottom pad: ratios with bigger fonts
c->cd(2);
gPad->SetPad(0, 0, 1, 0.35);
gPad->SetTopMargin(0.02);
gPad->SetBottomMargin(0.3);
gPad->SetGrid();

hRatio[0]->SetMaximum(1.3);
hRatio[0]->SetMinimum(0.7);
hRatio[0]->GetXaxis()->SetRangeUser(0, 20);

hRatio[0]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
hRatio[0]->GetYaxis()->SetTitle("Ratio to PYTHIA8");

// Increase font sizes on ratio plot to match upper panel
hRatio[0]->GetYaxis()->SetTitleSize(0.07);
hRatio[0]->GetYaxis()->SetTitleOffset(0.6);
hRatio[0]->GetYaxis()->SetLabelSize(0.07);

hRatio[0]->GetXaxis()->SetTitleSize(0.07);
hRatio[0]->GetXaxis()->SetLabelSize(0.07);

hRatio[0]->Draw("E1");

for (int sc = 1; sc < nScenarios; ++sc) {
    hRatio[sc]->SetTitle("");
    hRatio[sc]->Draw("E1 SAME");
}

// Draw horizontal line at ratio = 1
TLine *line = new TLine(0, 1, 20, 1);
line->SetLineColor(kBlack);
line->SetLineStyle(2);
line->Draw("same");

}
