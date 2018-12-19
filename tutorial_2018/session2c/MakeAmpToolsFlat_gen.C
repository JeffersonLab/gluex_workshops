#define MakeAmpToolsFlat_gen_cxx
#include "MakeAmpToolsFlat_gen.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void MakeAmpToolsFlat_gen::Loop()
{
  // This file was created in the following way
  //  root> TFile *f = TFile::Open("treeFlat_DSelector_Z2pi_trees.root")
  //  root> TTree *t = (TTree *) f->Get("pippimmisspb208_TreeFlat")
  //  root> t->MakeClass("MakeAmpToolsFlat_gen")
  //
//   In a ROOT session, you can do:
//      root> .L MakeAmpToolsFlat_gen.C
//      root> MakeAmpToolsFlat_gen t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   // Initialize output Tree

   outFile = new TFile("AmpToolsInputTree.root", "RECREATE");
   m_OutTree = new TTree("kin", "kin2");

   static size_t locNumFinalStateParticles = 3;

   m_OutTree->Branch("Weight", new float, "Weight/F");
   m_OutTree->Branch("E_Beam", new float, "E_Beam/F");
   m_OutTree->Branch("Px_Beam", new float, "Px_Beam/F");
   m_OutTree->Branch("Py_Beam", new float, "Py_Beam/F");
   m_OutTree->Branch("Pz_Beam", new float, "Pz_Beam/F");
   m_OutTree->Branch("Target_Mass", new float, "Target_Mass/F");
   m_OutTree->Branch("NumFinalState", new int, "NumFinalState/I");
   m_OutTree->Branch("PID_FinalState", new int[locNumFinalStateParticles], "PID_FinalState[NumFinalState]/I");
   m_OutTree->Branch("E_FinalState", new float[locNumFinalStateParticles], "E_FinalState[NumFinalState]/F");
   m_OutTree->Branch("Px_FinalState", new float[locNumFinalStateParticles], "Px_FinalState[NumFinalState]/F");
   m_OutTree->Branch("Py_FinalState", new float[locNumFinalStateParticles], "Py_FinalState[NumFinalState]/F");
   m_OutTree->Branch("Pz_FinalState", new float[locNumFinalStateParticles], "Pz_FinalState[NumFinalState]/F");
 
   m_OutTree->SetBranchAddress("NumFinalState", &m_nPart);
   m_nPart = 3;

   m_OutTree->SetBranchAddress("Target_Mass", &m_TargetMass);
   m_TargetMass = 208*0.931494;          // Pb mass in GeV.

   m_OutTree->SetBranchAddress("PID_FinalState", m_PID);
   m_PID[0] = 8; m_PID[1] = 9; m_PID[2] = 111;

   m_OutTree->SetBranchAddress("E_FinalState", m_e);
   m_OutTree->SetBranchAddress("Px_FinalState", m_px);
   m_OutTree->SetBranchAddress("Py_FinalState", m_py);
   m_OutTree->SetBranchAddress("Pz_FinalState", m_pz);
   m_OutTree->SetBranchAddress("E_Beam", &m_eBeam);
   m_OutTree->SetBranchAddress("Px_Beam", &m_pxBeam);
   m_OutTree->SetBranchAddress("Py_Beam", &m_pyBeam);
   m_OutTree->SetBranchAddress("Pz_Beam", &m_pzBeam);
   m_OutTree->SetBranchAddress("Weight", &m_weight);


  // Process entries in Tree


   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
        
      /*cout<< "jentry=" << jentry << " Beam x=" << Px_Beam << " Beam y=" <<  Py_Beam << " pz=" << Pz_Beam  << " Beam E=" << E_Beam << endl;

      cout<< "jentry=" << jentry << " pi+ px=" << Px_FinalState[0] << " py=" <<  Px_FinalState[0] << " pz=" <<  Px_FinalState[0] << " E=" << E_FinalState[0] << endl;
      cout<< "jentry=" << jentry << " pi- px=" << Px_FinalState[1] << " py=" <<  Px_FinalState[1] << " pz=" <<  Px_FinalState[1] << " E=" << E_FinalState[1] << endl;
      cout<< "jentry=" << jentry << " misspb px=" << Px_FinalState[2] << " py=" <<  Px_FinalState[2] << " pz=" <<  Px_FinalState[2] << " E=" << E_FinalState[2] << endl << endl;*/

   m_e[0] = E_FinalState[0];
   m_px[0] = Px_FinalState[0];
   m_py[0] = Py_FinalState[0];
   m_pz[0] = Pz_FinalState[0];
   m_e[1] = E_FinalState[1];
   m_px[1] = Px_FinalState[1];
   m_py[1] = Py_FinalState[1];
   m_pz[1] = Pz_FinalState[1];
   m_e[2] = E_FinalState[2];
   m_px[2] = Px_FinalState[2];
   m_py[2] = Py_FinalState[2];
   m_pz[2] = Pz_FinalState[2];
   m_eBeam = E_Beam;
   m_pxBeam = Px_Beam;
   m_pyBeam = Py_Beam;
   m_pzBeam = Pz_Beam;
   m_weight = 1;

   m_OutTree->Fill();

   }

   // write out tree

   cout << "Completed loop: nbytes =" << nbytes << " nentries=" << nentries << endl;
   m_OutTree->Write();
   outFile->Close();
}
