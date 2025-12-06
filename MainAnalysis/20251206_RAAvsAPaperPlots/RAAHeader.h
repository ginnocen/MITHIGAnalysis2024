
// == LUMI AND GLOBAL UNCERTAINTIES ===========================================
// 2023 ppRef
const TString lumipp23        = "6.2 pb^{-1}";
const float   lumiUncertpp23  = 0.03;
// OO
const TString lumiOO          = "6.2 nb^{-1}";
const float   lumiUncertOO    = 0.04;
const float   raaUncertOO     = 0.05;
// NeNe
const TString lumiNeNe        = "0.62 nb^{-1}";
const float   lumiUncertNeNe  = 0.048; // TODO: CHECK THIS!
const float   raaUncertNeNe   = 0.0566;
// 2018 ppRef
const TString lumipp18        = "6.2 pb^{-1}";
const float   lumiUncertpp18  = 0.023; // TODO: CHECK THIS!
// XeXe
const TString lumiXeXe        = "0.62 nb^{-1}";
const float   lumiUncertXeXe  = 0.048;
const float   raaUncertXeXe   = 0.0566;
// PbPb
const TString lumiPbPb        = "0.62 nb^{-1}";
const float   lumiUncertPbPb  = 0.048;
const float   raaUncertPbPb   = 0.0566;

vector<float> ptBins = {
   3.2,  4.0,  4.8,  5.6,  6.4,
   7.2,  9.6, 12.0, 14.4, 19.2,
  24.0, 28.8, 35.2, 48.0, 73.6,
  103.6
};


// OO Styles
const int colorOO = TColor::GetColor("#5790fc");
const int colorOOSyst = TColor::GetColor("#5790fc");
const float alphaOOSyst = 0.8;
const int markerOO = 20;

// NeNe Styles
const int colorNeNe = kViolet+2;
const int colorNeNeSyst = kViolet-9;
const float alphaNeNeSyst = 0.75;
const int markerNeNe = 21;

// XeXe Styles
const int colorXeXe = kViolet+2;
const int colorXeXeSyst = kViolet-9;
const float alphaXeXeSyst = 0.75;
const int markerXeXe = 24;

// PbPb Styles
const int colorPbPb = kViolet+2;
const int colorPbPbSyst = kViolet-9;
const float alphaPbPbSyst = 0.75;
const int markerPbPb = 24;

// pPb Styles
const int colorpPb = kViolet+2;
const int colorpPbSyst = kViolet-9;
const float alphapPbSyst = 0.75;
const int markerpPb = 24;
