#define MakeAmpToolsFlat_cxx
#include "MakeAmpToolsFlat.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

Int_t foption;    // select which file to write


void MakeAmpToolsFlat::Loop(Int_t foption)
{
  // This file was created in the following way
  //  root> TFile *f = TFile::Open("treeFlat_DSelector_Z2pi_trees.root")
  //  root> TTree *t = (TTree *) f->Get("pippimmisspb208_TreeFlat")
  //  root> t->MakeClass("MakeAmpToolsFlat_gen")
  //
//   In a ROOT session, you can do:
//      root> .L MakeAmpToolsFlat.C
//      root> MakeAmpToolsFlat t
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


   // Define diagnostic histograms
   TH1D *h1_twopimass_intime = new TH1D ("h1_twopimass_intime","Two pi mass intime (w=1)",200,0.2,0.8);
   TH1D *h1_twopimass_outtime = new TH1D ("h1_twopimass_outtime","Two pi mass outtime (w=1)",200,0.2,0.8);
   TH1D *h1_twopimass_total = new TH1D ("h1_twopimass_total","Two pi mass total (w=1)",200,0.2,0.8);
   TH1D *h1_twopimass_signal = new TH1D ("h1_twopimass_signal","Two pi mass (weighted)",200,0.2,0.8);
   TH1D *h1_twopimass_signalpos = new TH1D ("h1_twopimass_signalpos","Two pi mass (positive weighted)",200,0.2,0.8);

   // Three output Trees, depending on foption
   // foption=1: First output has weights (positive and negative)
   // foption=2: Second output has in-time only (weight=1)
   // foption=3: Third output has out-time only (weight=negative 1/nbunches)
   // foption=4: Third output has in-time only (weight=1 - out-time*(1/nbunches)/in-time)

   if (foption == 1){ 
     outFile = new TFile("AmpToolsInputTree.root", "RECREATE");
   }
   else if (foption == 2) { 
     outFile = new TFile("AmpToolsInputTreeInTime.root", "RECREATE");
     }
   else if (foption == 3) {
     outFile = new TFile("AmpToolsInputTreeOutTime.root", "RECREATE");
     }
   else if (foption == 4) {
     outFile = new TFile("AmpToolsInputTreeInTimeW.root", "RECREATE");
     }
   else {
     cout << "*** Loop, illegal foption=" << foption << endl;
   }
    
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

   int NumIntime = 0;
   int NumOuttime = 0;

   // loop over entries to count ratio of NumOuttime/NumIntime

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);
      if (AccWeight > 0) NumIntime++; 
      if (AccWeight < 0) NumOuttime++;
   }

   int Nbunches=4;
   double signal_frac = (NumIntime - (1./Nbunches)*NumOuttime) / NumIntime;
   cout << "Signal_frac=" << signal_frac << endl;


   nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry); 
      /*nb = b_pip_p4_kin->GetEntry(jentry);
      nbytes += nb; 
      nb = b_pim_p4_kin->GetEntry(jentry);
      nbytes += nb; 
      nb = b_misspb_p4_kin->GetEntry(jentry);
      nbytes += nb; 
      nb = b_beam_p4_kin->GetEntry(jentry);
      nbytes += nb;
      nb = b_AccWeight->GetEntry(jentry);*/
      nbytes += nb;
      // if (Cut(ientry) < 0) continue;
        
      /*cout<< "jentry=" << jentry << " px=" << beam_p4_kin->Px()<< " py=" << beam_p4_kin->Py()<< " pz=" << beam_p4_kin->Pz()<< " E=" << beam_p4_kin->E()<< endl; 
      cout<< "jentry=" << jentry << " weight=" << AccWeight << endl; 
      cout<< "jentry=" << jentry << " px=" << pip_p4_kin->Px()<< " py=" << pip_p4_kin->Py()<< " pz=" << pip_p4_kin->Pz()<< " E=" << pip_p4_kin->E()<< endl; 
      cout<< "jentry=" << jentry << " px=" << pim_p4_kin->Px()<< " py=" << pim_p4_kin->Py()<< " pz=" << pim_p4_kin->Pz()<< " E=" << pim_p4_kin->E()<< endl; 
      cout<< "jentry=" << jentry << " px=" << misspb_p4_kin->Px()<< " py=" << misspb_p4_kin->Py()<< " pz=" << misspb_p4_kin->Pz()<< " E=" << misspb_p4_kin->E()<< endl << endl;*/

   m_e[0] = pip_p4_kin->E();
   m_px[0] = pip_p4_kin->Px();
   m_py[0] = pip_p4_kin->Py();
   m_pz[0] = pip_p4_kin->Pz();
   m_e[1] = pim_p4_kin->E();
   m_px[1] = pim_p4_kin->Px();
   m_py[1] = pim_p4_kin->Py();
   m_pz[1] = pim_p4_kin->Pz();
   m_e[2] = misspb_p4_kin->E();
   m_px[2] = misspb_p4_kin->Px();
   m_py[2] = misspb_p4_kin->Py();
   m_pz[2] = misspb_p4_kin->Pz();
   m_eBeam = beam_p4_kin->E();
   m_pxBeam = beam_p4_kin->Px();
   m_pyBeam = beam_p4_kin->Py();
   m_pzBeam = beam_p4_kin->Pz();
   m_weight = AccWeight;
   // m_weight = abs(AccWeight);

   TLorentzVector twopi;
   twopi = *pip_p4_kin + *pim_p4_kin;
   Double_t twopimass = twopi.M(); 

   if (foption == 1) {
     if (AccWeight > 0) h1_twopimass_intime->Fill(twopimass);
     if (AccWeight < 0) h1_twopimass_outtime->Fill(twopimass);
     h1_twopimass_total->Fill(twopimass);
     h1_twopimass_signal->Fill(twopimass,AccWeight);
     h1_twopimass_signalpos->Fill(twopimass,abs(AccWeight));
     m_OutTree->Fill();
   }
   else if (foption == 2) {
     if (AccWeight > 0) m_OutTree->Fill();   // in time
   }
   else if (foption == 3) {
     if (AccWeight < 0) {
       m_weight = abs(AccWeight);            // make all weights positve
       m_OutTree->Fill();   // out of time
     }
   }
   else if (foption == 4) {
     if (AccWeight > 0) {
       m_weight = signal_frac;
       m_OutTree->Fill();   // in time, weight is fraction of signal
     }
   }
   else {
     cout << " *** Loop, illegal foption=" << foption << endl;
     exit (1);
   }
   }

   // write out tree

   cout << "Completed loop: foption=" << foption << " nbytes =" << nbytes << " nentries=" << nentries << endl;
   m_OutTree->Write();
   outFile->Close();

   if (foption == 1) {
   TFile *histFile = new TFile("AmpToolsDiagnosticHist.root", "RECREATE");
   h1_twopimass_intime->Write();
   h1_twopimass_outtime->Write();
   h1_twopimass_total->Write();
   h1_twopimass_signal->Write();
   h1_twopimass_signalpos->Write();
   histFile->Close();
   }

}
