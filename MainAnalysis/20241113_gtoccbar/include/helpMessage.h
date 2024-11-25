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
              << "  --MinJetPT <value>     Minimum jet transverse momentum.\n"
              << "  --MaxJetPT <value>     Maximum jet transverse momentum.\n"
              << "  --Input <path>         Path to the input ROOT file.\n"
              << "  --Output <path>        Path to the output ROOT file.\n"
              << "  --Fraction <value>     Fraction of event to be processed.\n"
              << "  --nThread <value>      Number of threads for parallel processing.\n"
              << "  --nChunk <value>       Specifies which chunk of the data to process.\n"
              << "Example:\n"
              << "  " << argv[0] << " --Input sample/HISingleMuon.root --Output output.root --MinJetPT 80 --Fraction 0.1 --nThread 4 --nChunk 0\n";
           return 1;
        }
    }
    return 0;  
}
