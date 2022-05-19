// macro to process analysis TTree with TSelector
#include <iostream> 

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
   
void runSelector(TString runNumber = "3", TString myPath = "/work/halld/home/jrsteven/tutorial_2022/bggen/merged/") 
{
  // Load DSelector library
  gROOT->ProcessLine(".x $(ROOT_ANALYSIS_HOME)/scripts/Load_DSelector.C");
  int Proof_Nthreads = 8;

  // process signal 
  TString sampleDir = myPath;
  cout<<"running selector on files in: "<<sampleDir.Data()<<endl;
  
  TChain *chain = new TChain("pi0eta__B4_M17_M7_Tree");
  TSystemDirectory dir(sampleDir, sampleDir);
  TList *files = dir.GetListOfFiles();
  if(files) {
	  TSystemFile *file;
	  TString fileName;
	  TIter next(files);
	  
	  // loop over files
	  while ((file=(TSystemFile*)next())) {
		  fileName = file->GetName();
		  if(fileName.Contains(runNumber)) {
			  cout<<fileName.Data()<<endl;
			  
			  // check if file corrupted
			  TFile f(sampleDir+fileName);
			  if(f.TestBit(TFile::kRecovered)) {
				  cout<<"file corrupted -> skipping"<<endl;
				  continue;
			  }
			  if(f.IsZombie()) {
				  cout<<"file is a Zombie -> skipping"<<endl;
				  continue;
			  }
			  
			  // add file to chain
			  chain->Add(sampleDir+fileName);
		  }
	  }

	  cout<<"total entries in TChain = "<<chain->GetEntries()<<endl;
	  DPROOFLiteManager::Process_Chain(chain, "DSelector_session1b.C+", Proof_Nthreads, Form("hist_etapi_%s.acc.root", runNumber.Data()));
  }

  return;
}
