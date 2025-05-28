#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <iostream>

using namespace std;

int main() {
    const char* skimFile = "/home/xirong/OOAnalysis_2025/EventLevelHistograms.root";
    const char* forestFile = "/data00/kdeverea/skimValidation.root";

    string hist1name = "Run";
    string hist2name = "hnRun";

    // Open ROOT files
    TFile* file1 = TFile::Open(skimFile);
    TFile* file2 = TFile::Open(forestFile);

    if (!file1 || file1->IsZombie()) {
        cerr << "Cannot open file: " << skimFile << endl;
        return 1;
    }
    if (!file2 || file2->IsZombie()) {
        cerr << "Cannot open file: " << forestFile << endl;
        return 1;
    }

    // Get histograms
    TH1F* h1 = (TH1F*)file1->Get(hist1name.c_str());
    TH1D* h2 = (TH1D*)file2->Get(hist2name.c_str());

    if (!h1) {
        std::cerr << "Cannot find histogram " << std::endl;
        return 1;
    }
    if (!h2) {
        std::cerr << "Cannot find histogram " << hist2name <<  std::endl;
        return 1;
    }

    // Draw histograms
    TCanvas* c1 = new TCanvas("c1", "Overlayed Histograms", 800, 600);
    h1->SetLineColor(kRed);
    h2->SetLineColor(kBlue);
    h1->Draw("HIST");
    h2->Draw("HIST SAME");

    // Add legend
    TLegend* legend = new TLegend(0.7,0.7,0.9,0.85);
    legend->AddEntry(h1, (hist1name+"Skim").c_str(), "l");
    legend->AddEntry(h2, (hist1name+"Forest").c_str(), "l");
    legend->Draw();

    c1->SaveAs(("Plots/" + hist1name + "overlay.png").c_str());
    return 0;
}