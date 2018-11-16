#define mcthrown_selector_cxx
// The class definition in mcthrown_selector.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("mcthrown_selector.C")
// root> T->Process("mcthrown_selector.C","some options")
// root> T->Process("mcthrown_selector.C+")
//

#include "mcthrown_selector.h"
#include <TH2.h>
#include <TStyle.h>

void mcthrown_selector::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   hEgamma = new TH1F("hEgamma", "hEgamma; E_{#gamma} (GeV)", 600, 6., 12.);
   ht_Egamma = new TH2F("ht_Egamma", "ht_Egamma; E_{#gamma}; -t (GeV^{2})", 900, 3, 12, 250, 0, 2.5);

}

void mcthrown_selector::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t mcthrown_selector::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either mcthrown_selector::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

   GetEntry(entry);

   TLorentzVector *locProton;
   TLorentzVector *locPiPlus;
   TLorentzVector *locPiMinus;
   TLorentzVector *locPi0;

   // P4 for thrown final state particles (check for particle by PDG type)
   for(uint loc_i = 0; loc_i < NumThrown; loc_i++) {
	   if(Thrown__PID[loc_i] == 2212) 
		   locProton = (TLorentzVector*)Thrown__P4->At(loc_i);
	   else if(Thrown__PID[loc_i] == 211)
		   locPiPlus = (TLorentzVector*)Thrown__P4->At(loc_i);
	   else if(Thrown__PID[loc_i] == -211)
		   locPiMinus = (TLorentzVector*)Thrown__P4->At(loc_i);
	   else if(Thrown__PID[loc_i] == 111)
		   locPi0 = (TLorentzVector*)Thrown__P4->At(loc_i);
   }

   // check that all thrown particles exist
   if(!locProton || !locPiPlus || !locPiMinus || !locPi0)
	   return kTRUE;

   // P4 for proton target
   TLorentzVector locTarget(0, 0, 0, 0.938);

   // Calculated kinematic quantities for efficiency denominator histograms
   double t = (*locProton - locTarget).M2();
   double locBeamEnergy = ThrownBeam__P4->E();
   hEgamma->Fill(locBeamEnergy);
   ht_Egamma->Fill(locBeamEnergy, -t);

   return kTRUE;
}

void mcthrown_selector::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void mcthrown_selector::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

   hEgamma->Write();
   ht_Egamma->Write();

}
