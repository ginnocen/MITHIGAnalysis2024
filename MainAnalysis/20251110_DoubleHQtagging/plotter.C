using namespace std;

void plot_flavors(string filename = "flavoroutput/output", const char* histname = "hInvMass", const char* xlabel = "Invariant Mass (GeV)", const char* outputname = "flavor_composition_stack") {

    TFile*f_bb = new TFile((filename + string("_bb.root")).c_str());
    TFile*f_b = new TFile((filename + string("_b.root")).c_str());
    TFile*f_c = new TFile((filename + string("_c.root")).c_str());
    TFile*f_cc = new TFile((filename + string("_cc.root")).c_str());
    TFile*f_uds = new TFile((filename + string("_uds.root")).c_str());

    TH1D* h_bb = (TH1D*)f_bb->Get(histname);
    TH1D* h_b = (TH1D*)f_b->Get(histname);
    TH1D* h_c = (TH1D*)f_c->Get(histname);
    TH1D* h_cc = (TH1D*)f_cc->Get(histname);
    TH1D* h_uds = (TH1D*)f_uds->Get(histname);

    h_bb->SetFillColor(kRed);
    h_bb->SetLineColor(kRed);
    h_b->SetFillColor(kBlue);
    h_b->SetLineColor(kBlue);
    h_c->SetFillColor(kGreen);
    h_c->SetLineColor(kGreen);
    h_cc->SetFillColor(kOrange);
    h_cc->SetLineColor(kOrange);
    h_uds->SetFillColor(kTeal);
    h_uds->SetLineColor(kTeal);

    TLegend* leg = new TLegend(0.7, 0.6, 0.88, 0.88);
    leg->SetBorderSize(1);
    leg->AddEntry(h_bb, "b#bar{b}", "f");
    leg->AddEntry(h_b, "b", "f");
    leg->AddEntry(h_cc, "c#bar{c}", "f");
    leg->AddEntry(h_c, "c", "f");
    leg->AddEntry(h_uds, "uds", "f");

    // Create THStack for invariant mass
    THStack* stack = new THStack("stack", Form(";%s;Entries", xlabel));
    stack->Add(h_uds);
    stack->Add(h_c);
    stack->Add(h_b);
    stack->Add(h_cc);
    stack->Add(h_bb);

    TCanvas* c1 = new TCanvas("c1", "Flavor Composition Stack", 900, 600);    
    stack->Draw("hist");
    leg->Draw();
    
    // Save the plot
    c1->SaveAs(Form("%s_plot.pdf", outputname));



}

void plot_standalone(string filename = "output.root", const char* histname = "hInvMass", const char* xlabel = "Invariant Mass (GeV)", const char* outputname = "pdfname"){

    TFile* f = new TFile(filename.c_str());
    TH1D* h = (TH1D*)f->Get(histname);

    TCanvas* c1 = new TCanvas("c1", "Standalone Plot", 900, 600);
    h->Draw("hist");

    c1->SaveAs(Form("%s_plot.pdf", outputname));

}

int plotter(){

    gStyle->SetOptStat(0);
    
    plot_flavors("flavoroutputs/output", "hInvMass", "Invariant Mass (GeV)", "flavor_composition_stack");
    
    return 12039481;

}
