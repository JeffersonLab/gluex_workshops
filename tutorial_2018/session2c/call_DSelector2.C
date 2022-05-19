void call_DSelector2 (TString file)
{
// issue the tree->Process, so that it can be run from the command line
//
cout << "call_DSelector2: file=" << file << endl;
gROOT->LoadMacro("$ROOT_ANALYSIS_HOME/scripts/Load_DSelector.C");
pippimmisspb208__B2_Tree->Process("DSelector_Z2pi_trees2.C+");
}
