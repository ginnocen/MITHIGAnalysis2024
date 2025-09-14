

void scrapechecker(){


    TFile*F = TFile::Open("data/XeXeDifferential.root");
    TDirectoryFile* dir = (TDirectoryFile*)F->Get("Table 3");

    dir->cd();

    TGraphAsymmErrors* g1 = (TGraphAsymmErrors*)dir->Get("Graph1D_y1");
    TGraphAsymmErrors* g2 = (TGraphAsymmErrors*)dir->Get("Graph1D_y2");
    TGraphAsymmErrors* g3 = (TGraphAsymmErrors*)dir->Get("Graph1D_y3");
    TGraphAsymmErrors* g4 = (TGraphAsymmErrors*)dir->Get("Graph1D_y4");
    TGraphAsymmErrors* g5 = (TGraphAsymmErrors*)dir->Get("Graph1D_y5");
    TGraphAsymmErrors* g6 = (TGraphAsymmErrors*)dir->Get("Graph1D_y6");

    cout << "POINT " << "1.    " << "2.    " << "3.    " << "4.    " << "5.    " << "6.    " << endl; 

    for(int i = 0; i < g6->GetN(); i++){


        cout << i << " " << g1->GetY()[i] << " " << g2->GetY()[i] << " " << g3->GetY()[i] << " " << g4->GetY()[i] << " " << g5->GetY()[i] << " " << g6->GetY()[i] << " " << endl;

    }

    TFile*Fw = TFile::Open("data/c70_90.root");
    TDirectoryFile* dir2 = (TDirectoryFile*)Fw->Get("Table 13");

    dir2->cd();

    TH1F* hist = (TH1F*)dir2->Get("Hist1D_y1");

    cout << "POINT " << "7.    " << "8.    " << "9.    " << "10.    " << "11.    " << "12.    " << endl;

    for(int i = 0; i < hist->GetNbinsX(); i++){

        cout << i << " " << hist->GetBinContent(i) << " " << hist->GetBinError(i) << " " << endl;

    }

    TFile*Fw2 = TFile::Open("data/pPb.root");
    TDirectoryFile* dir3 = (TDirectoryFile*)Fw2->Get("Table 16");

    dir3->cd();

    TH1F* hist2 = (TH1F*)dir3->Get("Hist1D_y1");

    cout << "POINT " << "13.    " << "14.    " << "15.    " << "16.    " << "17.    " << "18.    " << endl;

    for(int i = 0; i < hist2->GetNbinsX(); i++){

        cout << i << " " << hist2->GetBinContent(i) << " " << hist2->GetBinError(i) << " " << endl;

    }


    TFile* f2 = TFile::Open("data/pPb.root");
    TDirectoryFile* dir4 = (TDirectoryFile*)f2->Get("Table 16");

    dir4->cd();

    cout << "POINT " << "19.    " << "20.    " << "21.    " << "22.    " << "23.    " << "24.    " << endl;

    for(int i = 0; i < hist2->GetNbinsX(); i++){

        cout << i << " " << hist2->GetBinContent(i) << " " << hist2->GetBinError(i) << " " << endl;

    }

}