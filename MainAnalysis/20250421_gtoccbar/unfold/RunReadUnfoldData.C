#include "./examples/RooUnfoldData.cxx"
void RunReadUnfoldData(int i = 1)
{
  gSystem->Load("libRooUnfold.so");
  gROOT->ProcessLine(".L ./examples/RooUnfoldData.cxx");
  RooUnfoldData();

}
