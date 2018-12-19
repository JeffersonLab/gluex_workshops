//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Apr 26 14:51:04 2018 by ROOT version 6.08/06
// from TTree pippimmisspb208_TreeFlat/pippimmisspb208_TreeFlat
// found on file: treeFlat_DSelector_Z2pi_trees_sw1pw1000_TGT_signal_100000.root
//////////////////////////////////////////////////////////

#ifndef MakeAmpToolsFlat_h
#define MakeAmpToolsFlat_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "TLorentzVector.h"

class MakeAmpToolsFlat {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   UInt_t          run;
   ULong64_t       event;
   Float_t         weight;
   ULong64_t       numtruepid_final;
   ULong64_t       truepids_decay;
   Bool_t          is_truetop;
   Bool_t          is_truecombo;
   Bool_t          is_bdtcombo;
   Bool_t          rftime;
   Float_t         kin_chisq;
   UInt_t          kin_ndf;
   UInt_t          beam_beamid;
   Bool_t          beam_isgen;
   TLorentzVector  *beam_x4_meas;
   TLorentzVector  *beam_p4_meas;
   TLorentzVector  *beam_x4_kin;
   TLorentzVector  *beam_p4_kin;
   TLorentzVector  *beam_x4_true;
   TLorentzVector  *beam_p4_true;
   UInt_t          pip_trkid;
   TLorentzVector  *pip_x4_meas;
   TLorentzVector  *pip_p4_meas;
   TLorentzVector  *pip_x4_kin;
   TLorentzVector  *pip_p4_kin;
   Float_t         pip_true_fom;
   TLorentzVector  *pip_x4_true;
   TLorentzVector  *pip_p4_true;
   Float_t         pip_beta_time;
   Float_t         pip_chisq_time;
   UInt_t          pip_ndf_time;
   UInt_t          pip_ndf_trk;
   Float_t         pip_chisq_trk;
   UInt_t          pip_ndf_dedx;
   Float_t         pip_chisq_dedx;
   Float_t         pip_dedx_cdc;
   Float_t         pip_dedx_fdc;
   Float_t         pip_dedx_tof;
   Float_t         pip_dedx_st;
   Float_t         pip_ebcal;
   Float_t         pip_eprebcal;
   Float_t         pip_efcal;
   Float_t         pip_bcal_delphi;
   Float_t         pip_bcal_delz;
   Float_t         pip_fcal_doca;
   UInt_t          pim_trkid;
   TLorentzVector  *pim_x4_meas;
   TLorentzVector  *pim_p4_meas;
   TLorentzVector  *pim_x4_kin;
   TLorentzVector  *pim_p4_kin;
   Float_t         pim_true_fom;
   TLorentzVector  *pim_x4_true;
   TLorentzVector  *pim_p4_true;
   Float_t         pim_beta_time;
   Float_t         pim_chisq_time;
   UInt_t          pim_ndf_time;
   UInt_t          pim_ndf_trk;
   Float_t         pim_chisq_trk;
   UInt_t          pim_ndf_dedx;
   Float_t         pim_chisq_dedx;
   Float_t         pim_dedx_cdc;
   Float_t         pim_dedx_fdc;
   Float_t         pim_dedx_tof;
   Float_t         pim_dedx_st;
   Float_t         pim_ebcal;
   Float_t         pim_eprebcal;
   Float_t         pim_efcal;
   Float_t         pim_bcal_delphi;
   Float_t         pim_bcal_delz;
   Float_t         pim_fcal_doca;
   TLorentzVector  *misspb_p4_kin;
   Double_t        AccWeight;

   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_event;   //!
   TBranch        *b_weight;   //!
   TBranch        *b_numtruepid_final;   //!
   TBranch        *b_truepids_decay;   //!
   TBranch        *b_is_truetop;   //!
   TBranch        *b_is_truecombo;   //!
   TBranch        *b_is_bdtcombo;   //!
   TBranch        *b_rftime;   //!
   TBranch        *b_kin_chisq;   //!
   TBranch        *b_kin_ndf;   //!
   TBranch        *b_beam_beamid;   //!
   TBranch        *b_beam_isgen;   //!
   TBranch        *b_beam_x4_meas;   //!
   TBranch        *b_beam_p4_meas;   //!
   TBranch        *b_beam_x4_kin;   //!
   TBranch        *b_beam_p4_kin;   //!
   TBranch        *b_beam_x4_true;   //!
   TBranch        *b_beam_p4_true;   //!
   TBranch        *b_pip_trkid;   //!
   TBranch        *b_pip_x4_meas;   //!
   TBranch        *b_pip_p4_meas;   //!
   TBranch        *b_pip_x4_kin;   //!
   TBranch        *b_pip_p4_kin;   //!
   TBranch        *b_pip_true_fom;   //!
   TBranch        *b_pip_x4_true;   //!
   TBranch        *b_pip_p4_true;   //!
   TBranch        *b_pip_beta_time;   //!
   TBranch        *b_pip_chisq_time;   //!
   TBranch        *b_pip_ndf_time;   //!
   TBranch        *b_pip_ndf_trk;   //!
   TBranch        *b_pip_chisq_trk;   //!
   TBranch        *b_pip_ndf_dedx;   //!
   TBranch        *b_pip_chisq_dedx;   //!
   TBranch        *b_pip_dedx_cdc;   //!
   TBranch        *b_pip_dedx_fdc;   //!
   TBranch        *b_pip_dedx_tof;   //!
   TBranch        *b_pip_dedx_st;   //!
   TBranch        *b_pip_ebcal;   //!
   TBranch        *b_pip_eprebcal;   //!
   TBranch        *b_pip_efcal;   //!
   TBranch        *b_pip_bcal_delphi;   //!
   TBranch        *b_pip_bcal_delz;   //!
   TBranch        *b_pip_fcal_doca;   //!
   TBranch        *b_pim_trkid;   //!
   TBranch        *b_pim_x4_meas;   //!
   TBranch        *b_pim_p4_meas;   //!
   TBranch        *b_pim_x4_kin;   //!
   TBranch        *b_pim_p4_kin;   //!
   TBranch        *b_pim_true_fom;   //!
   TBranch        *b_pim_x4_true;   //!
   TBranch        *b_pim_p4_true;   //!
   TBranch        *b_pim_beta_time;   //!
   TBranch        *b_pim_chisq_time;   //!
   TBranch        *b_pim_ndf_time;   //!
   TBranch        *b_pim_ndf_trk;   //!
   TBranch        *b_pim_chisq_trk;   //!
   TBranch        *b_pim_ndf_dedx;   //!
   TBranch        *b_pim_chisq_dedx;   //!
   TBranch        *b_pim_dedx_cdc;   //!
   TBranch        *b_pim_dedx_fdc;   //!
   TBranch        *b_pim_dedx_tof;   //!
   TBranch        *b_pim_dedx_st;   //!
   TBranch        *b_pim_ebcal;   //!
   TBranch        *b_pim_eprebcal;   //!
   TBranch        *b_pim_efcal;   //!
   TBranch        *b_pim_bcal_delphi;   //!
   TBranch        *b_pim_bcal_delz;   //!
   TBranch        *b_pim_fcal_doca;   //!
   TBranch        *b_misspb_p4_kin;   //!
   TBranch        *b_AccWeight;   //!

   MakeAmpToolsFlat(TTree *tree=0);
   virtual ~MakeAmpToolsFlat();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(Int_t foption);
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
   TTree *m_OutTreeInTime;
   TFile *outFileInTime;
   TTree *m_OutTreeOutTime;
   TFile *outFileOutTime;
};

#endif

#ifdef MakeAmpToolsFlat_cxx
MakeAmpToolsFlat::MakeAmpToolsFlat(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree. 

      if (_file0) {
	tree = (TTree *) _file0->Get("pippimmisspb208_TreeFlat");    // require  input file if provided!
      }
  if (tree == 0) {
    cout << "*** Input file has no pippimmisspb208_TreeFlat tree" << endl;
    /*TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("treeFlat_DSelector_Z2pi_trees_sw1pw1000_TGT_signal_100000.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("treeFlat_DSelector_Z2pi_trees_sw1pw1000_TGT_signal_100000.root");
      }
      f->GetObject("pippimmisspb208_TreeFlat",tree);*/

      }
   Init(tree);
}

MakeAmpToolsFlat::~MakeAmpToolsFlat()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t MakeAmpToolsFlat::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t MakeAmpToolsFlat::LoadTree(Long64_t entry)
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

void MakeAmpToolsFlat::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   beam_x4_meas = 0;
   beam_p4_meas = 0;
   beam_x4_kin = 0;
   beam_p4_kin = 0;
   beam_x4_true = 0;
   beam_p4_true = 0;
   pip_x4_meas = 0;
   pip_p4_meas = 0;
   pip_x4_kin = 0;
   pip_p4_kin = 0;
   pip_x4_true = 0;
   pip_p4_true = 0;
   pim_x4_meas = 0;
   pim_p4_meas = 0;
   pim_x4_kin = 0;
   pim_p4_kin = 0;
   pim_x4_true = 0;
   pim_p4_true = 0;
   misspb_p4_kin = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("weight", &weight, &b_weight);
   fChain->SetBranchAddress("numtruepid_final", &numtruepid_final, &b_numtruepid_final);
   fChain->SetBranchAddress("truepids_decay", &truepids_decay, &b_truepids_decay);
   fChain->SetBranchAddress("is_truetop", &is_truetop, &b_is_truetop);
   fChain->SetBranchAddress("is_truecombo", &is_truecombo, &b_is_truecombo);
   fChain->SetBranchAddress("is_bdtcombo", &is_bdtcombo, &b_is_bdtcombo);
   fChain->SetBranchAddress("rftime", &rftime, &b_rftime);
   fChain->SetBranchAddress("kin_chisq", &kin_chisq, &b_kin_chisq);
   fChain->SetBranchAddress("kin_ndf", &kin_ndf, &b_kin_ndf);
   fChain->SetBranchAddress("beam_beamid", &beam_beamid, &b_beam_beamid);
   fChain->SetBranchAddress("beam_isgen", &beam_isgen, &b_beam_isgen);
   fChain->SetBranchAddress("beam_x4_meas", &beam_x4_meas, &b_beam_x4_meas);
   fChain->SetBranchAddress("beam_p4_meas", &beam_p4_meas, &b_beam_p4_meas);
   fChain->SetBranchAddress("beam_x4_kin", &beam_x4_kin, &b_beam_x4_kin);
   fChain->SetBranchAddress("beam_p4_kin", &beam_p4_kin, &b_beam_p4_kin);
   fChain->SetBranchAddress("beam_x4_true", &beam_x4_true, &b_beam_x4_true);
   fChain->SetBranchAddress("beam_p4_true", &beam_p4_true, &b_beam_p4_true);
   fChain->SetBranchAddress("pip_trkid", &pip_trkid, &b_pip_trkid);
   fChain->SetBranchAddress("pip_x4_meas", &pip_x4_meas, &b_pip_x4_meas);
   fChain->SetBranchAddress("pip_p4_meas", &pip_p4_meas, &b_pip_p4_meas);
   fChain->SetBranchAddress("pip_x4_kin", &pip_x4_kin, &b_pip_x4_kin);
   fChain->SetBranchAddress("pip_p4_kin", &pip_p4_kin, &b_pip_p4_kin);
   fChain->SetBranchAddress("pip_true_fom", &pip_true_fom, &b_pip_true_fom);
   fChain->SetBranchAddress("pip_x4_true", &pip_x4_true, &b_pip_x4_true);
   fChain->SetBranchAddress("pip_p4_true", &pip_p4_true, &b_pip_p4_true);
   fChain->SetBranchAddress("pip_beta_time", &pip_beta_time, &b_pip_beta_time);
   fChain->SetBranchAddress("pip_chisq_time", &pip_chisq_time, &b_pip_chisq_time);
   fChain->SetBranchAddress("pip_ndf_time", &pip_ndf_time, &b_pip_ndf_time);
   fChain->SetBranchAddress("pip_ndf_trk", &pip_ndf_trk, &b_pip_ndf_trk);
   fChain->SetBranchAddress("pip_chisq_trk", &pip_chisq_trk, &b_pip_chisq_trk);
   fChain->SetBranchAddress("pip_ndf_dedx", &pip_ndf_dedx, &b_pip_ndf_dedx);
   fChain->SetBranchAddress("pip_chisq_dedx", &pip_chisq_dedx, &b_pip_chisq_dedx);
   fChain->SetBranchAddress("pip_dedx_cdc", &pip_dedx_cdc, &b_pip_dedx_cdc);
   fChain->SetBranchAddress("pip_dedx_fdc", &pip_dedx_fdc, &b_pip_dedx_fdc);
   fChain->SetBranchAddress("pip_dedx_tof", &pip_dedx_tof, &b_pip_dedx_tof);
   fChain->SetBranchAddress("pip_dedx_st", &pip_dedx_st, &b_pip_dedx_st);
   fChain->SetBranchAddress("pip_ebcal", &pip_ebcal, &b_pip_ebcal);
   fChain->SetBranchAddress("pip_eprebcal", &pip_eprebcal, &b_pip_eprebcal);
   fChain->SetBranchAddress("pip_efcal", &pip_efcal, &b_pip_efcal);
   fChain->SetBranchAddress("pip_bcal_delphi", &pip_bcal_delphi, &b_pip_bcal_delphi);
   fChain->SetBranchAddress("pip_bcal_delz", &pip_bcal_delz, &b_pip_bcal_delz);
   fChain->SetBranchAddress("pip_fcal_doca", &pip_fcal_doca, &b_pip_fcal_doca);
   fChain->SetBranchAddress("pim_trkid", &pim_trkid, &b_pim_trkid);
   fChain->SetBranchAddress("pim_x4_meas", &pim_x4_meas, &b_pim_x4_meas);
   fChain->SetBranchAddress("pim_p4_meas", &pim_p4_meas, &b_pim_p4_meas);
   fChain->SetBranchAddress("pim_x4_kin", &pim_x4_kin, &b_pim_x4_kin);
   fChain->SetBranchAddress("pim_p4_kin", &pim_p4_kin, &b_pim_p4_kin);
   fChain->SetBranchAddress("pim_true_fom", &pim_true_fom, &b_pim_true_fom);
   fChain->SetBranchAddress("pim_x4_true", &pim_x4_true, &b_pim_x4_true);
   fChain->SetBranchAddress("pim_p4_true", &pim_p4_true, &b_pim_p4_true);
   fChain->SetBranchAddress("pim_beta_time", &pim_beta_time, &b_pim_beta_time);
   fChain->SetBranchAddress("pim_chisq_time", &pim_chisq_time, &b_pim_chisq_time);
   fChain->SetBranchAddress("pim_ndf_time", &pim_ndf_time, &b_pim_ndf_time);
   fChain->SetBranchAddress("pim_ndf_trk", &pim_ndf_trk, &b_pim_ndf_trk);
   fChain->SetBranchAddress("pim_chisq_trk", &pim_chisq_trk, &b_pim_chisq_trk);
   fChain->SetBranchAddress("pim_ndf_dedx", &pim_ndf_dedx, &b_pim_ndf_dedx);
   fChain->SetBranchAddress("pim_chisq_dedx", &pim_chisq_dedx, &b_pim_chisq_dedx);
   fChain->SetBranchAddress("pim_dedx_cdc", &pim_dedx_cdc, &b_pim_dedx_cdc);
   fChain->SetBranchAddress("pim_dedx_fdc", &pim_dedx_fdc, &b_pim_dedx_fdc);
   fChain->SetBranchAddress("pim_dedx_tof", &pim_dedx_tof, &b_pim_dedx_tof);
   fChain->SetBranchAddress("pim_dedx_st", &pim_dedx_st, &b_pim_dedx_st);
   fChain->SetBranchAddress("pim_ebcal", &pim_ebcal, &b_pim_ebcal);
   fChain->SetBranchAddress("pim_eprebcal", &pim_eprebcal, &b_pim_eprebcal);
   fChain->SetBranchAddress("pim_efcal", &pim_efcal, &b_pim_efcal);
   fChain->SetBranchAddress("pim_bcal_delphi", &pim_bcal_delphi, &b_pim_bcal_delphi);
   fChain->SetBranchAddress("pim_bcal_delz", &pim_bcal_delz, &b_pim_bcal_delz);
   fChain->SetBranchAddress("pim_fcal_doca", &pim_fcal_doca, &b_pim_fcal_doca);
   fChain->SetBranchAddress("misspb_p4_kin", &misspb_p4_kin, &b_misspb_p4_kin);
   fChain->SetBranchAddress("AccWeight", &AccWeight, &b_AccWeight);
   Notify();
}

Bool_t MakeAmpToolsFlat::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void MakeAmpToolsFlat::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t MakeAmpToolsFlat::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef MakeAmpToolsFlat_cxx
