#include <TFile.h>
#include <TTree.h>
#include <vector>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    // Check if the correct number of arguments is provided
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file.root> <output_file.root>" << std::endl;
        return 1;
    }

    // Parse input and output file names
    std::string inputFileName = argv[1];
    std::string outputFileName = argv[2];

    // Open the input file and retrieve the TTree
    TFile *inputFile = TFile::Open(inputFileName.c_str(), "READ");
    if (!inputFile || inputFile->IsZombie()) {
        std::cerr << "Error opening input file: " << inputFileName << std::endl;
        return 1;
    }

    TTree *tree = (TTree *)inputFile->Get("Tree");
    if (!tree) {
        std::cerr << "Error retrieving TTree from file: " << inputFileName << std::endl;
        inputFile->Close();
        return 1;
    }

    // Prepare to read the "IsMuMuTagged" branch
    std::vector<bool> *IsMuMuTagged = nullptr;
    tree->SetBranchAddress("IsMuMuTagged", &IsMuMuTagged);

    // Create a new file and TTree for filtered events
    TFile *outputFile = TFile::Open(outputFileName.c_str(), "RECREATE");
    TTree *filteredTree = tree->CloneTree(0); // Clone the structure of the original TTree

    // Loop over all entries and apply the filter
    Long64_t nEntries = tree->GetEntries();
    Long64_t nFiltered = 0; // Counter for filtered events
    for (Long64_t i = 0; i < nEntries; ++i) {
        tree->GetEntry(i);

        // Check if any jet has IsMuMuTagged == true
        bool hasTaggedJet = false;
        if (IsMuMuTagged) {
            for (bool tag : *IsMuMuTagged) {
                if (tag) {
                    hasTaggedJet = true;
                    break;
                }
            }
        }

        // Keep the event if it has a tagged jet
        if (hasTaggedJet) {
            filteredTree->Fill();
            ++nFiltered;
        }
    }

    // Write the filtered TTree to the output file
    outputFile->cd();
    filteredTree->Write();
    outputFile->Close();

    // Clean up
    inputFile->Close();
    delete inputFile;
    delete outputFile;

    std::cout << "Filtered " << nFiltered << " events with at least one tagged jet." << std::endl;
    std::cout << "Filtered events saved to: " << outputFileName << std::endl;

    return 0;
}
