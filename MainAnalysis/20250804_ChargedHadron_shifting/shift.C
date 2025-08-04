#include <TH1.h>
#include <TF1.h>
#include <TMath.h>
#include <TMinuit.h>
#include <vector>
#include <iostream>

// Global pointers for minimization
TH1* gDataHist = nullptr;
TH1* gTemplateHist = nullptr;
TF1* gShiftFunc = nullptr;
double gFitMin = 0, gFitMax = 0;
int gConstraint = 0;

// 0. Get the desired histogram with a tcut
void GetHistogram(const char* variable, TTree* tree, TCut T, TH1D* h, float scale){
    tree->Project(h->GetName(), variable, T);
    TCut viable("HLT_MinimumBiasHF_OR_BptxAND_v1");
    //h->Scale(scale/tree->GetEntries(viable));
    h->Scale(1.0/h->Integral());
}

void GetHistogram_tracks(const char* variable, TTree* tree, TCut T, TH1D* h, float scale) {
    vector<float> values;
    tree->SetBranchAddress(variable, &values);
    vector<bool> trackpass_nom;
    tree->SetBranchAddress("trkPassChargedHadron_Nominal", &trackpass_nom);

    TTreeFormula formula("formula", variable, tree);
    for(int i = 0; i< tree->GetEntries(); i++) {
        tree->GetEntry(i);
        if(i%1000 == 0) {
            std::cout << "Processing entry " << i << " of " << tree->GetEntries() << std::endl;
        }
        double value = formula.EvalInstance();
        if (formula.EvalInstance() == false) {continue;}
        for(int j = 0; j < values.size(); j++){
            if(trackpass_nom[j]) {
                h->Fill(values[j]);
            }
        }
    }
    TCut viable("HLT_MinimumBiasHF_OR_BptxAND_v1");
    //h->Scale(scale/tree->GetEntries(viable));
    h->Scale(1.0/h->Integral());
}

// 1. Chi-square function
float chi2Hist(TH1D* h1, TH1D* h2, double xmin, double xmax){

    int blo = h1->FindBin(xmin);  
    int bhi = h1->FindBin(xmax);
    float chisq = 0;
    for(int i = blo; i <= bhi; ++i) {
        double data = h1->GetBinContent(i);
        double x = h1->GetBinCenter(i);
        double mc = h2->GetBinContent(h2->FindBin(x));

        double error = h1->GetBinError(i);
        
        if (error == 0) continue; // Avoid division by zero
        
        float diff = (data - mc) / error;
        chisq += diff * diff;
    }
    return chisq;
}

// 2. Shift and scale function
TH1* ShiftAndScaleHist(TH1* h, TF1* shiftFunc, const std::vector<double>& pars) {
    int nShiftPars = pars.size() - 1;
    double scale = pars.back();
    for (int k = 0; k < nShiftPars; ++k) {
        shiftFunc->SetParameter(k, pars[k]);
    }
    TH1* hNew = (TH1*)h->Clone("hShifted");
    hNew->Reset();
    for (int i = 1; i <= h->GetNbinsX(); ++i) {
        double x = h->GetBinCenter(i);
        double xprime = shiftFunc->Eval(x);
        double y = h->Interpolate(xprime);
        hNew->SetBinContent(i, y);
        hNew->SetBinError(i, h->GetBinError(i));
    }
    hNew->Scale(scale*h->Integral() / hNew->Integral());
    return hNew;
}

// 3. Minimization function
void FitFcn(Int_t& npar, Double_t* grad, Double_t& fval, Double_t* par, Int_t flag) {
    std::vector<double> pars(par, par + npar);
    TH1* hShifted = ShiftAndScaleHist(gTemplateHist, gShiftFunc, pars);
    fval = chi2Hist((TH1D*)gDataHist, (TH1D*)hShifted, gFitMin, gFitMax);

    if(gConstraint == 1){
        double constraint = pars[0] + pars[1]*4 + pars[2]*16 - 2;
        fval += 5e2 * constraint * constraint; // Large penalty if constraint is violated
    }

    delete hShifted;
}

std::vector<double> MinimizeFit(
    TH1* data, TH1* templ, TF1* shiftFunc, double xmin, double xmax,
    const std::vector<double>& initial = std::vector<double>(), int constraint = 0
) {
    gDataHist = data;
    gTemplateHist = templ;
    gShiftFunc = shiftFunc;
    gFitMin = xmin;
    gFitMax = xmax;
    gConstraint = constraint;

    int npars = shiftFunc->GetNpar() + 1; // shift params + scale
    TMinuit minuit(npars);
    minuit.SetFCN(FitFcn);

    std::vector<double> vstart(npars, 0.0);
    if (!initial.empty()) {
        for (int i = 0; i < npars && i < (int)initial.size(); ++i) {
            vstart[i] = initial[i];
        }
    } else {
        vstart[1] = 1.0; // Example: set linear term to 1
        vstart.back() = 1.0; // scale
    }
    std::vector<double> step(npars, 0.01);

    for (int i = 0; i < npars - 1; ++i) {
        minuit.DefineParameter(i, Form("p%d", i), vstart[i], step[i], 0, 0);
    }
    minuit.DefineParameter(npars - 1, "scale", vstart[npars - 1], step[npars - 1], 0, 0);

    minuit.Migrad();

    std::vector<double> outpar(npars), err(npars);
    for (int i = 0; i < npars; ++i) {
        minuit.GetParameter(i, outpar[i], err[i]);
        std::cout << "Parameter " << i << ": " << outpar[i] << " +/- " << err[i] << std::endl;
    }
    std::cout << "Relative fraction (scale): " << outpar.back() << std::endl;

    return outpar;
}

void shift(){

    /// SPECIFY THE INPUTS YOU DESIRE HERE
    TFile* fData = TFile::Open("/data00/kdeverea/OOsamples/Skims/output_20250730_Skim_OO_IonPhysics0_LowPtV2_250711_104114_MB_CROSSCHECK/merged_0-49.root");
    TFile* fMC = TFile::Open("/data00/kdeverea/OOsamples/Skims/output_20250728_Skim_OO_MinBias_OO_5p36TeV_hijing/20250728_Skim_OO_MinBias_OO_5p36TeV_hijing.root");
    TFile* fMC_Arg = TFile::Open("/data00/kdeverea/OOsamples/Skims/output_20250724_Skim_MinBias_Pythia_Angantyr_OO_5362GeV/20250724_Skim_MinBias_Pythia_Angantyr_OO_5362GeV.root");

    TTree* dataTree = (TTree*)fData->Get("Tree");
    TTree* MCTree = (TTree*)fMC->Get("Tree");
    TTree* MCTree_Arg = (TTree*)fMC_Arg->Get("Tree");

    TCut Datacut = "(VZ > -15 && VZ < 15) && (PVFilter == 1) && (ClusterCompatibilityFilter == 1)&& (leadingPtEta1p0_sel > 5.0)";
    TCut MCCut = "(VZ > -15 && VZ < 15) && (PVFilter == 1) && (ClusterCompatibilityFilter == 1) && (leadingPtEta1p0_sel > 5.0)";

    // DEFINE HISTOGRAMS
    TString hfemin = Form("TMath::Min(%s,%s)", "HFEMaxPlus", "HFEMaxMinus");

    TH1D* data_hfe = new TH1D("data_hfe", "TMath::Max(HFEMaxPlus; HFEMax Ming (GeV); A.U.", 300, 0, 600);
    TH1D* mc_hfe = new TH1D("mc_hfe", "HFEMaxPlus; HFEMax Min (GeV); A.U.", 300, 0, 600);
    
    GetHistogram(hfemin.Data(), dataTree, Datacut, data_hfe, 1.3);
    GetHistogram(hfemin.Data(), MCTree, MCCut, mc_hfe, 1.3);

    // SCALE MC TO DAT
    float xmin = 40; 
    float xmax = 400;
    TF1* shiftFunc = new TF1("shiftFunc", "[0] + [1]*x + [2]*x*x", xmin, xmax);
    std::vector<double> initialparams = {  -0.398937, 0.8308, 0.00161735, 0.992122};
    int useConstraint = 1; // Set to 1 to enable constraint, 0 to disable
    std::vector<double> bestPars = MinimizeFit(data_hfe, mc_hfe, shiftFunc, xmin, xmax, initialparams, useConstraint);

    TH1* shifted = ShiftAndScaleHist(mc_hfe, shiftFunc, bestPars);
    TH1D* shiftedD = (TH1D*)shifted;

    /// WRITE METRICS
    float chi2 = chi2Hist(data_hfe, shiftedD, xmin, xmax);
    int bin_lo = data_hfe->FindBin(xmin);
    int bin_hi = data_hfe->FindBin(xmax);
    int nfitbins = bin_hi - bin_lo + 1;
    int npars = bestPars.size();
    float ndf = nfitbins - npars;
    std::cout << "Final chi2/ndf: " << chi2 << " / " << ndf << " = " << (chi2/ndf) << std::endl;

    float efficiency = 1.0 - shiftedD->Integral(shiftedD->FindBin(0),shiftedD->FindBin(13)) / mc_hfe->Integral();
    float efficiency_data = 1.0 - data_hfe->Integral(data_hfe->FindBin(0),data_hfe->FindBin(13)) / data_hfe->Integral();
    std::cout << "Efficiency of shifted MC: " << efficiency << std::endl;
    std::cout << "Efficiency of data: " << efficiency_data << std::endl;

    /// Write params and files to output file
    TFile* outFile = TFile::Open("nene_hfe_output.root", "RECREATE");

    // Write histograms
    data_hfe->Write();
    mc_hfe->Write();
    shiftedD->Write();

    // Write fit parameters
    TNamed* fitParams = new TNamed("fitParams", Form("Shift: %f, %f, %f; Scale: %f",
        bestPars[0], bestPars[1], bestPars[2], bestPars[3]));
    fitParams->Write();

    // Write fit quality information
    TNamed* fitQuality = new TNamed("fitQuality", Form("chi2: %.2f; ndf: %.0f; chi2/ndf: %.3f; xmin: %.0f; xmax: %.0f",
        chi2, ndf, chi2/ndf, xmin, xmax));
    fitQuality->Write();

    outFile->Close();
}