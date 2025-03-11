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

//#include "CommandLine.h" // Yi's Commandline bundle

using namespace std;
using namespace RooFit;
#include <RooArgSet.h>
#include <RooRealVar.h>
#include <RooConstVar.h>
#include <RooFormulaVar.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include <sstream>
#include <regex>

using namespace std;

#define DMASS 1.86484
#define DMASSMIN 1.67
#define DMASSMAX 2.07
#define DMASSNBINS 32

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
    file << "name,title,value,lower_error,upper_error\n";

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
      std::string name, title, valueStr, lowerErrorStr, upperErrorStr;

      // Parse line
      if (std::getline(stream, name, ',') &&
          std::getline(stream, title, ',') &&
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
    lambda("lambda", "lambda", -4.0, -10.0, 10.0),
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
    mean("swap_mean", "[swap] mean", 1.86484, 1.85, 1.88),
    sigma("swap_sigma", "[swap] width of first Gaussian", 0.1, 0.03, 3.1)
  {
    params[mean.GetName()] = &mean;
    params[sigma.GetName()] = &sigma;
  }

  SwapParams(string dat) : SwapParams() { readFromDat(dat); }
};

struct PeakingKKParams : public ParamsBase {
  RooRealVar mean;
  RooRealVar sigma;
  RooRealVar alpha;
  RooRealVar n;

  PeakingKKParams() :
    mean("kk_mean", "[kk] mean", 1.73, 1.8),
    sigma("kk_sigma", "[kk] width of Crystal Ball", 0.01, 0.081),
    alpha("kk_alpha", "[kk] alpha of Crystal Ball", 1e-3, 2),
    n("kk_n", "[kk] n of Crystal Ball", 1e-2, 10) 
  {
    params[mean.GetName()] = &mean;
    params[sigma.GetName()] = &sigma;
    params[alpha.GetName()] = &alpha;
    params[n .GetName()] = &n;
  }

  PeakingKKParams(string dat) : PeakingKKParams() { readFromDat(dat); }
};

struct PeakingPiPiParams : public ParamsBase {
  RooRealVar mean;
  RooRealVar sigma;
  RooRealVar alpha;
  RooRealVar n;

  PeakingPiPiParams() :
    mean("pipi_mean", "[pipi] mean", 1.89, 1.99),
    sigma("pipi_sigma", "[pipi] width of Crystal Ball", 0.01, 0.08),
    alpha("pipi_alpha", "[pipi] alpha of Crystal Ball", -3, 1),
    n("pipi_n", "[pipi] n of Crystal Ball", 1e-2, 40) 
  {
    params[mean.GetName()] = &mean;
    params[sigma.GetName()] = &sigma;
    params[alpha.GetName()] = &alpha;
    params[n .GetName()] = &n;
  }

  PeakingPiPiParams(string dat) : PeakingPiPiParams() { readFromDat(dat); }
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
              double _fswp = 0.5, double _fpkkk = 0.5, double _fpkpp = 0.5)
      : nsig("nsig", "number of signal events", _nsig, 0, _nsig * 3),
        nbkg("nbkg", "number of background events", _nbkg, 0, _nbkg * 3),
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

void styleHist_MC(TH1D* mcHist)
{
  mcHist->SetMarkerColor(kRed);
  mcHist->SetMarkerStyle(24);
  mcHist->SetMarkerSize(1.);
  mcHist->SetLineColor(kRed);
  mcHist->SetLineWidth(2);
}

void styleHist_Data(TH1D* dataHist)
{
  dataHist->SetMarkerColor(kBlack);
  dataHist->SetMarkerStyle(20);
  dataHist->SetMarkerSize(1.);
  dataHist->SetLineColor(kBlack);
  dataHist->SetLineWidth(2);
}

void plot_mean(
  vector<string> rstDirs,
  string plotTitle,
  string plotLabel,
  int nYBins,
  double* yBins
)
{
  string figureTitles = plotTitle + "; y; Signal Peak Mean, #mu (GeV)";
  TH1D* meanTemplate = new TH1D("meanTemplate", figureTitles.c_str(), nYBins, yBins);
  TH1D* meanMC = new TH1D("meanMC", figureTitles.c_str(), nYBins, yBins);
  TH1D* meanData = new TH1D("meanData", figureTitles.c_str(), nYBins, yBins);
  
  for (int yBin = 1; yBin <= nYBins; yBin++) {
    string siglmcdat = Form("%s/MassFit/siglmc.dat", rstDirs[yBin-1].c_str());
    string sigldatadat = Form("%s/MassFit/sigldata.dat", rstDirs[yBin-1].c_str());
    SignalParams siglmc = SignalParams(siglmcdat);
    SignalParams sigldata = SignalParams(sigldatadat);
    double meanMCVal = siglmc.mean.getVal();
    double meanMCErr = siglmc.mean.getError();
    double meanDataVal = sigldata.mean.getVal();
    double meanDataErr = sigldata.mean.getError();
    meanMC->SetBinContent(yBin, meanMCVal);
    meanMC->SetBinError(yBin, meanMCErr);
    meanData->SetBinContent(yBin, meanDataVal);
    meanData->SetBinError(yBin, meanDataErr);
  }
  
  meanTemplate->SetMinimum(1.847);
  meanTemplate->SetMaximum(1.897);
  styleHist_MC(meanMC);
  styleHist_Data(meanData);
  
  TCanvas* canvas = new TCanvas("canvas", "", 600, 600);
  canvas->SetMargin(0.18, 0.02, 0.11, 0.09);
  meanTemplate->Draw();
  meanMC->Draw("same");
  meanData->Draw("same");
  
  TLine* DmassPDG = new TLine(yBins[0], DMASS, yBins[nYBins], DMASS);
  DmassPDG->SetLineColor(kBlue);
  DmassPDG->SetLineWidth(2);
  DmassPDG->SetLineStyle(9);
  TLine* rangeMin = new TLine(yBins[0], DMASS-0.015, yBins[nYBins], DMASS-0.015);
  rangeMin->SetLineColor(kGray);
  rangeMin->SetLineWidth(2);
  rangeMin->SetLineStyle(2);
  TLine* rangeMax = new TLine(yBins[0], DMASS+0.015, yBins[nYBins], DMASS+0.015);
  rangeMax->SetLineColor(kGray);
  rangeMax->SetLineWidth(2);
  rangeMax->SetLineStyle(2);
  DmassPDG->Draw();
  rangeMin->Draw();
  rangeMax->Draw();
  
  TLegend* legend = new TLegend(0.23, 0.65, 0.58, 0.85);
  legend->SetFillStyle(0);
  legend->SetBorderSize(0);
  legend->AddEntry(meanData, "Data #mu_{Gauss}", "lp");
  legend->AddEntry(meanMC, "MC #mu_{Gauss}", "lp");
  legend->AddEntry(DmassPDG, "PDG D^{0} mass (1.86484)", "l");
  legend->AddEntry(rangeMax, "Float bounds for #mu_{Gauss}", "l");
  legend->Draw();
  
  gStyle->SetOptStat(0);
  canvas->SaveAs(Form("plot/MassFitSignalStudy/%s_mean.pdf", plotLabel.c_str()));
}

void plot_sigma1(
  vector<string> rstDirs,
  string plotTitle,
  string plotLabel,
  int nYBins,
  double* yBins
)
{
  string figureTitles = plotTitle + "; y; Signal Gaussian 1 Width, #sigma_{1} (GeV)";
  TH1D* sigma1Template = new TH1D("sigma1Template", figureTitles.c_str(), nYBins, yBins);
  TH1D* sigma1MC = new TH1D("sigma1MC", figureTitles.c_str(), nYBins, yBins);
  TH1D* sigma1Data = new TH1D("sigma1Data", figureTitles.c_str(), nYBins, yBins);
  
  for (int yBin = 1; yBin <= nYBins; yBin++) {
    string siglmcdat = Form("%s/MassFit/siglmc.dat", rstDirs[yBin-1].c_str());
    string sigldatadat = Form("%s/MassFit/sigldata.dat", rstDirs[yBin-1].c_str());
    SignalParams siglmc = SignalParams(siglmcdat);
    SignalParams sigldata = SignalParams(sigldatadat);
    double sigma1MCVal = siglmc.sigma1.getVal();
    double sigma1MCErr = siglmc.sigma1.getError();
    double sigma1DataVal = sigldata.sigma1.getVal()
      * (1 + sigldata.alpha.getVal());
    double sigma1DataErr = sigldata.sigma1.getError()
      * (1 + sigldata.alpha.getVal() + sigldata.alpha.getError());
    sigma1MC->SetBinContent(yBin, sigma1MCVal);
    sigma1MC->SetBinError(yBin, sigma1MCErr);
    sigma1Data->SetBinContent(yBin, sigma1DataVal);
    sigma1Data->SetBinError(yBin, sigma1DataErr);
  }
  
  sigma1Template->SetMinimum(0.004);
  sigma1Template->SetMaximum(0.16);
  styleHist_MC(sigma1MC);
  styleHist_Data(sigma1Data);
  
  TCanvas* canvas = new TCanvas("canvas", "", 600, 600);
  canvas->SetMargin(0.18, 0.02, 0.11, 0.09);
  sigma1Template->Draw();
  sigma1MC->Draw("same");
  sigma1Data->Draw("same");
  
  TLine* rangeMin = new TLine(yBins[0], 0.0048, yBins[nYBins], 0.0048);
  rangeMin->SetLineColor(kGray);
  rangeMin->SetLineWidth(2);
  rangeMin->SetLineStyle(2);
  TLine* rangeMax = new TLine(yBins[0], 0.155, yBins[nYBins], 0.155);
  rangeMax->SetLineColor(kGray);
  rangeMax->SetLineWidth(2);
  rangeMax->SetLineStyle(2);
  rangeMin->Draw();
  rangeMax->Draw();
  
  TLegend* legend = new TLegend(0.23, 0.7, 0.58, 0.85);
  legend->SetFillStyle(0);
  legend->SetBorderSize(0);
  legend->AddEntry(sigma1Data, "Data, width = (1 + #alpha) #times #sigma_{1}", "lp");
  legend->AddEntry(sigma1MC, "MC, width = #sigma_{1}", "lp");
  legend->AddEntry(rangeMax, "Float bounds for #sigma_{1}", "l");
  legend->Draw();
  
  gStyle->SetOptStat(0);
  canvas->SaveAs(Form("plot/MassFitSignalStudy/%s_sigma1.pdf", plotLabel.c_str()));
}

void plot_sigma2(
  vector<string> rstDirs,
  string plotTitle,
  string plotLabel,
  int nYBins,
  double* yBins
)
{
  string figureTitles = plotTitle + "; y; Signal Gaussian 1 Width, #sigma_{1} (GeV)";
  TH1D* sigma2Template = new TH1D("sigma2Template", figureTitles.c_str(), nYBins, yBins);
  TH1D* sigma2MC = new TH1D("sigma2MC", figureTitles.c_str(), nYBins, yBins);
  TH1D* sigma2Data = new TH1D("sigma2Data", figureTitles.c_str(), nYBins, yBins);
  
  for (int yBin = 1; yBin <= nYBins; yBin++) {
    string siglmcdat = Form("%s/MassFit/siglmc.dat", rstDirs[yBin-1].c_str());
    string sigldatadat = Form("%s/MassFit/sigldata.dat", rstDirs[yBin-1].c_str());
    SignalParams siglmc = SignalParams(siglmcdat);
    SignalParams sigldata = SignalParams(sigldatadat);
    double sigma2MCVal = siglmc.sigma2.getVal();
    double sigma2MCErr = siglmc.sigma2.getError();
    double sigma2DataVal = sigldata.sigma2.getVal()
      * (1 + sigldata.alpha.getVal());
    double sigma2DataErr = sigldata.sigma2.getError()
      * (1 + sigldata.alpha.getVal() + sigldata.alpha.getError());
    sigma2MC->SetBinContent(yBin, sigma2MCVal);
    sigma2MC->SetBinError(yBin, sigma2MCErr);
    sigma2Data->SetBinContent(yBin, sigma2DataVal);
    sigma2Data->SetBinError(yBin, sigma2DataErr);
  }
  
  sigma2Template->SetMinimum(0.004);
  sigma2Template->SetMaximum(0.05);
  styleHist_MC(sigma2MC);
  styleHist_Data(sigma2Data);
  
  TCanvas* canvas = new TCanvas("canvas", "", 600, 600);
  canvas->SetMargin(0.18, 0.02, 0.11, 0.09);
  sigma2Template->Draw();
  sigma2MC->Draw("same");
  sigma2Data->Draw("same");
  
  TLine* rangeMin = new TLine(yBins[0], 0.0048, yBins[nYBins], 0.0048);
  rangeMin->SetLineColor(kGray);
  rangeMin->SetLineWidth(2);
  rangeMin->SetLineStyle(2);
  TLine* rangeMax = new TLine(yBins[0], 0.0465, yBins[nYBins], 0.0465);
  rangeMax->SetLineColor(kGray);
  rangeMax->SetLineWidth(2);
  rangeMax->SetLineStyle(2);
  rangeMin->Draw();
  rangeMax->Draw();
  
  TLegend* legend = new TLegend(0.23, 0.7, 0.58, 0.85);
  legend->SetFillStyle(0);
  legend->SetBorderSize(0);
  legend->AddEntry(sigma2Data, "Data, width = (1 + #alpha) #times #sigma_{2}", "lp");
  legend->AddEntry(sigma2MC, "MC, width = #sigma_{2}", "lp");
  legend->AddEntry(rangeMax, "Float bounds for #sigma_{2}", "l");
  legend->Draw();
  
  gStyle->SetOptStat(0);
  canvas->SaveAs(Form("plot/MassFitSignalStudy/%s_sigma2.pdf", plotLabel.c_str()));
}

void plot_alpha(
  vector<string> rstDirs,
  string plotTitle,
  string plotLabel,
  int nYBins,
  double* yBins
)
{
  string figureTitles = plotTitle + "; y; Signal Alpha (GeV), from: #sigma #times (1 + #alpha)";
  TH1D* alphaTemplate = new TH1D("alphaTemplate", figureTitles.c_str(), nYBins, yBins);
  TH1D* alphaData = new TH1D("alphaData", figureTitles.c_str(), nYBins, yBins);
  
  for (int yBin = 1; yBin <= nYBins; yBin++) {
    string sigldatadat = Form("%s/MassFit/sigldata.dat", rstDirs[yBin-1].c_str());
    SignalParams sigldata = SignalParams(sigldatadat);
    double alphaDataVal = sigldata.alpha.getVal();
    double alphaDataErr = sigldata.alpha.getError();
    alphaData->SetBinContent(yBin, alphaDataVal);
    alphaData->SetBinError(yBin, alphaDataErr);
  }
  
  alphaTemplate->SetMinimum(-0.50);
  alphaTemplate->SetMaximum(0.50);
  styleHist_Data(alphaData);
  
  TCanvas* canvas = new TCanvas("canvas", "", 600, 600);
  canvas->SetMargin(0.18, 0.02, 0.11, 0.09);
  alphaTemplate->Draw();
  alphaData->Draw("same");
  
  TLine* rangeMin = new TLine(yBins[0], -0.25, yBins[nYBins], -0.25);
  rangeMin->SetLineColor(kGray);
  rangeMin->SetLineWidth(2);
  rangeMin->SetLineStyle(2);
  TLine* rangeMax = new TLine(yBins[0], 0.25, yBins[nYBins], 0.25);
  rangeMax->SetLineColor(kGray);
  rangeMax->SetLineWidth(2);
  rangeMax->SetLineStyle(2);
  rangeMin->Draw();
  rangeMax->Draw();
  
  TLegend* legend = new TLegend(0.23, 0.75, 0.58, 0.85);
  legend->SetFillStyle(0);
  legend->SetBorderSize(0);
  legend->AddEntry(alphaData, "Data, (1 + #alpha) #times #sigma_{Gauss}", "lp");
  legend->AddEntry(rangeMax, "Float bounds for #alpha", "l");
  legend->Draw();
  
  gStyle->SetOptStat(0);
  canvas->SaveAs(Form("plot/MassFitSignalStudy/%s_alpha.pdf", plotLabel.c_str()));
}

void plot_extractedSignal(
  vector<string> rstDirs,
  string plotTitle,
  string plotLabel,
  int nYBins,
  double* yBins,
  double ptMin,
  double ptMax,
  int isGammaN
) {
  vector<string> input_nominal;
  vector<string> input_fitMean;
  vector<string> input_fitAlpha;
  vector<string> input_fitComb;
  vector<string> input_fitPkbg;
  
  for (const string& rstDir : rstDirs) {
    input_nominal.push_back(Form("%s/MassFit/correctedYields.md", rstDir.c_str()));
    input_fitMean.push_back(Form("%s/MassFit_systFitSigMean/correctedYields.md", rstDir.c_str()));
    input_fitAlpha.push_back(Form("%s/MassFit_systFitSigAlpha/correctedYields.md", rstDir.c_str()));
    input_fitComb.push_back(Form("%s/MassFit_systComb/correctedYields.md", rstDir.c_str()));
    input_fitPkbg.push_back(Form("%s/MassFit_systPkBg/correctedYields.md", rstDir.c_str()));
  }
  
  vector<Point> points_nominal  = getPointArr(ptMin, ptMax, isGammaN, input_nominal);
  vector<Point> points_fitMean  = getPointArr(ptMin, ptMax, isGammaN, input_fitMean);
  vector<Point> points_fitAlpha = getPointArr(ptMin, ptMax, isGammaN, input_fitAlpha);
  vector<Point> points_fitComb  = getPointArr(ptMin, ptMax, isGammaN, input_fitComb);
  vector<Point> points_fitPkbg  = getPointArr(ptMin, ptMax, isGammaN, input_fitPkbg);
  
  vector<double> corrYieldVal_nominal  = getDoubleArr(points_nominal, [](Point& p) -> double { return p.correctedYield;} );
  vector<double> corrYieldErr_nominal  = getDoubleArr(points_nominal, [](Point& p) -> double { return p.correctedYieldError;} );
  vector<double> corrYieldVal_fitMean  = getDoubleArr(points_fitMean, [](Point& p) -> double { return p.correctedYield;} );
  vector<double> corrYieldErr_fitMean  = getDoubleArr(points_fitMean, [](Point& p) -> double { return p.correctedYieldError;} );
  vector<double> corrYieldVal_fitAlpha = getDoubleArr(points_fitAlpha, [](Point& p) -> double { return p.correctedYield;} );
  vector<double> corrYieldErr_fitAlpha = getDoubleArr(points_fitAlpha, [](Point& p) -> double { return p.correctedYieldError;} );
  vector<double> corrYieldVal_fitComb  = getDoubleArr(points_fitComb, [](Point& p) -> double { return p.correctedYield;} );
  vector<double> corrYieldErr_fitComb  = getDoubleArr(points_fitComb, [](Point& p) -> double { return p.correctedYieldError;} );
  vector<double> corrYieldVal_fitPkbg  = getDoubleArr(points_fitPkbg, [](Point& p) -> double { return p.correctedYield;} );
  vector<double> corrYieldErr_fitPkbg  = getDoubleArr(points_fitPkbg, [](Point& p) -> double { return p.correctedYieldError;} );

  string figureTitles = plotTitle + "; y; Corrected Yield";
  TH1D* extrSiglTemplate  = new TH1D("extrSiglTemplate", figureTitles.c_str(), nYBins, yBins);
  TH1D* extrSigl_nominal  = new TH1D("extrSigl_nominal", figureTitles.c_str(), nYBins, yBins);
  TH1D* extrSigl_fitMean  = new TH1D("extrSigl_fitMean", figureTitles.c_str(), nYBins, yBins);
  TH1D* extrSigl_fitAlpha = new TH1D("extrSigl_fitAlpha", figureTitles.c_str(), nYBins, yBins);
  TH1D* extrSigl_fitComb  = new TH1D("extrSigl_fitComb", figureTitles.c_str(), nYBins, yBins);
  TH1D* extrSigl_fitPkbg  = new TH1D("extrSigl_fitPkbg", figureTitles.c_str(), nYBins, yBins);
  
  for (int yBin = 1; yBin <= nYBins; yBin++) {
    extrSigl_nominal->SetBinContent(yBin, corrYieldVal_nominal[yBin-1]);
    extrSigl_nominal->SetBinError(yBin, corrYieldErr_nominal[yBin-1]);
    extrSigl_fitMean->SetBinContent(yBin, corrYieldVal_fitMean[yBin-1]);
    extrSigl_fitMean->SetBinError(yBin, corrYieldErr_fitMean[yBin-1]);
    extrSigl_fitAlpha->SetBinContent(yBin, corrYieldVal_fitAlpha[yBin-1]);
    extrSigl_fitAlpha->SetBinError(yBin, corrYieldErr_fitAlpha[yBin-1]);
    extrSigl_fitComb->SetBinContent(yBin, corrYieldVal_fitComb[yBin-1]);
    extrSigl_fitComb->SetBinError(yBin, corrYieldErr_fitComb[yBin-1]);
    extrSigl_fitPkbg->SetBinContent(yBin, corrYieldVal_fitPkbg[yBin-1]);
    extrSigl_fitPkbg->SetBinError(yBin, corrYieldErr_fitPkbg[yBin-1]);
  }
  extrSiglTemplate->SetMinimum(0.0);
  extrSiglTemplate->SetMaximum(3.5);
  extrSigl_nominal->Sumw2();
  extrSigl_fitMean->Sumw2();
  extrSigl_fitAlpha->Sumw2();
  extrSigl_fitComb->Sumw2();
  extrSigl_fitPkbg->Sumw2();
  
  // Nominal
  extrSigl_nominal->SetMarkerColor(kBlack);
  extrSigl_nominal->SetMarkerStyle(20);
  extrSigl_nominal->SetMarkerSize(1.0);
  extrSigl_nominal->SetLineColor(kBlack);
  extrSigl_nominal->SetLineWidth(2);
  // SystFitMean
  extrSigl_fitMean->SetMarkerColor(kRed);
  extrSigl_fitMean->SetMarkerStyle(33);
  extrSigl_fitMean->SetMarkerSize(1.5);
  extrSigl_fitMean->SetLineColor(kRed);
  extrSigl_fitMean->SetLineWidth(2);
  // SystFitAlpha
  extrSigl_fitAlpha->SetMarkerColor(kOrange);
  extrSigl_fitAlpha->SetMarkerStyle(43);
  extrSigl_fitAlpha->SetMarkerSize(1.4);
  extrSigl_fitAlpha->SetLineColor(kOrange);
  extrSigl_fitAlpha->SetLineWidth(2);
  // SystFitComb
  extrSigl_fitComb->SetMarkerColor(kGreen);
  extrSigl_fitComb->SetMarkerStyle(21);
  extrSigl_fitComb->SetMarkerSize(0.9);
  extrSigl_fitComb->SetLineColor(kGreen);
  extrSigl_fitComb->SetLineWidth(2);
  // SystFitPkbg
  extrSigl_fitPkbg->SetMarkerColor(kViolet);
  extrSigl_fitPkbg->SetMarkerStyle(22);
  extrSigl_fitPkbg->SetMarkerSize(1.1);
  extrSigl_fitPkbg->SetLineColor(kViolet);
  extrSigl_fitPkbg->SetLineWidth(2);
  
  TH1D* ratio_fitMean  = (TH1D*) extrSigl_fitMean->Clone("ratio_fitMean");
  ratio_fitMean->Divide(extrSigl_nominal);
  TH1D* ratio_fitAlpha = (TH1D*) extrSigl_fitAlpha->Clone("ratio_fitMean");
  ratio_fitAlpha->Divide(extrSigl_nominal);
  TH1D* ratio_fitComb  = (TH1D*) extrSigl_fitComb->Clone("ratio_fitComb");
  ratio_fitComb->Divide(extrSigl_nominal);
  TH1D* ratio_fitPkbg  = (TH1D*) extrSigl_fitPkbg->Clone("ratio_fitPkbg");
  ratio_fitPkbg->Divide(extrSigl_nominal);
  TH1D* ratioTemplate  = (TH1D*) extrSiglTemplate->Clone("ratioTemplate");
  ratioTemplate->SetTitle("; y; Alt. / Nominal");
  ratioTemplate->SetMinimum(0.7);
  ratioTemplate->SetMaximum(1.3);
  
  TCanvas* canvas = new TCanvas("canvas", "", 600, 600);
  TPad* padTop = new TPad("padTop", "", 0.0, 0.3, 1.0, 1.0);
  TPad* padBot = new TPad("padBot", "", 0.0, 0.0, 1.0, 0.3);
  padTop->SetMargin(0.18, 0.02, 0.00, 0.16);
  padBot->SetMargin(0.18, 0.02, 0.30, 0.00);
  padTop->Draw();
  padBot->Draw();
  
  padTop->cd();
  extrSiglTemplate->Draw();
  extrSiglTemplate->SetLabelSize(0.03/0.7, "Y");
  extrSiglTemplate->SetTitleSize(0.035/0.7, "Y");
  extrSigl_nominal->Draw("same");
  extrSigl_fitComb->Draw("same");
  extrSigl_fitPkbg->Draw("same");
  extrSigl_fitAlpha->Draw("same");
  extrSigl_fitMean->Draw("same");
  gStyle->SetOptStat(0);
  
  padBot->cd();
  ratioTemplate->Draw();
  ratioTemplate->SetLabelSize(0.03/0.3, "XY");
  ratioTemplate->SetTitleSize(0.035/0.3, "XY");
  ratioTemplate->SetTitleOffset(0.4, "Y");
  TLine* unity = new TLine(yBins[0], 1.0, yBins[nYBins], 1.0);
  unity->SetLineColor(kGray);
  unity->SetLineWidth(1);
  unity->SetLineStyle(9);
  unity->Draw();
  ratio_fitComb->Draw("same");
  ratio_fitPkbg->Draw("same");
  ratio_fitAlpha->Draw("same");
  ratio_fitMean->Draw("same");
  gStyle->SetOptStat(0);
  
  canvas->cd();
  canvas->Update();
  float legShift = 0.;
  if (isGammaN) legShift = 0.3;
  TLegend* legend = new TLegend(0.2 + legShift, 0.60, 0.6 + legShift, 0.85);
  legend->SetFillStyle(0);
  legend->SetBorderSize(0);
  legend->AddEntry(extrSigl_nominal,  "Nominal Corr. Yield", "lp");
  legend->AddEntry(extrSigl_fitMean,  "Fit Syst: Signal mean", "lp");
  legend->AddEntry(extrSigl_fitAlpha, "Fit Syst: Signal alpha", "lp");
  legend->AddEntry(extrSigl_fitComb,  "Fit Syst: Comb. Background", "lp");
  legend->AddEntry(extrSigl_fitPkbg,  "Fit Syst: KK + #pi#pi Peaks", "lp");
  legend->Draw();
  
  canvas->SaveAs(Form("plot/MassFitSignalStudy/%s_extractedSignal_ratio.pdf", plotLabel.c_str()));
  
  TH1D* diff_fitMean  = (TH1D*) extrSigl_fitMean->Clone("diff_fitMean");
  diff_fitMean->Add(extrSigl_nominal, -1);
  diff_fitMean->Divide(extrSigl_nominal);
  TH1D* diff_fitAlpha = (TH1D*) extrSigl_fitAlpha->Clone("diff_fitAlpha");
  diff_fitAlpha->Add(extrSigl_nominal, -1);
  diff_fitAlpha->Divide(extrSigl_nominal);
  TH1D* diff_fitComb  = (TH1D*) extrSigl_fitComb->Clone("diff_fitComb");
  diff_fitComb->Add(extrSigl_nominal, -1);
  diff_fitComb->Divide(extrSigl_nominal);
  TH1D* diff_fitPkbg  = (TH1D*) extrSigl_fitPkbg->Clone("diff_fitPkbg");
  diff_fitPkbg->Add(extrSigl_nominal, -1);
  diff_fitPkbg->Divide(extrSigl_nominal);
  TH1D* diffTemplate  = (TH1D*) extrSiglTemplate->Clone("diffTemplate");
  diffTemplate->SetTitle("; y; (Alt. - Nom.) / Nom.");
  diffTemplate->SetMinimum(-0.3);
  diffTemplate->SetMaximum(0.3);
  
  padBot->cd();
  padBot->Clear();
  diffTemplate->Draw();
  diffTemplate->SetLabelSize(0.03/0.3, "XY");
  diffTemplate->SetTitleSize(0.035/0.3, "XY");
  diffTemplate->SetTitleOffset(0.4, "Y");
  diff_fitComb->Draw("same");
  diff_fitPkbg->Draw("same");
  diff_fitAlpha->Draw("same");
  diff_fitMean->Draw("same");
  gStyle->SetOptStat(0);
  
  canvas->SaveAs(Form("plot/MassFitSignalStudy/%s_extractedSignal_difference.pdf", plotLabel.c_str()));
}

void plotMassfitSignalStudy()
{
  int isGammaN[2] = {1, 0};
  double ptBins[2] = {2, 5};
  double yBins[5]  = {-2, -1, 0, 1, 2};
  int nPtBins = *(&ptBins + 1) - ptBins - 1;
  int nYBins = *(&yBins + 1) - yBins - 1;
  
  system("mkdir -p plot/MassFitSignalStudy/");
  
  // Iterate through gammaN options
  for (const int& gammaN : isGammaN) {
    // Iterate through pt bins
    for (int ipt = 0; ipt < nPtBins; ipt++) {
      double ptBinLo = ptBins[ipt];
      double ptBinHi = ptBins[ipt+1];
      // Iterate through y bins to make list of source directories
      vector<string> rstDirs;
      for (int iy = 0; iy < nYBins; iy++) {
        double yBinLo = yBins[iy];
        double yBinHi = yBins[iy+1];
        string dirName = Form(
          "fullAnalysis/pt%.0f-%.0f_y%.0f-%.0f_IsGammaN%d/",
          ptBinLo, ptBinHi, yBinLo, yBinHi, gammaN
        );
        rstDirs.push_back(dirName);
      }
      string plotLabel = Form("pt%.0f-%.0f_IsGammaN%d", ptBinLo, ptBinHi, gammaN);
      string plotTitle = Form("%.1f #leq D_{p_{T}} < %.1f (GeV), %s",
        ptBins[ipt], ptBins[ipt+1], (gammaN == 1 ? "#gammaN" : "N#gamma")
      );
      plot_mean(rstDirs, plotTitle, plotLabel, nYBins, yBins);
      plot_sigma1(rstDirs, plotTitle, plotLabel, nYBins, yBins);
      plot_sigma2(rstDirs, plotTitle, plotLabel, nYBins, yBins);
      plot_alpha(rstDirs, plotTitle, plotLabel, nYBins, yBins);
      plot_extractedSignal(rstDirs, plotTitle, plotLabel, nYBins, yBins, ptBinLo, ptBinHi, gammaN);
    }
  }
}
