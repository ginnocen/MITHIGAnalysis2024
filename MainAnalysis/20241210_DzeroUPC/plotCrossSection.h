#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm> // For std::find
#include <iomanip> // For std::fixed and std::setprecision

using namespace std;

/////////////////////////////////
// Struct to host the values in correctedYields.md
/////////////////////////////////
struct Point {
  double ptmin, ptmax, ymin, ymax;
  double effEvt, effD, rawYield, correctedYield;
  double effEvtError, effDError, rawYieldError, correctedYieldError;
  double numEvt, denEvt, numD, denD;

  void print() const {
    std::cout << "ptmin: " << ptmin << ", ptmax: " << ptmax
              << ", ymin: " << ymin << ", ymax: " << ymax << std::endl;
    std::cout << "effEvt: " << effEvt << " +/- " << effEvtError
              << ", effD: " << effD << " +/- " << effDError
              << ", raw yield: " << rawYield << " +/- " << rawYieldError
              << ", corrected yield: " << correctedYield << " +/- " << correctedYieldError << std::endl;
    std::cout << "Num #(Evt): " << numEvt << ", Den #(Evt): " << denEvt
              << ", Num #(D): " << numD << ", Den #(D): " << denD << std::endl;
  }
};

/////////////////////////////////
// Utility functions for reading md
/////////////////////////////////
// Function to trim leading and trailing spaces
std::string trim(const std::string& str) {
  size_t first = str.find_first_not_of(' ');
  if (first == std::string::npos) return ""; // No content

  size_t last = str.find_last_not_of(' ');
  return str.substr(first, (last - first + 1));
}

// Function to extract number from string with '+/-' sign
// Function to extract number and error from a string with '+/-' sign
void parseNumberAndError(const std::string& str, double &value, double &error) {
  std::regex rgx("([+-]?\\d*\\.?\\d+)\\s*\\+/-\\s*([+-]?\\d*\\.?\\d+)");
  std::smatch match;
  if (std::regex_search(str, match, rgx)) {
    value = std::stod(match.str(1));   // Extract the main value
    error = std::stod(match.str(2));   // Extract the error value
  } else {
    value = 0.0;
    error = 0.0;
  }
}

/////////////////////////////////
// Reading md into Point
/////////////////////////////////
void getPoint(string& mdInput, Point& p) {
  printf("read %s >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", mdInput.c_str());
  std::ifstream inputFile(mdInput.c_str());
  std::string line;

  // Skip header lines
  std::getline(inputFile, line);
  std::getline(inputFile, line);

  // Extract ptmin, ptmax, ymin, ymax
  std::getline(inputFile, line);
  // printf("read %s\n", line.c_str());
  std::stringstream ss1(line);
  std::string value;
  
  // Read the values separated by '|'
  std::getline(ss1, value, '|');
  std::getline(ss1, value, '|'); p.ptmin = std::stod(trim(value));
  std::getline(ss1, value, '|'); p.ptmax = std::stod(trim(value));
  std::getline(ss1, value, '|'); p.ymin = std::stod(trim(value));
  std::getline(ss1, value, '|'); p.ymax = std::stod(trim(value));


  // Extract effEvt, effD, rawYield, correctedYield
  std::getline(inputFile, line);
  std::getline(inputFile, line);
  // printf("read %s\n", line.c_str());
  std::stringstream ss2(line);
  std::getline(ss2, value, '|');
  std::getline(ss2, value, '|'); parseNumberAndError(trim(value), p.effEvt, p.effEvtError);
  std::getline(ss2, value, '|'); parseNumberAndError(trim(value), p.effD, p.effDError);
  std::getline(ss2, value, '|'); parseNumberAndError(trim(value), p.rawYield, p.rawYieldError);
  std::getline(ss2, value, '|'); parseNumberAndError(trim(value), p.correctedYield, p.correctedYieldError);

  // Extract numEvt, denEvt, numD, denD
  std::getline(inputFile, line);
  std::getline(inputFile, line);
  // printf("read %s\n", line.c_str());
  std::stringstream ss3(line);
  
  // Read the values separated by '|'
  std::getline(ss3, value, '|');
  std::getline(ss3, value, '|'); p.numEvt = std::stod(trim(value));
  std::getline(ss3, value, '|'); p.denEvt = std::stod(trim(value));
  std::getline(ss3, value, '|'); p.numD = std::stod(trim(value));
  std::getline(ss3, value, '|'); p.denD = std::stod(trim(value));

  // Output extracted data
  p.print();
}

vector<Point> getPointArr(float MinDzeroPT, float MaxDzeroPT, bool IsGammaN, 
													vector<string>& inputPoints)
{
  const int nPoints = inputPoints.size();
  std::vector<Point> PointsArr(nPoints);

  for (int i = 0; i < nPoints; ++i)
  {
    getPoint(inputPoints[i], PointsArr[i]);
    
    Point& p(PointsArr[i]);
    
    if (MinDzeroPT != p.ptmin || 
        MaxDzeroPT != p.ptmax ||
        IsGammaN != (inputPoints[i].find("IsGammaN1") != string::npos)) {
      printf("[Error] Unexpected usage: Concatenating different pt bins or inconsistent gammaN/Ngamma results together! (%d,%d,%o),(%d,%d,%o) Exiting ...\n",
        p.ptmin, p.ptmax, (inputPoints[i].find("IsGammaN1") != string::npos),
        MinDzeroPT, MaxDzeroPT, IsGammaN);
      exit(1);
    }
  }

  return PointsArr;
}

/////////////////////////////////
// Based on the Point array to derive the corresponded double array with some math manipulation
/////////////////////////////////
vector<double> getDoubleArr(vector<Point>& PointsArr, 
														const std::function<double(Point&)>& func)
{
  const int nPoints = PointsArr.size();
  std::vector<double> retArr(nPoints);

  for (int i = 0; i < nPoints; ++i)
  {
    Point& p(PointsArr[i]);
    retArr[i] = func(p);
  }

  return retArr;
}

/////////////////////////////////
// Reading the corrected yields results from the systematics directory into a double array, which will be the same (pt,y)-bin order as the central values (inputPoints)
/////////////////////////////////
vector<double> getAltCorrectedYieldArr(vector<string>& inputPoints,
                               string nominalRstDirName, string altRstDirName,
                               float MinDzeroPT, float MaxDzeroPT, bool IsGammaN)
{
  vector<string> _altPoints(inputPoints);
  for (std::string& _p : _altPoints) {
    size_t pos = _p.find(nominalRstDirName.c_str());
    if (pos != std::string::npos) {
      _p.replace(pos, nominalRstDirName.length(), altRstDirName.c_str());
    } else {
      printf("[Warning] %s not found. This component will give 0 uncertainty (use nominal values).\n", altRstDirName.c_str());
    }
  }

  std::vector<Point> _altPointsArr = getPointArr(MinDzeroPT, MaxDzeroPT, IsGammaN, _altPoints);

  std::vector<double> retArr = getDoubleArr(_altPointsArr,
                 [](Point& p) -> double { return p.correctedYield;} );
  
  return retArr;
}

/////////////////////////////////
// Utility functions for double array
/////////////////////////////////
void printArr(const std::vector<double>& vec,
              const std::string& delimiter = " ",
              const std::string& headMsg = "",
              const std::string& endMsg = "",
              int precision = 2)
{
  std::cout << headMsg; // Print the head string at the beginning
  if (precision >= 0)
  {
    std::cout << std::fixed << std::setprecision(precision);
  }

  for (size_t i = 0; i < vec.size(); ++i)
  {
    std::cout << vec[i];
    if (i < vec.size() - 1) std::cout << delimiter;
  }

  if (precision >= 0)
  {
    std::cout.unsetf(std::ios::fixed); // Reset fixed mode
  }
  std::cout << endMsg; // Print the end string after the elements
  std::cout << std::endl;
}


void printRatioArr(const std::vector<double>& vec1,
                   const std::vector<double>& vec2,
                   const std::string& delimiter = " ",
                   const std::string& headMsg = "",
                   const std::string& endMsg = "",
                   int precision = 2,
                   int columnWidth = 6)
{
  std::cout << headMsg; // Print the head string at the beginning
  if (precision >= 0)
  {
    std::cout << std::fixed << std::setprecision(precision);
  }

  for (size_t i = 0; i < vec1.size(); ++i)
  {
    std::cout << std::setw(columnWidth) << (vec1[i]/vec2[i])*100;
    if (i < vec1.size() - 1) std::cout << delimiter;
  }

  if (precision >= 0)
  {
    std::cout.unsetf(std::ios::fixed); // Reset fixed mode
  }
  std::cout << endMsg; // Print the end string after the elements
  std::cout << std::endl;
}

/* ************** Archived publication results ****************
 * [Todo] 1. To implement a class to host the cross-section curves (from previous publications or from MC predictions),
 *        2. This will initiate the curve instances, and set the style
 *        3. Users can just call the functions draw(), get() (,etc), to access those archived curves
 * ************************************************************ */
/////////////////////////////////
// Results (2-5 pT) in HIN-2024-003
/////////////////////////////////
double CMSHIN24003_pt2to5_yValues_gammaN[1]       = {0};
double CMSHIN24003_pt2to5_yErrors_gammaN[1]       = {1};
double CMSHIN24003_pt2to5_crossSection_gammaN[1]  = {1.07585};
double CMSHIN24003_pt2to5_crossStatUnc_gammaN[1]  = {0.10225};
double CMSHIN24003_pt2to5_crossSystUnc_gammaN[1]  = {0.29048};
TGraphErrors* CMSHIN24003_pt2to5_gammaN_stat = new TGraphErrors(1,
                             CMSHIN24003_pt2to5_yValues_gammaN, CMSHIN24003_pt2to5_crossSection_gammaN,
                             CMSHIN24003_pt2to5_yErrors_gammaN, CMSHIN24003_pt2to5_crossStatUnc_gammaN);
TGraphErrors* CMSHIN24003_pt2to5_gammaN_syst = new TGraphErrors(1,
                             CMSHIN24003_pt2to5_yValues_gammaN, CMSHIN24003_pt2to5_crossSection_gammaN,
                             CMSHIN24003_pt2to5_yErrors_gammaN, CMSHIN24003_pt2to5_crossSystUnc_gammaN);

double CMSHIN24003_pt2to5_yValues_Ngamma[1]       = {0};
double CMSHIN24003_pt2to5_yErrors_Ngamma[1]       = {1};
double CMSHIN24003_pt2to5_crossSection_Ngamma[1]  = {1.02555};
double CMSHIN24003_pt2to5_crossStatUnc_Ngamma[1]   = {0.100667};
double CMSHIN24003_pt2to5_crossSystUnc_Ngamma[1]    = {0.283257};
TGraphErrors* CMSHIN24003_pt2to5_Ngamma_stat = new TGraphErrors(1,
                             CMSHIN24003_pt2to5_yValues_Ngamma, CMSHIN24003_pt2to5_crossSection_Ngamma,
                             CMSHIN24003_pt2to5_yErrors_Ngamma, CMSHIN24003_pt2to5_crossStatUnc_Ngamma);
TGraphErrors* CMSHIN24003_pt2to5_Ngamma_syst = new TGraphErrors(1,
                             CMSHIN24003_pt2to5_yValues_Ngamma, CMSHIN24003_pt2to5_crossSection_Ngamma,
                             CMSHIN24003_pt2to5_yErrors_Ngamma, CMSHIN24003_pt2to5_crossSystUnc_Ngamma);

/////////////////////////////////
// Results (2-5 pT) in HIN-2025-002
/////////////////////////////////
double CMSHIN25002_pt2to5_yValues_gammaN[4]       = {-1.5, -0.5, 0.5 ,1.5};
double CMSHIN25002_pt2to5_yErrors_gammaN[4]       = {0.5, 0.5, 0.5, 0.5};
double CMSHIN25002_pt2to5_crossSection_gammaN[4]  = {1.812, 1.2586, 0.6666, 0.1865};
double CMSHIN25002_pt2to5_crossStatUnc_gammaN[4]  = {0.2854, 0.1397, 0.0933, 0.0795};
double CMSHIN25002_pt2to5_crossSystUnc_gammaN[4]  = {0.440474, 0.301792, 0.250459, 0.0842525};
TGraphErrors* CMSHIN25002_pt2to5_gammaN_stat = new TGraphErrors(4,
                             CMSHIN25002_pt2to5_yValues_gammaN, CMSHIN25002_pt2to5_crossSection_gammaN,
                             CMSHIN25002_pt2to5_yErrors_gammaN, CMSHIN25002_pt2to5_crossStatUnc_gammaN);
TGraphErrors* CMSHIN25002_pt2to5_gammaN_syst = new TGraphErrors(4,
                             CMSHIN25002_pt2to5_yValues_gammaN, CMSHIN25002_pt2to5_crossSection_gammaN,
                             CMSHIN25002_pt2to5_yErrors_gammaN, CMSHIN25002_pt2to5_crossSystUnc_gammaN);

double CMSHIN25002_pt2to5_yValues_Ngamma[4]       = {-1.5, -0.5, 0.5 ,1.5};
double CMSHIN25002_pt2to5_yErrors_Ngamma[4]       = {0.5, 0.5, 0.5, 0.5};
double CMSHIN25002_pt2to5_crossSection_Ngamma[4]  = {0.3222, 0.6175, 1.3696, 1.294};
double CMSHIN25002_pt2to5_crossStatUnc_Ngamma[4]  = {0.0887, 0.0899, 0.1594, 0.2699};
double CMSHIN25002_pt2to5_crossSystUnc_Ngamma[4]  = {0.132515, 0.172208, 0.387299, 0.504916};
TGraphErrors* CMSHIN25002_pt2to5_Ngamma_stat = new TGraphErrors(4,
                             CMSHIN25002_pt2to5_yValues_Ngamma, CMSHIN25002_pt2to5_crossSection_Ngamma,
                             CMSHIN25002_pt2to5_yErrors_Ngamma, CMSHIN25002_pt2to5_crossStatUnc_Ngamma);
TGraphErrors* CMSHIN25002_pt2to5_Ngamma_syst = new TGraphErrors(4,
                             CMSHIN25002_pt2to5_yValues_Ngamma, CMSHIN25002_pt2to5_crossSection_Ngamma,
                             CMSHIN25002_pt2to5_yErrors_Ngamma, CMSHIN25002_pt2to5_crossSystUnc_Ngamma);

// We will likely not use this in the short-term future.
// If we have a class, than we can instantiate these results fairly easily (without duplicating the code)
void drawPubCurves_CMSHIN24003(bool IsGammaN, TLegend* leg=nullptr)
{
  TGraphErrors* gr_stat = (IsGammaN)? CMSHIN24003_pt2to5_gammaN_stat: CMSHIN24003_pt2to5_Ngamma_stat;
  TGraphErrors* gr_syst = (IsGammaN)? CMSHIN24003_pt2to5_gammaN_syst: CMSHIN24003_pt2to5_Ngamma_syst;

  gr_stat->SetMarkerStyle(20);
  gr_stat->SetMarkerSize(1.2);
  gr_stat->SetLineColor(kBlack);
  gr_stat->SetMarkerColor(kBlack);
  gr_stat->SetLineWidth(2);

  gr_syst->SetFillColorAlpha(kBlack,0.3);

  gr_stat->Draw("P E1 SAME");
  gr_syst->Draw("2 SAME");

  if (leg) leg->AddEntry(gr_stat, "HIN-24-003", "P");
}

void drawPubCurves(bool IsGammaN, TLegend* leg=nullptr)
{
  TGraphErrors* gr_stat = (IsGammaN)? CMSHIN25002_pt2to5_gammaN_stat: CMSHIN25002_pt2to5_Ngamma_stat;
  TGraphErrors* gr_syst = (IsGammaN)? CMSHIN25002_pt2to5_gammaN_syst: CMSHIN25002_pt2to5_Ngamma_syst;

  gr_stat->SetMarkerStyle(20);
  gr_stat->SetMarkerSize(1.2);
  gr_stat->SetLineColor(kBlack);
  gr_stat->SetMarkerColor(kBlack);
  gr_stat->SetLineWidth(2);

  gr_syst->SetFillColorAlpha(kBlack,0.3);

  gr_stat->Draw("P E1 SAME");
  gr_syst->Draw("2 SAME");

  if (leg) leg->AddEntry(gr_stat, "HIN-25-002", "P");
}


