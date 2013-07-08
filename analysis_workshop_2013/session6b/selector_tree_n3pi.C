#define selector_tree_n3pi_cxx
// The class definition in selector_tree_n3pi.h has been generated automatically
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
// Root > T->Process("selector_tree_n3pi.C")
// Root > T->Process("selector_tree_n3pi.C","some options")
// Root > T->Process("selector_tree_n3pi.C+")
//

#include "selector_tree_n3pi.h"
#include <TH2.h>
#include <TStyle.h>


void selector_tree_n3pi::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   nSignal=0;
   user_BdtCut = -999.;

   // get user options
   TString option = GetOption();
   FriendOutput = option.Contains("FriendOutput");
   bggenData = option.Contains("bggenData");
   EvalTMVA = option.Contains("EvalTMVA");
   if(EvalTMVA) {
     Int_t firstEqual = option.First("=");
     user_BdtCut = atof(option(firstEqual+1,4).Data());
   }

   if(EvalTMVA) {
     // setup for TMVA evaluation
     tmvaReader = new TMVA::Reader( "!Color:!Silent" );
     
     // 3pi PID variables 
     tmvaReader->AddVariable("PiPlus1__timingFOM := (PiPlus1__ChiSq_Timing_KinFit==PiPlus1__ChiSq_Timing_KinFit) ? TMath::Prob(PiPlus1__ChiSq_Timing_KinFit,PiPlus1__NDF_Timing) : 0.",&PiPlus1__Timing_FOM);
     tmvaReader->AddVariable("PiPlus1__DCdEdx_FOM := TMath::Prob(PiPlus1__ChiSq_DCdEdx,PiPlus1__NDF_DCdEdx)",&PiPlus1__DCdEdx_FOM);
     tmvaReader->AddVariable("PiPlus1__NDF_Tracking",&PiPlus1__NDF_Tracking_Alt);
     tmvaReader->AddVariable("PiPlus2__Timing_FOM := (PiPlus2__ChiSq_Timing_KinFit==PiPlus2__ChiSq_Timing_KinFit) ? TMath::Prob(PiPlus2__ChiSq_Timing_KinFit,PiPlus2__NDF_Timing) : 0.",&PiPlus2__Timing_FOM);
     tmvaReader->AddVariable("PiPlus2__DCdEdx_FOM := TMath::Prob(PiPlus2__ChiSq_DCdEdx,PiPlus2__NDF_DCdEdx)",&PiPlus2__DCdEdx_FOM);
     tmvaReader->AddVariable("PiPlus2__NDF_Tracking",&PiPlus2__NDF_Tracking_Alt);
     tmvaReader->AddVariable("PiMinus__Timing_FOM := (PiMinus__ChiSq_Timing_KinFit==PiMinus__ChiSq_Timing_KinFit) ? TMath::Prob(PiMinus__ChiSq_Timing_KinFit,PiMinus__NDF_Timing) : 0.",&PiMinus__Timing_FOM);
     tmvaReader->AddVariable("PiMinus__DCdEdx_FOM := TMath::Prob(PiMinus__ChiSq_DCdEdx,PiMinus__NDF_DCdEdx)",&PiMinus__DCdEdx_FOM);
     tmvaReader->AddVariable("PiMinus__NDF_Tracking",&PiMinus__NDF_Tracking_Alt);
     
     // Kinematic fit variables
     tmvaReader->AddVariable("FOM_KinFit := TMath::Prob(ChiSq_KinFit,NDF_KinFit)",&FOM_KinFit);
     tmvaReader->AddVariable("PV_r := MissingNeutron__X4.Perp()",&PV_r);
     tmvaReader->AddVariable("MissingNeutron_PT := MissingNeutron__P4.Perp()",&MissingNeutron_PT);
     
     // Computed variables from friend tree
     tmvaReader->AddVariable("Unused__Energy_FCAL_Total",&Unused__Energy_FCAL_Total);
     tmvaReader->AddVariable("Unused__Energy_BCAL_Total",&Unused__Energy_BCAL_Total);
     tmvaReader->AddVariable("Unused__Max_Proton_FOM",&Unused__Max_Proton_FOM);
     tmvaReader->AddVariable("Unused__Max_KPlus_FOM",&Unused__Max_KPlus_FOM);
     tmvaReader->AddVariable("Unused__Max_KMinus_FOM" ,&Unused__Max_KMinus_FOM);
     
     // book the MVA methods
     TString methodName = "BDT method";
     TString weightFile = "weights/n3pi_BDT.weights.xml";
     tmvaReader->BookMVA( methodName, weightFile );
   }
   
}

void selector_tree_n3pi::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t selector_tree_n3pi::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either selector_tree_n3pi::GetEntry() or TBranch::GetEntry()
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
   if(entry==0) cout<<"--- Processing: "<<fChain->GetTree()->GetEntries()<<" total events"<<endl;
   if(entry%10000 == 0) cout<<"--- ... Processing event: "<<entry<<endl;
   
   // compute sums from unused tracks and calorimeter energies
   Unused__Energy_FCAL_Total=0.;
   Unused__Energy_BCAL_Total=0.;
   Unused__Momentum_Track_Total=0.;
   Unused__Max_Proton_FOM=0.;
   Unused__Max_KPlus_FOM=0.;
   Unused__Max_KMinus_FOM=0.;

   for(UInt_t i=0; i<NumUnused; i++){
     
     // select unused calorimeter energies not associated with a track
     if(Unused__Energy_FCAL[i]==Unused__Energy_FCAL[i] && Unused__NDF_Tracking[i]==0)
       Unused__Energy_FCAL_Total+=Unused__Energy_FCAL[i];
     if(Unused__Energy_BCAL[i]==Unused__Energy_BCAL[i] && Unused__NDF_Tracking[i]==0)
       Unused__Energy_BCAL_Total+=Unused__Energy_BCAL[i];
     // select unused tracks with some criteria for sum and max FOM
     if(Unused__NDF_Tracking[i] > 5) {
       TLorentzVector *unusedTrackP4 = (TLorentzVector*)Unused__P4_Measured->At(i);
       Unused__Momentum_Track_Total+=unusedTrackP4->Vect().Mag();
       
       // select highest CL for proton and kaon hypothesis
       float PID_FOM = TMath::Prob(Unused__ChiSq_DCdEdx[i]+Unused__ChiSq_Timing[i],Unused__NDF_DCdEdx[i]+Unused__NDF_Timing[i]);
       if(Unused__PID[i] == 2212 && PID_FOM > Unused__Max_Proton_FOM) Unused__Max_Proton_FOM=PID_FOM; 
       if(Unused__PID[i] == 321  && PID_FOM > Unused__Max_KPlus_FOM) Unused__Max_KPlus_FOM=PID_FOM; 
       if(Unused__PID[i] == -321 && PID_FOM > Unused__Max_KMinus_FOM) Unused__Max_KMinus_FOM=PID_FOM; 
       if(Unused__PID[i] == 211  && PID_FOM > Unused__Max_PiPlus_FOM) Unused__Max_PiPlus_FOM=PID_FOM; 
       if(Unused__PID[i] == -211 && PID_FOM > Unused__Max_PiMinus_FOM) Unused__Max_PiMinus_FOM=PID_FOM;
     }
   }

   // identify exclusive 3pi n channel from beam energy and matched 3pi tracks
   TLorentzVector thrownMissingP4(0,0,BeamPhoton__P4_Measured->Energy(),BeamPhoton__P4_Measured->Energy()+0.938); 

   // add thrown tracks to get missing mass of other thrown track
   Int_t MatchID[3] = {PiPlus1__MatchID, PiPlus2__MatchID, PiMinus__MatchID};
   for(int i=0; i<3; i++){
     if(MatchID[i]<0 || MatchID[i]>Thrown__P4_Thrown->GetSize()) continue;
     TLorentzVector *thrownP4 = (TLorentzVector*)Thrown__P4_Thrown->At(MatchID[i]);
     thrownMissingP4 -= TLorentzVector(thrownP4->Vect(),thrownP4->E());
   }
   
   // define signal as matched to proper thrown tracks and only other thrown track is a neutron
   trueSignal = false;
   if(PiPlus1__MatchID>=0 && PiPlus2__MatchID>=0 && PiMinus__MatchID>=0 && Thrown__PID[PiPlus1__MatchID]==211 && Thrown__PID[PiPlus2__MatchID]==211 && Thrown__PID[PiMinus__MatchID]==-211 && thrownMissingP4.M()<0.94 && thrownMissingP4.M()>0.936) {
     trueSignal = true;
     nSignal++;
   }

   if(EvalTMVA) ProcessTMVA();
   if(FriendOutput) outTree->Fill();

   return kTRUE;
}

void selector_tree_n3pi::ProcessTMVA()
{
   // initialize classifier value to nonsense
   BDT = -999.;
   
   // convert UInt_t to Float_t for TMVA reader...
   PiPlus1__NDF_Tracking_Alt = PiPlus1__NDF_Tracking;
   PiPlus2__NDF_Tracking_Alt = PiPlus2__NDF_Tracking;
   PiMinus__NDF_Tracking_Alt = PiMinus__NDF_Tracking;
   
   // same cuts as BDT training
   if(PiPlus1__NDF_Tracking<=5 || PiPlus2__NDF_Tracking<=5 || PiMinus__NDF_Tracking<=5) return;

   // remove signal events from bggen data since using separately generated signal data
   if(bggenData && trueSignal) return;

   // variables for reader
   PiPlus1__Timing_FOM = TMath::Prob(PiPlus1__ChiSq_Timing_KinFit,PiPlus1__NDF_Timing);
   PiPlus1__DCdEdx_FOM = TMath::Prob(PiPlus1__ChiSq_DCdEdx,PiPlus1__NDF_DCdEdx);
   //float PiPlus1_PID_FOM = TMath::Prob(PiPlus1__ChiSq_Timing_KinFit+PiPlus1__ChiSq_DCdEdx,PiPlus1__NDF_Timing+PiPlus1__NDF_DCdEdx);
   
   PiPlus2__Timing_FOM = TMath::Prob(PiPlus2__ChiSq_Timing_KinFit,PiPlus2__NDF_Timing);
   PiPlus2__DCdEdx_FOM = TMath::Prob(PiPlus2__ChiSq_DCdEdx,PiPlus2__NDF_DCdEdx);
   //float PiPlus2_PID_FOM = TMath::Prob(PiPlus2__ChiSq_Timing_KinFit+PiPlus2__ChiSq_DCdEdx,PiPlus2__NDF_Timing+PiPlus2__NDF_DCdEdx);
   
   PiMinus__Timing_FOM = TMath::Prob(PiMinus__ChiSq_Timing_KinFit,PiMinus__NDF_Timing);
   PiMinus__DCdEdx_FOM = TMath::Prob(PiMinus__ChiSq_DCdEdx,PiMinus__NDF_DCdEdx);
   //float PiMinus_PID_FOM = TMath::Prob(PiMinus__ChiSq_Timing_KinFit+PiMinus__ChiSq_DCdEdx,PiMinus__NDF_Timing+PiMinus__NDF_DCdEdx);
   
   FOM_KinFit = TMath::Prob(ChiSq_KinFit,NDF_KinFit);
   PV_r = MissingNeutron__X4->Perp();
   MissingNeutron_PT = MissingNeutron__P4->Perp();
   
   BDT = tmvaReader->EvaluateMVA("BDT method");

   //TLorentzVector threePi_p4 =  *PiPlus1__P4_KinFit + *PiPlus2__P4_KinFit + *PiMinus__P4_KinFit;
   //hBDT_threePiMass->Fill(threePi_p4.M(),BDT);

   if(BDT > user_BdtCut) outBdtTree->Fill();

}

void selector_tree_n3pi::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void selector_tree_n3pi::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

   // write friend tree
   if(FriendOutput){
     cout<<"found "<<nSignal<<" true 3pi n events"<<endl;
     outFile->Write();
     outFile->Close();
   }

   // write clone tree passing cuts
   if(EvalTMVA) {
     outBdtFile->Write();
     outBdtFile->Close();
   }

}
