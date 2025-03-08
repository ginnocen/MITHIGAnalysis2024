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



// Recursive function to collect final state particles
void collectFinalStateParticles(const Event& event, int index, std::set<int>& visited, std::vector<int>& finalParticles) {
    // Prevent infinite loops
    if (visited.find(index) != visited.end()) return;
    visited.insert(index);

    // Get the particle
    const Particle& p = event[index];

    // Check if it has daughters (if not, it's a final state particle)
    int d1 = p.daughter1();
    int d2 = p.daughter2();

    if (d1 == 0) {
        finalParticles.push_back(index);  // Store final state particle index
        return;
    }

    // Recur for each daughter
    for (int i = d1; i <= d2; ++i) {
        collectFinalStateParticles(event, i, visited, finalParticles);
    }
}

int findCharmHadronInDecay(const Event& event, int index, std::set<int>& visited) {
    // Prevent infinite loops
    if (visited.find(index) != visited.end()) return 0;
    visited.insert(index);

    // Get the particle
    const Particle& p = event[index];

    // Check if the particle has daughters (otherwise, it's a final-state particle)
    int d1 = p.daughter1();
    int d2 = p.daughter2();

    if (d1 == 0) return 0;  // No daughters → cannot decay further

    // Loop over all daughters
    for (int i = d1; i <= d2; ++i) {
        int pdgId = abs(event[i].id());  // Use abs() to handle antiparticles

        // Check if this is a D meson (400-499) or a charm baryon (4000-4999)
        if ((pdgId >= 400 && pdgId <= 499) || (pdgId >= 4000 && pdgId <= 4999)) {
            std::cout << "Charm hadron found: PDG ID = " << pdgId 
                      << " at index " << i << std::endl;
            return pdgId;  // Return the PDG ID of the found charm hadron
        }

        // Recursively check if this daughter decays into a charm hadron
        int result = findCharmHadronInDecay(event, i, visited);
        if (result != 0) {
            return result;  // If found deeper in the decay chain, return it
        }
    }

    return 0;  // No charm hadron found in this decay chain
}

int getIntermediateCharmHadron(const Event& event, int particleIndex) {
    std::set<int> visited;
    return findCharmHadronInDecay(event, particleIndex, visited);
}
// Function to check for opposite-sign muon pairs in finalParticles
bool hasOppositeSignMuons(const Event& event, const std::vector<int>& finalParticles) {
    bool hasMuon = false;
    bool hasAntiMuon = false;

    // Loop through final state particles
    for (int idx : finalParticles) {
        int pid = event[idx].id();
        if (pid == 13) hasAntiMuon = true;  // Found mu+
        if (pid == -13) hasMuon = true;    // Found mu-

        // If both exist, return true
        if (hasMuon && hasAntiMuon) return true;
    }
    return false;
}

int main(int argc, char *argv[]) {

  Int_t nEvents = 1000;
  Double_t eCM = 13000.0;
  Pythia pythia;
  pythia.readString(Form("Beams:eCM = %g", eCM));

  // Turn on HardQCD processes that produce b-bbar
  pythia.readString("HardQCD:gg2bbbar = on");
  pythia.readString("HardQCD:qqbar2bbbar = on");

  pythia.readString("PhaseSpace:pTHatMin = 5");
  // run pp collisions
  pythia.readString("Beams:idA = 2212");
  pythia.readString("Beams:idB = 2212");
  pythia.readString("tune:pp = 14");
  //turn on initial and final state radiation
  pythia.readString("PartonLevel:ISR = off");
  pythia.readString("PartonLevel:FSR = off");
  // Initialize the generator
  //switch off decays
  //pythia.readString("HadronLevel:Decay = off");
  //pythia.readString("511:mayDecay = off"); // B0
  //pythia.readString("521:mayDecay = off"); // B+
  //pythia.readString("531:mayDecay = off"); // Bs
  pythia.init();
  int countergood = 0;
  // Event loop
  for (int iEvent = 0; iEvent < nEvents; ++iEvent) {
    
    // Generate the next event; skip if it fails
    if (!pythia.next()) continue;

    // Loop over all particles
    for (int i = 0; i < pythia.event.size(); ++i) {

      // Check if this is a B0 or B+ (PDG IDs: B0=511, B+ =521).
      // If you'd like B- or Bbar0 as well, you can check abs(id) == 511 or 521
      int pid = pythia.event[i].id();
      std::set<int> visited; // Set to track visited particles
      std::vector<int> finalParticles; // List to store final state particles

      if (pid == 511 || pid == 521) {
        // Print a header for clarity
        collectFinalStateParticles(pythia.event, i, visited, finalParticles);
        if (!hasOppositeSignMuons(pythia.event, finalParticles)) continue;
        countergood++;
        if (countergood > 4) break;
        std::cout << "------------------------------------------------------------------------------------------------" << std::endl;
        std::cout << "------------------------------------------------------------------------------------------------" << std::endl;
        std::cout << "Final State Particles the decay of Particle " << i << " (ID: " << pid << ")" << std::endl;
        for (int idx : finalParticles) {
            int pid = pythia.event[idx].id();
            std::cout << "Particle " << idx << " (" << pythia.particleData.name(pid) 
                      << ", ID: " << pid << ")" << std::endl;
        }
       int charmHadronPdg = getIntermediateCharmHadron(pythia.event, i);
        if (charmHadronPdg != 0) {
            std::cout << "Event " << iEvent << ": Particle at index " << i 
                      << " decays into a charm hadron with PDG ID " 
                      << charmHadronPdg << std::endl;
        }

        visited.clear();
        finalParticles.clear();
        //pythia.event.list();

    }
      }
    }
  }
