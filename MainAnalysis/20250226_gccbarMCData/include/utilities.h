#include "vector"
#include <TMath.h>

int findIndexOfLargestValue(vector<double>* arr) {
    int indexOfLargestValue = 0;
    for (unsigned int i = 1; i < arr->size(); i++) {
        if ((*arr)[i] > (*arr)[indexOfLargestValue]) {
            indexOfLargestValue = i;
        }
    }
    return indexOfLargestValue;
}

double DeltaPhi(double phi1,double phi2)
{
 double a=phi1-phi2;
 while (a<-TMath::Pi()) a+=2*TMath::Pi();
 while (a>TMath::Pi()) a-=2*TMath::Pi();
   
 if (a<-TMath::Pi()/2) a=2*TMath::Pi()+a;
 return a;
}

void smartWrite(TObject* h) {
   if (h!=0) {
      cout <<"write "<<h->GetName()<<endl;
      h->Write();
   } 
}
void smartWrite(TNtuple* h) {
   if (h!=0) {
      cout <<"write "<<h->GetName()<<endl;
      h->Write();
   } 
}


class HistogramRangeChecker {
private:
    double minValue;
    double maxValue;

public:
    HistogramRangeChecker() : minValue(0.0), maxValue(0.0) {}

    void checkHistogramRange(TH1D* histogram) {
        if (histogram == nullptr) {
            // Handle null histogram pointer
            return;
        }

        unsigned int numBins = histogram->GetNbinsX();

        if (numBins == 0) {
            // Handle empty histogram
            return;
        }

        double currentMinValue = histogram->GetBinContent(1);
        double currentMaxValue = histogram->GetBinContent(1);

        for (unsigned int i = 2; i <= numBins; ++i) {
            double binContent = histogram->GetBinContent(i);
            if (binContent < currentMinValue) {
                currentMinValue = binContent;
            }
            if (binContent > currentMaxValue) {
                currentMaxValue = binContent;
            }
        }

        if (histogram == nullptr || numBins == 0) {
            // If the histogram is null or empty, no need to update the range
            return;
        }

        if (histogram == nullptr || numBins == 0 || currentMinValue < minValue) {
            minValue = currentMinValue;
        }
        if (histogram == nullptr || numBins == 0 || currentMaxValue > maxValue) {
            maxValue = currentMaxValue;
        }
    }

    double getMinValue() const {
        return minValue;
    }

    double getMaxValue() const {
        return maxValue;
    }
};


