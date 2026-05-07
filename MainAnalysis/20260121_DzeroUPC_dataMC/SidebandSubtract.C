// SidebandSubtract.C
//
// Usage examples:
//
// 1) If you saved a snapshot named "postfit":
// root -l -q 'SidebandSubtract.C(
//   "nt_data.root","nt",
//   "fit_workspace.root","w","postfit",
//   "mean","sigma1","sigma2","lambda",
//   "Dpt",
//   60,0.0,12.0,
//   1.70,2.05,
//   "sidebandSub_Dpt.root"
// )'
//
// 2) If you did NOT save a snapshot (read current ws values):
// root -l -q 'SidebandSubtract.C(
//   "nt_data.root","nt",
//   "fit_workspace.root","w","",
//   "mean","sigma1","sigma2","lambda",
//   "Dpt",
//   60,0.0,12.0,
//   1.70,2.05,
//   "sidebandSub_Dpt.root"
// )'

#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TString.h>

#include <RooWorkspace.h>
#include <RooRealVar.h>

#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>

// ---- helpers ----
static bool GetWSVarVal(RooWorkspace* ws, const char* name, double& out)
{
  if (!ws) return false;
  RooRealVar* v = ws->var(name);
  if (!v) return false;
  out = v->getVal();
  return true;
}

// Integral of exp(lambda*m) dm from a to b
static double IntExp(double lambda, double a, double b)
{
  if (a > b) std::swap(a, b);
  const double eps = 1e-12;
  if (std::abs(lambda) < eps) return (b - a);
  return (std::exp(lambda*b) - std::exp(lambda*a)) / lambda;
}

// ---- main ----
void SidebandSubtract(
  const char* ntFile,
  const char* treeName,

  const char* wsFile,
  const char* wsName,
  const char* snapshotName,   // pass "" to skip loadSnapshot

  const char* meanName,       // e.g. "mean" or "SignalParams_mean"
  const char* sigma1Name,     // e.g. "sigma1"
  const char* sigma2Name,     // e.g. "sigma2"
  const char* lambdaName,     // e.g. "lambda" or "CombinatoricsBkgParams_lambda"

  const char* projBranch,     // e.g. "Dpt"
  int nbins, double xmin, double xmax,

  double massMin, double massMax, // mass range used for SB definition
  const char* outRoot)
{
  // --- open ntuple/tree ---
  TFile fnt(ntFile, "READ");
  if (fnt.IsZombie()) {
    std::cerr << "[SidebandSubtract] Cannot open ntFile=" << ntFile << "\n";
    return;
  }
  TTree* nt = (TTree*)fnt.Get(treeName);
  if (!nt) {
    std::cerr << "[SidebandSubtract] Cannot find tree " << treeName << " in " << ntFile << "\n";
    return;
  }

  // Quick check: does the projected branch exist?
  if (!nt->GetListOfLeaves()->FindObject(projBranch)) {
    std::cerr << "[SidebandSubtract] Branch/leaf '" << projBranch
              << "' not found in tree. Available leaves:\n";
    nt->GetListOfLeaves()->Print();
    return;
  }
  if (!nt->GetListOfLeaves()->FindObject("Dmass")) {
    std::cerr << "[SidebandSubtract] Leaf 'Dmass' not found in tree.\n";
    nt->GetListOfLeaves()->Print();
    return;
  }

  // --- open workspace ---
  TFile fws(wsFile, "READ");
  if (fws.IsZombie()) {
    std::cerr << "[SidebandSubtract] Cannot open wsFile=" << wsFile << "\n";
    return;
  }
  RooWorkspace* ws = (RooWorkspace*)fws.Get(wsName);
  if (!ws) {
    std::cerr << "[SidebandSubtract] Cannot find RooWorkspace '" << wsName
              << "' in " << wsFile << "\n";
    return;
  }

  // Load snapshot if provided
  if (snapshotName && TString(snapshotName).Length() > 0) {
    bool loaded = ws->loadSnapshot(snapshotName);
    if (!loaded) {
      std::cerr << "[SidebandSubtract] WARNING: snapshot '" << snapshotName
                << "' not found/failed to load. Proceeding with current ws values.\n";
    }
  }

  // --- read fit params from ws ---
  double mean=0, sigma1=0, sigma2=0, lambda=0;
  bool ok = true;
  ok &= GetWSVarVal(ws, meanName, mean);
  ok &= GetWSVarVal(ws, sigma1Name, sigma1);
  ok &= GetWSVarVal(ws, sigma2Name, sigma2);
  ok &= GetWSVarVal(ws, lambdaName, lambda);

  if (!ok) {
    std::cerr << "[SidebandSubtract] Missing one or more vars in workspace:\n"
              << "  meanName   = " << meanName   << "\n"
              << "  sigma1Name = " << sigma1Name << "\n"
              << "  sigma2Name = " << sigma2Name << "\n"
              << "  lambdaName = " << lambdaName << "\n"
              << "Dumping ws->allVars():\n";
    ws->allVars().Print("v");
    return;
  }

  // --- define signal window and sideband region ---
  const double sigW  = 2.0 * std::max(std::abs(sigma1), std::abs(sigma2));
  const double sigLo = mean - sigW;
  const double sigHi = mean + sigW;

  // Clamp to [massMin, massMax]
  const double sigLoC = std::max(sigLo, massMin);
  const double sigHiC = std::min(sigHi, massMax);

  // Signal: (sigLoC..sigHiC)
  // Sideband: (massMin..sigLoC) U (sigHiC..massMax)
  TString cutMassRange = Form("(Dmass>%.10g && Dmass<%.10g)", massMin, massMax);
  TString cutSig       = Form("(Dmass>%.10g && Dmass<%.10g)", sigLoC, sigHiC);
  TString cutSB        = Form("((Dmass>%.10g && Dmass<%.10g) && !(Dmass>%.10g && Dmass<%.10g))",
                              massMin, massMax, sigLoC, sigHiC);

  // --- book histograms ---
  TH1D* hSig = new TH1D(Form("hSig_%s", projBranch),
                        Form("%s (mass in signal);%s;Events", projBranch, projBranch),
                        nbins, xmin, xmax);
  TH1D* hSB  = new TH1D(Form("hSB_%s", projBranch),
                        Form("%s (mass in sideband);%s;Events", projBranch, projBranch),
                        nbins, xmin, xmax);
  TH1D* hAll = new TH1D(Form("hAll_%s", projBranch),
                        Form("%s (mass in range);%s;Events", projBranch, projBranch),
                        nbins, xmin, xmax);

  hSig->Sumw2(); hSB->Sumw2(); hAll->Sumw2();

  nt->Draw(Form("%s>>%s", projBranch, hSig->GetName()), cutSig.Data(), "goff");
  nt->Draw(Form("%s>>%s", projBranch, hSB->GetName()),  cutSB.Data(),  "goff");
  nt->Draw(Form("%s>>%s", projBranch, hAll->GetName()), cutMassRange.Data(), "goff");

  // --- scale factor from combinatorics-only mass pdf ---
  // Assumption consistent with your earlier pseudo-code: combinatoric component controlled by "lambda"
  // and behaves as exp(lambda * Dmass) over the mass variable.
  //
  // alpha = integral_sig / integral_sideband
  const double I_sig = IntExp(lambda, sigLoC, sigHiC);
  const double I_sb  = IntExp(lambda, massMin, sigLoC) + IntExp(lambda, sigHiC, massMax);

  if (I_sb <= 0) {
    std::cerr << "[SidebandSubtract] Bad I_sb (<=0). Check lambda and mass ranges.\n";
    std::cerr << "  lambda=" << lambda << " massMin=" << massMin << " massMax=" << massMax
              << " sigLoC=" << sigLoC << " sigHiC=" << sigHiC << "\n";
    return;
  }
  const double alpha = I_sig / I_sb;

  // --- subtraction ---
  TH1D* hSub = (TH1D*)hSig->Clone(Form("hSub_%s", projBranch));
  hSub->SetTitle(Form("%s (SB-subtracted);%s;Events", projBranch, projBranch));
  hSub->Add(hSB, -alpha);

  // --- write output ---
  TFile fout(outRoot, "RECREATE");
  hSub->Write();  // (1) sideband subtracted
  hSig->Write();  // (2) signal window
  hSB->Write();   // (3) sideband
  hAll->Write();  // (4) inclusive in mass range
  fout.Close();

  std::cout
    << "[SidebandSubtract] Done.\n"
    << "  WS: " << wsFile << " :: " << wsName
    << (snapshotName && TString(snapshotName).Length()>0 ? Form(" (snapshot=%s)", snapshotName) : "")
    << "\n"
    << "  Params: mean=" << mean << " sigma1=" << sigma1 << " sigma2=" << sigma2 << " lambda=" << lambda << "\n"
    << "  massRange=[" << massMin << "," << massMax << "]\n"
    << "  sigWindow=[" << sigLoC << "," << sigHiC << "]  (using 2*max(|sigma1|,|sigma2|))\n"
    << "  alpha(I_sig/I_sb)=" << alpha << "\n"
    << "  wrote: " << outRoot << "\n";
}
