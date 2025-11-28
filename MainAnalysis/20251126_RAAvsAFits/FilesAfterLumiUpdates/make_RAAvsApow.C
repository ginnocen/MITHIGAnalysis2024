#include "TFile.h"
#include "TKey.h"
#include "TClass.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"

#include <iostream>
#include <string>
#include <cmath>

// power = 2 -> x' = A^{2/3}
// power = 3 -> x' = A
void make_RAAvsApow(int power = 3,
                    const char* infile  = "ptBinned_RAAVsA_Pow1-3_Version1.root",
                    const char* outfile = "")
{
  if (power != 2 && power != 3) {
    std::cerr << "Error: power must be 2 or 3 (got " << power << ")" << std::endl;
    return;
  }

  // Build output file name if not explicitly provided
  std::string outname = Form("ptBinned_RAAVsA_Pow%d-3_Version1.root", power);
  TFile *fin = TFile::Open(infile, "READ");
  if (!fin || fin->IsZombie()) {
    std::cerr << "Error: cannot open input file " << infile << std::endl;
    return;
  }

  TFile *fout = TFile::Open(outname.c_str(), "RECREATE");
  if (!fout || fout->IsZombie()) {
    std::cerr << "Error: cannot create output file " << outname << std::endl;
    fin->Close();
    return;
  }

  std::cout << "Converting x = A^{1/3} -> x' = A^(" << power << "/3)"
            << " in file " << infile
            << " and writing to " << outname << std::endl;

  TIter next(fin->GetListOfKeys());
  TKey *key = nullptr;

  while ((key = (TKey*)next())) {
    TObject *obj = key->ReadObj();
    if (!obj) continue;

    // ---- TGraphAsymmErrors ----
    if (obj->InheritsFrom(TGraphAsymmErrors::Class())) {
      auto *g  = dynamic_cast<TGraphAsymmErrors*>(obj);
      auto *gn = (TGraphAsymmErrors*)g->Clone(); // same name & title

      for (int i = 0; i < gn->GetN(); ++i) {
        double x, y;
        gn->GetPoint(i, x, y);    // x = A^{1/3}

        double exl = gn->GetErrorXlow(i);
        double exh = gn->GetErrorXhigh(i);

        const double xnew    = std::pow(x, power);           // x' = x^p
        const double dxdxold = power * std::pow(x, power-1); // dx'/dx

        const double exl_new = dxdxold * exl;
        const double exh_new = dxdxold * exh;

        gn->SetPoint(i, xnew, y);
        gn->SetPointEXlow(i,  exl_new);
        gn->SetPointEXhigh(i, exh_new);
      }

      gn->Write();   // same name as original
      continue;
    }

    // ---- TGraphErrors ----
    if (obj->InheritsFrom(TGraphErrors::Class())) {
      auto *g  = dynamic_cast<TGraphErrors*>(obj);
      auto *gn = (TGraphErrors*)g->Clone();

      for (int i = 0; i < gn->GetN(); ++i) {
        double x, y;
        gn->GetPoint(i, x, y);    // x = A^{1/3}

        double ex = gn->GetErrorX(i);

        const double xnew    = std::pow(x, power);
        const double dxdxold = power * std::pow(x, power-1);
        const double ex_new  = dxdxold * ex;

        gn->SetPoint(i, xnew, y);
        gn->SetPointError(i, ex_new, gn->GetErrorY(i));
      }

      gn->Write();
      continue;
    }

    // ---- Plain TGraph ----
    if (obj->InheritsFrom(TGraph::Class())) {
      auto *g  = dynamic_cast<TGraph*>(obj);
      auto *gn = (TGraph*)g->Clone();

      for (int i = 0; i < gn->GetN(); ++i) {
        double x, y;
        gn->GetPoint(i, x, y);    // x = A^{1/3}
        const double xnew = std::pow(x, power);
        gn->SetPoint(i, xnew, y);
      }

      gn->Write();
      continue;
    }

    // ---- Non-graph objects: copy unchanged ----
    obj->Write();
  }

  fout->Write();
  fout->Close();
  fin->Close();

  std::cout << "Done." << std::endl;
}
