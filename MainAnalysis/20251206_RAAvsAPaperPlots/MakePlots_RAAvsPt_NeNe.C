#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TColor.h"
#include "TLine.h"
#include "TGraphMultiErrors.h"
#include "TGraphAsymmErrors.h"
#include "TBox.h"

void MakePlots_RAAvsPt_NeNe(){

  bool doNeNe = true;

  //NeNe
  TFile * NeNeRAA = TFile::Open("Results/NeNeOverPPRatio.root","read");
  TH1D * NeNeCentral = (TH1D*) NeNeRAA->Get("normalized_RAA_NucleusNucleus");
  TH1D * NeNeSyst = (TH1D*) NeNeRAA->Get("Raa_Total_uncertainty_NucleusNucleus");
  const int N_NeNe = NeNeCentral->GetNbinsX();
  TGraphMultiErrors * NeNe = new TGraphMultiErrors("NeNe", "", N_NeNe , 2);
  for (int i = 1; i <= N_NeNe-20; ++i) {
      double x = NeNeCentral->GetBinCenter(20+i);
      double ex = NeNeCentral->GetBinWidth(20+i) / 2.0;
      double y = NeNeCentral->GetBinContent(20+i);
      double ey = NeNeCentral->GetBinError(20+i);
      double exSyst = NeNeCentral->GetBinWidth(20+i) / 2.0;
      double eySyst = NeNeSyst->GetBinError(20+i);

      NeNe->SetPoint(i - 1, x, y);
      NeNe->SetPointEX(i-1,ex,ex);
      NeNe->SetPointEY(i-1,0,ey,ey);
      NeNe->SetPointEY(i-1,1,eySyst,eySyst);
  }
  NeNe->SetMarkerStyle(21);
  NeNe->SetMarkerSize(1.1);
  NeNe->SetMarkerColor(kViolet+2);
  NeNe->SetFillColorAlpha(kViolet-9,0.75);
  NeNe->GetAttLine(0)->SetLineWidth(2);
  NeNe->GetAttLine(0)->SetLineColor(kViolet+2);
  NeNe->GetAttLine(1)->SetLineColorAlpha(kViolet-9,0.75);
  NeNe->GetAttFill(1)->SetFillColorAlpha(kViolet-9,0.75);
  NeNe->GetAttFill(1)->SetFillStyle(1001);
  NeNeRAA->Close();

  //load old measurements
  TFile * oldRAA = TFile::Open("Models/CMS_RAA_Data_Other/HEPData-ins1496050-v2-Table_15.root","read");
  TGraphAsymmErrors * PbPb_loaded = (TGraphAsymmErrors*) oldRAA->Get("Table 15/Graph1D_y1");
  TH1D * PbPbStat = (TH1D*) oldRAA->Get("Table 15/Hist1D_y1_e1");
  TH1D * PbPbSyst = (TH1D*) oldRAA->Get("Table 15/Hist1D_y1_e2");
  const int N_PbPb = PbPbStat->GetNbinsX();
  double PbPbnormUncertU = 0.036;
  double PbPbnormUncertD = 0.041;
  TGraphMultiErrors * PbPb = new TGraphMultiErrors("PbPb", "", N_PbPb , 2);
  for (int i = 1; i <= N_PbPb; ++i) {
      double x = PbPbStat->GetBinCenter(i);
      double ex = PbPbStat->GetBinWidth(i) / 2.0;
      double y = PbPb_loaded->GetPointY(i-1);
      double ey = PbPbStat->GetBinContent(i);
      double exSyst = PbPbStat->GetBinWidth(i) / 2.0;
      double eySyst = PbPbSyst->GetBinContent(i);

      PbPb->SetPoint(i - 1, x, y);
      PbPb->SetPointEX(i-1,ex,ex);
      PbPb->SetPointEY(i-1,0,ey,ey);
      PbPb->SetPointEY(i-1,1,eySyst,eySyst);
  }
  PbPb->SetMarkerStyle(4);
  PbPb->SetMarkerSize(1.3);
  PbPb->GetAttLine(0)->SetLineWidth(2);
  PbPb->GetAttLine(1)->SetLineColor(kOrange);
  PbPb->GetAttFill(1)->SetFillColor(kOrange);
  PbPb->GetAttFill(1)->SetFillStyle(1001);
  oldRAA->Close();

  oldRAA = TFile::Open("Models/CMS_RAA_Data_Other/HEPData-ins1496050-v2-Table_16.root","read");
  TGraphAsymmErrors * pPb_loaded = (TGraphAsymmErrors*) oldRAA->Get("Table 16/Graph1D_y1");
  TH1D * pPbStat = (TH1D*) oldRAA->Get("Table 16/Hist1D_y1_e1");
  TH1D * pPbSystu = (TH1D*) oldRAA->Get("Table 16/Hist1D_y1_e2plus");
  TH1D * pPbSystd = (TH1D*) oldRAA->Get("Table 16/Hist1D_y1_e2minus");
  const int N_pPb = pPbStat->GetNbinsX();
  double pPbnormUncert = 0.05;
  TGraphMultiErrors * pPb = new TGraphMultiErrors("pPb", "", N_pPb , 2);
  for (int i = 1; i <= N_pPb; ++i) {
      double x = pPbStat->GetBinCenter(i);
      double ex = pPbStat->GetBinWidth(i) / 2.0;
      double y = pPb_loaded->GetPointY(i-1);
      double ey = pPbStat->GetBinContent(i);
      double exSyst = pPbStat->GetBinWidth(i) / 2.0;
      double eySystu = pPbSystu->GetBinContent(i);
      double eySystd = pPbSystd->GetBinContent(i);

      pPb->SetPoint(i - 1, x, y);
      pPb->SetPointEX(i-1,ex,ex);
      pPb->SetPointEY(i-1,0,ey,ey);
      pPb->SetPointEY(i-1,1,eySystu,-eySystd);
  }
  pPb->Print("All");
  pPb->SetMarkerStyle(25);
  pPb->SetMarkerSize(1.3);
  pPb->GetAttLine(0)->SetLineWidth(2);
  pPb->GetAttLine(1)->SetLineColor(kGreen+2);
  pPb->GetAttFill(1)->SetFillColor(kGreen+2);
  pPb->GetAttFill(1)->SetFillStyle(1001);
  oldRAA->Close();

  //XeXe
  oldRAA = TFile::Open("Models/CMS_RAA_Data_Other/HEPData-ins1692558-v1-Table_5.root","read");
  TGraphAsymmErrors * XeXe_loaded = (TGraphAsymmErrors*) oldRAA->Get("Table 5/Graph1D_y1");
  TH1D * XeXeStat = (TH1D*) oldRAA->Get("Table 5/Hist1D_y1_e1");
  TH1D * XeXeSyst = (TH1D*) oldRAA->Get("Table 5/Hist1D_y1_e2");
  TH1D * XeXeNorm = (TH1D*) oldRAA->Get("Table 5/Hist1D_y1_e3");
  double XeXeNormUncert = 0.084;
  const int N_XeXe = XeXeStat->GetNbinsX();
  //2 is for slightly smaller box to get borders
  TGraphMultiErrors * XeXe = new TGraphMultiErrors("XeXe", "", N_XeXe , 2);
  double XeXe_eySyst[50] = {0};
  double XeXe_exSyst[50] = {0};
  for (int i = 1; i <= N_XeXe; ++i) {
      double x = XeXeStat->GetBinCenter(i);
      double ex = XeXeStat->GetBinWidth(i) / 2.0;
      double y = XeXe_loaded->GetPointY(i-1);
      double ey = XeXeStat->GetBinContent(i);
      double exSyst = XeXeStat->GetBinWidth(i) / 2.0;
      double eySyst = XeXeSyst->GetBinContent(i);

      //store these for later, not used now
      XeXe_exSyst[i] = exSyst;
      XeXe_eySyst[i] = eySyst;

      XeXe->SetPoint(i - 1, x, y);
      XeXe->SetPointEX(i-1,ex,ex);
      XeXe->SetPointEY(i-1,0,ey,ey);
      XeXe->SetPointEY(i-1,1,eySyst,eySyst);
  }
  XeXe->SetMarkerStyle(28);
  XeXe->SetMarkerSize(1.3);
  XeXe->SetLineWidth(2);
  XeXe->SetMarkerColor(TColor::GetColor("#e42536"));
  XeXe->SetLineColor(TColor::GetColor("#e42536"));
  XeXe->GetAttLine(0)->SetLineWidth(2);
  XeXe->GetAttLine(0)->SetLineColor(TColor::GetColor("#e42536"));
  XeXe->GetAttLine(1)->SetLineColor(TColor::GetColor("#e42536"));
  XeXe->GetAttLine(1)->SetLineWidth(2);
  XeXe->GetAttFill(1)->SetFillColorAlpha(TColor::GetColor("#e42536"),0.25);
  XeXe->GetAttFill(1)->SetFillStyle(1001);
  oldRAA->Close();

  //stuff for models
  //Huss et al
  TFile * HFile = TFile::Open("Models/Huss_et_al/Huss_OORAA.root","read");
  TGraphAsymmErrors * huss5 = (TGraphAsymmErrors*) HFile->Get("hRAA_OO_5p2TeV");
  TGraphAsymmErrors * huss7 = (TGraphAsymmErrors*) HFile->Get("hRAA_OO_7TeV");
  TGraphErrors * huss_Band;
  TGraph * huss_Central; 
  {
    double x[45] = {0};
    double xerr[45] = {0};
    double y[45] = {0};
    double yerr[45] = {0};
    huss_Band = new TGraphErrors(45,x,y,xerr,yerr);
    huss_Central = new TGraph(45,x,y);
    for(int i = 0; i<45; i++){
      huss_Band->SetPoint(i,huss5->GetPointX(i), ((7-5.36)*huss5->GetPointY(i)+(5.36-5.02)*huss7->GetPointY(i))/(7.0-5.02));
      huss_Band->SetPointError(i,0,huss5->GetErrorY(i));
      huss_Central->SetPoint(i,huss5->GetPointX(i), ((7-5.36)*huss5->GetPointY(i)+(5.36-5.02)*huss7->GetPointY(i))/(7.0-5.02));
    }
  }
  huss_Central->SetLineColor(kGreen+1);
  huss_Central->SetLineWidth(7);
  huss_Band->SetFillColorAlpha(kGreen+1, 0.4); // transparent fill
  huss_Band->SetLineColor(kGreen+1);
  huss_Band->SetMarkerStyle(0);

  //Zhakarov model
  TGraph* z1, *z2, *z3;
  {
  double x_vals[] = {
    10.0000000, 11.0000000, 12.0000000, 13.0000000, 14.0000000, 15.4350004,
    17.0170994, 18.7612991, 20.6844006, 22.8045006, 25.1420002, 27.7191010,
    30.5603008, 33.6926994, 37.1461983, 40.9537010, 45.1515007, 49.7794991,
    54.8819008, 60.5074005, 66.7093964, 73.5470963, 81.0857010, 89.3970032,
    98.5602036, 108.663002, 119.801003, 132.080002, 145.617996, 160.544006
  };

  double y1_vals_w_mQGP[] = {
    0.755870640, 0.768004775, 0.778846323, 0.788747787, 0.797876358, 0.809746504,
    0.821415424, 0.832812488, 0.843893647, 0.854692161, 0.865275323, 0.875537395,
    0.885364175, 0.894692779, 0.903479338, 0.911794066, 0.919618964, 0.926908135,
    0.933604717, 0.939739883, 0.945301294, 0.950270712, 0.954610109, 0.958291113,
    0.961281240, 0.963542223, 0.965033233, 0.965768039, 0.965783417, 0.965081632
  };

  double y2_vals[] = {
    0.998347998, 1.00235999, 1.00588000, 1.00896001, 1.01171005, 1.01510000,
    1.01822996, 1.02108002, 1.02361000, 1.02585995, 1.02781999, 1.02952003,
    1.03096998, 1.03217995, 1.03313005, 1.03382003, 1.03422999, 1.03436995,
    1.03421998, 1.03376997, 1.03301001, 1.03190005, 1.03042996, 1.02857006,
    1.02629006, 1.02358997, 1.02046001, 1.01691997, 1.01296997, 1.00862002
  };
  double y1_vals_no_mQGP[] = {
    0.627859473, 0.644207239, 0.659336329, 0.673270345, 0.686168849,
    0.703034461, 0.719700396, 0.736057341, 0.752015352, 0.767541528,
    0.782687426, 0.797422767, 0.811700344, 0.825475216, 0.838696241,
    0.851207316, 0.863073468, 0.874228835, 0.884635925, 0.894250631,
    0.903063834, 0.911045194, 0.918154716, 0.924346566, 0.929598093,
    0.933848977, 0.937152386, 0.939622700, 0.941366196, 0.942614496
};
  
  const int n = sizeof(x_vals) / sizeof(double);
  z1 = new TGraph(n, x_vals, y1_vals_w_mQGP);
  z3 = new TGraph(n, x_vals, y1_vals_no_mQGP);
  z2 = new TGraph(n, x_vals, y2_vals);
  z1->SetLineWidth(7);
  z1->SetLineColor(kRed);
  z3->SetLineWidth(7);
  z3->SetLineColor(kRed);
  z3->SetLineStyle(7);
  z2->SetLineWidth(7);
  z2->SetLineColor(kRed);
  }

  //Xie & Zhang model
  TGraph* XZ_Central;
  TGraphAsymmErrors* XZ_Band;
  {
    double x[] = { 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100 };
    double y[] = { 0.7781, 0.8240, 0.8862, 0.9317, 0.9613, 0.9654, 0.9793, 0.9923, 1.0028, 0.9982, 1.0095, 1.0141, 1.0095, 1.0192, 1.0199, 1.0147, 1.0195, 1.0204, 1.0235, 1.0231 };
    double yLowErr[] = { 0.0488, 0.0475, 0.0397, 0.0442, 0.0308, 0.0282, 0.0164, 0.0142, 0.0294, 0.0168, 0.0137, 0.0140, 0.0094, 0.0141, 0.0143, 0.0089, 0.0080, 0.0095, 0.0145, 0.0053 };
    double yHighErr[] = { 0.0378, 0.0398, 0.0319, 0.0211, 0.0194, 0.0332, 0.0346, 0.0276, 0.0106, 0.0321, 0.0202, 0.0099, 0.0169, 0.0065, 0.0138, 0.0115, 0.0117, 0.0057, 0.0066, 0.0119 };
 
    const int n = sizeof(x) / sizeof(double);
    XZ_Central = new TGraph(n, x, y); 
    XZ_Central->SetLineColor(kViolet);
    XZ_Central->SetLineWidth(7);

    XZ_Band = new TGraphAsymmErrors(n, x, y, nullptr, nullptr, yLowErr, yHighErr);
    XZ_Band->SetFillColorAlpha(kViolet+1, 0.4); // transparent fill
    XZ_Band->SetLineColor(kViolet+1);
    XZ_Band->SetMarkerStyle(0);
  }

  //CUJET
  TGraph* CUJET_Central;
  TGraphAsymmErrors* CUJET_Band;
  {
    double x[] = {
  10, 10.5, 11, 11.5, 12, 12.5, 13, 13.5, 14, 14.5, 15, 15.5, 16, 16.5, 17, 17.5, 18, 18.5, 19, 19.5,
  20, 20.5, 21, 21.5, 22, 22.5, 23, 23.5, 24, 24.5, 25, 25.5, 26, 26.5, 27, 27.5, 28, 28.5, 29, 29.5,
  30, 30.5, 31, 31.5, 32, 32.5, 33, 33.5, 34, 34.5, 35, 35.5, 36, 36.5, 37, 37.5, 38, 38.5, 39, 39.5,
  40, 40.5, 41, 41.5, 42, 42.5, 43, 43.5, 44, 44.5, 45, 45.5, 46, 46.5, 47, 47.5, 48, 48.5, 49, 49.5,
  50, 50.5, 51, 51.5, 52, 52.5, 53, 53.5, 54, 54.5, 55, 55.5, 56, 56.5, 57, 57.5, 58, 58.5, 59, 59.5,
  60, 60.5, 61, 61.5, 62, 62.5, 63, 63.5, 64, 64.5, 65, 65.5, 66, 66.5, 67, 67.5, 68, 68.5, 69, 69.5,
  70, 70.5, 71, 71.5, 72, 72.5, 73, 73.5, 74, 74.5, 75, 75.5, 76, 76.5, 77, 77.5, 78, 78.5, 79, 79.5,
  80
  };
  double y[] = {
  0.6026423872970323, 0.6106961676039032, 0.6184250718022829, 0.6258170759953909, 0.6329095083065894,
  0.6396986071036402, 0.6462138863397712, 0.6524617258297771, 0.6584679733137445, 0.6642393629831047,
  0.6698008075903, 0.6751571979293418, 0.6803302251377364, 0.6853262774388422, 0.6901625683238787,
  0.6948438021953748, 0.6993848360006196, 0.7037880758555063, 0.7080653020356239, 0.7122203289481301,
  0.7162583763174358, 0.7201835766203944, 0.7240008469521151, 0.7277130079529464, 0.7313252885545019,
  0.7348410563984397, 0.7382628877531013, 0.741596406291496, 0.7448463540917482, 0.7480121773577403,
  0.751102479457552, 0.7541157605096946, 0.7570593180021645, 0.759933610873598, 0.7627418981377563,
  0.7654871643542457, 0.7681729231914466, 0.7707999533111931, 0.7733702273369777, 0.7758857940421229,
  0.7783478319711645, 0.7807576805906135, 0.7831165089610332, 0.7854251396132975, 0.7876844175983284,
  0.7898959465212282, 0.7920598810940519, 0.794178134069242, 0.7962525209458412, 0.7982832749981293,
  0.8002721545432637, 0.8022195380836071, 0.8041263454034323, 0.8059962235028357, 0.8078285099413276,
  0.8096247869436708, 0.8113858236877268, 0.8131130758613516, 0.8148093519317566, 0.8164722246232569,
  0.8181064939945015, 0.8197109403210797, 0.8212870859567043, 0.8228370582369228, 0.8243600468039988,
  0.8258580817400792, 0.8273294355923194, 0.8287797279843069, 0.8302049402083916, 0.8316069636366766,
  0.8329868727654902, 0.8343438290619484, 0.8356785136577156, 0.8369908506797318, 0.8382820690596102,
  0.8395520902351258, 0.8408008955151042, 0.8420294992480535, 0.8432375895974693, 0.8444259956122632,
  0.8455943574812476, 0.8467436402742143, 0.8478760685801107, 0.8489881409036928, 0.850082432437893,
  0.8511595576485181, 0.852217243971938, 0.8532599606623521, 0.854285756476608, 0.8552953030623975,
  0.8562881433551333, 0.8572648918206224, 0.8582271398908299, 0.859173598763093, 0.8601052640947447,
  0.8610231982089769, 0.8619251098509846, 0.86281314504749, 0.8636857722375777, 0.8645455853053545,
  0.8653888399973249, 0.8662196712424838, 0.867033493565287, 0.8678340352172198, 0.8686200166028213,
  0.8693917403513937, 0.870148349238878, 0.870892301405271, 0.8716194779235881, 0.872333461817232,
  0.8730327973983475, 0.873717642068154, 0.8743888248755632, 0.8750437012726207, 0.875686895225581,
  0.8763136188489321, 0.8769252463023701, 0.8775234451676904, 0.8781067719301751, 0.8786759009266805,
  0.8792291047043608, 0.8797685680574191, 0.8802932378698665, 0.8808031141417025, 0.8812975824071211,
  0.8817764093918419, 0.8822426555285315, 0.8826932603845237, 0.8831273949109066, 0.8835496324100877,
  0.8839555448077421, 0.884346118554001, 0.8847227464997353, 0.8850836451185748, 0.8854289905029139,
  0.8857603808795259, 0.8860765870400276, 0.886376840083327, 0.886662456987235, 0.8869342733187009,
  0.8871880690684671};

  double yHighErr[] = {
  0.039369, 0.039191, 0.039007, 0.038816, 0.038620, 0.038418, 0.038212, 0.038001, 0.037786, 0.037569, 0.037349, 0.037127, 0.036903, 0.036676, 0.036451, 0.036225, 0.035998, 0.035773, 0.035548, 0.035325, 0.035103, 0.034882, 0.034662, 0.034442, 0.034224, 0.034006, 0.033788, 0.033571, 0.033355, 0.033140, 0.032925, 0.032710, 0.032496, 0.032283, 0.032070, 0.031857, 0.031645, 0.031434, 0.031222, 0.031011, 0.030801, 0.030591, 0.030381, 0.030172, 0.029963, 0.029754, 0.029546, 0.029338, 0.029130, 0.028922, 0.028715, 0.028507, 0.028300, 0.028093, 0.027887, 0.027680, 0.027474, 0.027268, 0.027063, 0.026857, 0.026652, 0.026447, 0.026242, 0.026037, 0.025833, 0.025629, 0.025425, 0.025221, 0.025017, 0.024813, 0.024610, 0.024407, 0.024204, 0.024001, 0.023798, 0.023596, 0.023393, 0.023191, 0.022989, 0.022787, 0.022586, 0.022384, 0.022183, 0.021982, 0.021781, 0.021581, 0.021380, 0.021180, 0.020979, 0.020779, 0.020579, 0.020379, 0.020180, 0.019980, 0.019781, 0.019582, 0.019383, 0.019184, 0.018985, 0.018786, 0.018588, 0.018389, 0.018191, 0.017993, 0.017795, 0.017597, 0.017399, 0.017201, 0.017003, 0.016805, 0.016608, 0.016410, 0.016213, 0.016015, 0.015818, 0.015620, 0.015423, 0.015225, 0.015028, 0.014831, 0.014633, 0.014436, 0.014239, 0.014042, 0.013845, 0.013648, 0.013451, 0.013254, 0.013057, 0.012860, 0.012663, 0.012466, 0.012269, 0.012072, 0.011875, 0.011678, 0.011481, 0.011284, 0.011087, 0.010890, 0.010693, 0.010495, 0.010298, 0.010101, 0.009904, 0.009707, 0.009510, 0.009313, 0.009115, 0.008918, 0.008721, 0.008524, 0.008326, 0.008129, 0.007932, 0.007735, 0.007538, 0.007340, 0.007143, 0.006946, 0.006749, 0.006551, 0.006354, 0.006157, 0.005960, 0.005763, 0.005565, 0.005368, 0.005171, 0.004974, 0.004777, 0.004579, 0.004382, 0.004185, 0.003988, 0.003791, 0.003594, 0.003396, 0.003199, 0.003002, 0.002805, 0.002607, 0.002410, 0.002213, 0.002016, 0.001819, 0.001622, 0.001424, 0.001227, 0.001030, 0.000833, 0.000636, 0.000438, 0.000241, 0.000044
  };

    double yLowErr[] = {
  0.039369, 0.039191, 0.039007, 0.038816, 0.038620, 0.038418, 0.038212, 0.038001, 0.037786, 0.037569, 0.037349, 0.037127, 0.036903, 0.036676, 0.036451, 0.036225, 0.035998, 0.035773, 0.035548, 0.035325, 0.035103, 0.034882, 0.034662, 0.034442, 0.034224, 0.034006, 0.033788, 0.033571, 0.033355, 0.033140, 0.032925, 0.032710, 0.032496, 0.032283, 0.032070, 0.031857, 0.031645, 0.031434, 0.031222, 0.031011, 0.030801, 0.030591, 0.030381, 0.030172, 0.029963, 0.029754, 0.029546, 0.029338, 0.029130, 0.028922, 0.028715, 0.028507, 0.028300, 0.028093, 0.027887, 0.027680, 0.027474, 0.027268, 0.027063, 0.026857, 0.026652, 0.026447, 0.026242, 0.026037, 0.025833, 0.025629, 0.025425, 0.025221, 0.025017, 0.024813, 0.024610, 0.024407, 0.024204, 0.024001, 0.023798, 0.023596, 0.023393, 0.023191, 0.022989, 0.022787, 0.022586, 0.022384, 0.022183, 0.021982, 0.021781, 0.021581, 0.021380, 0.021180, 0.020979, 0.020779, 0.020579, 0.020379, 0.020180, 0.019980, 0.019781, 0.019582, 0.019383, 0.019184, 0.018985, 0.018786, 0.018588, 0.018389, 0.018191, 0.017993, 0.017795, 0.017597, 0.017399, 0.017201, 0.017003, 0.016805, 0.016608, 0.016410, 0.016213, 0.016015, 0.015818, 0.015620, 0.015423, 0.015225, 0.015028, 0.014831, 0.014633, 0.014436, 0.014239, 0.014042, 0.013845, 0.013648, 0.013451, 0.013254, 0.013057, 0.012860, 0.012663, 0.012466, 0.012269, 0.012072, 0.011875, 0.011678, 0.011481, 0.011284, 0.011087, 0.010890, 0.010693, 0.010495, 0.010298, 0.010101, 0.009904, 0.009707, 0.009510, 0.009313, 0.009115, 0.008918, 0.008721, 0.008524, 0.008326, 0.008129, 0.007932, 0.007735, 0.007538, 0.007340, 0.007143, 0.006946, 0.006749, 0.006551, 0.006354, 0.006157, 0.005960, 0.005763, 0.005565, 0.005368, 0.005171, 0.004974, 0.004777, 0.004579, 0.004382, 0.004185, 0.003988, 0.003791, 0.003594, 0.003396, 0.003199, 0.003002, 0.002805, 0.002607, 0.002410, 0.002213, 0.002016, 0.001819, 0.001622, 0.001424, 0.001227, 0.001030, 0.000833, 0.000636, 0.000438, 0.000241, 0.000044
  };
 
    const int n = sizeof(x) / sizeof(double);
    CUJET_Central = new TGraph(n, x, y); 
    CUJET_Central->SetLineColor(kOrange);
    CUJET_Central->SetLineWidth(7);

    CUJET_Band = new TGraphAsymmErrors(n, x, y, nullptr, nullptr, yLowErr, yHighErr);
    CUJET_Band->SetFillColorAlpha(kOrange, 0.4); // transparent fill
    CUJET_Band->SetLineColor(kOrange);
    CUJET_Band->SetMarkerStyle(0);
  }

  //Aleksas Mazeliauskas model
  TGraph* AM_Central;
  TGraphAsymmErrors* AM_Band, *AM_PDFBand;
  {
    double x[] = {
    4.00, 4.90, 6.00, 7.40, 9.10, 11.20, 13.80, 16.90, 20.80, 25.50,
    31.40, 38.50, 47.30, 58.10, 71.40, 87.80, 107.80, 132.50, 162.80, 200.0
    };

double yNLOcentral[] = {
    0.8021390, 0.8294373, 0.8537078, 0.8781214, 0.9004462,
    0.9221054, 0.9419311, 0.9589858, 0.9758838, 0.9896587,
    1.001355, 1.010209, 1.016157, 1.019174, 1.019154,
    1.016166, 1.010302, 1.001584, 0.9902693, 0.9762275
    };

//raw data before it was parsed   
//double yNLOminus[] = {
//    0.7406830, 0.7893894, 0.8288648, 0.8625064, 0.8903059,
//    0.9146040, 0.9366592, 0.9551386, 0.9725375, 0.9858797,
//    0.9978062, 1.006701, 1.012602, 1.016208, 1.016926,
//    1.014241, 1.007832, 0.9987311, 0.9869981, 0.9727541
//    };
//parsed data (central - minus)
double yNLO_errLow[] = { 0.0614560, 0.0400479, 0.0248430, 0.0156150, 0.0101403, 0.0075014, 0.0052719, 0.0038472, 0.0033463, 0.0037790, 0.0035488, 0.0035080, 0.0035550, 0.0029660, 0.0022280, 0.0019250, 0.0024700, 0.0028529, 0.0032712, 0.0034734 };

//raw data before it was parsed by me
//double yNLOplus[] = {
//    0.8217855, 0.8456615, 0.8684628, 0.8909454, 0.9115851,
//    0.9321704, 0.9508936, 0.9674709, 0.9824504, 0.9949510,
//    1.005256, 1.012626, 1.017300, 1.019573, 1.019296,
//    1.016734, 1.011142, 1.002984, 0.9920239, 0.9782483
//    };
//parsed data (plus-central)
double yNLO_errHigh[] = { 0.0196465, 0.0162242, 0.0147550, 0.0128240, 0.0111389, 0.0100650, 0.0089625, 0.0084851, 0.0065666, 0.0052923, 0.0039010, 0.0024170, 0.0011430, 0.0003990, 0.0001420, 0.0005680, 0.0008400, 0.0014000, 0.0017546, 0.0020208 };

//raw data before it was parsed by me
//double yPDFNLOminus[] = {
//    0.6755985, 0.7163800, 0.7523398, 0.7883388, 0.8211730,
//    0.8523931, 0.8803836, 0.9043922, 0.9267284, 0.9448161,
//    0.9595758, 0.9704224, 0.9776617, 0.9815366, 0.9822806,
//    0.9801074, 0.9753157, 0.9679304, 0.9581360, 0.9457553
//    };
//parsed data (central-low)
double yNLO_errPDFLow[] = { 0.1265405, 0.1126373, 0.1013672, 0.0907876, 0.0792729, 0.0697123, 0.0615475, 0.0545956, 0.0491504, 0.0448426, 0.0415792, 0.0395876, 0.0384953, 0.0376174, 0.0368734, 0.0354099, 0.0345863, 0.0338266, 0.0315333, 0.0304722 };

//raw data before it was parsed by me
//double yPDFNLOplus[] = {
//    0.9018966, 0.9222045, 0.9405245, 0.9592083, 0.9770935,
//    0.9948901, 1.011193, 1.026353, 1.040866, 1.053748,
//    1.064179, 1.071753, 1.076273, 1.077651, 1.075797,
//    1.070673, 1.062480, 1.051197, 1.037088, 1.020031
//    };
//parsed data (high-central)
double yNLO_errPDFHigh[] = { 0.0997576, 0.0927672, 0.086447, 0.080687, 0.076264, 0.0723797, 0.0690589, 0.0676492, 0.0629822, 0.0641169, 0.0628238, 0.061544, 0.060116, 0.0584776, 0.056643, 0.054507, 0.052178, 0.049613, 0.044039, 0.043804 };

  const int n = sizeof(x) / sizeof(double);
  AM_Central = new TGraph(n, x, yNLOcentral); 
  AM_Central->SetLineColor(kGreen+1);
  AM_Central->SetLineWidth(7);

  AM_Band = new TGraphAsymmErrors(n, x, yNLOcentral, nullptr, nullptr, yNLO_errLow, yNLO_errHigh);
  AM_Band->SetFillColorAlpha(kGreen+1, 0.4); // transparent fill
  AM_Band->SetLineColor(kGreen+1);
  AM_Band->SetMarkerStyle(0);
  AM_PDFBand = new TGraphAsymmErrors(n, x, yNLOcentral, nullptr, nullptr, yNLO_errPDFLow, yNLO_errPDFHigh);
  AM_PDFBand->SetFillColorAlpha(kGreen+1, 0.4); // transparent fill
  AM_PDFBand->SetLineColor(kGreen+1);
  AM_PDFBand->SetMarkerStyle(0);
}

  //***********************************************************************
  //***********************************************************************
  //stuff for data
  float lowPtLumi = 261927.26;
  float highPtLumi = 1571563.58;

  setTDRStyle();

  //gStyle->SetErrorX(0);
  gStyle->SetOptStat(0);
  gStyle->SetLegendBorderSize(0);
  gStyle->SetStatBorderSize(0);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetPadTickY(1);
  gStyle->SetPadTickX(1);

  //load data
  TFile * f = TFile::Open("Results/pp_OO_raa_20250904.root","read");
  //get raa data
  TH1D * data = (TH1D*)f->Get("normalized_RAA");
  data->Print("All");
 
  //for testing, comment for actual data 
  //for(int i = 0; i<data->GetSize(); i++){
  // data->SetBinContent(i,356);
  // data->SetBinError(i,2.6);
  //}

  //A squared scaling
  //data->Scale(1.0/256.0);
  //data->Print("All");
  

  //load systematics
  TH1D * raaSyst = (TH1D*)f->Get("Raa_Total_uncertainty");
  raaSyst->Print("All");

  //set up canvas and pads
  TCanvas * canv2 = new TCanvas("canv2","canv2",800,800);
  canv2->SetBorderSize(0);
  canv2->SetLineWidth(0);
  canv2->SetLeftMargin(0.15);
  canv2->SetTopMargin(0.07);
  canv2->SetBorderSize(0);
  canv2->SetBottomMargin(0.15);
  canv2->SetLogx();

  //dummy histogram to define the frame
  //TH1D * ppSpecD = new TH1D("specDummy1","",3,0.4,500);
  TH1D * ppSpecD = new TH1D("specDummy1","",3,1.5,140);
  ppSpecD->GetYaxis()->SetTitle("Charged particle R_{AA}");
  ppSpecD->GetYaxis()->SetTitleOffset(1.4);
  ppSpecD->GetYaxis()->SetTitleSize(0.045);
  ppSpecD->GetYaxis()->SetLabelSize(0.04);
  ppSpecD->GetYaxis()->CenterTitle();
  ppSpecD->GetYaxis()->SetLabelOffset(0.004);
  ppSpecD->GetYaxis()->SetRangeUser(0,1.3);
  ppSpecD->GetXaxis()->SetRangeUser(1.5,140);
  ppSpecD->GetXaxis()->SetTitleFont(42);
  ppSpecD->GetXaxis()->SetTitle("p_{T} (GeV)");
  ppSpecD->GetXaxis()->SetTitleSize(0.045);
  ppSpecD->GetXaxis()->SetLabelSize(0.04);
  ppSpecD->GetXaxis()->SetTitleOffset(1.2);
  ppSpecD->GetXaxis()->CenterTitle();
  ppSpecD->Draw("");

  float OOnormUncert = 0.050;
  float NeNeNormUncert = 0.057;
  TBox * b = new TBox(1.7,1-OOnormUncert,1.8, 1+OOnormUncert);
  b->SetFillStyle(1001);
  b->SetFillColorAlpha(TColor::GetColor("#5790fc"),0.5);
  b->Draw("same");

  TBox * bNeNe = new TBox(1.8,1-NeNeNormUncert,1.9, 1+NeNeNormUncert);
  bNeNe->SetFillStyle(1001);
  bNeNe->SetFillColorAlpha(kViolet-9,0.75);
  if(doNeNe) bNeNe->Draw("same");

  TBox * bpPb = new TBox(1.8,1-pPbnormUncert,1.9, 1+pPbnormUncert);
  bpPb->SetFillStyle(1001);
  bpPb->SetFillColorAlpha(kGreen+2,0.5);
  //bpPb->Draw("same");
  TBox * bPbPb = new TBox(1.9,1-PbPbnormUncertU,2.0, 1+PbPbnormUncertD);
  bPbPb->SetFillStyle(1001);
  bPbPb->SetFillColorAlpha(kOrange,0.5);
  bPbPb->Draw("same");
  TBox * bXeXe = new TBox(2.0,1-XeXeNormUncert,2.1, 1+XeXeNormUncert);
  bXeXe->SetFillStyle(1001);
  bXeXe->SetFillColorAlpha(kRed,0.5);
  bXeXe->Draw("same");

  // Create TGraphMultiErrors
  const int N = data->GetNbinsX();
  TGraphMultiErrors *gme = new TGraphMultiErrors("gme", "Converted from TH1D", N , 2);

  for (int i = 1; i <= N; ++i) {
      double x = data->GetBinCenter(i);
      double y = data->GetBinContent(i);
      //double y = 1.0;//for RAA=1 test

      double ex = data->GetBinWidth(i) / 2.0;
      //ex = 0;//disable x errors
      
      double ey = data->GetBinError(i);//for data
      //double ey = data->GetBinError(i)/data->GetBinContent(i);//for RAA=1 test
   
      //luminosity scaling
      //if( x < 10.0){
      //  y = y/lowPtLumi;
      //  ey = ey/lowPtLumi;
      //}else{
      //  y = y/highPtLumi;
      //  ey = ey/highPtLumi;
      //} 

      //manually zero out some data points (used for blinding subsets of data)
      //if( x < 10.0){
      //  y = 0;
      //  ey = 0;
      //}else{
      //  y = y/highPtLumi;
      //  ey = ey/highPtLumi;
      //} 

      //divide by 100 and multiply by central value to get uncertainty
      double eySyst = raaSyst->GetBinContent(i)/100.0*y;

      //std::cout << ex << " " << ey << std::endl;

      // Set point and errors (name, index, value, exLow, exHigh, eyLow, eyHigh)
      gme->SetPoint(i - 1, x, y);
      gme->SetPointEX(i-1,ex,ex);
      gme->SetPointEY(i-1,0,ey,ey);
      gme->SetPointEY(i-1,1,eySyst,eySyst);
  }
  gme->Print("All");

  //cut points above 100 and below 3 GeV
  for(int i = N-1; i>-1; i--){
  //  if(gme->GetPointX(i)>100) gme->SetPoint(i,0,0);
    if(gme->GetPointX(i)<3 || gme->GetPointX(i)>100) gme->SetPoint(i,0,0);
  }

  gme->SetMarkerStyle(20);
  gme->SetMarkerSize(1.3);
  gme->SetMarkerColor(kBlack);
  gme->SetLineColor(kBlack);
  //gme->Print("All");
  gme->GetAttLine(0)->SetLineWidth(2);
  gme->GetAttLine(1)->SetLineColor(TColor::GetColor("#5790fc"));
  gme->GetAttFill(1)->SetFillColor(TColor::GetColor("#5790fc"));
  gme->GetAttFill(1)->SetFillStyle(1001);
  gme->Draw("PZs s=0.01 same;2 s=1");
  
  //drawing a selection of points for CMS data
  TGraphMultiErrors * PbPbCut = (TGraphMultiErrors*) PbPb->Clone("PbPbCut");
  for(int i = 0; i<11; i++) PbPbCut->SetPoint(i,0,0);
  for(int i = 30; i<34; i++) PbPbCut->SetPoint(i,0,0);
  PbPbCut->Draw("PZs s=0.01 same;2 s=1");
  PbPbCut->Draw("PZs s=0.01 same;X");
  TGraphMultiErrors * pPbCut = (TGraphMultiErrors*) pPb->Clone("pPbCut");
  for(int i = 0; i<13; i++) pPbCut->SetPoint(i,0,0);
  for(int i = 32; i<33; i++) pPbCut->SetPoint(i,0,0);
  //pPbCut->Draw("PZs s=0.01 same;2 s=1");
  //pPbCut->Draw("PZs s=0.01 same;X");

  TGraphMultiErrors * NeNeCut = (TGraphMultiErrors*) NeNe->Clone("NeNeCut");
  NeNeCut->Print("All");
  //for(int i = 0; i<8; i++) NeNeCut->SetPoint(i,0,0);
  for(int i = 43; i<49; i++) NeNeCut->SetPoint(i,0,0);
  if(doNeNe) NeNeCut->Draw("PZs s=0.01 same;2 s=1");
  if(doNeNe) NeNeCut->Draw("PZs s=0.01 same;X");

  TGraphMultiErrors * XeXeCut = (TGraphMultiErrors*) XeXe->Clone("XeXeCut");
  for(int i = 0; i<13; i++) XeXeCut->SetPoint(i,0,0);
  //for(int i = 29; i<30; i++) XeXeCut->SetPoint(i,0,0);
  //XeXeCut->Draw("PZs s=0.01 same;2 s=1");
  XeXeCut->Draw("PZs s=0.01 same;X");
  // Draw box outlines manually
    for (int i = 13; i < 29; i++) {
        double xL = XeXeCut->GetPointX(i) - XeXe_exSyst[i+1];
        double xR = XeXeCut->GetPointX(i) + XeXe_exSyst[i+1];
        double yB = XeXeCut->GetPointY(i) - XeXe_eySyst[i+1];
        double yT = XeXeCut->GetPointY(i) + XeXe_eySyst[i+1];

        // Four border lines
        auto top    = new TLine(xL, yT, xR, yT);
        auto bottom = new TLine(xL, yB, xR, yB);
        auto left   = new TLine(xL, yB, xL, yT);
        auto right  = new TLine(xR, yB, xR, yT);

        for (auto line : {top, bottom, left, right}) {
            line->SetLineColor(TColor::GetColor("#e42536"));
            line->SetLineWidth(2);
            line->Draw("same");
        }
    }

  TLine * l = new TLine(1.5,1,140,1);
  l->SetLineStyle(2);
  l->Draw("same");
  
  gme->Draw("PZs s=0.01 same;X");

  //drawing data
  //raa->SetMarkerStyle(8);
  //raa->SetMarkerColor(kBlack);
  //raa->SetLineWidth(2);
  //raa->SetLineColor(kBlack);
  //raa->GetXaxis()->SetRangeUser(3,100);
  //raa->Draw("p same");

  //legends
  TLegend * specLeg = new TLegend(0.75,0.2,0.95,0.25);
  specLeg->SetTextFont(42);
  specLeg->SetTextSize(0.05);
  specLeg->SetFillStyle(0);
  specLeg->AddEntry((TObject*)0,"|#eta| < 1",""); 
  specLeg->SetFillStyle(0);
  specLeg->Draw("same"); 

  TLegend * RAASummaryLeg = new TLegend(0.38,0.8,0.95,0.91);
  RAASummaryLeg->SetTextFont(42);
  RAASummaryLeg->SetTextSize(0.026);
  RAASummaryLeg->SetFillStyle(0);
  pPb->SetFillColor(kGreen+2);
  gme->SetFillColor(TColor::GetColor("#5790fc"));
  PbPb->SetFillColor(kOrange);
  RAASummaryLeg->SetNColumns(2);
  RAASummaryLeg->AddEntry(gme,"OO (5.36 TeV)","fp"); 
  if(doNeNe) RAASummaryLeg->AddEntry(NeNe,"NeNe (5.36 TeV)","fp");
  RAASummaryLeg->AddEntry(PbPb,"PbPb (5.02 TeV)","fp");
  //RAASummaryLeg->AddEntry(pPb,"pPb (5.02 TeV)","fp");
  RAASummaryLeg->AddEntry(XeXe,"XeXe (5.44 TeV, 0-80%)","fp");
  RAASummaryLeg->Draw("same");

  //int iPeriod = 0;
  lumi_sqrtS = "0.5 nb^{-1} (5.36 TeV OO), 1.07 pb^{-1} (5.36 TeV pp)";
  //writeExtraText = true;  
  //extraText  = "Preliminary";
  if(!doNeNe) CMS_lumi( canv2, 0,11);
  if(doNeNe) CMS_lumi( canv2, 13,11);
  
  canv2->RedrawAxis();

  if(!doNeNe){
  canv2->SaveAs("plots/Figure_002.pdf");
  canv2->SaveAs("plots/Figure_002.png");
  canv2->SaveAs("plots/Figure_002.C");
  } else {
  canv2->SaveAs("plots/Figure_002_NeNe.pdf");
  canv2->SaveAs("plots/Figure_002_NeNe.png");
  canv2->SaveAs("plots/Figure_002_NeNe.C");
  }

  //plot with other datasets
  TH1D * ppSpecD2 = new TH1D("specDummy2","",3,0.4,500);
  ppSpecD2->GetYaxis()->SetTitle("Charged particle R_{AA}");
  ppSpecD2->GetYaxis()->SetTitleOffset(1.4);
  ppSpecD2->GetYaxis()->SetTitleSize(0.045);
  ppSpecD2->GetYaxis()->SetLabelSize(0.04);
  ppSpecD2->GetYaxis()->CenterTitle();
  ppSpecD2->GetYaxis()->SetLabelOffset(0.004);
  ppSpecD2->GetYaxis()->SetRangeUser(0,2.0);
  ppSpecD2->GetXaxis()->SetRangeUser(0.4,500);
  ppSpecD2->GetXaxis()->SetTitleFont(42);
  ppSpecD2->GetXaxis()->SetTitle("p_{T} (GeV)");
  ppSpecD2->GetXaxis()->SetTitleSize(0.045);
  ppSpecD2->GetXaxis()->SetLabelSize(0.04);
  ppSpecD2->GetXaxis()->SetTitleOffset(1.2);
  ppSpecD2->GetXaxis()->CenterTitle();
  ppSpecD2->Draw("");

  PbPb->Draw("PZs s=0.01 same;2 s=1");
  PbPb->Draw("PZs s=0.01 same;X");
  pPb->Draw("PZs s=0.01 same;2 s=1");
  pPb->Draw("PZs s=0.01 same;X");
  gme->Draw("PZs s=0.01 same;2 s=1");
  TBox * b2 = new TBox(0.5,1-OOnormUncert,0.55, 1+OOnormUncert);
  b2->SetFillStyle(1001);
  b2->SetFillColorAlpha(TColor::GetColor("#5790fc"),0.5);
  b2->Draw("same");
  TBox * b2NeNe = new TBox(0.45,1-NeNeNormUncert,0.5, 1+NeNeNormUncert);
  b2NeNe->SetFillStyle(1001);
  b2NeNe->SetFillColorAlpha(kViolet-9,0.75);
  if(doNeNe) b2NeNe->Draw("same");
  TBox * b2pPb = new TBox(0.55,1-pPbnormUncert,0.6, 1+pPbnormUncert);
  b2pPb->SetFillStyle(1001);
  b2pPb->SetFillColorAlpha(kGreen+2,0.5);
  b2pPb->Draw("same");
  TBox * b2PbPb = new TBox(0.6,1-PbPbnormUncertU,0.65, 1+PbPbnormUncertD);
  b2PbPb->SetFillStyle(1001);
  b2PbPb->SetFillColorAlpha(kOrange,0.5);
  b2PbPb->Draw("same");
  TBox * b2XeXe = new TBox(0.65,1-XeXeNormUncert,0.7, 1+XeXeNormUncert);
  b2XeXe->SetFillStyle(1001);
  b2XeXe->SetFillColorAlpha(kRed,0.5);
  b2XeXe->Draw("same");
  TLine * l2 = new TLine(0.4,1,500,1);
  l2->SetLineStyle(2);
  l2->Draw("same");
  gme->Draw("PZs s=0.01 same;X");

  XeXe->Draw("PZs s=0.01 same;X");
  // Draw box outlines manually
    for (int i = 0; i < 29; i++) {
        double xL = XeXe->GetPointX(i) - XeXe_exSyst[i+1];
        double xR = XeXe->GetPointX(i) + XeXe_exSyst[i+1];
        double yB = XeXe->GetPointY(i) - XeXe_eySyst[i+1];
        double yT = XeXe->GetPointY(i) + XeXe_eySyst[i+1];

        // Four border lines
        auto top    = new TLine(xL, yT, xR, yT);
        auto bottom = new TLine(xL, yB, xR, yB);
        auto left   = new TLine(xL, yB, xL, yT);
        auto right  = new TLine(xR, yB, xR, yT);

        for (auto line : {top, bottom, left, right}) {
            line->SetLineColor(TColor::GetColor("#e42536"));
            line->SetLineWidth(2);
            line->Draw("same");
        }
    }
  if(doNeNe){
    NeNeCut->Draw("PZs s=0.01 same;2 s=1"); 
    NeNeCut->Draw("PZs s=0.01 same;X");
  }

  if(!doNeNe) CMS_lumi( canv2, 0,11);
  if(doNeNe) CMS_lumi( canv2, 13,11);
  specLeg->Draw("same");
  RAASummaryLeg->Draw("same");
  canv2->RedrawAxis();

  if(!doNeNe){
  canv2->SaveAs("plots/Figure_002_compare.pdf");
  canv2->SaveAs("plots/Figure_002_compare.png");
  canv2->SaveAs("plots/Figure_002_compare.C");
   } else {
  canv2->SaveAs("plots/Figure_002_compare_NeNe.pdf");
  canv2->SaveAs("plots/Figure_002_compare_NeNe.png");
  canv2->SaveAs("plots/Figure_002_compare_NeNe.C");
  }

  //plot with no-quenching baselines
  //redraw result
  
  TH1D * ppSpecD3 = new TH1D("specDummy3","",3,2.2,120);
  ppSpecD3->GetYaxis()->SetTitle("Charged particle R_{AA}");
  ppSpecD3->GetYaxis()->SetTitleOffset(1.4);
  ppSpecD3->GetYaxis()->SetTitleSize(0.045);
  ppSpecD3->GetYaxis()->SetLabelSize(0.04);
  ppSpecD3->GetYaxis()->CenterTitle();
  ppSpecD3->GetYaxis()->SetLabelOffset(0.004);
  ppSpecD3->GetYaxis()->SetRangeUser(0,2.0);
  ppSpecD3->GetXaxis()->SetRangeUser(0.4,500);
  ppSpecD3->GetXaxis()->SetTitleFont(42);
  ppSpecD3->GetXaxis()->SetTitle("p_{T} (GeV)");
  ppSpecD3->GetXaxis()->SetTitleSize(0.045);
  ppSpecD3->GetXaxis()->SetLabelSize(0.04);
  ppSpecD3->GetXaxis()->SetTitleOffset(1.2);
  ppSpecD3->GetXaxis()->CenterTitle();
  ppSpecD3->GetXaxis()->SetRangeUser(2.2,120);
  ppSpecD3->GetYaxis()->SetRangeUser(0.5,1.5);
  ppSpecD3->Draw();


  TBox * b3 = new TBox(2.4,1-OOnormUncert,2.5, 1+OOnormUncert);
  b3->SetFillStyle(1001);
  b3->SetFillColorAlpha(TColor::GetColor("#5790fc"),0.5);
  b3->Draw("same");
  gme->Draw("PZs s=0.01 same;2 s=1");

  AM_Band->Draw("3 same");
  AM_PDFBand->Draw("3 same");
  AM_Central->Draw("L same");

  z2->Draw("same");

  //redraw some elements on top
  TLine * l3 = new TLine(2.2,1,120,1);
  l3->SetLineStyle(2);
  l3->Draw("same");
  gme->Draw("PZs s=0.01 same;X");

  TLegend * specLegNoQ = new TLegend(0.4,0.75,0.9,0.9);
  specLegNoQ->SetTextFont(42);
  specLegNoQ->SetTextSize(0.03);
  specLegNoQ->SetFillStyle(0);
  specLegNoQ->AddEntry((TObject*)0,"Baseline models (no energy loss)",""); 
  specLegNoQ->AddEntry(AM_Central,"NLO pQCD + EPPS21 (Huss et al.)","l"); 
  specLegNoQ->AddEntry(z2,"LO pQCD + EPS09 (Zakharov)","l"); 
  specLegNoQ->SetFillStyle(0);
  specLegNoQ->Draw("same"); 

  specLeg->Draw("same"); 
  CMS_lumi( canv2, 0,11);
  canv2->RedrawAxis();

  canv2->SaveAs("plots/Figure_003-a.pdf");
  canv2->SaveAs("plots/Figure_003-a.png");
  canv2->SaveAs("plots/Figure_003-a.C");


  //plot with quenching models
  //redraw result
  ppSpecD3->GetYaxis()->SetRangeUser(0.5,1.5);
  ppSpecD3->Draw();
  b3->Draw("same");
  gme->Draw("PZs s=0.01 same;2 s=1");

  XZ_Band->Draw("3 same");
  XZ_Central->Draw("L same");

  huss_Band->Draw("3 same");
  huss_Central->Draw("L same");

  CUJET_Band->Draw("3 same");
  CUJET_Central->Draw("L same");

  z1->Draw("same");
  z3->Draw("same");

  TLegend * specLegWQ = new TLegend(0.4,0.6,0.9,0.9);
  specLegWQ->SetTextFont(42);
  specLegWQ->SetTextSize(0.03);
  specLegWQ->SetFillStyle(0);
  specLegWQ->AddEntry((TObject*)0,"Energy loss models",""); 
  specLegWQ->AddEntry(huss_Central,"BDMPS-Z (Huss et al.)","lf"); 
  specLegWQ->AddEntry(z1,"LCPI + mQGP in pp (Zakharov)","l"); 
  specLegWQ->AddEntry(z3,"LCPI + no mQGP in pp (Zakharov)","l"); 
  specLegWQ->AddEntry(XZ_Central,"pQCD+Bayesian #hat{q} (Xie and Zhang)","lf"); 
  specLegWQ->AddEntry(CUJET_Central,"CUJET/CIBJET","lf"); 
  specLegWQ->SetFillStyle(0);
  specLegWQ->Draw("same"); 

  //redraw some elements on top
  l3->Draw("same");
  gme->Draw("PZs s=0.01 same;X");
  specLeg->Draw("same"); 
  CMS_lumi( canv2, 0,11);
  canv2->RedrawAxis();

  canv2->SaveAs("plots/Figure_003-b.pdf");
  canv2->SaveAs("plots/Figure_003-b.png");
  canv2->SaveAs("plots/Figure_003-b.C");
}
