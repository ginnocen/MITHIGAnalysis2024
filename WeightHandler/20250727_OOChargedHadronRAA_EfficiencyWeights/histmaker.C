#include <ROOT/RDataFrame.hxx>
#include <TH1F.h>
#include <TFile.h>
#include <TMath.h>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <TChain.h>

using namespace std;
using namespace ROOT;

TChain* createDataChain(const char* dataSource) {
    TChain* chain = new TChain("Tree");
    string source(dataSource);
    if (source.find(".txt") != string::npos) {
        ifstream infile(dataSource);
        string line;
        while (getline(infile, line)) {
            if (!line.empty() && line[0] != '#') {
                chain->Add(line.c_str());
            }
        } 
    }
    return chain;
}

//// CREATE RDATAFRAMES /////
ROOT::RDF::RNode MakeFrame(const char* filename, TH1D* VZReweight, TH1D* MultReweight, TH1D* TrackPtReweight, int doVZweight = 1, int doMultweight = 1, int doTrackPtweight = 1) {
    
    TFile* file = TFile::Open(filename);
    TTree* tree = (TTree*)file->Get("Tree");
    RDataFrame df(*tree);
    
    // VZ REWEIGHTING
    ROOT::RDF::RNode df_weighted = df;
    if (doVZweight && VZReweight) {
        df_weighted = df_weighted.Define("VZWeight", 
            [VZReweight](float VZ) { 
                int bin = VZReweight->FindBin(VZ); 
                return VZReweight->GetBinContent(bin);
            }, {"VZ"});
    }
    else {
        df_weighted = df_weighted.Define("VZWeight", "1.0");
    }

    // MULTIPLICITY REWEIGHTING
    if (doMultweight && MultReweight) {
        df_weighted = df_weighted.Define("MultWeight",
            [MultReweight](int mult) { 
                int bin = MultReweight->FindBin(mult); 
                return MultReweight->GetBinContent(bin);
            }, {"multiplicityEta2p4"});
    }
    else {
        df_weighted = df_weighted.Define("MultWeight", "1.0");
    }

    // LEADING TRACK PT REWEIGHTING
    if(doTrackPtweight && TrackPtReweight) {
        df_weighted = df_weighted.Define("leadingTrkPtWeight",
            [TrackPtReweight](float leadingtrackpT) {
                int bin = TrackPtReweight->FindBin(leadingtrackpT);
                return TrackPtReweight->GetBinContent(bin);
            }, {"leadingPtEta1p0_sel"});
    }

    // TRACK PT REWEIGHTING 
    if (doTrackPtweight && TrackPtReweight) {
        df_weighted = df_weighted.Define("trkPtWeights",
            [TrackPtReweight](const vector<float>& trkPt) {
                vector<float> weights;
                weights.reserve(trkPt.size());
                for (float pt : trkPt) {
                    int bin = TrackPtReweight->FindBin(pt);
                    weights.push_back(TrackPtReweight->GetBinContent(bin));
                }
                return weights;
            }, {"trkPt"});
    }
    else {
        // UNIT WEIGHT VECTOR
        df_weighted = df_weighted.Define("trkPtWeights",
            [](const vector<float>& trkPt) {
                vector<float> weights(trkPt.size(), 1.0);
                return weights;
            }, {"trkPt"});
    }

    return df_weighted;
}

//// CREATE RDATAFRAMES FROM TCHAIN /////
ROOT::RDF::RNode MakeFrame(TChain* chain, TH1D* VZReweight, TH1D* MultReweight, TH1D* TrackPtReweight, int doVZweight = 1, int doMultweight = 1, int doTrackPtweight = 1) {
    
    RDataFrame df(*chain);
    
    // VZ REWEIGHTING
    ROOT::RDF::RNode df_weighted = df;
    if (doVZweight && VZReweight) {
        df_weighted = df_weighted.Define("VZWeight", 
            [VZReweight](float VZ) { 
                int bin = VZReweight->FindBin(VZ); 
                return VZReweight->GetBinContent(bin);
            }, {"VZ"});
    }
    else {
        df_weighted = df_weighted.Define("VZWeight", "1.0");
    }

    // MULTIPLICITY REWEIGHTING
    if (doMultweight && MultReweight) {
        df_weighted = df_weighted.Define("MultWeight",
            [MultReweight](int mult) { 
                int bin = MultReweight->FindBin(mult); 
                return MultReweight->GetBinContent(bin);
            }, {"multiplicityEta2p4"});
    }
    else {
        df_weighted = df_weighted.Define("MultWeight", "1.0");
    }

    // LEADING TRACK PT REWEIGHTING
    if(doTrackPtweight && TrackPtReweight) {
        df_weighted = df_weighted.Define("leadingTrkPtWeight",
            [TrackPtReweight](float leadingtrackpT) {
                int bin = TrackPtReweight->FindBin(leadingtrackpT);
                return TrackPtReweight->GetBinContent(bin);
            }, {"leadingPtEta1p0_sel"});
    }

    // TRACK PT REWEIGHTING 
    if (doTrackPtweight && TrackPtReweight) {
        df_weighted = df_weighted.Define("trkPtWeights",
            [TrackPtReweight](const vector<float>& trkPt) {
                vector<float> weights;
                weights.reserve(trkPt.size());
                for (float pt : trkPt) {
                    int bin = TrackPtReweight->FindBin(pt);
                    weights.push_back(TrackPtReweight->GetBinContent(bin));
                }
                return weights;
            }, {"trkPt"});
    }
    else {
        // UNIT WEIGHT VECTOR
        df_weighted = df_weighted.Define("trkPtWeights",
            [](const vector<float>& trkPt) {
                vector<float> weights(trkPt.size(), 1.0);
                return weights;
            }, {"trkPt"});
    }

    return df_weighted;
}

//// MAKE HISTOGRAMS /////
void FillHist(ROOT::RDF::RNode df, const char* variable, int doVZweight, int doMultweight, int doTrackPtweight, TH1D* hist, TCut tcut) {

    // APPLY TCUT
    string cut_str = string(tcut.GetTitle());
    auto df_cut = cut_str.empty() ? df.Filter("1") : df.Filter(cut_str);

    cout << "FILLING " << variable << " HISTOGRAM" << endl;
    cout << "CUT: " << tcut.GetTitle() << endl;
    cout << "EVENTS AFTER CUT: " << df_cut.Count().GetValue() << endl;
    
    // APPLY WEIGHTS TO HISTOGRAM
    if (doTrackPtweight) {
        // TRACK BY TRACK REWEIGHTING
        df_cut.Foreach([hist, doVZweight, doMultweight](const vector<float>& var_vec, const vector<float>& trkWeights, 
                                                        float vzWeight, float multWeight) {
            for (size_t i = 0; i < var_vec.size(); ++i) {
                double weight = trkWeights[i];
                if (doVZweight) weight *= vzWeight;
                if (doMultweight) weight *= multWeight;
                hist->Fill(var_vec[i], weight);
            }
        }, {variable, "trkPtWeights", "VZWeight", "MultWeight"});
    }
    else {
        // EVENT BY EVENT REWEIGHTING - Create combined weight column
        string weightCol;
        if (doVZweight && doMultweight) {
            // Create combined weight column
            auto df_with_weight = df_cut.Define("CombinedWeight", "VZWeight * MultWeight");
            weightCol = "CombinedWeight";
            auto h = df_with_weight.Histo1D(*hist, variable, weightCol);
            hist->Add(h.GetPtr());
        }
        else if (doVZweight) {
            weightCol = "VZWeight";
            auto h = df_cut.Histo1D(*hist, variable, weightCol);
            hist->Add(h.GetPtr());
        }
        else if (doMultweight) {
            weightCol = "MultWeight";
            auto h = df_cut.Histo1D(*hist, variable, weightCol);
            hist->Add(h.GetPtr());
        }
        else {
            // No weights - use default
            auto h = df_cut.Histo1D(*hist, variable);
            hist->Add(h.GetPtr());
        }
    }
}

//// SCALE DOWN BY BIN WIDTH ////
void BinWidthScale(TH1D* hist) {
    //hist->Sumw2();
    for (int i = 1; i <= hist->GetNbinsX(); ++i) {
        double binContent = hist->GetBinContent(i);
        double binError = hist->GetBinError(i);
        double binWidth = hist->GetBinWidth(i);
        
        hist->SetBinContent(i, binContent / binWidth);
        hist->SetBinError(i, binError / binWidth);
    }
}

//// CALCULATE EFFICIENCY WITH BAYESIAN ERRORS ////
TH1D* CalculateEfficiency(TH1D* hPass, TH1D* hTotal, const char* name, const char* title) {
    
    TH1D* hEff = (TH1D*)hPass->Clone(name);
    hEff->SetTitle(title);
    hEff->Reset();
    hEff->Divide(hPass, hTotal, 1.0, 1.0, "B");
    
    return hEff;
}

void histmaker(TCut Datacut,
                TCut MCcut,
                const char* dataSource = "datafiles.txt",
                const char* ooskim = "/data00/OOsamples/Skims20250704/skim_HiForest_250520_Hijing_MinimumBias_b015_OO_5362GeV_250518.root",
                const char* ooskim_arg = "/data00/OOsamples/Skims20250704/20250704_HiForest_250520_Pythia_Angantyr_OO_OO_5362GeV_250626.root",
                int doVZweight = 1,
                int doMultweight = 1,
                int doTrackPtweight = 1,
                const char* outfilename = "hists/output.root"
           ) {

    cout << "STARTING MULTIPLICITY REWEIGHT" << endl;
    
    //// GET REWEIGHT HISTOGRAMS ////
    TFile* fVZReweight = TFile::Open("VZReweight/VZReweight.root");
    TH1D* vzReweight = (TH1D*)fVZReweight->Get("VZReweight");
    TH1D* vzReweight_Arg = (TH1D*)fVZReweight->Get("VZReweight_Arg");

    TFile* fMultReweight = TFile::Open("MultReweight/MultReweight.root");
    TH1D* multReweight = (TH1D*)fMultReweight->Get("MultReweight");
    TH1D* multReweight_Arg = (TH1D*)fMultReweight->Get("MultReweight_Arg");

    TFile* fTrkPtReweight = TFile::Open("TrkPtReweight/TrkPtReweight.root");
    TH1D* TrkPtReweight = (TH1D*)fTrkPtReweight->Get("TrkPtReweight");
    TH1D* TrkPtReweight_Arg = (TH1D*)fTrkPtReweight->Get("TrkPtReweight_Arg");

    //// CREATE RDATAFRAMES
    TChain* chainData = createDataChain(dataSource);
    auto dfData = MakeFrame(chainData, vzReweight, multReweight, TrkPtReweight, doVZweight, doMultweight, doTrackPtweight);
    auto dfOO = MakeFrame(ooskim, vzReweight, multReweight, TrkPtReweight, doVZweight, doMultweight, doTrackPtweight);
    auto dfOO_Arg = MakeFrame(ooskim_arg, vzReweight_Arg, multReweight_Arg, TrkPtReweight_Arg, doVZweight, doMultweight, doTrackPtweight);

    //// INITIALIZE HISTOGRAMS
    const Int_t nMultBins = 34;
    const Double_t multBins[nMultBins + 1] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 
        12, 14, 16, 18, 20, 24, 28, 32, 36, 
        40, 45, 50, 55, 60, 70, 80, 100,
        120, 150, 200, 250, 300, 400, 1000
    };

    const Int_t nPtBins_log = 68;
    const Double_t pTBins_log[nPtBins_log + 1] = {
        0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0,2.2,2.4,
        2.6,2.8,3.0,3.2,3.4,3.6,3.8,4.0,4.4,4.8,5.2,5.6,6.0,6.4,6.8,7.2,7.6,8.0,
        8.5,9.0,9.5,10.0,11.,12.,13.,14.,15.,16.,17.,18.,19.,20.,21.,22.6,24.6,
        26.6,28.6,32.6,36.6,42.6,48.6,54.6,60.6,74.0,86.4,103.6,120.8,140.,165.,
        250.,400.
    };

    /*const Int_t nPtBins = 26;
    const Double_t ptBins[nPtBins + 1] = {
        0.5, 0.603, 0.728, 0.879, 1.062, 1.284, 1.553, 1.878, 2.272, 2.749, 
        3.327, 4.027, 4.872, 5.891, 7.117, 8.591, 10.36, 12.48, 
        15.03, 18.08, 21.73, 26.08, 31.28, 37.48, 44.89, 53.73, 64.31
    };*/

    // MULTIPLICITY ----- OTHER HISTOGRAMS EASILY ADDABLE
    TH1D* hDataMult_raw = new TH1D("hDataMult_raw", "Data Multiplicity Raw", nMultBins, multBins);
    TH1D* hDataMult_baseline = new TH1D("hDataMult_baseline", "Data Multiplicity with Baseline", nMultBins, multBins);
    TH1D* hDataMult_ESEL = new TH1D("hDataMult_ESEL", "Data Multiplicity with ESEL", nMultBins, multBins);
    TH1D* hDataMult_leadpT5 = new TH1D("hDataMult_leadpT5", "Data Multiplicity with Leading Track pT > 5 GeV", nMultBins, multBins);

    TH1D* hOO_Mult_raw = new TH1D("hOO_Mult_raw", "HIJING Multiplicity Raw", nMultBins, multBins);
    TH1D* hOO_Mult_baseline = new TH1D("hOO_Mult_baseline", "HIJING Multiplicity with Baseline", nMultBins, multBins);
    TH1D* hOO_Mult_ESEL = new TH1D("hOO_Mult_ESEL", "HIJING Multiplicity with ESEL", nMultBins, multBins);
    TH1D* hOO_Mult_leadpT5 = new TH1D("hOO_Mult_leadpT5", "HIJING Multiplicity with Leading Track pT > 5 GeV", nMultBins, multBins);    

    TH1D* hOO_Arg_Mult_raw = new TH1D("hOO_Arg_Mult_raw", "Angantyr Multiplicity Raw", nMultBins, multBins);
    TH1D* hOO_Arg_Mult_baseline = new TH1D("hOO_Arg_Mult_baseline", "Angantyr Multiplicity with Baseline", nMultBins, multBins);
    TH1D* hOO_Arg_Mult_ESEL = new TH1D("hOO_Arg_Mult_ESEL", "Angantyr Multiplicity with ESEL", nMultBins, multBins);
    TH1D* hOO_Arg_Mult_leadpT5 = new TH1D("hOO_Arg_Mult_leadpT5", "Angantyr Multiplicity with Leading Track pT > 5 GeV", nMultBins, multBins);

    // FILL THE HISTOGRAMS
    TCut raw("");
    TCut baseline("(VZ > -15 && VZ < 15) && (PVFilter == 1) && (ClusterCompatibilityFilter == 1)");
    TCut leadpt5(("(leadingPtEta1p0_sel > 5) && " + string(MCcut.GetTitle())).c_str());

    TCut rawDATA("HLT_OxyZeroBias_v1");
    TCut baselineDATA("HLT_OxyZeroBias_v1 && (VZ > -15 && VZ < 15) && (PVFilter == 1) && (ClusterCompatibilityFilter == 1)");
    TCut leadpt5DATA(("HLT_OxyZeroBias_v1 && (leadingPtEta1p0_sel > 5) && " + string(Datacut.GetTitle())).c_str());

    // FILL THE HISTOGRAMS
    FillHist(dfData, "multiplicityEta2p4", 0, 0, 0, hDataMult_raw, rawDATA);
    FillHist(dfData, "multiplicityEta2p4", 0, 0, 0, hDataMult_baseline, baselineDATA);
    FillHist(dfData, "multiplicityEta2p4", 0, 0, 0, hDataMult_ESEL, Datacut);
    FillHist(dfData, "multiplicityEta2p4", 0, 0, 0, hDataMult_leadpT5, leadpt5DATA);

    FillHist(dfOO, "multiplicityEta2p4", doVZweight, doMultweight, 0, hOO_Mult_raw, raw);
    FillHist(dfOO, "multiplicityEta2p4", doVZweight, doMultweight, 0, hOO_Mult_baseline, baseline);
    FillHist(dfOO, "multiplicityEta2p4", doVZweight, doMultweight, 0, hOO_Mult_ESEL, MCcut);
    FillHist(dfOO, "multiplicityEta2p4", doVZweight, doMultweight, 0, hOO_Mult_leadpT5, leadpt5);

    FillHist(dfOO_Arg, "multiplicityEta2p4", doVZweight, doMultweight, 0, hOO_Arg_Mult_raw, raw);
    FillHist(dfOO_Arg, "multiplicityEta2p4", doVZweight, doMultweight, 0, hOO_Arg_Mult_baseline, baseline);
    FillHist(dfOO_Arg, "multiplicityEta2p4", doVZweight, doMultweight, 0, hOO_Arg_Mult_ESEL, MCcut);
    FillHist(dfOO_Arg, "multiplicityEta2p4", doVZweight, doMultweight, 0, hOO_Arg_Mult_leadpT5, leadpt5);

    cout << "DONE FILLING HISTOGRAMS" << endl; 

    // CALCULATE EFFICIENCY BEFORE BIN WIDTH SCALING
    TH1D* hDataMult_Eff = CalculateEfficiency(hDataMult_ESEL, hDataMult_raw, "hDataMult_Eff", "Data Multiplicity Efficiency");
    TH1D* hOO_Mult_Eff = CalculateEfficiency(hOO_Mult_ESEL, hOO_Mult_raw, "hOO_Mult_Eff", "HIJING Multiplicity Efficiency");
    TH1D* hOO_Arg_Mult_Eff = CalculateEfficiency(hOO_Arg_Mult_ESEL, hOO_Arg_Mult_raw, "hOO_Arg_Mult_Eff", "Angantyr Multiplicity Efficiency");

    // RESCALE HISTOGRAMS AFTER EFFICIENCY CALCULATION
    BinWidthScale(hDataMult_raw);
    BinWidthScale(hDataMult_baseline);
    BinWidthScale(hDataMult_ESEL);
    BinWidthScale(hDataMult_leadpT5);

    BinWidthScale(hOO_Mult_raw);
    BinWidthScale(hOO_Mult_baseline);
    BinWidthScale(hOO_Mult_ESEL);
    BinWidthScale(hOO_Mult_leadpT5);

    BinWidthScale(hOO_Arg_Mult_raw);
    BinWidthScale(hOO_Arg_Mult_baseline);
    BinWidthScale(hOO_Arg_Mult_ESEL);
    BinWidthScale(hOO_Arg_Mult_leadpT5);

    // WRITE HISTOGRAMS TO FILE
    TFile* outFile = TFile::Open(outfilename, "RECREATE");
    outFile->cd();
    hDataMult_raw->Write();
    hDataMult_baseline->Write();
    hDataMult_ESEL->Write();
    hDataMult_leadpT5->Write();
    hOO_Mult_raw->Write();
    hOO_Mult_baseline->Write();
    hOO_Mult_ESEL->Write();
    hOO_Mult_leadpT5->Write();
    hOO_Arg_Mult_raw->Write();
    hOO_Arg_Mult_baseline->Write();
    hOO_Arg_Mult_ESEL->Write();
    hOO_Arg_Mult_leadpT5->Write();
    hDataMult_Eff->Write();
    hOO_Mult_Eff->Write();
    hOO_Arg_Mult_Eff->Write();  

    outFile->Close();
}