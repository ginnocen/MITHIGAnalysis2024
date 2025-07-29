
//// FILE TO HANDLE THE OFFICIAL OUTPUT TO BE USED AS WEIGHTS IN THE SKIMMER. 
//// SAVES WEIGHT HISTS AS WELL AS CONFIG. INFO TO A ROOT OUTPUT FILE
void FileSaver(
    TCut TCutData,
    TCut TCutMC,
    const char* DataFile,
    const char* OOFile,
    const char* OOFile_Arg,
    const char* EfficiencyFile = "hists/output.root",
    const char* EfficiencyHistName = "hOO_Mult_Eff",
    const char* outfilename = "testweights.root"
    ){

    cout << "STARTING FILE SAVER" << endl;

    //// OPEN FILES
    TFile* fEff = TFile::Open(EfficiencyFile);
    TH1D* hEff = (TH1D*)fEff->Get(EfficiencyHistName);
    hEff->SetName("hEff");

    TH1D* hRatio = (TH1D*)fEff->Get("hRatio");
    hRatio->SetName("pTCorrection");

    TFile* fout = TFile::Open(outfilename, "RECREATE");
    fout->cd();

    // SAVE HISTOGRAMS
    hEff->Write();
    hRatio->Write();

    // SAVE CONFIG. INFO
    TNamed* cutData = new TNamed("DataCut", TCutData.GetTitle());
    TNamed* cutMC = new TNamed("MCCut", TCutMC.GetTitle());
    TNamed* ooFile = new TNamed("OOFile", OOFile);
    TNamed* ooFile_Arg = new TNamed("OOFile_Arg", OOFile_Arg);
    TNamed* dataFile = new TNamed("DataFile", DataFile);
    TNamed* effFile = new TNamed("EfficiencyFile", EfficiencyFile);
    TNamed* effHistName = new TNamed("EfficiencyHistName", EfficiencyHistName);
    
    cutData->Write();
    cutMC->Write();
    ooFile->Write();
    ooFile_Arg->Write();
    dataFile->Write();
    effFile->Write();
    effHistName->Write();

    fout->Close();

    cout << "COMPLETED FILE SAVER" << endl;
    cout << endl;
    cout << endl;

}