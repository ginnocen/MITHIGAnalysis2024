//// FILE TO HANDLE THE OFFICIAL OUTPUT TO BE USED AS WEIGHTS IN THE SKIMMER. 
//// SAVES WEIGHT HISTS AS WELL AS CONFIG. INFO TO A ROOT OUTPUT FILE
void FileSaver(
    TCut TCutData,
    TCut TCutMC,
    const char* DataSource,
    const char* OOFile,
    const char* OOFile_Arg,
    const char* EfficiencyFile = "hists/output.root",
    const char* EfficiencyHistName = "hOO_Mult_Eff",
    const char* outfilename = "testweights.root"
    ){

    cout << "STARTING FILE SAVER" << endl;

    //// OPEN FILES 
    TFile* fEff = TFile::Open(EfficiencyFile);
    TFile* fVZReweight = TFile::Open("VZReweight/VZReweight.root");
    TFile* fMultReweight = TFile::Open("MultReweight/MultReweight.root");
    TFile* fTrkPtReweight = TFile::Open("TrkPtReweight/TrkPtReweight.root");

    //// GET HISTOGRAMS
    
    TH1D* hEff = (TH1D*)fEff->Get(EfficiencyHistName);
    hEff->SetName("hEff");

    TH1D* hRatio = (TH1D*)fEff->Get("hRatio");
    hRatio->SetName("pTCorrection");

    TH1D* vzReweight = (TH1D*)fVZReweight->Get("VZReweight");
    vzReweight->SetName("VZReweight");
    TH1D* vzReweight_Arg = (TH1D*)fVZReweight->Get("VZReweight_Arg");
    vzReweight_Arg->SetName("VZReweight_Arg"); 

    TH1D* multReweight = (TH1D*)fMultReweight->Get("MultReweight");
    multReweight->SetName("MultReweight");

    TH1D* multReweight_Arg = (TH1D*)fMultReweight->Get("MultReweight_Arg");
    multReweight_Arg->SetName("MultReweight_Arg");

    TH1D* TrkPtReweight = (TH1D*)fTrkPtReweight->Get("TrkPtReweight");
    TrkPtReweight->SetName("TrkPtReweight");
    TH1D* TrkPtReweight_Arg = (TH1D*)fTrkPtReweight->Get("TrkPtReweight_Arg");
    TrkPtReweight_Arg->SetName("TrkPtReweight_Arg");

    TFile* fout = TFile::Open(outfilename, "RECREATE");
    fout->cd();

    // SAVE CORRECTION HISTOGRAMS
    hEff->Write();
    hRatio->Write();

    // SAVE MC REWEIGHTING HISTOGRAMS TO SUBDIRECTORY
    vzReweight->Write();
    vzReweight_Arg->Write();
    multReweight->Write();
    multReweight_Arg->Write();
    TrkPtReweight->Write();
    TrkPtReweight_Arg->Write();

    // SAVE CONFIG. INFO
    TNamed* cutData = new TNamed("DataCut", TCutData.GetTitle());
    TNamed* cutMC = new TNamed("MCCut", TCutMC.GetTitle());
    TNamed* ooFile = new TNamed("OOFile", OOFile);
    TNamed* ooFile_Arg = new TNamed("OOFile_Arg", OOFile_Arg);
    TNamed* dataSource = new TNamed("DataSource", DataSource);
    TNamed* effFile = new TNamed("EfficiencyFile", EfficiencyFile);
    TNamed* effHistName = new TNamed("EfficiencyHistName", EfficiencyHistName);
    
    cutData->Write();
    cutMC->Write();
    ooFile->Write();
    ooFile_Arg->Write();
    dataSource->Write();
    effFile->Write();
    effHistName->Write();

    fout->Close();

    cout << "COMPLETED FILE SAVER" << endl;
    cout << endl;
    cout << endl;

}