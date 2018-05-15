#include <string>
#include <iostream>
#include "TROOT.h"
#include "TPad.h"
#include "TCanvas.h"

using namespace std;

void Make_Plots(string locMacroPath, string locSaveName)
{
	string locCommand = string(".x ") + locMacroPath;
	Long_t locResult = gROOT->ProcessLine(locCommand.c_str());
	gPad->GetCanvas()->SaveAs(locSaveName.c_str());
}

