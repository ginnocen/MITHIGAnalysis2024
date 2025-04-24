#include "./examples/RooUnfoldUnfold.cxx"
void RunReadUnfold(int i = 1)
{
  gSystem->Load("libRooUnfold.so");
  gROOT->ProcessLine(".L ./examples/RooUnfoldUnfold.cxx");
  RooUnfoldUnfold();

}
