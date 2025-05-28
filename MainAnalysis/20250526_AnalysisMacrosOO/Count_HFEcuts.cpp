#include <iostream>
#include <TFile.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLegend.h>

using namespace std;

#include <utility>

pair<double, int> counting(const char* inFileName, float cut = 0, bool cutBool = false, float leadingPtSelection = 0) {
    cout << "------- Counting  -------" << endl;

    TFile* inFile = new TFile(inFileName,"READ");
    if (!inFile || inFile->IsZombie()) {
        cout << "Error: Could not open the file!" << endl;
        return make_pair(0.0, 0);
    }

    TTree* tree = (TTree*)inFile->Get("Tree");
    if (!tree) {
        cout << "Error: Could not find the tree!" << endl;
        return make_pair(0.0, 0);
    }

    // Set up the branches
    
    int CCFilter, PVFilter, nVtx;
    float HFEMaxPlus, HFEMaxMinus;
    float VZ, leadingPtEta1p0_sel;

    tree->SetBranchAddress("ClusterCompatibilityFilter", &CCFilter);
    tree->SetBranchAddress("PVFilter", &PVFilter);
    tree->SetBranchAddress("nVtx", &nVtx);
    tree->SetBranchAddress("VZ", &VZ);
    tree->SetBranchAddress("HFEMaxPlus", &HFEMaxPlus);
    tree->SetBranchAddress("HFEMaxMinus", &HFEMaxMinus);
    tree->SetBranchAddress("leadingPtEta1p0_sel",&leadingPtEta1p0_sel);

    const Long64_t nEvt = tree->GetEntries();

    int eventCounter = 0, subeventCounter = 0;
    int eventCounterTotal = 0;
    int CC0Counter = 0, CC1Counter = 0, PV0Counter = 0, PV1Counter = 0, nVtxCounter = 0, VZCounter = 0;

    for (int i = 0; i < nEvt; i++) {
        tree->GetEntry(i);
        if (CCFilter == 1 && PVFilter == 1 && nVtx > 0 && abs(VZ) < 15 && (leadingPtSelection == 0 ? leadingPtEta1p0_sel > 2 : true)) {
            eventCounterTotal ++;
            if (cutBool == false){
                if (HFEMaxPlus > cut || HFEMaxMinus > cut){
                    eventCounter++;
                }
            }
            else if (cutBool == true){
                if (HFEMaxPlus > cut && HFEMaxMinus > cut){
                    eventCounter++;
                }
            }
        }
    }

    double ratio = static_cast<double>(eventCounter) / eventCounterTotal;
    cout << "Ratio between Original and Filtered: " << ratio << endl;
    cout << "Total number of events: " << nEvt << endl;
    cout << "Total number of events after HFEcut "<< cut << ": " <<eventCounter << endl;
    cout << endl;

    cout << "------- Count Complete -------" << endl;
    return make_pair(ratio, eventCounter);
}

pair<double, int> counting_Jingcuts(const char* inFileName, float cut = 0, bool cutBool = false) {
    cout << "------- Counting (Cuts Same as Jing) -------" << endl;

    TFile* inFile = new TFile(inFileName,"READ");
    if (!inFile || inFile->IsZombie()) {
        cout << "Error: Could not open the file!" << endl;
        return make_pair(0.0, 0);
    }

    TTree* tree = (TTree*)inFile->Get("Tree");
    if (!tree) {
        cout << "Error: Could not find the tree!" << endl;
        return make_pair(0.0, 0);
    }

    // Set up the branches
    
    int CCFilter, PVFilter, nVtx;
    float HFEMaxPlus, HFEMaxMinus;
    float VZ, leadingPtEta1p0_sel;

    tree->SetBranchAddress("ClusterCompatibilityFilter", &CCFilter);
    tree->SetBranchAddress("PVFilter", &PVFilter);
    tree->SetBranchAddress("nVtx", &nVtx);
    tree->SetBranchAddress("VZ", &VZ);
    tree->SetBranchAddress("HFEMaxPlus", &HFEMaxPlus);
    tree->SetBranchAddress("HFEMaxMinus", &HFEMaxMinus);
    tree->SetBranchAddress("leadingPtEta1p0_sel",&leadingPtEta1p0_sel);

    const Long64_t nEvt = tree->GetEntries();

    int eventCounter = 0, subeventCounter = 0;
    int eventCounterTotal = 0;
    int CC0Counter = 0, CC1Counter = 0, PV0Counter = 0, PV1Counter = 0, nVtxCounter = 0, VZCounter = 0;

    for (int i = 0; i < nEvt; i++) {
        tree->GetEntry(i);
        if (CCFilter == 1 && PVFilter == 1 && abs(VZ) < 15) {
            eventCounterTotal ++;
            if (cutBool == false){
                if (HFEMaxPlus > cut || HFEMaxMinus > cut){
                    eventCounter++;
                }
            }
            else if (cutBool == true){
                if (HFEMaxPlus > cut && HFEMaxMinus > cut){
                    eventCounter++;
                }
            }
        }
    }

    double ratio = static_cast<double>(eventCounter) / eventCounterTotal;
    cout << "Ratio between Original and Filtered: " << ratio << endl;
    cout << "Total number of events: " << nEvt << endl;
    cout << "Total number of events after HFEcut "<< cut << ": " <<eventCounter << endl;
    cout << endl;

    cout << "------- Count Complete -------" << endl;
    return make_pair(ratio, eventCounter);
}


void ROCrun() {
    const char* HIJINGFile = "/data/public/bakovacs/OOskims_20250527/skim_HiForest_250520_Hijing_MinimumBias_b015_OO_5362GeV_250518.root";
    const char* StarlightSD = "/data/public/bakovacs/OOskims_20250527/skim_HiForest_250514_Starlight_SingleDiffraction_OO_5362GeV_1505PR47944.root";

    //const char* HIJINGFile = "/data00/OOsamples/Skims/skim_HiForest_250520_Hijing_MinimumBias_b015_OO_5362GeV_250518.root";
   // const char* StarlightSD = "/data00/OOsamples/Skims/skim_HiForest_250514_Starlight_SingleDiffraction_OO_5362GeV_1505PR47944.root";
    //const char* BalazsHIJING = "/data/public/bakovacs/OOskims_20250527/skim_HiForest_250520_Hijing_MinimumBias_b015_OO_5362GeV_250518.root";
   // const char* inFileName = HIJINGFile;

    const int N = 51;
    float xMax = 50.0;
    double x[N], y[N], y2[N];
    double EfficiencyHijingAND[N], EfficiencyHijingOR[N];
    double EfficiencySDAND[N], EfficiencySDOR[N];
    double BKGRejectionAND[N], BKGRejectionOR[N];
    double PurityAND[N], PurityOR[N];
    float xsec_EM = 0.3;
    float xsec_had = 1.08; // Assuming these are the cross-sections for EM and hadronic processes
    float leadingPtCut = 0.0;

    TH1F* h_HijingAND = new TH1F("h_HijingAND", "Event Selection Efficiency with different HFE_{max} cuts (HIJING)", N, 0, xMax);
    TH1F* h_HijingOR = new TH1F("h_HijingOR", "", N, 0, xMax);

    TH1F* h_SDAND = new TH1F("h_SDAND", "Event Selection Efficiency with different HFE_{max} cuts (SD)", N, 0, xMax);
    TH1F* h_SDOR = new TH1F("h_SDOR", "", N, 0, xMax);

    TCanvas* c1 = new TCanvas("c1", "Number of Events with each Filter", 800, 600);
    TCanvas* c2 = new TCanvas("c2", "Number of Events with each Filter", 800, 600);

    for (int i = 0; i < N; ++i) {
        x[i] = xMax * i / (N - 1); // Uniform spacing from 0 to xMax
        EfficiencyHijingAND[i] = counting(HIJINGFile, x[i], true,leadingPtCut).first;
        EfficiencyHijingOR[i] = counting(HIJINGFile, x[i], false,leadingPtCut).first;
        EfficiencySDAND[i] = counting(StarlightSD, x[i], true,leadingPtCut).first;
        EfficiencySDOR[i] = counting(StarlightSD, x[i], false,leadingPtCut).first;
        BKGRejectionAND[i] = 1.0 - EfficiencySDAND[i];
        BKGRejectionOR[i] = 1.0 - EfficiencySDOR[i];

        PurityAND[i] = 1 - (xsec_EM * EfficiencySDAND[i]) / (xsec_EM * EfficiencySDAND[i] + xsec_had * EfficiencyHijingAND[i]);
        PurityOR[i] = 1 - (xsec_EM * EfficiencySDOR[i]) / (xsec_EM * EfficiencySDOR[i] + xsec_had * EfficiencyHijingOR[i]);

        cout << "Purity AND for  " << x[i] << ": " << PurityAND[i] << endl;
        cout << "Purity OR for  " << x[i] << ": " << PurityOR[i] << endl;

        h_HijingAND->SetBinContent(i + 1, EfficiencyHijingAND[i]);
        h_HijingOR->SetBinContent(i + 1, EfficiencyHijingOR[i]);
        h_SDAND->SetBinContent(i + 1, EfficiencySDAND[i]);
        h_SDOR->SetBinContent(i + 1, EfficiencySDOR[i]);

        // Only set bin label at integer multiples of 5
        if (i == 0 || i == N - 1 || (static_cast<int>(x[i]) % 5 == 0 && fabs(x[i] - round(x[i])) < 1e-3)) {
            h_HijingAND->GetXaxis()->SetBinLabel(i + 1, Form("%.0f", x[i]));
            h_HijingOR->GetXaxis()->SetBinLabel(i + 1, Form("%.0f", x[i]));
        } else {
            h_HijingAND->GetXaxis()->SetBinLabel(i + 1, "");
            h_HijingOR->GetXaxis()->SetBinLabel(i + 1, "");
        }
    }
    
    TGraph *gr_ROC_AND = new TGraph(N, EfficiencyHijingAND, BKGRejectionAND);
    TGraph *gr_ROC_OR = new TGraph(N, EfficiencyHijingOR, BKGRejectionOR);

    TGraph *gr_Purity_AND = new TGraph(N, EfficiencyHijingAND, PurityAND);
    TGraph *gr_Purity_OR = new TGraph(N, EfficiencyHijingOR, PurityOR);

    c1->cd();   
    gr_ROC_AND->SetTitle("ROC Curve of Bkg Rejection vs HIJING;Efficiency(HIJING);Background Rejection (StarLight SD)");
    gr_ROC_AND->SetLineColor(kBlue);
    gr_ROC_AND->SetLineWidth(2);
    gr_ROC_AND->Draw("AL"); 
    gr_ROC_AND->GetYaxis()->SetRangeUser(0, 1);
    gStyle->SetOptStat(0);

    gr_ROC_OR->SetTitle("ROC Curve of Bkg Rejection;Efficiency(HIJING);Background Rejection (StarLight SD)");
    gr_ROC_OR->SetLineColor(kRed);
    gr_ROC_OR->SetLineWidth(2);
    gr_ROC_OR->Draw("AL SAME"); 
    gr_ROC_OR->GetYaxis()->SetRangeUser(0, 1);
    gStyle->SetOptStat(0);

    c1->SetGrid(); // add grid to ROC curve

    c2->cd();
    gr_Purity_AND->SetTitle("Purity Curve (HIJING and Starlight SD);Efficiency(HIJING);Purity (StarLight SD)");
    gr_Purity_AND->SetLineColor(kBlue);
    gr_Purity_AND->SetLineWidth(2);
    gr_Purity_AND->Draw("ALP");
    gr_Purity_AND->GetYaxis()->SetRangeUser(0, 1.05);
    gStyle->SetOptStat(0);
    
    gr_Purity_OR->SetTitle("Purity Curve (HIJING and Starlight SD);Efficiency(HIJING);Purity (StarLight SD)");
    gr_Purity_OR->SetLineColor(kRed);
    gr_Purity_OR->SetLineWidth(2);
    gr_Purity_OR->Draw("ALP SAME");
    gr_Purity_OR->GetYaxis()->SetRangeUser(0.8, 1.05);
    gr_Purity_OR->GetXaxis()->SetRangeUser(0.6, 1);
    gStyle->SetOptStat(0);

    c2->SetGrid(); // add grid to Purity curve
    
    // Draw four lines of text in the left hand bottom corner, moved further down

    c1->cd();
    TLatex latex;
    latex.SetNDC();
    latex.SetTextSize(0.025);
    latex.DrawLatex(0.72, 0.85, "HIJING (1.08 b)");
    latex.DrawLatex(0.72, 0.82, "Starlight SD (0.3 b)");
  //  latex.DrawLatex(0.23, 0.33, Form("leadingPtEta1p0_{sel} > %.1f GeV/c", leadingPtCut));
    latex.DrawLatex(0.23, 0.30, "PVFilter == 1");
    latex.DrawLatex(0.23, 0.27, "ClusterCompatibilityFilter == 1");
    latex.DrawLatex(0.23, 0.24, "nVtx > 0");
    latex.DrawLatex(0.23, 0.21, "abs(V_{z}) < 15");

    c2->cd();
    TLatex latex2;
    latex2.SetNDC();
    latex2.SetTextSize(0.025);
    latex2.DrawLatex(0.72, 0.85, "HIJING (1.08 b)");
    latex2.DrawLatex(0.72, 0.82, "Starlight SD (0.3 b)");
  //  latex2.DrawLatex(0.23, 0.33, Form("leadingPtEta1p0_{sel} > %.1f GeV/c", leadingPtCut));
    latex2.DrawLatex(0.23, 0.30, "PVFilter == 1");
    latex2.DrawLatex(0.23, 0.27, "ClusterCompatibilityFilter == 1");
    latex2.DrawLatex(0.23, 0.24, "nVtx > 0");
    latex2.DrawLatex(0.23, 0.21, "abs(V_{z}) < 15");


    c1->SaveAs("Plots/Filter Efficiency Results/ROCcurve_new.png");
    c2->SaveAs("Plots/Filter Efficiency Results/Puritycurve_new.png");
}

/*
void drawHistRun_JingComparison() {
    const char* HIJINGFile = "/data00/OOsamples/Skims/skim_HiForest_250520_Hijing_MinimumBias_b015_OO_5362GeV_250518.root";
    const char* StarlightSD = "/data00/OOsamples/Skims/skim_HiForest_250514_Starlight_SingleDiffraction_OO_5362GeV_1505PR47944.root";
    const char* StarlightDD = "/data00/OOsamples/Skims/skim_HiForest_250514_Starlight_DoubleDiffraction_OO_5362GeV_1505PR47944.root";

    const int N = 91;
    float xMax = 30.0;
    double x[N], y[N], y2[N];
    double EfficiencyHijingAND[N], EfficiencyHijingOR[N];
    double EfficiencySDAND[N], EfficiencySDOR[N];
    double EfficiencyDDAND[N], EfficiencyDDOR[N];
    //float leadingPtCut = 5.0;

    TH1F* h_HijingAND = new TH1F("h_HijingAND", "Event Selection Efficiency with different HFE_{max} cuts (HIJING)", N, 0, xMax);
    TH1F* h_HijingOR = new TH1F("h_HijingOR", "", N, 0, xMax);

    TH1F* h_SDAND = new TH1F("h_SDAND", "Event Selection Efficiency with different HFE_{max} cuts (HIJING)", N, 0, xMax);
    TH1F* h_SDOR = new TH1F("h_SDOR", "", N, 0, xMax);

    TH1F* h_DDAND = new TH1F("h_DDAND", "Event Selection Efficiency with different HFE_{max} cuts (HIJING)", N, 0, xMax);
    TH1F* h_DDOR = new TH1F("h_DDOR", "", N, 0, xMax);

    TCanvas* c1 = new TCanvas("c1", "Number of Events with each Filter", 800, 600);
    TCanvas* c2 = new TCanvas("c2", "Number of Events with each Filter", 800, 600);
    TCanvas* c_DD = new TCanvas("c_DD", "Number of Events with each Filter", 800, 600);

    for (int i = 0; i < N; ++i) {
        x[i] = xMax * i / (N - 1); // Uniform spacing from 0 to xMax
        EfficiencyHijingAND[i] = counting_Jingcuts(HIJINGFile, x[i], true).first;
        EfficiencyHijingOR[i] = counting_Jingcuts(HIJINGFile, x[i], false).first;
        EfficiencySDAND[i] = counting_Jingcuts(StarlightSD, x[i], true).first;
        EfficiencySDOR[i] = counting_Jingcuts(StarlightSD, x[i], false).first;
        EfficiencyDDAND[i] = counting_Jingcuts(StarlightDD, x[i], true).first;
        EfficiencyDDOR[i] = counting_Jingcuts(StarlightDD, x[i], false).first;

        h_HijingAND->SetBinContent(i + 1, EfficiencyHijingAND[i]);
        h_HijingOR->SetBinContent(i + 1, EfficiencyHijingOR[i]);
        h_SDAND->SetBinContent(i + 1, EfficiencySDAND[i]);
        h_SDOR->SetBinContent(i + 1, EfficiencySDOR[i]);
        h_DDAND->SetBinContent(i + 1, EfficiencyDDAND[i]);
        h_DDOR->SetBinContent(i + 1, EfficiencyDDOR[i]);
        // Only set bin label at integer multiples of 5
        if (i == 0 || i == N - 1 || (static_cast<int>(x[i]) % 5 == 0 && fabs(x[i] - round(x[i])) < 1e-3)) {
            h_HijingAND->GetXaxis()->SetBinLabel(i + 1, Form("%.0f", x[i]));
            h_HijingOR->GetXaxis()->SetBinLabel(i + 1, Form("%.0f", x[i]));
            h_SDAND->GetXaxis()->SetBinLabel(i + 1, Form("%.0f", x[i]));
            h_SDOR->GetXaxis()->SetBinLabel(i + 1, Form("%.0f", x[i]));
            h_DDAND->GetXaxis()->SetBinLabel(i + 1, Form("%.0f", x[i]));
            h_DDOR->GetXaxis()->SetBinLabel(i + 1, Form("%.0f", x[i]));
        } else {
            h_HijingAND->GetXaxis()->SetBinLabel(i + 1, "");
            h_HijingOR->GetXaxis()->SetBinLabel(i + 1, "");
            h_SDAND->GetXaxis()->SetBinLabel(i + 1, "");
            h_SDOR->GetXaxis()->SetBinLabel(i + 1, "");
            h_DDAND->GetXaxis()->SetBinLabel(i + 1, "");
            h_DDOR->GetXaxis()->SetBinLabel(i + 1, "");
        }
    }
    
    c1->cd();
    h_HijingAND->Draw("HIST");
    h_HijingOR->Draw("HIST SAME");
    gStyle->SetOptStat(0);

    h_HijingAND->GetYaxis()->SetRangeUser(0, 1);
    h_HijingAND->GetXaxis()->SetTitle("Cut value (X)");
    h_HijingAND->GetYaxis()->SetTitle("Percent of Events Survived");

    h_HijingAND->SetLineColor(kBlue);
    h_HijingAND->SetLineWidth(2);
    h_HijingOR->SetLineColor(kRed);
    h_HijingOR->SetLineWidth(2);

    c2->cd();
    h_SDAND->Draw("HIST");
    h_SDOR->Draw("HIST SAME");
    gStyle->SetOptStat(0);

    h_SDAND->GetYaxis()->SetRangeUser(0, 1);
    h_SDAND->GetXaxis()->SetTitle("Cut value (X)");
    h_SDAND->GetYaxis()->SetTitle("Percent of Events Survived");

    h_SDAND->SetLineColor(kBlue);
    h_SDAND->SetLineWidth(2);
    h_SDOR->SetLineColor(kRed);
    h_SDOR->SetLineWidth(2);

    c_DD->cd();
    h_DDAND->Draw("HIST");
    h_DDOR->Draw("HIST SAME");
    gStyle->SetOptStat(0);

    h_DDAND->GetYaxis()->SetRangeUser(0, 1);
    h_DDAND->GetXaxis()->SetTitle("Cut value (X)");
    h_DDAND->GetYaxis()->SetTitle("Percent of Events Survived");

    h_DDAND->SetLineColor(kBlue);
    h_DDAND->SetLineWidth(2);
    h_DDOR->SetLineColor(kRed);
    h_DDOR->SetLineWidth(2);


    // Legend for c1 (HIJING)
    c1->cd();
    TLegend *leg1 = new TLegend(0.72, 0.3, 0.95, 0.4); // move to lower right corner, y-range at 0.3-0.4
    leg1->SetTextSize(0.022); // slightly smaller text
    leg1->SetMargin(0.15); // reduce left margin for tighter spacing
    leg1->SetEntrySeparation(0.18); // reduce vertical spacing between entries
    leg1->AddEntry(h_HijingAND, "HFE_{max}^{+} > X AND HFE_{max}^{-} > X", "l");
    leg1->AddEntry(h_HijingOR, "HFE_{max}^{+} > X OR HFE_{max}^{-} > X", "l");
    leg1->Draw();
    c1->SetGrid();

    // Legend for c2 (Starlight SD)
    c2->cd();
    TLegend *leg2 = new TLegend(0.72, 0.3, 0.95, 0.4);
    leg2->SetTextSize(0.022);
    leg2->SetMargin(0.15);
    leg2->SetEntrySeparation(0.18);
    leg2->AddEntry(h_SDAND, "HFE_{max}^{+} > X AND HFE_{max}^{-} > X", "l");
    leg2->AddEntry(h_SDOR, "HFE_{max}^{+} > X OR HFE_{max}^{-} > X", "l");
    leg2->Draw();
    c2->SetGrid();

    c_DD->cd();
    TLegend *leg_DD = new TLegend(0.72, 0.3, 0.95, 0.4);
    leg_DD->SetTextSize(0.022);
    leg_DD->SetMargin(0.15);
    leg_DD->SetEntrySeparation(0.18);
    leg_DD->AddEntry(h_DDAND, "HFE_{max}^{+} > X AND HFE_{max}^{-} > X", "l");
    leg_DD->AddEntry(h_DDOR, "HFE_{max}^{+} > X OR HFE_{max}^{-} > X", "l");
    leg_DD->Draw();
    c_DD->SetGrid();
    
    // Draw text on c1
    c1->cd();
    TLatex latex1;
    latex1.SetNDC();
    latex1.SetTextSize(0.025);
    latex1.DrawLatex(0.23, 0.33, Form("leadingPtEta1p0_{sel} > %.1f GeV/c", leadingPtCut));
    latex1.DrawLatex(0.23, 0.30, "PVFilter == 1");
    latex1.DrawLatex(0.23, 0.27, "ClusterCompatibilityFilter == 1");
    latex1.DrawLatex(0.23, 0.24, "nVtx > 0");
    latex1.DrawLatex(0.23, 0.21, "abs(V_{z}) < 15");

    // Draw text on c2
    c2->cd();
    TLatex latex2;
    latex2.SetNDC();
    latex2.SetTextSize(0.025);
    latex2.DrawLatex(0.23, 0.33, Form("leadingPtEta1p0_{sel} > %.1f GeV/c", leadingPtCut));
    latex2.DrawLatex(0.23, 0.30, "PVFilter == 1");
    latex2.DrawLatex(0.23, 0.27, "ClusterCompatibilityFilter == 1");
    latex2.DrawLatex(0.23, 0.24, "nVtx > 0");
    latex2.DrawLatex(0.23, 0.21, "abs(V_{z}) < 15");

    c_DD->cd();
    TLatex latex_DD;
    latex_DD.SetNDC();
    latex_DD.SetTextSize(0.025);
    latex_DD.DrawLatex(0.23, 0.33, Form("leadingPtEta1p0_{sel} > %.1f GeV/c", leadingPtCut));
    latex_DD.DrawLatex(0.23, 0.30, "PVFilter == 1");
    latex_DD.DrawLatex(0.23, 0.27, "ClusterCompatibilityFilter == 1");
    latex_DD.DrawLatex(0.23, 0.24, "nVtx > 0");
    latex_DD.DrawLatex(0.23, 0.21, "abs(V_{z}) < 15");

    c1->SaveAs(Form("Plots/Filter Efficiency Results/countingHFE_HIJING_AfterCuts_Hist_leadingptsel%.0f.png", leadingPtCut));
    c2->SaveAs(Form("Plots/Filter Efficiency Results/countingHFE_SD_AfterCuts_Hist_leadingptsel%.0f.png", leadingPtCut));
    c_DD->SaveAs(Form("Plots/Filter Efficiency Results/countingHFE_DD_AfterCuts_Hist_leadingptsel%.0f.png", leadingPtCut));
}*/

void drawHistRun() {
    const char* HIJINGFile = "/data00/OOsamples/Skims/skim_HiForest_250520_Hijing_MinimumBias_b015_OO_5362GeV_250518.root";
    const char* StarlightSD = "/data00/OOsamples/Skims/skim_HiForest_250514_Starlight_SingleDiffraction_OO_5362GeV_1505PR47944.root";
    const char* StarlightDD = "/data00/OOsamples/Skims/skim_HiForest_250514_Starlight_DoubleDiffraction_OO_5362GeV_1505PR47944.root";

    const int N = 91;
    float xMax = 30.0;
    double x[N], y[N], y2[N];
    double EfficiencyHijingAND[N], EfficiencyHijingOR[N];
    double EfficiencySDAND[N], EfficiencySDOR[N];
    double EfficiencyDDAND[N], EfficiencyDDOR[N];
    float leadingPtCut = 5.0;

    TH1F* h_HijingAND = new TH1F("h_HijingAND", "Event Selection Efficiency with different HFE_{max} cuts (HIJING)", N, 0, xMax);
    TH1F* h_HijingOR = new TH1F("h_HijingOR", "", N, 0, xMax);

    TH1F* h_SDAND = new TH1F("h_SDAND", "Event Selection Efficiency with different HFE_{max} cuts (HIJING)", N, 0, xMax);
    TH1F* h_SDOR = new TH1F("h_SDOR", "", N, 0, xMax);

    TH1F* h_DDAND = new TH1F("h_DDAND", "Event Selection Efficiency with different HFE_{max} cuts (HIJING)", N, 0, xMax);
    TH1F* h_DDOR = new TH1F("h_DDOR", "", N, 0, xMax);

    TCanvas* c1 = new TCanvas("c1", "Number of Events with each Filter", 800, 600);
    TCanvas* c2 = new TCanvas("c2", "Number of Events with each Filter", 800, 600);
    TCanvas* c_DD = new TCanvas("c_DD", "Number of Events with each Filter", 800, 600);

    for (int i = 0; i < N; ++i) {
        x[i] = xMax * i / (N - 1); // Uniform spacing from 0 to xMax
        EfficiencyHijingAND[i] = counting(HIJINGFile, x[i], true,leadingPtCut).first;
        EfficiencyHijingOR[i] = counting(HIJINGFile, x[i], false,leadingPtCut).first;
        EfficiencySDAND[i] = counting(StarlightSD, x[i], true,leadingPtCut).first;
        EfficiencySDOR[i] = counting(StarlightSD, x[i], false,leadingPtCut).first;
        EfficiencyDDAND[i] = counting(StarlightDD, x[i], true,leadingPtCut).first;
        EfficiencyDDOR[i] = counting(StarlightDD, x[i], false,leadingPtCut).first;

        h_HijingAND->SetBinContent(i + 1, EfficiencyHijingAND[i]);
        h_HijingOR->SetBinContent(i + 1, EfficiencyHijingOR[i]);
        h_SDAND->SetBinContent(i + 1, EfficiencySDAND[i]);
        h_SDOR->SetBinContent(i + 1, EfficiencySDOR[i]);
        h_DDAND->SetBinContent(i + 1, EfficiencyDDAND[i]);
        h_DDOR->SetBinContent(i + 1, EfficiencyDDOR[i]);
        // Only set bin label at integer multiples of 5
        if (i == 0 || i == N - 1 || (static_cast<int>(x[i]) % 5 == 0 && fabs(x[i] - round(x[i])) < 1e-3)) {
            h_HijingAND->GetXaxis()->SetBinLabel(i + 1, Form("%.0f", x[i]));
            h_HijingOR->GetXaxis()->SetBinLabel(i + 1, Form("%.0f", x[i]));
            h_SDAND->GetXaxis()->SetBinLabel(i + 1, Form("%.0f", x[i]));
            h_SDOR->GetXaxis()->SetBinLabel(i + 1, Form("%.0f", x[i]));
            h_DDAND->GetXaxis()->SetBinLabel(i + 1, Form("%.0f", x[i]));
            h_DDOR->GetXaxis()->SetBinLabel(i + 1, Form("%.0f", x[i]));
        } else {
            h_HijingAND->GetXaxis()->SetBinLabel(i + 1, "");
            h_HijingOR->GetXaxis()->SetBinLabel(i + 1, "");
            h_SDAND->GetXaxis()->SetBinLabel(i + 1, "");
            h_SDOR->GetXaxis()->SetBinLabel(i + 1, "");
            h_DDAND->GetXaxis()->SetBinLabel(i + 1, "");
            h_DDOR->GetXaxis()->SetBinLabel(i + 1, "");
        }
    }
    
    c1->cd();
    h_HijingAND->Draw("HIST");
    h_HijingOR->Draw("HIST SAME");
    gStyle->SetOptStat(0);

    h_HijingAND->GetYaxis()->SetRangeUser(0, 1);
    h_HijingAND->GetXaxis()->SetTitle("Cut value (X)");
    h_HijingAND->GetYaxis()->SetTitle("Percent of Events Survived");

    h_HijingAND->SetLineColor(kBlue);
    h_HijingAND->SetLineWidth(2);
    h_HijingOR->SetLineColor(kRed);
    h_HijingOR->SetLineWidth(2);

    c2->cd();
    h_SDAND->Draw("HIST");
    h_SDOR->Draw("HIST SAME");
    gStyle->SetOptStat(0);

    h_SDAND->GetYaxis()->SetRangeUser(0, 1);
    h_SDAND->GetXaxis()->SetTitle("Cut value (X)");
    h_SDAND->GetYaxis()->SetTitle("Percent of Events Survived");

    h_SDAND->SetLineColor(kBlue);
    h_SDAND->SetLineWidth(2);
    h_SDOR->SetLineColor(kRed);
    h_SDOR->SetLineWidth(2);

    c_DD->cd();
    h_DDAND->Draw("HIST");
    h_DDOR->Draw("HIST SAME");
    gStyle->SetOptStat(0);

    h_DDAND->GetYaxis()->SetRangeUser(0, 1);
    h_DDAND->GetXaxis()->SetTitle("Cut value (X)");
    h_DDAND->GetYaxis()->SetTitle("Percent of Events Survived");

    h_DDAND->SetLineColor(kBlue);
    h_DDAND->SetLineWidth(2);
    h_DDOR->SetLineColor(kRed);
    h_DDOR->SetLineWidth(2);


    // Legend for c1 (HIJING)
    c1->cd();
    TLegend *leg1 = new TLegend(0.72, 0.3, 0.95, 0.4); // move to lower right corner, y-range at 0.3-0.4
    leg1->SetTextSize(0.022); // slightly smaller text
    leg1->SetMargin(0.15); // reduce left margin for tighter spacing
    leg1->SetEntrySeparation(0.18); // reduce vertical spacing between entries
    leg1->AddEntry(h_HijingAND, "HFE_{max}^{+} > X AND HFE_{max}^{-} > X", "l");
    leg1->AddEntry(h_HijingOR, "HFE_{max}^{+} > X OR HFE_{max}^{-} > X", "l");
    leg1->Draw();
    c1->SetGrid();

    // Legend for c2 (Starlight SD)
    c2->cd();
    TLegend *leg2 = new TLegend(0.72, 0.3, 0.95, 0.4);
    leg2->SetTextSize(0.022);
    leg2->SetMargin(0.15);
    leg2->SetEntrySeparation(0.18);
    leg2->AddEntry(h_SDAND, "HFE_{max}^{+} > X AND HFE_{max}^{-} > X", "l");
    leg2->AddEntry(h_SDOR, "HFE_{max}^{+} > X OR HFE_{max}^{-} > X", "l");
    leg2->Draw();
    c2->SetGrid();

    c_DD->cd();
    TLegend *leg_DD = new TLegend(0.72, 0.3, 0.95, 0.4);
    leg_DD->SetTextSize(0.022);
    leg_DD->SetMargin(0.15);
    leg_DD->SetEntrySeparation(0.18);
    leg_DD->AddEntry(h_DDAND, "HFE_{max}^{+} > X AND HFE_{max}^{-} > X", "l");
    leg_DD->AddEntry(h_DDOR, "HFE_{max}^{+} > X OR HFE_{max}^{-} > X", "l");
    leg_DD->Draw();
    c_DD->SetGrid();
    
    // Draw text on c1
    c1->cd();
    TLatex latex1;
    latex1.SetNDC();
    latex1.SetTextSize(0.025);
    latex1.DrawLatex(0.23, 0.33, Form("leadingPtEta1p0_{sel} > %.1f GeV/c", leadingPtCut));
    latex1.DrawLatex(0.23, 0.30, "PVFilter == 1");
    latex1.DrawLatex(0.23, 0.27, "ClusterCompatibilityFilter == 1");
    latex1.DrawLatex(0.23, 0.24, "nVtx > 0");
    latex1.DrawLatex(0.23, 0.21, "abs(V_{z}) < 15");

    // Draw text on c2
    c2->cd();
    TLatex latex2;
    latex2.SetNDC();
    latex2.SetTextSize(0.025);
    latex2.DrawLatex(0.23, 0.33, Form("leadingPtEta1p0_{sel} > %.1f GeV/c", leadingPtCut));
    latex2.DrawLatex(0.23, 0.30, "PVFilter == 1");
    latex2.DrawLatex(0.23, 0.27, "ClusterCompatibilityFilter == 1");
    latex2.DrawLatex(0.23, 0.24, "nVtx > 0");
    latex2.DrawLatex(0.23, 0.21, "abs(V_{z}) < 15");

    c_DD->cd();
    TLatex latex_DD;
    latex_DD.SetNDC();
    latex_DD.SetTextSize(0.025);
    latex_DD.DrawLatex(0.23, 0.33, Form("leadingPtEta1p0_{sel} > %.1f GeV/c", leadingPtCut));
    latex_DD.DrawLatex(0.23, 0.30, "PVFilter == 1");
    latex_DD.DrawLatex(0.23, 0.27, "ClusterCompatibilityFilter == 1");
    latex_DD.DrawLatex(0.23, 0.24, "nVtx > 0");
    latex_DD.DrawLatex(0.23, 0.21, "abs(V_{z}) < 15");

    c1->SaveAs(Form("Plots/Filter Efficiency Results/countingHFE_HIJING_AfterCuts_Hist_leadingptsel%.0f.png", leadingPtCut));
    c2->SaveAs(Form("Plots/Filter Efficiency Results/countingHFE_SD_AfterCuts_Hist_leadingptsel%.0f.png", leadingPtCut));
    c_DD->SaveAs(Form("Plots/Filter Efficiency Results/countingHFE_DD_AfterCuts_Hist_leadingptsel%.0f.png", leadingPtCut));
}