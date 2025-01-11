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
#include "param.h"
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

// Photon flux from leptons, corresponds to internal Lepton2gamma.

class Lepton2gamma2 : public PDF {

public:
  // Constructor.
  Lepton2gamma2(int idBeamIn) : PDF(idBeamIn) {}

  // Update the photon flux.
  void xfUpdate(int, double x, double Q2) {
    xgamma = 0.5 * 0.007297353080 / M_PI * (1. + pow2(1. - x)) / Q2;
  }
};

// Photon flux from lead-ions. Integrated over impact parameters > 2*r_Pb.
// Suitable for photo-nuclear processes but not for photon-photon.
// This should be considered as an experimental setup and used with caution.

class Nucleus2gamma2 : public PDF {

public:
  // Constructor.
  Nucleus2gamma2(int idBeamIn) : PDF(idBeamIn) {}

  // Update the photon flux.
  void xfUpdate(int, double x, double) {

    // Minimum impact parameter (~2*radius) [fm].
    double bmin = 2 * 6.636;

    // Charge of the nucleus.
    double z = 82.;

    // Per-nucleon mass for lead.
    double m2 = pow2(0.9314);
    double alphaEM = 0.007297353080;
    double hbarc = 0.197;
    double xi = x * sqrt(m2) * bmin / hbarc;
    double bK0 = besselK0(xi);
    double bK1 = besselK1(xi);
    double intB = xi * bK1 * bK0 - 0.5 * pow2(xi) * (pow2(bK1) - pow2(bK0));
    xgamma = 2. * alphaEM * pow2(z) / M_PI * intB;
  }
};

// The main program.

int main(int argc, char *argv[]) {
  bool verbose = false;
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
  pythia.readString("Beams:eCM = 5360.");
  pythia.readString("Beams:idA = 2212");
  pythia.readString("Beams:idB = 2212");
  pythia.readString("PDF:beamA2gamma = on");
  pythia.readString("PDF:proton2gammaSet = 0");

  // Use nuclear PDF for the hard process generation in the proton side.
  // pythia.readString("PDF:useHardNPDFB = on");
  // Modify the minimum impact parameter to match the flux defined above.
  pythia.readString("PDF:gammaFluxApprox2bMin = 13.272");
  // Optimized sampling for photon flux from nuclei.
  pythia.readString("PDF:beam2gammaApprox = 2");
  // Do not sample virtuality since use b-integrated flux here.
  pythia.readString("Photon:sampleQ2 = off");
  photonFlux = make_shared<Nucleus2gamma2>(2212);

  // Set the external photon flux for beam A.
  pythia.setPhotonFluxPtr(photonFlux, 0);

  // Switch relevant processes on.
  // pythia.readString("HardQCD:all = on");
  pythia.readString("SoftQCD:all = on");
  pythia.readString("PhotonParton:all = on");

  // Limit partonic pThat.
  // pythia.readString("PhaseSpace:pTHatMin = 10.0");

  // Use optimized pT0ref for photon-hadron.
  pythia.readString("MultipartonInteractions:pT0Ref = 3.0");

  // Initialize the generator.
  pythia.init();

  // Parameters for histograms.
  double pTmin = 0.0;
  double pTmax = 20.0;
  int nBinsPT = 10;

  int idD0 = 421;
  // double yMin = -2.;
  // double yMax =  2.;
  double yMin = -1.;
  double yMax = 0.;

  // vector<double> yBins = {-2., -1., 0., 1., 2.};
  // vector<double> ptBins = {2., 4., 8., 12.};
  // int nYBins = yBins.size() - 1;
  // int nPtBins = ptBins.size() - 1;

  vector<vector<double>> dsigma(nPtBins, vector<double>(nYBins, 0));
  // vector<vector<int>> vec(m, vector<int> (n, 0));

  // - pt ranges {2,4}, {4,8}, {8,12} GeV
  // - rapidity {-2,-1}, {-1, 0}, {0,1}, {1,2}.

  // Initialize the histograms.
  Hist pTD0("D0 pT distribution", nBinsPT, pTmin, pTmax);
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
  TH1D *hphotonEnergy[nPtBins][nYBins];
  TH1D *hgluonxBjorken[nPtBins][nYBins];
  TH1D *hgluonQ2[nPtBins][nYBins];
  for (int i = 0; i < nPtBins; ++i) {
    for (int j = 0; j < nYBins; ++j) {
      hphotonEnergy[i][j] =
          new TH1D(Form("hphotonEnergy_pt%d_y%d", i, j),
                   Form("Photon energy distribution for pT in [%f, %f] and y "
                        "in [%f, %f]; Photon energy (GeV); Entries",
                        ptBins[i], ptBins[i + 1], yBins[j], yBins[j + 1]),
                   10000, 0., 1000.);
      hgluonxBjorken[i][j] =
          new TH1D(Form("hgluonxBjorken_pt%d_y%d", i, j),
                   "Gluon xBjorken distribution for pT in [%f, %f] and y in "
                   "[%f, %f]; Gluon Bjorken x; Entries",
                   500000, 0., 0.5);
      hgluonQ2[i][j] =
          new TH1D(Form("hgluonQ2_pt%d_y%d", i, j),
                   "Gluon Q^2 distribution for pT in [%f, %f] and y in [%f, "
                   "%f]; Gluon Q^2 (GeV^2); Entries",
                   1000, 0., 1000.);
    }
  }
  // Number of events.
  int nEvent = 500000;
  // Begin event loop. Skip if fails.
  for (int iEvent = 0; iEvent < nEvent; ++iEvent) {
    // Generate next event.
    if (!pythia.next())
      continue;
    /*
        bool isbevent = false;
        for (int i = 0; i < pythia.event.size(); ++i) {
          if (pythia.event[i].idAbs() == 5) {
            isbevent = true;
          }
        }
        // if (isbevent) continue;
    */
    // Event weight.
    double weight = pythia.info.weight();
    // Loop over event record.
    // std::cout << "photonIndex = " << photonIndex << std::endl;
    int photonIndex = -1;
    int parton2id = pythia.info.id2();

    for (int i = 0; i < pythia.event.size(); ++i) {
      if (pythia.event[i].idAbs() == 22 && pythia.event[i].status() == -13) {
        photonIndex = i;
        break;
      }
    }
    if (photonIndex < 0) {
      std::cout << "Photon not found in the event." << std::endl;
      // continue;
    }
    double energyph = -1.;
    // double xBJ_kinestimation = -1.;
    for (int i = 0; i < pythia.event.size(); ++i) {
      if (pythia.event[i].idAbs() == idD0) {
        // Store the pT value for the selected rapidity interval.
        double pT = pythia.event[i].pT();
        double y = pythia.event[i].y();
        if (y > yMin && y < yMax)
          pTD0.fill(pT, weight);
        hUnnormCrossSection->Fill(pT, y);
        int processCode = pythia.info.code();
        int subprocessCode = pythia.info.codeSub();
        // Print the subprocess code
        // pythia.event.list();
        int iptbin = hUnnormCrossSection->GetXaxis()->FindBin(pT);
        int iybin = hUnnormCrossSection->GetYaxis()->FindBin(y);
        if (photonIndex > 0 && iptbin > 0 && iptbin < nPtBins + 1 &&
            iybin > 0 && iybin < nYBins + 1) {
          energyph = pythia.event[photonIndex].e();
          hphotonEnergy[iptbin - 1][iybin - 1]->Fill(energyph);
          if (parton2id == 21) {

            hgluonxBjorken[iptbin - 1][iybin - 1]->Fill(pythia.info.x2pdf());
            hgluonQ2[iptbin - 1][iybin - 1]->Fill(pythia.info.Q2Fac());
            // double sqrtsgammagluon = 4*pythia.info.x2()*2.68*energyph;
          }
          if (verbose) {
            std::cout << "Event #" << iEvent + 1
                      << ": Process code = " << processCode << std::endl;
            std::cout << "Subprocess code = " << subprocessCode << std::endl;
            std::cout << "pT = " << pT << " y = " << y << std::endl;
            std::cout << "photon energy = " << energyph << std::endl;
            std::cout << "Parton 2 is a gluon with x = " << pythia.info.x2()
                      << std::endl;
          }
        }
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
  double pTbin = (pTmax - pTmin) / (1. * nBinsPT);

  // Normalize to cross section, account for 208 target nucleons.
  pTD0 *= 208 * sigmaNorm / pTbin;

  cout << pTD0;

  cout << endl
       << "Double-differential cross section dsigma for single-particle D0 / "
          "dpT dy in nb"
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
           << dsigma[j][k] * 208 * sigmaNorm * 1e6 * 0.5 /
                  (ptBins[j + 1] - ptBins[j])
           << " | ";
    }
    cout << endl;
  }
  cout << endl;
  double lumiinvmbAAequivalent =
      pythia.info.nAccepted() / (208 * pythia.info.sigmaGen());
  hlumicounter->SetBinContent(1, pythia.info.sigmaGen());
  hlumicounter->SetBinContent(2, lumiinvmbAAequivalent);
  hlumicounter->SetBinContent(3, pythia.info.nAccepted());

  TFile *fout = new TFile("output.root", "RECREATE");
  fout->cd();
  hlumicounter->Write();
  hUnnormCrossSection->Write();
  for (int i = 0; i < nPtBins; ++i) {
    for (int j = 0; j < nYBins; ++j) {
      hphotonEnergy[i][j]->Write();
      hgluonxBjorken[i][j]->Write();
      hgluonQ2[i][j]->Write();
    }
  }
  fout->Close();
  delete fout;
  // Done.
}
