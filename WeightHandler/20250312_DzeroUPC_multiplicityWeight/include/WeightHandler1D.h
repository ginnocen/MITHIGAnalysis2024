#ifndef _WEIGHTHANDLER1D_H_
#define _WEIGHTHANDLER1D_H_

#include <string>
#include <iostream>

#include "TFile.h"
#include "TH1D.h"

#include "HistoHelper.h"

using namespace std;

/* + Description:
 *    1D weight handler to read and write the event weight, parametrized by multiplicity
 */

class WeightHandler1D
{
public:
  WeightHandler1D() : h_num(nullptr), h_den(nullptr), h_num_norm(nullptr), h_den_norm(nullptr), h_ratio(nullptr) {}

  WeightHandler1D(TH1D* in_h_num, TH1D* in_h_den,
                  bool  weightShapeOnly=true  /* to weight the distribution to the target histogram's SHAPE, or also the NORMALIZATION
                                               * - weightShapeOnly==true:  take the normalized histograms (h_num_norm and h_den_norm) to determine the ratio/weight
                                               * - weightShapeOnly==false: take the unnormalized histograms (h_num and h_den) to determine the ratio/weight
                                               */

                  )
  {
    h_num   = (TH1D*) in_h_num->Clone("h_num");
    h_den   = (TH1D*) in_h_den->Clone("h_den");
    h_num_norm   = (TH1D*) in_h_num->Clone("h_num_norm"); h_num_norm->Scale(1/h_num_norm->Integral());
    h_den_norm   = (TH1D*) in_h_den->Clone("h_den_norm"); h_den_norm->Scale(1/h_den_norm->Integral());

    normalizeHistoBinWidth(h_num);
    normalizeHistoBinWidth(h_den);
    normalizeHistoBinWidth(h_num_norm);
    normalizeHistoBinWidth(h_den_norm);

    if (weightShapeOnly)
    {
      h_ratio = (TH1D*) h_num_norm->Clone("h_ratio");
      h_ratio->Divide(h_den_norm);
    } else {
      h_ratio = (TH1D*) h_num->Clone("h_ratio");
      h_ratio->Divide(h_den);      
    }
  }

  ~WeightHandler1D() 
  {
    if (h_num) delete h_num;
    if (h_den) delete h_den;
    if (h_num_norm) delete h_num_norm;
    if (h_den_norm) delete h_den_norm;
    if (h_ratio) delete h_ratio;
    if (file) delete file;
  }



  bool LoadFromFile(string filename, 
                    string ratio_name="h_ratio",
                    string num_name="h_num",
                    string den_name="h_den");



  bool WriteToFile( string filename,
                    string ratio_name="h_ratio",
                    string num_name="h_num",
                    string den_name="h_den");

  double GetWeight(double _mult);

  TFile *file=NULL;
  TH1D* h_num;
  TH1D* h_den;
  TH1D* h_num_norm;
  TH1D* h_den_norm;
  TH1D* h_ratio;

};

bool WeightHandler1D::LoadFromFile( string filename, 
                                  string ratio_name,
                                  string num_name,
                                  string den_name) 
{
  file = new TFile(filename.c_str(),"READ");
  
  if (!file->IsOpen())
  {
    std::cerr << "Error: Could not open file " << filename << std::endl;
    return false;
  }

  h_ratio = (TH1D*) file->Get(ratio_name.c_str());
  h_num = (TH1D*) file->Get(num_name.c_str());
  h_den = (TH1D*) file->Get(den_name.c_str());
  h_num_norm = (TH1D*) file->Get(Form("%s_norm", num_name.c_str()));
  h_den_norm = (TH1D*) file->Get(Form("%s_norm", den_name.c_str()));

  if (!h_ratio || !h_num || !h_den || !h_num_norm || !h_den_norm)
  {
    std::cerr << "Error: Failed to retrieve histograms from file " << filename << std::endl;
    return false;
  }

  return true;
}


bool WeightHandler1D::WriteToFile(string filename,
                                string ratio_name,
                                string num_name,
                                string den_name) 
{
  file = new TFile(filename.c_str(),"RECREATE");

  if (!file->IsOpen())
  {
    std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
    return false;
  }

  if (h_ratio) h_ratio->Write(ratio_name.c_str());
  if (h_num) h_num->Write(num_name.c_str());
  if (h_den) h_den->Write(den_name.c_str());
  if (h_num_norm) h_num_norm->Write(Form("%s_norm", num_name.c_str()));
  if (h_den_norm) h_den_norm->Write(Form("%s_norm", den_name.c_str()));

  file->Close();

  return true;
}


double WeightHandler1D::GetWeight(double _mult)
{
  static int print_count = 0;

  if (print_count<=10 && !h_ratio)
  {
    std::cerr << "Error: h_ratio is not initialized." << std::endl;
    print_count++;
    return 0.0;
  }

  int binX = h_ratio->GetXaxis()->FindBin(_mult);

  return h_ratio->GetBinContent(binX);
}

#endif
