#include <TTree.h>
#include <TFile.h>
#include <TDirectoryFile.h>
#include <TChain.h>
#include <TH1D.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TLatex.h>

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

#include "CommandLine.h" // Yi's Commandline bundle


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

#define DMASSMIN 1.68
#define DMASSMAX 2.05
#define DMASSNBINS 74

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

  SignalParams() :
    mean("sig_mean", "[signal] mean", 1.86484, 1.85, 1.88),
    sigma1("sig_sigma1", "[signal] width of first Gaussian", 0.03, 0.0048, 0.155),
    sigma2("sig_sigma2", "[signal] width of second Gaussian", 0.01, 0.0048, 0.0465),
    frac1("sig_frac1", "[signal] fraction of first Gaussian", 0.1, 0.001, 0.5) 
  {
    // cout << "signal default" << endl;
    params[mean.GetName()] = &mean;
    params[sigma1.GetName()] = &sigma1;
    params[sigma2.GetName()] = &sigma2;
    params[frac1 .GetName()] = &frac1;
  }

  SignalParams(string dat) : SignalParams() { readFromDat(dat); }
  SignalParams(string dat, bool doSyst) : SignalParams()
  {
    readFromDat(dat);
    if (!doSyst)
    {
      mean.setConstant(false); // Nominal fit strategy is to let the mean value float
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

void sigswpmc_fit(TTree *mctree, string rstDir,
                string& sigldat, string& swapdat,
                string plotTitle)
{
  std::cout << "=======================================================" << std::endl;
  std::cout << "=    Doing signal prefit ......" << std::endl;
  std::cout << "=======================================================" << std::endl;

  TCanvas* canvas = new TCanvas("canvas", "Fit Plot", 800, 600);
  // Define the mass range and variables
  RooRealVar m("Dmass", "Mass [GeV]", DMASSMIN, DMASSMAX);
  RooRealVar Dgen("Dgen", "Dgen", 0, 30000);

  // Import data
  RooDataSet _data("_data", "dataset", RooArgSet(m, Dgen), Import(*mctree));
  RooDataSet& data = *( (RooDataSet*) _data.reduce( "(Dgen == 23333 || Dgen == 41022 || Dgen == 41044) || "
                                                    "(Dgen == 23344 || Dgen == 41122 || Dgen == 41144)") );

  std::cout << "[Info] Number of entries: " << data.sumEntries() << std::endl;

  SignalParams sigl;
  SwapParams swap;

  // Define the signal model: double Gaussian
  RooGaussian gauss1("gauss1", "first Gaussian", m, sigl.mean, sigl.sigma1);
  RooGaussian gauss2("gauss2", "second Gaussian", m, sigl.mean, sigl.sigma2);
  RooAddPdf siglPDF("signal", "signal model", RooArgList(gauss1, gauss2), sigl.frac1);

  // Define the swap model: single Gaussian
  RooGaussian swapPDF("swap", "swap model", m, swap.mean, swap.sigma);

  // Define the combined model
  RooRealVar nsig("nsig", "number of signal events", data.sumEntries()*0.5, 0, data.sumEntries());
  RooRealVar nswp("nswp", "number of swap events", data.sumEntries()*0.5, 0, data.sumEntries());
  RooAddPdf model("model", "signal + swap", RooArgList(siglPDF, swapPDF), RooArgList(nsig, nswp));

  // Fit the model to data
  RooFitResult* result = model.fitTo(data, Save());

  // Plot the data and the fit result
  RooPlot* frame = m.frame(DMASSNBINS);
  frame->SetTitle(plotTitle.c_str());
  data.plotOn(frame);
  model.plotOn(frame);
  model.plotOn(frame, Components(siglPDF), LineStyle(kSolid), LineColor(kRed));
  model.plotOn(frame, Components(gauss1), LineStyle(2), LineWidth(2), LineColor(kRed));
  model.plotOn(frame, Components(gauss2), LineStyle(3), LineWidth(2), LineColor(kRed));
  model.plotOn(frame, Components(swapPDF), LineStyle(kSolid), LineColor(kOrange+1));
  frame->Draw();

  // Add fitted values and errors using TLatex
  TLatex latex;
  latex.SetNDC(); // Use normalized coordinates (0 to 1)
  latex.SetTextSize(0.03);
  latex.SetTextAlign(13); // Align at top-left corner

  double xpos = 0.60, ypos = 0.85, ypos_step = 0.05; // Starting position and step for annotations

  // Draw each parameter value explicitly using ypos - n*ypos_step
  latex.DrawLatex(xpos, ypos - 0 * ypos_step, Form("Mean (Signal): %.3f #pm %.3f", sigl.mean.getVal(), sigl.mean.getError()));
  latex.DrawLatex(xpos, ypos - 1 * ypos_step, Form("Sigma1 (Signal): %.3f #pm %.3f", sigl.sigma1.getVal(), sigl.sigma1.getError()));
  latex.DrawLatex(xpos, ypos - 2 * ypos_step, Form("Sigma2 (Signal): %.3f #pm %.3f", sigl.sigma2.getVal(), sigl.sigma2.getError()));
  latex.DrawLatex(xpos, ypos - 3 * ypos_step, Form("Frac1 (Signal): %.3f #pm %.3f", sigl.frac1.getVal(), sigl.frac1.getError()));
  latex.DrawLatex(xpos, ypos - 4 * ypos_step, Form("Mean (Swap): %.3f #pm %.3f", swap.mean.getVal(), swap.mean.getError()));
  latex.DrawLatex(xpos, ypos - 5 * ypos_step, Form("Sigma (Swap): %.3f #pm %.3f", swap.sigma.getVal(), swap.sigma.getError()));
  latex.DrawLatex(xpos, ypos - 6 * ypos_step, Form("N_{Sig}: %.3f #pm %.3f", nsig.getVal(), nsig.getError()));
  latex.DrawLatex(xpos, ypos - 7 * ypos_step, Form("N_{Swap}: %.3f #pm %.3f", nswp.getVal(), nswp.getError()));

  canvas->SaveAs(Form("%s/fit_result_signal_and_swap.pdf", rstDir.c_str()));
  
  sigl.print();
  swap.print();

  sigldat=Form("%s/sigl.dat", rstDir.c_str());
  swapdat=Form("%s/swap.dat", rstDir.c_str());
  sigl.writeToDat(sigldat.c_str());
  swap.writeToDat(swapdat.c_str());

  delete canvas;
}

void kkmc_fit(TTree *mctree, string rstDir,
              string& pkkkdat,
              string plotTitle)
{
  std::cout << "=======================================================" << std::endl;
  std::cout << "=    Doing kk prefit ......" << std::endl;
  std::cout << "=======================================================" << std::endl;

  TCanvas* canvas = new TCanvas("canvas", "Fit Plot", 800, 600);
  // Define the mass range and variables
  RooRealVar m("Dmass", "Mass [GeV]", DMASSMIN, DMASSMAX);
  RooRealVar Dgen("Dgen", "Dgen", 0, 30000);

  // Import data
  RooDataSet _data("_data", "dataset", RooArgSet(m, Dgen), Import(*mctree));
  RooDataSet& data = *( (RooDataSet*) _data.reduce( "Dgen == 333 && Dmass < 1.8648" ) );

  std::cout << "[Info] Number of entries: " << data.sumEntries() << std::endl;

  PeakingKKParams pkkk;

  // Define the KK model: Crystal Ball
  RooCBShape pkkkPDF("peaking_kk", "peaking background KK state", m, pkkk.mean, pkkk.sigma, pkkk.alpha, pkkk.n );
  // Define the combined model
  RooRealVar npkkk("npkkk", "number of signal events", data.sumEntries(), 0, data.sumEntries()*1.1);
  RooAddPdf model("model", "kk", RooArgList(pkkkPDF), RooArgList(npkkk));

  // Fit the model to data
  RooFitResult* result = model.fitTo(data, Save());

  // Plot the data and the fit result
  RooPlot* frame = m.frame(DMASSNBINS);
  frame->SetTitle(plotTitle.c_str());
  data.plotOn(frame);
  model.plotOn(frame);
  frame->Draw();

  // Add fitted parameter values and errors using TLatex
  double xpos = 0.60, ypos = 0.85, ypos_step = 0.05; // Starting position and step for annotations

  TLatex latex;
  latex.SetTextSize(0.03);
  latex.SetNDC();

  latex.DrawLatex(xpos, ypos - 0 * ypos_step, Form("#mu = %.3f #pm %.3f", pkkk.mean.getVal(), pkkk.mean.getError()));
  latex.DrawLatex(xpos, ypos - 1 * ypos_step, Form("#sigma = %.3f #pm %.3f", pkkk.sigma.getVal(), pkkk.sigma.getError()));
  latex.DrawLatex(xpos, ypos - 2 * ypos_step, Form("#alpha = %.3f #pm %.3f", pkkk.alpha.getVal(), pkkk.alpha.getError()));
  latex.DrawLatex(xpos, ypos - 3 * ypos_step, Form("n = %.3f #pm %.3f", pkkk.n.getVal(), pkkk.n.getError()));
  latex.DrawLatex(xpos, ypos - 4 * ypos_step, Form("N_{KK} = %.3f #pm %.3f", npkkk.getVal(), npkkk.getError()));

  canvas->SaveAs(Form("%s/fit_result_peaking_kk.pdf", rstDir.c_str()));
  
  pkkk.print();

  pkkkdat=Form("%s/pkkk.dat", rstDir.c_str());
  pkkk.writeToDat(pkkkdat.c_str());

  delete canvas;
}

void pipimc_fit(TTree *mctree, string rstDir,
                string& pkppdat,
                string plotTitle)
{
  std::cout << "=======================================================" << std::endl;
  std::cout << "=    Doing pipi prefit ......" << std::endl;
  std::cout << "=======================================================" << std::endl;

  TCanvas* canvas = new TCanvas("canvas", "Fit Plot", 800, 600);
  // Define the mass range and variables
  RooRealVar m("Dmass", "Mass [GeV]", DMASSMIN, DMASSMAX);
  RooRealVar Dgen("Dgen", "Dgen", 0, 30000);

  // Import data
  RooDataSet _data("_data", "dataset", RooArgSet(m, Dgen), Import(*mctree));
  RooDataSet& data = *( (RooDataSet*) _data.reduce( "Dgen == 333 && Dmass > 1.8648" ) );

  PeakingPiPiParams pkpp;

  // Define the pipi model: Crystal Ball
  RooCBShape pkppPDF("peaking_pipi", "peaking background pipi state", m, pkpp.mean, pkpp.sigma, pkpp.alpha, pkpp.n );
  // Define the combined model
  RooRealVar npkpp("npkpp", "number of signal events", data.sumEntries(), 0, data.sumEntries()*1.1);
  RooAddPdf model("model", "pipi", RooArgList(pkppPDF), RooArgList(npkpp));

  std::cout << "[Info] Number of entries: " << data.sumEntries() << std::endl;

  // Fit the model to data
  RooFitResult* result = model.fitTo(data, Save());

  // Plot the data and the fit result
  RooPlot* frame = m.frame(DMASSNBINS);
  frame->SetTitle(plotTitle.c_str());
  data.plotOn(frame);
  model.plotOn(frame);
  frame->Draw();

  // Add fitted parameter values and errors using TLatex
  double xpos = 0.15, ypos = 0.85, ypos_step = 0.05; // Starting position and step for annotations

  TLatex latex;
  latex.SetTextSize(0.03);
  latex.SetNDC();

  latex.DrawLatex(xpos, ypos - 0 * ypos_step, Form("#mu = %.3f #pm %.3f", pkpp.mean.getVal(), pkpp.mean.getError()));
  latex.DrawLatex(xpos, ypos - 1 * ypos_step, Form("#sigma = %.3f #pm %.3f", pkpp.sigma.getVal(), pkpp.sigma.getError()));
  latex.DrawLatex(xpos, ypos - 2 * ypos_step, Form("#alpha = %.3f #pm %.3f", pkpp.alpha.getVal(), pkpp.alpha.getError()));
  latex.DrawLatex(xpos, ypos - 3 * ypos_step, Form("n = %.3f #pm %.3f", pkpp.n.getVal(), pkpp.n.getError()));
  latex.DrawLatex(xpos, ypos - 4 * ypos_step, Form("N_{#pi#pi} = %.3f #pm %.3f", npkpp.getVal(), npkpp.getError()));

  canvas->SaveAs(Form("%s/fit_result_peaking_pipi.pdf", rstDir.c_str()));
  
  pkpp.print();

  pkppdat=Form("%s/pkpp.dat", rstDir.c_str());
  pkpp.writeToDat(pkppdat.c_str());

  delete canvas;
}

void main_fit(TTree *datatree, string rstDir, string output,
              string sigldat, string swapdat,
              string pkkkdat, string pkppdat,
              string eventsdat,
              bool doSyst_sig, bool doSyst_comb,
              bool doPkkk, bool doPkpp,
              string plotTitle)
{
  std::cout << "=======================================================" << std::endl;
  std::cout << "=    Doing main fit ......" << std::endl;
  std::cout << "=======================================================" << std::endl;

  TCanvas* canvas = new TCanvas("canvas", "Fit Plot", 800, 600);
  // Define the mass range and variables
  RooRealVar m("Dmass", "Mass [GeV]", DMASSMIN, DMASSMAX);

  // Import data
  RooDataSet data("data", "dataset", RooArgSet(m), Import(*datatree));

  std::cout << "[Info] Number of entries: " << data.sumEntries() << std::endl;

  SignalParams sigl = SignalParams(sigldat, doSyst_sig);
  SwapParams swap = SwapParams(swapdat);
  PeakingKKParams pkkk = PeakingKKParams(pkkkdat);
  PeakingPiPiParams pkpp = PeakingPiPiParams(pkppdat);
  CombinatoricsBkgParams comb = CombinatoricsBkgParams(doSyst_comb);
  EventParams events = EventParams(eventsdat, data.sumEntries()*0.3, data.sumEntries()*0.7);

  std::cout << "Signal Parameters:\n";
  sigl.print();

  std::cout << "Combinatorics Background Parameters:\n";
  comb.print();

  std::cout << "Swap Parameters:\n";
  swap.print();

  if (doPkkk) {
    std::cout << "KK Parameters:\n";
    pkkk.print();
  }

  if (doPkpp) {
    std::cout << "pipi Parameters:\n";
    pkpp.print();
  }

  std::cout << "Event Parameters:\n";
  events.print();

  // Define the signal model: double Gaussian
  RooGaussian gauss1("gauss1", "first Gaussian", m, sigl.mean, sigl.sigma1);
  RooGaussian gauss2("gauss2", "second Gaussian", m, sigl.mean, sigl.sigma2);
  RooAddPdf siglPDF("signal", "signal model", RooArgList(gauss1, gauss2), sigl.frac1);

  // Define the background model: (Nominal) Exponential (Systematics) Chebychev polynomial
  RooExponential combPDF_exp("combinatorics_exp", "combinatorics model", m, comb.lambda);
  RooChebychev combPDF_cheb("combinatorics_cheb", "combinatorics model", m, RooArgList(comb.a0, comb.a1));
  RooAbsPdf& combPDF = (comb.doSyst)? (RooAbsPdf&) combPDF_cheb : (RooAbsPdf&) combPDF_exp;

  // Define the swap model: single Gaussian
  RooGaussian swapPDF("swap", "swap model", m, swap.mean, swap.sigma);
  
  // Define the KK model: Crystal Ball
  RooCBShape pkkkPDF("peaking_kk", "peaking background KK state", m, pkkk.mean, pkkk.sigma, pkkk.alpha, pkkk.n );
  
  // Define the pipi model: Crystal Ball
  RooCBShape pkppPDF("peaking_pipi", "peaking background pipi state", m, pkpp.mean, pkpp.sigma, pkpp.alpha, pkpp.n );

  // Define the combined model
  RooArgList PDFList(siglPDF, swapPDF);
  RooArgList eventList(events.nsig, events.nswp);
  if (doPkkk) {
    PDFList.add(pkkkPDF);
    eventList.add(events.npkkk);
  }
  if (doPkpp) {
    PDFList.add(pkppPDF);
    eventList.add(events.npkpp);
  }
  PDFList.add(combPDF);
  eventList.add(events.nbkg);
  RooAddPdf model("model", "signal + background", PDFList, eventList);

  // Fit the model to data
  RooFitResult* result = model.fitTo(data, Save());

  // Create a RooWorkspace
  TFile outputFile(Form("%s/%s", rstDir.c_str(), output.c_str()), "RECREATE");
  RooWorkspace ws("ws");

  // Import the combined model into the workspace
  ws.import(model);

  // Import all relevant parameters and datasets
  ws.import(data); // RooDataSet
  ws.import(siglPDF); // Signal PDF
  ws.import(combPDF); // Combinatorics PDF
  ws.import(swapPDF); // Swap PDF
  if (doPkkk) ws.import(pkkkPDF); // Peaking KK PDF
  if (doPkpp) ws.import(pkppPDF); // Peaking PiPi PDF

  // Optionally, add the fit result
  if (result) {
    result->SetName("FitResult");
    ws.import(*result);
  }

  // Save the workspace into a ROOT file
  ws.Write();
  outputFile.Close();

  std::cout << "Model, parameters, and data saved in fit_results.root." << std::endl;

  // Plot the data and the fit result
  RooPlot* frame = m.frame(DMASSNBINS);
  frame->SetTitle(plotTitle.c_str());
  printf("Plot title: %s\n", plotTitle.c_str());
  data.plotOn(frame);
  model.plotOn(frame);
  model.plotOn(frame, Components(siglPDF), LineStyle(kSolid), LineColor(kRed));
  model.plotOn(frame, Components(swapPDF), LineStyle(kSolid), LineColor(kOrange+1));
  if (doPkkk) model.plotOn(frame, Components(pkkkPDF), LineStyle(kSolid), LineColor(kViolet-3));
  if (doPkpp) model.plotOn(frame, Components(pkppPDF), LineStyle(kSolid), LineColor(kTeal-7));
  model.plotOn(frame, Components(combPDF), LineStyle(kDashed), LineColor(kGray));
  frame->Draw();

  // Add parameter annotations
  double xpos = 0.60, ypos = 0.85, ypos_step = 0.05; // Starting position and step for annotations

  TLatex latex;
  latex.SetTextSize(0.03);
  latex.SetNDC();
  
  
  int lineCount = 0;
  if (comb.doSyst)
  {
    latex.DrawLatex(xpos, ypos - (lineCount++) * ypos_step, Form("a_{0} = %.3f #pm %.3f", comb.a0.getVal(), comb.a0.getError()));
    latex.DrawLatex(xpos, ypos - (lineCount++) * ypos_step, Form("a_{1} = %.3f #pm %.3f", comb.a1.getVal(), comb.a1.getError()));
  } else {
    latex.DrawLatex(xpos, ypos - (lineCount++) * ypos_step, Form("#lambda = %.3f #pm %.3f", comb.lambda.getVal(), comb.lambda.getError()));
  }
  latex.DrawLatex(xpos, ypos - (lineCount++) * ypos_step, Form("Mean = %.3f #pm %.3f (%s)", sigl.mean.getVal(), sigl.mean.getError(),
                                                    sigl.mean.isConstant()? "fixed": "float" ));

  latex.DrawLatex(xpos, ypos - (lineCount++) * ypos_step, Form("N_{Sig} = %.3f #pm %.3f", events.nsig.getVal(), events.nsig.getError()));
  latex.DrawLatex(xpos, ypos - (lineCount++) * ypos_step, Form("N_{Swap} = %.3f #pm %.3f", events.nswp.getVal(),
                                                    events.nswp.getPropagatedError(*result)));
  if (doPkkk) latex.DrawLatex(xpos, ypos - (lineCount++) * ypos_step, Form("N_{KK} = %.3f #pm %.3f", events.npkkk.getVal(),
                                                    events.npkkk.getPropagatedError(*result)));
  if (doPkpp) latex.DrawLatex(xpos, ypos - (lineCount++) * ypos_step, Form("N_{#pi#pi} = %.3f #pm %.3f", events.npkpp.getVal(),
                                                    events.npkpp.getPropagatedError(*result)));
  latex.DrawLatex(xpos, ypos - (lineCount++) * ypos_step, Form("N_{Comb} = %.3f #pm %.3f", events.nbkg.getVal(), events.nbkg.getError()));

  double SoverB = events.nsig.getVal()/TMath::Sqrt(events.nsig.getVal()+events.nbkg.getVal());

  double nll_sb = model.createNLL(data)->getVal();

  events.nsig.setVal(0);
  events.nsig.setConstant(kTRUE); // Fix signal to 0
  model.fitTo(data);
  double nll_b = model.createNLL(data)->getVal();

  // Compute the p-value from delta NLL
  double deltaNLL = nll_b - nll_sb; // Replace with actual values
  double test_stat = 2 * deltaNLL;
  double p_value = TMath::Prob(test_stat, 1); // 1 dof

  // Compute significance (Z-score)
  double significance = TMath::Abs(TMath::NormQuantile(p_value));

  std::cout << "=======================================================" << std::endl;
  std::cout << "=    Significance summary ......" << std::endl;
  std::cout << "=======================================================" << std::endl;
  std::cout << "nll_b: " << nll_b << ", nll_sb: " << nll_sb << ", deltaNLL: " << deltaNLL << std::endl;
  std::cout << "p-value: " << p_value << std::endl;
  std::cout << "Significance: " << significance << " sigma" << std::endl;
  latex.DrawLatex(0.20, ypos - 1 * ypos_step, Form("S/#sqrt{S+B} = %.1f", SoverB));
  latex.DrawLatex(0.20, ypos - 2 * ypos_step, Form("p-value = %.3e", p_value));
  latex.DrawLatex(0.20, ypos - 3 * ypos_step, Form("Significance = %.1f#sigma", significance));

  canvas->SaveAs(Form("%s/fit_result.pdf", rstDir.c_str()));

  delete canvas;
}


int main(int argc, char *argv[]) {
  CommandLine CL(argc, argv);
  string dataInput     = CL.Get      ("dataInput",    "output.root"); // Input data file
  vector<string> mcInputs       = CL.GetStringVector("mcInputs",      "output.root"); // Input mc file
  
  ///// for the component modeling (optional: default is taking the MC sample to do the modeling)
  ///// if the user specify the prefitted shape parameters (.dat), then the main_fit will take those parameters without a prefit procedure directly
  vector<string> sigswpInputs   = CL.GetStringVector("sigswpInputs", 
                                                      CL.Get("mcInputs") ); // Input mc file for signal and swap component
  vector<string> KKmcInputs     = CL.GetStringVector("KKmcInputs",  
                                                      CL.Get("mcInputs") ); // Input mc file for D0 > K K component
  vector<string> pipimcInputs   = CL.GetStringVector("pipimcInputs",
                                                      CL.Get("mcInputs") ); // Input mc file for D0 > pi pi component
  string neventsInput  = CL.Get      ("neventsInput",  ""); // for EventParams that contains the normalization info

  ///// for fitting systematics study
  bool doSyst_sig      = CL.GetBool  ("doSyst_sig", false); // do systematics study for the signal
  bool doSyst_comb     = CL.GetBool  ("doSyst_comb", false); // do systematics study for the combinatorics background
  bool doPkkk          = CL.GetBool  ("doPkkk", true); // include KK peak in background model
  bool doPkpp          = CL.GetBool  ("doPkpp", true); // include pipi peak in background model
  
  string output        = CL.Get      ("Output",  "fit.root");    // Output file
  string rstDir  = CL.Get      ("RstDir","./");       // Label for output file
  
  TFile *in_data_f  = new TFile(dataInput.c_str());
  TTree *datatree = (TTree*) in_data_f->Get("nt");

  ///// to get the plot title
  TDirectoryFile* dataDir = (TDirectoryFile*) in_data_f->Get("par");
  if (!dataDir) {
    std::cerr << "Error: Directory par not found in the file!" << std::endl;
    return 1;
  }

  double parMinDzeroPT = -999.;
  double parMaxDzeroPT = -999.;
  double parMinDzeroY = -999.;
  double parMaxDzeroY = -999.;
  int parIsGammaN = -999.;
  int parTriggerChoice = -999.;

  TH1D* hist = nullptr;

  hist = dynamic_cast<TH1D*>(dataDir->Get("parMinDzeroPT"));
  if (hist) parMinDzeroPT = hist->GetBinContent(1);
  hist = dynamic_cast<TH1D*>(dataDir->Get("parMaxDzeroPT"));
  if (hist) parMaxDzeroPT = hist->GetBinContent(1);
  hist = dynamic_cast<TH1D*>(dataDir->Get("parMinDzeroY"));
  if (hist) parMinDzeroY = hist->GetBinContent(1);
  hist = dynamic_cast<TH1D*>(dataDir->Get("parMaxDzeroY"));
  if (hist) parMaxDzeroY = hist->GetBinContent(1);
  hist = dynamic_cast<TH1D*>(dataDir->Get("parIsGammaN"));
  if (hist) parIsGammaN = (int) hist->GetBinContent(1);
  hist = dynamic_cast<TH1D*>(dataDir->Get("parTriggerChoice"));
  if (hist) parTriggerChoice = (int) hist->GetBinContent(1);

  // Construct the formatted string
  std::ostringstream plotTitle;
  plotTitle << parMinDzeroPT << " #leq D_{p_{T}} < " << parMaxDzeroPT
            << " (GeV/#it{c}), " << parMinDzeroY << " #leq D_{y} < " << parMaxDzeroY
            << (parIsGammaN == 1 ? ", #gammaN" : ", N#gamma")
            << (parTriggerChoice == 1 ? ", ZDCOR" : ", ZDCXORJet8");

  TChain *mctree = new TChain("nt");
  for (auto file : mcInputs) mctree->Add(file.c_str());

  string sigldat, swapdat, pkkkdat, pkppdat;
  string nevtdat;
  if (neventsInput=="")
  {
    double nsig = mctree->GetEntries("(Dgen == 23333 || Dgen == 41022 || Dgen == 41044) && Dmass>1.68 && Dmass<2.05");
    double nswp = mctree->GetEntries("(Dgen == 23344 || Dgen == 41122 || Dgen == 41144) && Dmass>1.68 && Dmass<2.05");
    double npkkk = mctree->GetEntries("Dgen == 333 && Dmass < 1.8648 && Dmass>1.68 && Dmass<2.05");
    double npkpp = mctree->GetEntries("Dgen == 333 && Dmass > 1.8648 && Dmass>1.68 && Dmass<2.05");
    nevtdat = Form("%s/events.dat", rstDir.c_str());
    EventParams::writeFracToDat(nevtdat, nswp / nsig,
                                         npkkk / nsig,
                                         npkpp / nsig);
  } else {
    nevtdat = neventsInput;
  }

  if (!(sigswpInputs.size()==1 && sigswpInputs[0].find(".dat")!=string::npos)) {
    TChain *sigswptree = new TChain("nt");
    for (auto file : sigswpInputs) sigswptree->Add(file.c_str());
    sigswpmc_fit(sigswptree, rstDir, sigldat, swapdat, plotTitle.str());
  }
  if (doPkkk && !(KKmcInputs.size()==1 && KKmcInputs[0].find(".dat")!=string::npos)) {
    TChain *KKmctree = new TChain("nt");
    for (auto file : KKmcInputs) KKmctree->Add(file.c_str());
    kkmc_fit(KKmctree, rstDir, pkkkdat, plotTitle.str());
  }
  if (doPkpp && !(pipimcInputs.size()==1 && pipimcInputs[0].find(".dat")!=string::npos)) {
    TChain *pipimctree = new TChain("nt");
    for (auto file : pipimcInputs) pipimctree->Add(file.c_str());
    pipimc_fit(mctree, rstDir, pkppdat, plotTitle.str());
  }
  
  main_fit(datatree, rstDir, output,
           sigldat, swapdat, pkkkdat, pkppdat,
           nevtdat,
           doSyst_sig, doSyst_comb,
           doPkkk, doPkpp,
           plotTitle.str());

  return 0;
}
