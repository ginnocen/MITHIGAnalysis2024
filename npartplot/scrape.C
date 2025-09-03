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

GraphEntry getSmallSystemStatistical(int ptBin, int index){
    std::string centval = "gRAA_PtVsNpart_Stat_ptBin" + std::to_string(ptBin);
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
    return {x, y, ey, ey};
}

GraphEntry getSmallSystemSystematic(int ptBin, int index) {
    //std::string centval = "gRAA_PtVsNpart_ptBin" + std::to_string(ptBin);
    //std::string centval = "gRAA_PtVsNpart_TotUncert_ptBin" + std::to_string(ptBin);
    std::string centval1 = "gRAA_PtVsNpart_Syst_ptBin" + std::to_string(ptBin);
    std::string centval2 = "gRAA_PtVsNpart_Norm_ptBin" + std::to_string(ptBin);
    TFile* file = TFile::Open("data/ptBinned_RAAVsNpart.root");

    TGraphErrors* graph1 = (TGraphErrors*)file->Get(centval1.c_str());
    TGraphErrors* graph2 = (TGraphErrors*)file->Get(centval2.c_str());

    if (index < 0 || index >= graph1->GetN()) {
        file->Close();
        throw std::out_of_range("Index out of range.");
    }
    double x, y;
    graph1->GetPoint(index, x, y);
    double ex = graph1->GetErrorX(index);
    double ey = sqrt(graph1->GetErrorY(index) * graph1->GetErrorY(index) + graph2->GetErrorY(index) * graph2->GetErrorY(index));
    file->Close();
    //cout << "success" << endl;
    return {x, y, ey, ey};
}

GraphEntry getSmallSystemSystematic_only(int ptBin, int index) {
    //std::string centval = "gRAA_PtVsNpart_ptBin" + std::to_string(ptBin);
    //std::string centval = "gRAA_PtVsNpart_TotUncert_ptBin" + std::to_string(ptBin);
    std::string centval1 = "gRAA_PtVsNpart_Syst_ptBin" + std::to_string(ptBin);
    std::string centval2 = "gRAA_PtVsNpart_Norm_ptBin" + std::to_string(ptBin);
    TFile* file = TFile::Open("data/ptBinned_RAAVsNpart.root");

    TGraphErrors* graph1 = (TGraphErrors*)file->Get(centval1.c_str());
    TGraphErrors* graph2 = (TGraphErrors*)file->Get(centval2.c_str());

    if (index < 0 || index >= graph1->GetN()) {
        file->Close();
        throw std::out_of_range("Index out of range.");
    }
    double x, y;
    graph1->GetPoint(index, x, y);
    double ex = graph1->GetErrorX(index);
    double ey = sqrt(graph1->GetErrorY(index) * graph1->GetErrorY(index));
    file->Close();
    //cout << "success" << endl;
    return {x, y, ey, ey};
}

GraphEntry getXeXeStatistical(int ptBin, int centbin) {

    TFile* F = TFile::Open("data/XeXeDifferential.root");
    TDirectoryFile* dir = (TDirectoryFile*)F->Get("Table 3");
    TH1F* graph = (TH1F*)dir->Get(("Hist1D_y" + std::to_string(centbin + 1)).c_str());
    TH1F* graph1 = (TH1F*)dir->Get(("Hist1D_y" + std::to_string(centbin + 1) + "_e1").c_str());
    TH1F* graph2 = (TH1F*)dir->Get(("Hist1D_y" + std::to_string(centbin + 1) + "_e2").c_str());
    TH1F* graph3 = (TH1F*)dir->Get(("Hist1D_y" + std::to_string(centbin + 1) + "_e3").c_str());


    double x, y;
    x = graph->GetBinCenter(ptBin);
    y = graph->GetBinContent(ptBin);
    //double ex = graph->GetErrorX(ptBin);
    double eyl = graph1->GetBinError(ptBin);
    double eyh = graph1->GetBinError(ptBin);
    F->Close();
    return {x, y, eyl, eyh};
}

GraphEntry getXeXeSystematic(int ptBin, int centbin) {

    TFile* F = TFile::Open("data/XeXeDifferential.root");
    TDirectoryFile* dir = (TDirectoryFile*)F->Get("Table 3");
    TH1F* graph = (TH1F*)dir->Get(("Hist1D_y" + std::to_string(centbin + 1)).c_str());
    TH1F* graph1 = (TH1F*)dir->Get(("Hist1D_y" + std::to_string(centbin + 1) + "_e1").c_str());
    TH1F* graph2 = (TH1F*)dir->Get(("Hist1D_y" + std::to_string(centbin + 1) + "_e2").c_str());
    TH1F* graph3 = (TH1F*)dir->Get(("Hist1D_y" + std::to_string(centbin + 1) + "_e3").c_str());

    double x, y;
    x = graph->GetBinCenter(ptBin);
    y = graph->GetBinContent(ptBin);
    //double ex = graph->GetErrorX(ptBin);
    double eyl = sqrt(graph2->GetBinError(ptBin)*graph2->GetBinError(ptBin) + graph3->GetBinError(ptBin)*graph3->GetBinError(ptBin));
    double eyh = sqrt(graph2->GetBinError(ptBin)*graph2->GetBinError(ptBin) + graph3->GetBinError(ptBin)*graph3->GetBinError(ptBin));
    F->Close();
    return {x, y, eyl, eyh};
}

GraphEntry getXeXeSystematic_only(int ptBin, int centbin){

    TFile* F = TFile::Open("data/XeXeDifferential.root");
    TDirectoryFile* dir = (TDirectoryFile*)F->Get("Table 3");
    TH1F* graph = (TH1F*)dir->Get(("Hist1D_y" + std::to_string(centbin + 1)).c_str());
    TH1F* graph1 = (TH1F*)dir->Get(("Hist1D_y" + std::to_string(centbin + 1) + "_e1").c_str());
    TH1F* graph2 = (TH1F*)dir->Get(("Hist1D_y" + std::to_string(centbin + 1) + "_e2").c_str());
    TH1F* graph3 = (TH1F*)dir->Get(("Hist1D_y" + std::to_string(centbin + 1) + "_e3").c_str());

    double x, y;
    x = graph->GetBinCenter(ptBin);
    y = graph->GetBinContent(ptBin);
    //double ex = graph->GetErrorX(ptBin);
    double eyl = sqrt(graph2->GetBinError(ptBin)*graph2->GetBinError(ptBin));
    double eyh = sqrt(graph2->GetBinError(ptBin)*graph2->GetBinError(ptBin));
    F->Close();
    return {x, y, eyl, eyh};

}

GraphEntry getXeXeEntry(int ptBin, int centbin) {

    TFile* F = TFile::Open("data/XeXeDifferential.root");
    TDirectoryFile* dir = (TDirectoryFile*)F->Get("Table 3");
    TGraphAsymmErrors* graph = (TGraphAsymmErrors*)dir->Get(("Hist1D_y" + std::to_string(centbin + 1)).c_str());
    //cout << ("Graph1D_y" + std::to_string(centbin)).c_str() << endl;

    double x, y;
    graph->GetPoint(ptBin, x, y);
    double ex = graph->GetErrorX(ptBin);
    double eyl = graph->GetErrorYlow(ptBin);
    double eyh = graph->GetErrorYhigh(ptBin);
    F->Close();
    return {x, y, eyl, eyh};
}

GraphEntry getPbPbStatistical(int ptBin, int centbin) {

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
    TH1F* hist = (TH1F*)dir->Get("Hist1D_y1_e1");
    TH1F* histc = (TH1F*)dir->Get("Hist1D_y1");

    double x, y;
    //hist->GetPoint(ptBin, x, y);
    x = histc->GetBinCenter(ptBin);
    y = histc->GetBinContent(ptBin);
    //double ex = hist->GetErrorX(ptBin);
    double eyl = hist->GetBinError(ptBin);
    double eyh = hist->GetBinError(ptBin);
    F->Close();
    return {x, y, eyl, eyh};
}

GraphEntry getPbPbSystematic(int ptBin, int centbin) {

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
    TH1F* hist1 = (TH1F*)dir->Get("Hist1D_y1");
    TH1F* hist2 = (TH1F*)dir->Get("Hist1D_y1_e2");
    TH1F* hist3 = (TH1F*)dir->Get("Hist1D_y1_e3plus");
    TH1F* hist4 = (TH1F*)dir->Get("Hist1D_y1_e3minus");
    TH1F* hist5 = (TH1F*)dir->Get("Hist1D_y1_e4");

    double x, y;
    x = hist1->GetBinCenter(ptBin);
    y = hist1->GetBinContent(ptBin);
    //double ex = hist->GetErrorX(ptBin);
    double eyh = sqrt(hist2->GetBinError(ptBin)*hist2->GetBinError(ptBin) + hist3->GetBinError(ptBin)*hist3->GetBinError(ptBin) + hist5->GetBinError(ptBin)*hist5->GetBinError(ptBin));
    double eyl = sqrt(hist2->GetBinError(ptBin)*hist2->GetBinError(ptBin) + hist4->GetBinError(ptBin)*hist4->GetBinError(ptBin) + hist5->GetBinError(ptBin)*hist5->GetBinError(ptBin));
    F->Close();
    return {x, y, eyl, eyh};
}

GraphEntry getPbPbSystematic_only(int ptBin, int centbin) {

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
    TH1F* hist1 = (TH1F*)dir->Get("Hist1D_y1");
    TH1F* hist2 = (TH1F*)dir->Get("Hist1D_y1_e2");
    TH1F* hist3 = (TH1F*)dir->Get("Hist1D_y1_e3plus");
    TH1F* hist4 = (TH1F*)dir->Get("Hist1D_y1_e3minus");
    TH1F* hist5 = (TH1F*)dir->Get("Hist1D_y1_e4");

    double x, y;
    x = hist1->GetBinCenter(ptBin);
    y = hist1->GetBinContent(ptBin);
    //double ex = hist->GetErrorX(ptBin);
    double eyh = sqrt(hist2->GetBinError(ptBin)*hist2->GetBinError(ptBin));
    double eyl = sqrt(hist2->GetBinError(ptBin)*hist2->GetBinError(ptBin));
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

GraphEntry getpPbStatistical(int ptBin){

    TFile*F = TFile::Open("data/pPb.root");
    TDirectoryFile* dir = (TDirectoryFile*)F->Get("Table 16");
    TH1F* hist = (TH1F*)dir->Get("Hist1D_y1");
    TH1F* hist1 = (TH1F*)dir->Get("Hist1D_y1_e1");

    double x, y;
    x = hist->GetBinCenter(ptBin);
    y = hist->GetBinContent(ptBin);
    double ex = hist->GetBinError(ptBin);
    double eyl = hist1->GetBinError(ptBin);
    double eyh = hist1->GetBinError(ptBin);
    F->Close();
    return {x, y, eyl, eyh};
}

GraphEntry getpPbSystematic(int ptBin){

    TFile*F = TFile::Open("data/pPb.root");
    TDirectoryFile* dir = (TDirectoryFile*)F->Get("Table 16");
    TH1F* hist = (TH1F*)dir->Get("Hist1D_y1");
    TH1F* hist2 = (TH1F*)dir->Get("Hist1D_y1_e2plus");
    TH1F* hist3 = (TH1F*)dir->Get("Hist1D_y1_e2minus");
    TH1F* hist4 = (TH1F*)dir->Get("Hist1D_y1_e3plus");
    TH1F* hist5 = (TH1F*)dir->Get("Hist1D_y1_e3minus");
    TH1F* hist6 = (TH1F*)dir->Get("Hist1D_y1_e4");

    double x, y;
    x = hist->GetBinCenter(ptBin);
    y = hist->GetBinContent(ptBin);
    double ex = hist->GetBinError(ptBin);
    double eyh = sqrt(hist2->GetBinError(ptBin)*hist2->GetBinError(ptBin) + hist4->GetBinError(ptBin)*hist4->GetBinError(ptBin) + hist6->GetBinError(ptBin)*hist6->GetBinError(ptBin));
    double eyl = sqrt(hist3->GetBinError(ptBin)*hist3->GetBinError(ptBin) + hist5->GetBinError(ptBin)*hist5->GetBinError(ptBin) + hist6->GetBinError(ptBin)*hist6->GetBinError(ptBin));
    F->Close();
    return {x, y, eyl, eyh};

}

GraphEntry getpPbSystematic_only(int ptBin){

    TFile*F = TFile::Open("data/pPb.root");
    TDirectoryFile* dir = (TDirectoryFile*)F->Get("Table 16");
    TH1F* hist = (TH1F*)dir->Get("Hist1D_y1");
    TH1F* hist2 = (TH1F*)dir->Get("Hist1D_y1_e2plus");
    TH1F* hist3 = (TH1F*)dir->Get("Hist1D_y1_e2minus");
    TH1F* hist4 = (TH1F*)dir->Get("Hist1D_y1_e3plus");
    TH1F* hist5 = (TH1F*)dir->Get("Hist1D_y1_e3minus");
    TH1F* hist6 = (TH1F*)dir->Get("Hist1D_y1_e4");

    double x, y;
    x = hist->GetBinCenter(ptBin);
    y = hist->GetBinContent(ptBin);
    double ex = hist->GetBinError(ptBin);
    double eyh = sqrt(hist2->GetBinError(ptBin)*hist2->GetBinError(ptBin));
    double eyl = sqrt(hist3->GetBinError(ptBin)*hist3->GetBinError(ptBin));
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