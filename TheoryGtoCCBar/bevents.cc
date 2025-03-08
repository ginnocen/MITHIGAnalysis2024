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
#include <algorithm>
#include <cstdio> 
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <valarray>
#include <vector>
#include <set>

using namespace Pythia8;

//------------------------------------------------------------------------------
// Recursive function to collect final-state particles from a given particle index
void collectFinalStateParticles(const Event &event, int index, std::set<int> &visited,
                                std::vector<int> &finalParticles) {
  // Prevent infinite loops
  if (visited.find(index) != visited.end()) return;
  visited.insert(index);

  // Get the particle
  const Particle &p = event[index];

  // Check if it has daughters (if not, it's final-state)
  int d1 = p.daughter1();
  int d2 = p.daughter2();

  if (d1 == 0) {
    finalParticles.push_back(index); // Store final-state particle index
    return;
  }

  // Recur for each daughter
  for (int i = d1; i <= d2; ++i) {
    collectFinalStateParticles(event, i, visited, finalParticles);
  }
}

//------------------------------------------------------------------------------
// Helper function to get final-state particles for a given particle index
// Ensures we start with a fresh visited set each time
std::vector<int> getFinalStateParticles(const Event &event, int particleIndex) {
  std::set<int> visited;
  std::vector<int> finalParticles;
  collectFinalStateParticles(event, particleIndex, visited, finalParticles);
  return finalParticles;
}

//------------------------------------------------------------------------------
// Recursive function to find a charm hadron in the decay chain
int findCharmHadronInDecay(const Event &event, int index, std::set<int> &visited) {
  // Prevent infinite loops
  if (visited.find(index) != visited.end()) return 0;
  visited.insert(index);

  // Get the particle
  const Particle &p = event[index];

  // Check if the particle has daughters (otherwise, it's final-state)
  int d1 = p.daughter1();
  int d2 = p.daughter2();

  if (d1 == 0) return 0; // no daughters => cannot decay further

  // Loop over daughters
  for (int i = d1; i <= d2; ++i) {
    int pdgId = std::abs(event[i].id()); // abs() to handle antiparticles
    // Check if this is a D meson (400-499) or a charm baryon (4000-4999)
    if ((pdgId >= 400 && pdgId <= 499) || (pdgId >= 4000 && pdgId <= 4999)) {
      std::cout << "Charm hadron found: PDG ID = " << pdgId 
                << " at index " << i << std::endl;
      return pdgId; 
    }
    // Recursively check for charm hadron
    int result = findCharmHadronInDecay(event, i, visited);
    if (result != 0) return result;
  }

  return 0;
}

//------------------------------------------------------------------------------
// Wrapper to get a charm hadron (if any) from a given particle index
int getIntermediateCharmHadron(const Event &event, int particleIndex) {
  std::set<int> visited;
  return findCharmHadronInDecay(event, particleIndex, visited);
}

//------------------------------------------------------------------------------
// Check for opposite-sign muon pair in final state
bool hasOppositeSignMuons(const Event &event, const std::vector<int> &finalParticles) {
  bool hasMuMinus = false; // PDG 13
  bool hasMuPlus  = false; // PDG -13

  for (int idx : finalParticles) {
    int pid = event[idx].id();
    if      (pid ==  13) hasMuMinus = true; 
    else if (pid == -13) hasMuPlus  = true; 

    if (hasMuMinus && hasMuPlus) return true;
  }
  return false;
}

//------------------------------------------------------------------------------
// Main
int main(int argc, char *argv[]) {

  // Basic settings
  Int_t nEvents = 1000;
  Double_t eCM  = 13000.0;
  Pythia pythia;

  // Pythia settings
  pythia.readString(Form("Beams:eCM = %g", eCM));
  pythia.readString("Beams:idA = 2212");
  pythia.readString("Beams:idB = 2212");
  pythia.readString("Tune:pp = 14");
  pythia.readString("PhaseSpace:pTHatMin = 5");
  
  // Turn on HardQCD processes that produce b-bbar
  pythia.readString("HardQCD:gg2bbbar = on");
  pythia.readString("HardQCD:qqbar2bbbar = on");

  // If you want showers on, set these to 'on' instead of 'off':
  pythia.readString("PartonLevel:ISR = off");
  pythia.readString("PartonLevel:FSR = off");

  // Initialize
  pythia.init();

  int countergood = 0;

  // Event loop
  for (int iEvent = 0; iEvent < nEvents; ++iEvent) {

    // Generate the next event; skip if it fails
    if (!pythia.next()) continue;

    // Loop over all particles in this event
    for (int i = 0; i < pythia.event.size(); ++i) {

      // Check if this is a B0 or B+ (PDG IDs: 511 or 521)
      int pid = pythia.event[i].id();
      if (pid == 511 || pid == 521) {

        // Get final-state particles from B hadron
        std::vector<int> finalParticles = getFinalStateParticles(pythia.event, i);

        // Check if final state has an opposite-sign muon pair
        if (!hasOppositeSignMuons(pythia.event, finalParticles)) continue;

        ++countergood;

        // Optional: break if you only want the first 5 total across events
        if (countergood > 4) {
          std::cout << "Found at least 5 B-hadrons with opposite-sign muons; stopping.\n";
          break;
        }

        // Print results
        std::cout << "---------------------------------------------------------\n";
        std::cout << "Event #" << iEvent << ", found B index = " << i 
                  << " (PDG = " << pid << ") with opposite-sign muons.\n";

        std::cout << "Final State Particles from B hadron:\n";
        for (int idx : finalParticles) {
          int fpid = pythia.event[idx].id();
          std::cout << "  -> Index " << idx 
                    << " : " << pythia.particleData.name(fpid) 
                    << " (PDG ID = " << fpid << ")\n";
        }

        int charmHadronPdg = getIntermediateCharmHadron(pythia.event, i);
        if (charmHadronPdg != 0) {
          std::cout << "This B-hadron decays into charm hadron PDG ID = " 
                    << charmHadronPdg << std::endl;
        }

      } // end if (B hadron)
    }   // end loop over particles

    // If we found 5 or more, break out of the event loop too
    if (countergood > 4) break;
  } // end event loop

  return 0;
}
