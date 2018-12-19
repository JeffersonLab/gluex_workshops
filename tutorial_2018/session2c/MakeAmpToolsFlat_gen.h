//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Apr 27 10:52:10 2018 by ROOT version 6.08/06
// from TTree kin/Kinematics
// found on file: tree_gen_2pi_primakoff_flat_100000.root
//////////////////////////////////////////////////////////

#ifndef MakeAmpToolsFlat_gen_h
#define MakeAmpToolsFlat_gen_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class MakeAmpToolsFlat_gen {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           NumFinalState;
   Float_t         E_FinalState[3];   //[NumFinalState]
   Float_t         Px_FinalState[3];   //[NumFinalState]
   Float_t         Py_FinalState[3];   //[NumFinalState]
   Float_t         Pz_FinalState[3];   //[NumFinalState]
   Float_t         E_Beam;
   Float_t         Px_Beam;
   Float_t         Py_Beam;
   Float_t         Pz_Beam;

   // List of branches
   TBranch        *b_NumFinalState;   //!
   TBranch        *b_E_FinalState;   //!
   TBranch        *b_Px_FinalState;   //!
   TBranch        *b_Py_FinalState;   //!
   TBranch        *b_Pz_FinalState;   //!
   TBranch        *b_E_Beam;   //!
   TBranch        *b_Px_Beam;   //!
   TBranch        *b_Py_Beam;   //!
   TBranch        *b_Pz_Beam;   //!

   MakeAmpToolsFlat_gen(TTree *tree=0);
   virtual ~MakeAmpToolsFlat_gen();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

   int m_nPart;
   int m_PID[3];
   float m_e[3];
   float m_px[3];
   float m_py[3];
   float m_pz[3];
   float m_eBeam;
   float m_pxBeam;
   float m_pyBeam;
   float m_pzBeam;
   float m_weight;
   float m_TargetMass;

   TTree *m_OutTree;
   TFile *outFile;

};

#endif

#ifdef MakeAmpToolsFlat_gen_cxx
MakeAmpToolsFlat_gen::MakeAmpToolsFlat_gen(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
      if (_file0) {
	tree = (TTree *) _file0->Get("pippimmisspb208_TreeFlat");    // require  input file if provided!
      }
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("tree_gen_2pi_primakoff_flat_100000.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("tree_gen_2pi_primakoff_flat_100000.root");
      }
      f->GetObject("kin",tree);

   }
   Init(tree);
}

MakeAmpToolsFlat_gen::~MakeAmpToolsFlat_gen()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t MakeAmpToolsFlat_gen::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t MakeAmpToolsFlat_gen::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void MakeAmpToolsFlat_gen::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("NumFinalState", &NumFinalState, &b_NumFinalState);
   fChain->SetBranchAddress("E_FinalState", E_FinalState, &b_E_FinalState);
   fChain->SetBranchAddress("Px_FinalState", Px_FinalState, &b_Px_FinalState);
   fChain->SetBranchAddress("Py_FinalState", Py_FinalState, &b_Py_FinalState);
   fChain->SetBranchAddress("Pz_FinalState", Pz_FinalState, &b_Pz_FinalState);
   fChain->SetBranchAddress("E_Beam", &E_Beam, &b_E_Beam);
   fChain->SetBranchAddress("Px_Beam", &Px_Beam, &b_Px_Beam);
   fChain->SetBranchAddress("Py_Beam", &Py_Beam, &b_Py_Beam);
   fChain->SetBranchAddress("Pz_Beam", &Pz_Beam, &b_Pz_Beam);
   Notify();
}

Bool_t MakeAmpToolsFlat_gen::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void MakeAmpToolsFlat_gen::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t MakeAmpToolsFlat_gen::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef MakeAmpToolsFlat_gen_cxx
