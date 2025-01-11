// main70.cc is a part of the PYTHIA event generator.
// Copyright (C) 2024 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL v2 or later, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// Authors: Ilkka Helenius <ilkka.m.helenius@jyu.fi>

// Keywords: photon beam; DIS; UPC; heavy ions; photoproduction

// Main program to demonstrate how to define a photon flux and use that
// to generate charged-particle pT spectra in photo-production processes.

#include "Pythia8/Pythia.h"

using namespace Pythia8;

// Photon flux from leptons, corresponds to internal Lepton2gamma.

class Lepton2gamma2 : public PDF {

public:

  // Constructor.
  Lepton2gamma2(int idBeamIn) : PDF(idBeamIn) {}

  // Update the photon flux.
  void xfUpdate(int , double x, double Q2) {
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
  void xfUpdate(int , double x, double ) {

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
    double intB = xi * bK1 * bK0 - 0.5 * pow2(xi) * ( pow2(bK1) - pow2(bK0) );
    xgamma = 2. * alphaEM * pow2(z) / M_PI * intB;
  }

};

// The main program.

int main() {

  // Generator.
  Pythia pythia;

  // Decrease the output.
  pythia.readString("Init:showChangedSettings = off");
  pythia.readString("Init:showChangedParticleData = off");
  pythia.readString("Next:numberCount = 10000");
  pythia.readString("Next:numberShowInfo = 0");
  pythia.readString("Next:numberShowProcess = 1");
  pythia.readString("Next:numberShowEvent = 3");

  // Pointer to externally defined photon flux.
  PDFPtr photonFlux = 0;

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
  int nBinsPT  = 10;

  int idD0 = 421;
  // double yMin = -2.;
  // double yMax =  2.;
  double yMin = -1.;
  double yMax = 0.;

  vector<double> yBins = {-2., -1., 0., 1., 2.};
  vector<double> pTBins = {2., 4., 8., 12.};
  int nyBins = yBins.size() - 1;
  int npTBins = pTBins.size() - 1;
  
  vector <vector<double> > dsigma(npTBins, vector<double> (nyBins,0) );
  // vector<vector<int>> vec(m, vector<int> (n, 0));

  // - pt ranges {2,4}, {4,8}, {8,12} GeV
  // - rapidity {-2,-1}, {-1, 0}, {0,1}, {1,2}.
    
  // Initialize the histograms.
  Hist pTD0("D0 pT distribution", nBinsPT, pTmin, pTmax);
  
  // Number of events.
  int nEvent = 10000000;
  
  // Begin event loop. Skip if fails.
  for (int iEvent = 0; iEvent < nEvent; ++iEvent) {

    // Generate next event.
    if (!pythia.next()) continue;

    // Event weight.
    double weight = pythia.info.weight();

    // Loop over event record.
    for (int i = 0; i < pythia.event.size(); ++i){
      if ( pythia.event[i].idAbs() == idD0 ) {

        // Store the pT value for the selected rapidity interval.
	double pT = pythia.event[i].pT();
	double y = pythia.event[i].y();
	if (y > yMin && y < yMax) pTD0.fill(pT, weight );

	for (int j = 0; j < npTBins; ++j){
	  for (int k = 0; k < nyBins; ++k){
	    if ( pT > pTBins[j] && pT < pTBins[j + 1]
		 && y > yBins[k] && y < yBins[k+1] ) {
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
  double pTbin     = (pTmax - pTmin) / (1. * nBinsPT);

  // Normalize to cross section, account for 208 target nucleons.
  pTD0 *= 208 * sigmaNorm / pTbin;

  cout << pTD0;

  cout << endl << "Double-differential cross section dsigma D0 / dpT dy in mb"
       << endl;
  cout << fixed << " Bins in pT and y | ";
  for (int k = 0; k < nyBins; ++k){
    cout << yBins[k] << ", " << yBins[k+1] << " | ";
  }
  cout << endl;
  for (int j = 0; j < npTBins; ++j){
    cout << fixed << pTBins[j] << ", " << pTBins[j+1] << " | ";
    for (int k = 0; k < nyBins; ++k){
      cout << scientific << dsigma[j][k] * 208 * sigmaNorm / (pTBins[j+1] - pTBins[j]) << " | ";
    }
    cout << endl;
  }
  cout << endl;

  // Done.
  return 0;
}
