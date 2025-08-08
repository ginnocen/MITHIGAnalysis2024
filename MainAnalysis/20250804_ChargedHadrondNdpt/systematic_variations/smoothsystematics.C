TGraph* Errors(TH1D* hist){
    TGraph* graph = new TGraph();
    for(int i = 1; i <= hist->GetNbinsX(); i++){
        float binCenter = hist->GetBinCenter(i);
        graph->SetPoint(graph->GetN(), binCenter, hist->GetBinError(i));
    }
    return graph;
}

TGraph* RelErrors(TH1D* hist){
    TGraph* graph = new TGraph();
    for(int i = 1; i <= hist->GetNbinsX(); i++){
        float binCenter = hist->GetBinCenter(i);
        double binContent = hist->GetBinContent(i);
        double binError = hist->GetBinError(i);
        if(binContent > 0) {
            double relativeError = binError / binContent * 100.0;
            graph->SetPoint(graph->GetN(), binCenter, relativeError);
        }
    }
    return graph;
}

class systfit{

public:
    TGraph* binerr;
    TGraph* binrelerr;
    TH1D* hist;
    float xmin;

    systfit(TH1D* h, float x) : hist(h), xmin(x) {
        // Create the TGraphs and assign to member variables
        binerr = Errors(h);
        binrelerr = RelErrors(h);
        
        // Debug output
        cout << "systfit constructor: Created graphs with " << binerr->GetN() << " points for bin errors and " 
             << binrelerr->GetN() << " points for relative errors" << endl;
    }

    ~systfit() {
        delete binerr;
        delete binrelerr;
    }

    TH1D* polyfit_relerr(int deg = 1){

        if(!binrelerr || !hist) {
            cout << "ERROR: binrelerr or hist is null!" << endl;
            return nullptr;
        }

        cout << "Fitting polynomial of degree " << deg << " to relative errors above pT = " << xmin << " GeV" << endl;
        cout << "Graph has " << binrelerr->GetN() << " points" << endl;

        // Perform the fit
        TFitResultPtr fitResult = binrelerr->Fit(Form("pol%d", deg), "QS", "", xmin, 400);
        
        if(!fitResult.Get() || fitResult->Status() != 0) {
            cout << "ERROR: Fit failed with status " << fitResult->Status() << endl;
            return nullptr;
        }

        TF1* fitFunc = binrelerr->GetFunction(Form("pol%d", deg));
        if(!fitFunc) {
            cout << "ERROR: Could not retrieve fit function!" << endl;
            return nullptr;
        }

        cout << "Fit successful! Chi2/NDF = " << fitFunc->GetChisquare() << "/" << fitFunc->GetNDF() << endl;
        
        // Print fit parameters
        for(int i = 0; i <= deg; i++) {
            cout << "Parameter " << i << ": " << fitFunc->GetParameter(i) << " +/- " << fitFunc->GetParError(i) << endl;
        }

        TH1D* smoothed = (TH1D*)hist->Clone();
        int binsModified = 0;
        
        for(int i = 1; i <= smoothed->GetNbinsX(); i++){
            float binCenter = smoothed->GetBinCenter(i);
            if(binCenter >= xmin){
                double originalError = smoothed->GetBinError(i);
                double fitValue = fitFunc->Eval(binCenter);
                double binContent = smoothed->GetBinContent(i);
                
                if(fitValue > 0 && binContent > 0) {
                    double newError = fitValue * binContent / 100.0;
                    smoothed->SetBinError(i, newError);
                    binsModified++;
                    
                    if(binsModified <= 5) { // Debug output for first few bins
                        cout << "Bin " << i << " (pT=" << binCenter << "): Original error=" << originalError 
                             << ", Fit value=" << fitValue << "%, New error=" << newError << endl;
                    }
                }
            }
        }
        
        cout << "Modified " << binsModified << " bins with smoothed errors" << endl;
        return smoothed;
    }

    TH1D* expfit_relerr(){

        if(!binrelerr || !hist) return nullptr;

        binrelerr->Fit("expo", "Q", "", xmin, 400);
        TF1* fitFunc = binrelerr->GetFunction("expo");
        if(!fitFunc) return nullptr;

        TH1D* smoothed = (TH1D*)hist->Clone();
        for(int i = 1; i <= smoothed->GetNbinsX(); i++){
            float binCenter = smoothed->GetBinCenter(i);
            if(binCenter >= xmin){
                double fitValue = fitFunc->Eval(binCenter);
                if(fitValue > 0) smoothed->SetBinError(i, fitValue * smoothed->GetBinContent(i) / 100.0); 
            }
        }

        return smoothed;
    }

};

int smoothsystematics(){
    return 1;
}


