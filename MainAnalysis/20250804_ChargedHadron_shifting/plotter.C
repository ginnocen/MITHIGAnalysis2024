

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

    // Create canvas with CMS style
    TCanvas* c1 = new TCanvas("c1", "Canvas", 800, 600);
    TPad* pad = (TPad*)c1->GetPad(0);
    
    // Set pad margins
    pad->SetMargin(marginLeft, marginRight, marginBottom, marginTop);
    
    c1->SetLogy();
    c1->SetLogx();
    c1->SetGrid();
    
    // Set fonts for histograms
    SetTH1Fonts(data_hfe);
    SetTH1Fonts(mc_hfe);
    SetTH1Fonts(shiftedD);
    
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
    AddCMSHeader(pad, "Preliminary");
    AddUPCHeader(pad, "5.36 TeV", "NeNe 1 nb^{-1}");
    
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
    
    AddPlotLabels(pad, fitInfo, plotTextSize*0.85, plotTextOffset, 0.22, 0.25);
    
    // Add event selection information
    std::vector<TString> eventSelInfo;
    eventSelInfo.push_back("Event selection:");
    eventSelInfo.push_back("PV + CC Filters, |V_{z}| < 15 cm");
    eventSelInfo.push_back("HLT MinBias (Data only)");
    
    AddPlotLabels(pad, eventSelInfo, plotTextSize*0.80, plotTextOffset, 0.2, 0.78);
    
    // Add plot label
    //AddPlotLabel(pad, "HFE Max  Distributions");

    c1->SaveAs("shifted_hfe_comparison.pdf");

    fInput->Close();
}