void plotter(){

  TFile *f = TFile::Open("zdc_energy_histograms.root");
  TH1F *hZDCPlus = (TH1F*)f->Get("hZDCPlus"); 
  TH1F *hZDCMinus = (TH1F*)f->Get("hZDCMinus");

 
  TCanvas *c1 = new TCanvas("c1", "ZDC Energy Distributions", 800, 600);
  gPad->SetLogy();
  hZDCPlus->SetLineColor(kRed);
  hZDCPlus->SetTitle("ZDC Energy Distributions;Energy (arb. units);Counts");
  hZDCPlus->Draw();
  hZDCMinus->SetLineColor(kBlue);
  hZDCMinus->Draw("SAME");
  TLegend *legend = new TLegend(0.7,0.7,0.9,0.9);
  legend->AddEntry(hZDCPlus, "ZDC Plus", "l");
  legend->AddEntry(hZDCMinus, "ZDC Minus", "l");
  legend->Draw();

}
