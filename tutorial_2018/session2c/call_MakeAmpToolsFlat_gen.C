#include "MakeAmpToolsFlat_gen.C"
void call_MakeAmpToolsFlat_gen ()
{
// issue the tree->Loop() from the command line.
//
  // gROOT->ProcessLine(".L MakeAmpToolsFlat_gen.C");
  //gROOT->LoadMacro("MakeAmpToolsFlat_gen.C");
 MakeAmpToolsFlat_gen t;
 t.Loop();
}
