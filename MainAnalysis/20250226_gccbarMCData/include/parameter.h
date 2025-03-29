//============================================================//
// Define analysis parameters
//============================================================//
class Parameters {
public:
    Parameters( float MinJetPT, int TriggerChoice, bool IsData, float scaleFactor = 1.0)
	: MinJetPT(MinJetPT), TriggerChoice(TriggerChoice), IsData(IsData), scaleFactor(scaleFactor) {}
    Parameters() {}
   string input;          // Input file name
   string output;         // Output file name
   float MinJetPT;        // Lower limit of jet pt
   int TriggerChoice;     // 0 = no trigger sel, 1 = isL1ZDCOr, 2 = isL1ZDCXORJet8
   bool IsData;           // Data or MC
   float scaleFactor;     // Scale factor
   int nThread;           // Number of Threads
   int nChunk;            // Process the Nth chunk
   void printParameters() const {
       cout << "Input file: " << input << endl;
       cout << "Output file: " << output << endl;
       cout << "MinJetPT: " << MinJetPT << endl;
       cout << "TriggerChoice: " << TriggerChoice << endl;
       cout << "IsData: " << IsData << endl;
       cout << "scaleFactor: " << scaleFactor << endl;
       cout << "nThread: " << nThread << endl;
       cout << "nChunk: " << nChunk << endl;
   }
};

void saveParametersToHistograms(const Parameters& par, TFile* outf) {
    outf->cd();  // Navigate to the output file directory
    outf->mkdir("par"); // Create a directory named "par"
    outf->cd("par"); // Change to the "par" directory

    // Create and fill histograms for each parameter
    TH1D* hMinJetPT = new TH1D("parMinJetPT", "parMinJetPT", 1, 0, 1);
    hMinJetPT->SetBinContent(1, par.MinJetPT);
    TH1D* hTriggerChoice = new TH1D("parTriggerChoice", "parTriggerChoice", 1, 0, 1);
    hTriggerChoice->SetBinContent(1, par.TriggerChoice);
    TH1D* hIsData = new TH1D("parIsData", "parIsData", 1, 0, 1);
    hIsData->SetBinContent(1, par.IsData);
    TH1D* hScaleFactor = new TH1D("parScaleFactor", "parScaleFactor", 1, 0, 1);
    hScaleFactor->SetBinContent(1, par.scaleFactor);
    TH1D* hNThread = new TH1D("parNThread", "parNThread", 1, 0, 1);
    hNThread->SetBinContent(1, par.nThread);
    TH1D* hNChunk = new TH1D("parNChunk", "parNChunk", 1, 0, 1);
    hNChunk->SetBinContent(1, par.nChunk);
    
    // Write histograms to the output file
    hMinJetPT->Write();
    hTriggerChoice->Write();
    hIsData->Write();
    hScaleFactor->Write();
    hNThread->Write();
    hNChunk->Write();

    delete hMinJetPT;
    delete hTriggerChoice;
    delete hIsData;
    delete hScaleFactor;
    delete hNThread;
    delete hNChunk;
}
