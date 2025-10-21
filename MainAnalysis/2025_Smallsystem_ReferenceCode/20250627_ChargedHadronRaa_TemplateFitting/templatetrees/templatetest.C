#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <vector>



void templatetest(){

    cout << "start" << endl;

    vector<string> tfiles = {"template_OO.root", "template_DD.root", "template_SD.root"};
    vector<double> fracs = {0.1, 0.1, 0.8};

    TFile*foo = TFile::Open("template_OO.root");
    TH1D*hoo = (TH1D*)foo->Get("hMult");

    TFile*fsd = TFile::Open("template_SD.root");
    TH1D*hsd = (TH1D*)fsd->Get("hMult");

    TFile*fdd = TFile::Open("template_DD.root");
    TH1D*hdd = (TH1D*)fdd->Get("hMult");

    TFile*outfile = new TFile("data.root", "RECREATE");
    outfile->cd();
    TTree*T = new TTree("Mixtree", "Mixture Tree");
    float d = 0;
    T->Branch("Mult", &d, "Mult/F");
    TH1D* h = new TH1D("hMult", "hMult", 100, 0, 100);

    for(int i = 0; i< 10000; i++){
        double r = gRandom->Rndm();
        int idx = -1;
        double sum = 0;
        for(int j = 0; j < fracs.size(); j++){
            sum += fracs[j];
            if(r < sum){
                idx = j;
                break;
            }
        }
        
        if(idx == 0) d = hoo->GetRandom();
        else if(idx == 1) d = hsd->GetRandom();
        else if(idx == 2) d = hdd->GetRandom();
        
        T->Fill();
    }
    
    outfile->Write();
    outfile->Close();
    
    cout << "done" << endl;

}
