#ifndef DSelector_Z2pi_trees2_h
#define DSelector_Z2pi_trees2_h

#include <iostream>

#include "DSelector/DSelector.h"
#include "DSelector/DHistogramActions.h"
#include "DSelector/DCutActions.h"

#include "TH1I.h"
#include "TH2I.h"
#include "TLorentzVector.h"
#include "TLorentzRotation.h"

class DSelector_Z2pi_trees2 : public DSelector
{
	public:

		DSelector_Z2pi_trees2(TTree* locTree = NULL) : DSelector(locTree){}
		virtual ~DSelector_Z2pi_trees2(){}

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
		DKinematicData* dMissingPb208Wrapper;

		// DEFINE YOUR HISTOGRAMS HERE
		// EXAMPLES:
		TH1I* dHist_MissingMassSquared;
		TH1I* dHist_BeamEnergy;
		TH1I* dHist_pMomentumMeasured;
		TH1I* dHist_piplusMomentumMeasured;
		TH1I* dHist_piminusMomentumMeasured;
		TH2I* dHist_Proton_dEdx_P;
		TH1I* dHist_KinFitChiSq;
		TH1I* dHist_KinFitCL;
		TH1I* dHist_M2pigen;
		TH1I* dHist_M2pikin;
		TH1I* dHist_M2pidiff;
		TH1I* dHist_tgen;
		TH1I* dHist_tkin;
		TH1I* dHist_tdiff;
		TH2I* dHist_tkin_tgen;
		TH1I* dHist_CosTheta;
		TH1I* dHist_CosThetadiff;
		TH2I* dHist_CosTheta_psi;
		TH2I* dHist_CosThetakin_CosThetagen;
		TH2I* dHist_phimeas_phigen;
		TH2I* dHist_phikin_phigen;
		TH2I* dHist_Phimeas_Phigen;
		TH2I* dHist_Phikin_Phigen;
		TH2I* dHist_phikin_Phikin;
		TH2I* dHist_phigen_Phigen;
		TH2I* dHist_Delta_phi;
		TH2I* dHist_Delta_Phi;
		TH2I* dHist_Delta_phimeas;
		TH2I* dHist_Delta_Phimeas;


		TH1I* dHist_Phigen;
		TH1I* dHist_phigen;
		TH1I* dHist_Phikin;
		TH1I* dHist_phikin;
		TH1I* dHist_Phimeas;
		TH1I* dHist_phimeas;
		TH1I* dHist_psigen;
		TH1I* dHist_psidiff;
		TH1I* dHist_Phidiff;
		TH1I* dHist_phidiff;
		TH1I* dHist_psikin;
		TH1I* dHist_pDeltap;
		TH1I* dHist_pipDeltap;
		TH1I* dHist_pimDeltap;
		TH1I* dHist_pDeltap_Measured;
		TH1I* dHist_pipDeltap_Measured;
		TH1I* dHist_pimDeltap_Measured;
		TH1I* dHist_TaggerAccidentals;

		// Cut parameters
		TF1* fMinProton_dEdx;
		TF1* fMaxPion_dEdx;
		Double_t dMinKinFitCL;
		Double_t dMaxKinFitChiSq;
		Double_t dMinBeamEnergy;
		Double_t dMaxBeamEnergy;
		Double_t dMin2piMass;
		Double_t dMax2piMass;
		Double_t dMinMissingMassSquared;
		Double_t dMaxMissingMassSquared;
		
		Double_t AccWeight;    // used to store weights due to accidental tagger subtraction

	ClassDef(DSelector_Z2pi_trees2, 0);
};

void DSelector_Z2pi_trees2::Get_ComboWrappers(void)
{
	//Step 0
	dStep0Wrapper = dComboWrapper->Get_ParticleComboStep(0);
	dComboBeamWrapper = static_cast<DBeamParticle*>(dStep0Wrapper->Get_InitialParticle());
	dPiPlusWrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(0));
	dPiMinusWrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(1));
	dMissingPb208Wrapper = dStep0Wrapper->Get_FinalParticle(2);
}

#endif // DSelector_Z2pi_trees2_h
