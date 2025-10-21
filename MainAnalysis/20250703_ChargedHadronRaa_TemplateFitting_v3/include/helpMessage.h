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
                << "  --MinTrackPt <value>    Minimum track transverse momentum threshold for track selection.\n"
                << "  --Input <path>          Path to the input ROOT file.\n"
                << "  --IsData <value>        Flag to specify if the input is real data.\n"
                << "  --Output <path>         Path to the output ROOT file.\n"
                << "  --TriggerChoice <value> Choice of trigger for event selection.\n"
                << "  --ScaleFactor <value>   Scale factor for normalization.\n"
                << "Example:\n"
                << "  " << argv[0]
                << " --Input sample.root --Output output.root --IsData true --MinTrackPt 0.5 --TriggerChoice 0 "
                   "--ScaleFactor 1.0 --IsPP true \n";
      return 1;
    }
  }
  return 0;
}
