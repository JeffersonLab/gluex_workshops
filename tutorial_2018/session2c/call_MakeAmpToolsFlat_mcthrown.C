#include "MakeAmpToolsFlat_mcthrown.C"
void call_MakeAmpToolsFlat_mcthrown ()
{
// issue the tree->Loop() from the command line.
//
  // gROOT->ProcessLine(".L MakeAmpToolsFlat_mcthrown.C");
  //gROOT->LoadMacro("MakeAmpToolsFlat_mcthrown.C");
 MakeAmpToolsFlat_mcthrown t;
 t.Loop();
}
