//============================================================//
// Define analysis parameters
//============================================================//
class Parameters {
public:
    Parameters( float MinDzeroPT, float MaxDzeroPT, float MinDzeroY, float MaxDzeroY, float MinJetPT, float MaxJetPT, float MinJetY, float MaxJetY, bool IsGammaN, int TriggerChoice, bool IsData, float scaleFactor = 1.0)
	: MinDzeroPT(MinDzeroPT), MaxDzeroPT(MaxDzeroPT), MinDzeroY(MinDzeroY), MaxDzeroY(MaxDzeroY), MinJetPT(MinJetPT), MaxJetPT(MaxJetPT), MinJetY(MinJetY), MaxJetY(MaxJetY), IsGammaN(IsGammaN), TriggerChoice(TriggerChoice), IsData(IsData), scaleFactor(scaleFactor) {}
    Parameters() {}
   string input;          // Input file name
   string output;         // Output file name
   float MinDzeroPT;      // lower limit of the Dzero pT
   float MaxDzeroPT;      // upper limit of the Dzero pT
   float MinDzeroY;       // lower limit of the Dzero y
   float MaxDzeroY;       // upper limit of the Dzero y
   float MinJetPT;        // Lower limit of Jet pt
   float MaxJetPT;        // Upper limit of Jet pt
   float MinJetY;         // Lower limit of Jet rapidity
   float MaxJetY;         // Upper limit of Jet rapidity
   bool IsGammaN;         // GammaN analysis (or NGamma)
   int TriggerChoice;     // 0 = no trigger sel, 1 = isL1ZDCOr, 2 = isL1ZDCXORJet8
   bool IsData;           // Data or MC
   float scaleFactor;     // Scale factor
   int nThread;           // Number of Threads
   int nChunk;            // Process the Nth chunk
   void printParameters() const {
       cout << "Input file: " << input << endl;
       cout << "Output file: " << output << endl;
       cout << "Min Dzero PT: " << MinDzeroPT << endl;
       cout << "Max Dzero PT: " << MaxDzeroPT << endl;
       cout << "Min Dzero Y: " << MinDzeroY << endl;
       cout << "Max Dzero Y: " << MaxDzeroY << endl;
       cout << "MinJetPT: " << MinJetPT << endl;
       cout << "MaxJetPT: " << MaxJetPT << endl;
       cout << "MinJetY: " << MinJetY << endl;
       cout << "MaxJetY: " << MaxJetY << endl;
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
    TH1D* hMinDzeroPT = new TH1D("parMinDzeroPT", "parMinDzeroPT", 1, 0, 1);
    hMinDzeroPT->SetBinContent(1, par.MinDzeroPT);
    TH1D* hMaxDzeroPT = new TH1D("parMaxDzeroPT", "parMaxDzeroPT", 1, 0, 1);
    hMaxDzeroPT->SetBinContent(1, par.MaxDzeroPT);
    TH1D* hMinDzeroY = new TH1D("parMinDzeroY", "parMinDzeroY", 1, 0, 1);
    hMinDzeroY->SetBinContent(1, par.MinDzeroY);
    TH1D* hMaxDzeroY = new TH1D("parMaxDzeroY", "parMaxDzeroY", 1, 0, 1);
    hMaxDzeroY->SetBinContent(1, par.MaxDzeroY);
    TH1D* hMinJetPT = new TH1D("parMinJetPT", "parMinJetPT", 1, 0, 1);
    hMinJetPT->SetBinContent(1, par.MinJetPT);
    TH1D* hMaxJetPT = new TH1D("parMaxJetPT", "parMaxJetPT", 1, 0, 1);
    hMaxJetPT->SetBinContent(1, par.MaxJetPT);
    TH1D* hMinJetY = new TH1D("parMinJetY", "parMinJetY", 1, 0, 1);
    hMinJetY->SetBinContent(1, par.MinJetY);
    TH1D* hMaxJetY = new TH1D("parMaxJetY", "parMaxJetY", 1, 0, 1);
    hMaxJetY->SetBinContent(1, par.MaxJetY);
    TH1D* hIsGammaN = new TH1D("parIsGammaN", "parIsGammaN", 1, 0, 1);
    hIsGammaN->SetBinContent(1, par.IsGammaN);
    TH1D* hTriggerChoice = new TH1D("parTriggerChoice", "parTriggerChoice", 1, 0, 1);
    hTriggerChoice->SetBinContent(1, par.TriggerChoice);
    TH1D* hIsData = new TH1D("parIsData", "parIsData", 1, 0, 1);
    hIsData->SetBinContent(1, par.IsData);
    TH1D* hScaleFactor = new TH1D("parScaleFactor", "parScaleFactor", 1, 0, 1);
    hScaleFactor->SetBinContent(1, par.scaleFactor);

    // Write histograms to the output file
    hMinDzeroPT->Write();
    hMaxDzeroPT->Write();
    hMinDzeroY->Write();
    hMaxDzeroY->Write();
    hMinJetPT->Write();
    hMaxJetPT->Write();
    hMinJetY->Write();
    hMaxJetY->Write();
    hIsGammaN->Write();
    hTriggerChoice->Write();
    hIsData->Write();
    hScaleFactor->Write();
    // Clean up
    delete hMinDzeroPT;
    delete hMaxDzeroPT;
    delete hMinDzeroY;
    delete hMaxDzeroY;
    delete hMinJetPT;
    delete hMaxJetPT;
    delete hMinJetY;
    delete hMaxJetY;
    delete hIsGammaN;
    delete hTriggerChoice;
    delete hIsData;
    delete hScaleFactor;
}
