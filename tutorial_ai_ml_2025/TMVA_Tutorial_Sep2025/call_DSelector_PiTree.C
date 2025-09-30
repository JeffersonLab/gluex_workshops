void call_DSelector_PiTree (TString file)
{
  // issue the tree->Process, so that it can be run from the command line
  //
  cout << "call_DSelector: file=" << file << endl;
  gROOT->LoadMacro("$ROOT_ANALYSIS_HOME/scripts/Load_DSelector.C");

  pippim__B4_Tree->Process("DSelector_kpi_TMVA.C+");
  
}
