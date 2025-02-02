//============================================================//
// Define analysis parameters
//============================================================//
class Parameters {
public:
    Parameters( float MinDzeroPT, float MaxDzeroPT, float MinDzeroY, float MaxDzeroY, bool IsGammaN, int TriggerChoice, bool IsData, float scaleFactor = 1.0,
                int in_DoSystRapGap = 0, int in_DoSystD = 0)
	: MinDzeroPT(MinDzeroPT), MaxDzeroPT(MaxDzeroPT), MinDzeroY(MinDzeroY), MaxDzeroY(MaxDzeroY), IsGammaN(IsGammaN), TriggerChoice(TriggerChoice), IsData(IsData), scaleFactor(scaleFactor)
    {
        if (in_DoSystRapGap > 9) {
            printf("[Warning] Using custom rapidity gap energy threshold!");
            DoSystRapGap = in_DoSystRapGap;
        }
        else if (in_DoSystRapGap!=0 && in_DoSystRapGap != 1 && in_DoSystRapGap != -1)
        {
            printf("[Error] Couldn't recognize the option DoSystRapGap=%d (should be 0 = nominal, 1 = tight, -1: loose). Exiting...\n", in_DoSystRapGap);
            exit(1);
        }
        else { DoSystRapGap = in_DoSystRapGap; }

        if (in_DoSystD!=0 && in_DoSystD != 1 && in_DoSystD != 2 \
                          && in_DoSystD != 3 && in_DoSystD != 4)
        {
            printf("[Error] Couldn't recognize the option DoSystD=%d (should be 0 = nominal, 1 = Dsvpv variation, 2: DtrkPt variation, 3: Dalpha variation, 4: Dchi2cl variation). Exiting...\n", in_DoSystD);
            exit(1);
        } else { DoSystD = in_DoSystD; }
    }
    Parameters() {}
   string input;          // Input file name
   string output;         // Output file name
   float MinDzeroPT;      // Lower limit of Dzero pt
   float MaxDzeroPT;      // Upper limit of Dzero pt
   float MinDzeroY;       // Lower limit of Dzero rapidity
   float MaxDzeroY;       // Upper limit of Dzero rapidity
   bool IsGammaN;         // GammaN analysis (or NGamma)
   int TriggerChoice;     // 0 = no trigger sel, 1 = isL1ZDCOr, 2 = isL1ZDCXORJet8
   bool IsData;           // Data or MC
   float scaleFactor;     // Scale factor
   int DoSystRapGap;      // Systematic study: apply the alternative event selections
                          // 0 = nominal, 1 = tight, -1: loose
   int DoSystD;           // Systematic study: apply the alternative D selections
                          // 0 = nominal, 1 = Dsvpv variation, 2: DtrkPt variation
                          // 3 = Dalpha variation, 4: Dchi2cl variation

   int nThread;           // Number of Threads
   int nChunk;            // Process the Nth chunk
   void printParameters() const {
       cout << "Input file: " << input << endl;
       cout << "Output file: " << output << endl;
       cout << "MinDzeroPT: " << MinDzeroPT << endl;
       cout << "MaxDzeroPT: " << MaxDzeroPT << endl;
       cout << "MinDzeroY: " << MinDzeroY << endl;
       cout << "MaxDzeroY: " << MaxDzeroY << endl;
       cout << "IsGammaN: " << IsGammaN << endl; 
       cout << "TriggerChoice: " << TriggerChoice << endl;
       cout << "IsData: " << IsData << endl;
       cout << "Scale factor: " << scaleFactor << endl;
       cout << "DoSystRapGap: " << ((DoSystRapGap==0)? "No" :
                                    (DoSystRapGap==1)? "Tight" : "Loose")
                                << endl;
       cout << "DoSystD: "      << ((DoSystD==0)? "No" :
                                    (DoSystD==1)? "Dsvpv variation" :
                                    (DoSystD==2)? "DtrkPt variation" :
                                    (DoSystD==3)? "Dalpha variation" : "Dchi2cl variation")
                                << endl;
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
    TH1D* hIsGammaN = new TH1D("parIsGammaN", "parIsGammaN", 1, 0, 1);
    hIsGammaN->SetBinContent(1, par.IsGammaN);
    TH1D* hTriggerChoice = new TH1D("parTriggerChoice", "parTriggerChoice", 1, 0, 1);
    hTriggerChoice->SetBinContent(1, par.TriggerChoice);
    TH1D* hIsData = new TH1D("parIsData", "parIsData", 1, 0, 1);
    hIsData->SetBinContent(1, par.IsData);
    TH1D* hScaleFactor = new TH1D("parScaleFactor", "parScaleFactor", 1, 0, 1);
    hScaleFactor->SetBinContent(1, par.scaleFactor);
    TH1D* hDoSystRapGap = new TH1D("parDoSystRapGap", "parDoSystRapGap", 1, 0, 1);
    hDoSystRapGap->SetBinContent(1, par.DoSystRapGap);
    TH1D* hDoSystD = new TH1D("parDoSystD", "parDoSystD", 1, 0, 1);
    hDoSystD->SetBinContent(1, par.DoSystD);

    // Write histograms to the output file
    hMinDzeroPT->Write();
    hMaxDzeroPT->Write();
    hMinDzeroY->Write();
    hMaxDzeroY->Write();
    hIsGammaN->Write();
    hTriggerChoice->Write();
    hIsData->Write();
    hScaleFactor->Write();
    hDoSystRapGap->Write();
    hDoSystD->Write();

    // Clean up
    delete hMinDzeroPT;
    delete hMaxDzeroPT;
    delete hMinDzeroY;
    delete hMaxDzeroY;
    delete hIsGammaN;
    delete hTriggerChoice;
    delete hIsData;
    delete hScaleFactor;
    delete hDoSystRapGap;
    delete hDoSystD;
}
