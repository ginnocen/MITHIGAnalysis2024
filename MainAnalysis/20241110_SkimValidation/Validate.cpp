/* + Descriptions:
 *		Macro to compare root files create in either DumpNewSkim or DumpOldSkim
 * + Output:
 * 		+ Will create a summary pdf for the histograms overlay and ratio
 * 		+ Calculate and print chisq for two skims, chisq=0 means the two are exactly the same
 */
#include <string>

#include "CommandLine.h"

#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include "TMath.h"
#include "TStyle.h"
#include "ValidateHist.h"

using namespace std;

void formatLegend(TLegend *leg, double textsize = 27) {
  leg->SetBorderSize(0);
  leg->SetTextFont(43);
  leg->SetTextSize(textsize);
  leg->SetFillStyle(0);
  leg->SetFillColor(0);
  leg->SetLineColor(0);
}

template <class T>
void sethempty(T *hempty, Float_t xoffset = 0, Float_t yoffset = 0, Float_t xsize = 1, Float_t ysize = 1) {
  hempty->GetXaxis()->CenterTitle();
  hempty->GetYaxis()->CenterTitle();
  hempty->GetXaxis()->SetTitleOffset(1.10 + xoffset);
  hempty->GetYaxis()->SetTitleOffset(1.30 + yoffset);
  hempty->GetXaxis()->SetTitleSize(0.05 * xsize);
  hempty->GetYaxis()->SetTitleSize(0.05 * ysize);
  hempty->GetXaxis()->SetTitleFont(42);
  hempty->GetYaxis()->SetTitleFont(42);
  hempty->GetXaxis()->SetLabelFont(42);
  hempty->GetYaxis()->SetLabelFont(42);
  hempty->GetXaxis()->SetLabelSize(0.045 * xsize);
  hempty->GetYaxis()->SetLabelSize(0.045 * ysize);
  hempty->SetStats(0);
}

void doRatio(TH1D *ref, const std::vector<TH1D *> &cmp_vec, TPad *motherPad, TLegend *leg,
             string ratioYAxisName = "Ratio", bool logy = false, string curvePlotStyle = "h", double padSplit = 0.3) {
  int numBinsX = ref->GetNbinsX();
  double xMin = ref->GetXaxis()->GetXmin();
  double xMax = ref->GetXaxis()->GetXmax();

  sethempty(ref, 0, padSplit / 3);

  TH1D *hempty_ratio =
      new TH1D(Form("hempty_ratio_%s", ref->GetName()),
               Form(";%s;%s", ref->GetXaxis()->GetTitle(), ratioYAxisName.c_str()), numBinsX, xMin, xMax);

  sethempty(hempty_ratio, 0, -2 * padSplit, 0.1, 0.1);
  hempty_ratio->GetYaxis()->SetNdivisions(505);
  hempty_ratio->GetYaxis()->SetLabelSize(0.1);
  hempty_ratio->GetXaxis()->SetLabelSize(0.1);

  hempty_ratio->SetTitle("");
  hempty_ratio->GetXaxis()->SetTitleSize(hempty_ratio->GetXaxis()->GetTitleSize() * 20.0);
  hempty_ratio->GetXaxis()->SetLabelSize(hempty_ratio->GetXaxis()->GetLabelSize());
  hempty_ratio->GetYaxis()->SetTitleSize(hempty_ratio->GetYaxis()->GetTitleSize() * 20.0);
  hempty_ratio->GetYaxis()->SetLabelSize(hempty_ratio->GetYaxis()->GetLabelSize());
  hempty_ratio->GetXaxis()->SetTitleOffset(0); // hempty_ratio->GetXaxis()->GetTitleOffset());
  hempty_ratio->GetYaxis()->SetTitleOffset(0); // hempty_ratio->GetYaxis()->GetTitleOffset() * 0.42);
  hempty_ratio->GetYaxis()->SetNdivisions(205);

  // hempty_ratio->GetXaxis()->SetRangeUser(0,xRangeMax);
  hempty_ratio->GetYaxis()->SetRangeUser(0.95, 1.05);

  motherPad->cd();
  motherPad->Divide(1, 2, 0, 0);
  TPad *pad1 = (TPad *)motherPad->GetPad(1);
  TPad *pad2 = (TPad *)motherPad->GetPad(2);
  pad1->SetPad(0.0, 0.30, 1.0, 1.0);
  pad2->SetPad(0.0, 0.0, 1.0, 0.30);
  motherPad->SetLogy();
  motherPad->SetTickx(1);
  motherPad->SetTicky(1);

  pad1->SetTopMargin(0.1);
  pad1->SetBottomMargin(0);
  pad1->SetLeftMargin(0.15);
  pad1->SetRightMargin(0.05);
  if (logy)
    pad1->SetLogy();
  pad1->SetTickx(1);
  pad1->SetTicky(1);
  pad1->Draw();
  pad2->SetTopMargin(0);
  pad2->SetBottomMargin(0.3);
  pad2->SetLeftMargin(0.15);
  pad2->SetRightMargin(0.05);
  pad2->SetTickx(1);
  pad2->SetTicky(1);
  pad2->Draw();

  pad1->cd();

  ref->DrawClone(curvePlotStyle.c_str());
  for (auto cmp : cmp_vec) {
    cmp->DrawClone(Form("%s same", curvePlotStyle.c_str()));
  }
  if (leg)
    leg->DrawClone("same");

  pad2->cd();
  hempty_ratio->DrawClone();
  for (auto cmp : cmp_vec) {
    TH1D *hratio = (TH1D *)cmp->Clone(Form("hratio_%s", cmp->GetName()));
    hratio->Divide(ref);
    hratio->DrawClone("ple same");
  }

  TLine *line = new TLine(xMin, 1, xMax, 1);
  line->SetLineStyle(2);
  line->SetLineColor(kGray + 2);
  line->DrawClone("same");

  delete line;
  delete hempty_ratio;
}

double getChiSquare(TH1D *h1, TH1D *h2) {
  // Check that the histograms have the same binning
  if (h1->GetNbinsX() != h2->GetNbinsX()) {
    std::cerr << "Histograms have different binning." << std::endl;
    return -1;
  }

  double chiSquare = 0.;
  for (int i = 1; i <= h1->GetNbinsX(); ++i) {
    double content1 = h1->GetBinContent(i);
    double content2 = h2->GetBinContent(i);

    double error1 = h1->GetBinError(i);
    double error2 = h2->GetBinError(i);

    // Calculate the denominator (combined variance from two independent random variables)
    double denominator = error1 * error1 + error2 * error2;

    // Add the contribution to the chi-square, checking for zero denominator
    if (denominator != 0) {
      chiSquare += (content1 - content2) * (content1 - content2) / denominator;
    } else {
      if (content1 != 0 || content2 != 0)
        std::cerr << "Warning: Zero denominator in bin " << i << std::endl;
    }
  }
  return (chiSquare);
}

int main(int argc, char *argv[]) {
  CommandLine CL(argc, argv);

  string VHName1 = CL.Get("Input1");
  string VHName2 = CL.Get("Input2");
  string LegendName1 = CL.Get("LegendName1");
  string LegendName2 = CL.Get("LegendName2");
  string PlotName = CL.Get("PlotName");

  ValidateHist v1(VHName1, "r");
  ValidateHist v2(VHName2, "r");

  vector<string> histNameToPlot;
  for (const auto &[key, hist1] : v1.histDict) {
    if (!v2.histDict[key]) {
      printf("[Warning] Couldn't find hist (%s) in %s. Skipping the comparison.\n", key.c_str(), VHName2.c_str());
      return 1;
    }
    TH1D *hist2 = v2.histDict[key];
    if (hist1->GetEntries() == 0 && hist2->GetEntries() == 0)
      continue;

    histNameToPlot.push_back(key);
  }

  int _nHist = histNameToPlot.size();
  int _nCol = 5;
  int _nRow = TMath::Ceil(_nHist / ((float)_nCol));
  TCanvas *canv = new TCanvas("canv", "", 600 * _nCol, 600 * _nRow);
  canv->Divide(_nCol, _nRow);

  TLegend *legend = new TLegend(0.6, 0.7, 0.8, 0.9);

  gStyle->SetOptStat(0);

  for (int iPlot = 0; iPlot < histNameToPlot.size(); ++iPlot) {
    string key = histNameToPlot[iPlot];
    TH1D *hist1 = v1.histDict[key];
    TH1D *hist2 = v2.histDict[key];

    hist1->SetLineColor(kBlue);
    // hist1->SetLineWidth(2);

    hist2->SetFillColorAlpha(kRed, 0.3);
    hist2->SetLineColorAlpha(kRed, 0.3);

    if (iPlot == 0) {
      legend->AddEntry(hist1, LegendName1.c_str(), "le");
      legend->AddEntry(hist2, LegendName2.c_str(), "lf");
      formatLegend(legend, 21);
    }

    double chisq = getChiSquare(hist1, hist2);
    printf("%s: chisq=%f\n", key.c_str(), chisq);

    doRatio(hist2, {hist1}, (TPad *)canv->GetPad(iPlot + 1), (iPlot == 0) ? legend : nullptr,
            Form("%s/%s", LegendName1.c_str(), LegendName2.c_str()),
            (key == "VX" || key == "VXError" || key == "VY" || key == "VYError" || key == "VZ" || key == "VZError" ||
             key == "DsvpvDistance" || key == "DsvpvDisErr" || key == "DsvpvDistance_2D" || key == "DsvpvDisErr_2D")
                ? true
                : false);
  }

  canv->SaveAs(PlotName.c_str());

  return 0;
}
