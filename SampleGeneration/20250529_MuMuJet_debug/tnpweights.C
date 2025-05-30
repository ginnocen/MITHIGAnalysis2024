// asldkfjsl
#include "../../CommonCode/include/tnp_weight_lowptPbPb.h"

struct weights {
  TH2D *muid;
  TH1D *trk;
  TH2D *trg;
};

weights *tnpweight(int mode) {

  TH2D *muid = new TH2D("muid", "Muon ID Correction;#eta;pT (GeV); ", 24, 0, 2.4, 200, 0, 20);
  TH1D *trk = new TH1D("trk", "Track Correction central values;#eta;", 48, -2.4, 2.4);
  TH2D *trg = new TH2D("trg", "Trigger Correction central values;#eta;pT (GeV); ", 24, 0, 2.4, 200, 0, 20);

  vector<double> vals(13);
  double value = 0;

  double eta = 0;
  double pt = 0;

  double val_muid = 0;
  double val_trk = 0;
  double val_trg = 0;

  for (int i = -24; i < 24; i++) {

    for (int j = 0; j < 200; j++) {

      eta = i * 0.1 + 0.05;
      pt = j * 0.1 + 0.05;

      val_muid = tnp_weight_muid_pbpb(pt, eta, mode);
      val_trk = tnp_weight_trk_pbpb(eta, mode);
      val_trg = tnp_weight_trg_pbpb(pt, eta, 0, mode);

      if (eta > 0) {
        muid->Fill(eta, pt, val_muid);
        trg->Fill(eta, pt, val_trg);
        // cout << val_trg << endl;
      }
    }

    trk->Fill(eta, val_trk);
  }

  muid->SetMinimum(0.95);
  muid->SetMaximum(1.05);

  trg->SetMinimum(0.85);
  trg->SetMaximum(1.15);

  trk->SetMinimum(.95);
  trk->SetMaximum(1.05);

  cout << "done" << endl;
  weights *w = new weights();
  w->muid = muid;
  w->trk = trk;
  w->trg = trg;

  return w;
}

int tnpweights() {
  gStyle->SetOptStat(0);

  cout << "start" << endl;
  weights *w_2 = tnpweight(-2);
  weights *w_1 = tnpweight(-1);
  weights *w0 = tnpweight(0);
  weights *w1 = tnpweight(1);
  weights *w2 = tnpweight(2);

  TH2D *muid = w0->trg;

  TCanvas *c2 = new TCanvas("c1", "Muon ID Correction", 900, 600);
  c2->SetMargin(0.15, 0.15, 0.15, 0.15);
  c2->cd();
  muid->SetStats(0);
  muid->Draw("COLZ");

  c2->SaveAs("trg_0.pdf");

  /*TH1D* t_2 = w_2->trk;
  TH1D* t_1 = w_1->trk;
  TH1D* t0 = w0->trk;
  TH1D* t1 = w1->trk;
  TH1D* t2 = w2->trk;

  t_2->SetLineColor(kRed);
  t_1->SetLineColor(kRed);
  t0->SetLineColor(kBlack);
  t1->SetLineColor(kBlue);
  t2->SetLineColor(kBlue);

  t_2->SetLineStyle(2);
  t2->SetLineStyle(2);



  TLegend* leg = new TLegend(0.35, 0.6, 0.55, 0.82);
  leg->SetHeader("Track Correction");
  leg->AddEntry(t0, "Central Value", "l");
  leg->AddEntry(t_1, "Syst +#sigma", "l");
  leg->AddEntry(t_2, "Syst -#sigma", "l");
  leg->AddEntry(t1, "Stat +#sigma", "l");
  leg->AddEntry(t2, "Stat -#sigma", "l");

  TCanvas* c1 = new TCanvas("c1", "Muon ID Correction", 900, 600);
  c1->SetMargin(0.15, 0.15, 0.15, 0.15);
  c1->cd();
  //muid->SetStats(0);
  //muid->Draw("COLZ");

  t_2->Draw("HIST");
  t_1->Draw("HIST SAME");
  t1->Draw("HIST SAME");
  t2->Draw("HIST SAME");
  t0->Draw("HIST SAME");
  leg->Draw("SAME");

  c1->SaveAs("trk.pdf");*/

  return 1;
}
