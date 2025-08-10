#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TGraph.h>
#include <TLegend.h>
#include <vector>
#include <string>

void Draw(const std::vector<std::string>& filenames,
     const char* histname = "h1",
     const std::vector<std::string>& legendVec = {}) {

    std::vector<TH1*> hists;
    std::vector<TFile*> files;
    
    for (const auto& fname : filenames) {
        TFile* file = TFile::Open(fname.c_str(), "READ");
        if (!file || file->IsZombie()) {
            printf("Cannot open file %s\n", fname.c_str());
            if (file) file->Close();
            continue;
        }

        TH1* hist = dynamic_cast<TH1*>(file->Get(histname));
        if (!hist) {
            printf("Histogram %s not found in file %s\n", histname, fname.c_str());
            file->Close();
            continue;
        }
        hist->SetDirectory(0);
        hists.push_back(hist);
        files.push_back(file);
    }

    if (hists.empty()) {
        printf("No histograms found.\n");
        return;
    }

    // Print bin contents for each histogram
    for (size_t h = 0; h < hists.size(); ++h) {
        printf("Histogram %zu: %s\n", h, hists[h]->GetName());
        int nbins = hists[h]->GetNbinsX();
        for (int i = 1; i <= nbins; ++i) {
            double binContent = hists[h]->GetBinContent(i);
            double binLowEdge = hists[h]->GetBinLowEdge(i);
            double binUpEdge = hists[h]->GetBinLowEdge(i+1);
            printf("  Bin %d [%.3f, %.3f): %.6f\n", i, binLowEdge, binUpEdge, binContent);
        }
    }

    // Draw all histograms on the same canvas
    TCanvas* c = new TCanvas("c", "Histograms", 800, 600);
    gStyle->SetOptStat(0);
    static const int rainbowColors[] = {kRed, kOrange+1, kYellow+1, kGreen+2, kCyan+1, kBlue, kViolet, kMagenta, kPink+7};

    for (size_t i = 0; i < hists.size(); ++i) {
        int colorIdx = i % (sizeof(rainbowColors)/sizeof(rainbowColors[0]));
        // Define a rainbow color array (ROOT color indices)
        hists[i]->SetLineColor(rainbowColors[colorIdx]);
        cout << "Drawing histogram: " << i << std::endl;
        if (i == 0){
            hists[i]->Draw("HIST");
            hists[i]->SetTitle("dN/dpT of OO collisions with different corrections");
        }
        else{
            hists[i]->Draw("HIST SAME");
        }
    }
    
    TLegend* leg = new TLegend(0.7, 0.7, 0.9, 0.9);
    for (size_t i = 0; i < hists.size(); ++i){
        leg->AddEntry(hists[i], legendVec[i].c_str(), "l");
        leg->Draw();
    }
    c->SetLogy();
    c->SaveAs("TestHistograms.png");

    // Close files
    for (auto file : files) {
        file->Close();
    }
}
int DrawHist(){
    std::vector<std::string> filenames = {
        "dNdPT_allweights.root",
        "dNdPT_eventonly.root",
        "dNdPT_trkonly.root",
        "dNdPT_trkspeciesonly.root"
    };
    std::vector<std::string> legendVec = {
        "All weights",
        "Event weight only",
        "Track weight only",
        "Track species weight only"
    };

    Draw(filenames, "h_dndpt",legendVec);

    return 0;
}