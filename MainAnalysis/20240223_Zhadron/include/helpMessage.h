#include <iostream>
#include <string>

//============================================================//
// Print usage
//============================================================//
int printHelpMessage(int argc, char *argv[]) {

    // Check if --help is present in the command-line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help") {
           std::cout << "Usage: " << argv[0] << " [OPTIONS]\n"
              << "Options:\n"
              << "  --MinZPT <value>       Minimum Z particle transverse momentum threshold for event selection.\n"
              << "  --MaxZPT <value>       Maximum Z particle transverse momentum threshold for event selection.\n"
              << "  --MinZY <value>        Minimum Z particle rapidity threshold for event selection.\n"
              << "  --MaxZY <value>        Maximum Z particle rapidity threshold for event selection.\n"
              << "  --MinTrackPT <value>   Minimum track transverse momentum threshold for track selection.\n"
              << "  --MaxTrackPT <value>   Maximum track transverse momentum threshold for track selection.\n"
              << "  --MinHiBin <value>     Minimum hiBin value for event selection.\n"
              << "  --MaxHiBin <value>     Maximum hiBin value for event selection.\n"
              << "  --Input <path>         Path to the input ROOT file.\n"
              << "  --MixFile <path>       Path to the mix ROOT file.\n"
              << "  --Output <path>        Path to the output ROOT file.\n"
              << "  --IsSelfMixing         Flag to enable/disable self-mixing.\n"
              << "  --IsGenZ               Flag to use generated level Z information.\n"
              << "  --IsData               Flag to specify if the input is real data.\n"
              << "  --IsPP                 Flag for Proton-Proton collision data.\n"
              << "  --IsJewel              Flag for Jewel analysis.\n"
              << "  --IsMuTagged           Flag for doing Z decay muon matching.\n"
              << "  --IsPUReject           Flag to reject PU sample for systematics.\n"
              << "  --Fraction <value>     Fraction of event to be processed.\n"
              << "  --nThread <value>      Number of threads for parallel processing.\n"
              << "  --nChunk <value>       Specifies which chunk of the data to process.\n"
              << "  --nMix <value>         Number of mixed events to be used.\n"
              << "  --IsHiBinUp            Flag to use the hiBinUp value for event selection.\n"
              << "  --IsHiBinDown          Flag to use the hiBinDown value for event selection.\n\n"
              << "  --Shift                Shift of sumHF in MB matching\n\n"
              << "Example:\n"
              << "  " << argv[0] << " --Input sample/HISingleMuon.root --Output output.root --MinZPT 20 --MaxZPT 100\n";
           return 1;
        }
    }
    return 0;  
}


