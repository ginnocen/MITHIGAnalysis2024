//============================================================//
// Define analysis parameters
//============================================================//
class Parameters {
public:
    Parameters( float MinZPT, float MaxZPT, float MinTrackPT, float MaxTrackPT, int MinHiBin = 0, int MaxHiBin = 200, bool mix = false, float scaleFactor = 1.0, float nMix = 1)
    : MinZPT(MinZPT), MaxZPT(MaxZPT), MinTrackPT(MinTrackPT), MaxTrackPT(MaxTrackPT), MinHiBin(MinHiBin), MaxHiBin(MaxHiBin), mix(mix), scaleFactor(scaleFactor), nMix(nMix) {}
    string input;          // Input file name
    string output;         // Output file name
    string mixFile;        // Mix File name
    float MinZPT;          // Lower limit of Z pt
    float MaxZPT;          // Upper limit of Z pt
    float MinZY;          // Lower limit of Z rapidity
    float MaxZY;          // Upper limit of Z rapidity
    float MinTrackPT;      // Lower limit of track pt
    float MaxTrackPT;      // Upper limit of track pt
    float scaleFactor;     // Scale factor
    float shift;           // shift in sumHF when doing mb matching
    int MinHiBin;          // Lower limit of hiBin
    int MaxHiBin;          // Upper limit of hiBin
    int nThread;           // Number of Threads
    int nChunk;            // Process the Nth chunk
    bool mix;              // Mix flag
    int nMix;              // Number of mixed events
    TH1D *hShift;
    bool isSelfMixing;     // isSelfMixing flag
    bool isGenZ;           // isGenZ flag
    bool isMuTagged;       // Flag to enable/disable muon tagging requirement
    bool isPUReject;       // Flag to reject PU sample for systemaitcs.
    bool isHiBinUp;        // Flag to do systematics with HiBinUp
    bool isHiBinDown;      // Flag to do systematics with HiBinDown
    bool isPP;             // Flag to check if this is a PP analysis
    bool isJewel;             // Flag to check if this is a Jewel analysis
    int ExtraZWeight;
    bool includeHole;      // Flag to see if we include hole particles (negative trackweight particle)

   void printParameters() const {
       cout << "Input file: " << input << endl;
       cout << "Output file: " << output << endl;
       cout << "Mix File: " << mixFile << endl;
       cout << "MinZPT: " << MinZPT << " GeV/c" << endl;
       cout << "MaxZPT: " << MaxZPT << " GeV/c" << endl;
       cout << "MinZY: " << MinZY << "" << endl;
       cout << "MaxZY: " << MaxZY << "" << endl;
       cout << "MinTrackPT: " << MinTrackPT << " GeV/c" << endl;
       cout << "MaxTrackPT: " << MaxTrackPT << " GeV/c" << endl;
       cout << "isSelfMixing: " << (isSelfMixing ? "true" : "false") << endl;
       cout << "isGenZ: " << (isGenZ ? "true" : "false") << endl;
       cout << "isJewel: " << (isJewel ? "true" : "false") << endl;
       cout << "isPP: " << (isPP ? "true" : "false") << endl;
       cout << "Scale factor: " << scaleFactor << endl;
       cout << "SumHF shift: " << shift << endl;
       cout << "MinHiBin: " << MinHiBin << endl;
       cout << "MaxHiBin: " << MaxHiBin << endl;
       cout << "Number of Threads: " << nThread << endl;
       cout << "Process the Nth chunk: " << nChunk << endl;
       cout << "Mix flag: " << (mix ? "true" : "false") << endl;
       cout << "Number of mixed events: " << nMix << endl;
       cout << "Muon Tagging Enabled: " << (isMuTagged ? "true" : "false") << endl;
       cout << "PU rejection: " << (isPUReject ? "true" : "false") << endl;
       if (mix) cout << "Event mixing!" << endl;
   }
};

void saveParametersToHistograms(const Parameters& par, TFile* outf) {
    outf->cd();  // Navigate to the output file directory
    outf->mkdir("par"); // Create a directory named "par"
    outf->cd("par"); // Change to the "par" directory

    // Create and fill histograms for each parameter
    TH1D* hMinZPT = new TH1D("parMinZPT", "parMinZPT", 1, 0, 1);
    hMinZPT->SetBinContent(1, par.MinZPT);
    
    TH1D* hMaxZPT = new TH1D("parMaxZPT", "parMaxZPT", 1, 0, 1);
    hMaxZPT->SetBinContent(1, par.MaxZPT);
    
    TH1D* hMinTrackPT = new TH1D("parMinTrackPT", "parMinTrackPT", 1, 0, 1);
    hMinTrackPT->SetBinContent(1, par.MinTrackPT);
    
    TH1D* hMaxTrackPT = new TH1D("parMaxTrackPT", "parMaxTrackPT", 1, 0, 1);
    hMaxTrackPT->SetBinContent(1, par.MaxTrackPT);
    
    TH1D* hMinHiBin = new TH1D("parMinHiBin", "parMinHiBin", 1, 0, 1);
    hMinHiBin->SetBinContent(1, par.MinHiBin);
    
    TH1D* hMaxHiBin = new TH1D("parMaxHiBin", "parMaxHiBin", 1, 0, 1);
    hMaxHiBin->SetBinContent(1, par.MaxHiBin);
    
    TH1D* hMix = new TH1D("parMix", "parMix", 1, 0, 1);
    hMix->SetBinContent(1, par.mix);
    
    TH1D* hScaleFactor = new TH1D("parScaleFactor", "parScaleFactor", 1, 0, 1);
    hScaleFactor->SetBinContent(1, par.scaleFactor);
    
    TH1D* hShift = new TH1D("parShift", "parShift", 1, 0, 1);
    hShift->SetBinContent(1, par.shift);
    
    TH1D* hNThread = new TH1D("parNThread", "parNThread", 1, 0, 1);
    hNThread->SetBinContent(1, par.nThread);
    
    TH1D* hNChunk = new TH1D("parNChunk", "parNChunk", 1, 0, 1);
    hNChunk->SetBinContent(1, par.nChunk);
    
    TH1D* hNMix = new TH1D("parNMix", "parNMix", 1, 0, 1);
    hNMix->SetBinContent(1, par.nMix);
    
    TH1D* hIsSelfMixing = new TH1D("parIsSelfMixing", "parIsSelfMixing", 1, 0, 1);
    hIsSelfMixing->SetBinContent(1, par.isSelfMixing);
    
    TH1D* hIsGenZ = new TH1D("parIsGenZ", "parIsGenZ", 1, 0, 1);
    hIsGenZ->SetBinContent(1, par.isGenZ);
    
    TH1D* hIsMuTagged = new TH1D("parIsMuTagged", "parIsMuTagged", 1, 0, 1);
    hIsMuTagged->SetBinContent(1, par.isMuTagged);
    
    TH1D* hIsPUReject = new TH1D("parIsPUReject", "parIsPUReject", 1, 0, 1);
    hIsPUReject->SetBinContent(1, par.isPUReject);
    
    TH1D* hIsHiBinUp = new TH1D("parIsHiBinUp", "parIsHiBinUp", 1, 0, 1);
    hIsHiBinUp->SetBinContent(1, par.isHiBinUp);
    
    TH1D* hIsHiBinDown = new TH1D("parIsHiBinDown", "parIsHiBinDown", 1, 0, 1);
    hIsHiBinDown->SetBinContent(1, par.isHiBinDown);
    
    TH1D* hIsPP = new TH1D("parIsPP", "parIsPP", 1, 0, 1);
    hIsPP->SetBinContent(1, par.isPP);
    
    TH1D* hMinZY = new TH1D("parMinZY", "parMinZY", 1, 0, 1);
    hMinZY->SetBinContent(1, par.MinZY);
    
    TH1D* hMaxZY = new TH1D("parMaxZY", "parMaxZY", 1, 0, 1);
    hMaxZY->SetBinContent(1, par.MaxZY);
    
    // Write histograms to the output file
    hMinZPT->Write();
    hMaxZPT->Write();
    hMinTrackPT->Write();
    hMaxTrackPT->Write();
    hMinHiBin->Write();
    hMaxHiBin->Write();
    hMix->Write();
    hScaleFactor->Write();
    hShift->Write();
    hNThread->Write();
    hNChunk->Write();
    hNMix->Write();
    hIsSelfMixing->Write();
    hIsGenZ->Write();
    hIsMuTagged->Write();
    hIsPUReject->Write();
    hIsHiBinUp->Write();
    hIsHiBinDown->Write();
    hIsPP->Write();
    hMinZY->Write();
    hMaxZY->Write();

    // Clean up
    delete hMinZPT;
    delete hMaxZPT;
    delete hMinTrackPT;
    delete hMaxTrackPT;
    delete hMinHiBin;
    delete hMaxHiBin;
    delete hMix;
    delete hScaleFactor;
    delete hShift;
    delete hNThread;
    delete hNChunk;
    delete hNMix;
    delete hIsSelfMixing;
    delete hIsGenZ;
    delete hIsMuTagged;
    delete hIsPUReject;
    delete hIsHiBinUp;
    delete hIsHiBinDown;
    delete hIsPP;
    delete hMinZY;
    delete hMaxZY;
}