#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>

void plotResult() {
    // Open the ROOT file
    TFile *file = new TFile("output.root", "READ");

    // Retrieve histograms from the file
    TH2D *hData = (TH2D*)file->Get("hData"); // Replace with your histogram name
    TH2D *hNZData = (TH2D*)file->Get("hNZData"); // Replace with your histogram name
    TH1D *hMixData = (TH1D*)file->Get("hMixData"); // Replace with your histogram name
    TH1D *hNZMixData = (TH1D*)file->Get("hNZMixData"); // Replace with your histogram name

    // Check if histograms are loaded
    if (!hData || !hNZData || !hMixData || !hNZMixData) {
        std::cerr << "Error: Histogram not found in file" << std::endl;
        return;
    }

    // Scale histograms
    hData->Scale(1. / hNZData->GetBinContent(1));
    hMixData->Scale(1. / hNZMixData->GetBinContent(1));

    // Subtract hMixData from hData
    hData->Add(hMixData, -1);

    // Create a canvas to draw the histogram
    TCanvas *c1 = new TCanvas("c1", "Canvas", 800, 600);
    hData->ProjectionY()->Draw();

    // Optionally: Save the canvas as an image
    // c1->SaveAs("output_histogram.png");

    // Cleanup: Close the file
    //file->Close();
    //delete file;
}

int main() {
    plotResult();
    return 0;
}
