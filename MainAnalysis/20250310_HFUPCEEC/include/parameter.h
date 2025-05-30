//============================================================//
// Define analysis parameters
//============================================================//
class Parameters {
public:
    Parameters(float MinTrackPT, float MaxTrackPT, float MinTrackY, float MaxTrackY, float MinPtSum, float MaxPtSum, float MinMtSum, float MaxMtSum, bool UsePtSumHardScale,  bool IsGammaN, int TriggerChoice, bool IsData, float scaleFactor = 1.0)
	:  MinTrackPT(MinTrackPT), MaxTrackPT(MaxTrackPT), MinTrackY(MinTrackY), MaxTrackY(MaxTrackY),  MinPtSum(MinPtSum), MaxPtSum(MaxPtSum),MinMtSum(MinMtSum), MaxMtSum(MaxMtSum), UsePtSumHardScale(UsePtSumHardScale), IsGammaN(IsGammaN), TriggerChoice(TriggerChoice), IsData(IsData), scaleFactor(scaleFactor) {}
    Parameters() {}
   string input;          // Input file name
   string output;         // Output file name
   float MinTrackPT;      // Lower limit of Track pt
   float MaxTrackPT;      // Upper limit of Track pt
   float MinTrackY;       // Lower limit of Track rapidity
   float MaxTrackY;       // Upper limit of Track rapidity
   float MinPtSum;         // lower limit on the visible energy 
   float MaxPtSum;         // upper  limit on the visible energy
   float MinMtSum;         // lower limit on the visible mass
   float MaxMtSum;         // upper limit on the visible mass
   bool UsePtSumHardScale; // select whether or not to use the visible energy to set the hard scale
   bool IsGammaN;         // GammaN analysis (or NGamma)
   int TriggerChoice;     // 0 = no trigger sel, 1 = isL1ZDCOr, 2 = isL1ZDCXORJet8
   bool IsData;           // Data or MC
   float scaleFactor;     // Scale factor
   int nThread;           // Number of Threads
   int nChunk;            // Process the Nth chunk
   void printParameters() const {
       cout << "Input file: " << input << endl;
       cout << "Output file: " << output << endl;
       cout << "MinTrackPT: " << MinTrackPT << endl;
       cout << "MaxTraackPT: " << MaxTrackPT << endl;
       cout << "MinTrackY: " << MinTrackY << endl;
       cout << "MaxTrackY: " << MaxTrackY << endl;
       cout << "MinPtSum: " << MinPtSum << endl;
       cout << "MaxPtSum: " << MaxPtSum << endl;
       cout << "MinMtSum: " << MinMtSum << endl;
       cout << "MaxMtSum: " << MaxMtSum << endl;
       cout << "UsePtSumHardScale: " << UsePtSumHardScale << endl; 
       cout << "IsGammaN: " << IsGammaN << endl;
       cout << "TriggerChoice: " << TriggerChoice << endl;
       cout << "IsData: " << IsData << endl;
       cout << "Scale factor: " << scaleFactor << endl;
       cout << "Number of Threads: " << nThread << endl;
       cout << "Process the Nth chunk: " << nChunk << endl;
   }
};

void saveParametersToHistograms(const Parameters& par, TFile* outf) {
    outf->cd();  // Navigate to the output file directory
    outf->mkdir("par"); // Create a directory named "par"
    outf->cd("par"); // Change to the "par" directory

    // Create and fill histograms for each parameter
    TH1D* hMinTrackPT = new TH1D("parMinTrackPT", "parMinTrackPT", 1, 0, 1);
    hMinTrackPT->SetBinContent(1, par.MinTrackPT);
    TH1D* hMaxTrackPT = new TH1D("parMaxTrackPT", "parMaxTrackPT", 1, 0, 1);
    hMaxTrackPT->SetBinContent(1, par.MaxTrackPT);
    TH1D* hMinTrackY = new TH1D("parMinTrackY", "parMinTrackY", 1, 0, 1);
    hMinTrackY->SetBinContent(1, par.MinTrackY);
    TH1D* hMaxTrackY = new TH1D("parMaxTrackY", "parMaxTrackY", 1, 0, 1);
    hMaxTrackY->SetBinContent(1, par.MaxTrackY);
    TH1D* hMinPtSum = new TH1D("parMinPtSum", "parMinPtSum", 1, 0, 1);
    hMinPtSum->SetBinContent(1, par.MinPtSum);
    TH1D* hMaxPtSum = new TH1D("parMaxPtSum", "parMaxPtSum", 1, 0, 1);
    hMaxPtSum->SetBinContent(1, par.MaxPtSum);
    TH1D* hMinMtSum = new TH1D("parMinMtSum", "parMinMtSum", 1, 0, 1);
    hMinMtSum->SetBinContent(1, par.MinPtSum);
    TH1D* hMaxMtSum = new TH1D("parMaxMtSum", "parMaxMtSum", 1, 0, 1);
    hMaxMtSum->SetBinContent(1, par.MaxMtSum);
    TH1D* hUsePtSumHardScale = new TH1D("parUsePtSumHardScale", "parUsePtSumHardScale", 1, 0, 1);
    hUsePtSumHardScale->SetBinContent(1, par.UsePtSumHardScale);
    TH1D* hIsGammaN = new TH1D("parIsGammaN", "parIsGammaN", 1, 0, 1);
    hIsGammaN->SetBinContent(1, par.IsGammaN);
    TH1D* hTriggerChoice = new TH1D("parTriggerChoice", "parTriggerChoice", 1, 0, 1);
    hTriggerChoice->SetBinContent(1, par.TriggerChoice);
    TH1D* hIsData = new TH1D("parIsData", "parIsData", 1, 0, 1);
    hIsData->SetBinContent(1, par.IsData);
    TH1D* hScaleFactor = new TH1D("parScaleFactor", "parScaleFactor", 1, 0, 1);
    hScaleFactor->SetBinContent(1, par.scaleFactor);

    // Write histograms to the output file
    hMinTrackPT->Write();
    hMaxTrackPT->Write();
    hMinTrackY->Write();
    hMaxTrackY->Write();
    hMinPtSum->Write(); 
    hMaxPtSum->Write(); 
    hMinMtSum->Write(); 
    hMaxMtSum->Write(); 
    hUsePtSumHardScale->Write(); 
    hIsGammaN->Write();
    hTriggerChoice->Write();
    hIsData->Write();
    hScaleFactor->Write();
    // Clean up
    delete hMinTrackPT;
    delete hMaxTrackPT;
    delete hMinTrackY;
    delete hMaxTrackY;
    delete hMinPtSum; 
    delete hMaxPtSum; 
    delete hMinMtSum; 
    delete hMaxMtSum; 
    delete hUsePtSumHardScale; 
    delete hIsGammaN;
    delete hTriggerChoice;
    delete hIsData;
    delete hScaleFactor;
}
