#ifndef _WEIGHTHANDLER_H_
#define _WEIGHTHANDLER_H_

#include <string>
#include <iostream>

#include "TFile.h"
#include "TH2D.h"

using namespace std;

/* + Description:
 *    2D weight handler to read and write the D0 candidate weight
 */

class WeightHandler
{
public:
  WeightHandler() : h_num(nullptr), h_den(nullptr), h_ratio(nullptr) {}

  WeightHandler(TH2D* in_h_num, TH2D* in_h_den) 
  {
    h_num   = (TH2D*) in_h_num->Clone("h_num");
    h_den   = (TH2D*) in_h_den->Clone("h_den");
    h_ratio = (TH2D*) h_num->Clone("h_ratio");
    h_ratio->Divide(h_den);
  }

  ~WeightHandler() 
  {
    if (h_num) delete h_num;
    if (h_den) delete h_den;
    if (h_ratio) delete h_ratio;
  }



  bool LoadFromFile(string filename, 
                    string ratio_name="h_ratio",
                    string num_name="h_num",
                    string den_name="h_den");



  bool WriteToFile( string filename,
                    string ratio_name="h_ratio",
                    string num_name="h_num",
                    string den_name="h_den");

  double GetWeight(double _Dpt, double _Dy);

private:

  TH2D* h_num;
  TH2D* h_den;
  TH2D* h_ratio;

};

bool WeightHandler::LoadFromFile( string filename, 
                                  string ratio_name,
                                  string num_name,
                                  string den_name) 
{
  TFile file(filename.c_str(), "READ");
  
  if (!file.IsOpen())
  {
    std::cerr << "Error: Could not open file " << filename << std::endl;
    return false;
  }

  h_ratio = dynamic_cast<TH2D*>(file.Get(ratio_name.c_str()));
  h_num   = dynamic_cast<TH2D*>(file.Get(num_name.c_str()));
  h_den   = dynamic_cast<TH2D*>(file.Get(den_name.c_str()));

  if (!h_ratio || !h_num || !h_den)
  {
    std::cerr << "Error: Failed to retrieve histograms from file " << filename << std::endl;
    return false;
  }

  return true;
}


bool WeightHandler::WriteToFile(string filename,
                                string ratio_name,
                                string num_name,
                                string den_name) 
{
  TFile file(filename.c_str(), "RECREATE");

  if (!file.IsOpen())
  {
    std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
    return false;
  }

  if (h_ratio) h_ratio->Write(ratio_name.c_str());
  if (h_num) h_num->Write(num_name.c_str());
  if (h_den) h_den->Write(den_name.c_str());

  file.Close();

  return true;
}


double WeightHandler::GetWeight(double _Dpt, double _Dy)
{
  if (!h_ratio)
  {
    std::cerr << "Error: h_ratio is not initialized." << std::endl;
    return 0.0;
  }

  int binX = h_ratio->GetXaxis()->FindBin(_Dpt);
  int binY = h_ratio->GetYaxis()->FindBin(_Dy);

  return h_ratio->GetBinContent(binX, binY);
}

#endif
