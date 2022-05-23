#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include "TLatex.h"
#include "TPaveStats.h"
#include "TGraphPainter.h"
#include "TString.h"
#include "TCollection.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2I.h"
#include "TF1.h"
#include "TMath.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TMinuit.h"
#include "TKey.h"
#include "TCut.h"
#include "TDatime.h"
#include "TAxis.h"
#include "TLine.h"
#include "TTree.h"
#include "TBranch.h"
#include "TStyle.h"
#include "TProfile.h"
#include "TSystem.h"
#include "TDirectory.h"
#include "TString.h"
#include "TLorentzVector.h"
#include "TLorentzRotation.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"

using namespace std;



void FlattenThrownExample(TString str_infile, TString str_outfile_flat, Int_t max_events=1e9, Double_t min_z=52., Double_t max_z=78.) {

	// Input following Thrown_Tree formatting
	TFile* f_in = new TFile(str_infile);
	f_in->cd();
	TTree* t_in = (TTree*)f_in->Get("Thrown_Tree");
	Int_t MaxArraySize = 15; // Required for array quantites
	UInt_t RunNumber=0;                    t_in->SetBranchAddress("RunNumber",&RunNumber);
	ULong64_t EventNumber=0;               t_in->SetBranchAddress("EventNumber",&EventNumber);
	UInt_t NumThrown=0;                    t_in->SetBranchAddress("NumThrown",&NumThrown);
	Int_t Thrown__PID[MaxArraySize];       t_in->SetBranchAddress("Thrown__PID",Thrown__PID);
	TLorentzVector* ThrownBeam__P4=NULL;   t_in->SetBranchAddress("ThrownBeam__P4",&ThrownBeam__P4);
	TClonesArray*   Thrown__X4=NULL;       t_in->SetBranchAddress("Thrown__X4",&Thrown__X4);
	TClonesArray*   Thrown__P4=NULL;       t_in->SetBranchAddress("Thrown__P4",&Thrown__P4);
	
	Long64_t nentries = t_in->GetEntries();
	cout << "Number of entries: " << nentries << endl;
	
	
	// Output format (flat)
	TFile* f_out = new TFile(str_outfile_flat,"RECREATE");
	TTree* t_out = new TTree("Thrown_Tree_flat","");
	
	ULong64_t EventNumber_out;                   t_out->Branch("EventNumber",&EventNumber_out); // Example output branch: simple data type
	Double_t  pi0eta_mass_out;                   t_out->Branch("pi0eta_mass",&pi0eta_mass_out); // Example output branch: simple data type
	Int_t     MaxArraySize_out = MaxArraySize;   t_out->Branch("MaxArraySize",&MaxArraySize_out,"MaxArraySize/I");
	Double_t  Thrown__E_out[MaxArraySize];       t_out->Branch("Thrown__E",Thrown__E_out,"Thrown__E[MaxArraySize]/D"); // Example output branch: array of simple data type
	Int_t     Thrown__PID_out[MaxArraySize];     t_out->Branch("Thrown__PID",Thrown__PID_out,"Thrown__PID[MaxArraySize]/I"); // Example output branch: array of simple data type
	
	// Each entry = 1 event
 	for(Long64_t i =0; i<nentries; ++i) {
		
		t_in->GetEntry(i);
		if(i%100000==0) cout << "Entry: "  << i << endl;
		if(i>=max_events) {cout << "Max events reached. Exiting..." << endl; break;}

			// Get initial 4-vectors
			TLorentzVector thrown_proton_x4   = *(TLorentzVector*)Thrown__X4->At(0);
			TLorentzVector thrown_proton_p4   = *(TLorentzVector*)Thrown__P4->At(0);
			if(thrown_proton_x4.Z()<min_z || thrown_proton_x4.Z()>max_z) continue; // Outsize our target Z selection

		if(NumThrown==7) {
			// Loop over all throwns in event
			TLorentzVector pi0eta_p4 = TLorentzVector(0,0,0,0);
			for(int ii=1;ii<NumThrown;++ii)  {
				TLorentzVector curr_thrown_p4 = *(TLorentzVector*)Thrown__P4->At(ii);
				Thrown__E_out[ii]   = curr_thrown_p4.E();
				Thrown__PID_out[ii] = Thrown__PID[ii];
				if(Thrown__PID[ii]==1) pi0eta_p4+=curr_thrown_p4;
			}
			
			EventNumber_out=EventNumber;
			pi0eta_mass_out=pi0eta_p4.M();
			
			t_out->Fill(); //Fill for the current thrown event
		}

	}
		
	f_out->Write("",4); //Option for int 4: write/delete (avoids issue of second ttree showing up)
	f_out->Close();
		
	
	return;
}