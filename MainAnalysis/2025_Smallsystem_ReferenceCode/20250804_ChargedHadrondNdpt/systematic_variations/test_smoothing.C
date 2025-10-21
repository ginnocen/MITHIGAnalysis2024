#include "CompareSystematics.C"

double chi2NDF_errors(TH1D* h1, TH1D* h2, double xmin = 1.0, double xmax = 400.0);
void PlotSystematics(TH1D* h1, TH1D* h2, const char* label1, const char* label2, const char* outfilename, 
                     double xmin = 1.0, double xmax = 400.0, double ymin = 0.01, double ymax = 1000.0);


// START TEST SMOOTHING                     
void test_smoothing(){

    // INITIALIZE HISTOGRAMS
    TH1D* hCentral = nullptr;
    TH1D* hUncorrected = nullptr;

    TH1D* hLooseTrack = nullptr;
    TH1D* hTightTrack = nullptr;
    TH1D* hSansTrack = nullptr;

    TH1D* hLooseEsel = nullptr;
    TH1D* hTightEsel = nullptr;
    TH1D* hSansEsel = nullptr;

    TH1D* hLooseSpecies = nullptr;
    TH1D* hTightSpecies = nullptr;
    TH1D* hSansSpecies = nullptr;

    const char* system = "NeNe"; 

    // CENTRAL HISTOGRAM
    hCentral = HistFromFile(GenerateFilePath(system, "CentralValue"), "hTrkPt");    
    hCentral->SetName("hCentral");
    hUncorrected = HistFromFile(GenerateFilePath(system, "Uncorrected"), "hTrkPt");
    hUncorrected->SetName("hUncorrected");

    // TRACK HISTOGRAMS
    hLooseTrack = HistFromFile(GenerateFilePath(system, "SystLooseTrack"), "hTrkPt");
    hTightTrack = HistFromFile(GenerateFilePath(system, "SystTightTrack"), "hTrkPt");
    hSansTrack = HistFromFile(GenerateFilePath(system, "SansTrackWeight"), "hTrkPt");

    //SPECIES HISTOGRAMS
    hLooseSpecies = HistFromFile(GenerateFilePath(system, "SystLooseSpecies"), "hTrkPt");
    hTightSpecies = HistFromFile(GenerateFilePath(system, "SystTightSpecies"), "hTrkPt");
    hSansSpecies = HistFromFile(GenerateFilePath(system, "SansSpeciesWeight"), "hTrkPt");

    // EVENT SELECTION HISTOGRAMS
    hLooseEsel = HistFromFile(GenerateFilePath(system, "SystLooseEsel"), "hTrkPt");
    hTightEsel = HistFromFile(GenerateFilePath(system, "SystTightEsel"), "hTrkPt");
    hSansEsel = HistFromFile(GenerateFilePath(system, "SansEselWeight"), "hTrkPt");

    // CONSTRUCT SYSTEMATIC HISTOGRAMS - TRACKS 
    TH1D* hSystematic_Tracks = Hist_Symmetrized_Errors(hTightTrack, hCentral, hLooseTrack, -1);
    TH1D* hSystematic_EvtSel = Hist_Symmetrized_Errors(hTightEsel, hCentral, hLooseEsel, -1);
    TH1D* hSystematic_Species = Hist_Symmetrized_Errors(hTightSpecies, hCentral, hLooseSpecies, -1);


    // CONSTRUCT THE SMOOTHED CLASSES   
    systfit smoother_tracks(hSystematic_Tracks, 5.0, -1); // START SMOOTHING FOR POINTS AFTER 15 GEV <- CAN TOGGLE THIS
    systfit smoother_evtSel(hSystematic_EvtSel, 5.0, -1);
    systfit smoother_species(hSystematic_Species, 5.0, -1);

    // SMOOTH GAUSSIAN ADAPTIVE
    TH1D* hSystematic_Tracks_adaptive = smoother_tracks.relerr_gaussian_smooth_adaptive(1.0);
    TH1D* hSystematic_EvtSel_adaptive = smoother_evtSel.relerr_gaussian_smooth_adaptive(1.0);
    TH1D* hSystematic_Species_adaptive = smoother_species.relerr_gaussian_smooth_adaptive(1.0);

    // SMOOTH GAUSSIAN FLAT
    TH1D* hSystematic_Tracks_flat = smoother_tracks.relerr_gaussian_smooth_flat(10.0);
    TH1D* hSystematic_EvtSel_flat = smoother_evtSel.relerr_gaussian_smooth_flat(10.0);
    TH1D* hSystematic_Species_flat = smoother_species.relerr_gaussian_smooth_flat(10.0);

        // POLYFIT LOG 
    TH1D* hSystematic_Tracks_polylog = smoother_tracks.relerr_polyfit_log(3);
    TH1D* hSystematic_EvtSel_polylog = smoother_evtSel.relerr_polyfit_log(3);
    TH1D* hSystematic_Species_polylog = smoother_species.relerr_polyfit_log(3);

    ////////////////////////////////////
    ///                              ///
    ///      TRACK SYSTEMATICS       ///
    ///                              ///
    ////////////////////////////////////

    cout << "\n=== PLOTTING SYSTEMATIC COMPARISONS - TRACKS ===" << endl;

    cout << "\n--- Tracks: Flat Gaussian Smoothing vs Unsmoothed ---" << endl;
    double chi2_tracks_flat = chi2NDF_errors(hSystematic_Tracks, hSystematic_Tracks_flat, 3.0, 400.0);
    PlotSystematics(hSystematic_Tracks, hSystematic_Tracks_flat, "Tracks unsmoothed", "Tracks gaussian flat", "smoothing_comp/Tracks_flat_Comparison.pdf", 3.0, 400.0, 1, 1000.0);
    
    cout << "\n--- Tracks: Adaptive Gaussian Smoothing vs Unsmoothed ---" << endl;
    double chi2_tracks_adaptive = chi2NDF_errors(hSystematic_Tracks, hSystematic_Tracks_adaptive, 3.0, 400.0);
    PlotSystematics(hSystematic_Tracks, hSystematic_Tracks_adaptive, "Tracks unsmoothed", "Tracks gaussian adaptive", "smoothing_comp/Tracks_adaptive_Comparison.pdf", 3.0, 400.0, 1, 1000.0);

    cout << "\n--- Tracks: Polynomial Log Smoothing vs Unsmoothed ---" << endl;
    double chi2_tracks_polylog = chi2NDF_errors(hSystematic_Tracks, hSystematic_Tracks_polylog, 3.0, 400.0);
    PlotSystematics(hSystematic_Tracks, hSystematic_Tracks_polylog, "Tracks unsmoothed", "Tracks polylog", "smoothing_comp/Tracks_polylog_Comparison.pdf", 3.0, 400.0, 1, 1000.0);

    cout << "\n=== CHI2/NDF SUMMARY ===" << endl;
    cout << "Tracks Flat vs Unsmoothed: Chi2/NDF = " << chi2_tracks_flat << endl;
    cout << "Tracks Adaptive vs Unsmoothed: Chi2/NDF = " << chi2_tracks_adaptive << endl;
    cout << "Tracks Polylog vs Unsmoothed: Chi2/NDF = " << chi2_tracks_polylog << endl;

    ////////////////////////////////////
    ///                              ///
    ///       ESEL SYSTEMATICS       ///
    ///                              ///
    ////////////////////////////////////

    cout << "\n=== PLOTTING SYSTEMATIC COMPARISONS - EVENT SELECTION ===" << endl;

    cout << "\n--- EventSel: Flat Gaussian Smoothing vs Unsmoothed ---" << endl;
    double chi2_events_flat = chi2NDF_errors(hSystematic_EvtSel, hSystematic_EvtSel_flat, 3.0, 400.0);
    PlotSystematics(hSystematic_EvtSel, hSystematic_EvtSel_flat, "EventSel unsmoothed", "EventSel gaussian flat", "smoothing_comp/EventSel_flat_Comparison.pdf", 3.0, 400.0, 0.001, 1000.0);

    cout << "\n--- EventSel: Adaptive Gaussian Smoothing vs Unsmoothed ---" << endl;
    double chi2_events_adaptive = chi2NDF_errors(hSystematic_EvtSel, hSystematic_EvtSel_adaptive, 3.0, 400.0);
    PlotSystematics(hSystematic_EvtSel, hSystematic_EvtSel_adaptive, "EventSel unsmoothed", "EventSel gaussian adaptive", "smoothing_comp/EventSel_adaptive_Comparison.pdf", 3.0, 400.0, 0.001, 1000.0);

    cout << "\n--- EventSel: Polynomial Log Smoothing vs Unsmoothed ---" << endl;
    double chi2_events_polylog = chi2NDF_errors(hSystematic_EvtSel, hSystematic_EvtSel_polylog, 3.0, 400.0);
    PlotSystematics(hSystematic_EvtSel, hSystematic_EvtSel_polylog, "EventSel unsmoothed", "EventSel polylog", "smoothing_comp/EventSel_polylog_Comparison.pdf", 3.0, 400.0, 0.001, 1000.0);

    cout << "\n=== CHI2/NDF SUMMARY ===" << endl;
    cout << "EventSel Flat vs Unsmoothed: Chi2/NDF = " << chi2_events_flat << endl;
    cout << "EventSel Adaptive vs Unsmoothed: Chi2/NDF = " << chi2_events_adaptive << endl;
    cout << "EventSel Polylog vs Unsmoothed: Chi2/NDF = " << chi2_events_polylog << endl;


    ////////////////////////////////////
    ///                              ///
    ///    SPECIES SYSTEMATICS       ///
    ///                              ///
    ////////////////////////////////////

    cout << "\n=== PLOTTING SYSTEMATIC COMPARISONS - SPECIES SELECTION ===" << endl;

    cout << "\n--- Species: Flat Gaussian Smoothing vs Unsmoothed ---" << endl;
    double chi2_species_flat = chi2NDF_errors(hSystematic_Species, hSystematic_Species_flat, 3.0, 400.0);
    PlotSystematics(hSystematic_Species, hSystematic_Species_flat, "Species unsmoothed", "Species gaussian flat", "smoothing_comp/Species_flat_Comparison.pdf", 3.0, 400.0, 0.01, 1000.0);

    cout << "\n--- Species: Adaptive Gaussian Smoothing vs Unsmoothed ---" << endl;
    double chi2_species_adaptive = chi2NDF_errors(hSystematic_Species, hSystematic_Species_adaptive, 3.0, 400.0);
    PlotSystematics(hSystematic_Species, hSystematic_Species_adaptive, "Species unsmoothed", "Species gaussian adaptive", "smoothing_comp/Species_adaptive_Comparison.pdf", 3.0, 400.0, 0.01, 1000.0);

    cout << "\n--- Species: Polynomial Log Smoothing vs Unsmoothed ---" << endl;
    double chi2_species_polylog = chi2NDF_errors(hSystematic_Species, hSystematic_Species_polylog, 3.0, 400.0);
    PlotSystematics(hSystematic_Species, hSystematic_Species_polylog, "Species unsmoothed", "Species polylog", "smoothing_comp/Species_polylog_Comparison.pdf", 3.0, 400.0, 0.01, 1000.0);

    cout << "\n=== CHI2/NDF SUMMARY ===" << endl;
    cout << "Species Flat vs Unsmoothed: Chi2/NDF = " << chi2_species_flat << endl;
    cout << "Species Adaptive vs Unsmoothed: Chi2/NDF = " << chi2_species_adaptive << endl;
    cout << "Species Polylog vs Unsmoothed: Chi2/NDF = " << chi2_species_polylog << endl;

}

double chi2NDF_errors(TH1D* h1, TH1D* h2, double xmin = 1.0, double xmax = 400.0) {
    
    double chi2 = 0.0;
    int ndf = 0;

    cout << "Computing chi2 between " << h1->GetName() << " and " << h2->GetName() << endl;
    
    // BIN LOOP
    for(int i = 1; i <= h1->GetNbinsX(); i++){
        double binCenter = h1->GetBinCenter(i);
        if(binCenter < xmin || binCenter > xmax) continue;
        
        double val1 = h1->GetBinContent(i);
        double val2 = h2->GetBinContent(i);
        double error1 = h1->GetBinError(i);
        double error2 = h2->GetBinError(i);

        if(val1 > 0 && val2 > 0 && error1 > 0 && error2 > 0) {
            // Convert to percentage for consistency with plotting
            double relErr1 = (error1 / val1) * 100.0;  // Percentage
            double relErr2 = (error2 / val2) * 100.0;  // Percentage
            double diff = relErr1 - relErr2;
            
            chi2 += diff * diff;
            ndf++;
            
            // Debug output for first few bins
            if(ndf <= 3) {
                cout << "Bin " << i << " (pT=" << binCenter << "): " 
                     << relErr1 << "% vs " << relErr2 << "%, diff=" << diff 
                     << "%, diff^2=" << diff*diff << endl;
            }
        }
    }
    
    if(ndf == 0) {
        cout << "no bins buddy" << endl;
        return -1.0;
    }
    
    cout << "Total chi2=" << chi2 << ", ndf=" << ndf << ", chi2/ndf=" << chi2/ndf << endl;
    return chi2 / ndf;
}

void PlotSystematics(TH1D* h1, TH1D* h2, const char* label1, const char* label2, const char* outfilename, 
                     double xmin = 1.0, double xmax = 400.0, double ymin = 0.01, double ymax = 1000.0){

    // Create relative error graphs for both histograms
    TGraph* relErr1 = new TGraph();
    TGraph* relErr2 = new TGraph();
    
    // CALCULATE RELATIVE ERRORS HIST1
    int nPoints1 = 0;
    for(int i = 1; i <= h1->GetNbinsX(); i++){
        double binCenter = h1->GetBinCenter(i);
        double binContent = h1->GetBinContent(i);
        double binError = h1->GetBinError(i);
        
        if(binContent > 0 && binError > 0) {
            double relativeError = (binError / binContent) * 100.0; // PERCENTAGE
            relErr1->SetPoint(nPoints1, binCenter, relativeError);
            nPoints1++;
        }
    }
    
    // CALCULATE RELATIVE ERRORS HIST2
    int nPoints2 = 0;
    for(int i = 1; i <= h2->GetNbinsX(); i++){
        double binCenter = h2->GetBinCenter(i);
        double binContent = h2->GetBinContent(i);
        double binError = h2->GetBinError(i);
        
        if(binContent > 0 && binError > 0) {
            double relativeError = (binError / binContent) * 100.0; // PERCENTAGE
            relErr2->SetPoint(nPoints2, binCenter, relativeError);
            nPoints2++;
        }
    }
    
    // PLOT
    TCanvas* c1 = new TCanvas("c_systematic_overlay", "Systematic Errors Comparison", 800, 600);
    c1->SetLogx();
    c1->SetLogy();
    c1->SetGrid();
    
    // Style the graphs
    relErr1->SetMarkerStyle(20);
    relErr1->SetMarkerSize(0.8);
    relErr1->SetMarkerColor(kBlue);
    relErr1->SetLineColor(kBlue);
    relErr1->SetLineWidth(2);
    
    relErr2->SetMarkerStyle(21);
    relErr2->SetMarkerSize(0.8);
    relErr2->SetMarkerColor(kRed);
    relErr2->SetLineColor(kRed);
    relErr2->SetLineWidth(2);
    
    // Draw first graph
    if(nPoints1 > 0) {
        relErr1->Draw("APL");
        relErr1->SetTitle("Systematic Errors Comparison;p_{T} [GeV];Relative Error [%]");
        relErr1->GetXaxis()->SetRangeUser(xmin, xmax);
        relErr1->GetYaxis()->SetRangeUser(ymin, ymax);
    }
    
    // Draw second graph on same canvas
    if(nPoints2 > 0) {
        relErr2->Draw("PL SAME");
    }
    
    // Add legend
    TLegend* leg = new TLegend(0.65, 0.75, 0.9, 0.9);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->SetTextSize(0.03);
    if(nPoints1 > 0) leg->AddEntry(relErr1, label1, "pl");
    if(nPoints2 > 0) leg->AddEntry(relErr2, label2, "pl");
    leg->Draw();

    cout << "Plotted " << nPoints1 << " points for " << label1 << endl;
    cout << "Plotted " << nPoints2 << " points for " << label2 << endl;

    c1->SaveAs(outfilename);
}