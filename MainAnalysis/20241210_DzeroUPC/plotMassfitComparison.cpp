#include <TROOT.h>
#include <vector>
#include <filesystem>

using namespace std;

void plotMassfitComparison(
) {
  vector<float> DptBins = {2, 5, 8};
  vector<float> DyBins  = {-2, -1, 0, 1, 2};
  vector<int>   gNBins  = {0, 1};
  
  for (int iDpt = 0; iDpt < DptBins.size()-1; iDpt++) {
    float DptMin = DptBins[iDpt];
    float DptMax = DptBins[iDpt+1];
    cout << "Processing Dpt bin: " << DptMin << "-" << DptMax << " GeV..." << endl;
    
    int   DmassNbins = 52;
    float DmassMin = 1.65;
    float DmassMax = 2.17;
    if (DptMin == 5) {
      DmassNbins = 36;
      DmassMin = 1.67;
      DmassMax = 2.06;
    }
    
    TString fHIN25002Str = "/home/data/public/yuchenc/RST_redo0326_MCReweighting_cf_PR101_nomp3_ANv4GL/fullAnalysis/dmassDataPoint_archived25002.root";
    TFile* fHIN25002;
    if (DptMin == 2) {
      cout << "Opening " << fHIN25002Str.Data() << "...";
      fHIN25002 = TFile::Open(fHIN25002Str, "READ");
      cout << " Opened!" << endl;
    }
    
    for (int iDy = 0; iDy < DyBins.size()-1; iDy++) {
      float DyMin = DyBins[iDy];
      float DyMax = DyBins[iDy+1];
      cout << "Processing Dy bin: " << DyMin << "-" << DyMax << endl;
      
      TString fHIN24003Str;
      if (DptMin == 2) {
        fHIN24003Str = "fitrootfiles_nominal/fit_hlt1_pt2-5_y-1.0-1.0.root";
      }
      else if (DptMin == 5) {
        fHIN24003Str = Form(
          "fitrootfiles_nominal/fit_hlt8_pt5-8_y%.1f-%.1f.root",
          DyMin, DyMax
        );
      }
      cout << "Opening " << fHIN24003Str.Data() << "...";
      TFile* fHIN24003 = TFile::Open(fHIN24003Str, "READ");
      cout << " Opened!" << endl;
      
      for (int igN = 0; igN < gNBins.size(); igN++) {
        int gammaN = gNBins[igN];
        TString gNStr = "0nxn";
        if (gammaN) gNStr = "xn0n";
        cout << "Processing gammaN bin: " << gNStr.Data() << endl;
        
        TString fReforestStr = Form(
          "fullAnalysis/pt%.0f-%.0f_y%.0f-%.0f_IsGammaN%d/Data.root",
          DptMin, DptMax, DyMin, DyMax, gammaN
        );
        cout << "Opening " << fReforestStr.Data() << "...";
        TFile* fReforest = TFile::Open(fReforestStr, "READ");
        cout << " Opened!" << endl;
        cout << "Accessing hist: hDmass" << endl;
        TString hDmassStr = "hDmass";
        if (DptMin == 2) hDmassStr = "hDmass_25002Bins";
        if (DptMin == 5) hDmassStr = "hDmass_24003Bins";
        TH1D* hDmassReforest = (TH1D*) (fReforest->Get(hDmassStr))->Clone("hDmassReforest");
        
        TString histStr;
        if (DptMin == 2) {
          histStr = Form(
            "dmass_%s_SystNominal_dpt_2_5_dy_m1p0_1p0_hlt1_floatwidth_exp",
            gNStr.Data()
          );
        }
        else if (DptMin == 5) {
          TString DyStr;
          if (DyMin < 0 && DyMax < 0) DyStr = Form("dy_m%.0fp0_m%.0fp0", fabs(DyMin), fabs(DyMax));
          else if (DyMin < 0) DyStr = Form("dy_m%.0fp0_%.0fp0", fabs(DyMin), DyMax);
          else DyStr = Form("dy_%.0fp0_%.0fp0", DyMin, DyMax);
          histStr = Form(
            "dmass_%s_SystNominal_dpt_5_8_%s_hlt8_floatwidth_exp",
            gNStr.Data(), DyStr.Data()
          );
        }
        cout << "Accessing hist: " << histStr.Data() << endl;
        TH1D* hDmassHIN24003 = (TH1D*) (fHIN24003->Get(histStr))->Clone("hDmassHIN24003");
        
        TH1D* hDmassHIN25002;
        TString hDmassHIN25002Str = Form(
          "pt%.0f-%.0f_y%.0f-%.0f_IsGammaN%d__Dmass",
          DptMin, DptMax, DyMin, DyMax, gammaN
        );
        if (DptMin == 2) hDmassHIN25002 = (TH1D*) (fHIN25002->Get(hDmassHIN25002Str))->Clone("hDmassHIN25002");
        
        TCanvas* canvas = new TCanvas("canvas", "", 600, 600);
        
        TPad* pDmass = new TPad("pDmass", "", 0.0, 0.3, 1.0, 1.0);
        pDmass->SetMargin(0.1, 0.1, 0.0, 0.1/0.7);
        pDmass->Draw();
        pDmass->cd();
        gStyle->SetOptStat(0);
        gStyle->SetTitleFontSize(0.035/0.7);
        gStyle->SetTitleSize(0.030/0.7, "xyx");
        gStyle->SetTitleOffset(1.0, "xyz");
        gStyle->SetLabelSize(0.025/0.7, "xyz");
        gStyle->SetLabelOffset(0.01/0.7, "xyz");
        
        TH1D* hDmassBase = new TH1D(
          "hDmassBase",
          Form("%.0f < Dp_{T} < %.0f, %.0f < Dy < %.0f, %s;; Events / (%.4f GeV)",
            DptMin, DptMax, DyMin, DyMax, gNStr.Data(), hDmassReforest->GetBinWidth(1)
          ),
          DmassNbins, DmassMin, DmassMax
        );
        hDmassBase->Draw();
        hDmassBase->SetMinimum(0.);
        
        int nDSigReg_25002 = 0;
        int nDSigReg_24003 = 0;
        int nDSigReg = 0;
        int nDFullReg_25002 = 0;
        int nDFullReg_24003 = 0;
        int nDFullReg = 0;
        float xSigMin = 0.;
        float xSigMax = 0.;
        
        if (DptMin == 2) {
          for (int iBin = 1; iBin <= hDmassHIN25002->GetNbinsX(); iBin++) {
            nDFullReg_25002 += hDmassHIN25002->GetBinContent(iBin);
            if (hDmassHIN25002->GetBinCenter(iBin) > 1.795 &&
                hDmassHIN25002->GetBinCenter(iBin) < 1.935) {
              nDSigReg_25002 += hDmassHIN25002->GetBinContent(iBin);
            }
          }
          if (DyMin == -1 || DyMax == 1) {
            hDmassHIN24003->SetMarkerColor(kGray);
            hDmassHIN24003->SetLineColor(kGray);
            hDmassHIN24003->SetMarkerStyle(24);
            hDmassHIN24003->Draw("same");
          }
          hDmassHIN25002->SetMarkerColor(kBlack);
          hDmassHIN25002->SetLineColor(kBlack);
          hDmassHIN25002->SetMarkerStyle(20);
          hDmassBase->SetMaximum(1.3 * TMath::Max(
              hDmassHIN25002->GetMaximum(), hDmassReforest->GetMaximum()
            )
          );
        }
        else if (DptMin == 5) {
          for (int iBin = 1; iBin <= hDmassHIN24003->GetNbinsX(); iBin++) {
            nDFullReg_24003 += hDmassHIN24003->GetBinContent(iBin);
            if (hDmassHIN24003->GetBinCenter(iBin) > 1.795 &&
                hDmassHIN24003->GetBinCenter(iBin) < 1.935) {
              nDSigReg_24003 += hDmassHIN24003->GetBinContent(iBin);
            }
          }
          hDmassBase->SetMaximum(1.3 * TMath::Max(
              hDmassHIN24003->GetMaximum(), hDmassReforest->GetMaximum()
            )
          );
          hDmassHIN24003->SetMarkerColor(kBlack);
          hDmassHIN24003->SetLineColor(kBlack);
          hDmassHIN24003->SetMarkerStyle(20);
        }
        for (int iBin = 1; iBin <= hDmassReforest->GetNbinsX(); iBin++) {
          nDFullReg += hDmassReforest->GetBinContent(iBin);
          if (hDmassReforest->GetBinCenter(iBin) > 1.795 &&
              hDmassReforest->GetBinCenter(iBin) < 1.935) {
            nDSigReg += hDmassReforest->GetBinContent(iBin);
            if (xSigMin == 0.)
              xSigMin = hDmassReforest->GetBinLowEdge(iBin);
            if (xSigMax < hDmassReforest->GetBinLowEdge(iBin+1))
              xSigMax = hDmassReforest->GetBinLowEdge(iBin+1);
          }
        }
        TBox* bSigRegion = new TBox(
          xSigMin, 0.0, xSigMax, hDmassBase->GetMaximum()
        );
        bSigRegion->SetFillColorAlpha(kAzure-4, 0.25);
        bSigRegion->SetLineWidth(0);
        bSigRegion->Draw();
        if (DptMin == 5 || DyMin == -1 || DyMax == 1) hDmassHIN24003->Draw("same");
        if (DptMin ==2) hDmassHIN25002->Draw("same");
        hDmassReforest->SetMarkerColor(kRed);
        hDmassReforest->SetLineColor(kRed);
        hDmassReforest->SetMarkerStyle(24);
        hDmassReforest->Draw("same");
        
        canvas->cd();
        TLegend* legend = new TLegend(0.6, 0.7, 0.9, 0.9);
        legend->AddEntry(hDmassReforest, "Reforest", "lpe");
        if (DptMin == 2) legend->AddEntry(hDmassHIN25002, "HIN-25-002", "lpe");
        if (DptMin == 5 || (DptMin == 2 && (DyMin == -1 || DyMax == 1))
          ) legend->AddEntry(hDmassHIN24003, "HIN-24-003", "lpe");
        legend->SetFillStyle(0);
        legend->SetBorderSize(0);
        legend->SetTextSize(0.035);
        legend->Draw();
        
        TLatex latex;
        latex.SetNDC();
        latex.SetTextSize(0.030);
        latex.SetTextFont(42);
        latex.SetTextColor(kAzure-5);
        latex.DrawLatex(0.15, 0.85, Form("N_{%.3f-%.3f}^{Reforest} = %d", xSigMin, xSigMax, nDSigReg));
        if (DptMin == 2) latex.DrawLatex(0.15, 0.79, Form("N_{%.3f-%.3f}^{HIN-25-002} = %d", xSigMin, xSigMax, nDSigReg_25002));
        if (DptMin == 5) latex.DrawLatex(0.15, 0.79, Form("N_{%.3f-%.3f}^{HIN-24-003} = %d", xSigMin, xSigMax, nDSigReg_24003));
        latex.SetTextColor(kBlack);
        latex.DrawLatex(0.15, 0.72, Form("N_{Full Region}^{Reforest} = %d", nDFullReg));
        if (DptMin == 2) latex.DrawLatex(0.15, 0.66, Form("N_{Full Region}^{HIN-25-002} = %d", nDFullReg_25002));
        if (DptMin == 5) latex.DrawLatex(0.15, 0.66, Form("N_{Full Region}^{HIN-24-003} = %d", nDFullReg_24003));
        
        TPad* pMassRatio = new TPad("pMassRatio", "", 0.0, 0.0, 1.0, 0.3);
        pMassRatio->SetMargin(0.1, 0.1, 0.1/0.3, 0.0);
        pMassRatio->Draw();
        pMassRatio->cd();
        gStyle->SetOptStat(0);
        gStyle->SetTitleFontSize(0.035/0.3);
        gStyle->SetTitleSize(0.030/0.3, "xyx");
        gStyle->SetTitleOffset(1.0, "xyz");
        gStyle->SetLabelSize(0.025/0.3, "xyz");
        gStyle->SetLabelOffset(0.01/0.3, "xyz");
        
        TH1D* hMassRatio = new TH1D(
          "hMassRatio",
          "; DMass (GeV); R_{new/old}",
          DmassNbins, DmassMin, DmassMax
        );
        hMassRatio->Draw();
        hMassRatio->SetMinimum(0.725);
        hMassRatio->SetMaximum(1.275);
        TBox* bSigRegionRatio = new TBox(
          xSigMin, hMassRatio->GetMinimum(), xSigMax, hMassRatio->GetMaximum()
        );
        bSigRegionRatio->SetFillColorAlpha(kAzure-4, 0.25);
        bSigRegionRatio->SetLineWidth(0);
        bSigRegionRatio->Draw();
        TLine* unity = new TLine(DmassMin, 1.0, DmassMax, 1.0);
        unity->SetLineColor(kGray+1);
        unity->SetLineStyle(2);
        unity->Draw();
        TH1D* hRatioReforest = (TH1D*) hDmassReforest->Clone("hRatioReforest");
        if (DptMin == 2) {
          hRatioReforest->Divide(hDmassHIN25002);
          hRatioReforest->SetMarkerStyle(0);
          hRatioReforest->Draw("same");
        }
        if (DptMin == 5) {
          hRatioReforest->Divide(hDmassHIN24003);
          hRatioReforest->SetMarkerStyle(0);
          hRatioReforest->Draw("same");
        }
        
        canvas->cd();
        canvas->Update();
        system("mkdir -p plot/massFit/HIN24003_HIN25002_comparisons");
        canvas->SaveAs(Form(
          "plot/massFit/HIN24003_HIN25002_comparisons/Dpt%.0f-%.0f_Dy%.0f-%.0f_gammaN%d.pdf",
          DptMin, DptMax, DyMin, DyMax, gammaN
        ));
        
        fReforest->Close();
//        canvas->Clear();
        delete canvas;
//        delete hDmassBase;
//        delete hDmassHIN24003;
//        delete hDmassReforest;
//        delete hRatioReforest;
      }
      fHIN24003->Close();
    }
    fHIN25002->Close();
  }
}

