#include <variant>
#include <TCanvas.h>
#include <TCut.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <THStack.h>
#include <TLegend.h>
#include <TColor.h>
#include <TTree.h>
#include "CommandLine.h"

#include <RooFormulaVar.h>
#include <RooRealVar.h>
#include <RooPlot.h>
#include <RooHist.h> 
#include <RooHistPdf.h>
#include <RooDataHist.h>
#include <RooKeysPdf.h> 
#include <RooAddPdf.h>
#include <RooFitResult.h>
#include <RooSimultaneous.h>
#include <RooCategory.h>
#include <RooKeysPdf.h>
#include <RooDataSet.h>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;
using namespace RooFit;
using TemplateInput = variant<TH1D*, TTree*>;
using DataInput = variant<TH1D*, TTree*>;

TH1D* gethist(const char* name, const char* hname) {

    TFile* f = TFile::Open(name);
    if(!f || f->IsZombie()){
        cerr << "Error opening file: " << name << endl;
        return nullptr;
    }
    TH1D* hist = dynamic_cast<TH1D*>(f->Get(hname));
    if(!hist){
        cerr << "Error retrieving histogram 'hist' from file: " << name << endl;
        return nullptr;
    }
    return hist;

}


class fit {
    public:
        
        vector<vector<TemplateInput>> templates; // [template][channel]
        vector<string> template_names; // [template]
        vector<DataInput> data;             // [channel]
        vector<string> channel_names;  // [channel]
        vector<double> fractions; // [template]
        string varname = "";
        vector<double> xmin;   // [channel][template]
        vector<double> xmax;   // [channel][template]

        fit(
             vector<vector<TemplateInput>>& templates,
             vector<string>& template_names,
             vector<DataInput>& data,
             vector<string>& channel_names,
             vector<double>& fractions,
             string& varname,
             vector<double>& xmin,
             vector<double>& xmax
        ) {
            this->templates = templates;
            this->template_names = template_names;
            this->data = data;
            this->channel_names = channel_names;
            this->fractions = fractions;
            this->varname = varname;
            this->xmin = xmin;
            this->xmax = xmax;
        }

        // Helper: get RooKeysPdf from TemplateInput if TTree*
        RooKeysPdf* getKeysPdfFromTTree(const TemplateInput& input, RooRealVar& x, const string& name) {
        if (holds_alternative<TTree*>(input)) {
            RooDataSet* ds = new RooDataSet((name + "_ds").c_str(), (name + "_ds").c_str(), RooArgSet(x), Import(*get<TTree*>(input)));
            return new RooKeysPdf((name + "_keys").c_str(), (name + "_keys").c_str(), x, *ds);
            }
            return nullptr;
        }

        // Helper: get RooDataHist from TemplateInput (TH1D*)
        RooDataHist* getDataHistFromTH1D(const TemplateInput& input, RooRealVar& x, const string& name) {
        if (holds_alternative<TH1D*>(input)) {
            return new RooDataHist((name + "_dh").c_str(), (name + "_dh").c_str(), RooArgList(x), get<TH1D*>(input));
            }
            return nullptr;
        }

        // Helper: get RooDataSet from DataInput (TTree*)
        RooDataSet* getDataSetFromTTree(TTree* tree, RooRealVar& x, const string& name) {
           return new RooDataSet((name + "_ds").c_str(), (name + "_ds").c_str(), RooArgSet(x), Import(*tree));
        }

        // Helper: get RooDataHist from DataInput (TH1D*)
        RooDataHist* getDataHistFromTH1D_Data(const DataInput& input, RooRealVar& x, const string& name) {
            if (holds_alternative<TH1D*>(input)) {
                return new RooDataHist((name + "_dh").c_str(), (name + "_dh").c_str(), RooArgList(x), get<TH1D*>(input));
            }
            return nullptr;
        }
        
        void simultaneousFit() {
            int M = data.size();
            int N = template_names.size();

            // Find global min/max for all channels
            double global_xmin = *min_element(xmin.begin(), xmin.end());
            double global_xmax = *max_element(xmax.begin(), xmax.end());

            // Define the observable and category ONCE
            RooRealVar x(varname.c_str(), varname.c_str(), global_xmin, global_xmax);
            RooCategory channelCat("channelCat", "channelCat");
            for (int c = 0; c < M; ++c)
                channelCat.defineType(channel_names[c].c_str(), c);

            // Shared fractions (N-1 free, last is 1-sum)
            vector<RooRealVar*> fracs;
            RooArgList fracList;
            for (int i = 0; i < N-1; ++i) {
                fracs.push_back(new RooRealVar(("f_" + template_names[i]).c_str(), "", 1.0/N, 0, 1));
                fracList.add(*fracs.back());
            }

            // PDFs and data for each channel
            map<string, RooAbsPdf*> pdfs;
            RooDataHist* combData = nullptr;
            RooDataSet* combDataSet = nullptr;

            for (int c = 0; c < M; ++c) {
                // Optionally set per-channel fit range
                x.setRange(("fitRange_" + channel_names[c]).c_str(), xmin[c], xmax[c]);

                // Data
                RooAbsData* dataObj = nullptr;
                if (holds_alternative<TH1D*>(data[c])) {
                    dataObj = new RooDataHist(("data_" + channel_names[c]).c_str(), "", RooArgList(x), get<TH1D*>(data[c]));
                } else {
                    dataObj = new RooDataSet(("data_" + channel_names[c]).c_str(), "", RooArgSet(x), Import(*get<TTree*>(data[c])));
                }

                // Templates
                RooArgList pdfList;
                for (int t = 0; t < N; ++t) {
                    RooAbsPdf* tempPdf = nullptr;
                    string pdfname = "pdf_" + template_names[t] + "_" + channel_names[c];
                    if (holds_alternative<TH1D*>(templates[t][c])) {
                        RooDataHist* tempHist = new RooDataHist(("temp_" + template_names[t] + "_" + channel_names[c]).c_str(), "", RooArgList(x), get<TH1D*>(templates[t][c]));
                        tempPdf = new RooHistPdf(pdfname.c_str(), "", x, *tempHist);
                    } else {
                        RooDataSet* ds = new RooDataSet(("ds_" + template_names[t] + "_" + channel_names[c]).c_str(), "", RooArgSet(x), Import(*get<TTree*>(templates[t][c])));
                        tempPdf = new RooKeysPdf(pdfname.c_str(), "", x, *ds);
                    }
                    pdfList.add(*tempPdf);
                }

                // Last fraction is 1-sum
                string formula = "1";
                for (int i = 0; i < N-1; ++i) {
                    formula += "-f_" + template_names[i];
                }
                RooFormulaVar* lastFrac = new RooFormulaVar(("f_" + template_names[N-1]).c_str(), formula.c_str(), fracList);

                RooArgList allFracs(fracList);
                allFracs.add(*lastFrac);

                RooAddPdf* model = new RooAddPdf(("model_" + channel_names[c]).c_str(), "", pdfList, allFracs);
                model->fixCoefNormalization(RooArgSet(x));
                pdfs[channel_names[c]] = model;

                // Fill combined data
                if (dataObj->IsA() == RooDataHist::Class()) {
                    if (!combData)
                        combData = new RooDataHist("combData", "combData", RooArgList(x, channelCat));
                    RooDataHist* dataHist = static_cast<RooDataHist*>(dataObj);
                    for (int i = 0; i < dataHist->numEntries(); ++i) {
                        const RooArgSet* entry = dataHist->get(i);
                        double xval = entry->getRealValue(x.GetName());
                        double weight = dataHist->weight();
                        x.setVal(xval);
                        channelCat.setLabel(channel_names[c].c_str());
                        combData->add(RooArgSet(x, channelCat), weight);
                    }
                } else {
                    if (!combDataSet)
                        combDataSet = new RooDataSet("combData", "combData", RooArgSet(x, channelCat));
                    RooDataSet* dataSet = static_cast<RooDataSet*>(dataObj);
                    for (int i = 0; i < dataSet->numEntries(); ++i) {
                        const RooArgSet* entry = dataSet->get(i);
                        double xval = entry->getRealValue(x.GetName());
                        x.setVal(xval);
                        channelCat.setLabel(channel_names[c].c_str());
                        combDataSet->add(RooArgSet(x, channelCat));
                    }
                }
                delete dataObj;
            }

            // Simultaneous PDF
            RooSimultaneous simPdf("simPdf", "simPdf", channelCat);
            for (int c = 0; c < M; ++c)
                simPdf.addPdf(*pdfs[channel_names[c]], channel_names[c].c_str());

            // Fit
            if (combData) {
                simPdf.fitTo(*combData, Extended(kTRUE), 
                    Save(), 
                    Range("fitRange"), 
                    Verbose(), 
                    PrintLevel(1),
                    SumW2Error(true),
                    Minimizer("Minuit2","Migrad"),
                    Strategy(2),
                    MaxCalls(1000000));
            } else if (combDataSet) {
                simPdf.fitTo(*combDataSet, Extended(kTRUE), 
                    Save(), 
                    Range("fitRange"), 
                    Verbose(), 
                    PrintLevel(1),
                    SumW2Error(true),
                    Minimizer("Minuit2","Migrad"),
                    Strategy(2),
                    MaxCalls(1000000));
            } else {
                cerr << "No combined data to fit!" << endl;
                return;
            }

            cout << "Fitted fractions:" << endl;
            for (int i = 0; i < N-1; ++i) {
                cout << setw(15) << template_names[i] << ": "
                    << setw(10) << fracs[i]->getVal()
                    << " +/- " << setw(10) << fracs[i]->getError() << endl;
            }
            // Last fraction (computed as 1-sum)
            double lastFracVal = 1.0;
            for (int i = 0; i < N-1; ++i) lastFracVal -= fracs[i]->getVal();
            double lastFracErr = 0.0;
            for (int i = 0; i < N-1; ++i) lastFracErr += pow(fracs[i]->getError(), 2);
            lastFracErr = sqrt(lastFracErr);

            cout << setw(15) << template_names[N-1] << ": "
                << setw(10) << lastFracVal
                << " +/- " << setw(10) << lastFracErr << endl;
        }
   
    };


int main(int argc, char *argv[]) {
    CommandLine CL(argc, argv);

    string dataFile = CL.Get("Data");
    vector<string> templateFiles = CL.GetStringVector("Templates"); // one file per template
    vector<string> templateNames = CL.GetStringVector("TemplateNames");
    vector<double> fractions = CL.GetDoubleVector("Fractions");
    vector<string> branches = CL.GetStringVector("Branches"); // one per channel
    vector<string> channelNames = branches; // use branch names as channel names
    string varname = CL.Get("VarName");
    vector<double> xmin = CL.GetDoubleVector("Xmin");
    vector<double> xmax = CL.GetDoubleVector("Xmax");
    int ishisttempaltes = CL.GetInt("IsHistTemplates", 1); // 1 if templates are TH1D, 0 if TTree
    int ishistdata = CL.GetInt("IsHistData", 1); // 1 if data is TH1D, 0 if TTree

    int M = branches.size();
    int N = templateNames.size();

    // Load data histograms for each channel from the same file
    vector<DataInput> data(M);
    TFile* dataF = TFile::Open(dataFile.c_str());
    for (int c = 0; c < M; ++c) {
        TH1D* h = dynamic_cast<TH1D*>(dataF->Get(branches[c].c_str()));
        data[c] = h;
    }

    // Load templates: [template][channel]
    vector<vector<TemplateInput>> templates(N, vector<TemplateInput>(M));
    for (int t = 0; t < N; ++t) {
        TFile* f = TFile::Open(templateFiles[t].c_str());
        for (int c = 0; c < M; ++c) {
            TH1D* h = dynamic_cast<TH1D*>(f->Get(branches[c].c_str()));
            if (h) {
                templates[t][c] = h;
            } else {
                TTree* tree = dynamic_cast<TTree*>(f->Get(branches[c].c_str()));
                if (tree) templates[t][c] = tree;
            }
        }
    }

    fit a(
        templates,
        templateNames,
        data,
        channelNames,
        fractions,
        varname,
        xmin,
        xmax
    );
    a.simultaneousFit();

    return 0;
}

