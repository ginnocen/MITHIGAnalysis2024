#include <vector>
#include <cmath>
using namespace std;

TGraph* Errors(TH1D* hist, float xignore){
    TGraph* graph = new TGraph();
    for(int i = 1; i <= hist->GetNbinsX(); i++){
        float binCenter = hist->GetBinCenter(i);
        if(xignore > 0 && binCenter > xignore){graph->SetPoint(graph->GetN(), binCenter, 0);}
        else{graph->SetPoint(graph->GetN(), binCenter, hist->GetBinError(i));}
    }
    return graph;
}

TGraph* RelErrors(TH1D* hist, float xignore){
    TGraph* graph = new TGraph();
    for(int i = 1; i <= hist->GetNbinsX(); i++){
        float binCenter = hist->GetBinCenter(i);
        double binContent = hist->GetBinContent(i);
        double binError = hist->GetBinError(i);
        if(xignore > 0 && binCenter > xignore){binError = 0;}
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
    float xignore;

    systfit(TH1D* h, float x, float xi) : hist(h), xmin(x), xignore(xi) {
        // Create the TGraphs and assign to member variables
        binerr = Errors(h, xignore);
        binrelerr = RelErrors(h, xignore);

        // Debug output
        cout << "systfit constructor: Created graphs with " << binerr->GetN() << " points for bin errors and " 
             << binrelerr->GetN() << " points for relative errors" << endl;
    }

    ~systfit() {
        delete binerr;
        delete binrelerr;
    }

    TH1D* polyfit_relerr(int deg = 1){

        
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

    TH1D* relerr_gaussian_smooth_adaptive(double sigma_scale = 1.0){

        cout << "Performing adaptive Gaussian smoothing with sigma_scale = " << sigma_scale << " on relative errors above pT = " << xmin << " GeV" << endl;
        
        // Get the points from the relative error graph (only non-zero values)
        vector<double> xvals, yvals;
        for(int i = 0; i < binrelerr->GetN(); i++){
            double x, y;
            binrelerr->GetPoint(i, x, y);
            if(x >= xmin && y > 0) {  // Only consider points above threshold AND with non-zero values
                xvals.push_back(x);
                yvals.push_back(y);
            }
        }

        if(xvals.size() < 3) {
            cout << "ERROR: Not enough non-zero points above threshold for smoothing! Only " << xvals.size() << " points found." << endl;
            return nullptr;
        }

        cout << "Using " << xvals.size() << " non-zero points above threshold for adaptive smoothing" << endl;
        
        // Calculate local bin widths for adaptive smoothing
        vector<double> localSigmas(xvals.size());
        for(int i = 0; i < xvals.size(); i++){
            double avgSpacing = 0.0;
            int neighbors = 0;
            
            // Calculate average spacing to neighboring points
            if(i > 0) {
                avgSpacing += xvals[i] - xvals[i-1];
                neighbors++;
            }
            if(i < xvals.size() - 1) {
                avgSpacing += xvals[i+1] - xvals[i];
                neighbors++;
            }
            
            if(neighbors > 0) {
                avgSpacing /= neighbors;
            } else {
                avgSpacing = 1.0; // fallback
            }
            
            localSigmas[i] = sigma_scale * avgSpacing;
            
            // Cap the sigma to prevent over-smoothing at high pT
            double maxSigma = 15.0; // Maximum smoothing width in GeV
            if(localSigmas[i] > maxSigma) {
                localSigmas[i] = maxSigma;
            }
            
            if(i < 3 || i >= xvals.size() - 3) { // Debug output for first and last few points
                cout << "Point " << i << " (pT=" << xvals[i] << "): local spacing=" << avgSpacing 
                     << ", raw sigma=" << (sigma_scale * avgSpacing) << ", capped sigma=" << localSigmas[i] << endl;
            }
        }
        
        // Perform Gaussian smoothing with adaptive sigma
        vector<double> smoothedYvals(yvals.size());
        
        for(int i = 0; i < xvals.size(); i++){
            double weightedSum = 0.0;
            double weightSum = 0.0;
            
            // Apply Gaussian kernel to all points
            for(int j = 0; j < xvals.size(); j++){
                double dx = xvals[i] - xvals[j];
                double sigma_eff = localSigmas[i];
                double weight = exp(-0.5 * dx * dx / (sigma_eff * sigma_eff));
                
                weightedSum += yvals[j] * weight;
                weightSum += weight;
            }
            
            smoothedYvals[i] = (weightSum > 0) ? weightedSum / weightSum : yvals[i];
            
            // Debug output for the last few smoothed values
            if(i >= xvals.size() - 3) {
                cout << "Adaptive Point " << i << " (pT=" << xvals[i] << "): Original=" << yvals[i] 
                     << "%, Smoothed=" << smoothedYvals[i] << "%" << endl;
            }
        }
        
        // Create smoothed histogram - assuming bin centers are identical
        TH1D* smoothed = (TH1D*)hist->Clone();
        int binsModified = 0;
        int smoothedIndex = 0;
        
        for(int i = 1; i <= smoothed->GetNbinsX(); i++){
            float binCenter = smoothed->GetBinCenter(i);
            
            // Check if this bin corresponds to a smoothed point
            if(smoothedIndex < xvals.size() && abs(binCenter - xvals[smoothedIndex]) < 1e-6){
                double binContent = smoothed->GetBinContent(i);
                if(binContent > 0 && smoothedYvals[smoothedIndex] > 0) {
                    double newError = smoothedYvals[smoothedIndex] * binContent / 100.0;
                    smoothed->SetBinError(i, newError);
                    binsModified++;
                }
                smoothedIndex++;
            }
        }
        
        cout << "Adaptive smoothing modified " << binsModified << " bins" << endl;
        return smoothed;
    }

    TH1D* relerr_gaussian_smooth_flat(double sigma = 10.0){
        
        cout << "Performing flat Gaussian smoothing with sigma = " << sigma << " GeV on relative errors above pT = " << xmin << " GeV" << endl;
        
        // Get the points from the relative error graph (only non-zero values)
        vector<double> xvals, yvals;
        for(int i = 0; i < binrelerr->GetN(); i++){
            double x, y;
            binrelerr->GetPoint(i, x, y);
            if(x >= xmin && y > 0) {
                xvals.push_back(x);
                yvals.push_back(y);
            }
        }

        cout << "Using " << xvals.size() << " non-zero points above threshold for flat smoothing" << endl;
        
        // Perform Gaussian smoothing with constant sigma
        vector<double> smoothedYvals(yvals.size());
        
        for(int i = 0; i < xvals.size(); i++){
            double weightedSum = 0.0;
            double weightSum = 0.0;
            
            // Apply Gaussian kernel to all points with constant sigma
            for(int j = 0; j < xvals.size(); j++){
                double dx = xvals[i] - xvals[j];
                double weight = exp(-0.5 * dx * dx / (sigma * sigma));
                
                weightedSum += yvals[j] * weight;
                weightSum += weight;
            }
            
            smoothedYvals[i] = (weightSum > 0) ? weightedSum / weightSum : yvals[i];
            
            // Debug output for the last few smoothed values
            if(i >= xvals.size() - 10) {
                cout << "Point " << i << " pT=" << xvals[i] << ",  Original=" << yvals[i] 
                     << "%, Smoothed=" << smoothedYvals[i] << "%" << endl;
            }
        }
        
        // Create smoothed histogram - assuming bin centers are identical
        TH1D* smoothed = (TH1D*)hist->Clone();
        int binsModified = 0;
        int smoothedIndex = 0;
        
        for(int i = 1; i <= smoothed->GetNbinsX(); i++){
            float binCenter = smoothed->GetBinCenter(i);
            
            // Check if this bin corresponds to a smoothed point
            if(smoothedIndex < xvals.size() && abs(binCenter - xvals[smoothedIndex]) < 1e-6){
                double binContent = smoothed->GetBinContent(i);
                if(binContent > 0 && smoothedYvals[smoothedIndex] > 0) {
                    double newError = smoothedYvals[smoothedIndex] * binContent / 100.0;
                    smoothed->SetBinError(i, newError);
                    binsModified++;
                }
                smoothedIndex++;
            }
        }
        
        cout << "Flat smoothing modified " << binsModified << " bins" << endl;
        return smoothed;
    }

    TH1D* relerr_polyfit_log(int deg = 1){

        if(!binrelerr || !hist) {
            cout << "ERROR: binrelerr or hist is null!" << endl;
            return nullptr;
        }

        // Create a new TGraph with log-transformed data
        TGraph* logGraph = new TGraph();
        int validPoints = 0;
        
        cout << "Creating log-log transformed data for fitting..." << endl;
        
        for(int i = 0; i < binrelerr->GetN(); i++){
            double x, y;
            binrelerr->GetPoint(i, x, y);
            
            // Only use points above threshold with positive, non-zero values
            if(x >= xmin && y > 1e-10) {  // Use small epsilon instead of 0 to avoid numerical issues
                double logX = log(x);
                double logY = log(y);
                logGraph->SetPoint(validPoints, logX, logY);
                validPoints++;
                
                // Debug output for first few points
                if(validPoints <= 5) {
                    cout << "Point " << validPoints << ": x=" << x << " -> log(x)=" << logX 
                         << ", y=" << y << "% -> log(y)=" << logY << endl;
                }
            }
        }
        
        if(validPoints < deg + 1) {
            cout << "ERROR: Not enough valid points (" << validPoints << ") for degree " << deg << " polynomial fit!" << endl;
            delete logGraph;
            return nullptr;
        }
        
        cout << "Fitting polynomial of degree " << deg << " to " << validPoints << " log-transformed points" << endl;
        
        // Fit polynomial in log-log space
        TFitResultPtr fitResult = logGraph->Fit(Form("pol%d", deg), "QS");
        
        if(!fitResult.Get() || fitResult->Status() != 0) {
            cout << "ERROR: Log-log fit failed with status " << fitResult->Status() << endl;
            delete logGraph;
            return nullptr;
        }
        
        TF1* logFitFunc = logGraph->GetFunction(Form("pol%d", deg));
        if(!logFitFunc) {
            cout << "ERROR: Could not retrieve log-log fit function!" << endl;
            delete logGraph;
            return nullptr;
        }
        
        cout << "Log-log fit successful! Chi2/NDF = " << logFitFunc->GetChisquare() << "/" << logFitFunc->GetNDF() << endl;
        
        // Print fit parameters
        for(int i = 0; i <= deg; i++) {
            cout << "Log-log Parameter " << i << ": " << logFitFunc->GetParameter(i) << " +/- " << logFitFunc->GetParError(i) << endl;
        }
        
        // Create a custom TF1 that transforms x to log, evaluates polynomial, then transforms back
        TF1* transformedFunc = new TF1("loglog_fit", [logFitFunc](double* x, double* p) {
            double logX = log(x[0]);
            double logY = logFitFunc->Eval(logX);
            return exp(logY);  // Transform back from log space
        }, xmin, 400, 0);
        
        // Apply the fit to create smoothed histogram
        TH1D* smoothed = (TH1D*)hist->Clone();
        int binsModified = 0;
        
        for(int i = 1; i <= smoothed->GetNbinsX(); i++){
            float binCenter = smoothed->GetBinCenter(i);
            if(binCenter >= xmin){
                double originalError = smoothed->GetBinError(i);
                double fitValue = transformedFunc->Eval(binCenter);
                double binContent = smoothed->GetBinContent(i);
                
                if(fitValue > 0 && binContent > 0) {
                    double newError = fitValue * binContent / 100.0;
                    if(binCenter > xignore && xignore > 0){newError = 0;}
                    smoothed->SetBinError(i, newError);
                    binsModified++;
                    
                    if(binsModified <= 5) { // Debug output for first few bins
                        cout << "Bin " << i << " (pT=" << binCenter << "): Original error=" << originalError 
                             << ", Log-log fit value=" << fitValue << "%, New error=" << newError << endl;
                    }
                }
            }
        }
        
        cout << "Log-log fit modified " << binsModified << " bins with smoothed errors" << endl;
        
        // Clean up
        delete logGraph;
        delete transformedFunc;
        
        return smoothed;
    }

};

int smoothsystematics(){
    return 1;
}


