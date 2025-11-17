#include "plotter.C"

Parameters drawcuts_nopt(TFile* infile, TCanvas* c){

    //cout << infile->GetName() << endl;
    TH1D* parMinJetPt = (TH1D*)infile->Get("par/parMinJetPT");
    TH1D* parMaxJetPt = (TH1D*)infile->Get("par/parMaxJetPT");
    TH1D* parIsData = (TH1D*)infile->Get("par/parIsData");
    TH1D* parIsPP = (TH1D*)infile->Get("par/parIsPP");
    TH1D* parChargeSelection = (TH1D*)infile->Get("par/parChargeSelection");
    TNamed* parDCAString = (TNamed*)infile->Get("par/parDCAString");
    
    float MaxJetPt = parMaxJetPt->GetBinContent(1);
    float MinJetPt = parMinJetPt->GetBinContent(1);
    float IsData = parIsData->GetBinContent(1);
    float IsPP = parIsPP->GetBinContent(1);
    float ChargeSelection = parChargeSelection->GetBinContent(1);
    string DCAString = parDCAString->GetTitle();

    string descriptor = "";
    if(IsData == 1){
        if(IsPP == 1){
            descriptor = "pp Data";
        } else {
            descriptor = "PbPb Min-Bias data";
        }
    } else {
        if(IsPP == 1){
            descriptor = "PYTHIA8 CP5 Tune, QCD Jets";
        } else {
            descriptor = "PYTHIA8 CP5 + HIJING, QCD Jets";
        }
    }

    Parameters par;
    par.MaxJetPt = MaxJetPt;
    par.MinJetPt = MinJetPt;
    par.IsData = IsData;
    par.IsPP = IsPP;
    par.ChargeSelection = ChargeSelection;
    par.DCAString = DCAString;  
    par.descriptor = descriptor;  // Assign string directly


    if(c == nullptr){
        cout << "Canvas pointer is null!" << endl;
        return Parameters();
    }

    c->cd();

    // Create TLatex object for adding text
    TLatex* latex = new TLatex();
    latex->SetNDC();  // Use normalized coordinates (0-1)
    latex->SetTextSize(0.04);
    latex->SetTextFont(42);
    
    // Enable LaTeX interpretation
    gStyle->SetLabelFont(42, "XYZ");
    gStyle->SetTitleFont(42, "XYZ");


    latex->SetTextFont(62);  // Bold font for CMS
    latex->DrawLatex(0.14, 0.94, "CMS");
    latex->SetTextFont(52);  // Italic font for Preliminary
    latex->DrawLatex(0.21, 0.94, "Preliminary");
    latex->SetTextFont(42);  // Reset to normal font

    if(IsPP == 1)
        latex->DrawLatex(0.69, 0.91, "2017 pp 5.02 TeV");
    else
        latex->DrawLatex(0.69, 0.91, "2018 PbPb 5.02 TeV");

    latex->DrawLatex(0.20, 0.85, "#mu p_{T} > 3.5 GeV");
    latex->DrawLatex(0.20, 0.79, "#DeltaR(#mu, jet) < 0.3");
    latex->DrawLatex(0.20, 0.73, parseDCAcut(DCAString));
    
    if(ChargeSelection == -1){
        latex->DrawLatex(0.20, 0.67, "#mu^{+}#mu^{-} pairs only");
    } else if (ChargeSelection == 1){
        latex->DrawLatex(0.20, 0.67, "Same Sign Dimuons");
    } 

    return par;
}

void result(const char* directory = "output_ptdep3", const char* output = "output_ptdep3/output_DoubleHQtagging_120_160.root"){

    gStyle->SetOptStat(0);
    
    // Use system command to find all massfit files
    string search_command = Form("find %s -name 'massfit_*.root' 2>/dev/null", directory);
    FILE* pipe = popen(search_command.c_str(), "r");
    
    vector<TFile*> massfit_files;
    vector<string> filenames;
    vector<pair<int,int>> pt_ranges;
    vector<float> ptbins;
    
    if(pipe) {
        char buffer[256];
        while(fgets(buffer, sizeof(buffer), pipe) != NULL) {
            // Remove newline character
            string filename = buffer;
            filename.erase(filename.find_last_not_of(" \n\r\t") + 1);
            
            // Extract pT values from filename using regex-like parsing
            size_t start = filename.find("massfit_");
            if(start != string::npos) {
                start += 8; // Length of "massfit_"
                size_t underscore = filename.find("_", start);
                size_t dot = filename.find(".root");
                
                if(underscore != string::npos && dot != string::npos) {
                    int ptmin = atoi(filename.substr(start, underscore - start).c_str());
                    int ptmax = atoi(filename.substr(underscore + 1, dot - underscore - 1).c_str());
                    
                    TFile* file = TFile::Open(filename.c_str());
                    if(file && !file->IsZombie()) {
                        cout << "Successfully opened: " << filename << " (pT: " << ptmin << "-" << ptmax << " GeV)" << endl;
                        massfit_files.push_back(file);
                        filenames.push_back(filename);
                        pt_ranges.push_back(make_pair(ptmin, ptmax));
                        if(ptbins.empty() || ptbins.back() != ptmin) {
                            ptbins.push_back(ptmin);
                        }
                        ptbins.push_back(ptmax);
                    } else {
                        cout << "Warning: Could not open file: " << filename << endl;
                        if(file) file->Close();
                    }
                }
            }
        }
        pclose(pipe);
    }
    
    cout << "Successfully loaded " << massfit_files.size() << " massfit files." << endl;
    
    // Create indices for sorting
    vector<int> indices(massfit_files.size());
    iota(indices.begin(), indices.end(), 0);
    
    // Sort indices based on ptmin values
    sort(indices.begin(), indices.end(), [&](int a, int b) {
        return pt_ranges[a].first < pt_ranges[b].first;
    });
    
    // Reorder all vectors according to sorted indices
    vector<TFile*> sorted_files;
    vector<string> sorted_filenames;
    vector<pair<int,int>> sorted_pt_ranges;
    
    for(int idx : indices) {
        sorted_files.push_back(massfit_files[idx]);
        sorted_filenames.push_back(filenames[idx]);
        sorted_pt_ranges.push_back(pt_ranges[idx]);
    }
    
    // Replace original vectors with sorted ones
    massfit_files = sorted_files;
    filenames = sorted_filenames;
    pt_ranges = sorted_pt_ranges;
    
    // Rebuild ptbins in sorted order
    ptbins.clear();
    for(auto& range : pt_ranges) {
        if(ptbins.empty() || ptbins.back() != range.first) {
            ptbins.push_back(range.first);
        }
        if(ptbins.back() != range.second) {
            ptbins.push_back(range.second);
        }
    }
    
    cout << "Files sorted by pT range. pT bins: ";
    for(float bin : ptbins) cout << bin << " ";
    cout << endl;

    TH1D* h_spectrum_all = new TH1D("h_spectrum_all", "N_{#mu#mu} / N_{jets};Jet p_{T} [GeV];  N_{#mu#mu} / N_{jets}", ptbins.size()-1, ptbins.data());

    
    for(int i = 0; i < massfit_files.size(); i++) {
        TFile* file = massfit_files[i];
        int ptmin = pt_ranges[i].first;
        int ptmax = pt_ranges[i].second;

        int nmumus  = file->Get<TH1F>("hYieldMuMuJetPt")->GetBinContent(1);
        int njets   = file->Get<TH1F>("hYieldInclusiveJetPt")->GetBinContent(1);
        
        h_spectrum_all->Fill((ptmin + ptmax) / 2.0, (float)nmumus / (float)njets);
        h_spectrum_all->SetBinError(h_spectrum_all->FindBin((ptmin + ptmax) / 2.0), sqrt(nmumus) / (float)njets);
        cout << "pT range: " << ptmin << "-" << ptmax << " GeV, N_mumu: " << nmumus << ", N_jets: " << njets << ", Ratio: " << (float)nmumus / (float)njets << endl;
    }
    
    
    TCanvas* c1 = new TCanvas("c1", "Spectrum", 800, 600);
    c1->SetLeftMargin(0.15);  // Increase left margin
    h_spectrum_all->SetLineColor(kBlack);
    h_spectrum_all->SetLineWidth(2);
    h_spectrum_all->SetMarkerStyle(20);
    h_spectrum_all->SetMarkerSize(0.8);
    h_spectrum_all->SetMaximum(h_spectrum_all->GetMaximum() * 1.2);
    h_spectrum_all->Draw("PE");

    TFile* parfile = TFile::Open(output);
    drawcuts_nopt(parfile, c1); 

    c1->SaveAs(Form("%s/dimuon_spectrum_all.pdf", directory));
    TFile* outputfile = TFile::Open(Form("%s/dimuon_result.root", directory), "RECREATE");
    outputfile->cd();
    h_spectrum_all->Write();
    outputfile->Close();


    // Clean up - close all files
    for(TFile* file : massfit_files){
        if(file) file->Close();
    }
    
}