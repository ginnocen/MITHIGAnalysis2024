

void makeZcorrection(){


    TFile* f_mc = TFile::Open("../mcdistros.root");
    TFile* f_data = TFile::Open("../datadistros.root");

    TH1D* h_mc_z = ((TH2D*)f_mc->Get("hmumuZ"))->ProjectionY("h_mc_z", 0, 7);
    TH1D* h_data_z = ((TH2D*)f_data->Get("hmumuZ"))->ProjectionY("h_data_z", 0, 7);

    TH1D* weight_histo = (TH1D*)h_data_z->Clone("weight_histo");
    weight_histo->Divide(h_mc_z);

    TFile* ZCorrection = new TFile("Zcorrection.root", "RECREATE");
    weight_histo->Write();
    ZCorrection->Close();




}