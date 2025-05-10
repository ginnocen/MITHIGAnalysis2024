#include "./examples/RooUnfoldSystUncJESdecomp.cxx"
void RunReadSystUncJESdecomp(int i = 1)
{
  gSystem->Load("libRooUnfold.so");
  gROOT->ProcessLine(".L ./examples/RooUnfoldSystUncJESdecomp.cxx");
  RooUnfoldSystUncJESdecomp();
}
