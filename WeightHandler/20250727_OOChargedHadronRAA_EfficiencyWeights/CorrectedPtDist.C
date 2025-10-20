
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <ROOT/RDataFrame.hxx>

float getweight(float value, TH1D* hist) {
    if(!hist) {
        cout << "WARNING: Null histogram!" << endl;
        return 1.0;
    }
    int bin = hist->FindBin(value);
    float binContent = hist->GetBinContent(bin);
    if(binContent <= 0) {
        cout << "WARNING: Zero/negative bin content for value " << value << ", using mult=999 weight" << endl;
        int bin999 = hist->FindBin(999);
        float binContent999 = hist->GetBinContent(bin999);
        return binContent999 > 0 ? 1.0/binContent999 : 1.0;
    }
    float weight = 1.0/binContent;
    if(!std::isfinite(weight)) {
        cout << "WARNING: Non-finite weight for value " << value << ", using mult=999 weight" << endl;
        int bin999 = hist->FindBin(999);
        float binContent999 = hist->GetBinContent(bin999);
        return binContent999 > 0 ? 1.0/binContent999 : 1.0;
    }
    return weight;
}

TChain* createDataChain(const char* dataSource) {
    TChain* chain = new TChain("Tree");
    string source(dataSource);
    if (source.find(".txt") != string::npos) {
        ifstream infile(dataSource);
        string line;
        int fileCount = 0;
        while (getline(infile, line)) {
            // Skip empty lines and comments
            if (!line.empty() && line[0] != '#') {
                chain->Add(line.c_str());
                fileCount++;
            }
        }
    }
    return chain;
}

void CorrectedPtDist(
                TCut Datacut,
                const char* dataSource = "datafiles.txt",
                const char* efficiencyfile = "hists/output.root",
                const char* effhistname = "hMult_Eff",
                int useMultiplicity = 1
                ) {
                  
    cout << "STARTING TRACK PT CORRECTION DISTRIBUTIONS" << endl;
    
    // Disable multithreading for debugging
    // ROOT::EnableImplicitMT();

    ///// OPEN FILES             
    TChain* chainData = createDataChain(dataSource);
    TFile* fEff = TFile::Open(efficiencyfile);
    TH1D* hEff = (TH1D*)fEff->Get(effhistname);

    const Int_t nPtBins_log = 48;
    const Double_t pTBins_log[nPtBins_log + 1] = {
        3.0,3.2,3.4,3.6,3.8,4.0,4.4,4.8,5.2,5.6,6.0,6.4,6.8,7.2,7.6,8.0,
        8.5,9.0,9.5,10.0,11.,12.,13.,14.,15.,16.,17.,18.,19.,20.,21.,22.6,24.6,
        26.6,28.6,32.6,36.6,42.6,48.6,54.6,60.6,74.0,86.4,103.6,120.8,140.,165.,
        250.,400.
    };

    /*const Int_t nPtBins = 26;
    const Double_t ptBins[nPtBins + 1] = {
        0.5, 0.603, 0.728, 0.879, 1.062, 1.284, 1.553, 1.878, 2.272, 2.749, 
        3.327, 4.027, 4.872, 5.891, 7.117, 8.591, 10.36, 12.48, 
        15.03, 18.08, 21.73, 26.08, 31.28, 37.48, 44.89, 53.73, 64.31
    };*/

    TH1D* hCorrected = new TH1D("hCorrected", "Corrected p_{T} Distribution", nPtBins_log, pTBins_log);
    TH1D* hUncorrected = new TH1D("hUncorrected", "Uncorrected p_{T} Distribution", nPtBins_log, pTBins_log);
    hCorrected->Sumw2();
    hUncorrected->Sumw2();
    
    // Simple RDataFrame approach - no TTreeFormula
    ROOT::RDataFrame df(*chainData);
    
    // Convert TCut to simple string filter
    std::string cutStr = Datacut.GetTitle();
    cout << "Applying cut: " << cutStr << endl;
    
    auto dfFiltered = df.Filter(cutStr);
    
    cout << "Processing entries..." << endl;
    
    // Add debugging counters
    int totalEvents = 0;
    int totalTracks = 0;
    int passedTracks = 0;
    
    // Simple lambda to fill histograms
    dfFiltered.Foreach([&](const ROOT::RVec<float>& trkPt, const ROOT::RVec<bool>& trkPass, int mult, float leadingPt) {
        totalEvents++;
        
        float weight = 1.0;
        if(useMultiplicity) {
            weight = getweight((float)mult, hEff);
        } else {
            weight = getweight(leadingPt, hEff);
        }
        
        // Better weight printing with validation - only print if problematic
        if(std::isfinite(weight) && weight > 0) {
            // Check if weight is suspiciously low (should be > 1 for efficiency corrections)
            if(weight < 1.0) {
                cout << "WARNING: Weight < 1.0: " << weight << " (mult=" << mult << ", leadingPt=" << leadingPt << ")" << endl;
            }
        } else {
            cout << "Invalid weight: " << weight << " (mult=" << mult << ", leadingPt=" << leadingPt << "), using mult=999 fallback" << endl;
           
            weight = getweight(999, hEff);
        }

        // Apply track quality cuts
        for(size_t i = 0; i < trkPt.size(); i++) {
            totalTracks++;
            // Only fill if track passes quality cut
            if(i < trkPass.size() && trkPass[i]) {
                passedTracks++;
                
                // DEBUG: Print weight for first few tracks
                if(passedTracks <= 5) {
                    cout << "Track " << passedTracks << ": pT=" << trkPt[i] << ", weight=" << weight << endl;
                }
                
                // Fill both histograms with same track
                hUncorrected->Fill(trkPt[i], 1.0);
                hCorrected->Fill(trkPt[i], weight);
                
                // Additional check for problematic weights
                if(!std::isfinite(weight) || weight <= 0) {
                    cout << "ERROR: Filling with bad weight " << weight << " for track pT " << trkPt[i] << endl;
                }
            }
        }
    }, {"trkPt", "trkPassChargedHadron_Nominal", "multiplicityEta2p4", "leadingPtEta1p0_sel"});
    
    cout << "DEBUGGING TRACK FILLING:" << endl;
    cout << "Total events processed: " << totalEvents << endl;
    cout << "Total tracks: " << totalTracks << endl;  
    cout << "Tracks passing quality cuts: " << passedTracks << endl;

    // Debug: Print some statistics before creating ratio
    cout << "DEBUG: Histogram statistics before ratio:" << endl;
    cout << "Corrected entries: " << hCorrected->GetEntries() << ", integral: " << hCorrected->Integral() << endl;
    cout << "Uncorrected entries: " << hUncorrected->GetEntries() << ", integral: " << hUncorrected->Integral() << endl;
    
    // Check a few bins at low pT
    for(int i = 1; i <= 5; i++) {
        double corrContent = hCorrected->GetBinContent(i);
        double uncorrContent = hUncorrected->GetBinContent(i);
        double ratio = uncorrContent > 0 ? corrContent/uncorrContent : 0;
        cout << "Bin " << i << " (pT=" << hCorrected->GetBinCenter(i) << "): corr=" << corrContent << ", uncorr=" << uncorrContent << ", ratio=" << ratio << endl;
    }

    TH1D* hRatio = (TH1D*)hCorrected->Clone("hRatio");
    hRatio->Divide(hCorrected, hUncorrected, 1.0, 1.0, "B");

    for (int i = 1; i <= hCorrected->GetNbinsX(); ++i) {
        double binWidth = hCorrected->GetBinWidth(i);
        
        double corrContent = hCorrected->GetBinContent(i);
        double corrError = hCorrected->GetBinError(i);
        hCorrected->SetBinContent(i, corrContent / binWidth);
        hCorrected->SetBinError(i, corrError / binWidth);   
       
        double uncorrContent = hUncorrected->GetBinContent(i);
        double uncorrError = hUncorrected->GetBinError(i);
        hUncorrected->SetBinContent(i, uncorrContent / binWidth);
        hUncorrected->SetBinError(i, uncorrError / binWidth);
    }

    TFile* fEffUpdate = TFile::Open(efficiencyfile, "UPDATE");
    fEffUpdate->cd();
    hCorrected->Write();
    hUncorrected->Write();
    hRatio->Write();
    fEffUpdate->Close();
    
    gStyle->SetOptStat(0);
    TCanvas* c1 = new TCanvas("c1", "Track pT Spectra with Ratio", 800, 800);
    c1->Divide(1, 2);
    
    c1->cd(1);
    gPad->SetPad(0, 0.3, 1, 1);
    gPad->SetLogy();
    gPad->SetLogx();
    gPad->SetBottomMargin(0.02);
    
    hUncorrected->SetLineColor(kRed);
    hUncorrected->SetLineWidth(2);
    hUncorrected->SetTitle("");
    hUncorrected->GetYaxis()->SetTitle("1/N_{ev} dN/dp_{T}");
    hUncorrected->GetXaxis()->SetLabelSize(0);
    
    hCorrected->SetLineColor(kBlue);
    hCorrected->SetLineWidth(2);
    
    hUncorrected->Draw("HIST");
    hCorrected->Draw("HIST SAME");
    
    TLegend* leg = new TLegend(0.5, 0.75, 0.85, 0.9);
    leg->AddEntry(hUncorrected, "Uncorrected", "l");
    leg->AddEntry(hCorrected, "Efficiency Corrected", "l");
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->Draw();
    
    c1->cd(2);
    gPad->SetPad(0, 0, 1, 0.3);
    gPad->SetLogx();
    gPad->SetTopMargin(0.02);
    gPad->SetBottomMargin(0.3);
    
    hRatio->SetLineColor(kBlack);
    hRatio->SetLineWidth(2);
    hRatio->SetMarkerStyle(20);
    hRatio->SetMarkerSize(0.5);
    hRatio->GetYaxis()->SetTitle("Eff Corrected/Uncorrected");
    hRatio->GetYaxis()->SetTitleSize(0.08);
    hRatio->GetYaxis()->SetLabelSize(0.08);
    hRatio->GetYaxis()->SetTitleOffset(0.4);
    hRatio->GetYaxis()->SetNdivisions(505);
    hRatio->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    hRatio->GetXaxis()->SetTitleSize(0.08);
    hRatio->GetXaxis()->SetLabelSize(0.08);
    hRatio->GetXaxis()->SetTitleOffset(1.0);
    hRatio->SetMaximum(1.05);
    hRatio->SetMinimum(0.95);

    hRatio->Draw("PE");

    TLine* line = new TLine(hRatio->GetXaxis()->GetXmin(), 1, hRatio->GetXaxis()->GetXmax(), 1);
    line->SetLineColor(kRed);
    line->SetLineStyle(2);
    line->SetLineWidth(2);
    line->Draw("same");
    
    c1->SaveAs("TrackPtSpectra.pdf");

    cout << "COMPLETED TRACK PT CORRECTION DISTRIBUTIONS" << endl;
    cout << endl;
    cout << endl;


}