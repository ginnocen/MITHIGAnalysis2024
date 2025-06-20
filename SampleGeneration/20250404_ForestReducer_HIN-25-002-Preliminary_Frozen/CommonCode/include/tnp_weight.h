#ifndef tnp_weight_h
#define tnp_weight_h

#include "TMath.h"
#include <iostream>
#include <map>

// IN THIS FILE YOU WILL FIND:
// ++++++++++++++
//
// - GlbTrk: (tnp_weight_glbPFtrk_pbpb) 
//   * idx = 0: nominal
//   * idx = -1: syst variation,  +1 sigma
//   * idx = -2: syst variation,  -1 sigma
//   * idx = +1: stat variation,  +1 sigma
//   * idx = +2: stat variation,  -1 sigma

//   * idx = -3: syst variation, no PF uncertainty included  +1 sigma
//   * idx = -4: syst variation, no PF uncertainty included  -1 sigma
//   * idx = -5: only PF uncertainty included (1 +1 sigma)
//   * idx = -6: only PF uncertainty included (1 -1 sigma)


// - MuID: (tnp_weight_muid_pbpb) 
//   * idx = 0: nominal
//   * idx = -1: syst variation,  +1 sigma
//   * idx = -2: syst variation,  -1 sigma
//   * idx = +1: stat variation,  +1 sigma
//   * idx = +2: stat variation,  -1 sigma
//
// - Trigger: (tnp_weight_trig_pbpb) 
//   * idx = 0:  nominal
//   * idx = -1: TnP syst variation,  +1 sigma
//   * idx = -2: TnP syst variation,  -1 sigma
//   * idx = +1: stat variation,  +1 sigma
//   * idx = +2: stat variation,  -1 sigma

// For all:
//   * idx = +200: tnp efficiency for data
//   * idx = +300: tnp efficiency for MC

// THE INDIVIDUAL SFs
// ++++++++++++++++++
double tnp_weight_glbPFtrk_pbpb(double eta, double cent, int idx = 0);
double tnp_weight_muid_pbpb(double eta, int idx=0);
double tnp_weight_trig_pbpb(double pt, double eta, double cent, int idx=0);
pair<double, double> tnp_trig_pbpb(double pt, double eta, double cent, int idx=0);
double tnp_weight_trig_double_pbpb(double pt1, double eta1, double cent1, double pt2, double eta2, double cent2, int idx=0);

double tnp_weight_TightID_pp(double eta, int idx = 0);
double tnp_weight_TightID_PFIsoTight_pp(double eta, int idx = 0);
double tnp_weight_L3Mu12_pp(double eta, int idx = 0);
double tnp_weight_L3Mu12_PFIsoTight_pp(double eta, int idx = 0);
double eff_data_TightID_pp(double eta, int idx = 0);
double eff_data_TightID_PFIsoTight_pp(double eta, int idx = 0);
double eff_data_L3Mu12_pp(double eta, int idx = 0);
double eff_data_L3Mu12_PFIsoTight_pp(double eta, int idx = 0);
double eff_MC_TightID_pp(double eta, int idx = 0);
double eff_MC_TightID_PFIsoTight_pp(double eta, int idx = 0);
double eff_MC_L3Mu12_pp(double eta, int idx = 0);
double eff_MC_L3Mu12_PFIsoTight_pp(double eta, int idx = 0);
double tnp_weight_L3Mu12_double_pp(double eta1, double eta2, int idx=0);


///////////////////////////////////////////////////
//              G l b P F  T r k    P b P b           //
///////////////////////////////////////////////////

double tnp_uncertainty_pf_pbpb(double eta, double cent, int idx = 0); //not a standalone efficiency - only systematic uncertainty is provided, and added to the values of glbpftrk (as it started by taking PF out of glbtrk). No need to call, the syst uncertainty is included in the glbtrk by default

double tnp_weight_glbPFtrk_pbpb(double eta, double cent, int idx) //cent 0-100%
{
	double num = 1, den = 1;

	if (fabs(eta)>2.4) { cout << "[WARNING] Muon pseudo-rapidity (" << eta << ") outside [-2.4, 2.4]" << endl; return 1.0; }
	if (cent < 0 || cent>100) { cout << "[ERROR] Centrality (" << cent << ") outside [0%, 100%]" << endl; return 1.0; }

	double abseta = fabs(eta);

	if (cent >= 0 && cent < 40)
	{
		// MC
		if (abseta >= 0 && abseta <= 0.9) { den = 0.978912; }
		else if (abseta > 0.9 && abseta <= 1.2) { den = 0.980928; }
		else if (abseta > 1.2 && abseta <= 1.6) { den = 0.99245; }
		else if (abseta > 1.6 && abseta <= 2.1) { den = 0.986688; }
		else if (abseta > 2.1 && abseta <= 2.4) { den = 0.980687; }

		// data
		if (idx == 0|| idx == 200) { // nominal
			if (abseta >= 0 && abseta <= 0.9) { num = 0.971173; }
			else if (abseta > 0.9 && abseta <= 1.2) { num = 0.958148; }
			else if (abseta > 1.2 && abseta <= 1.6) { num = 0.987326; }
			else if (abseta > 1.6 && abseta <= 2.1) { num = 0.983108; }
			else if (abseta > 2.1 && abseta <= 2.4) { num = 0.965458; }
		}

		if (idx == 1) { // stat up
			if (abseta >= 0 && abseta <= 0.9) { num = 0.972533; }
			else if (abseta > 0.9 && abseta <= 1.2) { num = 0.963009; }
			else if (abseta > 1.2 && abseta <= 1.6) { num = 0.988907; }
			else if (abseta > 1.6 && abseta <= 2.1) { num = 0.984995; }
			else if (abseta > 2.1 && abseta <= 2.4) { num = 0.96963; }
		}
		if (idx == 2) { // stat down
			if (abseta >= 0 && abseta <= 0.9) { num = 0.969776; }
			else if (abseta > 0.9 && abseta <= 1.2) { num = 0.954932; }
			else if (abseta > 1.2 && abseta <= 1.6) { num = 0.985623; }
			else if (abseta > 1.6 && abseta <= 2.1) { num = 0.981102; }
			else if (abseta > 2.1 && abseta <= 2.4) { num = 0.96104; }
		}
		if (idx == -1|| idx == -3) { // syst up
			if (abseta >= 0 && abseta <= 0.9) { num = 0.971762; }
			else if (abseta > 0.9 && abseta <= 1.2) { num = 0.959959; }
			else if (abseta > 1.2 && abseta <= 1.6) { num = 0.988186; }
			else if (abseta > 1.6 && abseta <= 2.1) { num = 0.983305; }
			else if (abseta > 2.1 && abseta <= 2.4) { num = 0.966265; }
		}
		if (idx == -2 || idx == -4) { // syst down
			if (abseta >= 0 && abseta <= 0.9) { num = 0.970584; }
			else if (abseta > 0.9 && abseta <= 1.2) { num = 0.956336; }
			else if (abseta > 1.2 && abseta <= 1.6) { num = 0.986467; }
			else if (abseta > 1.6 && abseta <= 2.1) { num = 0.982912; }
			else if (abseta > 2.1 && abseta <= 2.4) { num = 0.964651; }
		}

	}
	else if (cent >= 40 && cent <= 100)
	{

		// MC
		if (abseta >= 0 && abseta <= 0.9) { den = 0.98744; }
		else if (abseta > 0.9 && abseta <= 1.2) { den = 0.99433; }
		else if (abseta > 1.2 && abseta <= 1.6) { den = 0.998913; }
		else if (abseta > 1.6 && abseta <= 2.1) { den = 0.996517; }
		else if (abseta > 2.1 && abseta <= 2.4) { den = 0.992023; }

		// data
		if (idx == 0 || idx == 200) { // nominal
			if (abseta >= 0 && abseta <= 0.9) { num = 0.985895; }
			else if (abseta > 0.9 && abseta <= 1.2) { num = 0.994328; }
			else if (abseta > 1.2 && abseta <= 1.6) { num = 0.997029; }
			else if (abseta > 1.6 && abseta <= 2.1) { num = 0.993172; }
			else if (abseta > 2.1 && abseta <= 2.4) { num = 0.990654; }
		}

		if (idx == 1) { // stat up
			if (abseta >= 0 && abseta <= 0.9) { num = 0.988874; }
			else if (abseta > 0.9 && abseta <= 1.2) { num = 0.997005; }
			else if (abseta > 1.2 && abseta <= 1.6) { num = 0.999124; }
			else if (abseta > 1.6 && abseta <= 2.1) { num = 0.996226; }
			else if (abseta > 2.1 && abseta <= 2.4) { num = 0.995055; }
		}
		if (idx == 2) { // stat down
			if (abseta >= 0 && abseta <= 0.9) { num = 0.982558; }
			else if (abseta > 0.9 && abseta <= 1.2) { num = 0.990409; }
			else if (abseta > 1.2 && abseta <= 1.6) { num = 0.993971; }
			else if (abseta > 1.6 && abseta <= 2.1) { num = 0.989008; }
			else if (abseta > 2.1 && abseta <= 2.4) { num = 0.984224; }
		}
		if (idx == -1 || idx == -3) { // syst up
			if (abseta >= 0 && abseta <= 0.9) { num = 0.986813; }
			else if (abseta > 0.9 && abseta <= 1.2) { num = 0.994583; }
			else if (abseta > 1.2 && abseta <= 1.6) { num = 0.997692; }
			else if (abseta > 1.6 && abseta <= 2.1) { num = 0.994297; }
			else if (abseta > 2.1 && abseta <= 2.4) { num = 0.991597; }
		}
		if (idx == -2 || idx == -4) { // syst down
			if (abseta >= 0 && abseta <= 0.9) { num = 0.984977; }
			else if (abseta > 0.9 && abseta <= 1.2) { num = 0.994074; }
			else if (abseta > 1.2 && abseta <= 1.6) { num = 0.996367; }
			else if (abseta > 1.6 && abseta <= 2.1) { num = 0.992047; }
			else if (abseta > 2.1 && abseta <= 2.4) { num = 0.989711; }
		}
	}

	if (idx == -1 || idx == -2) //add systematic from PF
	{
		double glbtrkNomRD = tnp_weight_glbPFtrk_pbpb(eta, cent, 200); //get the nominal RD value
		double pfSyst = (tnp_uncertainty_pf_pbpb(eta, cent, 200) - tnp_uncertainty_pf_pbpb(eta, cent, 300)) / tnp_uncertainty_pf_pbpb(eta, cent, 300); //relative pf syst uncertainty
		double glbtrkSyst = ((num - glbtrkNomRD) / glbtrkNomRD); //relative glbtrk syst uncertainty on RD
		double totalglbtrkSyst = TMath::Sqrt(pfSyst*pfSyst + glbtrkSyst * glbtrkSyst); //relative total uncertainty
		if (idx == -1) { num = glbtrkNomRD + totalglbtrkSyst * glbtrkNomRD; }//calculate back the actual value
		else { num = glbtrkNomRD - totalglbtrkSyst * glbtrkNomRD; }
		if (num > 1) num = 1; //with addition of PF, numerator could be higher than 1 - unphysical 
	}

	if (idx == -5 || idx == -6) //return 1+- PF syst
	{
		double pfSyst = (tnp_uncertainty_pf_pbpb(eta, cent, 200) - tnp_uncertainty_pf_pbpb(eta, cent, 300)) / tnp_uncertainty_pf_pbpb(eta, cent, 300);
		if (idx == -5) { num = den + pfSyst * den; }
		else { num = den - pfSyst * den; }
	}


	if (idx == 200) den = 1.;
	if (idx == 300) num = den * den;


	return (num / den);

}

//              P F    P b P b           //

double tnp_uncertainty_pf_pbpb(double eta, double cent, int idx)
{
	double num = 1, den = 1;

	if (fabs(eta) > 2.4) { cout << "[WARNING] Muon pseudo-rapidity (" << eta << ") outside [-2.4, 2.4]" << endl; return 1.0; }
	if (cent < 0 || cent>100) { cout << "[ERROR] Centrality (" << cent << ") outside [0%, 100%]" << endl; return 1.0; }

	double abseta = fabs(eta);

	if (cent >= 0 && cent < 40)
	{
		// MC
		if (abseta >= 0 && abseta <= 0.9) { den = 0.990737; }
		else if (abseta > 0.9 && abseta <= 1.2) { den = 0.991441; }
		else if (abseta > 1.2 && abseta <= 1.6) { den = 0.98534; }
		else if (abseta > 1.6 && abseta <= 2.1) { den = 0.991639; }
		else if (abseta > 2.1 && abseta <= 2.4) { den = 0.978643; }


		if (idx == 0 || idx == 200) { 
			if (abseta >= 0 && abseta <= 0.9) { num = 0.989266; }
			else if (abseta > 0.9 && abseta <= 1.2) { num = 0.984868; }
			else if (abseta > 1.2 && abseta <= 1.6) { num = 0.981871; }
			else if (abseta > 1.6 && abseta <= 2.1) { num = 0.984655; }
			else if (abseta > 2.1 && abseta <= 2.4) { num = 0.949866; }
		}

	}
	else if (cent >= 40 && cent <= 100)
	{
		// MC
		if (abseta >= 0 && abseta <= 0.9) { den = 0.997979; }
		else if (abseta > 0.9 && abseta <= 1.2) { den = 0.997599; }
		else if (abseta > 1.2 && abseta <= 1.6) { den = 0.997771; }
		else if (abseta > 1.6 && abseta <= 2.1) { den = 0.997692; }
		else if (abseta > 2.1 && abseta <= 2.4) { den = 0.994569; }

		if (idx == 0 || idx == 200) {
			if (abseta >= 0 && abseta <= 0.9) { num = 0.998014; }
			else if (abseta > 0.9 && abseta <= 1.2) { num = 0.992184; }
			else if (abseta > 1.2 && abseta <= 1.6) { num = 0.996689; }
			else if (abseta > 1.6 && abseta <= 2.1) { num = 0.991708; }
			else if (abseta > 2.1 && abseta <= 2.4) { num = 0.969514; }
		}
	}

	if (idx == 200) den = 1.;
	if (idx == 300) num = den * den;


	return (num / den);


}


///////////////////////////////////////////////////
//                 M u I D    P b P b            //
///////////////////////////////////////////////////
double tnp_weight_muid_pbpb(double eta, int idx)
{
	double syst = 0.6e-2;    //preliminary

	double num=1,den=1;

	if (fabs(eta) > 2.4) { cout << "[WARNING] Muon pseudo-rapidity (" << eta << ") outside [-2.4, 2.4]" << endl; return 1.0; }

	// MC
	if (eta >= -2.4 && eta <= -2.1) { den = 0.994139; }
	else if (eta > -2.1 && eta <= -1.6) { den = 0.99449; }
	else if (eta > -1.6 && eta <= -1.2) { den = 0.983536; }
	else if (eta > -1.2 && eta <= -0.9) { den = 0.964562; }
	else if (eta > -0.9 && eta <= -0.6) { den = 0.973316; }
	else if (eta > -0.6 && eta <= -0.3) { den = 0.981446; }
	else if (eta > -0.3 && eta <= 0) { den = 0.968189; }
	else if (eta > 0 && eta <= 0.3) { den = 0.9617; }
	else if (eta > 0.3 && eta <= 0.6) { den = 0.979738; }
	else if (eta > 0.6 && eta <= 0.9) { den = 0.969536; }
	else if (eta > 0.9 && eta <= 1.2) { den = 0.960259; }
	else if (eta > 1.2 && eta <= 1.6) { den = 0.983279; }
	else if (eta > 1.6 && eta <= 2.1) { den = 0.99477; }
	else if (eta > 2.1 && eta <= 2.4) { den = 0.994065; }


	// data
	if (idx <= 0 || idx > 10) { // nominal
		if (eta >= -2.4 && eta <= -2.1) { num = 0.984278; }
		else if (eta > -2.1 && eta <= -1.6) { num = 0.994031; }
		else if (eta > -1.6 && eta <= -1.2) { num = 0.978562; }
		else if (eta > -1.2 && eta <= -0.9) { num = 0.954321; }
		else if (eta > -0.9 && eta <= -0.6) { num = 0.966508; }
		else if (eta > -0.6 && eta <= -0.3) { num = 0.98402; }
		else if (eta > -0.3 && eta <= 0) { num = 0.958369; }
		else if (eta > 0 && eta <= 0.3) { num = 0.959429; }
		else if (eta > 0.3 && eta <= 0.6) { num = 0.976528; }
		else if (eta > 0.6 && eta <= 0.9) { num = 0.967646; }
		else if (eta > 0.9 && eta <= 1.2) { num = 0.961046; }
		else if (eta > 1.2 && eta <= 1.6) { num = 0.980274; }
		else if (eta > 1.6 && eta <= 2.1) { num = 0.991677; }
		else if (eta > 2.1 && eta <= 2.4) { num = 0.993007; }
	}
	else if (idx == 1) { // stat up
		if (eta >= -2.4 && eta <= -2.1) { num = 0.987203; }
		else if (eta > -2.1 && eta <= -1.6) { num = 0.995641; }
		else if (eta > -1.6 && eta <= -1.2) { num = 0.981641; }
		else if (eta > -1.2 && eta <= -0.9) { num = 0.958889; }
		else if (eta > -0.9 && eta <= -0.6) { num = 0.970274; }
		else if (eta > -0.6 && eta <= -0.3) { num = 0.986882; }
		else if (eta > -0.3 && eta <= 0) { num = 0.962433; }
		else if (eta > 0 && eta <= 0.3) { num = 0.96344; }
		else if (eta > 0.3 && eta <= 0.6) { num = 0.979706; }
		else if (eta > 0.6 && eta <= 0.9) { num = 0.971414; }
		else if (eta > 0.9 && eta <= 1.2) { num = 0.965537; }
		else if (eta > 1.2 && eta <= 1.6) { num = 0.983167; }
		else if (eta > 1.6 && eta <= 2.1) { num = 0.99336; }
		else if (eta > 2.1 && eta <= 2.4) { num = 0.995579; }
	}
	else if (idx == 2) { // stat down
		if (eta >= -2.4 && eta <= -2.1) { num = 0.98094; }
		else if (eta > -2.1 && eta <= -1.6) { num = 0.992199; }
		else if (eta > -1.6 && eta <= -1.2) { num = 0.975247; }
		else if (eta > -1.2 && eta <= -0.9) { num = 0.949482; }
		else if (eta > -0.9 && eta <= -0.6) { num = 0.962497; }
		else if (eta > -0.6 && eta <= -0.3) { num = 0.980916; }
		else if (eta > -0.3 && eta <= 0) { num = 0.954075; }
		else if (eta > 0 && eta <= 0.3) { num = 0.955169; }
		else if (eta > 0.3 && eta <= 0.6) { num = 0.973111; }
		else if (eta > 0.6 && eta <= 0.9) { num = 0.963634; }
		else if (eta > 0.9 && eta <= 1.2) { num = 0.956295; }
		else if (eta > 1.2 && eta <= 1.6) { num = 0.977135; }
		else if (eta > 1.6 && eta <= 2.1) { num = 0.987932; }
		else if (eta > 2.1 && eta <= 2.4) { num = 0.989895; }
	}

	if (idx == 200) den = 1.;
	if (idx == 300) num = den * den;


	double syst_factor = 1.;
	if (idx == -1) syst_factor = 1 + syst;
	if (idx == -2) syst_factor = 1 - syst;
	return (num / den)*syst_factor;
}


///////////////////////////////////////////////////
//               T R G      P b P b              //
///////////////////////////////////////////////////
double tnp_weight_trig_pbpb(double pt, double eta, double cent, int idx)
{
	// Check input variables
	double abseta = fabs(eta);
	if (pt < 15) { cout << "[WARNING] Muon pT (" << pt << ") < 15 GeV/c" << endl; return 1.0; }
	if (abseta > 2.4) { cout << "[WARNING] Muon pseudo-rapidity (" << eta << ") outside [-2.4, 2.4]" << endl; return 1.0; }
	if (cent < 0 || cent>100) { cout << "[ERROR] Centrality (" << cent << ") outside [0%, 100%]" << endl; return 1.0; }

	double num = 1.0, den = 1.0;

	// MC
	if (cent >= 20 && cent <= 100) den = 0.9775;
	else if (abseta >= 0 && abseta < 1.2) {
		if (cent >= 0 && cent < 10) {
			if (pt >= 15 && pt < 20) den = 0.90236;
			else if (pt >= 20 && pt < 30) den = 0.926668;
			else if (pt >= 30 && pt < 50) den = 0.950628;
			else if (pt >= 50 && pt < 80) den = 0.955633;
			else if (pt >= 80 && pt < 9999) den = 0.941845;
		}
		else if (cent >= 10 && cent < 20) {
			if (pt >= 15 && pt < 20) den = 0.945948;
			else if (pt >= 20 && pt < 30) den = 0.957968;
			else if (pt >= 30 && pt < 50) den = 0.966709;
			else if (pt >= 50 && pt < 80) den = 0.96932;
			else if (pt >= 80 && pt < 9999) den = 0.962404;
		}
	}
	else if (abseta >= 1.2 && abseta < 2.1) {
		if (cent >= 0 && cent < 10) {
			if (pt >= 15 && pt < 20) den = 0.886045;
			else if (pt >= 20 && pt < 30) den = 0.905927;
			else if (pt >= 30 && pt < 50) den = 0.927901;
			else if (pt >= 50 && pt < 80) den = 0.93394;
			else if (pt >= 80 && pt < 9999) den = 0.941171;
		}
		else if (cent >= 10 && cent < 20) {
			if (pt >= 15 && pt < 20) den = 0.930556;
			else if (pt >= 20 && pt < 30) den = 0.939308;
			else if (pt >= 30 && pt < 50) den = 0.95477;
			else if (pt >= 50 && pt < 80) den = 0.961815;
			else if (pt >= 80 && pt < 9999) den = 0.961547;
		}
	}
	else if (abseta >= 2.1 && abseta <= 2.4) {
		if (cent >= 0 && cent < 20) {
			if (pt >= 15 && pt < 20) den = 0.851172;
			else if (pt >= 20 && pt < 30) den = 0.883812;
			else if (pt >= 30 && pt < 50) den = 0.910599;
			else if (pt >= 50 && pt < 9999) den = 0.925773;
		}
	}

	// data
	if (idx <= 0 || idx > 10) { // nominal
		if (cent >= 20 && cent <= 100) { num = 0.969; }
		else if (abseta >= 0 && abseta < 1.2) {
			if (cent >= 0 && cent < 10) {
				if (pt >= 15 && pt < 20) num = 0.825623;
				else if (pt >= 20 && pt < 30) num = 0.88834;
				else if (pt >= 30 && pt < 50) num = 0.924312;
				else if (pt >= 50 && pt < 80) num = 0.92636;
				else if (pt >= 80 && pt < 9999) num = 0.957096;
			}
			else if (cent >= 10 && cent < 20) {
				if (pt >= 15 && pt < 20) num = 0.902638;
				else if (pt >= 20 && pt < 30) num = 0.935704;
				else if (pt >= 30 && pt < 50) num = 0.949354;
				else if (pt >= 50 && pt < 80) num = 0.949748;
				else if (pt >= 80 && pt < 9999) num = 0.930156;
			}
		}
		else if (abseta >= 1.2 && abseta < 2.1) {
			if (cent >= 0 && cent < 10) {
				if (pt >= 15 && pt < 20) num = 0.837076;
				else if (pt >= 20 && pt < 30) num = 0.85444;
				else if (pt >= 30 && pt < 50) num = 0.886979;
				else if (pt >= 50 && pt < 80) num = 0.906104;
				else if (pt >= 80 && pt < 9999) num = 0.924293;
			}
			else if (cent >= 10 && cent < 20) {
				if (pt >= 15 && pt < 20) num = 0.887064;
				else if (pt >= 20 && pt < 30) num = 0.914084;
				else if (pt >= 30 && pt < 50) num = 0.924853;
				else if (pt >= 50 && pt < 80) num = 0.948331;
				else if (pt >= 80 && pt < 9999) num = 0.864777;
			}
		}
		else if (abseta >= 2.1 && abseta <= 2.4) {
			if (cent >= 0 && cent < 20) {
				if (pt >= 15 && pt < 20) num = 0.792647;
				else if (pt >= 20 && pt < 30) num = 0.813261;
				else if (pt >= 30 && pt < 50) num = 0.861218;
				else if (pt >= 50 && pt < 9999) num = 0.884202;
			}
		}
	}
	else if (idx == 1) { // stat up
		if (cent >= 20 && cent <= 100) { num = 0.971; }
		else if (abseta >= 0 && abseta < 1.2) {
			if (cent >= 0 && cent < 10) {
				if (pt >= 15 && pt < 20) num = 0.849434;
				else if (pt >= 20 && pt < 30) num = 0.898723;
				else if (pt >= 30 && pt < 50) num = 0.927712;
				else if (pt >= 50 && pt < 80) num = 0.935152;
				else if (pt >= 80 && pt < 9999) num = 0.973337;
			}
			else if (cent >= 10 && cent < 20) {
				if (pt >= 15 && pt < 20) num = 0.929686;
				else if (pt >= 20 && pt < 30) num = 0.944892;
				else if (pt >= 30 && pt < 50) num = 0.952679;
				else if (pt >= 50 && pt < 80) num = 0.957877;
				else if (pt >= 80 && pt < 9999) num = 0.95577;
			}
		}
		else if (abseta >= 1.2 && abseta < 2.1) {
			if (cent >= 0 && cent < 10) {
				if (pt >= 15 && pt < 20) num = 0.86489;
				else if (pt >= 20 && pt < 30) num = 0.866847;
				else if (pt >= 30 && pt < 50) num = 0.892546;
				else if (pt >= 50 && pt < 80) num = 0.919425;
				else if (pt >= 80 && pt < 9999) num = 0.956828;
			}
			else if (cent >= 10 && cent < 20) {
				if (pt >= 15 && pt < 20) num = 0.91195;
				else if (pt >= 20 && pt < 30) num = 0.926608;
				else if (pt >= 30 && pt < 50) num = 0.930635;
				else if (pt >= 50 && pt < 80) num = 0.959537;
				else if (pt >= 80 && pt < 9999) num = 0.914375;
			}
		}
		else if (abseta >= 2.1 && abseta <= 2.4) {
			if (cent >= 0 && cent < 20) {
				if (pt >= 15 && pt < 20) num = 0.834505;
				else if (pt >= 20 && pt < 30) num = 0.834372;
				else if (pt >= 30 && pt < 50) num = 0.870948;
				else if (pt >= 50 && pt < 9999) num = 0.906348;
			}
		}
	}
	else if (idx == 2) { // stat down
		if (cent >= 20 && cent <= 100) { num = 0.967; }
		else if (abseta >= 0 && abseta < 1.2) {
			if (cent >= 0 && cent < 10) {
				if (pt >= 15 && pt < 20) num = 0.799965;
				else if (pt >= 20 && pt < 30) num = 0.877428;
				else if (pt >= 30 && pt < 50) num = 0.920806;
				else if (pt >= 50 && pt < 80) num = 0.916977;
				else if (pt >= 80 && pt < 9999) num = 0.935705;
			}
			else if (cent >= 10 && cent < 20) {
				if (pt >= 15 && pt < 20) num = 0.871347;
				else if (pt >= 20 && pt < 30) num = 0.925738;
				else if (pt >= 30 && pt < 50) num = 0.945883;
				else if (pt >= 50 && pt < 80) num = 0.940674;
				else if (pt >= 80 && pt < 9999) num = 0.897599;
			}
		}
		else if (abseta >= 1.2 && abseta < 2.1) {
			if (cent >= 0 && cent < 10) {
				if (pt >= 15 && pt < 20) num = 0.807131;
				else if (pt >= 20 && pt < 30) num = 0.8416;
				else if (pt >= 30 && pt < 50) num = 0.881247;
				else if (pt >= 50 && pt < 80) num = 0.89155;
				else if (pt >= 80 && pt < 9999) num = 0.880776;
			}
			else if (cent >= 10 && cent < 20) {
				if (pt >= 15 && pt < 20) num = 0.857638;
				else if (pt >= 20 && pt < 30) num = 0.900577;
				else if (pt >= 30 && pt < 50) num = 0.918849;
				else if (pt >= 50 && pt < 80) num = 0.935638;
				else if (pt >= 80 && pt < 9999) num = 0.802326;
			}
		}
		else if (abseta >= 2.1 && abseta <= 2.4) {
			if (cent >= 0 && cent < 20) {
				if (pt >= 15 && pt < 20) num = 0.746978;
				else if (pt >= 20 && pt < 30) num = 0.790995;
				else if (pt >= 30 && pt < 50) num = 0.851077;
				else if (pt >= 50 && pt < 9999) num = 0.859061;
			}
		}
	}
	else if (idx == -1) { // TnP fit syst up
		if (cent >= 20 && cent <= 100) { num = 0.970; }
		else if (abseta >= 0 && abseta < 1.2) {
			if (cent >= 0 && cent < 10) {
				if (pt >= 15 && pt < 20) num = 0.841672;
				else if (pt >= 20 && pt < 30) num = 0.892162;
				else if (pt >= 30 && pt < 50) num = 0.927346;
				else if (pt >= 50 && pt < 80) num = 0.932938;
				else if (pt >= 80 && pt < 9999) num = 0.957671;
			}
			else if (cent >= 10 && cent < 20) {
				if (pt >= 15 && pt < 20) num = 0.914009;
				else if (pt >= 20 && pt < 30) num = 0.940075;
				else if (pt >= 30 && pt < 50) num = 0.950756;
				else if (pt >= 50 && pt < 80) num = 0.950313;
				else if (pt >= 80 && pt < 9999) num = 0.93906;
			}
		}
		else if (abseta >= 1.2 && abseta < 2.1) {
			if (cent >= 0 && cent < 10) {
				if (pt >= 15 && pt < 20) num = 0.864864;
				else if (pt >= 20 && pt < 30) num = 0.863452;
				else if (pt >= 30 && pt < 50) num = 0.89454;
				else if (pt >= 50 && pt < 80) num = 0.911352;
				else if (pt >= 80 && pt < 9999) num = 0.936901;
			}
			else if (cent >= 10 && cent < 20) {
				if (pt >= 15 && pt < 20) num = 0.898048;
				else if (pt >= 20 && pt < 30) num = 0.9333;
				else if (pt >= 30 && pt < 50) num = 0.926577;
				else if (pt >= 50 && pt < 80) num = 0.951679;
				else if (pt >= 80 && pt < 9999) num = 0.885396;
			}
		}
		else if (abseta >= 2.1 && abseta <= 2.4) {
			if (cent >= 0 && cent < 20) {
				if (pt >= 15 && pt < 20) num = 0.814721;
				else if (pt >= 20 && pt < 30) num = 0.825021;
				else if (pt >= 30 && pt < 50) num = 0.865798;
				else if (pt >= 50 && pt < 9999) num = 0.886639;
			}
		}
	}
	else if (idx == -2) { // TnP fit syst down
		if (cent >= 20 && cent <= 100) { num = 0.968; }
		else if (abseta >= 0 && abseta < 1.2) {
			if (cent >= 0 && cent < 10) {
				if (pt >= 15 && pt < 20) num = 0.809573;
				else if (pt >= 20 && pt < 30) num = 0.884517;
				else if (pt >= 30 && pt < 50) num = 0.921277;
				else if (pt >= 50 && pt < 80) num = 0.919781;
				else if (pt >= 80 && pt < 9999) num = 0.956521;
			}
			else if (cent >= 10 && cent < 20) {
				if (pt >= 15 && pt < 20) num = 0.891268;
				else if (pt >= 20 && pt < 30) num = 0.931334;
				else if (pt >= 30 && pt < 50) num = 0.947952;
				else if (pt >= 50 && pt < 80) num = 0.949184;
				else if (pt >= 80 && pt < 9999) num = 0.921253;
			}
		}
		else if (abseta >= 1.2 && abseta < 2.1) {
			if (cent >= 0 && cent < 10) {
				if (pt >= 15 && pt < 20) num = 0.809289;
				else if (pt >= 20 && pt < 30) num = 0.845428;
				else if (pt >= 30 && pt < 50) num = 0.879417;
				else if (pt >= 50 && pt < 80) num = 0.900856;
				else if (pt >= 80 && pt < 9999) num = 0.911685;
			}
			else if (cent >= 10 && cent < 20) {
				if (pt >= 15 && pt < 20) num = 0.87608;
				else if (pt >= 20 && pt < 30) num = 0.894868;
				else if (pt >= 30 && pt < 50) num = 0.923129;
				else if (pt >= 50 && pt < 80) num = 0.944984;
				else if (pt >= 80 && pt < 9999) num = 0.844158;
			}
		}
		else if (abseta >= 2.1 && abseta <= 2.4) {
			if (cent >= 0 && cent < 20) {
				if (pt >= 15 && pt < 20) num = 0.770573;
				else if (pt >= 20 && pt < 30) num = 0.8015;
				else if (pt >= 30 && pt < 50) num = 0.856638;
				else if (pt >= 50 && pt < 9999) num = 0.881766;
			}
		}
	}

	if (idx == 200) den = 1.0;
	if (idx == 300) num = den * den;

	return (num / den);
}

pair<double, double> tnp_trig_pbpb(double pt, double eta, double cent, int idx)
{
	// Check input variables
	double abseta = fabs(eta);
	if (pt < 15) { cout << "[WARNING] Muon pT (" << pt << ") < 15 GeV/c" << endl; return pair<double, double>(1.0, 1.0); }
	if (abseta > 2.4) { cout << "[WARNING] Muon pseudo-rapidity (" << eta << ") outside [-2.4, 2.4]" << endl; return pair<double, double>(1.0, 1.0); }
	if (cent < 0 || cent>100) { cout << "[ERROR] Centrality (" << cent << ") outside [0%, 100%]" << endl; return pair<double, double>(1.0, 1.0); }

	double num = 1.0, den = 1.0;

	// MC
	if (cent >= 20 && cent <= 100) den = 0.9775;
	else if (abseta >= 0 && abseta < 1.2) {
		if (cent >= 0 && cent < 10) {
			if (pt >= 15 && pt < 20) den = 0.90236;
			else if (pt >= 20 && pt < 30) den = 0.926668;
			else if (pt >= 30 && pt < 50) den = 0.950628;
			else if (pt >= 50 && pt < 80) den = 0.955633;
			else if (pt >= 80 && pt < 9999) den = 0.941845;
		}
		else if (cent >= 10 && cent < 20) {
			if (pt >= 15 && pt < 20) den = 0.945948;
			else if (pt >= 20 && pt < 30) den = 0.957968;
			else if (pt >= 30 && pt < 50) den = 0.966709;
			else if (pt >= 50 && pt < 80) den = 0.96932;
			else if (pt >= 80 && pt < 9999) den = 0.962404;
		}
	}
	else if (abseta >= 1.2 && abseta < 2.1) {
		if (cent >= 0 && cent < 10) {
			if (pt >= 15 && pt < 20) den = 0.886045;
			else if (pt >= 20 && pt < 30) den = 0.905927;
			else if (pt >= 30 && pt < 50) den = 0.927901;
			else if (pt >= 50 && pt < 80) den = 0.93394;
			else if (pt >= 80 && pt < 9999) den = 0.941171;
		}
		else if (cent >= 10 && cent < 20) {
			if (pt >= 15 && pt < 20) den = 0.930556;
			else if (pt >= 20 && pt < 30) den = 0.939308;
			else if (pt >= 30 && pt < 50) den = 0.95477;
			else if (pt >= 50 && pt < 80) den = 0.961815;
			else if (pt >= 80 && pt < 9999) den = 0.961547;
		}
	}
	else if (abseta >= 2.1 && abseta <= 2.4) {
		if (cent >= 0 && cent < 20) {
			if (pt >= 15 && pt < 20) den = 0.851172;
			else if (pt >= 20 && pt < 30) den = 0.883812;
			else if (pt >= 30 && pt < 50) den = 0.910599;
			else if (pt >= 50 && pt < 9999) den = 0.925773;
		}
	}

	// data
	if (idx <= 0 || idx > 10) { // nominal
		if (cent >= 20 && cent <= 100) { num = 0.969; }
		else if (abseta >= 0 && abseta < 1.2) {
			if (cent >= 0 && cent < 10) {
				if (pt >= 15 && pt < 20) num = 0.825623;
				else if (pt >= 20 && pt < 30) num = 0.88834;
				else if (pt >= 30 && pt < 50) num = 0.924312;
				else if (pt >= 50 && pt < 80) num = 0.92636;
				else if (pt >= 80 && pt < 9999) num = 0.957096;
			}
			else if (cent >= 10 && cent < 20) {
				if (pt >= 15 && pt < 20) num = 0.902638;
				else if (pt >= 20 && pt < 30) num = 0.935704;
				else if (pt >= 30 && pt < 50) num = 0.949354;
				else if (pt >= 50 && pt < 80) num = 0.949748;
				else if (pt >= 80 && pt < 9999) num = 0.930156;
			}
		}
		else if (abseta >= 1.2 && abseta < 2.1) {
			if (cent >= 0 && cent < 10) {
				if (pt >= 15 && pt < 20) num = 0.837076;
				else if (pt >= 20 && pt < 30) num = 0.85444;
				else if (pt >= 30 && pt < 50) num = 0.886979;
				else if (pt >= 50 && pt < 80) num = 0.906104;
				else if (pt >= 80 && pt < 9999) num = 0.924293;
			}
			else if (cent >= 10 && cent < 20) {
				if (pt >= 15 && pt < 20) num = 0.887064;
				else if (pt >= 20 && pt < 30) num = 0.914084;
				else if (pt >= 30 && pt < 50) num = 0.924853;
				else if (pt >= 50 && pt < 80) num = 0.948331;
				else if (pt >= 80 && pt < 9999) num = 0.864777;
			}
		}
		else if (abseta >= 2.1 && abseta <= 2.4) {
			if (cent >= 0 && cent < 20) {
				if (pt >= 15 && pt < 20) num = 0.792647;
				else if (pt >= 20 && pt < 30) num = 0.813261;
				else if (pt >= 30 && pt < 50) num = 0.861218;
				else if (pt >= 50 && pt < 9999) num = 0.884202;
			}
		}
	}
	else if (idx == 1) { // stat up
		if (cent >= 20 && cent <= 100) { num = 0.971; }
		else if (abseta >= 0 && abseta < 1.2) {
			if (cent >= 0 && cent < 10) {
				if (pt >= 15 && pt < 20) num = 0.849434;
				else if (pt >= 20 && pt < 30) num = 0.898723;
				else if (pt >= 30 && pt < 50) num = 0.927712;
				else if (pt >= 50 && pt < 80) num = 0.935152;
				else if (pt >= 80 && pt < 9999) num = 0.973337;
			}
			else if (cent >= 10 && cent < 20) {
				if (pt >= 15 && pt < 20) num = 0.929686;
				else if (pt >= 20 && pt < 30) num = 0.944892;
				else if (pt >= 30 && pt < 50) num = 0.952679;
				else if (pt >= 50 && pt < 80) num = 0.957877;
				else if (pt >= 80 && pt < 9999) num = 0.95577;
			}
		}
		else if (abseta >= 1.2 && abseta < 2.1) {
			if (cent >= 0 && cent < 10) {
				if (pt >= 15 && pt < 20) num = 0.86489;
				else if (pt >= 20 && pt < 30) num = 0.866847;
				else if (pt >= 30 && pt < 50) num = 0.892546;
				else if (pt >= 50 && pt < 80) num = 0.919425;
				else if (pt >= 80 && pt < 9999) num = 0.956828;
			}
			else if (cent >= 10 && cent < 20) {
				if (pt >= 15 && pt < 20) num = 0.91195;
				else if (pt >= 20 && pt < 30) num = 0.926608;
				else if (pt >= 30 && pt < 50) num = 0.930635;
				else if (pt >= 50 && pt < 80) num = 0.959537;
				else if (pt >= 80 && pt < 9999) num = 0.914375;
			}
		}
		else if (abseta >= 2.1 && abseta <= 2.4) {
			if (cent >= 0 && cent < 20) {
				if (pt >= 15 && pt < 20) num = 0.834505;
				else if (pt >= 20 && pt < 30) num = 0.834372;
				else if (pt >= 30 && pt < 50) num = 0.870948;
				else if (pt >= 50 && pt < 9999) num = 0.906348;
			}
		}
	}
	else if (idx == 2) { // stat down
		if (cent >= 20 && cent <= 100) { num = 0.967; }
		else if (abseta >= 0 && abseta < 1.2) {
			if (cent >= 0 && cent < 10) {
				if (pt >= 15 && pt < 20) num = 0.799965;
				else if (pt >= 20 && pt < 30) num = 0.877428;
				else if (pt >= 30 && pt < 50) num = 0.920806;
				else if (pt >= 50 && pt < 80) num = 0.916977;
				else if (pt >= 80 && pt < 9999) num = 0.935705;
			}
			else if (cent >= 10 && cent < 20) {
				if (pt >= 15 && pt < 20) num = 0.871347;
				else if (pt >= 20 && pt < 30) num = 0.925738;
				else if (pt >= 30 && pt < 50) num = 0.945883;
				else if (pt >= 50 && pt < 80) num = 0.940674;
				else if (pt >= 80 && pt < 9999) num = 0.897599;
			}
		}
		else if (abseta >= 1.2 && abseta < 2.1) {
			if (cent >= 0 && cent < 10) {
				if (pt >= 15 && pt < 20) num = 0.807131;
				else if (pt >= 20 && pt < 30) num = 0.8416;
				else if (pt >= 30 && pt < 50) num = 0.881247;
				else if (pt >= 50 && pt < 80) num = 0.89155;
				else if (pt >= 80 && pt < 9999) num = 0.880776;
			}
			else if (cent >= 10 && cent < 20) {
				if (pt >= 15 && pt < 20) num = 0.857638;
				else if (pt >= 20 && pt < 30) num = 0.900577;
				else if (pt >= 30 && pt < 50) num = 0.918849;
				else if (pt >= 50 && pt < 80) num = 0.935638;
				else if (pt >= 80 && pt < 9999) num = 0.802326;
			}
		}
		else if (abseta >= 2.1 && abseta <= 2.4) {
			if (cent >= 0 && cent < 20) {
				if (pt >= 15 && pt < 20) num = 0.746978;
				else if (pt >= 20 && pt < 30) num = 0.790995;
				else if (pt >= 30 && pt < 50) num = 0.851077;
				else if (pt >= 50 && pt < 9999) num = 0.859061;
			}
		}
	}
	else if (idx == -1) { // TnP fit syst up
		if (cent >= 20 && cent <= 100) { num = 0.970; }
		else if (abseta >= 0 && abseta < 1.2) {
			if (cent >= 0 && cent < 10) {
				if (pt >= 15 && pt < 20) num = 0.841672;
				else if (pt >= 20 && pt < 30) num = 0.892162;
				else if (pt >= 30 && pt < 50) num = 0.927346;
				else if (pt >= 50 && pt < 80) num = 0.932938;
				else if (pt >= 80 && pt < 9999) num = 0.957671;
			}
			else if (cent >= 10 && cent < 20) {
				if (pt >= 15 && pt < 20) num = 0.914009;
				else if (pt >= 20 && pt < 30) num = 0.940075;
				else if (pt >= 30 && pt < 50) num = 0.950756;
				else if (pt >= 50 && pt < 80) num = 0.950313;
				else if (pt >= 80 && pt < 9999) num = 0.93906;
			}
		}
		else if (abseta >= 1.2 && abseta < 2.1) {
			if (cent >= 0 && cent < 10) {
				if (pt >= 15 && pt < 20) num = 0.864864;
				else if (pt >= 20 && pt < 30) num = 0.863452;
				else if (pt >= 30 && pt < 50) num = 0.89454;
				else if (pt >= 50 && pt < 80) num = 0.911352;
				else if (pt >= 80 && pt < 9999) num = 0.936901;
			}
			else if (cent >= 10 && cent < 20) {
				if (pt >= 15 && pt < 20) num = 0.898048;
				else if (pt >= 20 && pt < 30) num = 0.9333;
				else if (pt >= 30 && pt < 50) num = 0.926577;
				else if (pt >= 50 && pt < 80) num = 0.951679;
				else if (pt >= 80 && pt < 9999) num = 0.885396;
			}
		}
		else if (abseta >= 2.1 && abseta <= 2.4) {
			if (cent >= 0 && cent < 20) {
				if (pt >= 15 && pt < 20) num = 0.814721;
				else if (pt >= 20 && pt < 30) num = 0.825021;
				else if (pt >= 30 && pt < 50) num = 0.865798;
				else if (pt >= 50 && pt < 9999) num = 0.886639;
			}
		}
	}
	else if (idx == -2) { // TnP fit syst down
		if (cent >= 20 && cent <= 100) { num = 0.968; }
		else if (abseta >= 0 && abseta < 1.2) {
			if (cent >= 0 && cent < 10) {
				if (pt >= 15 && pt < 20) num = 0.809573;
				else if (pt >= 20 && pt < 30) num = 0.884517;
				else if (pt >= 30 && pt < 50) num = 0.921277;
				else if (pt >= 50 && pt < 80) num = 0.919781;
				else if (pt >= 80 && pt < 9999) num = 0.956521;
			}
			else if (cent >= 10 && cent < 20) {
				if (pt >= 15 && pt < 20) num = 0.891268;
				else if (pt >= 20 && pt < 30) num = 0.931334;
				else if (pt >= 30 && pt < 50) num = 0.947952;
				else if (pt >= 50 && pt < 80) num = 0.949184;
				else if (pt >= 80 && pt < 9999) num = 0.921253;
			}
		}
		else if (abseta >= 1.2 && abseta < 2.1) {
			if (cent >= 0 && cent < 10) {
				if (pt >= 15 && pt < 20) num = 0.809289;
				else if (pt >= 20 && pt < 30) num = 0.845428;
				else if (pt >= 30 && pt < 50) num = 0.879417;
				else if (pt >= 50 && pt < 80) num = 0.900856;
				else if (pt >= 80 && pt < 9999) num = 0.911685;
			}
			else if (cent >= 10 && cent < 20) {
				if (pt >= 15 && pt < 20) num = 0.87608;
				else if (pt >= 20 && pt < 30) num = 0.894868;
				else if (pt >= 30 && pt < 50) num = 0.923129;
				else if (pt >= 50 && pt < 80) num = 0.944984;
				else if (pt >= 80 && pt < 9999) num = 0.844158;
			}
		}
		else if (abseta >= 2.1 && abseta <= 2.4) {
			if (cent >= 0 && cent < 20) {
				if (pt >= 15 && pt < 20) num = 0.770573;
				else if (pt >= 20 && pt < 30) num = 0.8015;
				else if (pt >= 30 && pt < 50) num = 0.856638;
				else if (pt >= 50 && pt < 9999) num = 0.881766;
			}
		}
	}

	if (idx == 200) den = 1.0;
	if (idx == 300) num = den * den;

	return pair<double, double>(num, den);
}

double tnp_weight_trig_double_pbpb(double pt1, double eta1, double cent1, double pt2, double eta2, double cent2, int idx)
{
   pair<double, double> eff1 = tnp_trig_pbpb(pt1, eta1, cent1, idx);
   pair<double, double> eff2 = tnp_trig_pbpb(pt2, eta2, cent2, idx);

   return (1 - (1 - eff1.first) * (1 - eff2.first) / (1 - (1 - eff1.second) * (1 - eff2.second)));
}

double tnp_weight_TightID_pp(double eta, int idx)
{
   if(idx == 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 0.99197;
      if(eta < -1.6 && eta >= -2.1)   return 0.997666;
      if(eta < -1.2 && eta >= -1.6)   return 0.996269;
      if(eta < -0.9 && eta >= -1.2)   return 0.994066;
      if(eta < -0.6 && eta >= -0.9)   return 0.996537;
      if(eta < -0.3 && eta >= -0.6)   return 0.995147;
      if(eta < 0 && eta >= -0.3)   return 0.997214;
      if(eta < 0.3 && eta >= 0)   return 0.989001;
      if(eta < 0.6 && eta >= 0.3)   return 0.992532;
      if(eta < 0.9 && eta >= 0.6)   return 0.997035;
      if(eta < 1.2 && eta >= 0.9)   return 0.995832;
      if(eta < 1.6 && eta >= 1.2)   return 0.996298;
      if(eta < 2.1 && eta >= 1.6)   return 0.995379;
      if(eta < 2.4 && eta >= 2.1)   return 0.986206;
   }

   if(idx < 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 0.98798;
      if(eta < -1.6 && eta >= -2.1)   return 0.994208;
      if(eta < -1.2 && eta >= -1.6)   return 0.994789;
      if(eta < -0.9 && eta >= -1.2)   return 0.990871;
      if(eta < -0.6 && eta >= -0.9)   return 0.994272;
      if(eta < -0.3 && eta >= -0.6)   return 0.993019;
      if(eta < 0 && eta >= -0.3)   return 0.994244;
      if(eta < 0.3 && eta >= 0)   return 0.985501;
      if(eta < 0.6 && eta >= 0.3)   return 0.989337;
      if(eta < 0.9 && eta >= 0.6)   return 0.993503;
      if(eta < 1.2 && eta >= 0.9)   return 0.992745;
      if(eta < 1.6 && eta >= 1.2)   return 0.992151;
      if(eta < 2.1 && eta >= 1.6)   return 0.993462;
      if(eta < 2.4 && eta >= 2.1)   return 0.97791;
   }

   if(idx > 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 0.99583;
      if(eta < -1.6 && eta >= -2.1)   return 1.00109;
      if(eta < -1.2 && eta >= -1.6)   return 0.997653;
      if(eta < -0.9 && eta >= -1.2)   return 0.997178;
      if(eta < -0.6 && eta >= -0.9)   return 0.998698;
      if(eta < -0.3 && eta >= -0.6)   return 0.997167;
      if(eta < 0 && eta >= -0.3)   return 1.0001;
      if(eta < 0.3 && eta >= 0)   return 0.992336;
      if(eta < 0.6 && eta >= 0.3)   return 0.995651;
      if(eta < 0.9 && eta >= 0.6)   return 1.0005;
      if(eta < 1.2 && eta >= 0.9)   return 0.998838;
      if(eta < 1.6 && eta >= 1.2)   return 1.00041;
      if(eta < 2.1 && eta >= 1.6)   return 0.997219;
      if(eta < 2.4 && eta >= 2.1)   return 0.994454;
   }

   return 1;
}

double tnp_weight_TightID_PFIsoTight_pp(double eta, int idx)
{
   if(idx == 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 0.995003;
      if(eta < -1.6 && eta >= -2.1)   return 1.00209;
      if(eta < -1.2 && eta >= -1.6)   return 0.995934;
      if(eta < -0.9 && eta >= -1.2)   return 0.989854;
      if(eta < -0.6 && eta >= -0.9)   return 0.993851;
      if(eta < -0.3 && eta >= -0.6)   return 0.992648;
      if(eta < 0 && eta >= -0.3)   return 0.994836;
      if(eta < 0.3 && eta >= 0)   return 0.988882;
      if(eta < 0.6 && eta >= 0.3)   return 0.99508;
      if(eta < 0.9 && eta >= 0.6)   return 0.998866;
      if(eta < 1.2 && eta >= 0.9)   return 0.992233;
      if(eta < 1.6 && eta >= 1.2)   return 0.99683;
      if(eta < 2.1 && eta >= 1.6)   return 0.995585;
      if(eta < 2.4 && eta >= 2.1)   return 0.991093;
   }

   if(idx < 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 0.989961;
      if(eta < -1.6 && eta >= -2.1)   return 0.998392;
      if(eta < -1.2 && eta >= -1.6)   return 0.991391;
      if(eta < -0.9 && eta >= -1.2)   return 0.984239;
      if(eta < -0.6 && eta >= -0.9)   return 0.989812;
      if(eta < -0.3 && eta >= -0.6)   return 0.987306;
      if(eta < 0 && eta >= -0.3)   return 0.98971;
      if(eta < 0.3 && eta >= 0)   return 0.984;
      if(eta < 0.6 && eta >= 0.3)   return 0.99131;
      if(eta < 0.9 && eta >= 0.6)   return 0.991863;
      if(eta < 1.2 && eta >= 0.9)   return 0.988633;
      if(eta < 1.6 && eta >= 1.2)   return 0.992627;
      if(eta < 2.1 && eta >= 1.6)   return 0.99141;
      if(eta < 2.4 && eta >= 2.1)   return 0.983076;
   }

   if(idx > 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 0.999905;
      if(eta < -1.6 && eta >= -2.1)   return 1.00574;
      if(eta < -1.2 && eta >= -1.6)   return 1.00042;
      if(eta < -0.9 && eta >= -1.2)   return 0.995397;
      if(eta < -0.6 && eta >= -0.9)   return 0.997803;
      if(eta < -0.3 && eta >= -0.6)   return 0.997924;
      if(eta < 0 && eta >= -0.3)   return 0.999897;
      if(eta < 0.3 && eta >= 0)   return 0.993715;
      if(eta < 0.6 && eta >= 0.3)   return 0.998762;
      if(eta < 0.9 && eta >= 0.6)   return 1.00581;
      if(eta < 1.2 && eta >= 0.9)   return 0.995745;
      if(eta < 1.6 && eta >= 1.2)   return 1.00097;
      if(eta < 2.1 && eta >= 1.6)   return 0.999707;
      if(eta < 2.4 && eta >= 2.1)   return 0.999017;
   }

   return 1;
}

double tnp_weight_L3Mu12_pp(double eta, int idx)
{
   if(idx == 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 1.00347;
      if(eta < -1.6 && eta >= -2.1)   return 0.977317;
      if(eta < -1.2 && eta >= -1.6)   return 0.984707;
      if(eta < -0.9 && eta >= -1.2)   return 0.976465;
      if(eta < -0.6 && eta >= -0.9)   return 0.983223;
      if(eta < -0.3 && eta >= -0.6)   return 0.990828;
      if(eta < 0 && eta >= -0.3)   return 0.987445;
      if(eta < 0.3 && eta >= 0)   return 0.989734;
      if(eta < 0.6 && eta >= 0.3)   return 0.986748;
      if(eta < 0.9 && eta >= 0.6)   return 0.976808;
      if(eta < 1.2 && eta >= 0.9)   return 0.969258;
      if(eta < 1.6 && eta >= 1.2)   return 0.969321;
      if(eta < 2.1 && eta >= 1.6)   return 0.98434;
      if(eta < 2.4 && eta >= 2.1)   return 0.999777;
   }

   if(idx < 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 0.997391;
      if(eta < -1.6 && eta >= -2.1)   return 0.973319;
      if(eta < -1.2 && eta >= -1.6)   return 0.981583;
      if(eta < -0.9 && eta >= -1.2)   return 0.973346;
      if(eta < -0.6 && eta >= -0.9)   return 0.979763;
      if(eta < -0.3 && eta >= -0.6)   return 0.988388;
      if(eta < 0 && eta >= -0.3)   return 0.984145;
      if(eta < 0.3 && eta >= 0)   return 0.986435;
      if(eta < 0.6 && eta >= 0.3)   return 0.984174;
      if(eta < 0.9 && eta >= 0.6)   return 0.973015;
      if(eta < 1.2 && eta >= 0.9)   return 0.965251;
      if(eta < 1.6 && eta >= 1.2)   return 0.964534;
      if(eta < 2.1 && eta >= 1.6)   return 0.980845;
      if(eta < 2.4 && eta >= 2.1)   return 0.994351;
   }

   if(idx > 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 1.00957;
      if(eta < -1.6 && eta >= -2.1)   return 0.98125;
      if(eta < -1.2 && eta >= -1.6)   return 0.987747;
      if(eta < -0.9 && eta >= -1.2)   return 0.979478;
      if(eta < -0.6 && eta >= -0.9)   return 0.986607;
      if(eta < -0.3 && eta >= -0.6)   return 0.993167;
      if(eta < 0 && eta >= -0.3)   return 0.990665;
      if(eta < 0.3 && eta >= 0)   return 0.992894;
      if(eta < 0.6 && eta >= 0.3)   return 0.989243;
      if(eta < 0.9 && eta >= 0.6)   return 0.980524;
      if(eta < 1.2 && eta >= 0.9)   return 0.973275;
      if(eta < 1.6 && eta >= 1.2)   return 0.974044;
      if(eta < 2.1 && eta >= 1.6)   return 0.987806;
      if(eta < 2.4 && eta >= 2.1)   return 1.00509;
   }

   return 1;
}

double tnp_weight_L3Mu12_PFIsoTight_pp(double eta, int idx)
{
   if(idx == 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 1.00373;
      if(eta < -1.6 && eta >= -2.1)   return 0.980444;
      if(eta < -1.2 && eta >= -1.6)   return 0.98444;
      if(eta < -0.9 && eta >= -1.2)   return 0.976874;
      if(eta < -0.6 && eta >= -0.9)   return 0.982639;
      if(eta < -0.3 && eta >= -0.6)   return 0.990206;
      if(eta < 0 && eta >= -0.3)   return 0.987445;
      if(eta < 0.3 && eta >= 0)   return 0.989312;
      if(eta < 0.6 && eta >= 0.3)   return 0.987004;
      if(eta < 0.9 && eta >= 0.6)   return 0.977648;
      if(eta < 1.2 && eta >= 0.9)   return 0.969058;
      if(eta < 1.6 && eta >= 1.2)   return 0.96981;
      if(eta < 2.1 && eta >= 1.6)   return 0.983832;
      if(eta < 2.4 && eta >= 2.1)   return 1.00299;
   }

   if(idx < 0)
   {
      if(eta < -2.10048 && eta >= -2.40048)   return 0.998226;
      if(eta < -1.60009 && eta >= -2.10009)   return 0.975636;
      if(eta < -1.20007 && eta >= -1.60007)   return 0.981422;
      if(eta < -0.89995 && eta >= -1.19995)   return 0.973333;
      if(eta < -0.600216 && eta >= -0.900216)   return 0.973606;
      if(eta < -0.300024 && eta >= -0.600024)   return 0.987991;
      if(eta < -2.1e-05 && eta >= -0.300021)   return 0.986451;
      if(eta < 0.300381 && eta >= 0.000381)   return 0.986263;
      if(eta < 0.600264 && eta >= 0.300264)   return 0.984614;
      if(eta < 0.900435 && eta >= 0.600435)   return 0.974304;
      if(eta < 1.20062 && eta >= 0.90062)   return 0.965644;
      if(eta < 1.60033 && eta >= 1.20033)   return 0.966116;
      if(eta < 2.10042 && eta >= 1.60042)   return 0.9808;
      if(eta < 2.40001 && eta >= 2.10001)   return 0.99785;
   }

   if(idx > 0)
   {
      if(eta < -2.10048 && eta >= -2.40048)   return 1.00914;
      if(eta < -1.60009 && eta >= -2.10009)   return 0.985205;
      if(eta < -1.20007 && eta >= -1.60007)   return 0.987403;
      if(eta < -0.89995 && eta >= -1.19995)   return 0.980295;
      if(eta < -0.600216 && eta >= -0.900216)   return 0.99167;
      if(eta < -0.300024 && eta >= -0.600024)   return 0.992304;
      if(eta < -2.1e-05 && eta >= -0.300021)   return 0.990154;
      if(eta < 0.300381 && eta >= 0.000381)   return 0.992101;
      if(eta < 0.600264 && eta >= 0.300264)   return 0.989287;
      if(eta < 0.900435 && eta >= 0.600435)   return 0.98086;
      if(eta < 1.20062 && eta >= 0.90062)   return 0.972356;
      if(eta < 1.60033 && eta >= 1.20033)   return 0.973436;
      if(eta < 2.10042 && eta >= 1.60042)   return 0.986793;
      if(eta < 2.40001 && eta >= 2.10001)   return 1.00756;
   }

   return 1;
}

double eff_data_TightID_pp(double eta, int idx)
{
   if(idx == 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 0.980363;
      if(eta < -1.6 && eta >= -2.1)   return 0.990488;
      if(eta < -1.2 && eta >= -1.6)   return 0.99111;
      if(eta < -0.9 && eta >= -1.2)   return 0.980775;
      if(eta < -0.6 && eta >= -0.9)   return 0.981737;
      if(eta < -0.3 && eta >= -0.6)   return 0.986948;
      if(eta < 0 && eta >= -0.3)   return 0.976698;
      if(eta < 0.3 && eta >= 0)   return 0.96503;
      if(eta < 0.6 && eta >= 0.3)   return 0.984882;
      if(eta < 0.9 && eta >= 0.6)   return 0.982721;
      if(eta < 1.2 && eta >= 0.9)   return 0.981427;
      if(eta < 1.6 && eta >= 1.2)   return 0.990071;
      if(eta < 2.1 && eta >= 1.6)   return 0.988729;
      if(eta < 2.4 && eta >= 2.1)   return 0.976536;
   }
   if(idx < 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 0.977617;
      if(eta < -1.6 && eta >= -2.1)   return 0.989061;
      if(eta < -1.2 && eta >= -1.6)   return 0.989677;
      if(eta < -0.9 && eta >= -1.2)   return 0.978646;
      if(eta < -0.6 && eta >= -0.9)   return 0.979622;
      if(eta < -0.3 && eta >= -0.6)   return 0.985045;
      if(eta < 0 && eta >= -0.3)   return 0.974316;
      if(eta < 0.3 && eta >= 0)   return 0.962151;
      if(eta < 0.6 && eta >= 0.3)   return 0.98299;
      if(eta < 0.9 && eta >= 0.6)   return 0.980556;
      if(eta < 1.2 && eta >= 0.9)   return 0.979353;
      if(eta < 1.6 && eta >= 1.2)   return 0.988518;
      if(eta < 2.1 && eta >= 1.6)   return 0.987202;
      if(eta < 2.4 && eta >= 2.1)   return 0.973714;
   }
   if(idx > 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 0.982916;
      if(eta < -1.6 && eta >= -2.1)   return 0.991818;
      if(eta < -1.2 && eta >= -1.6)   return 0.992444;
      if(eta < -0.9 && eta >= -1.2)   return 0.982779;
      if(eta < -0.6 && eta >= -0.9)   return 0.98374;
      if(eta < -0.3 && eta >= -0.6)   return 0.98873;
      if(eta < 0 && eta >= -0.3)   return 0.978967;
      if(eta < 0.3 && eta >= 0)   return 0.967743;
      if(eta < 0.6 && eta >= 0.3)   return 0.986661;
      if(eta < 0.9 && eta >= 0.6)   return 0.984768;
      if(eta < 1.2 && eta >= 0.9)   return 0.983379;
      if(eta < 1.6 && eta >= 1.2)   return 0.99152;
      if(eta < 2.1 && eta >= 1.6)   return 0.990158;
      if(eta < 2.4 && eta >= 2.1)   return 0.979216;
   }

   return 1;
}

double eff_data_TightID_PFIsoTight_pp(double eta, int idx)
{
   if(idx == 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 0.967933;
      if(eta < -1.6 && eta >= -2.1)   return 0.97857;
      if(eta < -1.2 && eta >= -1.6)   return 0.973498;
      if(eta < -0.9 && eta >= -1.2)   return 0.955818;
      if(eta < -0.6 && eta >= -0.9)   return 0.958638;
      if(eta < -0.3 && eta >= -0.6)   return 0.9636;
      if(eta < 0 && eta >= -0.3)   return 0.954626;
      if(eta < 0.3 && eta >= 0)   return 0.944272;
      if(eta < 0.6 && eta >= 0.3)   return 0.966709;
      if(eta < 0.9 && eta >= 0.6)   return 0.964161;
      if(eta < 1.2 && eta >= 0.9)   return 0.95823;
      if(eta < 1.6 && eta >= 1.2)   return 0.972341;
      if(eta < 2.1 && eta >= 1.6)   return 0.972413;
      if(eta < 2.4 && eta >= 2.1)   return 0.967299;
   }
   if(idx < 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 0.964056;
      if(eta < -1.6 && eta >= -2.1)   return 0.976179;
      if(eta < -1.2 && eta >= -1.6)   return 0.970836;
      if(eta < -0.9 && eta >= -1.2)   return 0.952303;
      if(eta < -0.6 && eta >= -0.9)   return 0.955378;
      if(eta < -0.3 && eta >= -0.6)   return 0.960416;
      if(eta < 0 && eta >= -0.3)   return 0.95126;
      if(eta < 0.3 && eta >= 0)   return 0.940684;
      if(eta < 0.6 && eta >= 0.3)   return 0.963754;
      if(eta < 0.9 && eta >= 0.6)   return 0.960919;
      if(eta < 1.2 && eta >= 0.9)   return 0.954933;
      if(eta < 1.6 && eta >= 1.2)   return 0.969613;
      if(eta < 2.1 && eta >= 1.6)   return 0.969783;
      if(eta < 2.4 && eta >= 2.1)   return 0.963294;
   }
   if(idx > 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 0.971625;
      if(eta < -1.6 && eta >= -2.1)   return 0.980871;
      if(eta < -1.2 && eta >= -1.6)   return 0.976059;
      if(eta < -0.9 && eta >= -1.2)   return 0.959215;
      if(eta < -0.6 && eta >= -0.9)   return 0.961788;
      if(eta < -0.3 && eta >= -0.6)   return 0.966671;
      if(eta < 0 && eta >= -0.3)   return 0.95789;
      if(eta < 0.3 && eta >= 0)   return 0.947792;
      if(eta < 0.6 && eta >= 0.3)   return 0.96955;
      if(eta < 0.9 && eta >= 0.6)   return 0.967282;
      if(eta < 1.2 && eta >= 0.9)   return 0.961429;
      if(eta < 1.6 && eta >= 1.2)   return 0.974968;
      if(eta < 2.1 && eta >= 1.6)   return 0.974957;
      if(eta < 2.4 && eta >= 2.1)   return 0.971113;
   }
   return 1;
}

double eff_data_L3Mu12_pp(double eta, int idx)
{
   if(idx == 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 0.929123;
      if(eta < -1.6 && eta >= -2.1)   return 0.908958;
      if(eta < -1.2 && eta >= -1.6)   return 0.955541;
      if(eta < -0.9 && eta >= -1.2)   return 0.95538;
      if(eta < -0.6 && eta >= -0.9)   return 0.94576;
      if(eta < -0.3 && eta >= -0.6)   return 0.975668;
      if(eta < 0 && eta >= -0.3)   return 0.955186;
      if(eta < 0.3 && eta >= 0)   return 0.95791;
      if(eta < 0.6 && eta >= 0.3)   return 0.970682;
      if(eta < 0.9 && eta >= 0.6)   return 0.943816;
      if(eta < 1.2 && eta >= 0.9)   return 0.949827;
      if(eta < 1.6 && eta >= 1.2)   return 0.931801;
      if(eta < 2.1 && eta >= 1.6)   return 0.942522;
      if(eta < 2.4 && eta >= 2.1)   return 0.93423;
   }
   if(idx < 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 0.924441;
      if(eta < -1.6 && eta >= -2.1)   return 0.905533;
      if(eta < -1.2 && eta >= -1.6)   return 0.952759;
      if(eta < -0.9 && eta >= -1.2)   return 0.952328;
      if(eta < -0.6 && eta >= -0.9)   return 0.942796;
      if(eta < -0.3 && eta >= -0.6)   return 0.973456;
      if(eta < 0 && eta >= -0.3)   return 0.952279;
      if(eta < 0.3 && eta >= 0)   return 0.954979;
      if(eta < 0.6 && eta >= 0.3)   return 0.968354;
      if(eta < 0.9 && eta >= 0.6)   return 0.940699;
      if(eta < 1.2 && eta >= 0.9)   return 0.94669;
      if(eta < 1.6 && eta >= 1.2)   return 0.928459;
      if(eta < 2.1 && eta >= 1.6)   return 0.939454;
      if(eta < 2.4 && eta >= 2.1)   return 0.929886;
   }
   if(idx > 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 0.933838;
      if(eta < -1.6 && eta >= -2.1)   return 0.912303;
      if(eta < -1.2 && eta >= -1.6)   return 0.958228;
      if(eta < -0.9 && eta >= -1.2)   return 0.958323;
      if(eta < -0.6 && eta >= -0.9)   return 0.948634;
      if(eta < -0.3 && eta >= -0.6)   return 0.977768;
      if(eta < 0 && eta >= -0.3)   return 0.958003;
      if(eta < 0.3 && eta >= 0)   return 0.960684;
      if(eta < 0.6 && eta >= 0.3)   return 0.972922;
      if(eta < 0.9 && eta >= 0.6)   return 0.94684;
      if(eta < 1.2 && eta >= 0.9)   return 0.952979;
      if(eta < 1.6 && eta >= 1.2)   return 0.935047;
      if(eta < 2.1 && eta >= 1.6)   return 0.945558;
      if(eta < 2.4 && eta >= 2.1)   return 0.93843;
   }
   return 1;
}

double eff_data_L3Mu12_PFIsoTight_pp(double eta, int idx)
{
   if(idx == 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 0.929084;
      if(eta < -1.6 && eta >= -2.1)   return 0.911438;
      if(eta < -1.2 && eta >= -1.6)   return 0.955168;
      if(eta < -0.9 && eta >= -1.2)   return 0.956007;
      if(eta < -0.6 && eta >= -0.9)   return 0.94539;
      if(eta < -0.3 && eta >= -0.6)   return 0.97507;
      if(eta < 0 && eta >= -0.3)   return 0.955139;
      if(eta < 0.3 && eta >= 0)   return 0.957508;
      if(eta < 0.6 && eta >= 0.3)   return 0.970998;
      if(eta < 0.9 && eta >= 0.6)   return 0.944965;
      if(eta < 1.2 && eta >= 0.9)   return 0.949554;
      if(eta < 1.6 && eta >= 1.2)   return 0.932765;
      if(eta < 2.1 && eta >= 1.6)   return 0.942109;
      if(eta < 2.4 && eta >= 2.1)   return 0.937141;
   }
   if(idx < 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 0.924305;
      if(eta < -1.6 && eta >= -2.1)   return 0.90813;
      if(eta < -1.2 && eta >= -1.6)   return 0.952486;
      if(eta < -0.9 && eta >= -1.2)   return 0.952941;
      if(eta < -0.6 && eta >= -0.9)   return 0.936441;
      if(eta < -0.3 && eta >= -0.6)   return 0.973054;
      if(eta < 0 && eta >= -0.3)   return 0.955139;
      if(eta < 0.3 && eta >= 0)   return 0.954691;
      if(eta < 0.6 && eta >= 0.3)   return 0.968688;
      if(eta < 0.9 && eta >= 0.6)   return 0.941985;
      if(eta < 1.2 && eta >= 0.9)   return 0.946626;
      if(eta < 1.6 && eta >= 1.2)   return 0.929749;
      if(eta < 2.1 && eta >= 1.6)   return 0.939309;
      if(eta < 2.4 && eta >= 2.1)   return 0.932015;
   }
   if(idx > 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 0.933755;
      if(eta < -1.6 && eta >= -2.1)   return 0.914678;
      if(eta < -1.2 && eta >= -1.6)   return 0.957788;
      if(eta < -0.9 && eta >= -1.2)   return 0.958963;
      if(eta < -0.6 && eta >= -0.9)   return 0.954339;
      if(eta < -0.3 && eta >= -0.6)   return 0.976986;
      if(eta < 0 && eta >= -0.3)   return 0.957721;
      if(eta < 0.3 && eta >= 0)   return 0.960222;
      if(eta < 0.6 && eta >= 0.3)   return 0.973195;
      if(eta < 0.9 && eta >= 0.6)   return 0.947843;
      if(eta < 1.2 && eta >= 0.9)   return 0.952376;
      if(eta < 1.6 && eta >= 1.2)   return 0.935696;
      if(eta < 2.1 && eta >= 1.6)   return 0.944828;
      if(eta < 2.4 && eta >= 2.1)   return 0.941691;
   }
   return 1;
}

double eff_MC_TightID_pp(double eta, int idx)
{
   if(idx == 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 0.988299;
      if(eta < -1.6 && eta >= -2.1)   return 0.992805;
      if(eta < -1.2 && eta >= -1.6)   return 0.994822;
      if(eta < -0.9 && eta >= -1.2)   return 0.98663;
      if(eta < -0.6 && eta >= -0.9)   return 0.985149;
      if(eta < -0.3 && eta >= -0.6)   return 0.991761;
      if(eta < 0 && eta >= -0.3)   return 0.979427;
      if(eta < 0.3 && eta >= 0)   return 0.975762;
      if(eta < 0.6 && eta >= 0.3)   return 0.992292;
      if(eta < 0.9 && eta >= 0.6)   return 0.985643;
      if(eta < 1.2 && eta >= 0.9)   return 0.985535;
      if(eta < 1.6 && eta >= 1.2)   return 0.99375;
      if(eta < 2.1 && eta >= 1.6)   return 0.993319;
      if(eta < 2.4 && eta >= 2.1)   return 0.990195;
   }
   if(idx < 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 0.987902;
      if(eta < -1.6 && eta >= -2.1)   return 0.992587;
      if(eta < -1.2 && eta >= -1.6)   return 0.99461;
      if(eta < -0.9 && eta >= -1.2)   return 0.986302;
      if(eta < -0.6 && eta >= -0.9)   return 0.984783;
      if(eta < -0.3 && eta >= -0.6)   return 0.991491;
      if(eta < 0 && eta >= -0.3)   return 0.978939;
      if(eta < 0.3 && eta >= 0)   return 0.975762;
      if(eta < 0.6 && eta >= 0.3)   return 0.992292;
      if(eta < 0.9 && eta >= 0.6)   return 0.985311;
      if(eta < 1.2 && eta >= 0.9)   return 0.985192;
      if(eta < 1.6 && eta >= 1.2)   return 0.993057;
      if(eta < 2.1 && eta >= 1.6)   return 0.99311;
      if(eta < 2.4 && eta >= 2.1)   return 0.988484;
   }
   if(idx > 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 0.988687;
      if(eta < -1.6 && eta >= -2.1)   return 0.993019;
      if(eta < -1.2 && eta >= -1.6)   return 0.99503;
      if(eta < -0.9 && eta >= -1.2)   return 0.986958;
      if(eta < -0.6 && eta >= -0.9)   return 0.985512;
      if(eta < -0.3 && eta >= -0.6)   return 0.992026;
      if(eta < 0 && eta >= -0.3)   return 0.979871;
      if(eta < 0.3 && eta >= 0)   return 0.976211;
      if(eta < 0.6 && eta >= 0.3)   return 0.992557;
      if(eta < 0.9 && eta >= 0.6)   return 0.98597;
      if(eta < 1.2 && eta >= 0.9)   return 0.985873;
      if(eta < 1.6 && eta >= 1.2)   return 0.994443;
      if(eta < 2.1 && eta >= 1.6)   return 0.993525;
      if(eta < 2.4 && eta >= 2.1)   return 0.991906;
   }
   return 1;

}

double eff_MC_TightID_PFIsoTight_pp(double eta, int idx)
{
   if(idx == 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 0.972794;
      if(eta < -1.6 && eta >= -2.1)   return 0.976525;
      if(eta < -1.2 && eta >= -1.6)   return 0.977472;
      if(eta < -0.9 && eta >= -1.2)   return 0.965615;
      if(eta < -0.6 && eta >= -0.9)   return 0.964569;
      if(eta < -0.3 && eta >= -0.6)   return 0.970737;
      if(eta < 0 && eta >= -0.3)   return 0.959581;
      if(eta < 0.3 && eta >= 0)   return 0.954888;
      if(eta < 0.6 && eta >= 0.3)   return 0.971489;
      if(eta < 0.9 && eta >= 0.6)   return 0.965256;
      if(eta < 1.2 && eta >= 0.9)   return 0.965731;
      if(eta < 1.6 && eta >= 1.2)   return 0.975433;
      if(eta < 2.1 && eta >= 1.6)   return 0.976725;
      if(eta < 2.4 && eta >= 2.1)   return 0.975992;
   }
   if(idx < 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 0.97216;
      if(eta < -1.6 && eta >= -2.1)   return 0.976095;
      if(eta < -1.2 && eta >= -1.6)   return 0.976995;
      if(eta < -0.9 && eta >= -1.2)   return 0.964996;
      if(eta < -0.6 && eta >= -0.9)   return 0.963955;
      if(eta < -0.3 && eta >= -0.6)   return 0.970185;
      if(eta < 0 && eta >= -0.3)   return 0.958951;
      if(eta < 0.3 && eta >= 0)   return 0.954229;
      if(eta < 0.6 && eta >= 0.3)   return 0.970945;
      if(eta < 0.9 && eta >= 0.6)   return 0.96465;
      if(eta < 1.2 && eta >= 0.9)   return 0.965117;
      if(eta < 1.6 && eta >= 1.2)   return 0.974937;
      if(eta < 2.1 && eta >= 1.6)   return 0.976259;
      if(eta < 2.4 && eta >= 2.1)   return 0.975312;
   }
   if(idx > 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 0.973428;
      if(eta < -1.6 && eta >= -2.1)   return 0.976955;
      if(eta < -1.2 && eta >= -1.6)   return 0.977945;
      if(eta < -0.9 && eta >= -1.2)   return 0.966222;
      if(eta < -0.6 && eta >= -0.9)   return 0.96518;
      if(eta < -0.3 && eta >= -0.6)   return 0.971284;
      if(eta < 0 && eta >= -0.3)   return 0.960208;
      if(eta < 0.3 && eta >= 0)   return 0.955543;
      if(eta < 0.6 && eta >= 0.3)   return 0.972028;
      if(eta < 0.9 && eta >= 0.6)   return 0.965858;
      if(eta < 1.2 && eta >= 0.9)   return 0.96634;
      if(eta < 1.6 && eta >= 1.2)   return 0.975925;
      if(eta < 2.1 && eta >= 1.6)   return 0.977186;
      if(eta < 2.4 && eta >= 2.1)   return 0.976665;
   }
   return 1;
}

double eff_MC_L3Mu12_pp(double eta, int idx)
{
   if(idx == 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 0.925911;
      if(eta < -1.6 && eta >= -2.1)   return 0.930054;
      if(eta < -1.2 && eta >= -1.6)   return 0.970381;
      if(eta < -0.9 && eta >= -1.2)   return 0.978407;
      if(eta < -0.6 && eta >= -0.9)   return 0.961898;
      if(eta < -0.3 && eta >= -0.6)   return 0.9847;
      if(eta < 0 && eta >= -0.3)   return 0.967331;
      if(eta < 0.3 && eta >= 0)   return 0.967846;
      if(eta < 0.6 && eta >= 0.3)   return 0.983718;
      if(eta < 0.9 && eta >= 0.6)   return 0.966225;
      if(eta < 1.2 && eta >= 0.9)   return 0.979953;
      if(eta < 1.6 && eta >= 1.2)   return 0.961292;
      if(eta < 2.1 && eta >= 1.6)   return 0.957517;
      if(eta < 2.4 && eta >= 2.1)   return 0.934438;
   }
   if(idx < 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 0.924854;
      if(eta < -1.6 && eta >= -2.1)   return 0.929304;
      if(eta < -1.2 && eta >= -1.6)   return 0.9699;
      if(eta < -0.9 && eta >= -1.2)   return 0.977988;
      if(eta < -0.6 && eta >= -0.9)   return 0.961359;
      if(eta < -0.3 && eta >= -0.6)   return 0.983792;
      if(eta < 0 && eta >= -0.3)   return 0.966804;
      if(eta < 0.3 && eta >= 0)   return 0.9669;
      if(eta < 0.6 && eta >= 0.3)   return 0.983347;
      if(eta < 0.9 && eta >= 0.6)   return 0.965692;
      if(eta < 1.2 && eta >= 0.9)   return 0.978849;
      if(eta < 1.6 && eta >= 1.2)   return 0.960782;
      if(eta < 2.1 && eta >= 1.6)   return 0.956539;
      if(eta < 2.4 && eta >= 2.1)   return 0.933489;
   }
   if(idx > 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 0.926963;
      if(eta < -1.6 && eta >= -2.1)   return 0.93079;
      if(eta < -1.2 && eta >= -1.6)   return 0.970866;
      if(eta < -0.9 && eta >= -1.2)   return 0.978823;
      if(eta < -0.6 && eta >= -0.9)   return 0.962431;
      if(eta < -0.3 && eta >= -0.6)   return 0.985608;
      if(eta < 0 && eta >= -0.3)   return 0.967861;
      if(eta < 0.3 && eta >= 0)   return 0.968792;
      if(eta < 0.6 && eta >= 0.3)   return 0.984086;
      if(eta < 0.9 && eta >= 0.6)   return 0.966758;
      if(eta < 1.2 && eta >= 0.9)   return 0.981057;
      if(eta < 1.6 && eta >= 1.2)   return 0.961795;
      if(eta < 2.1 && eta >= 1.6)   return 0.958495;
      if(eta < 2.4 && eta >= 2.1)   return 0.935393;
   }
   return 1;
}

double eff_MC_L3Mu12_PFIsoTight_pp(double eta, int idx)
{
   if(idx == 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 0.92563;
      if(eta < -1.6 && eta >= -2.1)   return 0.929618;
      if(eta < -1.2 && eta >= -1.6)   return 0.970265;
      if(eta < -0.9 && eta >= -1.2)   return 0.978639;
      if(eta < -0.6 && eta >= -0.9)   return 0.962093;
      if(eta < -0.3 && eta >= -0.6)   return 0.984714;
      if(eta < 0 && eta >= -0.3)   return 0.967283;
      if(eta < 0.3 && eta >= 0)   return 0.967852;
      if(eta < 0.6 && eta >= 0.3)   return 0.983783;
      if(eta < 0.9 && eta >= 0.6)   return 0.96657;
      if(eta < 1.2 && eta >= 0.9)   return 0.979873;
      if(eta < 1.6 && eta >= 1.2)   return 0.961802;
      if(eta < 2.1 && eta >= 1.6)   return 0.957591;
      if(eta < 2.4 && eta >= 2.1)   return 0.934344;
   }
   if(idx < 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 0.924129;
      if(eta < -1.6 && eta >= -2.1)   return 0.928938;
      if(eta < -1.2 && eta >= -1.6)   return 0.969399;
      if(eta < -0.9 && eta >= -1.2)   return 0.978639;
      if(eta < -0.6 && eta >= -0.9)   return 0.961554;
      if(eta < -0.3 && eta >= -0.6)   return 0.984714;
      if(eta < 0 && eta >= -0.3)   return 0.967283;
      if(eta < 0.3 && eta >= 0)   return 0.967363;
      if(eta < 0.6 && eta >= 0.3)   return 0.983433;
      if(eta < 0.9 && eta >= 0.6)   return 0.96657;
      if(eta < 1.2 && eta >= 0.9)   return 0.979873;
      if(eta < 1.6 && eta >= 1.2)   return 0.96129;
      if(eta < 2.1 && eta >= 1.6)   return 0.957454;
      if(eta < 2.4 && eta >= 2.1)   return 0.934311;
   }
   if(idx > 0)
   {
      if(eta < -2.1 && eta >= -2.4)   return 0.927131;
      if(eta < -1.6 && eta >= -2.1)   return 0.930297;
      if(eta < -1.2 && eta >= -1.6)   return 0.971131;
      if(eta < -0.9 && eta >= -1.2)   return 0.979054;
      if(eta < -0.6 && eta >= -0.9)   return 0.962665;
      if(eta < -0.3 && eta >= -0.6)   return 0.985052;
      if(eta < 0 && eta >= -0.3)   return 0.967845;
      if(eta < 0.3 && eta >= 0)   return 0.968943;
      if(eta < 0.6 && eta >= 0.3)   return 0.984124;
      if(eta < 0.9 && eta >= 0.6)   return 0.967091;
      if(eta < 1.2 && eta >= 0.9)   return 0.980281;
      if(eta < 1.6 && eta >= 1.2)   return 0.962304;
      if(eta < 2.1 && eta >= 1.6)   return 0.957623;
      if(eta < 2.4 && eta >= 2.1)   return 0.934377;
   }
   return 1;
}

double tnp_weight_L3Mu12_double_pp(double eta1, double eta2, int idx)
{
   double Eff1Data = eff_data_L3Mu12_pp(eta1, idx);
   double Eff1MC   = eff_MC_L3Mu12_pp(eta1, idx);
   double Eff2Data = eff_data_L3Mu12_pp(eta2, idx);
   double Eff2MC   = eff_MC_L3Mu12_pp(eta2, idx);

   return (1 - (1 - Eff1Data) * (1 - Eff2Data) / (1 - (1 - Eff1MC) * (1 - Eff2MC)));
}

#endif //#ifndef tnp_weight_h
