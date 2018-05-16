#include <string>
#include <iostream>
#include "TROOT.h"

using namespace std;

void Make_Delta_Plots()
{
    gROOT->ProcessLine(".L MakeAndFitHists.C");
    gROOT->ProcessLine("MakeDeltappHists(\"PiDeltaDSelectorOut.root\",\"outputfile.root\")");
    gROOT->ProcessLine("FitInvHist(\"outputfile.root\",\"TagName\")");
}
