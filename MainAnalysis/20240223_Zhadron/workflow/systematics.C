#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TF1.h>
#include <TLine.h>
#include <vector>
#include <string>
#include <iostream>
#include <cmath>

std::vector<std::string> generateSystematicFiles(const std::string& nominalFile, std::vector<std::string>& systematics, std::vector<std::string>& systematicNames) {
//    std::vector<std::string> systematics = {"HiBinDown", "HiBinUp", "MuTagged"};
    std::vector<std::string> systematicFiles;

    // Remove the ".root" extension from the nominal file
    std::string baseName = nominalFile.substr(0, nominalFile.find_last_of('.'));

    // Generate the systematic file names
    for (const auto& systematic : systematics) {
        systematicFiles.push_back(systematic + "_" + baseName + ".root");
        systematicNames.push_back(systematic);
    }

    return systematicFiles;
}

void replaceWithConstantFit(TH1D *h)
{
   TF1 *f = new TF1("f","[0]");
   h->Fit("f","LL");
   for (int i=1;i<=h->GetNbinsX();i++) {
      h->SetBinContent(i,f->GetParameter(0));
   }
}

void maxDeviation(const char* nominalFile, const std::vector<std::string>& histNames, std::vector<std::string> systematics = {"HiBinDown","HiBinUp"}, const std::string outputname = "sys", bool doSmoothFit=0) {
    std::vector<std::string> systematicNames;
    std::vector<std::string> variationFiles = generateSystematicFiles(nominalFile, systematics, systematicNames);
    static int index=0;

    TFile* fileNominal = TFile::Open(nominalFile);
    if (!fileNominal || fileNominal->IsZombie()) {
        std::cerr << "Error opening nominal file: " << nominalFile << std::endl;
        return;
    }

    for (const auto& histName : histNames) {
        index++;
	TH1D* histNominal = (TH1D*)fileNominal->Get(histName.c_str());
        if (!histNominal) {
            std::cerr << "Error retrieving histogram: " << histName << " from nominal file" << std::endl;
            continue;
        }

        TCanvas* c = new TCanvas(Form("c%d",index), "Systematic Checks", 1000, 1000);
        TLegend* legend = new TLegend(0.5, 0.7, 0.9, 0.9);

        TH1D* histMaxDeviation = (TH1D*)histNominal->Clone(Form("hMax%d",index));
        histMaxDeviation->Reset();
        histMaxDeviation->SetLineColor(kBlack);
        histMaxDeviation->SetTitle("Max Absolute Deviation");

        int colorIndex = 2;

        for (size_t i = 0; i < variationFiles.size(); ++i) {
            const auto& variationFile = variationFiles[i];
            const auto& systematicName = systematicNames[i];

            TFile* fileVariation = TFile::Open(variationFile.c_str());
            if (!fileVariation || fileVariation->IsZombie()) {
                std::cerr << "Error opening variation file: " << variationFile << std::endl;
                continue;
            }

            TH1D* histVariation = (TH1D*)fileVariation->Get(histName.c_str());
            if (!histVariation) {
                std::cerr << "Error retrieving histogram: " << histName << " from variation file" << std::endl;
                fileVariation->Close();
                continue;
            }

            TH1D* histDiff = (TH1D*)histNominal->Clone();
            histDiff->Add(histVariation, -1);
            for (int bin = 1; bin <= histDiff->GetNbinsX(); ++bin) {
                histDiff->SetBinContent(bin, std::abs(histDiff->GetBinContent(bin)));
            }
            histDiff->SetLineColor(colorIndex);
            histDiff->SetTitle(Form("Difference between %s and Nominal", variationFile.c_str()));

            if (colorIndex == 2) {
                histDiff->SetAxisRange(-0.2, 2, "Y");
                histDiff->Draw("hist");
            } else {
                histDiff->Draw("hist SAME");
            }

            legend->AddEntry(histDiff, systematicName.c_str(), "l");

            for (int bin = 1; bin <= histMaxDeviation->GetNbinsX(); ++bin) {
                double currentMax = histMaxDeviation->GetBinContent(bin);
                double diffValue = histDiff->GetBinContent(bin);
                if (diffValue > currentMax) {
                    histMaxDeviation->SetBinContent(bin, diffValue);
                }
            }

            colorIndex++;
        }

        histMaxDeviation->SetLineStyle(2);
	histMaxDeviation->SetLineWidth(2);
        histMaxDeviation->Draw("hist SAME");
        TLine* l = new TLine(histMaxDeviation->GetBinLowEdge(1), 0, histMaxDeviation->GetBinLowEdge(histMaxDeviation->GetNbinsX() + 1), 0);
        l->SetLineStyle(2);
        l->Draw();
        legend->AddEntry(histMaxDeviation, "Max Absolute Deviation", "l");

        legend->SetFillStyle(0);
        legend->SetBorderSize(0);
        legend->Draw();

        std::string canvasName = "SystematicChecks_MaxDeviation_" + histName+ ".pdf";
        c->SaveAs(canvasName.c_str());
	if (doSmoothFit) {
	   replaceWithConstantFit(histMaxDeviation);
	}
	histMaxDeviation->Add(histNominal);

        std::string outputFileName = outputname +"_"+ std::string(nominalFile).substr(0, std::string(nominalFile).find_last_of('.')) + ".root";
        TFile* outputFile = TFile::Open(outputFileName.c_str(), "UPDATE");
        if (!outputFile || outputFile->IsZombie()) {
            std::cerr << "Error creating output file: " << outputFileName << std::endl;
            fileNominal->Close();
            return;
        }
        
        histMaxDeviation->Write(histName.c_str(), TObject::kOverwrite);
        outputFile->Close();
    }

}

void compareHistograms(const char* nominalFile, const std::vector<std::string>& histNames, std::vector<std::string> systematics = {"HiBinDown","HiBinUp"}, const std::string outputname = "sys", double trackingUncertainty=0.024) {
    std::vector<std::string> systematicNames;
    std::vector<std::string> variationFiles = generateSystematicFiles(nominalFile, systematics, systematicNames);

    static int index=0;
    // Load the nominal file
    TFile* fileNominal = TFile::Open(nominalFile);
    if (!fileNominal || fileNominal->IsZombie()) {
        std::cerr << "Error opening nominal file: " << nominalFile << std::endl;
        return;
    }

    // Iterate over the histogram names
    for (const auto& histName : histNames) {
        // Retrieve the nominal histogram
        TH1D* histNominal = (TH1D*)fileNominal->Get(histName.c_str());
        if (!histNominal) {
            std::cerr << "Error retrieving histogram: " << histName << " from nominal file" << std::endl;
            continue;
        }

        // Create a canvas to draw the histograms
        TCanvas* c = new TCanvas(Form("c%d",index), "Systematic Checks", 1000, 1000);
        // Create a legend
        TLegend* legend = new TLegend(0.5, 0.7, 0.9, 0.9);

        // Create a histogram for the total contribution
        TH1D* histTotal = (TH1D*)histNominal->Clone(Form("hTotalSys%d",index));
        histTotal->Reset();
        histTotal->SetLineColor(kBlack);
        histTotal->SetTitle("Total Systematic Contribution");
        index++;

        // Loop over variation files
        int colorIndex = 2; // Starting color index for variations
        for (size_t i = 0; i < variationFiles.size(); ++i) {
            const auto& variationFile = variationFiles[i];
            const auto& systematicName = systematicNames[i];

            // Load the variation file
            TFile* fileVariation = TFile::Open(variationFile.c_str());
            if (!fileVariation || fileVariation->IsZombie()) {
                std::cerr << "Error opening variation file: " << variationFile << std::endl;
                continue;
            }

            // Retrieve the variation histogram
            TH1D* histVariation = (TH1D*)fileVariation->Get(histName.c_str());
            if (!histVariation) {
                std::cerr << "Error retrieving histogram: " << histName << " from variation file" << std::endl;
                fileVariation->Close();
                continue;
            }

            // Calculate the absolute difference
            TH1D* histDiff = (TH1D*)histNominal->Clone();
            histDiff->Add(histVariation, -1);
            for (int bin = 1; bin <= histDiff->GetNbinsX(); ++bin) {
                histDiff->SetBinContent(bin, std::abs(histDiff->GetBinContent(bin)));
            }
            histDiff->SetLineColor(colorIndex);
            histDiff->SetTitle(histName.c_str());
	    if (histDiff->Integral(1,histDiff->GetNbinsX())==0) continue;

            // Draw the histogram
            if (colorIndex == 2) {
                histDiff->SetAxisRange(-0.2, 2, "Y");
                histDiff->Draw("hist");
            } else {
                histDiff->Draw("hist SAME");
            }

            // Add to the legend
            legend->AddEntry(histDiff, systematicName.c_str(), "l");

            // Add the absolute difference to the total histogram in quadrature
            for (int bin = 1; bin <= histTotal->GetNbinsX(); ++bin) {
                double currentTotal = histTotal->GetBinContent(bin);
                double diffValue = histDiff->GetBinContent(bin);
                histTotal->SetBinContent(bin, std::sqrt(currentTotal * currentTotal + diffValue * diffValue));
            }

            // Close the variation file
            //fileVariation->Close();

            // Increment the color index
            colorIndex++;
	    if (colorIndex==5) colorIndex++;
        }

        // Add Tracking systematic (4% of nominal)
        TH1D* histTracking = (TH1D*)histNominal->Clone();
        histTracking->Reset();
        for (int bin = 1; bin <= histNominal->GetNbinsX(); ++bin) {
            histTracking->SetBinContent(bin, fabs(trackingUncertainty * histNominal->GetBinContent(bin)));
        }
        histTracking->SetLineColor(kMagenta);
        histTracking->SetTitle("Tracking Systematic");

        // Draw the Tracking histogram
        histTracking->Draw("hist SAME");
        legend->AddEntry(histTracking, "Tracking", "l");

        // Add the tracking contribution to the total histogram in quadrature
        for (int bin = 1; bin <= histTotal->GetNbinsX(); ++bin) {
            double currentTotal = histTotal->GetBinContent(bin);
            double trackingValue = histTracking->GetBinContent(bin);
            histTotal->SetBinContent(bin, std::sqrt(currentTotal * currentTotal + trackingValue * trackingValue));
        }

        // Draw the total contribution histogram
        histTotal->Draw("hist SAME");
        TLine* l = new TLine(histTotal->GetBinLowEdge(1), 0, histTotal->GetBinLowEdge(histTotal->GetNbinsX() + 1), 0);
        l->SetLineStyle(2);
        l->Draw();
        legend->AddEntry(histTotal, "Total Contribution", "l");

        // Draw the legend
        legend->SetFillStyle(0);
        legend->SetBorderSize(0);
        legend->Draw();

        // Save the canvas as an image
        std::string canvasName = "SystematicChecks_" + histName + nominalFile + ".pdf";
        c->SaveAs(canvasName.c_str());

        // Create a new ROOT file to save the total histogram
        std::string outputFileName = "sys_" + std::string(nominalFile).substr(0, std::string(nominalFile).find_last_of('.')) + ".root";
        TFile* outputFile = TFile::Open(outputFileName.c_str(), "UPDATE");
        if (!outputFile || outputFile->IsZombie()) {
            std::cerr << "Error creating output file: " << outputFileName << std::endl;
            fileNominal->Close();
            return;
        }

        // Write the total histogram to the new file
        histTotal->Write(histName.c_str(), TObject::kOverwrite);

        // Close the output file
        outputFile->Close();
    }

    // Close the nominal file
    fileNominal->Close();
}

void systematics(const char* nominalFile = "PbPb0_30-result.root", double trackingUncertainty=0.02) {
    // Define the nominal file and histogram names
    
    std::vector<std::string> histNames = {"DeltaEta_Result1_2", "DeltaPhi_Result1_2", "DeltaEta_Result2_4", "DeltaPhi_Result2_4", "DeltaEta_Result4_10", "DeltaPhi_Result4_10"};


    std::vector<std::string> systematicsMuon;
    // Add MuVar0 to MuVar11 to the list of systematics
    for (int i = 0; i <= 11; ++i) {
        cout <<"MuVar" + std::to_string(i)<<endl;
        systematicsMuon.push_back("MuVar" + std::to_string(i));
	
    }
    
    std::vector<std::string> systematicsHiBin={"HiBinUp","HiBinDown"};
    maxDeviation(nominalFile, histNames, systematicsMuon,"Muon",0);
    maxDeviation(nominalFile, histNames, systematicsHiBin,"HiBin",0);

    std::vector<std::string> systematics={"PU","HiBin","Muon","MuTagged"};
    compareHistograms(nominalFile, histNames, systematics, "sys", trackingUncertainty);


}
