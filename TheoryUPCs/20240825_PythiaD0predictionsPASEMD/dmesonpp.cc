// main70.cc is a part of the PYTHIA event generator.
// Copyright (C) 2024 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL v2 or later, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// Authors: Ilkka Helenius <ilkka.m.helenius@jyu.fi>

// Keywords: photon beam; DIS; UPC; heavy ions; photoproduction

// Main program to demonstrate how to define a photon flux and use that
// to generate charged-particle pT spectra in photo-production processes.
#include "Pythia8/Pythia.h"
#include "TClonesArray.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH1F.h"
#include "TH2F.h"
#include "THnSparse.h"
#include "TList.h"
#include "TMath.h"
#include "TNtuple.h"
#include "TProfile.h"
#include "TRandom3.h"
#include "TString.h"
#include "TTree.h"
#include "TVector3.h"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/PseudoJet.hh"
#include "parampp.h"
#include <algorithm>
#include <cstdio> // needed for io
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iostream> // needed for io
#include <math.h>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <time.h> /* time */
#include <valarray>
#include <vector>

using namespace Pythia8;
// The main program.

int main(int argc, char *argv[]) {

  // Generator.
  Pythia pythia;
  int cislo = -1; // unique number for each file
  cislo = atoi(argv[1]);

  // Decrease the output.
  pythia.readString("Init:showChangedSettings = off");
  pythia.readString("Init:showChangedParticleData = off");
  pythia.readString("Next:numberCount = 100000");
  pythia.readString("Next:numberShowInfo = 0");
  pythia.readString("Next:numberShowProcess = 1");
  pythia.readString("Next:numberShowEvent = 3");

  // Pointer to externally defined photon flux.
  PDFPtr photonFlux = 0;
  pythia.readString("Random:setSeed = on");
  pythia.readString(Form("Random:seed = %d", cislo));
  std::cout << "Using seed = " << cislo << std::endl;
  // Beam parameters.
  pythia.readString("tune:pp = 14");
  pythia.readString("Beams:eCM = 5020.");
  pythia.readString("Beams:idA = 2212");
  pythia.readString("Beams:idB = 2212");
  pythia.readString("SoftQCD:all = on");

  // Initialize the generator.
  pythia.init();

  // Parameters for histograms.
  int idD0 = 421;

  vector<vector<double>> dsigma(nPtBins, vector<double>(nYBins, 0));
  // Initialize the histograms.
  const char *labellumi[3] = {"sigmagen(mb)", "luminosity (mb^-1)",
                              "Accepted events"};
  TH1F *hlumicounter =
      new TH1F("hlumicounter", "Luminosity counter; ;Value", 3, 0.5, 3.5);
  for (int i = 1; i <= 3; i++) {
    hlumicounter->GetXaxis()->SetBinLabel(i, labellumi[i - 1]);
  }
  TH2D *hUnnormCrossSection =
      new TH2D("hUnnormCrossSection", "Unnormalized cross section; pT (GeV); y",
               nPtBins, ptBins, nYBins, yBins);
  // Number of events.
  int nEvent = 500000;
  // Begin event loop. Skip if fails.
  for (int iEvent = 0; iEvent < nEvent; ++iEvent) {
    // Generate next event.
    if (!pythia.next())
      continue;
    bool isbevent = false;
    for (int i = 0; i < pythia.event.size(); ++i) {
      if (pythia.event[i].idAbs() == 5) {
        isbevent = true;
      }
    }
    if (isbevent)
      continue;
    // Event weight.
    double weight = pythia.info.weight();

    // Loop over event record.
    // std::cout << "photonIndex = " << photonIndex << std::endl;
    for (int i = 0; i < pythia.event.size(); ++i) {
      if (pythia.event[i].idAbs() == idD0) {
        // Store the pT value for the selected rapidity interval.
        double pT = pythia.event[i].pT();
        double y = pythia.event[i].y();
        hUnnormCrossSection->Fill(pT, y);
        for (int j = 0; j < nPtBins; ++j) {
          for (int k = 0; k < nYBins; ++k) {
            if (pT > ptBins[j] && pT < ptBins[j + 1] && y > yBins[k] &&
                y < yBins[k + 1]) {
              dsigma[j][k] += weight;
            }
          }
        }
      }
    }

  } // End of event loop.

  // Show statistics.
  pythia.stat();

  // Normalize to cross section [mb].
  double sigmaNorm = pythia.info.sigmaGen() / pythia.info.weightSum();

  cout << endl
       << "Double-differential cross section dsigma for single-particle D0 / "
          "dpT dy in nb, attention!!!!!!!!!!!! b events are now removed"
       << endl;
  << endl;
  cout << fixed << " Bins in pT and y | ";
  for (int k = 0; k < nYBins; ++k) {
    cout << yBins[k] << ", " << yBins[k + 1] << " | ";
  }
  cout << endl;
  for (int j = 0; j < nPtBins; ++j) {
    cout << fixed << ptBins[j] << ", " << ptBins[j + 1] << " | ";
    for (int k = 0; k < nYBins; ++k) {
      cout << scientific
           << dsigma[j][k] * sigmaNorm * 1e6 * 0.5 /
                  ((ptBins[j + 1] - ptBins[j]) * (yBins[k + 1] - yBins[k]))
           << " | ";
    }
    cout << endl;
  }
  cout << endl;
  double lumiinvmb = pythia.info.nAccepted() / (pythia.info.sigmaGen());
  hlumicounter->SetBinContent(1, pythia.info.sigmaGen());
  hlumicounter->SetBinContent(2, lumiinvmb);
  hlumicounter->SetBinContent(3, pythia.info.nAccepted());

  TFile *fout = new TFile("output.root", "RECREATE");
  fout->cd();
  hlumicounter->Write();
  hUnnormCrossSection->Write();
  fout->Close();
  delete fout;
  // Done.
}
