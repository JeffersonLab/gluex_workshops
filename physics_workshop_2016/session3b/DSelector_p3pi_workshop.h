#ifndef DSelector_p3pi_workshop_h
#define DSelector_p3pi_workshop_h

#include <iostream>

#include "DSelector/DSelector.h"
#include "DSelector/DHistogramActions.h"
#include "DSelector/DCutActions.h"

#include "TH1I.h"
#include "TH2I.h"
#include "TF1.h"

#include "TLorentzRotation.h"

class DSelector_p3pi_workshop : public DSelector
{
	public:

		DSelector_p3pi_workshop(TTree* locTree = NULL) : DSelector(locTree){}
		virtual ~DSelector_p3pi_workshop(){}

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
		DChargedTrackHypothesis* dPiPlusWrapper;
		DChargedTrackHypothesis* dPiMinusWrapper;

		//Step 2
		DParticleComboStep* dStep2Wrapper;
		DKinematicData* dDecayingPi0Wrapper;
		DNeutralParticleHypothesis* dPhoton1Wrapper;
		DNeutralParticleHypothesis* dPhoton2Wrapper;

		// DEFINE YOUR HISTOGRAM ACTIONS HERE
		// EXAMPLES:
		DHistogramAction_ParticleComboKinematics* dHistComboKinematics;
		DHistogramAction_ParticleID* dHistComboPID, *dHistComboPID_KinFit;

		// DEFINE YOUR CUT ACTIONS HERE
		// EXAMPLES:
		DCutAction_PIDDeltaT* dCutPIDDeltaT;

		// DEFINE YOUR HISTOGRAMS HERE
		// EXAMPLES:
		TH1I* dHist_KinFitChiSq, *dHist_KinFitCL;
		TH1I* dHist_MissingMassSquared;
		TH1I* dHist_BeamEnergy;
		TH1I* dHist_M3pi, *dHist_t;
		TH2I* dHist_Proton_dEdx_P;
		TH2I* dHist_CosTheta_Psi;

		// DEFINE CUT PARAMETERS HERE
		TF1 *fMinProton_dEdx;
		TF1 *fMaxPion_dEdx;
		double dMinKinFitCL, dMaxKinFitChiSq, dMinBeamEnergy, dMaxBeamEnergy, dMinOmegaMass, dMaxOmegaMass;

	ClassDef(DSelector_p3pi_workshop, 0);
};

void DSelector_p3pi_workshop::Get_ComboWrappers(void)
{
	//Step 0
	dStep0Wrapper = dComboWrapper->Get_ParticleComboStep(0);
	dComboBeamWrapper = static_cast<DBeamParticle*>(dStep0Wrapper->Get_InitialParticle());
	dProtonWrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(1));

	//Step 1
	dStep1Wrapper = dComboWrapper->Get_ParticleComboStep(1);
	dPiPlusWrapper = static_cast<DChargedTrackHypothesis*>(dStep1Wrapper->Get_FinalParticle(0));
	dPiMinusWrapper = static_cast<DChargedTrackHypothesis*>(dStep1Wrapper->Get_FinalParticle(1));

	//Step 2
	dStep2Wrapper = dComboWrapper->Get_ParticleComboStep(2);
	dDecayingPi0Wrapper = dStep2Wrapper->Get_InitialParticle();
	dPhoton1Wrapper = static_cast<DNeutralParticleHypothesis*>(dStep2Wrapper->Get_FinalParticle(0));
	dPhoton2Wrapper = static_cast<DNeutralParticleHypothesis*>(dStep2Wrapper->Get_FinalParticle(1));
}

#endif // DSelector_p3pi_workshop_h
