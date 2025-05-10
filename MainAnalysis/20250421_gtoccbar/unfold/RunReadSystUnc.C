#include "./examples/RooUnfoldSystUnc.cxx"
void RunReadSystUnc(int i = 1)
{
  gSystem->Load("libRooUnfold.so");
  gROOT->ProcessLine(".L ./examples/RooUnfoldSystUnc.cxx");
  RooUnfoldSystUnc();
}
