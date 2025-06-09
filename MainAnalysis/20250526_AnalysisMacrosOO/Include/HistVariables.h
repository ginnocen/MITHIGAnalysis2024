#ifndef HISTVARIABLES_H
#define HISTVARIABLES_H
#include <TMath.h>
#include <iostream>

struct HistVar1D{
	string histTitle = "Hist Title";
	const char* xLabel = "xLabel";
    const char* yLabel = "yLabel";
    int nbin = 50;
    float xmin = 0;
    float xmax = TMath::Pi()+0.0001;
    float ymin = 0;
    float ymax = 100;
    string outFolderName = "Plots/";
    string outFileName = "Unknown.png";
};

struct CanvasVar{
    const char* canvasName = "CanvasName";
    string outFileName = "Plots/Unknown.png";
    bool isLog = false;
};

#endif