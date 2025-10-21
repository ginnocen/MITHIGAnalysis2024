#include "../MITHIG_CMSStyle.h"
#include "smoothsystematics.C"

// DECLARE HELPERS
TH1D* HistFromFile(const char* filename, const char* histname);
void RatioWithCentral(vector<TH1D*> variations, vector<const char*> labels, TH1D* central, const char* centlabel, const char* xlabel, const char* ylabel, int logx, int logy, float xmin, float xmax, float ymin_ratio, float ymax_ratio, float ymin_spec, float ymax_spec, const char* system, const char* outputname);
TH1D* Hist_Symmetrized_Errors(TH1D* hUp, TH1D* hCentral, TH1D* hDown, float xignore);
TH1D* Hist_Total_Systematic(vector<TH1D*> systematics);
TH1D* Hist_Smooth_Systematic(TH1D* hist, float lowx);
void PlotUncerts(vector<TH1D*> varhists, TH1D* tothist, const char* xlabel = "Track p_{T} (GeV/c)", float miny = 0, float maxy = 20, const char* system = "OO", const char* pdfname = "SystUncerts.pdf");
TString GenerateFilePath(const char* system, const char* variation);


/// MAIN CODE!
void CompareSystematics(const char* system = "NeNe", int doTrack = 1, int doEvtSel = 1, int doSpecies = 1, const char* outfilename = "nenesystematics_adaptive.root", std::string smoothtype = "adaptive"){
    
    cout << "STARTING SYSTEMATIC COMPARISON" << endl;

    // MAKE SYSTEMATIC HISTS
    vector<TH1D*> hSystematics;
    vector<TH1D*> hSystematics_smooth;  
    TH1D* hLooseTrack = nullptr;
    TH1D* hTightTrack = nullptr;
    TH1D* hSansTrack = nullptr;
    TH1D* hLooseEsel = nullptr;
    TH1D* hTightEsel = nullptr;
    TH1D* hSansEsel = nullptr;
    TH1D* hLooseSpecies = nullptr;
    TH1D* hTightSpecies = nullptr;
    TH1D* hSansSpecies = nullptr;

    /// HISTOGRAMS TO COMPARE FOR EACH SYSTEMATIC
    TH1D* hCentral = HistFromFile(GenerateFilePath(system, "CentralValue"), "hTrkPt");    
    hCentral->SetName("hCentral");

    TH1D* hUncorrected = HistFromFile(GenerateFilePath(system, "Uncorrected"), "hTrkPt");
    hUncorrected->SetName("hUncorrected");


    if(doTrack == 1){
        // TRACK HISTOGRAMS
        cout << "TRACK WEIGHT SYSTEMATICS" << endl;
        hLooseTrack = HistFromFile(GenerateFilePath(system, "SystLooseTrack"), "hTrkPt");
        hTightTrack = HistFromFile(GenerateFilePath(system, "SystTightTrack"), "hTrkPt");
        hSansTrack = HistFromFile(GenerateFilePath(system, "SansTrackWeight"), "hTrkPt");

        if(hLooseTrack && hTightTrack && hSansTrack){

            //GENERATE SYSTEMATIC
            TH1D* hSystematic_Tracks = Hist_Symmetrized_Errors(hTightTrack, hCentral, hLooseTrack, -1); 
            hSystematic_Tracks->SetName("hSystematic_Tracks");
            hSystematics.push_back(hSystematic_Tracks);

            // SMOOTHED SYSTEMATIC
            systfit smoother_tracks(hSystematic_Tracks, 5.0, -1);
            TH1D* hSystematic_Tracks_smooth = nullptr;
            if(smoothtype == "adaptive"){hSystematic_Tracks_smooth = smoother_tracks.relerr_gaussian_smooth_adaptive(1.0);}
            else if(smoothtype == "flat"){hSystematic_Tracks_smooth = smoother_tracks.relerr_gaussian_smooth_flat(10.0);}
            else if(smoothtype == "poly"){hSystematic_Tracks_smooth = smoother_tracks.relerr_polyfit_log(3);}
            else {hSystematic_Tracks_smooth = hSystematic_Tracks;}
            hSystematic_Tracks_smooth->SetName("hSystematic_Tracks_smooth");
            hSystematics_smooth.push_back(hSystematic_Tracks_smooth);

            //PLOTS WITH RATIOS
            RatioWithCentral( 
                {hLooseTrack, hTightTrack}, 
                {"DCA Significance < 5", "DCA Significance < 2"}, 
                hCentral, 
                "Central Value",
                "Track p_{T} (GeV/c)", 
                "dN/dp_{T} (GeV/c)^{-1}",
                 1, 1, 3, 400, 0.9, 1.1, 1e-5, 1e10, 
                 system, "TrackWeightSystematic_Comparison.pdf"
            );
        
            RatioWithCentral(
                {hLooseTrack, hTightTrack, hCentral},
                {"DCA Significance < 5", "DCA Significance < 2", "Central Value"},
                hSansTrack, 
                "No Track Weight Correction",
                "Track p_{T} (GeV/c)",
                "dN/dp_{T} (GeV/c)^{-1}",
                1, 1, 3, 400, 0.85, 1.15, 1e-5, 1e10,
                system, "TrackWeight_compare_no_weights.pdf"
            );
        }
    }

    if(doEvtSel == 1){

        // EVENT SELECTION HISTOGRAMS
        cout << "EVENT SELECTION SYSTEMATICS" << endl;
        hLooseEsel = HistFromFile(GenerateFilePath(system, "SystLooseEsel"), "hTrkPt");
        hTightEsel = HistFromFile(GenerateFilePath(system, "SystTightEsel"), "hTrkPt");
        hSansEsel = HistFromFile(GenerateFilePath(system, "SansEselWeight"), "hTrkPt");

        if(hLooseEsel && hTightEsel && hSansEsel){

            //GENERATE SYSTEMATIC
            TH1D* hSystematic_EvtSel = Hist_Symmetrized_Errors(hTightEsel, hCentral, hLooseEsel, 10);
            hSystematic_EvtSel->SetName("hSystematic_EvtSel");
            hSystematics.push_back(hSystematic_EvtSel);

            // SMOOTHED SYSTEMATIC
            systfit smoother_evtSel(hSystematic_EvtSel, 5.0, 10);
            TH1D* hSystematic_EvtSel_smooth = nullptr;
            if(smoothtype == "adaptive"){hSystematic_EvtSel_smooth = smoother_evtSel.relerr_gaussian_smooth_adaptive(1.0);}
            else if(smoothtype == "flat"){hSystematic_EvtSel_smooth = smoother_evtSel.relerr_gaussian_smooth_flat(10.0);}
            else if(smoothtype == "poly"){hSystematic_EvtSel_smooth = smoother_evtSel.relerr_polyfit_log(3);}
            else {hSystematic_EvtSel_smooth = hSystematic_EvtSel;}
            hSystematic_EvtSel_smooth->SetName("hSystematic_EvtSel_smooth");
            hSystematics_smooth.push_back(hSystematic_EvtSel_smooth);

            //PLOTS WITH RATIOS
            RatioWithCentral(
                {hLooseEsel, hTightEsel}, 
                {"Offline HF AND 10 GeV", "Offline HF AND 19 GeV"}, 
                hCentral,
                "Central Value",
                "Track p_{T} (GeV/c)", 
                "dN/dp_{T} (GeV/c)^{-1}",
                1, 1, 3, 400, 0.9, 1.1, 1e-5, 1e10, 
                system, "EventSelectionSystematic_Comparison.pdf"
            );
            
            RatioWithCentral(
                {hLooseEsel, hTightEsel, hCentral},
                {"Offline HF AND 10 GeV", "Offline HF AND 19 GeV", "Central Value"},
                hSansEsel,
                "No Event Weight Correction",
                "Track p_{T} (GeV/c)",
                "dN/dp_{T} (GeV/c)^{-1}",
                1, 1, 3, 400, 0.85, 1.15, 1e-5, 1e10,
                system, "EventSelection_compare_no_weights.pdf"
            );
        }
    }

    if(doSpecies == 1){

        // SPECIES HISTOGRAMS
        cout << "SPECIES SELECTION SYSTEMATICS" << endl;
        hLooseSpecies = HistFromFile(GenerateFilePath(system, "SystLooseSpecies"), "hTrkPt");
        hTightSpecies = HistFromFile(GenerateFilePath(system, "SystTightSpecies"), "hTrkPt");
        hSansSpecies = HistFromFile(GenerateFilePath(system, "SansSpeciesWeight"), "hTrkPt");

        if(hLooseSpecies && hTightSpecies && hSansSpecies){

            //GENERATE SYSTEMATIC
            TH1D* hSystematic_Species = Hist_Symmetrized_Errors(hTightSpecies, hCentral, hLooseSpecies, -1);
            hSystematic_Species->SetName("hSystematic_Species");
            hSystematics.push_back(hSystematic_Species);

            // SMOOTHED SYSTEMATIC
            systfit smoother_species(hSystematic_Species, 5.0, 20);
            TH1D* hSystematic_Species_smooth = nullptr;
            if(smoothtype == "adaptive"){hSystematic_Species_smooth = smoother_species.relerr_gaussian_smooth_adaptive(1.0);}
            else if(smoothtype == "flat"){hSystematic_Species_smooth = smoother_species.relerr_gaussian_smooth_flat(10.0);}
            else if(smoothtype == "poly"){hSystematic_Species_smooth = smoother_species.relerr_polyfit_log(3);}
            else {hSystematic_Species_smooth = hSystematic_Species;}
            hSystematic_Species_smooth->SetName("hSystematic_Species_smooth");
            hSystematics_smooth.push_back(hSystematic_Species_smooth);

            //PLOTS WITH RATIOS
            RatioWithCentral(
                {hLooseSpecies, hTightSpecies}, 
                {"PPRef Species Selection", "dN/dEta = 100 Species Selection"}, 
                hCentral, 
                "Central Value",
                "Track p_{T} (GeV/c)", 
                "dN/dp_{T} (GeV/c)^{-1}",
                 1, 1, 3, 400, 0.9, 1.1, 1e-5, 1e10, 
                 system, "SpeciesSystematic_Comparison.pdf"
                );

            RatioWithCentral(
                {hLooseSpecies, hTightSpecies, hCentral},
                {"PPRef Species Selection", "dN/dEta = 100 Species Selection", "Central Value"},
                hSansSpecies,
                "No Species Weight Correction",
                "Track p_{T} (GeV/c)",
                "dN/dp_{T} (GeV/c)^{-1}",
                1, 1, 3, 400, 0.85, 1.15, 1e-5, 1e10,
                system, "Species_compare_no_weights.pdf"
            );
        }
    }

    // CALCULATE TOTAL SYSTEMATIC
    cout << "CALCULATING TOTAL SYSTEMATICS" << endl;
    TH1D* hSystematic_total = Hist_Total_Systematic(hSystematics);
    hSystematic_total->SetName("hSystematic_total");
    //hSystematics.push_back(hSystematic_total);

    TH1D* smoothSystematic_total = Hist_Total_Systematic(hSystematics_smooth);
    smoothSystematic_total->SetName("hSystematic_total_smooth");
    //hSystematics_smooth.push_back(smoothSystematic_total);


    // GENERATE SYSTEMATICS PLOT
    if(hSystematics.size() > 0){
        PlotUncerts(hSystematics_smooth, smoothSystematic_total, "Track p_{T} (GeV/c)", 0, 7000, system, "SystUncerts_Smooth.pdf");
        PlotUncerts(hSystematics, hSystematic_total, "Track p_{T} (GeV/c)", 0, 7000, system, "SystUncerts.pdf");
    }

    // WRITE SYSTEMATICS TO OUTPUT FILE
    TFile* fOutput = new TFile(outfilename, "RECREATE");
    fOutput->cd();
    for(int i = 0; i < hSystematics.size(); i++){
        hSystematics[i]->Write();
    }
    hSystematic_total->Write();
    for(int i = 0; i < hSystematics_smooth.size(); i++){
        hSystematics_smooth[i]->Write();
    }
    smoothSystematic_total->Write();
    hCentral->Write();
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
void RatioWithCentral(vector<TH1D*> variations, vector<const char*> labels, TH1D* central, const char* centlabel, const char* xlabel, const char* ylabel, int logx, int logy, float xmin, float xmax, float ymin_ratio, float ymax_ratio, float ymin_spec, float ymax_spec, const char* system, const char* outputname){

    // Check for null central histogram
    if(!central){
        cerr << "Error: Central histogram is null in RatioWithCentral" << endl;
        return;
    }

    // Check for null histograms in variations vector
    for(int i = 0; i < variations.size(); i++){
        if(!variations[i]){
            cerr << "Error: Variation histogram " << i << " is null in RatioWithCentral" << endl;
            return;
        }
    }

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
    
    // Clone central histogram (no bin width normalization)
    TH1D* centralNorm = (TH1D*)central->Clone("centralNorm");
    
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
    
    // Clone and draw variations (no bin width normalization)
    vector<TH1D*> variationsNorm;
    for(int i = 0; i < variations.size(); i++){
        TH1D* varNorm = (TH1D*)variations[i]->Clone(Form("varNorm_%d", i));
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
    legend->AddEntry(centralNorm, centlabel, "lpe");
    for(int i = 0; i < variationsNorm.size(); i++){
        legend->AddEntry(variationsNorm[i], labels[i], "lpe");
    }
    legend->Draw();
    
    // Add CMS headers
    AddCMSHeader(pad1, "Preliminary");
    if(strcmp(system, "OO") == 0){AddUPCHeader(pad1, "5.36 TeV", "OO 9.0 nb^{-1}");}
    if(strcmp(system, "NeNe") == 0){AddUPCHeader(pad1, "5.36 TeV", "NeNe 1.0 nb^{-1}");}

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
TH1D* Hist_Symmetrized_Errors(TH1D* histhi, TH1D* histcentral, TH1D* histlo, float xignore){
    TH1D* histSym = (TH1D*)histcentral->Clone("histSym");
    for(int i = 0; i < histSym->GetNbinsX(); i++){
        double centralValue = histcentral->GetBinContent(i+1);
        double highValue = histhi->GetBinContent(i+1);
        double lowValue = histlo->GetBinContent(i+1);
        double binCenter = histSym->GetBinCenter(i+1);
        double error = max(0.0, max(fabs(highValue - centralValue), fabs(centralValue - lowValue)));
        if(xignore > 0 && binCenter > xignore){error = 0;}
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
void PlotUncerts(vector<TH1D*> varhists, TH1D* tothist, const char* xlabel = "Track p_{T} (GeV/c)", float miny = 0, float maxy = 20, const char* system = "OO", const char* pdfname){

    // Set CMS style
    SetTDRStyle();
    BuildPalettes();
    
    vector<Int_t> colors = {cmsBlue, cmsRed, cmsPaleBlue, cmsPurple, cmsYellow, cmsViolet, cmsTeal};
    vector<const char*> labels = {"Track Systematic", "Event Selection Systematic", "Species Systematic"};
    
    // Create canvas
    TCanvas* c1 = new TCanvas("c1", "Systematic Uncertainties", 800, 600);
    c1->SetMargin(marginLeft, marginRight, marginBottom, marginTop);
    c1->SetLogx();
    c1->SetLogy();
    c1->SetGrid();
    
    // Create TGraphs to hold the relative errors (%) vs pT (excluding zero points)
    vector<TGraph*> errorGraphs;
    for(int i = 0; i < varhists.size(); i++){
        TGraph* errorGraph = new TGraph();
        errorGraph->SetTitle("");
        
        // Add only non-zero relative errors to the graph
        for(int bin = 1; bin <= varhists[i]->GetNbinsX(); bin++){
            double binContent = varhists[i]->GetBinContent(bin);
            double binError = varhists[i]->GetBinError(bin);
            double relativeError = (binContent > 0) ? (binError / binContent) * 100.0 : 0.0;
            
            if(relativeError > 0) { // Only add non-zero points
                double binCenter = varhists[i]->GetBinCenter(bin);
                errorGraph->SetPoint(errorGraph->GetN(), binCenter, relativeError);
            }
        }
        
        // Style the graph
        errorGraph->SetLineColor(colors[i % colors.size()]);
        errorGraph->SetMarkerColor(colors[i % colors.size()]);
        errorGraph->SetMarkerStyle(mCircleFill);
        errorGraph->SetLineWidth(2);
        errorGraph->SetMarkerSize(1.0);
        
        errorGraphs.push_back(errorGraph);
    }

    
    // Create total uncertainty graph (excluding zero points)
    TGraph* totalErrorGraph = new TGraph();
    totalErrorGraph->SetTitle("");
    
    // Add only non-zero relative errors to the total graph
    for(int bin = 1; bin <= tothist->GetNbinsX(); bin++){
        double binContent = tothist->GetBinContent(bin);
        double binError = tothist->GetBinError(bin);
        double relativeError = (binContent > 0) ? (binError / binContent) * 100.0 : 0.0;
        
        if(relativeError > 0) { // Only add non-zero points
            double binCenter = tothist->GetBinCenter(bin);
            totalErrorGraph->SetPoint(totalErrorGraph->GetN(), binCenter, relativeError);
        }
    }
    
    // Style total uncertainty with thick black line
    totalErrorGraph->SetLineColor(cmsBlack);
    totalErrorGraph->SetMarkerColor(cmsBlack);
    totalErrorGraph->SetMarkerStyle(mSquareFill);
    totalErrorGraph->SetLineWidth(3);
    totalErrorGraph->SetMarkerSize(1.2);
    
    // Create a dummy histogram for axis setup
    TH1F* axisFrame = new TH1F("axisFrame", "", 100, 3.0, 400);
    axisFrame->GetXaxis()->SetTitle(xlabel);
    axisFrame->GetYaxis()->SetTitle("Systematic Uncertainty (%)");
    axisFrame->SetMinimum(1e-2);
    axisFrame->SetMaximum(maxy);
    axisFrame->GetYaxis()->SetLabelSize(0.04);
    axisFrame->GetYaxis()->SetTitleSize(0.05);
    axisFrame->GetXaxis()->SetLabelSize(0.04);
    axisFrame->GetXaxis()->SetTitleSize(0.05);
    axisFrame->SetStats(0);
    SetTH1Fonts(axisFrame);
    
    // Draw axis frame first
    axisFrame->Draw("AXIS");
    
    // Draw total uncertainty first (as reference)
    totalErrorGraph->Draw("lp SAME");
    
    // Draw individual systematics
    for(int i = 0; i < errorGraphs.size(); i++){
        errorGraphs[i]->Draw("lp SAME");
    }
    
    // Create legend
    TLegend* legend = new TLegend(0.45, 0.7, 0.85, 0.93);  
    legend->SetTextFont(42);
    legend->SetTextSize(legendSize);
    legend->SetBorderSize(0);
    legend->SetFillStyle(0);
    legend->AddEntry(totalErrorGraph, "Total Systematic", "lp");
    for(int i = 0; i < errorGraphs.size(); i++){
        const char* label = (i < 3) ? labels[i] : Form("Systematic %d", i+1);
        //if(i == errorGraphs.size() - 1) label = "Total Systematic (unsmoothed)";
        legend->AddEntry(errorGraphs[i], label, "lp");
    }
    legend->Draw();
    
    // Add CMS headers
    AddCMSHeader(c1, "Preliminary");
    if(strcmp(system, "OO") == 0){AddUPCHeader(c1, "5.36 TeV", "OO 9.0 nb^{-1}");}
    else if(strcmp(system, "NeNe") == 0){AddUPCHeader(c1, "5.36 TeV", "NeNe 1.0 nb^{-1}");}

    // Save canvas
    c1->SaveAs(pdfname);
}

/// HELPER TO GENERATE FILE PATHS
TString GenerateFilePath(const char* system, const char* variation){
    TString filepath = Form("%s%s/MergedOutput.root", system, variation);
    cout << filepath << endl;
    return filepath;
}