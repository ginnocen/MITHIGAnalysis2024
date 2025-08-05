#include <iostream>
#include <string>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TGraph.h>
#include <TLegend.h>
#include <TTreeFormula.h>

void SaveHists_allweights_evtbyevt(
    const std::string& inputFileNameMC,
    const std::string& branchName,
    const std::string& ReweightFile,
    const std::string& cut = "",
    int nbins = 100,
    double xmin = NAN,
    double xmax = NAN,
    const std::string& outfilename = "",
    bool normalize = false,
    bool VZweight = true,
    bool Multweight = true)
{

    TFile* f_reweight = TFile::Open(ReweightFile.c_str());
    TFile* f_mc = TFile::Open(inputFileNameMC.c_str());

    if (!f_reweight || f_reweight->IsZombie() ||
        !f_mc || f_mc->IsZombie()) {
        std::cerr << "Cannot open one or more files!" << std::endl;
        return;
    }

    // Get weight histo
    TH1D* hVzWeight = (TH1D*) f_reweight->Get("VZReweight");
    TH1D* hMultWeight = (TH1D*) f_reweight->Get("MultReweight");
    if (!hVzWeight || !hMultWeight) {
        std::cerr << "Could not get reweight histogram!" << std::endl;
        return;
    }

    // Use MC tree for event loop
    TTree* tree = (TTree*) f_mc->Get("Tree"); // update to correct name
    if (!tree) {
        std::cerr << "Tree not found in MC file!" << std::endl;
        return;
    }

    float vz = 0;
    int mult = 0;
    tree->SetBranchAddress("VZ", &vz);
    tree->SetBranchAddress("multiplicityEta2p4", &mult);

    /// Set y and x
    tree->Draw((branchName + ">>htempDATA(10000)").c_str(), cut.c_str(), "goff");
    TH1D* htempDATA = (TH1D*)gDirectory->Get("htempDATA");
    if (std::isnan(xmin)) xmin = htempDATA->GetXaxis()->GetXmin();
    if (std::isnan(xmax)) xmax = htempDATA->GetXaxis()->GetXmax();
    delete htempDATA;

    // Prepare histograms
    std::string h_noWeight_name = "h_noWeight_" + branchName;
    std::string h_weighted_name = "h_weighted_" + branchName;

    const Int_t nMultBins = 39;
    const Double_t multBins[nMultBins + 1] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 
        12, 14, 16, 18, 20, 24, 28, 32, 36, 
        40, 45, 50, 55, 60, 70, 80, 100,
        120, 150, 200, 250, 300, 400, 500, 600, 700, 800, 900, 1000
    };

    const Int_t nPtBins_log = 26;
    const Double_t pTBins_log[nPtBins_log + 1] = {
        0.5, 0.603, 0.728, 0.879, 1.062, 1.284, 1.553, 1.878, 2.272, 2.749, 3.327, 4.027, 
        4.872, 5.891, 7.117, 8.591, 10.36, 12.48, 15.03, 18.08, 21.73, 26.08, 31.28, 
        37.48, 44.89, 53.73, 64.31
    };


    TH1D* h_noWeight = nullptr;
    TH1D* h_weighted = nullptr;

    std::string hname_data = "histDATA_" + branchName;
    if (branchName.find("multiplicity") != std::string::npos) {
        h_noWeight = new TH1D(h_noWeight_name.c_str(), (branchName + " (no weight)").c_str(), nMultBins, multBins);
        h_weighted = new TH1D(h_weighted_name.c_str(), (branchName + " (weighted)").c_str(), nMultBins, multBins);
    } else if (branchName == "trkPt") {
        h_noWeight = new TH1D(h_noWeight_name.c_str(), (branchName + " (no weight)").c_str(), nPtBins_log, pTBins_log);
        h_weighted = new TH1D(h_weighted_name.c_str(), (branchName + " (weighted)").c_str(), nPtBins_log, pTBins_log);
    } else if (branchName == "VZ"){
        xmin = -30;
        xmax = 30;
        nbins = 50;
        h_noWeight = new TH1D(h_noWeight_name.c_str(), (branchName + " (no weight)").c_str(), nbins, xmin, xmax);
        h_weighted = new TH1D(h_weighted_name.c_str(), (branchName + " (weighted)").c_str(), nbins, xmin, xmax);
    } else {
        h_noWeight = new TH1D(h_noWeight_name.c_str(), (branchName + " (no weight)").c_str(), nbins, xmin, xmax);
        h_weighted = new TH1D(h_weighted_name.c_str(), (branchName + " (weighted)").c_str(), nbins, xmin, xmax);
    }
    
    // Use TLeaf to generically access scalar branch
    TLeaf* leaf = tree->FindLeaf(branchName.c_str());
    if (!leaf) {
        std::cerr << "Branch " << branchName << " not found!" << std::endl;
        return;
    }

    // Add TTreeFormula for cut
    TTreeFormula* formula = nullptr;
    if (!cut.empty()) {
        formula = new TTreeFormula("cutFormula", cut.c_str(), tree);
    }
    
    Long64_t nEntries = tree->GetEntries();
    for (Long64_t i = 0; i < nEntries; ++i) {
        tree->GetEntry(i);
        // Apply cut if specified
        if (formula && !formula->EvalInstance()) continue;

        int bin_vz = hVzWeight->FindBin(vz);
        int bin_mult = hMultWeight->FindBin(mult);

        bin_vz = std::clamp(bin_vz, 1, hVzWeight->GetNbinsX());
        bin_mult = std::clamp(bin_mult, 1, hMultWeight->GetNbinsX());

        float vzWeight = hVzWeight->GetBinContent(bin_vz);
        float multWeight = hMultWeight->GetBinContent(bin_mult);

        if (!VZweight) vzWeight = 1.0;
        if (!Multweight) multWeight = 1.0;

        float totalWeight = vzWeight * multWeight;
        float value = leaf->GetValue();

        h_noWeight->Fill(value);
        h_weighted->Fill(value, totalWeight);
    }

    //Normalize by bin width
    for (int i = 1; i <= h_noWeight->GetNbinsX(); ++i) {
        double width = h_noWeight->GetBinWidth(i);
        if (width > 0) {
            h_noWeight->SetBinContent(i, h_noWeight->GetBinContent(i) / width);
            h_noWeight->SetBinError(i, h_noWeight->GetBinError(i) / width);
            h_weighted->SetBinContent(i, h_weighted->GetBinContent(i) / width);
            h_weighted->SetBinError(i, h_weighted->GetBinError(i) / width);
        }
    }

    if (formula) delete formula;

    cout << "Entries processed: " << nEntries << std::endl;

    if (normalize) {
        cout << "no weight Integral:" << (h_weighted->Integral()) << endl;
        cout << "weighted Integral:" << (h_weighted->Integral()) << endl;
        h_noWeight->Scale(1.0 / h_noWeight->Integral());
        h_weighted->Scale(1.0 / h_weighted->Integral());

    }

    cout << "Saving histograms to output file: " << outfilename << endl;
    TFile* outputFile = TFile::Open(outfilename.c_str(), "UPDATE");
    if (!outputFile || outputFile->IsZombie()) {
        std::cerr << "Cannot create output file!" << std::endl; 
        return;
    }
    if (outputFile->GetListOfKeys()->Contains(h_noWeight_name.c_str())) {
        std::cerr << "Histogram " << h_noWeight_name << " already exists in output file. Overwriting." << std::endl;
        outputFile->Delete(h_noWeight_name.c_str());
    }
    if (outputFile->GetListOfKeys()->Contains(h_weighted_name.c_str())) {
        std::cerr << "Histogram " << h_weighted_name << " already exists in output file. Overwriting." << std::endl;
        outputFile->Delete(h_weighted_name.c_str());
    }
    cout << "Number of bins for MC branch " << branchName << " unweighted : " << h_noWeight->GetNbinsX() << endl;
    cout << "Number of bins for MC branch " << branchName << " weighted: " << h_weighted->GetNbinsX() << endl;

    h_noWeight->Write();
    h_weighted->Write();
    outputFile->Close();
}

void SaveHists_allweights_vector(
    const std::string& inputFileNameMC,
    const std::string& branchName,
    const std::string& ReweightFile,
    const std::string& cut = "",
    int nbins = 100,
    double xmin = NAN,
    double xmax = NAN,
    const std::string& outfilename = "",
    bool normalize = false,
    bool VZweight = true,
    bool Multweight = true,
    bool TrkPtweight = true)
{

    TFile* f_reweight = TFile::Open(ReweightFile.c_str());
    TFile* f_mc = TFile::Open(inputFileNameMC.c_str());

    if (!f_reweight || f_reweight->IsZombie() ||
        !f_mc || f_mc->IsZombie()) {
        std::cerr << "Cannot open one or more files!" << std::endl;
        return;
    }

    // Get weight histo
    TH1D* hVzWeight = (TH1D*) f_reweight->Get("VZReweight");
    TH1D* hMultWeight = (TH1D*) f_reweight->Get("MultReweight");
    TH1D* hPtWeight = (TH1D*) f_reweight->Get("TrkPtReweight");

    if (!hVzWeight) {
        std::cerr << "Could not get reweight VZ histogram!" << std::endl;
        return;
    }
    if (!hMultWeight) {
        std::cerr << "Could not get reweight Mult histogram!" << std::endl;
        return;
    }
    if (!hPtWeight) {
        std::cerr << "Could not get reweight Pt histogram!" << std::endl;
        return;
    }

    // Use MC tree for event loop
    TTree* tree = (TTree*) f_mc->Get("Tree"); // update to correct name
    if (!tree) {
        std::cerr << "Tree not found in MC file!" << std::endl;
        return;
    }

    float vz = 0;
    int mult = 0;
    vector<float>* trkPt = nullptr;
    vector<bool>* trkPassChargedHadron_Nominal = nullptr;
    vector<float>* branch = nullptr;

    tree->SetBranchAddress("VZ", &vz);
    tree->SetBranchAddress("multiplicityEta2p4", &mult);
    if (branchName != "trkPt") {
        tree->SetBranchAddress("trkPt", &trkPt);
    }
    tree->SetBranchAddress("trkPassChargedHadron_Nominal", &trkPassChargedHadron_Nominal);
    tree->SetBranchAddress(branchName.c_str(), &branch);

    tree->Draw((branchName + ">>htempDATA(10000)").c_str(), cut.c_str(), "goff");
    TH1D* htempDATA = (TH1D*)gDirectory->Get("htempDATA");
    if (std::isnan(xmin)) xmin = htempDATA->GetXaxis()->GetXmin();
    if (std::isnan(xmax)) xmax = htempDATA->GetXaxis()->GetXmax();
    delete htempDATA;

    // Prepare histograms
    std::string h_noWeight_name = "h_noWeight_" + branchName;
    std::string h_weighted_name = "h_weighted_" + branchName;

    const Int_t nMultBins = 39;
    const Double_t multBins[nMultBins + 1] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 
        12, 14, 16, 18, 20, 24, 28, 32, 36, 
        40, 45, 50, 55, 60, 70, 80, 100,
        120, 150, 200, 250, 300, 400, 500, 600, 700, 800, 900, 1000
    };

    const Int_t nPtBins_log = 26;
    const Double_t pTBins_log[nPtBins_log + 1] = {
        0.5, 0.603, 0.728, 0.879, 1.062, 1.284, 1.553, 1.878, 2.272, 2.749, 3.327, 4.027, 
        4.872, 5.891, 7.117, 8.591, 10.36, 12.48, 15.03, 18.08, 21.73, 26.08, 31.28, 
        37.48, 44.89, 53.73, 64.31
    };

    TH1D* h_noWeight = nullptr;
    TH1D* h_weighted = nullptr;

    std::string hname_data = "histDATA_" + branchName;
    if (branchName.find("multiplicity") != std::string::npos) {
        h_noWeight = new TH1D(h_noWeight_name.c_str(), (branchName + " (no weight)").c_str(), nMultBins, multBins);
        h_weighted = new TH1D(h_weighted_name.c_str(), (branchName + " (weighted)").c_str(), nMultBins, multBins);
    } else if (branchName == "trkPt") {
        h_noWeight = new TH1D(h_noWeight_name.c_str(), (branchName + " (no weight)").c_str(), nPtBins_log, pTBins_log);
        h_weighted = new TH1D(h_weighted_name.c_str(), (branchName + " (weighted)").c_str(), nPtBins_log, pTBins_log);
    } else if (branchName == "VZ"){
        xmin = -30;
        xmax = 30;
        nbins = 50;
        h_noWeight = new TH1D(h_noWeight_name.c_str(), (branchName + " (no weight)").c_str(), nbins, xmin, xmax);
        h_weighted = new TH1D(h_weighted_name.c_str(), (branchName + " (weighted)").c_str(), nbins, xmin, xmax);
    } else {
        h_noWeight = new TH1D(h_noWeight_name.c_str(), (branchName + " (no weight)").c_str(), nbins, xmin, xmax);
        h_weighted = new TH1D(h_weighted_name.c_str(), (branchName + " (weighted)").c_str(), nbins, xmin, xmax);
    }

    // Check if branch exists
    if (!tree->GetListOfBranches()->FindObject(branchName.c_str())) {
        std::cerr << "Branch " << branchName << " not found!" << std::endl;
        return;
    }

    cout << "Branch " << branchName << endl;

    // Add TTreeFormula for cut
    TTreeFormula* formula = nullptr;
    if (!cut.empty()) {
        formula = new TTreeFormula("cutFormula", cut.c_str(), tree);
    }
    
    Long64_t nEntries = tree->GetEntries();
    for (Long64_t i = 0; i < nEntries; ++i) {
        tree->GetEntry(i);
        if (branchName == "trkPt") {
            trkPt = branch;
        }
        if (formula && !formula->EvalInstance()) continue;

        int bin_vz = std::clamp(hVzWeight->FindBin(vz), 1, hVzWeight->GetNbinsX());
        int bin_mult = std::clamp(hMultWeight->FindBin(mult), 1, hMultWeight->GetNbinsX());
        float vzWeight = hVzWeight->GetBinContent(bin_vz);
        float multWeight =  hMultWeight->GetBinContent(bin_mult);
        for (size_t j = 0; j < branch->size(); ++j) {
          // cout << "Processing entry " << i << ", index " << j << std::endl;
            if (trkPassChargedHadron_Nominal->at(j) != 1) continue; // Skip if track does not pass the cut
            if (trkPt->at(j) <= 3.0) continue; // Skip if track pt is less than 3.0 GeV/c
            float pt = trkPt->at(j);
            int bin_pt = std::clamp(hPtWeight->FindBin(pt), 1, hPtWeight->GetNbinsX());
            
            float trkWeight = hPtWeight->GetBinContent(bin_pt);
            double binWidth = h_weighted->GetBinWidth(h_weighted->FindBin(pt));

            if (!VZweight) vzWeight = 1.0;
            if (!Multweight) multWeight = 1.0;
            if (!TrkPtweight) trkWeight = 1.0;

            float totalWeight = vzWeight * multWeight * trkWeight;

            h_noWeight->Fill(branch->at(j), 1.0 / binWidth);
            h_weighted->Fill(branch->at(j), totalWeight / binWidth);
        }
    }    

    if (formula) delete formula;

    cout << "Entries processed: " << nEntries << std::endl;
    if (normalize) {
        cout << "no weight Integral:" << (h_weighted->Integral()) << endl;
        cout << "weighted Integral:" << (h_weighted->Integral()) << endl;
        h_noWeight->Scale(1.0 / h_noWeight->Integral());
        h_weighted->Scale(1.0 / h_weighted->Integral());

    }

    cout << "Saving histograms to output file: " << outfilename << endl;
    TFile* outputFile = TFile::Open(outfilename.c_str(), "UPDATE");
    if (!outputFile || outputFile->IsZombie()) {
        std::cerr << "Cannot create output file!" << std::endl; 
        return;
    }
    if (outputFile->GetListOfKeys()->Contains(h_noWeight_name.c_str())) {
        std::cerr << "Histogram " << h_noWeight_name << " already exists in output file. Overwriting." << std::endl;
        outputFile->Delete(h_noWeight_name.c_str());
    }
    if (outputFile->GetListOfKeys()->Contains(h_weighted_name.c_str())) {
        std::cerr << "Histogram " << h_weighted_name << " already exists in output file. Overwriting." << std::endl;
        outputFile->Delete(h_weighted_name.c_str());
    }

    cout << "Number of bins for MC branch " << branchName << " unweighted : " << h_noWeight->GetNbinsX() << endl;
    cout << "Number of bins for MC branch " << branchName << " weighted: " << h_weighted->GetNbinsX() << endl;

    h_noWeight->GetXaxis()->SetTitle(branchName.c_str());
    h_weighted->GetXaxis()->SetTitle(branchName.c_str());
    h_noWeight->GetYaxis()->SetTitle("Normalized Event Counts");
    h_weighted->GetYaxis()->SetTitle("Normalized Event Counts");

    h_noWeight->Write();
    h_weighted->Write();
    outputFile->Close();
}


void SaveHist_DatavsMC(
    const std::string& inputFileNameDATA, 
    const std::string& branchName, 
    const std::string& cutDATA,
    int nbins = 100,
    double xmin = NAN,
    double xmax = NAN,
    const std::string& outRootFile = "SavedHistograms.root",
    bool normalize = false)
{
    TFile* inFileDATA = TFile::Open(inputFileNameDATA.c_str());

    if (!inFileDATA || inFileDATA->IsZombie()) {
        std::cerr << "Cannot open file: " << inputFileNameDATA << std::endl;
        return;
    }

    TTree* TreeDATA = (TTree*)inFileDATA->Get("Tree");

    if (!TreeDATA) {
        std::cerr << "Cannot find TTree 'Tree' in input files." << std::endl;
        return;
    }

    // Use temporary histograms to auto-determine xmin/xmax
    TreeDATA->Draw((branchName + ">>htempDATA(10000)").c_str(), cutDATA.c_str(), "goff");
    TH1D* htempDATA = (TH1D*)gDirectory->Get("htempDATA");
    if (std::isnan(xmin)) xmin = htempDATA->GetXaxis()->GetXmin();
    if (std::isnan(xmax)) xmax = htempDATA->GetXaxis()->GetXmax();
    delete htempDATA;

    const Int_t nMultBins = 39;
    const Double_t multBins[nMultBins + 1] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 
        12, 14, 16, 18, 20, 24, 28, 32, 36, 
        40, 45, 50, 55, 60, 70, 80, 100,
        120, 150, 200, 250, 300, 400, 500, 600, 700, 800, 900, 1000
    };

    const Int_t nPtBins_log = 26;
    const Double_t pTBins_log[nPtBins_log + 1] = {
        0.5, 0.603, 0.728, 0.879, 1.062, 1.284, 1.553, 1.878, 2.272, 2.749, 3.327, 4.027, 
        4.872, 5.891, 7.117, 8.591, 10.36, 12.48, 15.03, 18.08, 21.73, 26.08, 31.28, 
        37.48, 44.89, 53.73, 64.31
    };


    TH1D* histDATA = nullptr;
    std::string hname_data = "histDATA_" + branchName;
    if (branchName.find("multiplicity") != std::string::npos) {
        histDATA =  new TH1D(hname_data.c_str(), (branchName + " DATA MC Comparison").c_str(), nMultBins, multBins);
    } else if (branchName == "trkPt") {
        histDATA = new TH1D(hname_data.c_str(), (branchName + " DATA MC Comparison").c_str(), nPtBins_log, pTBins_log);
    } else if (branchName == "VZ"){
        xmin = -30;
        xmax = 30;
        nbins = 50;
        histDATA = new TH1D(hname_data.c_str(), (branchName + " DATA MC Comparison").c_str(), nbins, xmin, xmax);
    } else {
        histDATA = new TH1D(hname_data.c_str(), (branchName + " DATA MC Comparison").c_str(), nbins, xmin, xmax);
    }
    TreeDATA->Draw((branchName + ">>" + hname_data).c_str(), cutDATA.c_str(), "goff");
    // Save to ROOT file

    cout << "Saving histograms to output file: " << outRootFile << endl;
    TFile* outFile = TFile::Open(outRootFile.c_str(), "UPDATE");
    if (!outFile || outFile->IsZombie()) {
        std::cerr << "Cannot open or create output file: " << outRootFile << std::endl;
        return;
    }
    if (outFile->GetListOfKeys()->Contains(hname_data.c_str())) {
        std::cerr << "Histogram " << hname_data << " already exists in output file. Overwriting." << std::endl;
        outFile->Delete(hname_data.c_str());
    }

    for (int i = 1; i <= histDATA->GetNbinsX(); ++i) {
        double width = histDATA->GetBinWidth(i);
        if (width > 0) {
            histDATA->SetBinContent(i, histDATA->GetBinContent(i) / width);
            histDATA->SetBinError(i, histDATA->GetBinError(i) / width);
        }
    }

    if (normalize) {
        cout << "Data Integral:" << (histDATA->Integral()) << endl;
        histDATA->Scale(1.0 / histDATA->Integral());
    }

    cout << "Number of Bins for Data branch " << branchName << ": " << histDATA->GetNbinsX() << endl;
    histDATA->Write();
    outFile->Close();

    delete histDATA;
    inFileDATA->Close();
}

void SaveHist_DatavsMC_Vector(
    const std::string& inputFileNameDATA, 
    const std::string& branchName, 
    const std::string& cutDATA,
    int nbins = 100,
    double xmin = NAN,
    double xmax = NAN,
    const std::string& outRootFile = "SavedHistograms.root",
    bool normalize = false)
{
    cout << "BEGIN SAVING HIST FOR: " << branchName << endl;
    TFile* inFileDATA = TFile::Open(inputFileNameDATA.c_str());
    if (!inFileDATA || inFileDATA->IsZombie()) {
        std::cerr << "Cannot open file: " << inputFileNameDATA << std::endl;
        return;
    }

    TTree* TreeDATA = (TTree*)inFileDATA->Get("Tree");
    if (!TreeDATA) {
        std::cerr << "Cannot find TTree 'Tree' in input file." << std::endl;
        return;
    }

    // Set up binning
    const Int_t nMultBins = 39;
    const Double_t multBins[nMultBins + 1] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 
        12, 14, 16, 18, 20, 24, 28, 32, 36, 
        40, 45, 50, 55, 60, 70, 80, 100,
        120, 150, 200, 250, 300, 400, 500, 600, 700, 800, 900, 1000
    };

    const Int_t nPtBins_log = 26;
    const Double_t pTBins_log[nPtBins_log + 1] = {
        0.5, 0.603, 0.728, 0.879, 1.062, 1.284, 1.553, 1.878, 2.272, 2.749, 3.327, 4.027, 
        4.872, 5.891, 7.117, 8.591, 10.36, 12.48, 15.03, 18.08, 21.73, 26.08, 31.28, 
        37.48, 44.89, 53.73, 64.31
    };

    // Initialize histogram
    TH1D* histDATA = nullptr;
    std::string hname_data = "histDATA_" + branchName;
    if (branchName.find("multiplicity") != std::string::npos) {
        histDATA = new TH1D(hname_data.c_str(), (branchName + " DATA").c_str(), nMultBins, multBins);
    } else if (branchName == "trkPt") {
        histDATA = new TH1D(hname_data.c_str(), (branchName + " DATA").c_str(), nPtBins_log, pTBins_log);
    } else if (branchName == "VZ") {
        xmin = -30; xmax = 30; nbins = 50;
        histDATA = new TH1D(hname_data.c_str(), (branchName + " DATA").c_str(), nbins, xmin, xmax);
    } else {
        histDATA = new TH1D(hname_data.c_str(), (branchName + " DATA").c_str(), nbins, xmin, xmax);
    }

    histDATA->Sumw2();

    // Prepare for vector branch reading
    std::vector<float>* dataVec = nullptr;
    std::vector<bool>* trkPassChargedHadron_Nominal = nullptr;
    std::vector<float>* trkPt = nullptr;
    TreeDATA->SetMakeClass(0);
    TreeDATA->SetBranchAddress(branchName.c_str(), &dataVec);
    if (branchName != "trkPt") {
        TreeDATA->SetBranchAddress("trkPt", &trkPt);
    }
    TreeDATA->SetBranchAddress("trkPassChargedHadron_Nominal", &trkPassChargedHadron_Nominal);
    // Prepare cut formula
    TTreeFormula* formulaData = new TTreeFormula("cutFormula", cutDATA.c_str(), TreeDATA);

    // Loop over entries
    Long64_t nEntries = TreeDATA->GetEntries();
    for (Long64_t i = 0; i < nEntries; ++i) {
        TreeDATA->GetEntry(i);
        if (branchName == "trkPt") {
            trkPt = dataVec; // Use trkPt vector if branch is "trkPt"
        }
        if (!formulaData->EvalInstance()) continue;
        if (!dataVec) continue;
        for (int j = 0; j < dataVec->size(); ++j) {
            if (trkPassChargedHadron_Nominal->at(j) != 1 ) continue; // Skip if track does not pass the cut
            if (trkPt->at(j) <= 3.0) continue; // Skip if track pt is less than 3.0 GeV/c
            float val = dataVec->at(j);
            histDATA->Fill(val);
        }
    }
    cout << "Entries processed: " << nEntries << endl;

    delete formulaData;

    // Normalize by bin width
    for (int i = 1; i <= histDATA->GetNbinsX(); ++i) {
        double width = histDATA->GetBinWidth(i);
        if (width > 0) {
            histDATA->SetBinContent(i, histDATA->GetBinContent(i) / width);
            histDATA->SetBinError(i, histDATA->GetBinError(i) / width);
        }
    }

    // Normalize to unit area
    if (normalize && histDATA->Integral() > 0) {
        histDATA->Scale(1.0 / histDATA->Integral());
    }

    // Write histogram to file
    TFile* outFile = TFile::Open(outRootFile.c_str(), "UPDATE");
    if (!outFile || outFile->IsZombie()) {
        std::cerr << "Cannot create output file: " << outRootFile << std::endl;
        return;
    }

    if (outFile->GetListOfKeys()->Contains(hname_data.c_str())) {
        std::cerr << "Histogram " << hname_data << " already exists, deleting." << std::endl;
        outFile->Delete((hname_data + ";*").c_str());  // delete all cycles
    }


    histDATA->Write();
    outFile->Close();
    delete histDATA;
    inFileDATA->Close();

}

void PlotWithWeightNene(
    std::string inFileDATA,
    std::string inFileHij,
    std::string cutDATA,
    std::string cutMC,
    std::string ReweightFile,
    std::string MCoutputVZ,
    std::string MCoutputMult,
    std::string MCoutputPt,
    std::string MCoutput,
    std::string DataOutput,
    bool Debug = false
) {

    string cutTrkDATA = cutDATA;
    string cutTrkMC = cutMC;

    // List of branch names to process
    std::vector<std::string> branches = {
        "VZ",
        "leadingPtEta1p0_sel",
        "HFEMaxPlus",
        "HFEMaxMinus",
        "hiHFMinus_pf",
        "hiHFPlus_pf",
        "hiHF_pf",
        "multiplicityEta2p4",
        "multiplicityEta1p0"
    };

    std::vector<float> xmin = {
        -15,
        0,
        0,
        0,
        0,
        0,
        NAN,
        NAN
    };

    std::vector<float> xmax = {
        15,
        60,
        400,
        250,
        250,
        400,
        NAN,
        NAN
    };



for (size_t i = 0; i < branches.size(); ++i) {
    cout << "Processing branch: " << branches[i] << endl;
    if (Debug && i == 1){
        break;
    }
    const auto& branch = branches[i];
    SaveHists_allweights_evtbyevt(
        inFileHij,
        branch,
        ReweightFile,
        cutMC,
        100,
        xmin[i],
        xmax[i],
        MCoutput,
        true,
        true,
        true);
    
    SaveHists_allweights_evtbyevt(
        inFileHij,
        branch,
        ReweightFile,
        cutMC,
        100,
        xmin[i],
        xmax[i],
        MCoutputVZ,
        true,
        true,
        false);

 SaveHists_allweights_evtbyevt(
        inFileHij,
        branch,
        ReweightFile,
        cutMC,
        100,
        xmin[i],
        xmax[i],
        MCoutputMult,
        true,
        false,
        true);

   SaveHist_DatavsMC(
        inFileDATA, 
        branch,
        cutDATA,
        100,
        xmin[i],
        xmax[i],
        DataOutput,
        true);
    }

    std::vector<std::string> vectorbranches = {
        "trkDxyAssociatedVtx",
        "trkDzAssociatedVtx",
        "trkPt",
        "trkEta"
    };

    std::vector<float> vectorxmin = {
        -1,
        -1,
        0,
        -2.4
    };

    std::vector<float> vectorxmax = {
        1,
        1,
        10000,
        2.4
    };
 

    for (size_t i = 0; i < vectorbranches.size(); ++i) {
        if (Debug && i == 0){
            break;
        }
        const auto& branch = vectorbranches[i];
        SaveHists_allweights_vector(
            inFileHij,
            branch,
            ReweightFile,
            cutTrkMC,
            100,
            vectorxmin[i],
            vectorxmax[i],
            MCoutput,
            true,
            true,
            true,
            true);

        SaveHists_allweights_vector(
            inFileHij,
            branch,
            ReweightFile,
            cutTrkMC,
            100,
            vectorxmin[i],
            vectorxmax[i],
            MCoutputVZ,
            true,
            true,
            false,
            false);

        SaveHists_allweights_vector(
            inFileHij,
            branch,
            ReweightFile,
            cutTrkMC,
            100,
            vectorxmin[i],
            vectorxmax[i],
            MCoutputMult,
            true,
            false,
            true,
            false);

        SaveHists_allweights_vector(
            inFileHij,
            branch,
            ReweightFile,
            cutTrkMC,
            100,
            vectorxmin[i],
            vectorxmax[i],
            MCoutputPt,
            true,
            false,
            false,
            true);   

        SaveHist_DatavsMC_Vector(
            inFileDATA, 
            branch,
            cutTrkDATA,
            100,
            vectorxmin[i],
            vectorxmax[i],
            DataOutput,
            true);
    }
}