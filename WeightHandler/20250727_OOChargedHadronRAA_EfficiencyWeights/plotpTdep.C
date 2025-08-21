#include "MITHIG_CMSStyle.h"

void histfromtree(TTree* T, TCut t, TH1D* h, const char* branch){
     T->Project(h->GetName(), branch, t);
     h->Scale(1.0/h->Integral());
}

void plotpTdep(){

     // Initialize CMS style and build palettes
    SetTDRStyle();
    BuildPalettes();
    gStyle->SetOptStat(0);

    TFile* f = TFile::Open("hists/nene_output_13.root");
    TH1D* hCorrected = (TH1D*)f->Get("hCorrected");
    TH1D* hUncorrected = (TH1D*)f->Get("hUncorrected");
    TH1D* hRatio = (TH1D*)f->Get("hRatio");
    
    // Debug: Check if histograms exist and have content
    if (!hCorrected) { cout << "ERROR: hCorrected not found!" << endl; return; }
    if (!hUncorrected) { cout << "ERROR: hUncorrected not found!" << endl; return; }
    if (!hRatio) { cout << "ERROR: hRatio not found!" << endl; return; }
    
    cout << "Corrected entries: " << hCorrected->GetEntries() << endl;
    cout << "Uncorrected entries: " << hUncorrected->GetEntries() << endl;
    
    // Create CMS-style canvas with ratio plot
    TCanvas* c1 = new TCanvas("c1", "Track pT Spectra with Ratio", squareWidth*1.2, squareHeight*1.3);
    
    // Top pad for main plot (70% of canvas)
    TPad* pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1);
    pad1->SetMargin(marginLeft, marginRight, 0.02, marginTop);
    pad1->SetLogy();
    pad1->SetLogx();
    pad1->Draw();
    
    // Bottom pad for ratio plot (30% of canvas)
    TPad* pad2 = new TPad("pad2", "pad2", 0, 0, 1, 0.3);
    pad2->SetMargin(marginLeft, marginRight, 0.35, 0.02);
    pad2->SetLogx();
    pad2->Draw();
    
    // Draw main spectra plot
    pad1->cd();
    
    // JUST DRAW BOTH HISTOGRAMS - WITH CMS STYLING
    cout << "Drawing uncorrected in GREY (large, transparent)..." << endl;
    hUncorrected->SetLineColor(kGray+1);
    hUncorrected->SetMarkerColor(kGray+1);
    hUncorrected->SetMarkerStyle(20);
    hUncorrected->SetMarkerSize(1.3);
    hUncorrected->SetMarkerColorAlpha(kGray+1, 0.6); // Make transparent
    hUncorrected->SetTitle("");
    hUncorrected->GetYaxis()->SetTitle("dN/dp_{T} (GeV/c)^{-1}");
    hUncorrected->GetXaxis()->SetLabelSize(0); // Hide x-axis labels for top plot
    
    // Get y-axis range and boost by factor of 100
    double yMin = TMath::Min(hUncorrected->GetMinimum(0), hCorrected->GetMinimum(0));
    double yMax = TMath::Max(hUncorrected->GetMaximum(), hCorrected->GetMaximum());
    if (yMin <= 0) yMin = 1e-10;
    hUncorrected->GetYaxis()->SetRangeUser(yMin * 0.1, yMax * 100);
    
    SetTH1Fonts(hUncorrected, 1.0);
    hUncorrected->Draw("PE");
    
    cout << "Drawing corrected in BRIGHT RED (small, on top)..." << endl;
    hCorrected->SetLineColor(cmsRed);
    hCorrected->SetMarkerColor(cmsRed);
    hCorrected->SetMarkerStyle(20);
    hCorrected->SetMarkerSize(0.8);
    SetTH1Fonts(hCorrected, 1.0);
    hCorrected->Draw("PE SAME");
    
    cout << "DONE! Both should be visible now." << endl;
    
    // CMS-style legend
    TLegend* leg = new TLegend(0.55, 0.70, 0.88, 0.85);
    leg->SetTextFont(42);
    leg->SetTextSize(legendSize);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->AddEntry(hCorrected, "Efficiency Corrected", "pe");
    leg->AddEntry(hUncorrected, "Uncorrected", "pe");
    leg->Draw();
    
    // Add CMS headers and collision info
    AddCMSHeader(pad1, "Preliminary");
    AddUPCHeader(pad1, "5.36 TeV", "NeNe 1 nb^{-1}");
    
    // Add physics process label
    AddPlotLabel(pad1, "Track p_{T} Spectra");
    
    // Add event selection text
    std::vector<TString> selectionText = {
        "Event Selection:",
        "|V_{z}| < 15 cm",
        "Cluster compatibility filter", 
        "PV Filter",
        "HF E_{T} Max > 13 GeV"
    };
    AddPlotLabels(pad1, selectionText, plotTextSize*0.7, plotTextOffset, 
                  1 - marginRight - 0.30, 1 - marginTop - 0.35);
    
    // Redraw axis for clean appearance
    gPad->RedrawAxis();
    
    // Draw ratio plot
    pad2->cd();
    SetTH1Fonts(hRatio, 1.0);
    
    hRatio->SetLineColor(cmsBlack);
    hRatio->SetMarkerColor(cmsBlack);
    hRatio->SetLineWidth(2);
    hRatio->SetMarkerStyle(20);
    hRatio->SetMarkerSize(0.8);
    hRatio->SetTitle(""); // No title for ratio plot
    
    // Set axis properties for ratio plot
    hRatio->GetYaxis()->SetTitle("Corrected/Uncorrected");
    hRatio->GetYaxis()->SetTitleSize(axisTitleSize * 1.4);
    hRatio->GetYaxis()->SetLabelSize(axisLabelSize * 1.4);
    hRatio->GetYaxis()->SetTitleOffset(axisTitleOffsetY * 0.7);
    hRatio->GetYaxis()->SetNdivisions(505);
    hRatio->GetYaxis()->SetRangeUser(0.95, 1.05); // Set y range
    
    hRatio->GetXaxis()->SetTitle("Track p_{T} (GeV/c)");
    hRatio->GetXaxis()->SetTitleSize(axisTitleSize * 1.4);
    hRatio->GetXaxis()->SetLabelSize(axisLabelSize * 1.4);
    hRatio->GetXaxis()->SetTitleOffset(axisTitleOffsetX * 0.9);
    hRatio->GetXaxis()->SetRangeUser(0.4, 400);
    
    // Draw ratio with error bars
    hRatio->Draw("PE");
    
    // Add reference line at y=1 (red dotted line) - only from 3 to 400 GeV
    TLine* line = new TLine(3.0, 1.0, 400, 1.0);
    line->SetLineColor(cmsRed);
    line->SetLineStyle(2); // Dashed line
    line->SetLineWidth(2);
    line->Draw("same");
    
    // Redraw axis for clean appearance
    gPad->RedrawAxis();
    
    // Save with CMS styling
    c1->SaveAs("TrackPtSpectra_WithRatio_CMS.pdf");


}