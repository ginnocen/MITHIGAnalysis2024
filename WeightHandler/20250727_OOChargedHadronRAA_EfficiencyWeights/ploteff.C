#include "MITHIG_CMSStyle.h"

void histfromtree(TTree* T, TCut t, TH1D* h, const char* branch){
     T->Project(h->GetName(), branch, t);
     h->Scale(1.0/h->Integral());
}

void ploteff(){
    
    // Initialize CMS style and build palettes
    SetTDRStyle();
    BuildPalettes();

    TFile* f = TFile::Open("hists/NENERAA_MULT_EFFICIENCY_HIJING_HF13AND.root");
    TH1D* hMultEff = (TH1D*)f->Get("hEff");
    hMultEff->GetXaxis()->SetRangeUser(1.0, 1000); // Set x-axis range    

    // Create canvas with CMS standard dimensions (square)
    TCanvas* c = new TCanvas("c", "Efficiency Plots", squareWidth, squareWidth);
    TPad* pad = (TPad*) c->GetPad(0);
    
    // Style the efficiency histogram using CMS style first
    StyleTH1(pad, hMultEff, "", cmsBlue, mCircleFill);
    
    // Set axis titles and ranges
    hMultEff->GetXaxis()->SetTitle("Multiplicity |#eta| < 2.4");
    hMultEff->GetYaxis()->SetTitle("Efficiency");
    hMultEff->GetXaxis()->SetRangeUser(1.0, 1000);
    hMultEff->GetYaxis()->SetRangeUser(0.0, 1.35);
    
    // Move y-axis title closer to the plot
    hMultEff->GetYaxis()->SetTitleOffset(1.1);
    
    // Reduce axis label and title sizes
    hMultEff->GetXaxis()->SetTitleSize(0.04);
    hMultEff->GetXaxis()->SetLabelSize(0.035);
    hMultEff->GetYaxis()->SetTitleSize(0.04);
    hMultEff->GetYaxis()->SetLabelSize(0.035);
    
    // Reduce marker size
    hMultEff->SetMarkerSize(0.8);
    
    // Set logarithmic x-axis
    pad->SetLogx();
    
    // Force margins directly - try multiple approaches
    c->SetMargin(0.15, 0.05, 0.25, 0.10);  // left, right, bottom, top
    pad->SetMargin(0.15, 0.05, 0.25, 0.10);
    c->cd();
    pad->Draw();
    
    // Draw histogram
    hMultEff->Draw("PE");
    
    // Draw a reference line at efficiency = 1
    TLine* line = new TLine(1.0, 1.0, 1000, 1.0);
    line->SetLineColor(cmsRed);
    line->SetLineStyle(lDash);
    line->SetLineWidth(2);
    line->Draw("SAME");
    
    // Add CMS headers
    AddCMSHeader(pad, "Preliminary");
    AddUPCHeader(pad, "5.36 TeV", "NeNe 1 nb^{-1}");
    
    // Add event selection text
    std::vector<TString> selectionText = {
        "Event Selection:",
        "|V_{z}| < 15 cm",
        "Cluster compatibility filter",
        "PV Filter",
        "HF E_{T} Max > 13 GeV AND"
    };
    AddPlotLabels(pad, selectionText, plotTextSize*0.6, plotTextOffset, 
                  1 - marginRight - 0.45, marginBottom + 0.35);

    
    // Redraw axis on top and save
    gPad->RedrawAxis();
    pad->Update();
    c->SaveAs("hists/efficiency_plots.pdf");


}