#ifndef DSelector_checkKFit_h
#define DSelector_checkKFit_h

#include <iostream>

#include "DSelector/DSelector.h"
#include "DSelector/DHistogramActions.h"
#include "DSelector/DCutActions.h"
//#include "angularScan_tools.h"

#include "TH1I.h"
#include "TH2I.h"

class DSelector_checkKFit : public DSelector
{
	public:

		DSelector_checkKFit(TTree* locTree = NULL) : DSelector(locTree){}
		virtual ~DSelector_checkKFit(){}

		void Init(TTree *tree);
		Bool_t Process(Long64_t entry);

	private:

		void Get_ComboWrappers(void);
		void Finalize(void);

		// BEAM POLARIZATION INFORMATION
		UInt_t dPreviousRunNumber;
		bool dIsPolarizedFlag; //else is AMO
		bool dIsPARAFlag; //else is PERP or AMO

		//CREATE REACTION-SPECIFIC PARTICLE ARRAYS

		//Step 0
		DParticleComboStep* dStep0Wrapper;
		DBeamParticle* dComboBeamWrapper;
		DChargedTrackHypothesis* dPiPlusWrapper;
		DChargedTrackHypothesis* dPiMinusWrapper;
		DChargedTrackHypothesis* dProtonWrapper;

		// DEFINE YOUR HISTOGRAMS HERE
		// EXAMPLES:
		TLorentzVector inVector,outVector;
		TLorentzVector mimassVec,imassVec;
		Double_t mimass,imass;
		Double_t mimass_all;
		Double_t missingP,missingE;
		TH2I *missingP_vs_missingE;
		TH1I *EM_balance;
		TH2I *MM_vs_IM;
		
		Float_t kfitProb;
	    TH2I *Pulls_vs_Prob[3][6];
	    
	    TH1I *probDist,*probDistCut,*probDistCut2;
	    TH2I *missingP_vs_missingE_Cut;
	    TH2I* MM_vs_IM_Cut;
	    TH1I *EM_balance_Cut;
	    TH1I *MMALL,*MMALL_Cut;
	    
	     TH2I *missingP_vs_missingE_Cut2;
	    TH2I* MM_vs_IM_Cut2;
	    TH1I *EM_balance_Cut2;
	    TH1I *MMALL_Cut2;
	    
	    TLorentzVector mimassPipPimVec,mimassPipPVec,mimassPimPVec;
	    Double_t mimassPipPim,mimassPipP,mimassPimP;    
	    TH2I *Theta_vs_phi_PiPlus;
	    TH2I *Theta_vs_phi_PiMinus;
	    TH2I *Theta_vs_phi_Prot;
	       
	    TH2I *PiPlus_PullPx_vs_Prob[5][5];
        TH2I *PiPlus_PullPy_vs_Prob[5][5];
        TH2I *PiPlus_PullPz_vs_Prob[5][5];
        TH2I *PiPlus_PullVx_vs_Prob[5][5];
        TH2I *PiPlus_PullVy_vs_Prob[5][5];
        TH2I *PiPlus_PullVz_vs_Prob[5][5];

        TH2I *PiMinus_PullPx_vs_Prob[5][5];
        TH2I *PiMinus_PullPy_vs_Prob[5][5];
        TH2I *PiMinus_PullPz_vs_Prob[5][5];
        TH2I *PiMinus_PullVx_vs_Prob[5][5];
        TH2I *PiMinus_PullVy_vs_Prob[5][5];
        TH2I *PiMinus_PullVz_vs_Prob[5][5];

        TH2I *Prot_PullPx_vs_Prob[5][5];
        TH2I *Prot_PullPy_vs_Prob[5][5]; 
        TH2I *Prot_PullPz_vs_Prob[5][5];
        TH2I *Prot_PullVx_vs_Prob[5][5];
        TH2I *Prot_PullVy_vs_Prob[5][5];
        TH2I *Prot_PullVz_vs_Prob[5][5];
	    

	ClassDef(DSelector_checkKFit, 0);
};

void DSelector_checkKFit::Get_ComboWrappers(void)
{
	//Step 0
	dStep0Wrapper = dComboWrapper->Get_ParticleComboStep(0);
	dComboBeamWrapper = static_cast<DBeamParticle*>(dStep0Wrapper->Get_InitialParticle());
	dPiPlusWrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(0));
	dPiMinusWrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(1));
	dProtonWrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(2));
}

#endif // DSelector_checkKFit_h
