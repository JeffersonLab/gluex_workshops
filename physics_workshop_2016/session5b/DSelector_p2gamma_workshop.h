#ifndef DSelector_p2gamma_workshop_h
#define DSelector_p2gamma_workshop_h

#include <iostream>

#include "DSelector/DSelector.h"
#include "DSelector/DHistogramActions.h"
#include "DSelector/DCutActions.h"

#include "TH1I.h"
#include "TH2I.h"
#include "TF1.h"
#include "TRandom3.h"

class DSelector_p2gamma_workshop : public DSelector
{
	public:

		DSelector_p2gamma_workshop(TTree* locTree = NULL) : DSelector(locTree){}
		virtual ~DSelector_p2gamma_workshop(){}

		void Init(TTree *tree);
		Bool_t Process(Long64_t entry);

	private:

		void Get_ComboWrappers(void);
		void Finalize(void);

		//CREATE REACTION-SPECIFIC PARTICLE ARRAYS

		//Step 0
		DParticleComboStep* dStep0Wrapper;
		DBeamParticle* dComboBeamWrapper;
		DChargedTrackHypothesis* dProtonWrapper;

		//Step 1
		DParticleComboStep* dStep1Wrapper;
		DKinematicData* dDecayingPi0Wrapper;
		DNeutralParticleHypothesis* dPhoton1Wrapper;
		DNeutralParticleHypothesis* dPhoton2Wrapper;

		// DEFINE YOUR HISTOGRAM ACTIONS HERE
		// EXAMPLE HISTOGRAM ACTIONS:
		DHistogramAction_ParticleComboKinematics* dHistComboKinematics;
		DHistogramAction_ParticleID* dHistComboPID;
		// EXAMPLE CUT ACTIONS:
		DCutAction_PIDDeltaT* dCutPIDDeltaT;

		// DEFINE YOUR HISTOGRAMS HERE
		// EXAMPLES:
		TH1I* dHist_MissingMassSquared;
		TH1I* dHist_BeamEnergy;
		TH1I* dHist_M2gamma;
		TH2I* dHist_Proton_dEdx_P;
                TH2I* dHist_phi_t, *dHist_acc_phi_t;

		// DEFINE CUT PARAMETERS HERE
                TF1 *fMinProton_dEdx;
                double dMinKinFitCL, dMinBeamEnergy, dMaxBeamEnergy, dMinPi0Mass, dMaxPi0Mass;

	ClassDef(DSelector_p2gamma_workshop, 0);
};

void DSelector_p2gamma_workshop::Get_ComboWrappers(void)
{
	//Step 0
	dStep0Wrapper = dComboWrapper->Get_ParticleComboStep(0);
	dComboBeamWrapper = static_cast<DBeamParticle*>(dStep0Wrapper->Get_InitialParticle());
	dProtonWrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(1));

	//Step 1
	dStep1Wrapper = dComboWrapper->Get_ParticleComboStep(1);
	dDecayingPi0Wrapper = dStep1Wrapper->Get_InitialParticle();
	dPhoton1Wrapper = static_cast<DNeutralParticleHypothesis*>(dStep1Wrapper->Get_FinalParticle(0));
	dPhoton2Wrapper = static_cast<DNeutralParticleHypothesis*>(dStep1Wrapper->Get_FinalParticle(1));
}

#endif // DSelector_p2gamma_workshop_h
