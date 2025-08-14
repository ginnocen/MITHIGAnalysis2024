#pragma once
#include <vector>
#include <TMath.h>

struct Parameters{
    float xsec_SD;
    float xsec_DD;
    float xsec_had;
    float xsec_alphaO;
    int NVtxMin = 1; // Minimum number of vertices
    float VzMax = 15.0; // Maximum Z vertex
    int CCFilter = 1; // Cluster Compatibility Filter
    int PVFilter = 1; // Primary Vertex Filter
    float HFEmax_Online_min1 = 0; // Minimum HFE energy for plus side
    float HFEmax_Online_min2 = 0; // Minimum HFE energy for minus side
    float HFEmax_Offline_min1 = 0; // Minimum HFE energy for plus side
    float HFEmax_Offline_min2 = 0; // Minimum HFE energy for minus side
};

struct FileNames{
    std::string SignalFile;
    std::vector<std::string> BackgroundFiles = {};
};

struct HistVar1D{
	std::string histTitle = "Hist Title";
	std::string xLabel = "xLabel";
    std::string yLabel = "yLabel";
    int nbin = 50;
    float xmin = 0;
    float xmax = TMath::Pi()+0.0001;
    float ymin = 0;
    float ymax = 100;
    std::string outFolderName = "Plots/";
    std::string outFileName = "Unknown.png";
};

struct CanvasVar{
    const char* canvasName = "CanvasName";
    std::string outFileName = "Plots/Unknown.png";
    bool isLog = false;
};
