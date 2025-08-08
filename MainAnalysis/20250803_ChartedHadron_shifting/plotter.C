

#include "MITHIG_CMSStyle.h"

void plotter(){

    // Set CMS style
    SetTDRStyle(800, 600, true, true, true, true, false);
    BuildPalettes();
    
    TFile* fInput = TFile::Open("nene_hfe_output.root");
    
    TH1D* data_hfe = (TH1D*)fInput->Get("data_hfe");
    TH1D* mc_hfe = (TH1D*)fInput->Get("mc_hfe");
    TH1D* shiftedD = (TH1D*)fInput->Get("hShifted");
    TNamed* fitParams = (TNamed*)fInput->Get("fitParams");
    TNamed* fitQuality = (TNamed*)fInput->Get("fitQuality");

    // Set histogram properties
    data_hfe->SetTitle("");
    data_hfe->GetXaxis()->SetTitle("HFE Max AND (GeV)");
    data_hfe->GetYaxis()->SetTitle("A.U.");
    data_hfe->GetXaxis()->SetRangeUser(0, 600);
    data_hfe->SetMaximum(0.5);

    // Use CMS official colors
    data_hfe->SetLineColor(cmsBlack);
    data_hfe->SetMarkerColor(cmsBlack);
    data_hfe->SetMarkerStyle(mCircleFill);
    
    mc_hfe->SetLineColor(cmsGray);
    mc_hfe->SetMarkerColor(cmsGray);
    mc_hfe->SetMarkerStyle(mSquareFill);
    
    shiftedD->SetLineColor(cmsYellow);
    shiftedD->SetMarkerColor(cmsYellow);
    shiftedD->SetMarkerStyle(mTriangleUpFill);

    data_hfe->SetLineWidth(2);
    mc_hfe->SetLineWidth(2);
    shiftedD->SetLineWidth(2);

    // Create canvas with CMS style - taller for ratio plot
    TCanvas* c1 = new TCanvas("c1", "Canvas", 800, 800);
    
    // Create upper pad for main plot
    TPad* pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0.02);
    pad1->SetMargin(marginLeft, marginRight, 0.02, marginTop);
    pad1->SetLogy();
    pad1->SetLogx();
    pad1->SetGrid();
    pad1->Draw();
    
    // Create lower pad for ratio plot
    TPad* pad2 = new TPad("pad2", "pad2", 0, 0.0, 1, 0.3);
    pad2->SetTopMargin(0.02);
    pad2->SetBottomMargin(0.35);
    pad2->SetMargin(marginLeft, marginRight, 0.35, 0.02);
    pad2->SetLogx();
    pad2->SetGrid();
    pad2->Draw();
    
    // Upper plot
    pad1->cd();
    
    // Set fonts for histograms
    SetTH1Fonts(data_hfe);
    SetTH1Fonts(mc_hfe);
    SetTH1Fonts(shiftedD);
    
    // Increase y-axis label font size for upper plot
    data_hfe->GetYaxis()->SetLabelSize(0.05);
    data_hfe->GetYaxis()->SetTitleSize(0.06);
    
    // Remove x-axis title and labels from upper plot
    data_hfe->GetXaxis()->SetLabelSize(0);
    data_hfe->GetXaxis()->SetTitle("");
    
    data_hfe->Draw("lpe");
    mc_hfe->Draw("HIST SAME");
    shiftedD->Draw("HIST SAME");

    // Create legend with CMS style
    TLegend* legend = new TLegend(0.70, 0.75, 0.85, 0.93);
    legend->SetTextFont(42);
    legend->SetTextSize(legendSize);
    legend->SetBorderSize(0);
    legend->SetFillStyle(0);
    
    legend->AddEntry(data_hfe, "Data", "lpe");
    legend->AddEntry(mc_hfe, "HIJING", "l");
    legend->AddEntry(shiftedD, "Shifted HIJING", "l");
    legend->Draw();

    // Add CMS headers
    AddCMSHeader(pad1, "Preliminary");
    AddUPCHeader(pad1, "5.36 TeV", "NeNe 1 nb^{-1}");
    
    // Add fit information
    std::vector<TString> fitInfo;
    
    // Parse and display fit quality info
    if (fitQuality) {
        TString qualityStr = fitQuality->GetTitle();
        // Extract chi2/ndf
        if (qualityStr.Contains("chi2/ndf:")) {
            TString chi2ndf = qualityStr(qualityStr.Index("chi2/ndf:")+9, qualityStr.Index(";", qualityStr.Index("chi2/ndf:"))-qualityStr.Index("chi2/ndf:")-9);
            chi2ndf = chi2ndf.Strip(TString::kBoth);
            fitInfo.push_back(Form("#chi^{2}/ndf = %s", chi2ndf.Data()));
        }
        // Extract fit range
        if (qualityStr.Contains("xmin:") && qualityStr.Contains("xmax:")) {
            TString xmin = qualityStr(qualityStr.Index("xmin:")+5, qualityStr.Index(";", qualityStr.Index("xmin:"))-qualityStr.Index("xmin:")-5);
            TString xmax = qualityStr(qualityStr.Index("xmax:")+5, qualityStr.Length());
            xmin = xmin.Strip(TString::kBoth);
            xmax = xmax.Strip(TString::kBoth);
            fitInfo.push_back(Form("Fit range: %s-%s GeV", xmin.Data(), xmax.Data()));
        }
    }
    
    // Add abbreviated description
    fitInfo.push_back("");
    
    AddPlotLabels(pad1, fitInfo, plotTextSize*0.85, plotTextOffset, 0.22, 0.18);
    
    // Add event selection information
    std::vector<TString> eventSelInfo;
    eventSelInfo.push_back("Event selection:");
    eventSelInfo.push_back("PV + CC Filters, |V_{z}| < 15 cm");
    eventSelInfo.push_back("HLT MinBias (Data only)");
    
    AddPlotLabels(pad1, eventSelInfo, plotTextSize*0.80, plotTextOffset, 0.2, 0.77);
    
    // Create ratio plots
    pad2->cd();
    
    // Create ratio histogram for fitted spectrum only (shifted/data)
    TH1D* ratio_fitted = (TH1D*)shiftedD->Clone("ratio_fitted");
    
    // Calculate ratio (Shifted/Data)
    ratio_fitted->Divide(data_hfe);
    
    // Set up ratio plot styling
    ratio_fitted->SetTitle("");
    ratio_fitted->GetXaxis()->SetTitle("HFE Max AND (GeV)");
    ratio_fitted->GetYaxis()->SetTitle("Ratio");
    ratio_fitted->GetXaxis()->SetRangeUser(0, 600);
    ratio_fitted->SetMinimum(0.5);
    ratio_fitted->SetMaximum(1.5);
    
    // Style the ratio plot in black with lpe
    ratio_fitted->SetLineColor(kBlack);
    ratio_fitted->SetMarkerColor(kBlack);
    ratio_fitted->SetMarkerStyle(mCircleFill);
    ratio_fitted->SetLineWidth(2);
    
    // Adjust font sizes for ratio plot
    ratio_fitted->GetXaxis()->SetTitleSize(0.12);
    ratio_fitted->GetXaxis()->SetLabelSize(0.10);
    ratio_fitted->GetYaxis()->SetTitleSize(0.12);
    ratio_fitted->GetYaxis()->SetLabelSize(0.10);
    ratio_fitted->GetYaxis()->SetTitleOffset(0.6);
    ratio_fitted->GetXaxis()->SetTitleOffset(1.2);
    ratio_fitted->GetYaxis()->SetNdivisions(505);
    
    // Set fonts for ratio histogram
    SetTH1Fonts(ratio_fitted);
    
    // Draw ratio plot
    ratio_fitted->Draw("lpe");
    
    // Add horizontal line at 1.0 in red
    TLine* line = new TLine(0, 1.0, 600, 1.0);
    line->SetLineColor(kRed);
    line->SetLineWidth(2);
    line->SetLineStyle(2);
    line->Draw();
    
    c1->SaveAs("shifted_hfe_comparison.pdf");

    fInput->Close();
}