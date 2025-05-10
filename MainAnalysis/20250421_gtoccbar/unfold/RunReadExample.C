#include "./examples/RooUnfoldExample.cxx"
void RunReadExample(int i = 1)
{
  gSystem->Load("libRooUnfold.so");
  gROOT->ProcessLine(".L ./examples/RooUnfoldExample.cxx");
  RooUnfoldExample();



}
