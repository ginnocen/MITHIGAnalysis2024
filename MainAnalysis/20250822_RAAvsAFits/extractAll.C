#include <TGraphAsymmErrors.h>
#include <TMath.h>
#include <TError.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH2F.h>
#include <TF1.h>
#include <TPaveText.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TString.h>
#include <TColor.h>
#include <cmath>
#include <iostream>

TGraphAsymmErrors* SumErrorsInQuadrature(const TGraphAsymmErrors* gStat,
                                         const TGraphAsymmErrors* gSyst,
                                         const char* name = "gTot",
                                         double tol = 1e-9)
{
  if (!gStat || !gSyst) {
    Error("SumErrorsInQuadrature","Null input graph.");
    return nullptr;
  }
  const int n = gStat->GetN();
  if (gSyst->GetN() != n) {
    Error("SumErrorsInQuadrature","Point counts differ: stat=%d, syst=%d.", n, gSyst->GetN());
    return nullptr;
  }

  auto gOut = new TGraphAsymmErrors(n);
  if (name) gOut->SetName(name);
  gOut->SetTitle(gStat->GetTitle());

  for (int i = 0; i < n; ++i) {
    double xs, ys, x2, y2;
    gStat->GetPoint(i, xs, ys);
    gSyst->GetPoint(i, x2, y2);

    if (TMath::Abs(xs - x2) > tol || TMath::Abs(ys - y2) > tol) {
      Warning("SumErrorsInQuadrature",
              "Point %d central values differ (stat x=%.6g y=%.6g; syst x=%.6g y=%.6g). Using stat centrals.",
              i, xs, ys, x2, y2);
    }

    // Take X errors from stat; if different, keep the larger one
    double exl = gStat->GetErrorXlow(i),  exl2 = gSyst->GetErrorXlow(i);
    double exh = gStat->GetErrorXhigh(i), exh2 = gSyst->GetErrorXhigh(i);
    if (TMath::Abs(exl - exl2) > tol) exl = TMath::Max(exl, exl2);
    if (TMath::Abs(exh - exh2) > tol) exh = TMath::Max(exh, exh2);

    auto qsum = [](double a, double b) {
      a = TMath::Max(0.0, a);
      b = TMath::Max(0.0, b);
      return TMath::Sqrt(a*a + b*b);
    };

    const double eyl = qsum(gStat->GetErrorYlow(i),  gSyst->GetErrorYlow(i));
    const double eyh = qsum(gStat->GetErrorYhigh(i), gSyst->GetErrorYhigh(i));

    gOut->SetPoint(i, xs, ys);
    gOut->SetPointError(i, exl, exh, eyl, eyh);
  }

  // inherit a basic style from the stat graph
  gOut->SetMarkerStyle(gStat->GetMarkerStyle());
  gOut->SetMarkerColor(gStat->GetMarkerColor());
  gOut->SetLineColor(gStat->GetLineColor());
  gOut->SetLineWidth(gStat->GetLineWidth());

  return gOut;
}

TGraphAsymmErrors *TransformX(TGraphAsymmErrors *gIn, int option = 1) {
  if (!gIn) return nullptr;

  double exp = static_cast<double>(option) / 3.0; // exponent for x transformation
  int n = gIn->GetN();
  TGraphAsymmErrors *gOut = new TGraphAsymmErrors(n);

  for (int i = 0; i < n; i++) {
    double x, y;
    gIn->GetPoint(i, x, y);

    // errors
    double eyl = gIn->GetErrorYlow(i);
    double eyh = gIn->GetErrorYhigh(i);

    // map x -> x^(option/3)
    double newx = TMath::Power(x, exp);

    gOut->SetPoint(i, newx, y);
    gOut->SetPointError(i, 0.0, 0.0, eyl, eyh); // x-errors set to 0 by design
  }

  return gOut;
}

const int nPt = 5;
double pLow[nPt]  = {9.6, 14.4, 24.0, 35.2, 48.0};
double pHigh[nPt] = {12.0, 19.2, 28.8, 48.0, 73.6};

TGraphAsymmErrors *extract(int ptBin = 0, int option = 1,
                           TString fileName = "ptBinned_RAAVsA.root") {
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);

  // Open the ROOT file (READ so we don't accidentally try to write to it)
  TFile *file = TFile::Open(fileName, "READ");
  if (!file || file->IsZombie()) {
    std::cerr << "Error opening file: " << fileName << std::endl;
    if (file) { file->Close(); delete file; }
    return nullptr;
  }

  // Retrieve & transform inputs
  TGraphAsymmErrors *gRAA_PtVsA_TotSyst_src =
      (TGraphAsymmErrors*)file->Get(Form("gRAA_PtVsA_TotSyst_ptBin%d", ptBin));
  TGraphAsymmErrors *gRAA_PtVsA_Stat_src =
      (TGraphAsymmErrors*)file->Get(Form("gRAA_PtVsA_Stat_ptBin%d", ptBin));

  TGraphAsymmErrors *gRAA_PtVsA_TotSyst_ptBin = TransformX(gRAA_PtVsA_TotSyst_src, option);
  TGraphAsymmErrors *gRAA_PtVsA_Stat_ptBin    = TransformX(gRAA_PtVsA_Stat_src,    option);

  if (!gRAA_PtVsA_TotSyst_ptBin || !gRAA_PtVsA_Stat_ptBin) {
    std::cerr << "Error retrieving graphs from file." << std::endl;
    file->Close(); delete file;
    if (gRAA_PtVsA_TotSyst_ptBin) delete gRAA_PtVsA_TotSyst_ptBin;
    if (gRAA_PtVsA_Stat_ptBin)    delete gRAA_PtVsA_Stat_ptBin;
    return nullptr;
  }

  // Sum in quadrature
  auto *gRAA_PtVsA_Tot_ptBin = SumErrorsInQuadrature(
      gRAA_PtVsA_Stat_ptBin, gRAA_PtVsA_TotSyst_ptBin,
      Form("gRAA_PtVsA_Tot_ptBin%d", ptBin));

  // Clean up temporaries before we leave the function
  delete gRAA_PtVsA_TotSyst_ptBin;
  delete gRAA_PtVsA_Stat_ptBin;

  if (!gRAA_PtVsA_Tot_ptBin) {
    file->Close(); delete file;
    return nullptr;
  }

  // print data points
  for (int i = 0; i < gRAA_PtVsA_Tot_ptBin->GetN(); i++) {
    double x, y;
    gRAA_PtVsA_Tot_ptBin->GetPoint(i, x, y);
    double exl = gRAA_PtVsA_Tot_ptBin->GetErrorXlow(i);
    double exh = gRAA_PtVsA_Tot_ptBin->GetErrorXhigh(i);
    double eyl = gRAA_PtVsA_Tot_ptBin->GetErrorYlow(i);
    double eyh = gRAA_PtVsA_Tot_ptBin->GetErrorYhigh(i);
    std::cout << "Point " << i << ": x = " << x << " +" << exh << " -" << exl
              << ", y = " << y << " +" << eyh << " -" << eyl << std::endl;
  }

  double xrangemin = 0.0;
  double xrangemax = 0.0;
  if (option == 1)      { xrangemin = 1.0; xrangemax = 6.0;   }
  else if (option == 2) { xrangemin = 1.0; xrangemax = 40.0;  }
  else if (option == 3) { xrangemin = 1.0; xrangemax = 210.0; }
  else {
    std::cerr << "Invalid option value. Use 1, 2, or 3." << std::endl;
    file->Close(); delete file;
    delete gRAA_PtVsA_Tot_ptBin;
    return nullptr;
  }

  TString ptrange = Form("p_{T} = [%.1f, %.1f] GeV/c", pLow[ptBin], pHigh[ptBin]);

  TCanvas *cRAAvsA = new TCanvas("cRAAvsA", "", 800, 600);
  TH2F *hRAA_PtVsA_Frame =
      new TH2F("hRAA_PtVsA_Frame", Form("; A^{%d/3}; R_{AA}", option), 100,
               xrangemin, xrangemax, 100, 0.0, 1.2);
  hRAA_PtVsA_Frame->Draw();

  gRAA_PtVsA_Tot_ptBin->SetMarkerStyle(20);
  gRAA_PtVsA_Tot_ptBin->SetMarkerColor(kBlack);
  gRAA_PtVsA_Tot_ptBin->SetLineColor(kBlack);
  gRAA_PtVsA_Tot_ptBin->Draw("PE same");

  cRAAvsA->Update();

  // linear fit within range
  TF1 *f1 = new TF1("f1", "[0] + [1]*x", xrangemin, xrangemax);
  gRAA_PtVsA_Tot_ptBin->Fit(f1, "R", "", xrangemin, xrangemax);
  f1->SetLineColor(kRed);
  f1->Draw("same");

  TPaveText *pt = new TPaveText(0.6, 0.6, 0.9, 0.9, "NDC");
  pt->SetFillColor(0);
  pt->SetBorderSize(0);
  pt->AddText(Form("Fit: y = %.2f + %.2f x", f1->GetParameter(0), f1->GetParameter(1)));
  pt->AddText(Form("#chi^{2}/ndf = %.2f/%d", f1->GetChisquare(), f1->GetNDF()));
  pt->Draw("same");

  cRAAvsA->SaveAs(Form("cRAAvsA_ptBin%d_option%d.png", ptBin, option));

  // done with the input file; close it before returning (prevents gDirectory issues)
  file->Close();
  delete file;

  return gRAA_PtVsA_Tot_ptBin;
}

// Loop over ptBins and options; write all graphs to one output file
void extractAll(TString fileName = "ptBinned_RAAVsA.root",
                TString outFileName = "extractedGraphs.root") {
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);

  TFile *fileout = TFile::Open(outFileName, "RECREATE");
  if (!fileout || fileout->IsZombie()) {
    std::cerr << "Error opening output file: " << outFileName << std::endl;
    return;
  }

  for (int ptBin = 0; ptBin < nPt; ptBin++) {
    for (int option = 1; option <= 3; option++) {
      TGraphAsymmErrors *gRAA_PtVsA_Tot_ptBin = extract(ptBin, option, fileName);
      if (gRAA_PtVsA_Tot_ptBin) {
        fileout->cd(); // ensure we write into the output file
        // Keep original name if you prefer; here we save with explicit "Tot"
        gRAA_PtVsA_Tot_ptBin->Write(
            Form("gRAA_PtVsA_Tot_ptBin%d_option%d", ptBin, option));
        delete gRAA_PtVsA_Tot_ptBin; // avoid leaks across loops
      }
    }
  }

  fileout->Close();
  delete fileout;
}
