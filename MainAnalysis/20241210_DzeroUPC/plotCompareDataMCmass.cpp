#include <TTree.h>
#include <TFile.h>
#include <TDirectoryFile.h>
#include <TChain.h>
#include <TH1D.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TLegend.h>

#include <RooAddPdf.h>
#include <RooAbsPdf.h>
#include <RooGaussian.h>
#include <RooCBShape.h>
#include <RooExponential.h>
#include <RooChebychev.h>
#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooFitResult.h>
#include <RooPlot.h>
#include <RooWorkspace.h>

#include "plotCrossSection.h"

using namespace std;
using namespace RooFit;

#define DMASS 1.86484
#define DMASSMIN 1.67
#define DMASSMAX 2.07
#define DMASSNBINS 32
string mcSource = "pthat0, forced D^{0}";
string mcIncSource = "pthat5";

struct ParamsBase {
  std::map<std::string, RooRealVar*> params; // Store RooRealVar objects

  ParamsBase() = default;

  virtual ~ParamsBase() = default; // Virtual destructor for proper cleanup

  // Assignment operator
  ParamsBase& operator=(const ParamsBase& other) {
    // Check for self-assignment
    if (this == &other) {
      return *this;
    }

    // Copy only the values and errors from other.params to this.params
    for (const auto& [name, var] : other.params) {
      if (params.find(name) != params.end()) {
        params[name]->setVal(var->getVal());
        params[name]->setAsymError(var->getErrorLo(), var->getErrorHi());
      } else {
        std::cerr << "Warning: Parameter " << name << " not found in params.\n";
      }
    }

    return *this; // Return the current object
  }

  // Method to print all parameters
  void print() const {
    for (const auto& [name, var] : params) {
      std::cout << name << " (" << var->GetTitle() << "): "
                << "Value = " << var->getVal()
                << ", Error = " << var->getError()
                << ", is constant = " << var->isConstant() << "\n";
    }
  }

  // Write parameters to .dat file
  void writeToDat(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
      std::cerr << "Error: Could not open file " << filename << " for writing.\n";
      return;
    }

    // Write header
    file << "name,plotTitle,value,lower_error,upper_error\n";

    // Write parameter data
    for (const auto& [name, var] : params) {
      file << name << ","
           << var->GetTitle() << ","
           << var->getVal() << ","
           << var->getErrorLo() << ","
           << var->getErrorHi() << "\n";
    }

    file.close();
    std::cout << "Parameters written to " << filename << "\n";
  }

  // Read parameters from .dat file
  void readFromDat(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
      std::cerr << "Error: Could not open file " << filename << " for reading.\n";
      return;
    }

    std::string line;
    bool isHeader = true;

    // this->print();

    while (std::getline(file, line)) {
      if (isHeader) {
        isHeader = false; // Skip the header line
        continue;
      }

      std::istringstream stream(line);
      std::string name, plotTitle, valueStr, lowerErrorStr, upperErrorStr;

      // Parse line
      if (std::getline(stream, name, ',') &&
          std::getline(stream, plotTitle, ',') &&
          std::getline(stream, valueStr, ',') &&
          std::getline(stream, lowerErrorStr, ',') &&
          std::getline(stream, upperErrorStr, ','))
      {
        double value = std::stod(valueStr);
        double lowerError = std::stod(lowerErrorStr);
        double upperError = std::stod(upperErrorStr);

        // Update parameter if it exists
        if (params.find(name) != params.end()) {
          auto& var = params[name];
          var->setVal(value);                                 // Set value
          var->setAsymError(lowerError, upperError);          // Asymmetric error
          var->setError((- lowerError + upperError)/2.);      // Approximate symmetric error
          var->setRange(value+lowerError, value+upperError);  // Range set to the +/- 1 sigma range
          var->setConstant(true);                             // [Warning!] Now the strategy is to fixed all the prefit parameters
        } else {
          std::cerr << "Warning: Parameter " << name << " not found in params map. Skipping.\n";
        }
      } else {
        std::cerr << "Error: Malformed line in " << filename << ": " << line << "\n";
      }
    }

    file.close();

    // this->print();

  }
};

struct SignalParams : public ParamsBase {
  RooRealVar mean;
  RooRealVar sigma1;
  RooRealVar sigma2;
  RooRealVar frac1;
  RooRealVar alpha;

  SignalParams() :
    mean("sig_mean", "[signal] mean", DMASS, DMASS - 0.015, DMASS + 0.015),
    sigma1("sig_sigma1", "[signal] width of first Gaussian", 0.03, 0.0048, 0.155),
    sigma2("sig_sigma2", "[signal] width of second Gaussian", 0.01, 0.0048, 0.0465),
    frac1("sig_frac1", "[signal] fraction of first Gaussian", 0.1, 0.001, 0.5),
    alpha("sig_alpha", "[signal] modification to data Gaussian width", 0.0, -0.25, 0.25)
  {
    // cout << "signal default" << endl;
    params[mean.GetName()] = &mean;
    params[sigma1.GetName()] = &sigma1;
    params[sigma2.GetName()] = &sigma2;
    params[frac1.GetName()] = &frac1;
    params[alpha.GetName()] = &alpha;
  }

  SignalParams(string dat) : SignalParams() { readFromDat(dat); }
  SignalParams(string dat, double sigMeanRange, double sigAlphaRange) : SignalParams()
  {
    readFromDat(dat);
    // Nominal model lets mean of data Gaussian float
    if (sigMeanRange > 0.)
    {
      mean.setConstant(false);
      mean.setRange(DMASS - sigMeanRange, DMASS + sigMeanRange);
    }
    // Nominal model lets width of data Gaussian float
    if (sigAlphaRange > 0.)
    {
      alpha.setConstant(false);
      alpha.setRange(0.0 - sigAlphaRange, 0.0 + sigAlphaRange);
    }
  }
};

struct CombinatoricsBkgParams : public ParamsBase {
  bool doSyst; // nominal configuration is an exponential

  // exponential
  RooRealVar lambda;

  // 2nd order chebyshev polynomial
  RooRealVar a0;
  RooRealVar a1;

  CombinatoricsBkgParams(bool _doSyst=false) :
    doSyst(_doSyst),
    lambda("lambda", "lambda", -4.0, -10.0, 0.0),
    a0("a0", "constant", 0.5, -1.0, 1.0),
    a1("a1", "slope", -0.2, -1.0, 1.0)
  {
    // switch to allow the using the functionalities in the base class:
    // print, write and read
    if (doSyst)
    {
      params[a0.GetName()] = &a0;
      params[a1.GetName()] = &a1;
    }
    else
    {
      params[lambda.GetName()] = &lambda;
    }
  }

  CombinatoricsBkgParams(string dat, bool _doSyst=false) : CombinatoricsBkgParams(_doSyst) { readFromDat(dat); }
};

struct SwapParams : public ParamsBase {
  RooRealVar mean;
  RooRealVar sigma;

  SwapParams() :
    mean("swap_mean", "[swap] mean", DMASS, DMASS - 0.015, DMASS + 0.015),
    sigma("swap_sigma", "[swap] width of  Gaussian", 0.1, 0.03, 3.1)
  {
    params[mean.GetName()] = &mean;
    params[sigma.GetName()] = &sigma;
  }

  SwapParams(string dat) : SwapParams() { readFromDat(dat); }
};

struct EventParams {
  RooRealVar nsig;
  RooRealVar nbkg;

  RooRealVar fswp;
  RooRealVar fpkkk;
  RooRealVar fpkpp;

  // A set of pointers and references are declared for the RooFormulaVar objects
  // since the class doesn't support the operator=
  std::unique_ptr<RooFormulaVar> nswpPtr;
  std::unique_ptr<RooFormulaVar> npkkkPtr;
  std::unique_ptr<RooFormulaVar> npkppPtr;

  RooFormulaVar& nswp;  // Reference for convenience
  RooFormulaVar& npkkk;
  RooFormulaVar& npkpp;


  // Print method
  void print() const {
    std::cout << "nsig: Value = " << nsig.getVal() << ", Error = " << nsig.getError() << ", is constant = " << nsig.isConstant() << "\n";
    std::cout << "nbkg: Value = " << nbkg.getVal() << ", Error = " << nbkg.getError() << ", is constant = " << nbkg.isConstant() << "\n";
    std::cout << "fswp: Value = " << fswp.getVal() << ", is constant = " << fswp.isConstant() << "\n";
    std::cout << "fpkkk: Value = " << fpkkk.getVal() << ", is constant = " << fpkkk.isConstant() << "\n";
    std::cout << "fpkpp: Value = " << fpkpp.getVal() << ", is constant = " << fpkpp.isConstant() << "\n";
    std::cout << "nswp: Formula = " << nswp.GetName() << ", Value = " << nswp.getVal() << "\n";
    std::cout << "npkkk: Formula = " << npkkk.GetName() << ", Value = " << npkkk.getVal() << "\n";
    std::cout << "npkpp: Formula = " << npkpp.GetName() << ", Value = " << npkpp.getVal() << "\n";
  }

  // Write to file
  static void writeFracToDat(const std::string& filename, double _fswp, double _fpkkk, double _fpkpp) {
    std::ofstream file(filename);
    if (!file.is_open()) {
      std::cerr << "Error: Could not open file " << filename << " for writing.\n";
      return;
    }

    file << "name,value\n";
    file << "fswp," << _fswp << "\n";
    file << "fpkkk," << _fpkkk << "\n";
    file << "fpkpp," << _fpkpp << "\n";
    file.close();
    std::cout << "Parameters written to " << filename << "\n";
  }

  // Read from file
  void readFracFromDat(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
      std::cerr << "Error: Could not open file " << filename << " for reading.\n";
      return;
    }

    std::string line;
    bool isHeader = true;

    while (std::getline(file, line)) {
      if (isHeader) {
        isHeader = false;
        continue;
      }

      std::istringstream stream(line);
      std::string name, valueStr;

      if (std::getline(stream, name, ',') &&
          std::getline(stream, valueStr, ',')) {
        double value = std::stod(valueStr);

        if (name == "fswp")
        {
          fswp.setVal(value);
        }
        else if (name == "fpkkk")
        {
          fpkkk.setVal(value);
        }
        else if (name == "fpkpp")
        {
          fpkpp.setVal(value);
        }
        else {
          std::cerr << "Warning: Parameter " << name << " not found. Skipping.\n";
        }
      } else {
        std::cerr << "Error: Malformed line in " << filename << ": " << line << "\n";
      }
    }

    file.close();
  }

  // Constructor
  EventParams(double _nsig = 500, double _nbkg = 500,
              double _fswp = 0.5, double _fpkkk = 0.0, double _fpkpp = 0.5)
      : nsig("nsig", "number of signal events", _nsig, 0, _nsig * 5),
        nbkg("nbkg", "number of background events", _nbkg, 0, _nbkg * 5),
        fswp("fswp", "fswp", _fswp),
        fpkkk("fpkkk", "fpkkk", _fpkkk),
        fpkpp("fpkpp", "fpkpp", _fpkpp),
        nswpPtr(std::make_unique<RooFormulaVar>("nswp", "nswp", "@0*@1", RooArgList(nsig, fswp))),
        npkkkPtr(std::make_unique<RooFormulaVar>("npkkk", "npkkk", "@0*@1", RooArgList(nsig, fpkkk))),
        npkppPtr(std::make_unique<RooFormulaVar>("npkpp", "npkpp", "@0*@1", RooArgList(nsig, fpkpp))),
        nswp(*nswpPtr),  // Initialize references
        npkkk(*npkkkPtr),
        npkpp(*npkppPtr) {}

  // Constructor
  EventParams(string dat, double _nsig = 500, double _nbkg = 500)
      : EventParams(_nsig, _nbkg) { readFracFromDat(dat); }

  // Copy assignment operator
  EventParams& operator=(const EventParams& other) {
    if (this != &other) {
      // Copy RooRealVars
      nsig = other.nsig;
      nbkg = other.nbkg;
      fswp = other.fswp;
      fpkkk = other.fpkkk;
      fpkpp = other.fpkpp;

      // No reassignment of pointers and references is needed, since they're bounded as how they should be
    }
    return *this;
  }
};


void compareBackgroundFit(
  string binTag,
  string plotTitle,
  string pathData,
  string pathMCIncl,
  string pathCombdata,
  bool doubleRebin = false
) {
  TFile* data = TFile::Open(pathData.c_str(), "READ");
  TFile* mcincl = TFile::Open(pathMCIncl.c_str(), "READ");
  CombinatoricsBkgParams comb = CombinatoricsBkgParams(pathCombdata);
  
  TH1D* hDmass;
  hDmass = (TH1D*) data->Get("hDmass");
  TH1D* hDmassData = (TH1D*) hDmass->Clone("hDmassData");
  hDmassData->SetTitle(plotTitle.c_str());
  hDmass->Clear();
  hDmass = (TH1D*) mcincl->Get("hDmass");
  TH1D* hDmassMCIncl = (TH1D*) hDmass->Clone("hDmassMCIncl");
  hDmassMCIncl->SetTitle(plotTitle.c_str());
  delete hDmass;
  
  int rebinFactor = 4;
//  if (doubleRebin) rebinFactor = 2 * rebinFactor;
  hDmassData->Scale(1/(hDmassData->Integral() * rebinFactor));
  hDmassMCIncl->Scale(1/(hDmassMCIncl->Integral() * rebinFactor));
  hDmassData->Rebin(rebinFactor);
  hDmassMCIncl->Rebin(rebinFactor);
  TH1D* hDmassBase = new TH1D(
    "hDmassBase", plotTitle.c_str(),
    DMASSNBINS, DMASSMIN, DMASSMAX
  );
  
  TH1D* hDmassRatio = (TH1D*) hDmassData->Clone("hDmassRatio");
  hDmassRatio->Divide(hDmassData, hDmassMCIncl);
  
  RooRealVar m("Dmass", "Mass [GeV]", DMASSMIN, DMASSMAX);
  m.setRange("DMassRange", DMASSMIN, DMASSMAX);
  RooExponential combPDF("combPDF", "combinatorics model", m, comb.lambda);
  
  hDmassData->SetMarkerColor(kBlack);
  hDmassData->SetMarkerStyle(20);
  hDmassData->SetLineColor(kBlack);
  hDmassData->SetLineWidth(2);
  hDmassMCIncl->SetMarkerColor(kRed);
  hDmassMCIncl->SetMarkerStyle(0);
  hDmassMCIncl->SetLineColor(kRed);
  hDmassMCIncl->SetLineWidth(2);
  RooPlot *frame = m.frame();
  combPDF.plotOn(frame, LineWidth(2), LineStyle(1), LineColor(kBlue));
  hDmassBase->SetMinimum(0.);
  hDmassBase->SetMaximum(0.05);
  
  hDmassRatio->SetTitle("; Mass (GeV); Data/MC");
  hDmassRatio->SetMarkerColor(kBlack);
  hDmassRatio->SetMarkerStyle(0);
  hDmassRatio->SetLineColor(kBlack);
  hDmassRatio->SetLineWidth(1);
  hDmassRatio->SetMinimum(0.4);
  hDmassRatio->SetMaximum(1.6);
  
  TCanvas* canvas = new TCanvas("canvas", "", 600, 900);
  TPad* padTop = new TPad("padTop", "", 0.0, 0.3, 1.0, 1.0);
  TPad* padBot = new TPad("padBot", "", 0.0, 0.0, 1.0, 0.3);
  padTop->SetMargin(0.15, 0.05, 0.00, 0.16);
  padBot->SetMargin(0.15, 0.05, 0.30, 0.00);
  padTop->Draw();
  padBot->Draw();
  
  padTop->cd();
  hDmassBase->Draw();
  hDmassBase->SetLabelSize(0.03/0.7, "Y");
  hDmassBase->SetTitleSize(0.035/0.7, "Y");
  hDmassBase->SetTitleOffset(1.6, "Y");
  hDmassData->Draw("same");
  frame->Draw("same");
  hDmassMCIncl->Draw("same h");
  gStyle->SetOptStat(0);
  
  padBot->cd();
  hDmassRatio->Draw();
  hDmassRatio->GetYaxis()->SetNdivisions(4, 5, 0);
  hDmassRatio->SetLabelSize(0.03/0.3, "XY");
  hDmassRatio->SetTitleSize(0.035/0.3, "XY");
  hDmassRatio->SetTitleOffset(0.65, "Y");
  TLine* unity = new TLine(DMASSMIN, 1.0, DMASSMAX, 1.0);
  unity->SetLineColor(kGray);
  unity->SetLineWidth(1);
  unity->SetLineStyle(9);
  unity->Draw();
  gStyle->SetOptStat(0);
  
  canvas->cd();
  canvas->Update();
  
  TLegend* legend = new TLegend(0.20, 0.78, 0.60, 0.88);
  legend->SetFillStyle(0);
  legend->SetBorderSize(0);
  legend->SetTextSize(0.03/0.7);
  legend->AddEntry(hDmassData, "Data", "lp");
  legend->AddEntry(hDmassMCIncl, Form("MC Inclusive, %s", mcIncSource.c_str()), "lp");
  legend->AddEntry("combPDF", "Comb. background fit, Data", "l");
  legend->Draw();
  
  canvas->SaveAs(Form("plot/DmassComparison/CompareBackground_%s.png", binTag.c_str()));
  
  data->Close();
  mcincl->Close();
  return;
}

void compareSignalFit(
  string binTag,
  string plotTitle,
  string pathData,
  string pathMCFltr,
  string pathSiglmc,
  string pathSwapmc,
  string pathEvents,
  bool doubleRebin = false
) {
  TFile* mcfltr = TFile::Open(pathMCFltr.c_str(), "READ");
  TH1D* hDmass = (TH1D*) mcfltr->Get("hDmass");
  TH1D* hDmassMCFltr = (TH1D*) hDmass->Clone("hDmassMCFltr");
  hDmassMCFltr->Scale(1/hDmassMCFltr->Integral());
//  int rebinFactor = 2;
//  if (doubleRebin) rebinFactor = 2 * rebinFactor;
//  hDmassMCFltr->Scale(1/(hDmassMCFltr->Integral() * rebinFactor));
//  hDmassMCFltr->Rebin(rebinFactor);
  delete hDmass;
  
  SignalParams siglmc = SignalParams(pathSiglmc);
  SwapParams swapmc  = SwapParams(pathSwapmc);
  EventParams events = EventParams(pathEvents);
  
  RooRealVar m("Dmass", "Mass [GeV]", DMASSMIN, DMASSMAX);
  m.setRange("DMassRange", DMASSMIN, DMASSMAX);
  RooGaussian gauss1("gauss1", "first Gaussian", m, siglmc.mean, siglmc.sigma1);
  RooGaussian gauss2("gauss2", "second Gaussian", m, siglmc.mean, siglmc.sigma2);
  RooAddPdf siglPDF("siglPDF", "signal model", RooArgList(gauss1, gauss2), siglmc.frac1);
  RooGaussian swapPDF("swapPDF", "swap model", m, swapmc.mean, swapmc.sigma);
  RooAddPdf totalPDF("totalPDF", "signal+swap", RooArgList(siglPDF, swapPDF), RooArgList(events.nsig, events.nswp));
  totalPDF.fixCoefNormalization(RooArgSet(m));
  
  RooPlot *frame = m.frame();
  totalPDF.plotOn(frame, Components(siglPDF), LineWidth(2), LineStyle(1), LineColor(kRed));
  totalPDF.plotOn(frame, Components(swapPDF), LineWidth(2), LineStyle(1), LineColor(kOrange));
  totalPDF.plotOn(frame, LineWidth(2), LineStyle(1), LineColor(kBlue));
  
  TH1D* hDmassBase = new TH1D(
    "hDmassBase", plotTitle.c_str(),
    DMASSNBINS, DMASSMIN, DMASSMAX
  );
  TH1D* hRatioBase = new TH1D(
    "hRatioBase", "; Mass (GeV); MC/Fits",
    DMASSNBINS, DMASSMIN, DMASSMAX
  );
  TH1D* hRatioSigl = (TH1D*) hDmassMCFltr->Clone("hRatioSigl");
  hRatioSigl->Clear();
  TH1D* hRatioSwap = (TH1D*) hDmassMCFltr->Clone("hRatioSwap");
  hRatioSwap->Clear();
  TH1D* hRatioTotal = (TH1D*) hDmassMCFltr->Clone("hRatioTotal");
  hRatioTotal->Clear();
  
  for (int i=1; i <= hDmassMCFltr->GetNbinsX(); i++) {
    double mcVal = hDmassMCFltr->GetBinContent(i);
    double mcErr = hDmassMCFltr->GetBinError(i);
    m.setVal(hDmassMCFltr->GetBinCenter(i));
    double totalFitVal = totalPDF.getVal()/100;
    if (totalFitVal > 0) hRatioTotal->SetBinContent(i, mcVal/totalFitVal);
    else hRatioTotal->SetBinContent(i, 0);
    if (totalFitVal > 0) hRatioTotal->SetBinError(i, mcErr/totalFitVal);
    else hRatioTotal->SetBinError(i, 0);
  }
  
  hDmassMCFltr->SetMarkerColor(kBlack);
  hDmassMCFltr->SetMarkerStyle(0);
  hDmassMCFltr->SetLineColor(kBlack);
  hDmassMCFltr->SetLineWidth(2);
  hRatioSigl->SetMarkerColor(kRed);
  hRatioSigl->SetMarkerStyle(0);
  hRatioSigl->SetLineColor(kRed);
  hRatioSigl->SetLineWidth(2);
  hRatioSwap->SetMarkerColor(kOrange);
  hRatioSwap->SetMarkerStyle(0);
  hRatioSwap->SetLineColor(kOrange);
  hRatioSwap->SetLineWidth(2);
  hRatioTotal->SetMarkerColor(kBlue);
  hRatioTotal->SetMarkerStyle(0);
  hRatioTotal->SetLineColor(kBlue);
  hRatioTotal->SetLineWidth(2);
  
  TCanvas* canvas = new TCanvas("canvas", "", 600, 900);
  TPad* padTop = new TPad("padTop", "", 0.0, 0.3, 1.0, 1.0);
  TPad* padBot = new TPad("padBot", "", 0.0, 0.0, 1.0, 0.3);
  padTop->SetMargin(0.15, 0.05, 0.00, 0.16);
  padBot->SetMargin(0.15, 0.05, 0.30, 0.00);
  padTop->Draw();
  padBot->Draw();
  
  padTop->cd();
  hDmassBase->SetMinimum(0.0);
  hDmassBase->SetMaximum(0.2);
  hDmassBase->Draw();
  hDmassBase->SetLabelSize(0.03/0.7, "Y");
  hDmassBase->SetTitleSize(0.035/0.7, "Y");
  hDmassBase->SetTitleOffset(1.6, "Y");
  frame->Draw("same");
  hDmassMCFltr->Draw("same h");
  gStyle->SetOptStat(0);
  
  padBot->cd();
  hRatioBase->SetMinimum(0.8);
  hRatioBase->SetMaximum(1.2);
  hRatioBase->Draw();
  hRatioBase->GetYaxis()->SetNdivisions(4, 5, 0);
  hRatioBase->SetLabelSize(0.03/0.3, "XY");
  hRatioBase->SetTitleSize(0.035/0.3, "XY");
  hRatioBase->SetTitleOffset(0.65, "Y");
  TLine* unity = new TLine(DMASSMIN, 1.0, DMASSMAX, 1.0);
  unity->SetLineColor(kGray);
  unity->SetLineWidth(1);
  unity->SetLineStyle(9);
  unity->Draw();
  hRatioTotal->Draw("same");
//  hRatioSigl->Draw("same h");
//  hRatioSwap->Draw("same h");
  gStyle->SetOptStat(0);
  
  canvas->cd();
  canvas->Update();
  
  TLegend* legend = new TLegend(0.20, 0.78, 0.60, 0.88);
  legend->SetFillStyle(0);
  legend->SetBorderSize(0);
  legend->SetTextSize(0.03/0.7);
  legend->AddEntry(hDmassMCFltr, "MC, D^{0} filtered", "lp");
  legend->AddEntry("siglPDF", "Signal model", "l");
  legend->AddEntry("swapPDF", "Swap model", "l");
  legend->AddEntry("totalPDF", "Signal+Swap model", "l");
  legend->Draw();
  
  canvas->SaveAs(Form("plot/DmassComparison/CompareSignalGauss_%s.png", binTag.c_str()));
  
  mcfltr->Close();
  return;
}

void plotCompareDataMCmass(
){
  vector<int> gNBins = {1, 0};
  vector<int> ptBins = {2, 5};
  vector<int> yBins = {-2, -1, 0, 1, 2};
  int nGNBins = gNBins.size();
  int nPtBins = ptBins.size();
  int nYBins = yBins.size();
  
  system("mkdir -p plot/DmassComparison");
  
  for ( int ign=0; ign < nGNBins; ign++) {
    for ( int ipt=0; ipt < nPtBins-1; ipt++) {
      for ( int iy=0; iy < nYBins-1; iy++) {
        
        string binTag = Form(
          "pt%d-%d_y%d-%d_IsGammaN%d",
          ptBins[ipt], ptBins[ipt+1],
          yBins[iy], yBins[iy+1],
          gNBins[ign]
        );
        string plotTitle = Form(
          "%d < Dp_{T} < %d, %d < Dy < %d, %s; Mass (GeV); Normalized Counts",
          ptBins[ipt], ptBins[ipt+1],
          yBins[iy], yBins[iy+1],
          (gNBins[ign] == 1 ? ", #gammaN" : ", N#gamma")
        );
        
        string pathData = Form("fullAnalysis/%s/Data.root", binTag.c_str());
        string pathMCFltr = Form("fullAnalysis/%s/MC.root", binTag.c_str());
        string pathMCIncl = Form("fullAnalysis/%s/MC_inclusive.root", binTag.c_str());
        if (gNBins[ign] == 0) {
          // If data is Ngamma, get reflected ybin from gammaN
          pathMCIncl = Form(
            "fullAnalysis/pt%d-%d_y%d-%d_IsGammaN1/MC_inclusive.root",
            ptBins[ipt], ptBins[ipt+1],
            yBins[nYBins-iy-2], yBins[nYBins-iy-1]
          );
        }
        string pathCombdata = Form("fullAnalysis/%s/MassFit/combdata.dat", binTag.c_str());
        string pathSiglmc = Form("fullAnalysis/%s/MassFit/siglmc.dat", binTag.c_str());
        string pathSwapmc = Form("fullAnalysis/%s/MassFit/swap.dat", binTag.c_str());
        string pathEvents = Form("fullAnalysis/%s/MassFit/events.dat", binTag.c_str());
        
        bool doubleRebin = false;
        if ((gNBins[ign] == 1 && yBins[iy] >= 0) ||
          (gNBins[ign] == 0 && yBins[iy] <= 0)) doubleRebin = true;
  
        compareBackgroundFit(
          binTag,
          plotTitle,
          pathData,
          pathMCIncl,
          pathCombdata,
          doubleRebin
        );
        compareSignalFit(
          binTag,
          plotTitle,
          pathData,
          pathMCFltr,
          pathSiglmc,
          pathSwapmc,
          pathEvents
        );
      }
    }
  }
  return;
}
