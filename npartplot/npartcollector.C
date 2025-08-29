
void npartcollector(){


    TFile*f = TFile::Open("data/ptBinned_RAAVsNpart.root");

    TGraphErrors*g = (TGraphErrors*)f->Get("gRAA_PtVsNpart_TotUncert_ptBin10");
    
    for(int i = 0; i< g->GetN(); i++){
        double x, y;
        g->GetPoint(i, x, y);
        cout << x << " " << y << endl;
        double ex = g->GetErrorX(i);
        double ey = g->GetErrorY(i);
        cout << "Errors: " << ex << " " << ey << endl;
    }

}