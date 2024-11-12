#ifndef _VALIDATEHIST_H_
#define _VALIDATEHIST_H_

/* + Descriptions:
 *		Class of reading and writing the root files for validation use
 */

#include <iostream>
#include <map>
#include <string>

#include "TFile.h"
#include "TH1D.h"

using namespace std;

class ValidateHist {
public:
  ValidateHist(string filename,
               string mode); // read mode 	= "r", "READ"
                             // write mode = "recreate", "RECREATE"
  ~ValidateHist();

  void FillEventInfo(int _Run, int _Lumi, int _Event, float _VX, float _VY, float _VZ, float _VXError, float _VYError,
                     float _VZError, int _Dsize, int _Gsize = -999) {
    h_Run->Fill(_Run);
    h_Lumi->Fill(_Lumi);
    h_Event->Fill(_Event);
    h_VX->Fill(_VX);
    h_VY->Fill(_VY);
    h_VZ->Fill(_VZ);
    h_VXError->Fill(_VXError);
    h_VYError->Fill(_VYError);
    h_VZError->Fill(_VZError);
    h_Dsize->Fill(_Dsize);
    if (_Gsize > 0)
      h_Gsize->Fill(_Gsize);
  };

  void FillEventSelectionInfo(int _isL1ZDCOr, int _isL1ZDCXORJet8, int _selectedBkgFilter, int _selectedVtxFilter,
                              int _gammaN, int _Ngamma) {
    h_isL1ZDCOr->Fill(_isL1ZDCOr);
    h_isL1ZDCXORJet8->Fill(_isL1ZDCXORJet8);
    h_selectedBkgFilter->Fill(_selectedBkgFilter);
    h_selectedVtxFilter->Fill(_selectedVtxFilter);
    h_gammaN->Fill(_gammaN);
    h_Ngamma->Fill(_Ngamma);
  };

  void FillRecoDInfo(float _Dpt, float _Dy, float _Dmass, float _Dtrk1Pt, float _Dtrk2Pt, float _Dchi2cl,
                     float _DsvpvDistance, float _DsvpvDisErr, float _Dalpha, float _DsvpvDistance_2D,
                     float _DsvpvDisErr_2D, float _Ddtheta) {
    h_Dpt->Fill(_Dpt);
    h_Dy->Fill(_Dy);
    h_Dmass->Fill(_Dmass);
    h_Dtrk1Pt->Fill(_Dtrk1Pt);
    h_Dtrk2Pt->Fill(_Dtrk2Pt);
    h_Dchi2cl->Fill(_Dchi2cl);
    h_DsvpvDistance->Fill(_DsvpvDistance);
    h_DsvpvDisErr->Fill(_DsvpvDisErr);
    h_Dalpha->Fill(_Dalpha);
    h_DsvpvDistance_2D->Fill(_DsvpvDistance_2D);
    h_DsvpvDisErr_2D->Fill(_DsvpvDisErr_2D);
    h_Ddtheta->Fill(_Ddtheta);
  };

  void FillRecoDGenMatchedInfo(float _Dgen, float _DisSignalCalc, float _DisSignalCalcPrompt,
                               float _DisSignalCalcFeeddown) {
    h_Dgen->Fill(_Dgen);
    h_DisSignalCalc->Fill(_DisSignalCalc);
    h_DisSignalCalcPrompt->Fill(_DisSignalCalcPrompt);
    h_DisSignalCalcFeeddown->Fill(_DisSignalCalcFeeddown);
  };

  void FillGenDInfo(float _Gpt, float _Gy, float _GisSignalCalc, float _GisSignalCalcPrompt,
                    float _GisSignalCalcFeeddown) {
    h_Gpt->Fill(_Gpt);
    h_Gy->Fill(_Gy);
    h_GisSignalCalc->Fill(_GisSignalCalc);
    h_GisSignalCalcPrompt->Fill(_GisSignalCalcPrompt);
    h_GisSignalCalcFeeddown->Fill(_GisSignalCalcFeeddown);
  };
  void Write();

  TFile *output;

  map<string, TH1D *> histDict;

private:
  // event by event info
  TH1D *h_Run;
  TH1D *h_Lumi;
  TH1D *h_Event;

  TH1D *h_isL1ZDCOr;
  TH1D *h_isL1ZDCXORJet8;
  TH1D *h_selectedBkgFilter;
  TH1D *h_selectedVtxFilter;
  TH1D *h_gammaN;
  TH1D *h_Ngamma;

  TH1D *h_VX;
  TH1D *h_VY;
  TH1D *h_VZ;
  TH1D *h_VXError;
  TH1D *h_VYError;
  TH1D *h_VZError;

  TH1D *h_Dsize;

  // D candidates
  TH1D *h_Dpt;
  TH1D *h_Dy;
  TH1D *h_Dmass;
  TH1D *h_Dtrk1Pt;
  TH1D *h_Dtrk2Pt;
  TH1D *h_Dchi2cl;
  TH1D *h_DsvpvDistance;
  TH1D *h_DsvpvDisErr;
  TH1D *h_Dalpha;
  TH1D *h_DsvpvDistance_2D;
  TH1D *h_DsvpvDisErr_2D;
  TH1D *h_Ddtheta;

  // MC only quantities
  TH1D *h_Dgen;
  TH1D *h_DisSignalCalc;
  TH1D *h_DisSignalCalcPrompt;
  TH1D *h_DisSignalCalcFeeddown;

  TH1D *h_Gsize;
  TH1D *h_Gpt;
  TH1D *h_Gy;
  TH1D *h_GisSignalCalc;
  TH1D *h_GisSignalCalcPrompt;
  TH1D *h_GisSignalCalcFeeddown;
};

ValidateHist::ValidateHist(string filename,
                           string mode) // read mode 	= "r", "READ", ""
                                        // write mode = "recreate", "RECREATE"
{
  output = new TFile(filename.c_str(), mode.c_str());

  if (mode == "r" || mode == "READ" || mode == "") {
    h_Run = (TH1D *)output->Get("h_Run");
    histDict["Run"] = h_Run;
    h_Lumi = (TH1D *)output->Get("h_Lumi");
    histDict["Lumi"] = h_Lumi;
    h_Event = (TH1D *)output->Get("h_Event");
    histDict["Event"] = h_Event;

    h_isL1ZDCOr = (TH1D *)output->Get("h_isL1ZDCOr");
    histDict["isL1ZDCOr"] = h_isL1ZDCOr;
    h_isL1ZDCXORJet8 = (TH1D *)output->Get("h_isL1ZDCXORJet8");
    histDict["isL1ZDCXORJet8"] = h_isL1ZDCXORJet8;
    h_selectedBkgFilter = (TH1D *)output->Get("h_selectedBkgFilter");
    histDict["selectedBkgFilter"] = h_selectedBkgFilter;
    h_selectedVtxFilter = (TH1D *)output->Get("h_selectedVtxFilter");
    histDict["selectedVtxFilter"] = h_selectedVtxFilter;
    h_gammaN = (TH1D *)output->Get("h_gammaN");
    histDict["gammaN"] = h_gammaN;
    h_Ngamma = (TH1D *)output->Get("h_Ngamma");
    histDict["Ngamma"] = h_Ngamma;

    h_VX = (TH1D *)output->Get("h_VX");
    histDict["VX"] = h_VX;
    h_VY = (TH1D *)output->Get("h_VY");
    histDict["VY"] = h_VY;
    h_VZ = (TH1D *)output->Get("h_VZ");
    histDict["VZ"] = h_VZ;
    h_VXError = (TH1D *)output->Get("h_VXError");
    histDict["VXError"] = h_VXError;
    h_VYError = (TH1D *)output->Get("h_VYError");
    histDict["VYError"] = h_VYError;
    h_VZError = (TH1D *)output->Get("h_VZError");
    histDict["VZError"] = h_VZError;

    h_Dsize = (TH1D *)output->Get("h_Dsize");
    histDict["Dsize"] = h_Dsize;

    h_Dpt = (TH1D *)output->Get("h_Dpt");
    histDict["Dpt"] = h_Dpt;
    h_Dy = (TH1D *)output->Get("h_Dy");
    histDict["Dy"] = h_Dy;
    h_Dmass = (TH1D *)output->Get("h_Dmass");
    histDict["Dmass"] = h_Dmass;
    h_Dtrk1Pt = (TH1D *)output->Get("h_Dtrk1Pt");
    histDict["Dtrk1Pt"] = h_Dtrk1Pt;
    h_Dtrk2Pt = (TH1D *)output->Get("h_Dtrk2Pt");
    histDict["Dtrk2Pt"] = h_Dtrk2Pt;
    h_Dchi2cl = (TH1D *)output->Get("h_Dchi2cl");
    histDict["Dchi2cl"] = h_Dchi2cl;
    h_DsvpvDistance = (TH1D *)output->Get("h_DsvpvDistance");
    histDict["DsvpvDistance"] = h_DsvpvDistance;
    h_DsvpvDisErr = (TH1D *)output->Get("h_DsvpvDisErr");
    histDict["DsvpvDisErr"] = h_DsvpvDisErr;
    h_Dalpha = (TH1D *)output->Get("h_Dalpha");
    histDict["Dalpha"] = h_Dalpha;
    h_DsvpvDistance_2D = (TH1D *)output->Get("h_DsvpvDistance_2D");
    histDict["DsvpvDistance_2D"] = h_DsvpvDistance_2D;
    h_DsvpvDisErr_2D = (TH1D *)output->Get("h_DsvpvDisErr_2D");
    histDict["DsvpvDisErr_2D"] = h_DsvpvDisErr_2D;
    h_Ddtheta = (TH1D *)output->Get("h_Ddtheta");
    histDict["Ddtheta"] = h_Ddtheta;

    h_Dgen = (TH1D *)output->Get("h_Dgen");
    histDict["Dgen"] = h_Dgen;
    h_DisSignalCalc = (TH1D *)output->Get("h_DisSignalCalc");
    histDict["DisSignalCalc"] = h_DisSignalCalc;
    h_DisSignalCalcPrompt = (TH1D *)output->Get("h_DisSignalCalcPrompt");
    histDict["DisSignalCalcPrompt"] = h_DisSignalCalcPrompt;
    h_DisSignalCalcFeeddown = (TH1D *)output->Get("h_DisSignalCalcFeeddown");
    histDict["DisSignalCalcFeeddown"] = h_DisSignalCalcFeeddown;

    h_Gsize = (TH1D *)output->Get("h_Gsize");
    histDict["Gsize"] = h_Gsize;
    h_Gpt = (TH1D *)output->Get("h_Gpt");
    histDict["Gpt"] = h_Gpt;
    h_Gy = (TH1D *)output->Get("h_Gy");
    histDict["Gy"] = h_Gy;
    h_GisSignalCalc = (TH1D *)output->Get("h_GisSignalCalc");
    histDict["GisSignalCalc"] = h_GisSignalCalc;
    h_GisSignalCalcPrompt = (TH1D *)output->Get("h_GisSignalCalcPrompt");
    histDict["GisSignalCalcPrompt"] = h_GisSignalCalcPrompt;
    h_GisSignalCalcFeeddown = (TH1D *)output->Get("h_GisSignalCalcFeeddown");
    histDict["GisSignalCalcFeeddown"] = h_GisSignalCalcFeeddown;
  } else if (mode == "recreate" || mode == "RECREATE") {
    h_Run = new TH1D("h_Run", "; Run; Count", 1000, 0, 400000);
    histDict["Run"] = h_Run;
    h_Lumi = new TH1D("h_Lumi", "; Lumi; Count", 1000, 0, 1000);
    histDict["Lumi"] = h_Lumi;
    h_Event = new TH1D("h_Event", "; Event; Count", 1000, 0, 1.30e9);
    histDict["Event"] = h_Event;

    h_isL1ZDCOr = new TH1D("h_isL1ZDCOr", "; isL1ZDCOr; Count", 2, 0, 2);
    histDict["isL1ZDCOr"] = h_isL1ZDCOr;
    h_isL1ZDCXORJet8 = new TH1D("h_isL1ZDCXORJet8", "; isL1ZDCXORJet8; Count", 2, 0, 2);
    histDict["isL1ZDCXORJet8"] = h_isL1ZDCXORJet8;
    h_selectedBkgFilter = new TH1D("h_selectedBkgFilter", "; selectedBkgFilter; Count", 2, 0, 2);
    histDict["selectedBkgFilter"] = h_selectedBkgFilter;
    h_selectedVtxFilter = new TH1D("h_selectedVtxFilter", "; selectedVtxFilter; Count", 2, 0, 2);
    histDict["selectedVtxFilter"] = h_selectedVtxFilter;
    h_gammaN = new TH1D("h_gammaN", "; gammaN; Count", 2, 0, 2);
    histDict["gammaN"] = h_gammaN;
    h_Ngamma = new TH1D("h_Ngamma", "; Ngamma; Count", 2, 0, 2);
    histDict["Ngamma"] = h_Ngamma;

    h_VX = new TH1D("h_VX", "; VX; Count", 40, -0.5, 0.5);
    histDict["VX"] = h_VX;
    h_VY = new TH1D("h_VY", "; VY; Count", 40, -0.5, 0.5);
    histDict["VY"] = h_VY;
    h_VZ = new TH1D("h_VZ", "; VZ; Count", 40, -20, 20);
    histDict["VZ"] = h_VZ;
    h_VXError = new TH1D("h_VXError", "; VXError; Count", 40, 0, 0.1);
    histDict["VXError"] = h_VXError;
    h_VYError = new TH1D("h_VYError", "; VYError; Count", 40, 0, 0.1);
    histDict["VYError"] = h_VYError;
    h_VZError = new TH1D("h_VZError", "; VZError; Count", 40, 0, 0.5);
    histDict["VZError"] = h_VZError;

    h_Dsize = new TH1D("h_Dsize", "; Dsize; Count", 40, 0, 20);
    histDict["Dsize"] = h_Dsize;

    h_Dpt = new TH1D("h_Dpt", "; Dpt; Count", 40, 0, 10);
    histDict["Dpt"] = h_Dpt;
    h_Dy = new TH1D("h_Dy", "; Dy; Count", 40, -3, 3);
    histDict["Dy"] = h_Dy;
    h_Dmass = new TH1D("h_Dmass", "; Dmass; Count", 40, 1.5, 2.2);
    histDict["Dmass"] = h_Dmass;
    h_Dtrk1Pt = new TH1D("h_Dtrk1Pt", "; Dtrk1Pt; Count", 40, 0, 5);
    histDict["Dtrk1Pt"] = h_Dtrk1Pt;
    h_Dtrk2Pt = new TH1D("h_Dtrk2Pt", "; Dtrk2Pt; Count", 40, 0, 5);
    histDict["Dtrk2Pt"] = h_Dtrk2Pt;
    h_Dchi2cl = new TH1D("h_Dchi2cl", "; Dchi2cl; Count", 40, 0, 1);
    histDict["Dchi2cl"] = h_Dchi2cl;
    h_DsvpvDistance = new TH1D("h_DsvpvDistance", "; DsvpvDistance; Count", 40, 0, 40);
    histDict["DsvpvDistance"] = h_DsvpvDistance;
    h_DsvpvDisErr = new TH1D("h_DsvpvDisErr", "; DsvpvDisErr; Count", 40, 0, 5);
    histDict["DsvpvDisErr"] = h_DsvpvDisErr;
    h_Dalpha = new TH1D("h_Dalpha", "; Dalpha; Count", 40, 0, 0.55);
    histDict["Dalpha"] = h_Dalpha;
    h_DsvpvDistance_2D = new TH1D("h_DsvpvDistance_2D", "; DsvpvDistance_2D; Count", 40, 0, 15);
    histDict["DsvpvDistance_2D"] = h_DsvpvDistance_2D;
    h_DsvpvDisErr_2D = new TH1D("h_DsvpvDisErr_2D", "; DsvpvDisErr_2D; Count", 40, 0, 1.5);
    histDict["DsvpvDisErr_2D"] = h_DsvpvDisErr_2D;
    h_Ddtheta = new TH1D("h_Ddtheta", "; Ddtheta; Count", 40, 0, 3.5);
    histDict["Ddtheta"] = h_Ddtheta;

    h_Dgen = new TH1D("h_Dgen", "; Dgen; Count", 40, 0, 45e3);
    histDict["Dgen"] = h_Dgen;
    h_DisSignalCalc = new TH1D("h_DisSignalCalc", "; DisSignalCalc; Count", 2, 0, 2);
    histDict["DisSignalCalc"] = h_DisSignalCalc;
    h_DisSignalCalcPrompt = new TH1D("h_DisSignalCalcPrompt", "; DisSignalCalcPrompt; Count", 2, 0, 2);
    histDict["DisSignalCalcPrompt"] = h_DisSignalCalcPrompt;
    h_DisSignalCalcFeeddown = new TH1D("h_DisSignalCalcFeeddown", "; DisSignalCalcFeeddown; Count", 2, 0, 2);
    histDict["DisSignalCalcFeeddown"] = h_DisSignalCalcFeeddown;

    h_Gsize = new TH1D("h_Gsize", "; Gsize; Count", 10, 0, 10);
    histDict["Gsize"] = h_Gsize;
    h_Gpt = new TH1D("h_Gpt", "; Gpt; Count", 40, 0, 20);
    histDict["Gpt"] = h_Gpt;
    h_Gy = new TH1D("h_Gy", "; Gy; Count", 40, -10, 10);
    histDict["Gy"] = h_Gy;
    h_GisSignalCalc = new TH1D("h_GisSignalCalc", "; GisSignalCalc; Count", 2, 0, 2);
    histDict["GisSignalCalc"] = h_GisSignalCalc;
    h_GisSignalCalcPrompt = new TH1D("h_GisSignalCalcPrompt", "; GisSignalCalcPrompt; Count", 2, 0, 2);
    histDict["GisSignalCalcPrompt"] = h_GisSignalCalcPrompt;
    h_GisSignalCalcFeeddown = new TH1D("h_GisSignalCalcFeeddown", "; GisSignalCalcFeeddown; Count", 2, 0, 2);
    histDict["GisSignalCalcFeeddown"] = h_GisSignalCalcFeeddown;
  } else {
    printf("[Error] mode (%s) is not recognized! Exiting ...\n", mode.c_str());
    exit(0);
  }
}

ValidateHist::~ValidateHist() {
  for (const auto &[key, value] : histDict) {
    if (value)
      delete value;
  }

  output->Close();
  if (output)
    delete output;
}

void ValidateHist::Write() {
  output->cd();

  for (const auto &[key, value] : histDict) {
    cout << "Save " << key << endl;
    value->Write();
  }
}

#endif