#ifndef tnp_weight_lowptPbPb_h
#define tnp_weight_lowptPbPb_h
#include "TMath.h"

// IN THIS FILE YOU WILL FIND:
// +++++++++++++++++++++++++++++++++++++++
// - Trigger: (tnp_weight_trg_pbpb)
//   * filterId = 0: Jpsi L2 filter
//   * filterId = 1: Jpsi L3 filter
//   * filterId = 2: Upsi L2 filter
//   * filterId = 3: Upsi L3 filter
//   * idx = 0:  nominal
//   * idx = -1: syst variation, +1 sigma
//   * idx = -2: syst variation, -1 sigma
//   * idx = +1: stat variation, +1 sigma
//   * idx = +2: stat variation, -1 sigma
//   * idx = 99: syst variation, tag selection
// - MuID: (tnp_weight_muid_pbpb)
//   * idx = 0:  nominal
//   * idx = -1: syst variation, +1 sigma
//   * idx = -2: syst variation, -1 sigma
//   * idx = +1: stat variation, +1 sigma
//   * idx = +2: stat variation, -1 sigma
//   * idx = 99: syst variation, tag selection
// - Inner tracking: (tnp_weight_trk_pbpb)
//   * idx = 0:  nominal
//   * idx = -1: syst variation, +1 sigma
//   * idx = -2: syst variation, -1 sigma
//   * idx = +1: stat variation, +1 sigma
//   * idx = +2: stat variation, -1 sigma
//   * idx = 99: syst variation, tag selection
// +++++++++++++++++++++++++++++++++++++++

double tnp_weight_muid_pbpb(double pt, double eta, int idx=0);
double tnp_weight_trk_pbpb(double eta, int idx=0);
double tnp_weight_trg_pbpb(double pt, double eta, int filterId=0,int idx=0);

///////////////////////////////////////////////////
//             M u I D    P b P b                //
///////////////////////////////////////////////////

double tnp_weight_muid_pbpb(double pt, double eta, int idx) {
  double x = pt;
  double num=1, den=1, syst=0, statUp=0, statDown=0;
  if (idx != 99) {
    // SF for 0 < |eta| < 1.2
    if (fabs(eta) >= 0 && fabs(eta) < 1.2) {
      if (x >= 3.5 && x <4) {num = 0.950063; den = 0.949953; statUp = 0.00533472; statDown = 0.0055493;}
      else if (x >= 4 && x <4.5) {num = 0.967001; den = 0.974793; statUp = 0.0044534; statDown = 0.00469609;}
      else if (x >= 4.5 && x <5) {num = 0.985975; den = 0.984666; statUp = 0.00409292; statDown = 0.00441588;}
      else if (x >= 5 && x <5.5) {num = 0.98654; den = 0.988924; statUp = 0.0042279; statDown = 0.00461031;}
      else if (x >= 5.5 && x <6.5) {num = 0.9864; den = 0.991143; statUp = 0.00318379; statDown = 0.00343892;}
      else if (x >= 6.5 && x <8) {num = 0.989987; den = 0.992602; statUp = 0.00284886; statDown = 0.00315788;}
      else if (x >= 8 && x <10.5) {num = 0.987301; den = 0.988221; statUp = 0.00346155; statDown = 0.00384835;}
      else if (x >= 10.5 && x <14) {num = 0.975692; den = 0.968807; statUp = 0.00608607; statDown = 0.00679025;}
      else if (x >= 14 && x <18) {num = 0.982855; den = 0.979757; statUp = 0.00843435; statDown = 0.0101701;}
      else {num = 0.983936; den = 0.983559; statUp = 0.00986974; statDown = 0.0122188;}
    }
    // SF for 1.2 < |eta| < 1.8
    if (fabs(eta) >= 1.2 && fabs(eta) < 1.8) {
      if (x >= 2.07 && x <3) {num = 0.984913; den = 0.981057; statUp = 0.009598; statDown = 0.00989626;}
      else if (x >= 3 && x <3.5) {num = 0.988701; den = 0.984902; statUp = 0.00593884; statDown = 0.00627794;}
      else if (x >= 3.5 && x <4) {num = 0.977074; den = 0.988051; statUp = 0.00583354; statDown = 0.00624515;}
      else if (x >= 4 && x <4.5) {num = 0.993705; den = 0.992735; statUp = 0.00425025; statDown = 0.00479705;}
      else if (x >= 4.5 && x <5) {num = 0.995157; den = 0.990413; statUp = 0.00463177; statDown = 0.00529916;}
      else if (x >= 5 && x <6) {num = 0.985933; den = 0.994153; statUp = 0.00456591; statDown = 0.00512391;}
      else if (x >= 6 && x <7.5) {num = 0.991523; den = 0.997056; statUp = 0.00437948; statDown = 0.00503813;}
      else if (x >= 7.5 && x <10) {num = 0.995197; den = 0.995474; statUp = 0.00380375; statDown = 0.0047413;}
      else if (x >= 10 && x <15) {num = 0.983951; den = 0.973431; statUp = 0.00832074; statDown = 0.00955641;}
      else {num = 0.971597; den = 0.983348; statUp = 0.0137801; statDown = 0.0166571;}
    }
    // SF for 1.8 < |eta| < 2.1
    if (fabs(eta) >= 1.8 && fabs(eta) < 2.1) {
      if (x >= 1.5 && x <2.5) {num = 0.979164; den = 0.947982; statUp = 0.0167706; statDown = 0.0168448;}
      else if (x >= 2.5 && x <3) {num = 0.995171; den = 0.985696; statUp = 0.00482874; statDown = 0.00808571;}
      else if (x >= 3 && x <3.5) {num = 0.988273; den = 0.991548; statUp = 0.00658297; statDown = 0.00748573;}
      else if (x >= 3.5 && x <4) {num = 0.980973; den = 0.996485; statUp = 0.00655784; statDown = 0.00772284;}
      else if (x >= 4 && x <4.5) {num = 1; den = 0.997746; statUp = 9.19584e-10; statDown = 0.00182312;}
      else if (x >= 4.5 && x <5.5) {num = 0.996082; den = 0.998014; statUp = 0.00337514; statDown = 0.00443746;}
      else if (x >= 5.5 && x <7) {num = 1; den = 0.998654; statUp = 2.54058e-09; statDown = 0.00317007;}
      else if (x >= 7 && x <9) {num = 0.99108; den = 0.999533; statUp = 0.00590722; statDown = 0.00727279;}
      else if (x >= 9 && x <12) {num = 1; den = 0.997779; statUp = 8.37862e-09; statDown = 0.00331019;}
      else {num = 1; den = 0.996649; statUp = 4.39015e-10; statDown = 0.00470679;}
    }
    // SF for 2.1 < |eta| < 2.4
    if (fabs(eta) >= 2.1 && fabs(eta) < 2.4) {
      if (x >= 1.5 && x <2.2) {num = 0.910379; den = 0.905616; statUp = 0.0266485; statDown = 0.0260023;}
      else if (x >= 2.2 && x <2.7) {num = 0.961502; den = 0.962387; statUp = 0.014631; statDown = 0.015103;}
      else if (x >= 2.7 && x <3.2) {num = 0.972829; den = 0.976239; statUp = 0.0116492; statDown = 0.0124969;}
      else if (x >= 3.2 && x <3.7) {num = 0.983185; den = 0.982425; statUp = 0.00885052; statDown = 0;}
      else if (x >= 3.7 && x <4.7) {num = 0.996796; den = 0.99483; statUp = 0.0032042; statDown = 0.00675329;}
      else if (x >= 4.7 && x <8) {num = 1; den = 0.997321; statUp = 1.92743e-08; statDown = 0.00395103;}
      else if (x >= 8 && x <11) {num = 1; den = 0.998724; statUp = 9.48282e-09; statDown = 0.00535409;}
      else if (x >= 11 && x <14) {num = 1; den = 0.997692; statUp = 7.74356e-09; statDown = 0.0128769;}
      else {num = 1; den = 0.993497; statUp = 7.83633e-08; statDown = 0.025201;}
    }

    if (fabs(eta) >= 0 && fabs(eta) < 1.2) {
      // syst uncertainties
      if (x >= 3.5 && x < 4) syst = 0.00176394;
      else if (x >= 4 && x < 4.5) syst = 0.00111834;
      else if (x >= 4.5 && x < 5) syst = 0.000449398;
      else if (x >= 5 && x < 5.5) syst = 0.00112726;
      else if (x >= 5.5 && x < 6.5) syst = 0.000776533;
      else if (x >= 6.5 && x < 8) syst = 0.000911058;
      else if (x >= 8 && x < 10.5) syst = 0.00203964;
      else if (x >= 10.5 && x < 14) syst = 0.00287311;
      else if (x >= 14 && x < 18) syst = 0.00131869;
      else syst = 0.00605208;
    }
    else if (fabs(eta) >= 1.2 && fabs(eta) < 1.8) {
      // syst uncertainties
      if (x >= 2.07 && x < 3) syst = 0.00349676;
      else if (x >= 3 && x < 3.5) syst = 0.00324231;
      else if (x >= 3.5 && x < 4) syst = 0.00236393;
      else if (x >= 4 && x < 4.5) syst = 0.00126545;
      else if (x >= 4.5 && x < 5) syst = 0.00202288;
      else if (x >= 5 && x < 6) syst = 0.00109481;
      else if (x >= 6 && x < 7.5) syst = 0.00247103;
      else if (x >= 7.5 && x < 10) syst = 0.00118213;
      else if (x >= 10 && x < 15) syst = 0.00147647;
      else syst = 0.00512414;
    }
    else if (fabs(eta) >= 1.8 && fabs(eta) < 2.1) {
      // syst uncertainties
      if (x >= 1.5 && x < 2.5) syst = 0.00490325;
      else if (x >= 2.5 && x < 3) syst = 0.00204865;
      else if (x >= 3 && x < 3.5) syst = 0.00032749;
      else if (x >= 3.5 && x < 4) syst = 0.0048872;
      else if (x >= 4 && x < 4.5) syst = 2.14126e-08;
      else if (x >= 4.5 && x < 5.5) syst = 0.000498982;
      else if (x >= 5.5 && x < 7) syst = 2.61329e-08;
      else if (x >= 7 && x < 9) syst = 0.00518478;
      else if (x >= 9 && x < 12) syst = 8.3503e-09;
      else syst = 7.88309e-09;
    }
    else if (fabs(eta) >= 2.1 && fabs(eta) < 2.4) {
      // syst uncertainties
      if (x >= 1.5 && x < 2.2) syst = 0.00823075;
      else if (x >= 2.2 && x < 2.7) syst = 0.0150309;
      else if (x >= 2.7 && x < 3.2) syst = 0.00542403;
      else if (x >= 3.2 && x < 3.7) syst = 0.00407213;
      else if (x >= 3.7 && x < 4.7) syst = 0.00269522;
      else if (x >= 4.7 && x < 8) syst = 0.0057144;
      else if (x >= 8 && x < 11) syst = 9.46919e-09;
      else if (x >= 11 && x < 14) syst = 7.74356e-09;
      else syst = 7.82098e-08;
    }
  }
  else {
    // SF for 0 < |eta| < 1.2
    if (fabs(eta) >= 0 && fabs(eta) < 1.2) {
      if (x >= 3.5 && x <4) {num = 0.925202; den = 0.950687; statUp = 0.00972645; statDown = 0.010175;}
      else if (x >= 4 && x <4.5) {num = 0.973254; den = 0.975632; statUp = 0.00726942; statDown = 0.00786806;}
      else if (x >= 4.5 && x <5) {num = 0.982892; den = 0.984034; statUp = 0.0065814; statDown = 0.00735102;}
      else if (x >= 5 && x <5.5) {num = 0.996676; den = 0.989647; statUp = 0.00332367; statDown = 0.00770647;}
      else if (x >= 5.5 && x <6.5) {num = 0.991131; den = 0.990874; statUp = 0.00514469; statDown = 0.00597022;}
      else if (x >= 6.5 && x <8) {num = 0.987993; den = 0.992506; statUp = 0.0053893; statDown = 0.00626197;}
      else if (x >= 8 && x <10.5) {num = 0.985741; den = 0.988617; statUp = 0.00606416; statDown = 0.00720336;}
      else if (x >= 10.5 && x <14) {num = 0.967462; den = 0.968527; statUp = 0.0118984; statDown = 0.0140123;}
      else if (x >= 14 && x <18) {num = 0.971597; den = 0.979222; statUp = 0.0210756; statDown = 0.0263842;}
      else {num = 0.939779; den = 0.98371; statUp = 0.0358796; statDown = 0.0421902;}
    }
    // SF for 1.2 < |eta| < 1.8
    if (fabs(eta) >= 1.2 && fabs(eta) < 1.8) {
      if (x >= 2.07 && x <3) {num = 0.984807; den = 0.981567; statUp = 0.0141147; statDown = 0.0144792;}
      else if (x >= 3 && x <3.5) {num = 0.976516; den = 0.984952; statUp = 0.00931142; statDown = 0.00978553;}
      else if (x >= 3.5 && x <4) {num = 0.990132; den = 0.989033; statUp = 0.0077507; statDown = 0.00831505;}
      else if (x >= 4 && x <4.5) {num = 0.979716; den = 0.992265; statUp = 0.00849793; statDown = 0.00934226;}
      else if (x >= 4.5 && x <5) {num = 1; den = 0.991969; statUp = 1.55101e-10; statDown = 0.00834803;}
      else if (x >= 5 && x <6) {num = 0.998619; den = 0.995108; statUp = 0.00138079; statDown = 0.00726117;}
      else if (x >= 6 && x <7.5) {num = 0.994131; den = 0.996947; statUp = 0.00586887; statDown = 0.00788471;}
      else if (x >= 7.5 && x <10) {num = 1; den = 0.996339; statUp = 7.59031e-08; statDown = 0.00282066;}
      else if (x >= 10 && x <15) {num = 0.999981; den = 0.972463; statUp = 1.894e-05; statDown = 0.0138268;}
      else {num = 0.972657; den = 0.985482; statUp = 0.0273431; statDown = 0.0472594;}
    }
    // SF for 1.8 < |eta| < 2.1
    if (fabs(eta) >= 1.8 && fabs(eta) < 2.1) {
      if (x >= 1.5 && x <2.5) {num = 0.788854; den = 0.953099; statUp = 0.0186173; statDown = 0.0184554;}
      else if (x >= 2.5 && x <3) {num = 0.983117; den = 0.984468; statUp = 0.011386; statDown = 0.0121255;}
      else if (x >= 3 && x <3.5) {num = 0.990054; den = 0.991599; statUp = 0.00994628; statDown = 0.0118987;}
      else if (x >= 3.5 && x <4) {num = 0.99577; den = 0.9958; statUp = 0.00423029; statDown = 0.0102048;}
      else if (x >= 4 && x <4.5) {num = 0.991044; den = 0.997323; statUp = 0.00702365; statDown = 0.00950553;}
      else if (x >= 4.5 && x <5.5) {num = 0.995896; den = 0.998046; statUp = 0.00410396; statDown = 0.00665801;}
      else if (x >= 5.5 && x <7) {num = 1; den = 0.998865; statUp = 2.45315e-07; statDown = 0.00646251;}
      else if (x >= 7 && x <9) {num = 0.993992; den = 0.999619; statUp = 0.00600798; statDown = 0.0141094;}
      else if (x >= 9 && x <12) {num = 1; den = 0.997037; statUp = 1.07078e-09; statDown = 0.010506;}
      else {num = 0.938537; den = 0.997466; statUp = 0.0310024; statDown = 0.0351368;}
    }
    // SF for 2.1 < |eta| < 2.4
    if (fabs(eta) >= 2.1 && fabs(eta) < 2.4) {
      if (x >= 1.5 && x <2.2) {num = 0.87859; den = 0.906667; statUp = 0.0288182; statDown = 0.0280427;}
      else if (x >= 2.2 && x <2.7) {num = 0.923374; den = 0.967495; statUp = 0.0204527; statDown = 0.0208072;}
      else if (x >= 2.7 && x <3.2) {num = 0.981304; den = 0.979751; statUp = 0.0166669; statDown = 0.0174863;}
      else if (x >= 3.2 && x <3.7) {num = 0.969957; den = 0.987414; statUp = 0.0134843; statDown = 0.0145909;}
      else if (x >= 3.7 && x <4.7) {num = 0.991971; den = 0.993668; statUp = 0.00802857; statDown = 0.0105704;}
      else if (x >= 4.7 && x <8) {num = 0.991034; den = 0.997648; statUp = 0.00781413; statDown = 0.00932118;}
      else if (x >= 8 && x <11) {num = 1; den = 0.998833; statUp = 1.58429e-13; statDown = 0.00513946;}
      else if (x >= 11 && x <14) {num = 0.956272; den = 0.998051; statUp = 0.0437282; statDown = 0.0631049;}
      else {num = 1; den = 0.994701; statUp = 1.1911e-07; statDown = 0.0477533;}
    }
  }

  double syst_factor = 0; double stat_factor = 0;
  if (idx == -1) syst_factor = syst;
  else if (idx == -2) syst_factor = -1*syst;
  else if (idx == +1) stat_factor = statUp;
  else if (idx == +2) stat_factor = -1*statDown;
  return ((num+syst_factor+stat_factor)/den);
}

///////////////////////////////////////////////////
//              T R G     P b P b                //
///////////////////////////////////////////////////

double tnp_weight_trg_pbpb(double pt, double eta, int filterId,int idx) {
  double x = pt;
  double num=1, den=1, syst=0, statUp=0, statDown=0;
  if (filterId==0) { //L2 Jpsi
    if (idx != 99) {
      // SF for 0 < |eta| < 1.2
      if (fabs(eta) >= 0 && fabs(eta) < 1.2) {
	if (x >= 3.5 && x <4) {num = 0.683516; den = 0.624883; statUp = 0.0108066; statDown = 0.0109176;}
	else if (x >= 4 && x <4.5) {num = 0.864766; den = 0.844349; statUp = 0.00811031; statDown = 0.00834878;}
	else if (x >= 4.5 && x <5) {num = 0.93532; den = 0.903951; statUp = 0.00633481; statDown = 0.00667111;}
	else if (x >= 5 && x <5.5) {num = 0.938106; den = 0.923036; statUp = 0.00664483; statDown = 0.0070676;}
	else if (x >= 5.5 && x <6.5) {num = 0.961913; den = 0.940699; statUp = 0.00441524; statDown = 0.00471286;}
	else if (x >= 6.5 && x <8) {num = 0.955209; den = 0.958559; statUp = 0.00486781; statDown = 0.00520504;}
	else if (x >= 8 && x <10.5) {num = 0.960097; den = 0.96542; statUp = 0.00522745; statDown = 0.00566401;}
	else if (x >= 10.5 && x <14) {num = 0.953912; den = 0.966812; statUp = 0.00737412; statDown = 0.0081774;}
	else if (x >= 14 && x <18) {num = 0.958011; den = 0.96975; statUp = 0.0115548; statDown = 0.0135024;}
	else {num = 0.958729; den = 0.964144; statUp = 0.0126207; statDown = 0.0154877;}
      }
      // SF for 1.2 < |eta| < 1.8
      if (fabs(eta) >= 1.2 && fabs(eta) < 1.8) {
	if (x >= 2.07 && x <3) {num = 0.69446; den = 0.640187; statUp = 0.0166834; statDown = 0.0168495;}
	else if (x >= 3 && x <3.5) {num = 0.828672; den = 0.79077; statUp = 0.0116385; statDown = 0.0119271;}
	else if (x >= 3.5 && x <4) {num = 0.914934; den = 0.865937; statUp = 0.00898234; statDown = 0.00943628;}
	else if (x >= 4 && x <4.5) {num = 0.926217; den = 0.912889; statUp = 0.00887486; statDown = 0.00942155;}
	else if (x >= 4.5 && x <5) {num = 0.940711; den = 0.933298; statUp = 0.00888396; statDown = 0.00963798;}
	else if (x >= 5 && x <6) {num = 0.962868; den = 0.949988; statUp = 0.00656083; statDown = 0.00720576;}
	else if (x >= 6 && x <7.5) {num = 0.940584; den = 0.959158; statUp = 0.00854681; statDown = 0.00928998;}
	else if (x >= 7.5 && x <10) {num = 0.950557; den = 0.958603; statUp = 0.00907201; statDown = 0.0101255;}
	else if (x >= 10 && x <15) {num = 0.958968; den = 0.96132; statUp = 0.0101043; statDown = 0.0116835;}
	else {num = 0.932633; den = 0.950433; statUp = 0.0169327; statDown = 0.0182775;}
      }
      // SF for 1.8 < |eta| < 2.1
      if (fabs(eta) >= 1.8 && fabs(eta) < 2.1) {
	if (x >= 1.5 && x <2.5) {num = 0.69714; den = 0.653685; statUp = 0.0201021; statDown = 0.020174;}
	else if (x >= 2.5 && x <3) {num = 0.906875; den = 0.867753; statUp = 0.0141174; statDown = 0.0148529;}
	else if (x >= 3 && x <3.5) {num = 0.919454; den = 0.903362; statUp = 0.0134254; statDown = 0.0144224;}
	else if (x >= 3.5 && x <4) {num = 0.946926; den = 0.922588; statUp = 0.0128289; statDown = 0.0140971;}
	else if (x >= 4 && x <4.5) {num = 0.904028; den = 0.926253; statUp = 0.0160491; statDown = 0.0175516;}
	else if (x >= 4.5 && x <5.5) {num = 0.948992; den = 0.939449; statUp = 0.0109821; statDown = 0.0121312;}
	else if (x >= 5.5 && x <6.5) {num = 0.910994; den = 0.939075; statUp = 0.0180899; statDown = 0.0197981;}
	else if (x >= 6.5 && x <8) {num = 0.916309; den = 0.939568; statUp = 0.0156774; statDown = 0.017337;}
	else if (x >= 8 && x <9.5) {num = 0.908937; den = 0.928398; statUp = 0.0221432; statDown = 0.0255568;}
	else if (x >= 9.5 && x <13) {num = 0.916874; den = 0.932892; statUp = 0.0236424; statDown = 0.0272396;}
	else {num = 0.901604; den = 0.919504; statUp = 0.0359305; statDown = 0.0435464;}
      }
      // SF for 2.1 < |eta| < 2.4
      if (fabs(eta) >= 2.1 && fabs(eta) < 2.4) {
	if (x >= 1.5 && x <2.5) {num = 0.784868; den = 0.755554; statUp = 0.0193752; statDown = 0.0193376;}
	else if (x >= 2.5 && x <3) {num = 0.884159; den = 0.875273; statUp = 0.0181843; statDown = 0.0190031;}
	else if (x >= 3 && x <4.5) {num = 0.893751; den = 0.899881; statUp = 0.106249; statDown = 0.0119269;}
	else if (x >= 4.5 && x <6.5) {num = 0.901613; den = 0.917252; statUp = 0.0142685; statDown = 0.0153256;}
	else if (x >= 6.5 && x <8.5) {num = 0.880979; den = 0.917815; statUp = 0.0250043; statDown = 0.027907;}
	else if (x >= 8.5 && x <11) {num = 0.864834; den = 0.9055; statUp = 0.0320361; statDown = 0.0373617;}
	else {num = 0.89517; den = 0.911155; statUp = 0.0490948; statDown = 0.0577133;}
      }

      if (fabs(eta) >= 0 && fabs(eta) < 1.2) {
	// syst uncertainties
	if (x >= 3.5 && x < 4) syst = 0.00146411;
	else if (x >= 4 && x < 4.5) syst = 0.00327651;
	else if (x >= 4.5 && x < 5) syst = 0.00316198;
	else if (x >= 5 && x < 5.5) syst = 0.00125837;
	else if (x >= 5.5 && x < 6.5) syst = 0.000930465;
	else if (x >= 6.5 && x < 8) syst = 0.0010186;
	else if (x >= 8 && x < 10.5) syst = 0.00133373;
	else if (x >= 10.5 && x < 14) syst = 0.00126452;
	else if (x >= 14 && x < 18) syst = 0.00702329;
	else syst = 0.00295619;
      }
      else if (fabs(eta) >= 1.2 && fabs(eta) < 1.8) {
	// syst uncertainties
	if (x >= 2.07 && x < 3) syst = 0.00239623;
	else if (x >= 3 && x < 3.5) syst = 0.00702354;
	else if (x >= 3.5 && x < 4) syst = 0.00178215;
	else if (x >= 4 && x < 4.5) syst = 0.00149079;
	else if (x >= 4.5 && x < 5) syst = 0.000960333;
	else if (x >= 5 && x < 6) syst = 0.00137786;
	else if (x >= 6 && x < 7.5) syst = 0.0024569;
	else if (x >= 7.5 && x < 10) syst = 0.00125311;
	else if (x >= 10 && x < 15) syst = 0.00219919;
	else syst = 0.00398135;
      }
      else if (fabs(eta) >= 1.8 && fabs(eta) < 2.1) {
	// syst uncertainties
	if (x >= 1.5 && x < 2.5) syst = 0.0111427;
	else if (x >= 2.5 && x < 3) syst = 0.00378269;
	else if (x >= 3 && x < 3.5) syst = 0.0037617;
	else if (x >= 3.5 && x < 4) syst = 0.00990851;
	else if (x >= 4 && x < 4.5) syst = 0.0108622;
	else if (x >= 4.5 && x < 5.5) syst = 0.00354057;
	else if (x >= 5.5 && x < 6.5) syst = 0.00219385;
	else if (x >= 6.5 && x < 8) syst = 0.00635034;
	else if (x >= 8 && x < 9.5) syst = 0.0137249;
	else if (x >= 9.5 && x < 13) syst = 0.0135164;
	else syst = 0.0181378;
      }
      else if (fabs(eta) >= 2.1 && fabs(eta) < 2.4) {
	// syst uncertainties
	if (x >= 1.5 && x < 2.5) syst = 0.0035764;
	else if (x >= 2.5 && x < 3) syst = 0.0201525;
	else if (x >= 3 && x < 4.5) syst = 0.00258387;
	else if (x >= 4.5 && x < 6.5) syst = 0.00410559;
	else if (x >= 6.5 && x < 8.5) syst = 0.00740031;
	else if (x >= 8.5 && x < 11) syst = 0.00884108;
	else syst = 0.00848748;
      }
    }
    else {
      // SF for 0 < |eta| < 1.2
      if (fabs(eta) >= 0 && fabs(eta) < 1.2) {
	if (x >= 3.5 && x <4) {num = 0.689722; den = 0.619881; statUp = 0.016743; statDown = 0.0169866;}
	else if (x >= 4 && x <4.5) {num = 0.872807; den = 0.842673; statUp = 0.0124568; statDown = 0.0130601;}
	else if (x >= 4.5 && x <5) {num = 0.930789; den = 0.903981; statUp = 0.0103367; statDown = 0.0111996;}
	else if (x >= 5 && x <5.5) {num = 0.941372; den = 0.926556; statUp = 0.0106639; statDown = 0.011868;}
	else if (x >= 5.5 && x <6.5) {num = 0.964656; den = 0.943467; statUp = 0.00706971; statDown = 0.00795962;}
	else if (x >= 6.5 && x <8) {num = 0.966201; den = 0.960068; statUp = 0.0072452; statDown = 0.00828099;}
	else if (x >= 8 && x <10.5) {num = 0.977796; den = 0.966774; statUp = 0.00709911; statDown = 0.00844906;}
	else if (x >= 10.5 && x <14) {num = 0.978258; den = 0.968852; statUp = 0.00861853; statDown = 0.011233;}
	else if (x >= 14 && x <18) {num = 0.963944; den = 0.971337; statUp = 0.0176948; statDown = 0.0253773;}
	else {num = 0.982179; den = 0.965525; statUp = 0.012435; statDown = 0.0239695;}
      }
      // SF for 1.2 < |eta| < 1.8
      if (fabs(eta) >= 1.2 && fabs(eta) < 1.8) {
	if (x >= 2.07 && x <3) {num = 0.631943; den = 0.63487; statUp = 0.0226865; statDown = 0.0227841;}
	else if (x >= 3 && x <3.5) {num = 0.819488; den = 0.794265; statUp = 0.0158833; statDown = 0.0163046;}
	else if (x >= 3.5 && x <4) {num = 0.906768; den = 0.869077; statUp = 0.0123133; statDown = 0.0129632;}
	else if (x >= 4 && x <4.5) {num = 0.94125; den = 0.918556; statUp = 0.0107543; statDown = 0.0116861;}
	else if (x >= 4.5 && x <5) {num = 0.93965; den = 0.937335; statUp = 0.012639; statDown = 0.0141481;}
	else if (x >= 5 && x <6) {num = 0.944663; den = 0.951197; statUp = 0.0107649; statDown = 0.0119937;}
	else if (x >= 6 && x <7.5) {num = 0.948277; den = 0.959474; statUp = 0.0113867; statDown = 0.0129185;}
	else if (x >= 7.5 && x <10) {num = 0.974937; den = 0.962066; statUp = 0.0100868; statDown = 0.012745;}
	else if (x >= 10 && x <15) {num = 0.963242; den = 0.963283; statUp = 0.0156471; statDown = 0.0199175;}
	else {num = 0.934011; den = 0.95995; statUp = 0.0274838; statDown = 0.0375923;}
      }
      // SF for 1.8 < |eta| < 2.1
      if (fabs(eta) >= 1.8 && fabs(eta) < 2.1) {
	if (x >= 1.5 && x <2.5) {num = 0.730106; den = 0.674204; statUp = 0.0217864; statDown = 0.0217898;}
	else if (x >= 2.5 && x <3) {num = 0.898861; den = 0.892813; statUp = 0.0167643; statDown = 0.0175803;}
	else if (x >= 3 && x <3.5) {num = 0.943188; den = 0.930248; statUp = 0.0158596; statDown = 0.0173418;}
	else if (x >= 3.5 && x <4) {num = 0.94083; den = 0.94509; statUp = 0.0155189; statDown = 0.0173437;}
	else if (x >= 4 && x <4.5) {num = 0.9494; den = 0.948955; statUp = 0.0166381; statDown = 0.0196105;}
	else if (x >= 4.5 && x <5.5) {num = 0.946559; den = 0.952479; statUp = 0.014182; statDown = 0.0160633;}
	else if (x >= 5.5 && x <6.5) {num = 0.91847; den = 0.953786; statUp = 0.0217287; statDown = 0.0253424;}
	else if (x >= 6.5 && x <8) {num = 0.912355; den = 0.947923; statUp = 0.022436; statDown = 0.026346;}
	else if (x >= 8 && x <9.5) {num = 0.891254; den = 0.942256; statUp = 0.0348021; statDown = 0.0415039;}
	else if (x >= 9.5 && x <13) {num = 0.966718; den = 0.941867; statUp = 0.0190895; statDown = 0.0268122;}
	else {num = 0.871006; den = 0.927524; statUp = 0.056407; statDown = 0.0696581;}
      }
      // SF for 2.1 < |eta| < 2.4
      if (fabs(eta) >= 2.1 && fabs(eta) < 2.4) {
	if (x >= 1.5 && x <2.5) {num = 0.76689; den = 0.765747; statUp = 0.0225837; statDown = 0.0225206;}
	else if (x >= 2.5 && x <3) {num = 0.910616; den = 0.88553; statUp = 0.023169; statDown = 0.0241312;}
	else if (x >= 3 && x <4.5) {num = 0.900222; den = 0.908224; statUp = 0.0133439; statDown = 0.0139174;}
	else if (x >= 4.5 && x <6.5) {num = 0.933402; den = 0.9265; statUp = 0.0161214; statDown = 0.0179175;}
	else if (x >= 6.5 && x <8.5) {num = 0.970163; den = 0.923434; statUp = 0.0264214; statDown = 0.0315982;}
	else if (x >= 8.5 && x <11) {num = 0.763007; den = 0.920759; statUp = 0.0531122; statDown = 0.0583712;}
	else {num = 0.909116; den = 0.915605; statUp = 0.0419393; statDown = 0.0588324;}
      }

    }
  }
  else if (filterId==1) { //L3 Jpsi
    if (idx != 99) {
      // SF for 0 < |eta| < 1.2
      if (fabs(eta) >= 0 && fabs(eta) < 1.2) {
	if (x >= 3.5 && x <4) {num = 0.0929084; den = 0.0679817; statUp = 0.006841; statDown = 0.0065872;}
	else if (x >= 4 && x <4.5) {num = 0.297766; den = 0.224427; statUp = 0.0108539; statDown = 0.010713;}
	else if (x >= 4.5 && x <5) {num = 0.502117; den = 0.417386; statUp = 0.0127763; statDown = 0.0127755;}
	else if (x >= 5 && x <5.5) {num = 0.635817; den = 0.565377; statUp = 0.0135751; statDown = 0.0136973;}
	else if (x >= 5.5 && x <6.5) {num = 0.731239; den = 0.670099; statUp = 0.0102029; statDown = 0.0103455;}
	else if (x >= 6.5 && x <8) {num = 0.770928; den = 0.743473; statUp = 0.0102308; statDown = 0.0104292;}
	else if (x >= 8 && x <10.5) {num = 0.788012; den = 0.786007; statUp = 0.0111432; statDown = 0.0114151;}
	else if (x >= 10.5 && x <14) {num = 0.810243; den = 0.820978; statUp = 0.0144525; statDown = 0.0150071;}
	else if (x >= 14 && x <18) {num = 0.792036; den = 0.829214; statUp = 0.0237166; statDown = 0.0248112;}
	else {num = 0.863475; den = 0.847132; statUp = 0.0232916; statDown = 0.0255443;}
      }
      // SF for 1.2 < |eta| < 1.8
      if (fabs(eta) >= 1.2 && fabs(eta) < 1.8) {
	if (x >= 2.07 && x <3) {num = 0.325521; den = 0.282497; statUp = 0.016271; statDown = 0.0160003;}
	else if (x >= 3 && x <3.5) {num = 0.423453; den = 0.416595; statUp = 0.0149866; statDown = 0.0148893;}
	else if (x >= 3.5 && x <4) {num = 0.543809; den = 0.511604; statUp = 0.0160433; statDown = 0.0160731;}
	else if (x >= 4 && x <4.5) {num = 0.584023; den = 0.581042; statUp = 0.0169952; statDown = 0.0170676;}
	else if (x >= 4.5 && x <5) {num = 0.61728; den = 0.623371; statUp = 0.0187614; statDown = 0.0189572;}
	else if (x >= 5 && x <6) {num = 0.685606; den = 0.6656; statUp = 0.0162022; statDown = 0.0164562;}
	else if (x >= 6 && x <7.5) {num = 0.650217; den = 0.708729; statUp = 0.0181033; statDown = 0.0183853;}
	else if (x >= 7.5 && x <10) {num = 0.73008; den = 0.748535; statUp = 0.0195931; statDown = 0.0201359;}
	else if (x >= 10 && x <15) {num = 0.773451; den = 0.799811; statUp = 0.0225743; statDown = 0.0234864;}
	else {num = 0.736842; den = 0.816195; statUp = 0.0351455; statDown = 0.0372627;}
      }
      // SF for 1.8 < |eta| < 2.1
      if (fabs(eta) >= 1.8 && fabs(eta) < 2.1) {
	if (x >= 1.5 && x <2.5) {num = 0.14084; den = 0.113341; statUp = 0.013025; statDown = 0.0125015;}
	else if (x >= 2.5 && x <3) {num = 0.476412; den = 0.440149; statUp = 0.0227841; statDown = 0.0226386;}
	else if (x >= 3 && x <3.5) {num = 0.650759; den = 0.62906; statUp = 0.0232733; statDown = 0.0235646;}
	else if (x >= 3.5 && x <4) {num = 0.727644; den = 0.698642; statUp = 0.0239828; statDown = 0.0245278;}
	else if (x >= 4 && x <4.5) {num = 0.727256; den = 0.718312; statUp = 0.0255525; statDown = 0.0262978;}
	else if (x >= 4.5 && x <5.5) {num = 0.777415; den = 0.768105; statUp = 0.0199169; statDown = 0.0206122;}
	else if (x >= 5.5 && x <6.5) {num = 0.776885; den = 0.799541; statUp = 0.0261671; statDown = 0.0273015;}
	else if (x >= 6.5 && x <8) {num = 0.791451; den = 0.821447; statUp = 0.0237416; statDown = 0.0248952;}
	else if (x >= 8 && x <9.5) {num = 0.786008; den = 0.817606; statUp = 0.0343974; statDown = 0.0367832;}
	else if (x >= 9.5 && x <13) {num = 0.824864; den = 0.84872; statUp = 0.0335901; statDown = 0.0363048;}
	else {num = 0.835512; den = 0.867058; statUp = 0.0463296; statDown = 0.0516908;}
      }
      // SF for 2.1 < |eta| < 2.4
      if (fabs(eta) >= 2.1 && fabs(eta) < 2.4) {
	if (x >= 1.5 && x <2.5) {num = 0.105786; den = 0.105436; statUp = 0.0111179; statDown = 0.0106879;}
	else if (x >= 2.5 && x <3) {num = 0.39082; den = 0.327966; statUp = 0.0261347; statDown = 0.0256485;}
	else if (x >= 3 && x <4.5) {num = 0.526405; den = 0.501953; statUp = 0.0184148; statDown = 0.0183235;}
	else if (x >= 4.5 && x <6.5) {num = 0.659384; den = 0.633247; statUp = 0.0237874; statDown = 0.0241398;}
	else if (x >= 6.5 && x <8.5) {num = 0.64115; den = 0.701749; statUp = 0.0384833; statDown = 0.0396889;}
	else if (x >= 8.5 && x <11) {num = 0.657733; den = 0.727318; statUp = 0.0490318; statDown = 0.0516174;}
	else {num = 0.751588; den = 0.772539; statUp = 0.0686162; statDown = 0.0726347;}
      }

      if (fabs(eta) >= 0 && fabs(eta) < 1.2) {
	// syst uncertainties
	if (x >= 3.5 && x < 4) syst = 0.00146036;
	else if (x >= 4 && x < 4.5) syst = 0.00419698;
	else if (x >= 4.5 && x < 5) syst = 0.00246527;
	else if (x >= 5 && x < 5.5) syst = 0.00634417;
	else if (x >= 5.5 && x < 6.5) syst = 0.00202178;
	else if (x >= 6.5 && x < 8) syst = 0.000738586;
	else if (x >= 8 && x < 10.5) syst = 0.00187763;
	else if (x >= 10.5 && x < 14) syst = 0.00110162;
	else if (x >= 14 && x < 18) syst = 0.00456042;
	else syst = 0.00325616;
      }
      else if (fabs(eta) >= 1.2 && fabs(eta) < 1.8) {
	// syst uncertainties
	if (x >= 2.07 && x < 3) syst = 0.00458779;
	else if (x >= 3 && x < 3.5) syst = 0.00836596;
	else if (x >= 3.5 && x < 4) syst = 0.00164349;
	else if (x >= 4 && x < 4.5) syst = 0.00620377;
	else if (x >= 4.5 && x < 5) syst = 0.00226104;
	else if (x >= 5 && x < 6) syst = 0.00425653;
	else if (x >= 6 && x < 7.5) syst = 0.00333209;
	else if (x >= 7.5 && x < 10) syst = 0.0010693;
	else if (x >= 10 && x < 15) syst = 0.00650836;
	else syst = 0.0138687;
      }
      else if (fabs(eta) >= 1.8 && fabs(eta) < 2.1) {
	// syst uncertainties
	if (x >= 1.5 && x < 2.5) syst = 0.00814591;
	else if (x >= 2.5 && x < 3) syst = 0.0115312;
	else if (x >= 3 && x < 3.5) syst = 0.00634851;
	else if (x >= 3.5 && x < 4) syst = 0.0175754;
	else if (x >= 4 && x < 4.5) syst = 0.0146908;
	else if (x >= 4.5 && x < 5.5) syst = 0.0110682;
	else if (x >= 5.5 && x < 6.5) syst = 0.00983664;
	else if (x >= 6.5 && x < 8) syst = 0.00319873;
	else if (x >= 8 && x < 9.5) syst = 0.027534;
	else if (x >= 9.5 && x < 13) syst = 0.0177867;
	else syst = 0.0318172;
      }
      else if (fabs(eta) >= 2.1 && fabs(eta) < 2.4) {
	// syst uncertainties
	if (x >= 1.5 && x < 2.5) syst = 0.00238882;
	else if (x >= 2.5 && x < 3) syst = 0.00627081;
	else if (x >= 3 && x < 4.5) syst = 0.00206241;
	else if (x >= 4.5 && x < 6.5) syst = 0.0134876;
	else if (x >= 6.5 && x < 8.5) syst = 0.0175673;
	else if (x >= 8.5 && x < 11) syst = 0.0140479;
	else syst = 0.0105411;
      }
    }
    else {
      // SF for 0 < |eta| < 1.2
      if (fabs(eta) >= 0 && fabs(eta) < 1.2) {
	if (x >= 3.5 && x <4) {num = 0.121625; den = 0.0719616; statUp = 0.0117235; statDown = 0.0111791;}
	else if (x >= 4 && x <4.5) {num = 0.356478; den = 0.236802; statUp = 0.0180185; statDown = 0.0177502;}
	else if (x >= 4.5 && x <5) {num = 0.516625; den = 0.42827; statUp = 0.0204454; statDown = 0.0204329;}
	else if (x >= 5 && x <5.5) {num = 0.673482; den = 0.570623; statUp = 0.0225258; statDown = 0.023002;}
	else if (x >= 5.5 && x <6.5) {num = 0.747192; den = 0.668258; statUp = 0.0174265; statDown = 0.0179388;}
	else if (x >= 6.5 && x <8) {num = 0.759122; den = 0.739998; statUp = 0.0181382; statDown = 0.0186968;}
	else if (x >= 8 && x <10.5) {num = 0.814959; den = 0.782242; statUp = 0.0185864; statDown = 0.0194487;}
	else if (x >= 10.5 && x <14) {num = 0.841196; den = 0.819894; statUp = 0.0235368; statDown = 0.0254018;}
	else if (x >= 14 && x <18) {num = 0.835715; den = 0.828248; statUp = 0.0400773; statDown = 0.0456275;}
	else {num = 0.91265; den = 0.846019; statUp = 0.0385611; statDown = 0.048033;}
      }
      // SF for 1.2 < |eta| < 1.8
      if (fabs(eta) >= 1.2 && fabs(eta) < 1.8) {
	if (x >= 2.07 && x <3) {num = 0.301749; den = 0.282102; statUp = 0.0206993; statDown = 0.0202013;}
	else if (x >= 3 && x <3.5) {num = 0.404378; den = 0.427544; statUp = 0.0197939; statDown = 0.0195719;}
	else if (x >= 3.5 && x <4) {num = 0.550337; den = 0.524241; statUp = 0.0204696; statDown = 0.0204586;}
	else if (x >= 4 && x <4.5) {num = 0.632661; den = 0.600426; statUp = 0.0221954; statDown = 0.0223902;}
	else if (x >= 4.5 && x <5) {num = 0.674002; den = 0.645763; statUp = 0.0263881; statDown = 0.0268861;}
	else if (x >= 5 && x <6) {num = 0.652999; den = 0.679457; statUp = 0.0237329; statDown = 0.02409;}
	else if (x >= 6 && x <7.5) {num = 0.698863; den = 0.718994; statUp = 0.0251336; statDown = 0.0258871;}
	else if (x >= 7.5 && x <10) {num = 0.771971; den = 0.75533; statUp = 0.0286891; statDown = 0.030171;}
	else if (x >= 10 && x <15) {num = 0.753758; den = 0.803949; statUp = 0.0378786; statDown = 0.0403045;}
	else {num = 0.745996; den = 0.82919; statUp = 0.053815; statDown = 0.0605636;}
      }
      // SF for 1.8 < |eta| < 2.1
      if (fabs(eta) >= 1.8 && fabs(eta) < 2.1) {
	if (x >= 1.5 && x <2.5) {num = 0.115278; den = 0.116283; statUp = 0.0129321; statDown = 0.0123899;}
	else if (x >= 2.5 && x <3) {num = 0.461883; den = 0.461492; statUp = 0.0265949; statDown = 0.0263642;}
	else if (x >= 3 && x <3.5) {num = 0.701404; den = 0.657357; statUp = 0.0304532; statDown = 0.0308107;}
	else if (x >= 3.5 && x <4) {num = 0.729678; den = 0.716402; statUp = 0.0300697; statDown = 0.0308006;}
	else if (x >= 4 && x <4.5) {num = 0.772151; den = 0.744289; statUp = 0.0339421; statDown = 0.0357068;}
	else if (x >= 4.5 && x <5.5) {num = 0.795417; den = 0.78389; statUp = 0.0258355; statDown = 0.027002;}
	else if (x >= 5.5 && x <6.5) {num = 0.80047; den = 0.818824; statUp = 0.0351114; statDown = 0.037368;}
	else if (x >= 6.5 && x <8) {num = 0.765323; den = 0.835004; statUp = 0.0372734; statDown = 0.0394468;}
	else if (x >= 8 && x <9.5) {num = 0.800271; den = 0.842257; statUp = 0.0468732; statDown = 0.0519374;}
	else if (x >= 9.5 && x <13) {num = 0.83651; den = 0.857865; statUp = 0.0438402; statDown = 0.0500843;}
	else {num = 0.818935; den = 0.87243; statUp = 0.0689499; statDown = 0.0781992;}
      }
      // SF for 2.1 < |eta| < 2.4
      if (fabs(eta) >= 2.1 && fabs(eta) < 2.4) {
	if (x >= 1.5 && x <2.5) {num = 0.105672; den = 0.107451; statUp = 0.0127215; statDown = 0.01219;}
	else if (x >= 2.5 && x <3) {num = 0.328121; den = 0.350297; statUp = 0.0318604; statDown = 0.0308867;}
	else if (x >= 3 && x <4.5) {num = 0.526157; den = 0.514685; statUp = 0.0212428; statDown = 0.0211059;}
	else if (x >= 4.5 && x <6.5) {num = 0.71337; den = 0.644854; statUp = 0.0304085; statDown = 0.0308433;}
	else if (x >= 6.5 && x <8.5) {num = 0.744769; den = 0.711723; statUp = 0.0527876; statDown = 0.0545914;}
	else if (x >= 8.5 && x <11) {num = 0.57168; den = 0.745813; statUp = 0.0650887; statDown = 0.0654034;}
	else {num = 0.696756; den = 0.780684; statUp = 0.0832727; statDown = 0.0883945;}
      }

    }
  }
  else if (filterId==2) { //L2 Jpsi
    // SF for 0 < |eta| < 1.2
    if (fabs(eta) >= 0 && fabs(eta) < 1.2) {
      if (x >= 3.5 && x <4) {num = 0.683516; den = 0.624883; statUp = 0.0108066; statDown = 0.0109176;}
      else if (x >= 4 && x <4.5) {num = 0.864766; den = 0.844349; statUp = 0.00811031; statDown = 0.00834878;}
      else if (x >= 4.5 && x <5) {num = 0.93532; den = 0.903951; statUp = 0.00633481; statDown = 0.00667111;}
      else if (x >= 5 && x <5.5) {num = 0.938106; den = 0.923036; statUp = 0.00664483; statDown = 0.0070676;}
      else if (x >= 5.5 && x <6.5) {num = 0.961913; den = 0.940699; statUp = 0.00441524; statDown = 0.00471286;}
      else if (x >= 6.5 && x <8) {num = 0.955209; den = 0.958559; statUp = 0.00486781; statDown = 0.00520504;}
      else if (x >= 8 && x <10.5) {num = 0.960097; den = 0.96542; statUp = 0.00522745; statDown = 0.00566401;}
      else if (x >= 10.5 && x <14) {num = 0.953912; den = 0.966812; statUp = 0.00737412; statDown = 0.0081774;}
      else if (x >= 14 && x <18) {num = 0.958011; den = 0.96975; statUp = 0.0115548; statDown = 0.0135024;}
      else {num = 0.958729; den = 0.964144; statUp = 0.0126207; statDown = 0.0154877;}
    }
    // SF for 1.2 < |eta| < 1.8
    if (fabs(eta) >= 1.2 && fabs(eta) < 1.8) {
      if (x >= 2.07 && x <3) {num = 0.69446; den = 0.640187; statUp = 0.0166834; statDown = 0.0168495;}
      else if (x >= 3 && x <3.5) {num = 0.828672; den = 0.79077; statUp = 0.0116385; statDown = 0.0119271;}
      else if (x >= 3.5 && x <4) {num = 0.914934; den = 0.865937; statUp = 0.00898234; statDown = 0.00943628;}
      else if (x >= 4 && x <4.5) {num = 0.926217; den = 0.912889; statUp = 0.00887486; statDown = 0.00942155;}
      else if (x >= 4.5 && x <5) {num = 0.940711; den = 0.933298; statUp = 0.00888396; statDown = 0.00963798;}
      else if (x >= 5 && x <6) {num = 0.962868; den = 0.949988; statUp = 0.00656083; statDown = 0.00720576;}
      else if (x >= 6 && x <7.5) {num = 0.940584; den = 0.959158; statUp = 0.00854681; statDown = 0.00928998;}
      else if (x >= 7.5 && x <10) {num = 0.950557; den = 0.958603; statUp = 0.00907201; statDown = 0.0101255;}
      else if (x >= 10 && x <15) {num = 0.958968; den = 0.96132; statUp = 0.0101043; statDown = 0.0116835;}
      else {num = 0.932633; den = 0.950433; statUp = 0.0169327; statDown = 0.0182775;}
    }
    // SF for 1.8 < |eta| < 2.1
    if (fabs(eta) >= 1.8 && fabs(eta) < 2.1) {
      if (x >= 1.5 && x <2.5) {num = 0.69714; den = 0.653685; statUp = 0.0201021; statDown = 0.020174;}
      else if (x >= 2.5 && x <3) {num = 0.906875; den = 0.867753; statUp = 0.0141174; statDown = 0.0148529;}
      else if (x >= 3 && x <3.5) {num = 0.919454; den = 0.903362; statUp = 0.0134254; statDown = 0.0144224;}
      else if (x >= 3.5 && x <4) {num = 0.946926; den = 0.922588; statUp = 0.0128289; statDown = 0.0140971;}
      else if (x >= 4 && x <4.5) {num = 0.904028; den = 0.926253; statUp = 0.0160491; statDown = 0.0175516;}
      else if (x >= 4.5 && x <5.5) {num = 0.948992; den = 0.939449; statUp = 0.0109821; statDown = 0.0121312;}
      else if (x >= 5.5 && x <6.5) {num = 0.910994; den = 0.939075; statUp = 0.0180899; statDown = 0.0197981;}
      else if (x >= 6.5 && x <8) {num = 0.916309; den = 0.939568; statUp = 0.0156774; statDown = 0.017337;}
      else if (x >= 8 && x <9.5) {num = 0.908937; den = 0.928398; statUp = 0.0221432; statDown = 0.0255568;}
      else if (x >= 9.5 && x <13) {num = 0.916874; den = 0.932892; statUp = 0.0236424; statDown = 0.0272396;}
      else {num = 0.901604; den = 0.919504; statUp = 0.0359305; statDown = 0.0435464;}
    }
    // SF for 2.1 < |eta| < 2.4
    if (fabs(eta) >= 2.1 && fabs(eta) < 2.4) {
      if (x >= 1.5 && x <2.5) {num = 0.784868; den = 0.755554; statUp = 0.0193752; statDown = 0.0193376;}
      else if (x >= 2.5 && x <3) {num = 0.884159; den = 0.875273; statUp = 0.0181843; statDown = 0.0190031;}
      else if (x >= 3 && x <4.5) {num = 0.893751; den = 0.899881; statUp = 0.106249; statDown = 0.0119269;}
      else if (x >= 4.5 && x <6.5) {num = 0.901613; den = 0.917252; statUp = 0.0142685; statDown = 0.0153256;}
      else if (x >= 6.5 && x <8.5) {num = 0.880979; den = 0.917815; statUp = 0.0250043; statDown = 0.027907;}
      else if (x >= 8.5 && x <11) {num = 0.864834; den = 0.9055; statUp = 0.0320361; statDown = 0.0373617;}
      else {num = 0.89517; den = 0.911155; statUp = 0.0490948; statDown = 0.0577133;}
    }

    if (fabs(eta) >= 0 && fabs(eta) < 1.2) {
      // syst uncertainties
      if (x >= 3.5 && x < 4) syst = 0.00146411;
      else if (x >= 4 && x < 4.5) syst = 0.00327651;
      else if (x >= 4.5 && x < 5) syst = 0.00316198;
      else if (x >= 5 && x < 5.5) syst = 0.00125837;
      else if (x >= 5.5 && x < 6.5) syst = 0.000930465;
      else if (x >= 6.5 && x < 8) syst = 0.0010186;
      else if (x >= 8 && x < 10.5) syst = 0.00133373;
      else if (x >= 10.5 && x < 14) syst = 0.00126452;
      else if (x >= 14 && x < 18) syst = 0.00702329;
      else syst = 0.00295619;
    }
    else if (fabs(eta) >= 1.2 && fabs(eta) < 1.8) {
      // syst uncertainties
      if (x >= 2.07 && x < 3) syst = 0.00239623;
      else if (x >= 3 && x < 3.5) syst = 0.00702354;
      else if (x >= 3.5 && x < 4) syst = 0.00178215;
      else if (x >= 4 && x < 4.5) syst = 0.00149079;
      else if (x >= 4.5 && x < 5) syst = 0.000960333;
      else if (x >= 5 && x < 6) syst = 0.00137786;
      else if (x >= 6 && x < 7.5) syst = 0.0024569;
      else if (x >= 7.5 && x < 10) syst = 0.00125311;
      else if (x >= 10 && x < 15) syst = 0.00219919;
      else syst = 0.00398135;
    }
    else if (fabs(eta) >= 1.8 && fabs(eta) < 2.1) {
      // syst uncertainties
      if (x >= 1.5 && x < 2.5) syst = 0.0111427;
      else if (x >= 2.5 && x < 3) syst = 0.00378269;
      else if (x >= 3 && x < 3.5) syst = 0.0037617;
      else if (x >= 3.5 && x < 4) syst = 0.00990851;
      else if (x >= 4 && x < 4.5) syst = 0.0108622;
      else if (x >= 4.5 && x < 5.5) syst = 0.00354057;
      else if (x >= 5.5 && x < 6.5) syst = 0.00219385;
      else if (x >= 6.5 && x < 8) syst = 0.00635034;
      else if (x >= 8 && x < 9.5) syst = 0.0137249;
      else if (x >= 9.5 && x < 13) syst = 0.0135164;
      else syst = 0.0181378;
    }
    else if (fabs(eta) >= 2.1 && fabs(eta) < 2.4) {
      // syst uncertainties
      if (x >= 1.5 && x < 2.5) syst = 0.0035764;
      else if (x >= 2.5 && x < 3) syst = 0.0201525;
      else if (x >= 3 && x < 4.5) syst = 0.00258387;
      else if (x >= 4.5 && x < 6.5) syst = 0.00410559;
      else if (x >= 6.5 && x < 8.5) syst = 0.00740031;
      else if (x >= 8.5 && x < 11) syst = 0.00884108;
      else syst = 0.00848748;
    }
  }
  else if (filterId==3) { //L3 Jpsi
    // SF for 0 < |eta| < 1.2
    if (fabs(eta) >= 0 && fabs(eta) < 1.2) {
      if (x >= 3.5 && x <4) {num = 0.0929084; den = 0.0679817; statUp = 0.006841; statDown = 0.0065872;}
      else if (x >= 4 && x <4.5) {num = 0.297766; den = 0.224427; statUp = 0.0108539; statDown = 0.010713;}
      else if (x >= 4.5 && x <5) {num = 0.502117; den = 0.417386; statUp = 0.0127763; statDown = 0.0127755;}
      else if (x >= 5 && x <5.5) {num = 0.635817; den = 0.565377; statUp = 0.0135751; statDown = 0.0136973;}
      else if (x >= 5.5 && x <6.5) {num = 0.731239; den = 0.670099; statUp = 0.0102029; statDown = 0.0103455;}
      else if (x >= 6.5 && x <8) {num = 0.770928; den = 0.743473; statUp = 0.0102308; statDown = 0.0104292;}
      else if (x >= 8 && x <10.5) {num = 0.788012; den = 0.786007; statUp = 0.0111432; statDown = 0.0114151;}
      else if (x >= 10.5 && x <14) {num = 0.810243; den = 0.820978; statUp = 0.0144525; statDown = 0.0150071;}
      else if (x >= 14 && x <18) {num = 0.792036; den = 0.829214; statUp = 0.0237166; statDown = 0.0248112;}
      else {num = 0.863475; den = 0.847132; statUp = 0.0232916; statDown = 0.0255443;}
    }
    // SF for 1.2 < |eta| < 1.8
    if (fabs(eta) >= 1.2 && fabs(eta) < 1.8) {
      if (x >= 2.07 && x <3) {num = 0.325521; den = 0.282497; statUp = 0.016271; statDown = 0.0160003;}
      else if (x >= 3 && x <3.5) {num = 0.423453; den = 0.416595; statUp = 0.0149866; statDown = 0.0148893;}
      else if (x >= 3.5 && x <4) {num = 0.543809; den = 0.511604; statUp = 0.0160433; statDown = 0.0160731;}
      else if (x >= 4 && x <4.5) {num = 0.584023; den = 0.581042; statUp = 0.0169952; statDown = 0.0170676;}
      else if (x >= 4.5 && x <5) {num = 0.61728; den = 0.623371; statUp = 0.0187614; statDown = 0.0189572;}
      else if (x >= 5 && x <6) {num = 0.685606; den = 0.6656; statUp = 0.0162022; statDown = 0.0164562;}
      else if (x >= 6 && x <7.5) {num = 0.650217; den = 0.708729; statUp = 0.0181033; statDown = 0.0183853;}
      else if (x >= 7.5 && x <10) {num = 0.73008; den = 0.748535; statUp = 0.0195931; statDown = 0.0201359;}
      else if (x >= 10 && x <15) {num = 0.773451; den = 0.799811; statUp = 0.0225743; statDown = 0.0234864;}
      else {num = 0.736842; den = 0.816195; statUp = 0.0351455; statDown = 0.0372627;}
    }
    // SF for 1.8 < |eta| < 2.1
    if (fabs(eta) >= 1.8 && fabs(eta) < 2.1) {
      if (x >= 1.5 && x <2.5) {num = 0.14084; den = 0.113341; statUp = 0.013025; statDown = 0.0125015;}
      else if (x >= 2.5 && x <3) {num = 0.476412; den = 0.440149; statUp = 0.0227841; statDown = 0.0226386;}
      else if (x >= 3 && x <3.5) {num = 0.650759; den = 0.62906; statUp = 0.0232733; statDown = 0.0235646;}
      else if (x >= 3.5 && x <4) {num = 0.727644; den = 0.698642; statUp = 0.0239828; statDown = 0.0245278;}
      else if (x >= 4 && x <4.5) {num = 0.727256; den = 0.718312; statUp = 0.0255525; statDown = 0.0262978;}
      else if (x >= 4.5 && x <5.5) {num = 0.777415; den = 0.768105; statUp = 0.0199169; statDown = 0.0206122;}
      else if (x >= 5.5 && x <6.5) {num = 0.776885; den = 0.799541; statUp = 0.0261671; statDown = 0.0273015;}
      else if (x >= 6.5 && x <8) {num = 0.791451; den = 0.821447; statUp = 0.0237416; statDown = 0.0248952;}
      else if (x >= 8 && x <9.5) {num = 0.786008; den = 0.817606; statUp = 0.0343974; statDown = 0.0367832;}
      else if (x >= 9.5 && x <13) {num = 0.824864; den = 0.84872; statUp = 0.0335901; statDown = 0.0363048;}
      else {num = 0.835512; den = 0.867058; statUp = 0.0463296; statDown = 0.0516908;}
    }
    // SF for 2.1 < |eta| < 2.4
    if (fabs(eta) >= 2.1 && fabs(eta) < 2.4) {
      if (x >= 1.5 && x <2.5) {num = 0.105786; den = 0.105436; statUp = 0.0111179; statDown = 0.0106879;}
      else if (x >= 2.5 && x <3) {num = 0.39082; den = 0.327966; statUp = 0.0261347; statDown = 0.0256485;}
      else if (x >= 3 && x <4.5) {num = 0.526405; den = 0.501953; statUp = 0.0184148; statDown = 0.0183235;}
      else if (x >= 4.5 && x <6.5) {num = 0.659384; den = 0.633247; statUp = 0.0237874; statDown = 0.0241398;}
      else if (x >= 6.5 && x <8.5) {num = 0.64115; den = 0.701749; statUp = 0.0384833; statDown = 0.0396889;}
      else if (x >= 8.5 && x <11) {num = 0.657733; den = 0.727318; statUp = 0.0490318; statDown = 0.0516174;}
      else {num = 0.751588; den = 0.772539; statUp = 0.0686162; statDown = 0.0726347;}
    }

    if (fabs(eta) >= 0 && fabs(eta) < 1.2) {
      // syst uncertainties
      if (x >= 3.5 && x < 4) syst = 0.00146036;
      else if (x >= 4 && x < 4.5) syst = 0.00419698;
      else if (x >= 4.5 && x < 5) syst = 0.00246527;
      else if (x >= 5 && x < 5.5) syst = 0.00634417;
      else if (x >= 5.5 && x < 6.5) syst = 0.00202178;
      else if (x >= 6.5 && x < 8) syst = 0.000738586;
      else if (x >= 8 && x < 10.5) syst = 0.00187763;
      else if (x >= 10.5 && x < 14) syst = 0.00110162;
      else if (x >= 14 && x < 18) syst = 0.00456042;
      else syst = 0.00325616;
    }
    else if (fabs(eta) >= 1.2 && fabs(eta) < 1.8) {
      // syst uncertainties
      if (x >= 2.07 && x < 3) syst = 0.00458779;
      else if (x >= 3 && x < 3.5) syst = 0.00836596;
      else if (x >= 3.5 && x < 4) syst = 0.00164349;
      else if (x >= 4 && x < 4.5) syst = 0.00620377;
      else if (x >= 4.5 && x < 5) syst = 0.00226104;
      else if (x >= 5 && x < 6) syst = 0.00425653;
      else if (x >= 6 && x < 7.5) syst = 0.00333209;
      else if (x >= 7.5 && x < 10) syst = 0.0010693;
      else if (x >= 10 && x < 15) syst = 0.00650836;
      else syst = 0.0138687;
    }
    else if (fabs(eta) >= 1.8 && fabs(eta) < 2.1) {
      // syst uncertainties
      if (x >= 1.5 && x < 2.5) syst = 0.00814591;
      else if (x >= 2.5 && x < 3) syst = 0.0115312;
      else if (x >= 3 && x < 3.5) syst = 0.00634851;
      else if (x >= 3.5 && x < 4) syst = 0.0175754;
      else if (x >= 4 && x < 4.5) syst = 0.0146908;
      else if (x >= 4.5 && x < 5.5) syst = 0.0110682;
      else if (x >= 5.5 && x < 6.5) syst = 0.00983664;
      else if (x >= 6.5 && x < 8) syst = 0.00319873;
      else if (x >= 8 && x < 9.5) syst = 0.027534;
      else if (x >= 9.5 && x < 13) syst = 0.0177867;
      else syst = 0.0318172;
    }
    else if (fabs(eta) >= 2.1 && fabs(eta) < 2.4) {
      // syst uncertainties
      if (x >= 1.5 && x < 2.5) syst = 0.00238882;
      else if (x >= 2.5 && x < 3) syst = 0.00627081;
      else if (x >= 3 && x < 4.5) syst = 0.00206241;
      else if (x >= 4.5 && x < 6.5) syst = 0.0134876;
      else if (x >= 6.5 && x < 8.5) syst = 0.0175673;
      else if (x >= 8.5 && x < 11) syst = 0.0140479;
      else syst = 0.0105411;
    }
  }
  double syst_factor = 0; double stat_factor = 0;
  if (idx == -1) syst_factor = syst;
  else if (idx == -2) syst_factor = -1*syst;
  else if (idx == +1) stat_factor = statUp;
  else if (idx == +2) stat_factor = -1*statDown;
  return ((num+syst_factor+stat_factor)/den);
}

///////////////////////////////////////////////////
//              T R K     P b P b                //
///////////////////////////////////////////////////

double tnp_weight_trk_pbpb(double eta, int idx) {
  double x = eta;
  double num=1, den=1, syst=0, statUp=0, statDown=0;
  if (idx != 99) {
    //SF in eta bins
    if (x >= -2.4 && x < -1.6) {num = 0.993926; den = 0.998702; statUp = 0.00419653; statDown = 0.00432197;}
    if (x >= -1.6 && x < -1.2) {num = 0.976902; den = 0.969637; statUp = 0.00590158; statDown = 0.00615432;}
    if (x >= -1.2 && x < -0.9) {num = 0.970023; den = 0.975344; statUp = 0.0129736; statDown = 0.013189;}
    if (x >= -0.9 && x < -0.6) {num = 0.966166; den = 0.975223; statUp = 0.0108924; statDown = 0.011145;}
    if (x >= -0.6 && x < -0.3) {num = 0.954508; den = 0.978331; statUp = 0.0101589; statDown = 0.0104172;}
    if (x >= -0.3 && x < 0.3) {num = 0.966412; den = 0.966149; statUp = 0.00796915; statDown = 0.00805333;}
    if (x >= 0.3 && x < 0.6) {num = 0.96047; den = 0.967223; statUp = 0.0113363; statDown = 0.0115807;}
    if (x >= 0.6 && x < 0.9) {num = 0.972201; den = 0.967365; statUp = 0.0112951; statDown = 0.0115885;}
    if (x >= 0.9 && x < 1.2) {num = 0.944248; den = 0.963713; statUp = 0.0161942; statDown = 0.016393;}
    if (x >= 1.2 && x < 1.6) {num = 0.957924; den = 0.964393; statUp = 0.00660789; statDown = 0.00682015;}
    if (x >= 1.6 && x < 2.4) {num = 0.9999; den = 0.999137; statUp = 0.000100279; statDown = 0.00556743;}

    // syst uncertainties
    if (x >= -2.4 && x < -1.6) syst = 0.00210453;
    else if (x >= -1.6 && x < -1.2) syst = 0.00690924;
    else if (x >= -1.2 && x < -0.9) syst = 0.0140428;
    else if (x >= -0.9 && x < -0.6) syst = 0.00314062;
    else if (x >= -0.6 && x < -0.3) syst = 0.00433076;
    else if (x >= -0.3 && x < 0.3) syst = 0.00497976;
    else if (x >= 0.3 && x < 0.6) syst = 0.0107458;
    else if (x >= 0.6 && x < 0.9) syst = 0.00145445;
    else if (x >= 0.9 && x < 1.2) syst = 0.0083856;
    else if (x >= 1.2 && x < 1.6) syst = 0.0064073;
    else syst = 0.000546022;
  }
  else {
    //SF in eta bins
    if (x >= -2.4 && x < -1.6) {num = 0.996699; den = 0.998413; statUp = 0.00330123; statDown = 0.00616931;}
    if (x >= -1.6 && x < -1.2) {num = 0.971703; den = 0.96732; statUp = 0.00981403; statDown = 0.0102412;}
    if (x >= -1.2 && x < -0.9) {num = 0.922503; den = 0.970676; statUp = 0.0254956; statDown = 0.0255154;}
    if (x >= -0.9 && x < -0.6) {num = 0.947585; den = 0.974462; statUp = 0.019426; statDown = 0.019939;}
    if (x >= -0.6 && x < -0.3) {num = 0.971967; den = 0.978043; statUp = 0.0177538; statDown = 0.0183016;}
    if (x >= -0.3 && x < 0.3) {num = 0.958389; den = 0.966616; statUp = 0.0155441; statDown = 0.0159405;}
    if (x >= 0.3 && x < 0.6) {num = 0.952296; den = 0.967543; statUp = 0.0183273; statDown = 0.0190258;}
    if (x >= 0.6 && x < 0.9) {num = 0.944595; den = 0.966998; statUp = 0.0251166; statDown = 0.0255716;}
    if (x >= 0.9 && x < 1.2) {num = 1; den = 0.955977; statUp = 5.54593e-09; statDown = 0.0214702;}
    if (x >= 1.2 && x < 1.6) {num = 0.967736; den = 0.964616; statUp = 0.0100742; statDown = 0.010361;}
    if (x >= 1.6 && x < 2.4) {num = 0.999971; den = 0.998768; statUp = 2.85144e-05; statDown = 0.0054368;}
  }

  double syst_factor = 0; double stat_factor = 0;
  if (idx == -1) syst_factor = syst;
  else if (idx == -2) syst_factor = -1*syst;
  else if (idx == +1) stat_factor = statUp;
  else if (idx == +2) stat_factor = -1*statDown;
  return ((num+syst_factor+stat_factor)/den);
}

#endif
