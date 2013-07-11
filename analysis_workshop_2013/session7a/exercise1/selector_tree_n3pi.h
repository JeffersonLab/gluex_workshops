//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Jul  5 08:17:10 2013 by ROOT version 5.32/04
// from TTree n3pi_Tree/n3pi_Tree
// found on file: /net/hisrv0001/home/jrsteven/scratch/2013-7-AnaWorkshop/treeFiles/set5/tree_n3pi.root
//////////////////////////////////////////////////////////

#include <iostream>
using namespace std;

#ifndef selector_tree_n3pi_h
#define selector_tree_n3pi_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TSystem.h>

// Header files for TMVA
#include <TMVA/Tools.h>
#include <TMVA/Reader.h>
#include <TMVA/MethodCuts.h>

// Header file for the classes stored in the TTree if any.
#include <TClonesArray.h>
#include <TLorentzVector.h>

// Fixed size dimensions of array or collections stored in the TTree if any.

class selector_tree_n3pi : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // options for the processor
   Bool_t FriendOutput;
   Bool_t EvalTMVA; 
   Bool_t bggenData; 
   Float_t user_BdtCut;

   // output friend tree file for TMVA training
   TFile *outFile;
   TTree *outTree;
   Int_t nSignal; //identified signal counter

   // variables for output friend tree
   Bool_t trueSignal;
   Float_t Unused__Max_Proton_FOM, Unused__Max_KPlus_FOM, Unused__Max_KMinus_FOM;
   Float_t Measured__MissingMass;

   // for TMVA reader
   TFile *outBdtFile;
   TTree *outBdtTree;
   Float_t BDT;
   Float_t PiPlus1__Timing_FOM, PiPlus1__DCdEdx_FOM;
   Float_t PiPlus2__Timing_FOM, PiPlus2__DCdEdx_FOM;
   Float_t PiMinus__Timing_FOM, PiMinus__DCdEdx_FOM;
   Float_t PiPlus1__NDF_Tracking_Alt, PiPlus2__NDF_Tracking_Alt, PiMinus__NDF_Tracking_Alt;
   Float_t FOM_KinFit, PV_r, MissingNeutron_PT;
   TMVA::Reader *tmvaReader;

   // fix at some large number since building with different trees gives different array length
   enum { NumThrownMax = 500 };
   enum { NumUnusedMax = 500 };

   // Declaration of leaf types
   UInt_t          RunNumber;
   UInt_t          EventNumber;
   Double_t        RFTime_Thrown;
   Double_t        RFTime_Measured;
   Double_t        ChiSq_KinFit;
   UInt_t          NDF_KinFit;
   Double_t        RFTime_KinFit;
   ULong64_t       NumPIDThrown_FinalState;
   ULong64_t       PIDThrown_Decaying;
   Double_t        MCWeight;
   UInt_t          NumThrown;
   Int_t           Thrown__ParentID[NumThrownMax];   //[NumThrown]
   Int_t           Thrown__PID[NumThrownMax];   //[NumThrown]
   Int_t           Thrown__MatchID[NumThrownMax];   //[NumThrown]
   TClonesArray    *Thrown__X4_Thrown;
   TClonesArray    *Thrown__P4_Thrown;
   Int_t           BeamPhoton__ObjectID;
   TLorentzVector  *BeamPhoton__X4_Measured;
   TLorentzVector  *BeamPhoton__P4_Measured;
   TLorentzVector  *BeamPhoton__X4_KinFit;
   TLorentzVector  *BeamPhoton__P4_KinFit;
   Double_t        TargetProton__Mass;
   Int_t           PiPlus1__ObjectID;
   Int_t           PiPlus1__MatchID;
   TLorentzVector  *PiPlus1__X4_Measured;
   TLorentzVector  *PiPlus1__P4_Measured;
   TLorentzVector  *PiPlus1__X4_KinFit;
   TLorentzVector  *PiPlus1__P4_KinFit;
   Double_t        PiPlus1__PathLength;
   Double_t        PiPlus1__ChiSq_Timing_Measured;
   Double_t        PiPlus1__ChiSq_Timing_KinFit;
   UInt_t          PiPlus1__NDF_Timing;
   Double_t        PiPlus1__ChiSq_Tracking;
   UInt_t          PiPlus1__NDF_Tracking;
   Double_t        PiPlus1__ChiSq_DCdEdx;
   UInt_t          PiPlus1__NDF_DCdEdx;
   Double_t        PiPlus1__dEdx_CDC;
   Double_t        PiPlus1__dEdx_FDC;
   Double_t        PiPlus1__dEdx_TOF;
   Double_t        PiPlus1__dEdx_ST;
   Double_t        PiPlus1__Energy_BCAL;
   Double_t        PiPlus1__Energy_FCAL;
   Int_t           PiPlus2__ObjectID;
   Int_t           PiPlus2__MatchID;
   TLorentzVector  *PiPlus2__X4_Measured;
   TLorentzVector  *PiPlus2__P4_Measured;
   TLorentzVector  *PiPlus2__X4_KinFit;
   TLorentzVector  *PiPlus2__P4_KinFit;
   Double_t        PiPlus2__PathLength;
   Double_t        PiPlus2__ChiSq_Timing_Measured;
   Double_t        PiPlus2__ChiSq_Timing_KinFit;
   UInt_t          PiPlus2__NDF_Timing;
   Double_t        PiPlus2__ChiSq_Tracking;
   UInt_t          PiPlus2__NDF_Tracking;
   Double_t        PiPlus2__ChiSq_DCdEdx;
   UInt_t          PiPlus2__NDF_DCdEdx;
   Double_t        PiPlus2__dEdx_CDC;
   Double_t        PiPlus2__dEdx_FDC;
   Double_t        PiPlus2__dEdx_TOF;
   Double_t        PiPlus2__dEdx_ST;
   Double_t        PiPlus2__Energy_BCAL;
   Double_t        PiPlus2__Energy_FCAL;
   Int_t           PiMinus__ObjectID;
   Int_t           PiMinus__MatchID;
   TLorentzVector  *PiMinus__X4_Measured;
   TLorentzVector  *PiMinus__P4_Measured;
   TLorentzVector  *PiMinus__X4_KinFit;
   TLorentzVector  *PiMinus__P4_KinFit;
   Double_t        PiMinus__PathLength;
   Double_t        PiMinus__ChiSq_Timing_Measured;
   Double_t        PiMinus__ChiSq_Timing_KinFit;
   UInt_t          PiMinus__NDF_Timing;
   Double_t        PiMinus__ChiSq_Tracking;
   UInt_t          PiMinus__NDF_Tracking;
   Double_t        PiMinus__ChiSq_DCdEdx;
   UInt_t          PiMinus__NDF_DCdEdx;
   Double_t        PiMinus__dEdx_CDC;
   Double_t        PiMinus__dEdx_FDC;
   Double_t        PiMinus__dEdx_TOF;
   Double_t        PiMinus__dEdx_ST;
   Double_t        PiMinus__Energy_BCAL;
   Double_t        PiMinus__Energy_FCAL;
   Double_t        MissingNeutron__Mass;
   TLorentzVector  *MissingNeutron__X4;
   TLorentzVector  *MissingNeutron__P4;
   UInt_t          NumUnused;
   Int_t           Unused__ObjectID[NumUnusedMax];   //[NumUnused]
   Int_t           Unused__PID[NumUnusedMax];   //[NumUnused]
   Int_t           Unused__MatchID[NumUnusedMax];   //[NumUnused]
   TClonesArray    *Unused__X4_Measured;
   TClonesArray    *Unused__P4_Measured;
   Double_t        Unused__PathLength[NumUnusedMax];   //[NumUnused]
   Double_t        Unused__ChiSq_Tracking[NumUnusedMax];   //[NumUnused]
   UInt_t          Unused__NDF_Tracking[NumUnusedMax];   //[NumUnused]
   Double_t        Unused__ChiSq_Timing[NumUnusedMax];   //[NumUnused]
   UInt_t          Unused__NDF_Timing[NumUnusedMax];   //[NumUnused]
   Double_t        Unused__ChiSq_DCdEdx[NumUnusedMax];   //[NumUnused]
   UInt_t          Unused__NDF_DCdEdx[NumUnusedMax];   //[NumUnused]
   Double_t        Unused__dEdx_CDC[NumUnusedMax];   //[NumUnused]
   Double_t        Unused__dEdx_FDC[NumUnusedMax];   //[NumUnused]
   Double_t        Unused__dEdx_TOF[NumUnusedMax];   //[NumUnused]
   Double_t        Unused__dEdx_ST[NumUnusedMax];   //[NumUnused]
   Double_t        Unused__Energy_BCAL[NumUnusedMax];   //[NumUnused]
   Double_t        Unused__Energy_FCAL[NumUnusedMax];   //[NumUnused]

   // List of branches
   TBranch        *b_RunNumber;   //!
   TBranch        *b_EventNumber;   //!
   TBranch        *b_RFTime_Thrown;   //!
   TBranch        *b_RFTime_Measured;   //!
   TBranch        *b_ChiSq_KinFit;   //!
   TBranch        *b_NDF_KinFit;   //!
   TBranch        *b_RFTime_KinFit;   //!
   TBranch        *b_NumPIDThrown_FinalState;   //!
   TBranch        *b_PIDThrown_Decaying;   //!
   TBranch        *b_MCWeight;   //!
   TBranch        *b_NumThrown;   //!
   TBranch        *b_Thrown__ParentID;   //!
   TBranch        *b_Thrown__PID;   //!
   TBranch        *b_Thrown__MatchID;   //!
   TBranch        *b_Thrown__X4_Thrown;   //!
   TBranch        *b_Thrown__P4_Thrown;   //!
   TBranch        *b_BeamPhoton__ObjectID;   //!
   TBranch        *b_BeamPhoton__X4_Measured;   //!
   TBranch        *b_BeamPhoton__P4_Measured;   //!
   TBranch        *b_BeamPhoton__X4_KinFit;   //!
   TBranch        *b_BeamPhoton__P4_KinFit;   //!
   TBranch        *b_TargetProton__Mass;   //!
   TBranch        *b_PiPlus1__ObjectID;   //!
   TBranch        *b_PiPlus1__MatchID;   //!
   TBranch        *b_PiPlus1__X4_Measured;   //!
   TBranch        *b_PiPlus1__P4_Measured;   //!
   TBranch        *b_PiPlus1__X4_KinFit;   //!
   TBranch        *b_PiPlus1__P4_KinFit;   //!
   TBranch        *b_PiPlus1__PathLength;   //!
   TBranch        *b_PiPlus1__ChiSq_Timing_Measured;   //!
   TBranch        *b_PiPlus1__ChiSq_Timing_KinFit;   //!
   TBranch        *b_PiPlus1__NDF_Timing;   //!
   TBranch        *b_PiPlus1__ChiSq_Tracking;   //!
   TBranch        *b_PiPlus1__NDF_Tracking;   //!
   TBranch        *b_PiPlus1__ChiSq_DCdEdx;   //!
   TBranch        *b_PiPlus1__NDF_DCdEdx;   //!
   TBranch        *b_PiPlus1__dEdx_CDC;   //!
   TBranch        *b_PiPlus1__dEdx_FDC;   //!
   TBranch        *b_PiPlus1__dEdx_TOF;   //!
   TBranch        *b_PiPlus1__dEdx_ST;   //!
   TBranch        *b_PiPlus1__Energy_BCAL;   //!
   TBranch        *b_PiPlus1__Energy_FCAL;   //!
   TBranch        *b_PiPlus2__ObjectID;   //!
   TBranch        *b_PiPlus2__MatchID;   //!
   TBranch        *b_PiPlus2__X4_Measured;   //!
   TBranch        *b_PiPlus2__P4_Measured;   //!
   TBranch        *b_PiPlus2__X4_KinFit;   //!
   TBranch        *b_PiPlus2__P4_KinFit;   //!
   TBranch        *b_PiPlus2__PathLength;   //!
   TBranch        *b_PiPlus2__ChiSq_Timing_Measured;   //!
   TBranch        *b_PiPlus2__ChiSq_Timing_KinFit;   //!
   TBranch        *b_PiPlus2__NDF_Timing;   //!
   TBranch        *b_PiPlus2__ChiSq_Tracking;   //!
   TBranch        *b_PiPlus2__NDF_Tracking;   //!
   TBranch        *b_PiPlus2__ChiSq_DCdEdx;   //!
   TBranch        *b_PiPlus2__NDF_DCdEdx;   //!
   TBranch        *b_PiPlus2__dEdx_CDC;   //!
   TBranch        *b_PiPlus2__dEdx_FDC;   //!
   TBranch        *b_PiPlus2__dEdx_TOF;   //!
   TBranch        *b_PiPlus2__dEdx_ST;   //!
   TBranch        *b_PiPlus2__Energy_BCAL;   //!
   TBranch        *b_PiPlus2__Energy_FCAL;   //!
   TBranch        *b_PiMinus__ObjectID;   //!
   TBranch        *b_PiMinus__MatchID;   //!
   TBranch        *b_PiMinus__X4_Measured;   //!
   TBranch        *b_PiMinus__P4_Measured;   //!
   TBranch        *b_PiMinus__X4_KinFit;   //!
   TBranch        *b_PiMinus__P4_KinFit;   //!
   TBranch        *b_PiMinus__PathLength;   //!
   TBranch        *b_PiMinus__ChiSq_Timing_Measured;   //!
   TBranch        *b_PiMinus__ChiSq_Timing_KinFit;   //!
   TBranch        *b_PiMinus__NDF_Timing;   //!
   TBranch        *b_PiMinus__ChiSq_Tracking;   //!
   TBranch        *b_PiMinus__NDF_Tracking;   //!
   TBranch        *b_PiMinus__ChiSq_DCdEdx;   //!
   TBranch        *b_PiMinus__NDF_DCdEdx;   //!
   TBranch        *b_PiMinus__dEdx_CDC;   //!
   TBranch        *b_PiMinus__dEdx_FDC;   //!
   TBranch        *b_PiMinus__dEdx_TOF;   //!
   TBranch        *b_PiMinus__dEdx_ST;   //!
   TBranch        *b_PiMinus__Energy_BCAL;   //!
   TBranch        *b_PiMinus__Energy_FCAL;   //!
   TBranch        *b_MissingNeutron__Mass;   //!
   TBranch        *b_MissingNeutron__X4;   //!
   TBranch        *b_MissingNeutron__P4;   //!
   TBranch        *b_NumUnused;   //!
   TBranch        *b_Unused__ObjectID;   //!
   TBranch        *b_Unused__PID;   //!
   TBranch        *b_Unused__MatchID;   //!
   TBranch        *b_Unused__X4_Measured;   //!
   TBranch        *b_Unused__P4_Measured;   //!
   TBranch        *b_Unused__PathLength;   //!
   TBranch        *b_Unused__ChiSq_Tracking;   //!
   TBranch        *b_Unused__NDF_Tracking;   //!
   TBranch        *b_Unused__ChiSq_Timing;   //!
   TBranch        *b_Unused__NDF_Timing;   //!
   TBranch        *b_Unused__ChiSq_DCdEdx;   //!
   TBranch        *b_Unused__NDF_DCdEdx;   //!
   TBranch        *b_Unused__dEdx_CDC;   //!
   TBranch        *b_Unused__dEdx_FDC;   //!
   TBranch        *b_Unused__dEdx_TOF;   //!
   TBranch        *b_Unused__dEdx_ST;   //!
   TBranch        *b_Unused__Energy_BCAL;   //!
   TBranch        *b_Unused__Energy_FCAL;   //!

   selector_tree_n3pi(TTree * /*tree*/ =0) : fChain(0) { }
   virtual ~selector_tree_n3pi() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    ProcessTMVA();
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(selector_tree_n3pi,0);
};

#endif

#ifdef selector_tree_n3pi_cxx
void selector_tree_n3pi::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   Thrown__X4_Thrown = 0;
   Thrown__P4_Thrown = 0;
   BeamPhoton__X4_Measured = 0;
   BeamPhoton__P4_Measured = 0;
   BeamPhoton__X4_KinFit = 0;
   BeamPhoton__P4_KinFit = 0;
   PiPlus1__X4_Measured = 0;
   PiPlus1__P4_Measured = 0;
   PiPlus1__X4_KinFit = 0;
   PiPlus1__P4_KinFit = 0;
   PiPlus2__X4_Measured = 0;
   PiPlus2__P4_Measured = 0;
   PiPlus2__X4_KinFit = 0;
   PiPlus2__P4_KinFit = 0;
   PiMinus__X4_Measured = 0;
   PiMinus__P4_Measured = 0;
   PiMinus__X4_KinFit = 0;
   PiMinus__P4_KinFit = 0;
   MissingNeutron__X4 = 0;
   MissingNeutron__P4 = 0;
   Unused__X4_Measured = 0;
   Unused__P4_Measured = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   // output friend tree for TMVA training
   if(FriendOutput){
     outFile = new TFile("tree_n3piFriend.root","recreate");
     outTree = new TTree("n3piFriend_Tree","");
     outTree->Branch("trueSignal",&trueSignal);
     outTree->Branch("Unused__Max_Proton_FOM",&Unused__Max_Proton_FOM);
     outTree->Branch("Unused__Max_KPlus_FOM",&Unused__Max_KPlus_FOM);
     outTree->Branch("Unused__Max_KMinus_FOM",&Unused__Max_KMinus_FOM);
     outTree->Branch("Measured__MissingMass",&Measured__MissingMass);
   }     
   
   // output tree after BDT cut for AmpTools fit
   if(EvalTMVA){
     outBdtFile = new TFile("tree_n3piBdtCut.root","recreate");
     outBdtTree = fChain->CloneTree(0);
   }

   fChain->SetBranchAddress("RunNumber", &RunNumber, &b_RunNumber);
   fChain->SetBranchAddress("EventNumber", &EventNumber, &b_EventNumber);
   fChain->SetBranchAddress("RFTime_Thrown", &RFTime_Thrown, &b_RFTime_Thrown);
   fChain->SetBranchAddress("RFTime_Measured", &RFTime_Measured, &b_RFTime_Measured);
   fChain->SetBranchAddress("ChiSq_KinFit", &ChiSq_KinFit, &b_ChiSq_KinFit);
   fChain->SetBranchAddress("NDF_KinFit", &NDF_KinFit, &b_NDF_KinFit);
   fChain->SetBranchAddress("RFTime_KinFit", &RFTime_KinFit, &b_RFTime_KinFit);
   fChain->SetBranchAddress("NumPIDThrown_FinalState", &NumPIDThrown_FinalState, &b_NumPIDThrown_FinalState);
   fChain->SetBranchAddress("PIDThrown_Decaying", &PIDThrown_Decaying, &b_PIDThrown_Decaying);
   fChain->SetBranchAddress("MCWeight", &MCWeight, &b_MCWeight);
   fChain->SetBranchAddress("NumThrown", &NumThrown, &b_NumThrown);
   fChain->SetBranchAddress("Thrown__ParentID", Thrown__ParentID, &b_Thrown__ParentID);
   fChain->SetBranchAddress("Thrown__PID", Thrown__PID, &b_Thrown__PID);
   fChain->SetBranchAddress("Thrown__MatchID", Thrown__MatchID, &b_Thrown__MatchID);
   fChain->SetBranchAddress("Thrown__X4_Thrown", &Thrown__X4_Thrown, &b_Thrown__X4_Thrown);
   fChain->SetBranchAddress("Thrown__P4_Thrown", &Thrown__P4_Thrown, &b_Thrown__P4_Thrown);
   fChain->SetBranchAddress("BeamPhoton__ObjectID", &BeamPhoton__ObjectID, &b_BeamPhoton__ObjectID);
   fChain->SetBranchAddress("BeamPhoton__X4_Measured", &BeamPhoton__X4_Measured, &b_BeamPhoton__X4_Measured);
   fChain->SetBranchAddress("BeamPhoton__P4_Measured", &BeamPhoton__P4_Measured, &b_BeamPhoton__P4_Measured);
   fChain->SetBranchAddress("BeamPhoton__X4_KinFit", &BeamPhoton__X4_KinFit, &b_BeamPhoton__X4_KinFit);
   fChain->SetBranchAddress("BeamPhoton__P4_KinFit", &BeamPhoton__P4_KinFit, &b_BeamPhoton__P4_KinFit);
   fChain->SetBranchAddress("TargetProton__Mass", &TargetProton__Mass, &b_TargetProton__Mass);
   fChain->SetBranchAddress("PiPlus1__ObjectID", &PiPlus1__ObjectID, &b_PiPlus1__ObjectID);
   fChain->SetBranchAddress("PiPlus1__MatchID", &PiPlus1__MatchID, &b_PiPlus1__MatchID);
   fChain->SetBranchAddress("PiPlus1__X4_Measured", &PiPlus1__X4_Measured, &b_PiPlus1__X4_Measured);
   fChain->SetBranchAddress("PiPlus1__P4_Measured", &PiPlus1__P4_Measured, &b_PiPlus1__P4_Measured);
   fChain->SetBranchAddress("PiPlus1__X4_KinFit", &PiPlus1__X4_KinFit, &b_PiPlus1__X4_KinFit);
   fChain->SetBranchAddress("PiPlus1__P4_KinFit", &PiPlus1__P4_KinFit, &b_PiPlus1__P4_KinFit);
   fChain->SetBranchAddress("PiPlus1__PathLength", &PiPlus1__PathLength, &b_PiPlus1__PathLength);
   fChain->SetBranchAddress("PiPlus1__ChiSq_Timing_Measured", &PiPlus1__ChiSq_Timing_Measured, &b_PiPlus1__ChiSq_Timing_Measured);
   fChain->SetBranchAddress("PiPlus1__ChiSq_Timing_KinFit", &PiPlus1__ChiSq_Timing_KinFit, &b_PiPlus1__ChiSq_Timing_KinFit);
   fChain->SetBranchAddress("PiPlus1__NDF_Timing", &PiPlus1__NDF_Timing, &b_PiPlus1__NDF_Timing);
   fChain->SetBranchAddress("PiPlus1__ChiSq_Tracking", &PiPlus1__ChiSq_Tracking, &b_PiPlus1__ChiSq_Tracking);
   fChain->SetBranchAddress("PiPlus1__NDF_Tracking", &PiPlus1__NDF_Tracking, &b_PiPlus1__NDF_Tracking);
   fChain->SetBranchAddress("PiPlus1__ChiSq_DCdEdx", &PiPlus1__ChiSq_DCdEdx, &b_PiPlus1__ChiSq_DCdEdx);
   fChain->SetBranchAddress("PiPlus1__NDF_DCdEdx", &PiPlus1__NDF_DCdEdx, &b_PiPlus1__NDF_DCdEdx);
   fChain->SetBranchAddress("PiPlus1__dEdx_CDC", &PiPlus1__dEdx_CDC, &b_PiPlus1__dEdx_CDC);
   fChain->SetBranchAddress("PiPlus1__dEdx_FDC", &PiPlus1__dEdx_FDC, &b_PiPlus1__dEdx_FDC);
   fChain->SetBranchAddress("PiPlus1__dEdx_TOF", &PiPlus1__dEdx_TOF, &b_PiPlus1__dEdx_TOF);
   fChain->SetBranchAddress("PiPlus1__dEdx_ST", &PiPlus1__dEdx_ST, &b_PiPlus1__dEdx_ST);
   fChain->SetBranchAddress("PiPlus1__Energy_BCAL", &PiPlus1__Energy_BCAL, &b_PiPlus1__Energy_BCAL);
   fChain->SetBranchAddress("PiPlus1__Energy_FCAL", &PiPlus1__Energy_FCAL, &b_PiPlus1__Energy_FCAL);
   fChain->SetBranchAddress("PiPlus2__ObjectID", &PiPlus2__ObjectID, &b_PiPlus2__ObjectID);
   fChain->SetBranchAddress("PiPlus2__MatchID", &PiPlus2__MatchID, &b_PiPlus2__MatchID);
   fChain->SetBranchAddress("PiPlus2__X4_Measured", &PiPlus2__X4_Measured, &b_PiPlus2__X4_Measured);
   fChain->SetBranchAddress("PiPlus2__P4_Measured", &PiPlus2__P4_Measured, &b_PiPlus2__P4_Measured);
   fChain->SetBranchAddress("PiPlus2__X4_KinFit", &PiPlus2__X4_KinFit, &b_PiPlus2__X4_KinFit);
   fChain->SetBranchAddress("PiPlus2__P4_KinFit", &PiPlus2__P4_KinFit, &b_PiPlus2__P4_KinFit);
   fChain->SetBranchAddress("PiPlus2__PathLength", &PiPlus2__PathLength, &b_PiPlus2__PathLength);
   fChain->SetBranchAddress("PiPlus2__ChiSq_Timing_Measured", &PiPlus2__ChiSq_Timing_Measured, &b_PiPlus2__ChiSq_Timing_Measured);
   fChain->SetBranchAddress("PiPlus2__ChiSq_Timing_KinFit", &PiPlus2__ChiSq_Timing_KinFit, &b_PiPlus2__ChiSq_Timing_KinFit);
   fChain->SetBranchAddress("PiPlus2__NDF_Timing", &PiPlus2__NDF_Timing, &b_PiPlus2__NDF_Timing);
   fChain->SetBranchAddress("PiPlus2__ChiSq_Tracking", &PiPlus2__ChiSq_Tracking, &b_PiPlus2__ChiSq_Tracking);
   fChain->SetBranchAddress("PiPlus2__NDF_Tracking", &PiPlus2__NDF_Tracking, &b_PiPlus2__NDF_Tracking);
   fChain->SetBranchAddress("PiPlus2__ChiSq_DCdEdx", &PiPlus2__ChiSq_DCdEdx, &b_PiPlus2__ChiSq_DCdEdx);
   fChain->SetBranchAddress("PiPlus2__NDF_DCdEdx", &PiPlus2__NDF_DCdEdx, &b_PiPlus2__NDF_DCdEdx);
   fChain->SetBranchAddress("PiPlus2__dEdx_CDC", &PiPlus2__dEdx_CDC, &b_PiPlus2__dEdx_CDC);
   fChain->SetBranchAddress("PiPlus2__dEdx_FDC", &PiPlus2__dEdx_FDC, &b_PiPlus2__dEdx_FDC);
   fChain->SetBranchAddress("PiPlus2__dEdx_TOF", &PiPlus2__dEdx_TOF, &b_PiPlus2__dEdx_TOF);
   fChain->SetBranchAddress("PiPlus2__dEdx_ST", &PiPlus2__dEdx_ST, &b_PiPlus2__dEdx_ST);
   fChain->SetBranchAddress("PiPlus2__Energy_BCAL", &PiPlus2__Energy_BCAL, &b_PiPlus2__Energy_BCAL);
   fChain->SetBranchAddress("PiPlus2__Energy_FCAL", &PiPlus2__Energy_FCAL, &b_PiPlus2__Energy_FCAL);
   fChain->SetBranchAddress("PiMinus__ObjectID", &PiMinus__ObjectID, &b_PiMinus__ObjectID);
   fChain->SetBranchAddress("PiMinus__MatchID", &PiMinus__MatchID, &b_PiMinus__MatchID);
   fChain->SetBranchAddress("PiMinus__X4_Measured", &PiMinus__X4_Measured, &b_PiMinus__X4_Measured);
   fChain->SetBranchAddress("PiMinus__P4_Measured", &PiMinus__P4_Measured, &b_PiMinus__P4_Measured);
   fChain->SetBranchAddress("PiMinus__X4_KinFit", &PiMinus__X4_KinFit, &b_PiMinus__X4_KinFit);
   fChain->SetBranchAddress("PiMinus__P4_KinFit", &PiMinus__P4_KinFit, &b_PiMinus__P4_KinFit);
   fChain->SetBranchAddress("PiMinus__PathLength", &PiMinus__PathLength, &b_PiMinus__PathLength);
   fChain->SetBranchAddress("PiMinus__ChiSq_Timing_Measured", &PiMinus__ChiSq_Timing_Measured, &b_PiMinus__ChiSq_Timing_Measured);
   fChain->SetBranchAddress("PiMinus__ChiSq_Timing_KinFit", &PiMinus__ChiSq_Timing_KinFit, &b_PiMinus__ChiSq_Timing_KinFit);
   fChain->SetBranchAddress("PiMinus__NDF_Timing", &PiMinus__NDF_Timing, &b_PiMinus__NDF_Timing);
   fChain->SetBranchAddress("PiMinus__ChiSq_Tracking", &PiMinus__ChiSq_Tracking, &b_PiMinus__ChiSq_Tracking);
   fChain->SetBranchAddress("PiMinus__NDF_Tracking", &PiMinus__NDF_Tracking, &b_PiMinus__NDF_Tracking);
   fChain->SetBranchAddress("PiMinus__ChiSq_DCdEdx", &PiMinus__ChiSq_DCdEdx, &b_PiMinus__ChiSq_DCdEdx);
   fChain->SetBranchAddress("PiMinus__NDF_DCdEdx", &PiMinus__NDF_DCdEdx, &b_PiMinus__NDF_DCdEdx);
   fChain->SetBranchAddress("PiMinus__dEdx_CDC", &PiMinus__dEdx_CDC, &b_PiMinus__dEdx_CDC);
   fChain->SetBranchAddress("PiMinus__dEdx_FDC", &PiMinus__dEdx_FDC, &b_PiMinus__dEdx_FDC);
   fChain->SetBranchAddress("PiMinus__dEdx_TOF", &PiMinus__dEdx_TOF, &b_PiMinus__dEdx_TOF);
   fChain->SetBranchAddress("PiMinus__dEdx_ST", &PiMinus__dEdx_ST, &b_PiMinus__dEdx_ST);
   fChain->SetBranchAddress("PiMinus__Energy_BCAL", &PiMinus__Energy_BCAL, &b_PiMinus__Energy_BCAL);
   fChain->SetBranchAddress("PiMinus__Energy_FCAL", &PiMinus__Energy_FCAL, &b_PiMinus__Energy_FCAL);
   fChain->SetBranchAddress("MissingNeutron__Mass", &MissingNeutron__Mass, &b_MissingNeutron__Mass);
   fChain->SetBranchAddress("MissingNeutron__X4", &MissingNeutron__X4, &b_MissingNeutron__X4);
   fChain->SetBranchAddress("MissingNeutron__P4", &MissingNeutron__P4, &b_MissingNeutron__P4);
   fChain->SetBranchAddress("NumUnused", &NumUnused, &b_NumUnused);
   fChain->SetBranchAddress("Unused__ObjectID", Unused__ObjectID, &b_Unused__ObjectID);
   fChain->SetBranchAddress("Unused__PID", Unused__PID, &b_Unused__PID);
   fChain->SetBranchAddress("Unused__MatchID", Unused__MatchID, &b_Unused__MatchID);
   fChain->SetBranchAddress("Unused__X4_Measured", &Unused__X4_Measured, &b_Unused__X4_Measured);
   fChain->SetBranchAddress("Unused__P4_Measured", &Unused__P4_Measured, &b_Unused__P4_Measured);
   fChain->SetBranchAddress("Unused__PathLength", Unused__PathLength, &b_Unused__PathLength);
   fChain->SetBranchAddress("Unused__ChiSq_Tracking", Unused__ChiSq_Tracking, &b_Unused__ChiSq_Tracking);
   fChain->SetBranchAddress("Unused__NDF_Tracking", Unused__NDF_Tracking, &b_Unused__NDF_Tracking);
   fChain->SetBranchAddress("Unused__ChiSq_Timing", Unused__ChiSq_Timing, &b_Unused__ChiSq_Timing);
   fChain->SetBranchAddress("Unused__NDF_Timing", Unused__NDF_Timing, &b_Unused__NDF_Timing);
   fChain->SetBranchAddress("Unused__ChiSq_DCdEdx", Unused__ChiSq_DCdEdx, &b_Unused__ChiSq_DCdEdx);
   fChain->SetBranchAddress("Unused__NDF_DCdEdx", Unused__NDF_DCdEdx, &b_Unused__NDF_DCdEdx);
   fChain->SetBranchAddress("Unused__dEdx_CDC", Unused__dEdx_CDC, &b_Unused__dEdx_CDC);
   fChain->SetBranchAddress("Unused__dEdx_FDC", Unused__dEdx_FDC, &b_Unused__dEdx_FDC);
   fChain->SetBranchAddress("Unused__dEdx_TOF", Unused__dEdx_TOF, &b_Unused__dEdx_TOF);
   fChain->SetBranchAddress("Unused__dEdx_ST", Unused__dEdx_ST, &b_Unused__dEdx_ST);
   fChain->SetBranchAddress("Unused__Energy_BCAL", Unused__Energy_BCAL, &b_Unused__Energy_BCAL);
   fChain->SetBranchAddress("Unused__Energy_FCAL", Unused__Energy_FCAL, &b_Unused__Energy_FCAL);
}

Bool_t selector_tree_n3pi::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef selector_tree_n3pi_cxx
