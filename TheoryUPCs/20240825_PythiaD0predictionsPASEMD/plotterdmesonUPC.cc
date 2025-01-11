#include "param.h"
void plotterdmesonUPC(TString filename = "output.root",
                      TString outname = "crosssection.root") {
  gStyle->SetOptStat(0);
  TFile *fin = new TFile(filename.Data(), "READ");
  TH1F *hlumicounter = (TH1F *)fin->Get("hlumicounter");
  TH2F *hUnnormCrossSection = (TH2F *)fin->Get("hUnnormCrossSection");
  hUnnormCrossSection->SetName("hCrossSection");
  TH2F *hCrossSection = (TH2F *)fin->Get("hUnnormCrossSection");
  hCrossSection->SetName("hCrossSection");
  hCrossSection->SetTitle("D^{0}-only d#sigma/dp_{T}dy from Pythia8");
  hCrossSection->GetYaxis()->SetTitle("D^{0} rapidity");
  hCrossSection->GetXaxis()->SetTitle("D^{0} p_{T} [GeV/c]");
  for (int iptbin = 1; iptbin <= hCrossSection->GetNbinsX(); iptbin++) {
    for (int iybin = 1; iybin <= hCrossSection->GetNbinsY(); iybin++) {
      double unnormcross = hUnnormCrossSection->GetBinContent(iptbin, iybin);
      double lumiinvnb = hlumicounter->GetBinContent(2) / 1e6;
      double binwidth = hUnnormCrossSection->GetXaxis()->GetBinWidth(iptbin) *
                        hUnnormCrossSection->GetYaxis()->GetBinWidth(iybin);
      double cross = unnormcross / (2. * lumiinvnb * binwidth);
      hCrossSection->SetBinContent(iptbin, iybin, cross);
      std::cout << "interval pt"
                << hUnnormCrossSection->GetXaxis()->GetBinLowEdge(iptbin) << " "
                << hUnnormCrossSection->GetXaxis()->GetBinUpEdge(iptbin);
      std::cout << " , interval y"
                << hUnnormCrossSection->GetYaxis()->GetBinLowEdge(iybin) << " "
                << hUnnormCrossSection->GetYaxis()->GetBinUpEdge(iybin);
      std::cout << " , cross section for D0-only in nb: " << cross << std::endl;
    }
  }
  std::vector<TH1D *> hcrossvsy;
  for (int iptbin = 1; iptbin <= hCrossSection->GetNbinsX(); iptbin++) {
    TString hname = Form("hcrossvsy_%d", iptbin);
    TH1D *h =
        new TH1D(hname.Data(),
                 "; D^{0} rapidity; d#sigma/dyp_{T} for particle only in nb",
                 nYBins, yBins);
    h->SetTitle(
        Form("Pythia d#sigma/dp_{T}dy (nb) for D^{0}-only p_{T} [%.1f, %.1f]",
             ptBins[iptbin - 1], ptBins[iptbin]));
    for (int iybin = 1; iybin <= hCrossSection->GetNbinsY(); iybin++) {
      h->SetBinContent(iybin, hCrossSection->GetBinContent(iptbin, iybin));
    }
    hcrossvsy.push_back(h);
  }
  TFile *fout = new TFile(outname.Data(), "RECREATE");
  for (int iptbin = 0; iptbin < hcrossvsy.size(); iptbin++) {
    hcrossvsy[iptbin]->SetName(Form("hDzerocrossbinnedvsy_pt%d", iptbin));
    hcrossvsy[iptbin]->Write();
  }
  hCrossSection->Write();
  fout->Close();
  TCanvas *c1 = new TCanvas("c1", "c1", 1400, 600);
  c1->Divide(2, 1);
  c1->cd(1);
  hCrossSection->GetYaxis()->SetTitleOffset(1.2);
  hCrossSection->Draw("colz text");
  c1->cd(2);
  gPad->SetLogy();
  TLegend *leg = new TLegend(0.5, 0.75, 0.8, 0.9);
  hcrossvsy[0]->SetTitle("Pythia d#sigma/dy for D^{0}-only");
  hcrossvsy[0]->SetMinimum(1.);
  hcrossvsy[0]->SetMaximum(1e9);
  leg->AddEntry(hcrossvsy[0], Form("p_{T} [%.1f, %.1f]", ptBins[0], ptBins[1]),
                "l");
  hcrossvsy[0]->SetLineColor(kBlack);
  hcrossvsy[0]->Draw();

  for (int iptbin = 1; iptbin < hcrossvsy.size(); iptbin++) {
    hcrossvsy[iptbin]->SetLineColor(iptbin + 1);
    hcrossvsy[iptbin]->Draw("same");
    leg->AddEntry(
        hcrossvsy[iptbin],
        Form("p_{T} [%.1f, %.1f]", ptBins[iptbin], ptBins[iptbin + 1]), "l");
  }
  leg->Draw();
  c1->SaveAs("plotterdmesonUPC.pdf");
  c1->SaveAs("plotterdmesonUPC.png");
  TFile *fref = new TFile("crosssection_ref.root", "READ");
  TH2F *href = (TH2F *)fref->Get("hCrossSection");
  href->SetName("href");
  for (int iptbin = 1; iptbin <= hCrossSection->GetNbinsX(); iptbin++) {
    for (int iybin = 1; iybin <= hCrossSection->GetNbinsY(); iybin++) {
      double cross = hCrossSection->GetBinContent(iptbin, iybin);
      double refcross = href->GetBinContent(iptbin, iybin);
      if (refcross > 0) {
        if (cross / refcross != 1) {
          std::cout << "cross section ratio: " << cross / refcross << std::endl;
        } else {
          std::cout << "ratio is=" << cross / refcross << std::endl;
        }
      }
    }
  }
}
