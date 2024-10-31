#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TLegend.h>

void compare() {
    // Open the ROOT files
    TFile *file1 = new TFile("plots/result-DY.root", "READ");
    TFile *file2 = new TFile("plots/result-DY-Gen.root", "READ");

    // Retrieve the TH1D* hProj from each file
    TH1D *hProj1 = (TH1D*)file1->Get("hProj");
    TH1D *hProj2 = (TH1D*)file2->Get("hProj");

    // Check if histograms are loaded
    if (!hProj1 || !hProj2) {
        std::cerr << "Error: Histogram not found in one of the files" << std::endl;
        return;
    }

    // Create a canvas
    TCanvas *c1 = new TCanvas("c1", "Canvas", 800, 800);

    // Draw the histograms
    hProj1->SetLineColor(kBlue);   // Set line color for the first histogram
    hProj1->SetMarkerColor(kBlue);
    

    hProj2->SetLineColor(kRed);    // Set line color for the second histogram
    hProj2->SetMarkerColor(kRed);
    hProj2->Draw();               // Draw the first histogram
    hProj2->SetXTitle("#Delta#phi");
    hProj1->Draw("SAME");         // Draw the second histogram on the same canvas

    // Create a legend
    TLegend *legend = new TLegend(0.2,0.7,0.7,0.9); // Adjust these coordinates as needed
    legend->SetBorderSize(0);
    legend->SetFillStyle(0);
    legend->AddEntry(hProj1, "PYTHIA+HYDJET", "l"); // "l" for line
    legend->AddEntry(hProj2, "DY-Gen", "l"); // "l" for line
    legend->Draw();

    // Optionally: Save the canvas as an image
    // c1->SaveAs("comparison_histogram.png");

    // Cleanup: Close the files
   // file1->Close();
   // file2->Close();
   // delete file1;
   // delete file2;
}

int main() {
    compare();
    return 0;
}
