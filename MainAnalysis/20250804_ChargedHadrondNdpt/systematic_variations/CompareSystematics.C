#include "../MITHIG_CMSStyle.h"

// DECLARE HELPERS
TH1D* HistFromFile(const char* filename, const char* histname);
void RatioWithCentral(vector<TH1D*> variations, vector<const char*> labels, TH1D* central, const char* xlabel, const char* ylabel, int logx, int logy, float xmin, float xmax, float ymin_ratio, float ymax_ratio, float ymin_spec, float ymax_spec, const char* outputname);
TH1D* Hist_Symmetrized_Errors(TH1D* hUp, TH1D* hCentral, TH1D* hDown);
TH1D* Hist_Total_Systematic(vector<TH1D*> systematics);
void PlotUncerts(vector<TH1D*> varhists, TH1D* tothist, const char* xlabel = "Track p_{T} (GeV/c)", float miny = 0, float maxy = 20);

void CompareSystematics(int doTrack = 1, int doEvtSel = 1, int doSpecies = 1, const char* outfilename = "trust.root"){

    // MAKE SYSTEMATIC HISTS
    vector<TH1D*> hSystematics;
    TH1D* hLooseTrack = nullptr;
    TH1D* hTightTrack = nullptr;
    TH1D* hLooseEsel = nullptr;
    TH1D* hTightEsel = nullptr;
    TH1D* hLooseSpecies = nullptr;
    TH1D* hTightSpecies = nullptr;

    /// HISTOGRAMS TO COMPARE FOR EACH SYSTEMATIC
    TH1D* hCentral = HistFromFile("OOCentralValue/MergedOutput.root", "hTrkPt");    
    
    if(doTrack == 1){
        hLooseTrack = HistFromFile("OOSystLooseTrack/MergedOutput.root", "hTrkPt");
        hTightTrack = HistFromFile("OOSystTightTrack/MergedOutput.root", "hTrkPt");
        if(hLooseTrack && hTightTrack){
            TH1D* hSystematic_Tracks = Hist_Symmetrized_Errors(hTightTrack, hCentral, hLooseTrack);
            hSystematic_Tracks->SetName("hSystematic_Tracks");
            hSystematics.push_back(hSystematic_Tracks);
            RatioWithCentral(
                {hLooseTrack, hTightTrack}, 
                {"Loose Track Selection", "Tight Track Selection"}, 
                hCentral, 
                "Track p_{T} (GeV/c)", 
                "dN/dp_{T} (GeV/c)^{-1}",
                 1, 1, 3, 400, 0.9, 1.1, 1e-3, 1e7, 
                 "TrackWeightSystematic_Comparison.pdf");
        }
    }

    if(doEvtSel == 1){
        hLooseEsel = HistFromFile("OOSystLooseEsel/MergedOutput.root", "hTrkPt");
        hTightEsel = HistFromFile("OOSystTightEsel/MergedOutput.root", "hTrkPt");
        if(hLooseEsel && hTightEsel){
            TH1D* hSystematic_EvtSel = Hist_Symmetrized_Errors(hTightEsel, hCentral, hLooseEsel);
            hSystematic_EvtSel->SetName("hSystematic_EvtSel");
            hSystematics.push_back(hSystematic_EvtSel);
            RatioWithCentral({hLooseEsel, hTightEsel}, 
                {"Loose Event Selection", "Tight Event Selection"}, 
                hCentral,
                 "Track p_{T} (GeV/c)", 
                 "dN/dp_{T} (GeV/c)^{-1}",
                  1, 1, 3, 400, 0.9, 1.1, 1e-3, 1e7, 
                  "EventSelectionSystematic_Comparison.pdf");
        }
    }

    if(doSpecies == 1){
        hLooseSpecies = HistFromFile("OOSystLooseSpecies/MergedOutput.root", "hTrkPt");
        hTightSpecies = HistFromFile("OOSystTightSpecies/MergedOutput.root", "hTrkPt");
        if(hLooseSpecies && hTightSpecies){
            TH1D* hSystematic_Species = Hist_Symmetrized_Errors(hTightSpecies, hCentral, hLooseSpecies);
            hSystematic_Species->SetName("hSystematic_Species");
            hSystematics.push_back(hSystematic_Species);
            RatioWithCentral({hLooseSpecies, hTightSpecies}, 
                {"Loose Species Selection", "Tight Species Selection"}, 
                hCentral, 
                "Track p_{T} (GeV/c)", 
                "dN/dp_{T} (GeV/c)^{-1}",
                 1, 1, 3, 400, 0.9, 1.1, 1e-3, 1e7, 
                 "SpeciesSystematic_Comparison.pdf");
        }
    }

    


    TH1D* hSystematic_total = Hist_Total_Systematic(hSystematics);
    hSystematic_total->SetName("hSystematic_total");
    if(hSystematics.size() > 0){PlotUncerts(hSystematics, hSystematic_total, "Track p_{T} (GeV/c)", 0, 20);}

    // WRITE SYSTEMATICS TO OUTPUT FILE
    TFile* fOutput = new TFile(outfilename, "RECREATE");
    fOutput->cd();
    for(int i = 0; i < hSystematics.size(); i++){
        hSystematics[i]->Write();
    }
    hSystematic_total->Write();
    fOutput->Close();

}


/// HELPER TO GET HIST FROM FILE
TH1D* HistFromFile(const char* filename, const char* histname){
    TFile* f = TFile::Open(filename);
    if(!f || f->IsZombie()){
        std::cerr << "Error opening file: " << filename << std::endl;
        return nullptr;
    }
    TH1D* hist = (TH1D*)f->Get(histname);
    if(!hist){
        std::cerr << "Error retrieving histogram: " << histname << " from file: " << filename << std::endl;
        f->Close();
        return nullptr;
    }
    hist->SetDirectory(0); // Detach histogram from file
    f->Close();
    return hist;
}

/// HELPER TO DO RATIO PLOTS 
void RatioWithCentral(vector<TH1D*> variations, vector<const char*> labels, TH1D* central, const char* xlabel, const char* ylabel, int logx, int logy, float xmin, float xmax, float ymin_ratio, float ymax_ratio, float ymin_spec, float ymax_spec, const char* outputname){

    // Set CMS style
    SetTDRStyle();
    BuildPalettes();
    
    vector<Int_t> colors = {cmsBlue, cmsRed, cmsPaleBlue, cmsPurple, cmsYellow, cmsViolet, cmsTeal};
    
    // Create canvas with two pads
    TCanvas* c1 = new TCanvas("c1", "Systematic Comparison", 800, 800);
    
    // Create upper pad for main plot
    TPad* pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0.02);
    pad1->SetMargin(marginLeft, marginRight, 0.02, marginTop);
    if(logy) pad1->SetLogy();
    if(logx) pad1->SetLogx();
    pad1->SetGrid();
    pad1->Draw();
    
    // Create lower pad for ratio plot
    TPad* pad2 = new TPad("pad2", "pad2", 0, 0.0, 1, 0.3);
    pad2->SetTopMargin(0.02);
    pad2->SetBottomMargin(0.35);
    pad2->SetMargin(marginLeft, marginRight, 0.35, 0.02);
    if(logx) pad2->SetLogx();
    //pad2->SetGridx();
    //pad2->SetGridy();
    pad2->Draw();
    
    // Upper plot - draw spectra
    pad1->cd();
    
    // Clone and normalize central histogram by bin width
    TH1D* centralNorm = (TH1D*)central->Clone("centralNorm");
    for(int i = 1; i <= centralNorm->GetNbinsX(); i++){
        double binWidth = centralNorm->GetBinWidth(i);
        double binContent = centralNorm->GetBinContent(i);
        double binError = centralNorm->GetBinError(i);
        centralNorm->SetBinContent(i, binContent / binWidth);
        centralNorm->SetBinError(i, binError / binWidth);
    }
    
    // Style central histogram
    centralNorm->SetTitle("");
    centralNorm->GetYaxis()->SetTitle(ylabel);
    centralNorm->GetXaxis()->SetRangeUser(xmin, xmax);
    centralNorm->SetMinimum(ymin_spec);
    centralNorm->SetMaximum(ymax_spec);
    centralNorm->SetLineColor(cmsBlack);
    centralNorm->SetMarkerColor(cmsBlack);
    centralNorm->SetMarkerStyle(mCircleFill);
    centralNorm->SetLineWidth(2);
    centralNorm->GetXaxis()->SetLabelSize(0);
    centralNorm->GetXaxis()->SetTitle("");
    centralNorm->GetYaxis()->SetLabelSize(0.05);
    centralNorm->GetYaxis()->SetTitleSize(0.06);
    SetTH1Fonts(centralNorm);
    
    centralNorm->Draw("lpe");
    
    // Clone, normalize and draw variations
    vector<TH1D*> variationsNorm;
    for(int i = 0; i < variations.size(); i++){
        TH1D* varNorm = (TH1D*)variations[i]->Clone(Form("varNorm_%d", i));
        for(int j = 1; j <= varNorm->GetNbinsX(); j++){
            double binWidth = varNorm->GetBinWidth(j);
            double binContent = varNorm->GetBinContent(j);
            double binError = varNorm->GetBinError(j);
            varNorm->SetBinContent(j, binContent / binWidth);
            varNorm->SetBinError(j, binError / binWidth);
        }
        varNorm->SetLineColor(colors[i % colors.size()]);
        varNorm->SetMarkerColor(colors[i % colors.size()]);
        varNorm->SetLineWidth(2);
        varNorm->SetMarkerStyle(mCircleFill);
        varNorm->SetMarkerSize(1.0);
        SetTH1Fonts(varNorm);
        varNorm->Draw("lpe SAME");
        variationsNorm.push_back(varNorm);
    }
    
    // Create legend
    TLegend* legend = new TLegend(0.55, 0.65, 0.80, 0.88);  
    legend->SetTextFont(42);
    legend->SetTextSize(legendSize);
    legend->SetBorderSize(0);
    legend->SetFillStyle(0);
    legend->AddEntry(centralNorm, "Central", "lpe");
    for(int i = 0; i < variationsNorm.size(); i++){
        legend->AddEntry(variationsNorm[i], labels[i], "lpe");
    }
    legend->Draw();
    
    // Add CMS headers
    AddCMSHeader(pad1, "Preliminary");
    AddUPCHeader(pad1, "5.36 TeV", "OO 9.0 nb^{-1}");
    
    // Lower plot - draw ratios
    pad2->cd();
    
    // Create ratios for all variations
    vector<TH1D*> ratios;
    for(int i = 0; i < variations.size(); i++){
        TH1D* ratio = (TH1D*)variations[i]->Clone(Form("ratio_%d", i));
        ratio->Divide(central);
        ratios.push_back(ratio);
    }
    
    // Create invisible frame to set up axes properly
    TH1F* frame = new TH1F("frame", "", 100, xmin, xmax);
    frame->SetMinimum(ymin_ratio);
    frame->SetMaximum(ymax_ratio);
    frame->SetStats(0);
    frame->GetXaxis()->SetTitle(xlabel);
    frame->GetYaxis()->SetTitle("Ratio");
    frame->GetXaxis()->SetTitleSize(0.11);
    frame->GetXaxis()->SetLabelSize(0.11);
    frame->GetYaxis()->SetTitleSize(0.11);
    frame->GetYaxis()->SetLabelSize(0.11);
    frame->GetYaxis()->SetTitleOffset(0.5);
    frame->GetXaxis()->SetTitleOffset(1.2);
    frame->GetYaxis()->SetNdivisions(505);
    //frame->GetXaxis()->SetTitleFont(42);
    //frame->GetXaxis()->SetLabelFont(42);
    //frame->GetYaxis()->SetTitleFont(42);
    //frame->GetYaxis()->SetLabelFont(42);
    
    // Draw invisible frame first
    frame->Draw("AXIS");
    
    // Force grid to be drawn
    pad2->SetGridx();
    pad2->SetGridy();
    gPad->Modified();
    gPad->Update();
    
    // Style and draw all ratios on top
    for(int i = 0; i < ratios.size(); i++){
        ratios[i]->SetLineColor(colors[i % colors.size()]);
        ratios[i]->SetMarkerColor(colors[i % colors.size()]);
        ratios[i]->SetLineWidth(2);
        ratios[i]->SetMarkerStyle(mCircleFill);
        ratios[i]->SetMarkerSize(1.0);
        ratios[i]->Draw("hist p SAME");
    }
    
    // Add horizontal line at 1.0 in red
    TLine* line = new TLine(xmin, 1.0, xmax, 1.0);
    line->SetLineColor(kBlack);
    line->SetLineWidth(2);
    line->SetLineStyle(2);
    line->Draw();
    
    // Save canvas
    c1->SaveAs(outputname);
}

/// HELPER TO GET SYSTEMATIC 
TH1D* Hist_Symmetrized_Errors(TH1D* histhi, TH1D* histcentral, TH1D* histlo){
    TH1D* histSym = (TH1D*)histcentral->Clone("histSym");
    for(int i = 0; i < histSym->GetNbinsX(); i++){
        double centralValue = histcentral->GetBinContent(i+1);
        double highValue = histhi->GetBinContent(i+1);
        double lowValue = histlo->GetBinContent(i+1);
        double error = max(0.0, max(fabs(highValue - centralValue), fabs(centralValue - lowValue)));
        histSym->SetBinError(i+1, error);
    }

    return histSym;
}

/// HELPER TO GET TOTAL SYSTEMATIC
TH1D* Hist_Total_Systematic(vector<TH1D*> varhists){
    TH1D*final = (TH1D*)varhists[0]->Clone("finalSystematic");
    for(int i = 0; i < final->GetNbinsX(); i++){
        double totalError = 0.0;
        for(int j = 0; j < varhists.size(); j++){
            double error = varhists[j]->GetBinError(i+1);
            totalError += error * error;
        }
        final->SetBinError(i+1, sqrt(totalError));
    }
    return final;
}

/// PLOT UNCERTAINTIES
void PlotUncerts(vector<TH1D*> varhists, TH1D* tothist, const char* xlabel = "Track p_{T} (GeV/c)", float miny = 0, float maxy = 20){

    // Set CMS style
    SetTDRStyle();
    BuildPalettes();
    
    vector<Int_t> colors = {cmsBlue, cmsRed, cmsPaleBlue, cmsPurple, cmsYellow, cmsViolet, cmsTeal};
    vector<const char*> labels = {"Track Systematic", "Event Selection Systematic", "Species Systematic"};
    
    // Create canvas
    TCanvas* c1 = new TCanvas("c1", "Systematic Uncertainties", 800, 600);
    c1->SetMargin(marginLeft, marginRight, marginBottom, marginTop);
    c1->SetLogx();
    c1->SetGrid();
    
    // Create histograms to hold the relative errors (%) vs pT
    vector<TH1D*> errorHists;
    for(int i = 0; i < varhists.size(); i++){
        TH1D* errorHist = (TH1D*)varhists[i]->Clone(Form("error_%d", i));
        errorHist->SetTitle("");
        
        // Convert bin errors to relative errors in %
        for(int bin = 1; bin <= errorHist->GetNbinsX(); bin++){
            double binContent = varhists[i]->GetBinContent(bin);
            double binError = varhists[i]->GetBinError(bin);
            double relativeError = (binContent > 0) ? (binError / binContent) * 100.0 : 0.0;
            errorHist->SetBinContent(bin, relativeError);
            errorHist->SetBinError(bin, 0); // No error bars on the uncertainty plot
        }
        
        // Style the histogram
        errorHist->SetLineColor(colors[i % colors.size()]);
        errorHist->SetMarkerColor(colors[i % colors.size()]);
        errorHist->SetMarkerStyle(mCircleFill);
        errorHist->SetLineWidth(2);
        errorHist->SetMarkerSize(1.0);
        SetTH1Fonts(errorHist);
        
        errorHists.push_back(errorHist);
    }

    
    // Create total uncertainty histogram
    TH1D* totalErrorHist = (TH1D*)tothist->Clone("totalError");
    totalErrorHist->SetTitle("");
    
    // Convert total bin errors to relative errors in %
    for(int bin = 1; bin <= totalErrorHist->GetNbinsX(); bin++){
        double binContent = tothist->GetBinContent(bin);
        double binError = tothist->GetBinError(bin);
        double relativeError = (binContent > 0) ? (binError / binContent) * 100.0 : 0.0;
        totalErrorHist->SetBinContent(bin, relativeError);
        totalErrorHist->SetBinError(bin, 0); // No error bars
    }
    
    // Style total uncertainty with thick black line
    totalErrorHist->SetLineColor(cmsBlack);
    totalErrorHist->SetMarkerColor(cmsBlack);
    totalErrorHist->SetMarkerStyle(mSquareFill);
    totalErrorHist->SetLineWidth(3);
    totalErrorHist->SetMarkerSize(1.2);
    SetTH1Fonts(totalErrorHist);
    
    // Set up axes
    totalErrorHist->GetXaxis()->SetTitle(xlabel);
    totalErrorHist->GetYaxis()->SetTitle("Systematic Uncertainty (%)");
    totalErrorHist->GetXaxis()->SetRangeUser(3.0, 400);
    totalErrorHist->SetMinimum(0);
    totalErrorHist->SetMaximum(20); // Adjust as needed
    totalErrorHist->GetYaxis()->SetLabelSize(0.04);
    totalErrorHist->GetYaxis()->SetTitleSize(0.05);
    totalErrorHist->GetXaxis()->SetLabelSize(0.04);
    totalErrorHist->GetXaxis()->SetTitleSize(0.05);
    
    // Draw total uncertainty first (as reference)
    totalErrorHist->Draw("lp");
    
    // Draw individual systematics
    for(int i = 0; i < errorHists.size(); i++){
        errorHists[i]->Draw("lp SAME");
    }
    
    // Create legend
    TLegend* legend = new TLegend(0.55, 0.65, 0.85, 0.88);  
    legend->SetTextFont(42);
    legend->SetTextSize(legendSize);
    legend->SetBorderSize(0);
    legend->SetFillStyle(0);
    legend->AddEntry(totalErrorHist, "Total Systematic", "lp");
    for(int i = 0; i < errorHists.size(); i++){
        const char* label = (i < 3) ? labels[i] : Form("Systematic %d", i+1);
        legend->AddEntry(errorHists[i], label, "lp");
    }
    legend->Draw();
    
    // Add CMS headers
    AddCMSHeader(c1, "Preliminary");
    AddUPCHeader(c1, "5.36 TeV", "OO 9.0 nb^{-1}");
    
    // Save canvas
    c1->SaveAs("SystUncerts.pdf");
}