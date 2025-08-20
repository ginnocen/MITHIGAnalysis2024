// Minimal ROOT macro for R_AA graphs
// Usage:
//   root -l -b -q 'MakeRAAGraph.C+' -e 'DrawRAA("data.csv","Label")'

#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TLegend.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <utility>
#include <TH2F.h>
#include <TFile.h>
#include <TStyle.h>
namespace {
  inline std::string trim(std::string s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
      [](int ch){ return !std::isspace(ch); }));
    s.erase(std::find_if(s.rbegin(), s.rend(),
      [](int ch){ return !std::isspace(ch); }).base(), s.end());
    return s;
  }
  inline bool skip_line(const std::string& s) {
    return s.empty() || std::isalpha((unsigned char)s[0]) || s[0]=='"';
  }
}

std::pair<TGraphAsymmErrors*, TGraphAsymmErrors*>
MakeRAAGraph(const char* txtfile)
{
  std::ifstream in(txtfile);
  if (!in) { std::cerr << "Cannot open " << txtfile << "\n"; return {}; }

  std::vector<double> x, y, estat, esyst;
  std::string line;
  while (std::getline(in, line)) {
    line = trim(line);
    if (skip_line(line)) continue;

    std::stringstream ss(line);
    std::string spt, sraa, sstat, ssyst;
    if (!(std::getline(ss, spt, ',') &&
          std::getline(ss, sraa, ',') &&
          std::getline(ss, sstat, ',') &&
          std::getline(ss, ssyst, ','))) continue;

    try {
      x.push_back(std::stod(trim(spt)));
      y.push_back(std::stod(trim(sraa)));
      estat.push_back(std::stod(trim(sstat)));
      esyst.push_back(std::stod(trim(ssyst)));
    } catch (...) { /* skip malformed row */ }
  }
  if (x.empty()) { std::cerr << "No data in " << txtfile << "\n"; return {}; }

  const int n = (int)x.size();
  auto gstat = new TGraphAsymmErrors(n, x.data(), y.data(),
                                     nullptr, nullptr, estat.data(), estat.data());
  gstat->SetName("gRAA_stat");

  auto gsyst = new TGraphAsymmErrors(n, x.data(), y.data(),
                                     nullptr, nullptr, esyst.data(), esyst.data());
  gsyst->SetName("gRAA_syst");

  return {gsyst, gstat};
}

// Backward-compat overload (ignore 2nd arg)
std::pair<TGraphAsymmErrors*, TGraphAsymmErrors*>
MakeRAAGraph(const char* txtfile, const char*) {
  return MakeRAAGraph(txtfile);
}

void DrawRAA(const char* txtfile, const char* label="RAA")
{
  gStyle->SetOptStat(0); // disable stats box

  auto pr = MakeRAAGraph(txtfile);
  auto gsyst = pr.first;
  auto gstat = pr.second;
  if (!gsyst || !gstat) return;

  TCanvas* c = new TCanvas("cRAA","",750, 600);
  float min, max;
  TString title = "R_{AA}";
  // The frame defines the axis ranges you want:
  min = 0.6, max = 1.1;       // R_AA plot range
  TH2F* hEmpty = new TH2F("hEmpty",Form(";p_T (GeV/c);%s", title.Data()),
                          10, 0, 30, 10, min, max);
  hEmpty->SetStats(0);
  hEmpty->SetTitleOffset(1.15, "Y");
  hEmpty->SetTitleOffset(1.1, "X");
  hEmpty->SetTitleFont(42);
  hEmpty->SetTitleSize(0.05);
  hEmpty->GetYaxis()->SetTitleSize(0.04);
  hEmpty->GetXaxis()->SetTitleSize(0.04);
  hEmpty->GetYaxis()->SetTitleFont(42);
  hEmpty->GetXaxis()->SetTitleFont(42);
  hEmpty->GetYaxis()->SetLabelFont(42);
  hEmpty->GetXaxis()->SetLabelFont(42);
  hEmpty->GetYaxis()->SetLabelSize(0.04);
  hEmpty->GetXaxis()->SetLabelSize(0.04);
  hEmpty->Draw();                 // draw axes/frame

  // IMPORTANT: do NOT use "A" here — that would create a new frame

  // Stat points on top
  gstat->SetMarkerStyle(20);
  gstat->SetMarkerSize(1);
  gstat->SetLineColor(kRed);
  gstat->SetMarkerColor(kRed);
  gstat->Draw("PZ SAME");

  gsyst->SetFillStyle(1001);
  gsyst->SetFillColor(kGray+1);     // avoid alpha; solid fill is safest across ROOT versions
  gsyst->SetLineColor(kGray+1);
  //draw graph as boxes filled
  gsyst->Draw("2 SAME");          // filled boxes on the existing frame
  
  gPad->Update();
  gPad->RedrawAxis();             // refresh axes after filled objects

  TLegend* leg = new TLegend(0.15, 0.7, 0.35, 0.85, label);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.04);
  leg->SetTextFont(42);
  leg->AddEntry(gstat, "Statistical uncertainty", "p");
  leg->AddEntry(gsyst, "Systematic uncertainty", "f");
  leg->Draw();
  c->SaveAs(TString::Format("%s.png", label));
  
  TFile *fout = new TFile(TString::Format("%s.root", label), "RECREATE");
  if (fout->IsOpen()) {
    fout->cd();
    gsyst->Write();
    gstat->Write();
    fout->Close();
  } else {
    std::cerr << "Failed to create output file\n";
  }
  delete c;                       // clean up
  delete hEmpty;                  // clean up
  delete gsyst;                   // clean up
  delete gstat;                   // clean up
  std::cout << "Done. Output saved to " << label << ".root and " << label << ".png\n";
}
