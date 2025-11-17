#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <THStack.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TLatex.h>
#include <map>
using namespace std;

struct Parameters {
    float MaxJetPt;
    float MinJetPt;
    float IsData;
    float IsPP;
    float ChargeSelection;
    string DCAString;
    string descriptor;  // Use string instead of const char* to avoid pointer issues
};

const char* parseDCAcut(string dcaString) {
    string readableDCAstring = dcaString;
    
    size_t pos = 0;
    // Replace " && " with ", " for better readability in plots
    pos = 0;
    while ((pos = readableDCAstring.find(" && ", pos)) != string::npos) {
        readableDCAstring.replace(pos, 4, ", ");
        pos += 2; // Move past the replacement
    }
    
    // Replace abs() notation with physics notation using absolute value bars
    pos = 0;
    while ((pos = readableDCAstring.find("abs(muDiDxy1)", pos)) != string::npos) {
        readableDCAstring.replace(pos, 13, "#left|D_{xy}^{#mu1}#right|");
        pos += 13;
    }
    
    pos = 0;
    while ((pos = readableDCAstring.find("abs(muDiDxy2)", pos)) != string::npos) {
        readableDCAstring.replace(pos, 13, "#left|D_{xy}^{#mu2}#right|");
        pos += 13;
    }

    pos = 0;
    while ((pos = readableDCAstring.find("abs(muDiDxy1 / muDiDxy1Err) > ", pos)) != string::npos) {
        readableDCAstring.replace(pos, 29, "#left|D_{xy}^{#mu1} / #sigma_{D_{xy}^{#mu1}}#right| > ");
        pos += 29;
    }

    pos = 0;
    while ((pos = readableDCAstring.find("abs(muDiDxy2 / muDiDxy2Err) > ", pos)) != string::npos) {
        readableDCAstring.replace(pos, 29, "#left|D_{xy}^{#mu2} / #sigma_{D_{xy}^{#mu2}}#right| > ");
        pos += 29;
    }
    
    // Return as const char* (note: this creates a temporary, so use immediately)
    static string result = readableDCAstring;
    return result.c_str();
}

Parameters drawcuts(TFile* infile, TCanvas* c){

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
    latex->DrawLatex(0.11, 0.91, "CMS");
    latex->SetTextFont(52);  // Italic font for Preliminary
    latex->DrawLatex(0.18, 0.91, "Preliminary");
    latex->SetTextFont(42);  // Reset to normal font

    if(IsPP == 1)
        latex->DrawLatex(0.65, 0.91, "2017 pp 5.02 TeV");
    else
        latex->DrawLatex(0.65, 0.91, "2018 PbPb 5.02 TeV");

    latex->DrawLatex(0.45, 0.85, Form("#mu#mu-tagged jets: %.0f < p_{T}^{jet} < %.0f GeV", MinJetPt, MaxJetPt));
    latex->DrawLatex(0.45, 0.79, "#mu p_{T} > 3.5 GeV");
    latex->DrawLatex(0.45, 0.73, "#DeltaR(#mu, jet) < 0.3");
    latex->DrawLatex(0.45, 0.67, parseDCAcut(DCAString));
    
    if(ChargeSelection == -1){
        latex->DrawLatex(0.45, 0.61, "#mu^{+}#mu^{-} pairs only");
    } else if (ChargeSelection == 1){
        latex->DrawLatex(0.45, 0.61, "Same Sign Dimuons");
    } 

    return par;
}

void DrawYields(TFile* infile, TCanvas* c){
    
    if(infile->GetListOfKeys()->Contains("hInvMass_bb") == false){
        cout << "Flavor-specific histograms not found!" << endl;
        return;
    }

    TH1D* h_dimuon_mass_bb = (TH1D*)infile->Get("hInvMass_bb");
    TH1D* h_dimuon_mass_b = (TH1D*)infile->Get("hInvMass_b");
    TH1D* h_dimuon_mass_c = (TH1D*)infile->Get("hInvMass_c");
    TH1D* h_dimuon_mass_cc = (TH1D*)infile->Get("hInvMass_cc");
    TH1D* h_dimuon_mass_uds = (TH1D*)infile->Get("hInvMass_uds");

    int yield_bb = h_dimuon_mass_bb->Integral();
    int yield_b = h_dimuon_mass_b->Integral();
    int yield_c = h_dimuon_mass_c->Integral();
    int yield_cc = h_dimuon_mass_cc->Integral();
    int yield_uds = h_dimuon_mass_uds->Integral();

    c->cd();

    TLatex* latex = new TLatex();
    latex->SetNDC();  // Use normalized coordinates (0-1)
    latex->SetTextSize(0.04);
    latex->SetTextFont(42);

    latex->DrawLatex(0.70, 0.45, "Flavor Yields");
    latex->DrawLatex(0.70, 0.40, Form("bb: %d", yield_bb));
    latex->DrawLatex(0.70, 0.35, Form("b: %d", yield_b));
    latex->DrawLatex(0.70, 0.30, Form("cc: %d", yield_cc));
    latex->DrawLatex(0.70, 0.25, Form("c: %d", yield_c));
    latex->DrawLatex(0.70, 0.20, Form("uds: %d", yield_uds));
}

void plotInvMass(TFile* infile,const char* outputdir=""){

    TCanvas* c1 = new TCanvas("c1", "Dimuon Mass", 800, 600);
    //c1->SetMargin(0.15, 0.05, 0.15, 0.1);  // left, right, bottom, top

    TH1D* h_dimuon_mass = (TH1D*)infile->Get("hInvMass");
    
    if (!h_dimuon_mass) {
        cout << "Histogram hInvMass not found!" << endl;
        return;
    }
    
    h_dimuon_mass->SetLineColor(kBlack);
    h_dimuon_mass->SetLineWidth(2);
    h_dimuon_mass->SetMarkerStyle(20);
    h_dimuon_mass->SetMarkerSize(0.8);
    h_dimuon_mass->GetXaxis()->SetTitle("m_{#mu#mu} [GeV]");
    h_dimuon_mass->GetYaxis()->SetTitle("Dimuon Jets");
    h_dimuon_mass->SetMaximum(h_dimuon_mass->GetMaximum()*1.6);
    h_dimuon_mass->SetMinimum(0);
    h_dimuon_mass->Draw("PE");
    
    Parameters parm = drawcuts(infile, c1);
    cout << parm.descriptor << endl;

    TLegend* legend = new TLegend(0.11, 0.75, 0.45, 0.88);
    legend->SetBorderSize(0);
    legend->SetFillColor(kWhite);
    legend->AddEntry(h_dimuon_mass, parm.descriptor.c_str(), "lep");
    legend->Draw();
    
    c1->SaveAs(Form("%s/dimuon_mass_%f_%f.pdf", outputdir, parm.MinJetPt, parm.MaxJetPt));
}

void plotInvMass_flavors(TFile* infile,const char* outputdir=""){

    TCanvas* c1 = new TCanvas("c1", "Dimuon Mass by Flavor", 800, 600);
    //c1->SetMargin(0.15, 0.05, 0.15, 0.1);  // left, right, bottom, top
    if(infile->GetListOfKeys()->Contains("hInvMass_bb") == false){
        cout << "Flavor-specific histograms not found!" << endl;
        return;
    }

    TH1D* h_dimuon_mass_bb = (TH1D*)infile->Get("hInvMass_bb");
    TH1D* h_dimuon_mass_b = (TH1D*)infile->Get("hInvMass_b");
    TH1D* h_dimuon_mass_c = (TH1D*)infile->Get("hInvMass_c");
    TH1D* h_dimuon_mass_cc = (TH1D*)infile->Get("hInvMass_cc");
    TH1D* h_dimuon_mass_uds = (TH1D*)infile->Get("hInvMass_uds");

    // Set colors and styles for each histogram with transparency
    h_dimuon_mass_bb->SetFillColorAlpha(kRed, 0.2);
    h_dimuon_mass_bb->SetLineColor(kRed);
    h_dimuon_mass_b->SetFillColorAlpha(kBlue, 0.2);
    h_dimuon_mass_b->SetLineColor(kBlue);
    h_dimuon_mass_c->SetFillColorAlpha(kGreen+2, 0.2);
    h_dimuon_mass_c->SetLineColor(kGreen+2);
    h_dimuon_mass_cc->SetFillColorAlpha(kOrange+2, 0.2);
    h_dimuon_mass_cc->SetLineColor(kOrange+2);
    h_dimuon_mass_uds->SetFillColorAlpha(kTeal, 0.2);
    h_dimuon_mass_uds->SetLineColor(kTeal);

    h_dimuon_mass_bb->SetMarkerStyle(20);
    h_dimuon_mass_b->SetMarkerStyle(20);
    h_dimuon_mass_c->SetMarkerStyle(20);
    h_dimuon_mass_cc->SetMarkerStyle(20);
    h_dimuon_mass_uds->SetMarkerStyle(20);

    h_dimuon_mass_bb->SetMarkerColor(kRed);
    h_dimuon_mass_b->SetMarkerColor(kBlue);
    h_dimuon_mass_c->SetMarkerColor(kGreen);
    h_dimuon_mass_cc->SetMarkerColor(kOrange);
    h_dimuon_mass_uds->SetMarkerColor(kTeal);

    float m = max({
        h_dimuon_mass_bb->GetMaximum(),
        h_dimuon_mass_b->GetMaximum(),
        h_dimuon_mass_c->GetMaximum(),
        h_dimuon_mass_cc->GetMaximum(),
        h_dimuon_mass_uds->GetMaximum()
    });

    h_dimuon_mass_bb->SetMaximum(m * 1.6);
    h_dimuon_mass_b->SetMaximum(m * 1.6);
    h_dimuon_mass_c->SetMaximum(m * 1.6);
    h_dimuon_mass_cc->SetMaximum(m * 1.6);
    h_dimuon_mass_uds->SetMaximum(m * 1.6);

    h_dimuon_mass_bb->SetMinimum(0);
    h_dimuon_mass_b->SetMinimum(0);
    h_dimuon_mass_c->SetMinimum(0);
    h_dimuon_mass_cc->SetMinimum(0);
    h_dimuon_mass_uds->SetMinimum(0);

    // Draw histograms with fills - order matters for visibility
    h_dimuon_mass_c->GetXaxis()->SetTitle("m_{#mu#mu} [GeV]");
    h_dimuon_mass_c->GetYaxis()->SetTitle("Dimuon Jets");
    h_dimuon_mass_c->Draw("HIST");
    h_dimuon_mass_bb->Draw("HIST same");
    h_dimuon_mass_b->Draw("HIST same");
    h_dimuon_mass_cc->Draw("HIST same");
    h_dimuon_mass_uds->Draw("HIST same");
    
    // Overlay with error bars (no fill, just markers)
    h_dimuon_mass_c->Draw("E same");
    h_dimuon_mass_bb->Draw("E same");
    h_dimuon_mass_b->Draw("E same");
    h_dimuon_mass_cc->Draw("E same");
    h_dimuon_mass_uds->Draw("E same");

    Parameters parm = drawcuts(infile, c1);
    DrawYields(infile, c1);

    TLegend* legend = new TLegend(0.11, 0.67, 0.40, 0.88);
    legend->SetBorderSize(0);
    legend->SetFillColor(kWhite);
    legend->AddEntry(h_dimuon_mass_bb, "b#bar{b} (N_{b} = 2)", "f");
    legend->AddEntry(h_dimuon_mass_b, "b (N_{b} = 1)", "f");
    legend->AddEntry(h_dimuon_mass_cc, "c#bar{c} (N_{c} = 2, N_{b} = 0)", "f");
    legend->AddEntry(h_dimuon_mass_c, "c (N_{c} = 1, N_{b} = 0)", "f");
    legend->AddEntry(h_dimuon_mass_uds, "uds (N_{c} = 0, N_{b} = 0)", "f");
    legend->Draw();

    c1->SaveAs(Form("%s/dimuon_mass_flavors_%f_%f.pdf", outputdir, parm.MinJetPt, parm.MaxJetPt));

}

void plotmumupt(TFile* infile, const char* outputdir=""){

    TCanvas* c1 = new TCanvas("c1", "Dimuon PT", 800, 600);
    //c1->SetMargin(0.15, 0.05, 0.15, 0.1);  // left, right, bottom, top

    TH1D* h_dimuon_pt = (TH1D*)infile->Get("hmumuPt");

    if(!h_dimuon_pt){
        cout << "Histogram hmumuPt not found!" << endl;
        return;
    }

    h_dimuon_pt->SetLineColor(kBlack);
    h_dimuon_pt->SetLineWidth(2);
    h_dimuon_pt->SetMarkerStyle(20);
    h_dimuon_pt->SetMarkerSize(0.8);
    h_dimuon_pt->GetXaxis()->SetTitle("p_{T}^{#mu#mu} [GeV]");
    h_dimuon_pt->GetYaxis()->SetTitle("Dimuon Jets");
    h_dimuon_pt->SetMaximum(h_dimuon_pt->GetMaximum()*1.4);
    h_dimuon_pt->SetMinimum(0);
    h_dimuon_pt->Draw("PE");    

    Parameters parm = drawcuts(infile, c1);
    cout << parm.descriptor << endl;

    TLegend* legend = new TLegend(0.11, 0.75, 0.45, 0.88);
    legend->SetBorderSize(0);
    legend->SetFillColor(kWhite);
    legend->AddEntry(h_dimuon_pt, parm.descriptor.c_str(), "lep");
    legend->Draw();

    c1->SaveAs(Form("%s/dimuon_pt_%f_%f.pdf", outputdir, parm.MinJetPt, parm.MaxJetPt));

    

}

void plotmumupt_flavors(TFile* infile,const char* outputdir=""){

    TCanvas* c1 = new TCanvas("c1", "Dimuon PT by Flavor", 800, 600);
    //c1->SetMargin(0.15, 0.05, 0.15, 0.1);  // left, right, bottom, top
    if(infile->GetListOfKeys()->Contains("hmumuPt_bb") == false){
        cout << "Flavor-specific histograms not found!" << endl;
        return;
    }

    TH1D* h_dimuon_pt_bb = (TH1D*)infile->Get("hmumuPt_bb");
    TH1D* h_dimuon_pt_b = (TH1D*)infile->Get("hmumuPt_b");
    TH1D* h_dimuon_pt_c = (TH1D*)infile->Get("hmumuPt_c");
    TH1D* h_dimuon_pt_cc = (TH1D*)infile->Get("hmumuPt_cc");
    TH1D* h_dimuon_pt_uds = (TH1D*)infile->Get("hmumuPt_uds");
    // Set colors and styles for each histogram with transparency
    h_dimuon_pt_bb->SetFillColorAlpha(kRed, 0.2);
    h_dimuon_pt_bb->SetLineColor(kRed);
    h_dimuon_pt_b->SetFillColorAlpha(kBlue, 0.2);
    h_dimuon_pt_b->SetLineColor(kBlue);
    h_dimuon_pt_c->SetFillColorAlpha(kGreen+2, 0.2);
    h_dimuon_pt_c->SetLineColor(kGreen+2);
    h_dimuon_pt_cc->SetFillColorAlpha(kOrange+2, 0.2);
    h_dimuon_pt_cc->SetLineColor(kOrange+2);
    h_dimuon_pt_uds->SetFillColorAlpha(kTeal, 0.2);
    h_dimuon_pt_uds->SetLineColor(kTeal);
    h_dimuon_pt_bb->SetMarkerStyle(20);
    h_dimuon_pt_b->SetMarkerStyle(20);
    h_dimuon_pt_c->SetMarkerStyle(20);
    h_dimuon_pt_cc->SetMarkerStyle(20);
    h_dimuon_pt_uds->SetMarkerStyle(20);
    h_dimuon_pt_bb->SetMarkerColor(kRed);
    h_dimuon_pt_b->SetMarkerColor(kBlue);
    h_dimuon_pt_c->SetMarkerColor(kGreen);
    h_dimuon_pt_cc->SetMarkerColor(kOrange);
    h_dimuon_pt_uds->SetMarkerColor(kTeal);
    float m = max({
        h_dimuon_pt_bb->GetMaximum(),
        h_dimuon_pt_b->GetMaximum(),
        h_dimuon_pt_c->GetMaximum(),
        h_dimuon_pt_cc->GetMaximum(),
        h_dimuon_pt_uds->GetMaximum()
    });
    h_dimuon_pt_bb->SetMaximum(m * 1.4);
    h_dimuon_pt_b->SetMaximum(m * 1.4);
    h_dimuon_pt_c->SetMaximum(m * 1.4);
    h_dimuon_pt_cc->SetMaximum(m * 1.4);
    h_dimuon_pt_uds->SetMaximum(m * 1.4);
    h_dimuon_pt_bb->SetMinimum(0);
    h_dimuon_pt_b->SetMinimum(0);
    h_dimuon_pt_c->SetMinimum(0);
    h_dimuon_pt_cc->SetMinimum(0);
    h_dimuon_pt_uds->SetMinimum(0);
    // Draw histograms with fills - order matters for visibility
    h_dimuon_pt_c->GetXaxis()->SetTitle("p_{T}^{#mu#mu} [GeV]");
    h_dimuon_pt_c->GetYaxis()->SetTitle("Dimuon Jets");
    h_dimuon_pt_c->Draw("HIST");
    h_dimuon_pt_bb->Draw("HIST same");
    h_dimuon_pt_b->Draw("HIST same");
    h_dimuon_pt_cc->Draw("HIST same");
    h_dimuon_pt_uds->Draw("HIST same");
    // Overlay with error bars (no fill, just markers)
    h_dimuon_pt_c->Draw("E same");
    h_dimuon_pt_bb->Draw("E same");
    h_dimuon_pt_b->Draw("E same");
    h_dimuon_pt_cc->Draw("E same");
    h_dimuon_pt_uds->Draw("E same");
    Parameters parm = drawcuts(infile, c1);
    DrawYields(infile, c1);
    TLegend* legend = new TLegend(0.11, 0.67, 0.40, 0.88);
    legend->SetBorderSize(0);
    legend->SetFillColor(kWhite);
    legend->AddEntry(h_dimuon_pt_bb, "b#bar{b} (N_{b} = 2)", "f");
    legend->AddEntry(h_dimuon_pt_b, "b (N_{b} = 1)", "f");
    legend->AddEntry(h_dimuon_pt_cc, "c#bar{c} (N_{c} = 2, N_{b} = 0)", "f");
    legend->AddEntry(h_dimuon_pt_c, "c (N_{c} = 1, N_{b} = 0)", "f");
    legend->AddEntry(h_dimuon_pt_uds, "uds (N_{c} = 0, N_{b} = 0)", "f");
    legend->Draw(); 

    c1->SaveAs(Form("%s/dimuon_pt_flavors_%f_%f.pdf", outputdir, parm.MinJetPt, parm.MaxJetPt));

}

void plotDCA(TFile* infile, const char* outputdir=""){

    TCanvas* c1 = new TCanvas("c1", "DCA", 800, 600);
    //c1->SetMargin(0.15, 0.05, 0.15, 0.1);  // left, right, bottom, top    
    TH1D* h_dca = (TH1D*)infile->Get("hmuDiDxy1Dxy2Sig");
    if(!h_dca){
        cout << "Histogram hmuDiDxy1Dxy2 not found!" << endl;
        return;
    }
    h_dca->SetLineColor(kBlack);
    h_dca->SetLineWidth(2);
    h_dca->SetMarkerStyle(20);
    h_dca->SetMarkerSize(0.8);
    h_dca->GetXaxis()->SetTitle("log10(|D_{xy}^{#mu1}| x |D_{xy}^{#mu2}|)");
    h_dca->GetYaxis()->SetTitle("Dimuon Jets");
    h_dca->SetMaximum(h_dca->GetMaximum()*1.7);
    h_dca->SetMinimum(0);
    h_dca->Draw("PE");  
    Parameters parm = drawcuts(infile, c1);
    cout << parm.descriptor << endl;
    TLegend* legend = new TLegend(0.11, 0.75, 0.45, 0.88);
    legend->SetBorderSize(0);
    legend->SetFillColor(kWhite);
    legend->AddEntry(h_dca, parm.descriptor.c_str(), "lep");
    legend->Draw();
    c1->SaveAs(Form("%s/dimuon_DCA_%f_%f.pdf", outputdir, parm.MinJetPt, parm.MaxJetPt));

}

void plotDCA_flavors(TFile* infile, const char* outputdir=""){

    TCanvas* c1 = new TCanvas("c1", "DCA by Flavor", 800, 600);
    //c1->SetMargin(0.15, 0.05, 0.15, 0.1);  // left, right, bottom, top
    if(infile->GetListOfKeys()->Contains("hmuDiDxy1Dxy2Sig_bb") == false){
        cout << "Flavor-specific histograms not found!" << endl;
        return;
    }

    TH1D* h_dca_bb = (TH1D*)infile->Get("hmuDiDxy1Dxy2Sig_bb");
    TH1D* h_dca_b = (TH1D*)infile->Get("hmuDiDxy1Dxy2Sig_b");
    TH1D* h_dca_c = (TH1D*)infile->Get("hmuDiDxy1Dxy2Sig_c");
    TH1D* h_dca_cc = (TH1D*)infile->Get("hmuDiDxy1Dxy2Sig_cc");
    TH1D* h_dca_uds = (TH1D*)infile->Get("hmuDiDxy1Dxy2Sig_uds");
    // Set colors and styles for each histogram with transparency
    h_dca_bb->SetFillColorAlpha(kRed, 0.2);
    h_dca_bb->SetLineColor(kRed);
    h_dca_b->SetFillColorAlpha(kBlue, 0.2);
    h_dca_b->SetLineColor(kBlue);
    h_dca_c->SetFillColorAlpha(kGreen+2, 0.2);
    h_dca_c->SetLineColor(kGreen+2);
    h_dca_cc->SetFillColorAlpha(kOrange+2, 0.2);
    h_dca_cc->SetLineColor(kOrange+2);
    h_dca_uds->SetFillColorAlpha(kTeal, 0.2);
    h_dca_uds->SetLineColor(kTeal);

    h_dca_bb->SetMarkerStyle(20);
    h_dca_b->SetMarkerStyle(20);
    h_dca_c->SetMarkerStyle(20);
    h_dca_cc->SetMarkerStyle(20);
    h_dca_uds->SetMarkerStyle(20);

    h_dca_bb->SetMarkerColor(kRed);
    h_dca_b->SetMarkerColor(kBlue);
    h_dca_c->SetMarkerColor(kGreen);
    h_dca_cc->SetMarkerColor(kOrange);
    h_dca_uds->SetMarkerColor(kTeal);

    float m = max({
        h_dca_bb->GetMaximum(),
        h_dca_b->GetMaximum(),
        h_dca_c->GetMaximum(),
        h_dca_cc->GetMaximum(),
        h_dca_uds->GetMaximum()
    });
    h_dca_bb->SetMaximum(m * 1.7);
    h_dca_b->SetMaximum(m * 1.7);
    h_dca_c->SetMaximum(m * 1.7);
    h_dca_cc->SetMaximum(m * 1.7);
    h_dca_uds->SetMaximum(m * 1.7);

    h_dca_bb->SetMinimum(0);
    h_dca_b->SetMinimum(0);
    h_dca_c->SetMinimum(0);
    h_dca_cc->SetMinimum(0);
    h_dca_uds->SetMinimum(0);

    // Draw histograms with fills - order matters for visibility
    h_dca_c->GetXaxis()->SetTitle("log10(#left|D_{xy}^{#mu1} / #sigma_{D_{xy}^{#mu1}}#right| x #left|D_{xy}^{#mu2} / #sigma_{D_{xy}^{#mu2}}#right|)");
    h_dca_c->GetYaxis()->SetTitle("Dimuon Jets");
    h_dca_c->Draw("HIST");
    h_dca_bb->Draw("HIST same");
    h_dca_b->Draw("HIST same");
    h_dca_cc->Draw("HIST same");
    h_dca_uds->Draw("HIST same");  

    // Overlay with error bars (no fill, just markers)
    h_dca_c->Draw("E same");
    h_dca_bb->Draw("E same");
    h_dca_b->Draw("E same");
    h_dca_cc->Draw("E same");
    h_dca_uds->Draw("E same");

    Parameters parm = drawcuts(infile, c1);
    DrawYields(infile, c1);
    TLegend* legend = new TLegend(0.11, 0.67, 0.40, 0.88);
    legend->SetBorderSize(0);
    legend->SetFillColor(kWhite);
    legend->AddEntry(h_dca_bb, "b#bar{b} (N_{b} = 2)", "f");
    legend->AddEntry(h_dca_b, "b (N_{b} = 1)", "f");
    legend->AddEntry(h_dca_cc, "c#bar{c} (N_{c} = 2, N_{b} = 0)", "f");
    legend->AddEntry(h_dca_c, "c (N_{c} = 1, N_{b} = 0)", "f");
    legend->AddEntry(h_dca_uds, "uds (N_{c} = 0, N_{b} = 0)", "f");
    legend->Draw();

    c1->SaveAs(Form("%s/dimuon_DCA_flavors_%f_%f.pdf", outputdir, parm.MinJetPt, parm.MaxJetPt));

}


void plotdR(TFile* infile, const char* outputdir=""){

    TCanvas* c1 = new TCanvas("c1", "Delta R", 800, 600);
    //c1->SetMargin(0.15, 0.05, 0.15, 0.1);  // left, right, bottom, top    
    TH1D* h_dR = (TH1D*)infile->Get("hmuDR");
    if(!h_dR){
        cout << "Histogram hmuDR not found!" << endl;
        return;
    }
    h_dR->SetLineColor(kBlack);
    h_dR->SetLineWidth(2);
    h_dR->SetMarkerStyle(20);
    h_dR->SetMarkerSize(0.8);
    h_dR->GetXaxis()->SetTitle("#DeltaR(#mu, jet)");
    h_dR->GetYaxis()->SetTitle("Dimuon Jets");
    h_dR->SetMaximum(h_dR->GetMaximum()*1.7);
    h_dR->SetMinimum(0);
    h_dR->Draw("PE");  
    Parameters parm = drawcuts(infile, c1);
    cout << parm.descriptor << endl;
    TLegend* legend = new TLegend(0.11, 0.75, 0.45, 0.88);
    legend->SetBorderSize(0);
    legend->SetFillColor(kWhite);
    legend->AddEntry(h_dR, parm.descriptor.c_str(), "lep");
    legend->Draw();
    c1->SaveAs(Form("%s/dimuon_DeltaR_%.0f_%.0f.pdf", outputdir, parm.MinJetPt, parm.MaxJetPt));
}

void plotDR_flavors(TFile* infile, const char* outputdir=""){

    TCanvas* c1 = new TCanvas("c1", "Delta R by Flavor", 800, 600);
    //c1->SetMargin(0.15, 0.05, 0.15, 0.1);  // left, right, bottom, top
    if(infile->GetListOfKeys()->Contains("hmuDR_bb") == false){
        cout << "Flavor-specific histograms not found!" << endl;
        return;
    }

    TH1D* h_dR_bb = (TH1D*)infile->Get("hmuDR_bb");
    TH1D* h_dR_b = (TH1D*)infile->Get("hmuDR_b");
    TH1D* h_dR_c = (TH1D*)infile->Get("hmuDR_c");
    TH1D* h_dR_cc = (TH1D*)infile->Get("hmuDR_cc");
    TH1D* h_dR_uds = (TH1D*)infile->Get("hmuDR_uds");
    // Set colors and styles for each histogram with transparency
    h_dR_bb->SetFillColorAlpha(kRed, 0.2);
    h_dR_bb->SetLineColor(kRed);
    h_dR_b->SetFillColorAlpha(kBlue, 0.2);
    h_dR_b->SetLineColor(kBlue);
    h_dR_c->SetFillColorAlpha(kGreen+2, 0.2);
    h_dR_c->SetLineColor(kGreen+2);
    h_dR_cc->SetFillColorAlpha(kOrange+2, 0.2);
    h_dR_cc->SetLineColor(kOrange+2);
    h_dR_uds->SetFillColorAlpha(kTeal, 0.2);
    h_dR_uds->SetLineColor(kTeal);

    h_dR_bb->SetMarkerStyle(20);
    h_dR_b->SetMarkerStyle(20);
    h_dR_c->SetMarkerStyle(20);
    h_dR_cc->SetMarkerStyle(20);
    h_dR_uds->SetMarkerStyle(20);

    h_dR_bb->SetMarkerColor(kRed);
    h_dR_b->SetMarkerColor(kBlue);
    h_dR_c->SetMarkerColor(kGreen);
    h_dR_cc->SetMarkerColor(kOrange);
    h_dR_uds->SetMarkerColor(kTeal);

    float m = max({
        h_dR_bb->GetMaximum(),
        h_dR_b->GetMaximum(),
        h_dR_c->GetMaximum(),
        h_dR_cc->GetMaximum(),
        h_dR_uds->GetMaximum()
    });
    h_dR_bb->SetMaximum(m * 1.7);
    h_dR_b->SetMaximum(m * 1.7);
    h_dR_c->SetMaximum(m * 1.7);
    h_dR_cc->SetMaximum(m * 1.7);
    h_dR_uds->SetMaximum(m * 1.7);  

    h_dR_bb->SetMinimum(0);
    h_dR_b->SetMinimum(0);
    h_dR_c->SetMinimum(0);
    h_dR_cc->SetMinimum(0);
    h_dR_uds->SetMinimum(0);

    // Draw histograms with fills - order matters for visibility
    h_dR_c->GetXaxis()->SetTitle("#DeltaR(#mu, jet)");
    h_dR_c->GetYaxis()->SetTitle("Dimuon Jets");
    h_dR_c->Draw("HIST");
    h_dR_bb->Draw("HIST same");
    h_dR_b->Draw("HIST same");
    h_dR_cc->Draw("HIST same");
    h_dR_uds->Draw("HIST same");

    // Overlay with error bars (no fill, just markers)
    h_dR_c->Draw("E same");
    h_dR_bb->Draw("E same");
    h_dR_b->Draw("E same");
    h_dR_cc->Draw("E same");
    h_dR_uds->Draw("E same");

    Parameters parm = drawcuts(infile, c1);
    DrawYields(infile, c1);
    TLegend* legend = new TLegend(0.11, 0.67, 0.40, 0.88);
    legend->SetBorderSize(0);
    legend->SetFillColor(kWhite);
    legend->AddEntry(h_dR_bb, "b#bar{b} (N_{b} = 2)", "f");
    legend->AddEntry(h_dR_b, "b (N_{b} = 1)", "f");
    legend->AddEntry(h_dR_cc, "c#bar{c} (N_{c} = 2, N_{b} = 0)", "f");
    legend->AddEntry(h_dR_c, "c (N_{c} = 1, N_{b} = 0)", "f");
    legend->AddEntry(h_dR_uds, "uds (N_{c} = 0, N_{b} = 0)", "f");
    legend->Draw();

    c1->SaveAs(Form("%s/dimuon_DeltaR_flavors_%f_%f.pdf", outputdir, parm.MinJetPt, parm.MaxJetPt));

}

void plotter(const char* inputfile = "output_DoubleHQtagging_1.root",const char* outputdir = "") {

    gStyle->SetOptStat(0);

    TFile* infile = TFile::Open(inputfile);
    if(!infile || infile->IsZombie()){
        cout << "Error opening file: " << inputfile << endl;
        return;
    }
    
    // INVARIANT MASS
    plotInvMass(infile, outputdir);
    plotInvMass_flavors(infile, outputdir);
    
    // DIMUON PT
    plotmumupt(infile, outputdir);
    plotmumupt_flavors(infile, outputdir);
    // DCA
    plotDCA(infile, outputdir);
    plotDCA_flavors(infile, outputdir);

    // DR
    plotdR(infile, outputdir);
    plotDR_flavors(infile, outputdir);

    infile->Close();
}
