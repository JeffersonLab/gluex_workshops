#include "MakeAmpToolsFlat.C"
void call_MakeAmpToolsFlat (Int_t foption=1)
{
// issue the tree->Loop() from the command line.
//
  // gROOT->ProcessLine(".L MakeAmpToolsFlat.C");
  //gROOT->LoadMacro("MakeAmpToolsFlat.C");
 MakeAmpToolsFlat t;
 t.Loop(foption);
}
