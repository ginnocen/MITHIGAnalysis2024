#include <TFile.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <iostream>
#include <string>

struct GraphEntry {
    double x;
    double y;
    double eyh;
    double eyl;
};

GraphEntry getSmallSystemEntry(int ptBin, int index) {
    //std::string centval = "gRAA_PtVsNpart_ptBin" + std::to_string(ptBin);
    std::string centval = "gRAA_PtVsNpart_TotUncert_ptBin" + std::to_string(ptBin);
    TFile* file = TFile::Open("data/ptBinned_RAAVsNpart.root");

    TGraphErrors* graph = (TGraphErrors*)file->Get(centval.c_str());
    
    if (index < 0 || index >= graph->GetN()) {
        file->Close();
        throw std::out_of_range("Index out of range.");
    }
    double x, y;
    graph->GetPoint(index, x, y);
    double ex = graph->GetErrorX(index);
    double ey = graph->GetErrorY(index);
    file->Close();
    //cout << "success" << endl;
    return {x, y, ey, ey};
}


GraphEntry getXeXeEntry(int ptBin, int centbin) {

    TFile* F = TFile::Open("data/XeXeDifferential.root");
    TDirectoryFile* dir = (TDirectoryFile*)F->Get("Table 3");
    TGraphAsymmErrors* graph = (TGraphAsymmErrors*)dir->Get(("Graph1D_y" + std::to_string(centbin + 1)).c_str());
    //cout << ("Graph1D_y" + std::to_string(centbin)).c_str() << endl;

    double x, y;
    graph->GetPoint(ptBin, x, y);
    double ex = graph->GetErrorX(ptBin);
    double eyl = graph->GetErrorYlow(ptBin);
    double eyh = graph->GetErrorYhigh(ptBin);
    F->Close();
    return {x, y, eyl, eyh};
}

GraphEntry getPbPbEntry(int ptBin, int centbin) {

    vector<const char*> filenames = {
        "data/c0_5.root",
        "data/c5_10.root",
        "data/c10_30.root",
        "data/c30_50.root",
        "data/c50_70.root",
        "data/c70_90.root"
    };

    TFile* F = TFile::Open(filenames[centbin]);
    std::string dirName = "Table " + std::to_string(centbin + 8);
    TDirectoryFile* dir = (TDirectoryFile*)F->Get(dirName.c_str());
    TGraphAsymmErrors* graph = (TGraphAsymmErrors*)dir->Get("Graph1D_y1");

    double x, y;
    graph->GetPoint(ptBin, x, y);
    double ex = graph->GetErrorX(ptBin);
    double eyl = graph->GetErrorYlow(ptBin);
    double eyh = graph->GetErrorYhigh(ptBin);
    F->Close();
    return {x, y, eyl, eyh};
}

GraphEntry getPbPb_Inclusive(int ptBin){

    TFile*F = TFile::Open("data/PbPbfull.root");
    TDirectoryFile* dir = (TDirectoryFile*)F->Get("Table 15");
    TGraphAsymmErrors* graph = (TGraphAsymmErrors*)dir->Get("Graph1D_y1");

    double x, y;
    graph->GetPoint(ptBin, x, y);
    double ex = graph->GetErrorX(ptBin);
    double eyl = graph->GetErrorYlow(ptBin);
    double eyh = graph->GetErrorYhigh(ptBin);
    F->Close();
    return {x, y, eyl, eyh};
}

GraphEntry getXeXe_Inclusive(int ptBin){

    TFile*F = TFile::Open("data/XeXe_full.root");
    TDirectoryFile* dir = (TDirectoryFile*)F->Get("Table 5");
    TGraphAsymmErrors* graph = (TGraphAsymmErrors*)dir->Get("Graph1D_y1");

    double x, y;
    graph->GetPoint(ptBin, x, y);
    double ex = graph->GetErrorX(ptBin);
    double eyl = graph->GetErrorYlow(ptBin);
    double eyh = graph->GetErrorYhigh(ptBin);
    F->Close();
    return {x, y, eyl, eyh};
}