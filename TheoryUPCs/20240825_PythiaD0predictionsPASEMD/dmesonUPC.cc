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

// Petja's calculation for the flux using interpolation grids.
// Based on arXiv:2404.09731 [hep-ph].
// Nucleus2gamma3 is for point-like approximation
//
class Nucleus2gamma3 : public PDF {

public:

  // Constructor.
  Nucleus2gamma3(int idBeamIn, double xminIn, double xmaxIn, double kPowIn,
                 bool emBreakUp) : PDF(idBeamIn) {

    // Initialize parameters.
    xk.resize(nPoints);
    w.resize(nPoints);
    logF.resize(nPoints);
    xmin = xminIn;
    xmax = xmaxIn;
    kPow = kPowIn;
    double xmin_k = pow(xmin, kPow);
    double xmax_k = pow(xmax, kPow);
    double d = 0;

    // Calculate Chebyshev interpolation points and read in table.
    for (size_t ipx = 0; ipx < nPoints; ipx++) {
      xk[ipx] = 0.5*(xmax_k + xmin_k)
        - 0.5*(xmax_k - xmin_k)*cos(M_PI*ipx/(nPoints-1.0));

      d = (ipx == 0 || ipx == nPoints - 1) ? 0.5 : 1.0;
      w[ipx] = (ipx % 2 == 0) ? d : -d;

      // Use globally defined hard-coded tables with or without EM break-up.
      logF[ipx] = emBreakUp ? fluxPointsAnPL[ipx]
        : logF[ipx] = fluxPoints0nPL[ipx];
    }
  }

  // Update the photon flux.
  void xfUpdate(int , double x, double ) {

    // If below first interpolation point return flux at min value.
    if (x < xmin) {
      xgamma = x*exp(logF[0]);
      return;
    }

    double sum1 = 0.0, sum2 = 0.0;
    double xdiff, r;
    double yk = pow(x, kPow);
    for (size_t ipx = 0; ipx < nPoints; ipx++) {
      xdiff = yk - xk[ipx];
      if (xdiff == 0.0) {
        xgamma = x*exp(logF[ipx]);
        return;
      }
      r = w[ipx] / xdiff;
      sum1 += r * logF[ipx];
      sum2 += r;
    }
    xgamma = x*exp(sum1 / sum2);
    // cout << x << ' ' << xgamma << ' ' << sum1 << ' ' << sum2 << endl;
  }

private:

  // Kinematic variables and
  vector<double> xk, w, logF;
  double xmin, xmax, kPow;

  static const int nPoints = 50;

  vector<double> fluxPointsAnPL = {1.418e+01, 1.414e+01, 1.400e+01, 1.379e+01,
    1.351e+01, 1.316e+01, 1.276e+01, 1.232e+01, 1.185e+01, 1.135e+01, 1.083e+01,
    1.028e+01, 9.703e+00, 9.094e+00, 8.443e+00, 7.737e+00, 6.958e+00, 6.085e+00,
    5.091e+00, 3.947e+00, 2.619e+00, 1.069e+00, -7.406e-01, -2.850e+00,
    -5.298e+00, -8.122e+00, -1.135e+01, -1.502e+01, -1.915e+01, -2.374e+01,
    -2.880e+01, -3.432e+01, -4.028e+01, -4.663e+01, -5.332e+01, -6.030e+01,
    -6.748e+01, -7.477e+01, -8.207e+01, -8.928e+01, -9.627e+01, -1.029e+02,
    -1.091e+02, -1.148e+02, -1.198e+02, -1.240e+02, -1.274e+02, -1.298e+02,
    -1.313e+02, -1.318e+02};

  vector<double> fluxPoints0nPL = {1.406e+01, 1.401e+01, 1.388e+01, 1.366e+01,
    1.337e+01, 1.301e+01, 1.260e+01, 1.214e+01, 1.165e+01, 1.112e+01, 1.056e+01,
    9.966e+00, 9.337e+00, 8.665e+00, 7.941e+00, 7.153e+00, 6.285e+00, 5.319e+00,
    4.233e+00, 2.999e+00, 1.588e+00, -3.700e-02, -1.912e+00, -4.077e+00,
    -6.572e+00, -9.435e+00, -1.270e+01, -1.639e+01, -2.054e+01, -2.515e+01,
    -3.022e+01, -3.576e+01, -4.172e+01, -4.808e+01, -5.478e+01, -6.177e+01,
    -6.895e+01, -7.624e+01, -8.355e+01, -9.076e+01, -9.775e+01, -1.044e+02,
    -1.106e+02, -1.163e+02, -1.213e+02, -1.255e+02, -1.289e+02, -1.313e+02,
    -1.328e+02, -1.333e+02};
};

// Petja's calculation for the flux using interpolation grids.
// Based on arXiv:2404.09731 [hep-ph].
// class Nucleus2gamma4 is for Woods-Saxon nuclear geometry.
class Nucleus2gamma4 : public PDF {

public:

  // Constructor.
  Nucleus2gamma4(int idBeamIn, double xminIn, double xmaxIn, double kPowIn,
                 bool emBreakUp) : PDF(idBeamIn) {

    // Initialize parameters.
    xk.resize(nPoints);
    w.resize(nPoints);
    logF.resize(nPoints);
    xmin = xminIn;
    xmax = xmaxIn;
    kPow = kPowIn;
    double xmin_k = pow(xmin, kPow);
    double xmax_k = pow(xmax, kPow);
    double d = 0;

    // Calculate Chebyshev interpolation points and read in table.
    for (size_t ipx = 0; ipx < nPoints; ipx++) {
      xk[ipx] = 0.5*(xmax_k + xmin_k)
        - 0.5*(xmax_k - xmin_k)*cos(M_PI*ipx/(nPoints-1.0));

      d = (ipx == 0 || ipx == nPoints - 1) ? 0.5 : 1.0;
      w[ipx] = (ipx % 2 == 0) ? d : -d;

      // Use globally defined hard-coded tables with or without EM break-up.
      logF[ipx] = emBreakUp ? fluxPointsAnWS[ipx]
        : logF[ipx] = fluxPoints0nWS[ipx];
    }
  }

  // Update the photon flux.
  void xfUpdate(int , double x, double ) {

    // If below first interpolation point return flux at min value.
    if (x < xmin) {
      xgamma = x*exp(logF[0]);
      return;
    }

    double sum1 = 0.0, sum2 = 0.0;
    double xdiff, r;
    double yk = pow(x, kPow);
    for (size_t ipx = 0; ipx < nPoints; ipx++) {
      xdiff = yk - xk[ipx];
      if (xdiff == 0.0) {
        xgamma = x*exp(logF[ipx]);
        return;
      }
      r = w[ipx] / xdiff;
      sum1 += r * logF[ipx];
      sum2 += r;
    }
    xgamma = x*exp(sum1 / sum2);
    // cout << x << ' ' << xgamma << ' ' << sum1 << ' ' << sum2 << endl;
  }

private:

  // Kinematic variables and
  vector<double> xk, w, logF;
  double xmin, xmax, kPow;

  static const int nPoints = 50;
// Table for log-flux values in the WS approximation with sqrts_NN=5.360 TeV, Z=82, R_WS=6.49 fm, d_WS=0.54 fm, sigma_NN=90.8533 mb.

  vector<double> fluxPointsAnWS = {14.168090,14.123162,13.990598,13.776638,13.490429,
    13.142574,12.743716,12.303402,11.829331,11.326879,10.798871,10.245390,9.663662,
    9.047903,8.389208,7.675616,6.892413,6.022879,5.049490,3.955548,2.726722,
    1.352103,-0.175720,-1.861918,-3.712321,-5.739908,-7.975804,-10.456943,
    -12.981940,-14.856877,-16.521829,-18.327160,-20.003986,-21.713577,-23.334190,
    -24.980114,-26.550540,-28.114975,-29.594659,-31.040437,-32.378179,-33.594314,
    -34.699394,-35.748627,-36.676191,-37.274820,-37.656167,-37.957383,-38.170632, -38.248746};

  vector<double> fluxPoints0nWS = {14.057896,14.012032,13.876623,13.657788,13.364499,
    13.007023,12.595571,12.139106,11.644568,11.116434,10.556535,9.963898,9.335042,
    8.663611,7.941111,7.156604,6.297297,5.349064,4.297280,3.127944,1.828848,0.390397,
    -1.194331,-2.930446,-4.823649,-6.886301,-9.148639,-11.649330,-14.203275,-16.111924,
    -17.783572,-19.596313,-21.276477,-22.989838,-24.617454,-26.264226,-27.842640,-29.407102,
    -30.893967,-32.339942,-33.683740,-34.889393,-35.988859,-37.045041,-37.976464,-38.550395,
    -38.900976,-39.185219,-39.392348,-39.469227};
};



// The main program.

int main(int argc, char *argv[]) {


  int photonFluxOption = 3;
  //photonFluxOptions:
  // 1     PYTHIA8 default
  // 2     Point-like approximation, 0nAn
  // 3     Point-like approximation, AnAn
  // 4     Woods-Saxon, 0nAn
  // 5     Woods-Saxon, AnAn

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
  // Set the external photon flux for beam A.
  if (photonFluxOption == 1)  photonFlux = make_shared<Nucleus2gamma2>(2212);
  if ( photonFluxOption == 2 ) photonFlux = make_shared<Nucleus2gamma3>(2212, 1.e-4, 1., 0.25, false); //PL 0nAn
  if ( photonFluxOption == 3 ) photonFlux = make_shared<Nucleus2gamma3>(2212, 1.e-4, 1., 0.25, true); //PL AnAn
  if ( photonFluxOption == 4 ) photonFlux = make_shared<Nucleus2gamma4>(2212, 1.e-4, 1., 0.25, false); //WS 0nAn
  if ( photonFluxOption == 5 ) photonFlux = make_shared<Nucleus2gamma4>(2212, 1.e-4, 1., 0.25, true); //WS AnAn
  pythia.setPhotonFluxPtr(photonFlux, 0);

  // Make sure that flux in the region where defined, at 5.36 TeV W = 54 GeV
  // corresponds to 10e-4 in y; used only for Petja's flux parametrization
  if (photonFluxOption != 1 )  pythia.readString("Photon:Wmin = 54."); //not needed for pythia8's default
  

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
  // Number of events.
  int nEvent = 200000;
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
    for (int i = 0; i < pythia.event.size(); ++i) {
      if (pythia.event[i].idAbs() == idD0) {
        // Store the pT value for the selected rapidity interval.
        double pT = pythia.event[i].pT();
        double y = pythia.event[i].y();
        if (y > yMin && y < yMax)
          pTD0.fill(pT, weight);
        hUnnormCrossSection->Fill(pT, y);
        // int iptbin = hUnnormCrossSection->GetXaxis()->FindBin(pT);
        // int iybin = hUnnormCrossSection->GetYaxis()->FindBin(y);
        //  std::cout << "iptbin = " << iptbin << " iybin = " << iybin <<
        //  std::endl; std::cout << "photon energy = " <<
        //  pythia.event[photonIndex].e() << std::endl;
        /*
        double energyph = 0.;
        if (photonIndex > 0 && iptbin > 0 && iptbin < nPtBins + 1 &&
            iybin > 0 && iybin < nYBins + 1) {
          energyph = pythia.event[photonIndex].e();
          hphotonEnergy[iptbin - 1][iybin - 1]->Fill(energyph);
        }
        */
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
  fout->Close();
  delete fout;
  // Done.
}
